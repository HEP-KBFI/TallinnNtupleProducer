#include "TallinnNtupleProducer/Writers/interface/RecoHadTauWriter.h"

#include "TallinnNtupleProducer/Framework/interface/BranchAddressInitializer.h" // BranchAddressInitializer
#include "TallinnNtupleProducer/Framework/interface/cmsException.h"             // cmsException()
#include "TallinnNtupleProducer/Readers/interface/RecoHadTauReader.h"           // RecoHadTauReader::get_supported_systematics()

#include "TString.h"                                                            // Form()

RecoHadTauWriter::RecoHadTauWriter(const edm::ParameterSet & cfg)
  : max_nHadTaus_(cfg.getParameter<unsigned>("numNominalHadTaus"))
  , branchName_num_("ntau")
  , branchName_obj_("tau")
  , current_central_or_shiftEntry_(nullptr)
{
  max_nHadTaus_ = cfg.getParameter<unsigned>("numNominalHadTaus");
  std::set<std::string> systematic_shifts;
  for ( auto shift : RecoHadTauReader::get_supported_systematics() )
  {
    systematic_shifts.insert(shift);
  }
  // CV: add central value 
  systematic_shifts.insert("central");
  for ( auto central_or_shift : systematic_shifts )
  {    
    central_or_shiftEntry it;
    it.nHadTaus_ = 0;
    it.pt_ = new Float_t[nHadTaus_];
    it.eta_ = new Float_t[nHadTaus_];
    it.phi_ = new Float_t[nHadTaus_];
    it.mass_ = new Float_t[nHadTaus_];
    it.decayMode_ = new Int_t[nHadTaus_];
    it.charge_ = new Int_t[nHadTaus_];
    it.isFakeable_ = new Bool_t[nHadTaus_];
    it.isTight_ = new Bool_t[nHadTaus_];
    it.genMatch_ = new Int_t[nHadTaus_];
    it.isFake_ = new Bool_t[nHadTaus_];
    it.isFlip_ = new Bool_t[nHadTaus_];
  }
}

RecoHadTauWriter::~RecoHadTauWriter()
{
  for ( auto it : central_or_shiftEntries_ )
  {
    delete[] it.pt_;
    delete[] it.eta_;
    delete[] it.phi_;
    delete[] it.mass_;
    delete[] it.decayMode_;
    delete[] it.charge_;
    delete[] it.isFakeable_;
    delete[] it.isTight_;
    delete[] it.genMatch_;
    delete[] it.isFake_;
    delete[] it.isFlip_;
  }
}

namespace
{
  std::string
  get_branchName_num(const std::string & branchName_num, const std::string & central_or_shift)
  {
    if ( central_or_shift == "central" ) return branchName_num;
    else                                 return Form("%s_%s", branchName_num.data(), central_or_shift.data());
  }
  std::string
  get_branchName_obj(const std::string & branchName_obj, int idx, const std::string & suffix, const std::string & central_or_shift)
  {
    if ( central_or_shift == "central" ) return Form("%s%i_%s_%s", branchName_obj.data(), idx, suffix.data());
    else                                 return Form("%s%i_%s_%s", branchName_obj.data(), idx, central_or_shift.data(), suffix.data());
  }
}

void
RecoHadTauWriter::setBranches(TTree * tree)
{
  BranchAddressInitializer bai(tree);
  for ( auto central_or_shift : systematic_shifts )
  {
    auto it = central_or_shiftEntries_.find(central_or_shift);
    assert(it != central_or_shiftEntries_.end());
    bai.setBranch(it->nHadTaus_, get_branchName_num(branchName_num_, central_or_shift));
    for ( size_t idxHadTau = 0; idxHadTau < nHadTaus_; ++idxHadTau )
    {
      bai.setBranch(it->pt_[idxHadTau], get_branchName_obj(branchName_obj_, idxHadTau, "pt", central_or_shift));
      bai.setBranch(it->eta_[idxHadTau], get_branchName_obj(branchName_obj_, idxHadTau, "eta", central_or_shift));
      bai.setBranch(it->phi_[idxHadTau], get_branchName_obj(branchName_obj_, idxHadTau, "phi", central_or_shift));
      bai.setBranch(it->mass_[idxHadTau], get_branchName_obj(branchName_obj_, idxHadTau, "mass", central_or_shift));
      bai.setBranch(it->decayMode_[idxHadTau], get_branchName_obj(branchName_obj_, idxHadTau, "decayMode", central_or_shift));
      bai.setBranch(it->charge_[idxHadTau], get_branchName_obj(branchName_obj_, idxHadTau, "charge", central_or_shift));
      bai.setBranch(it->isFakeable_[idxHadTau], get_branchName_obj(branchName_obj_, idxHadTau, "isFakeable", central_or_shift));
      bai.setBranch(it->isTight_[idxHadTau], get_branchName_obj(branchName_obj_, idxHadTau, "isTight", central_or_shift));    
      bai.setBranch(it->genMatch_[idxHadTau], get_branchName_obj(branchName_obj_, idxHadTau, "genMatch", central_or_shift));
      bai.setBranch(it->isFake_[idxHadTau], get_branchName_obj(branchName_obj_, idxHadTau, "isFake", central_or_shift));  
      bai.setBranch(it->isFlip_[idxHadTau], get_branchName_obj(branchName_obj_, idxHadTau, "isFlip", central_or_shift));  
    }
  }
}

void
RecoHadTauWriter::set_central_or_shift(const std::string & central_or_shift) const
{
  auto it = central_or_shiftEntries_.find(central_or_shift);
  if ( it != central_or_shiftEntries_.end()
  {
    current_central_or_shiftEntry_ = &it->second;
  }
  else throw cmsException(__func__, __LINE__) 
    << "Invalid systematic shift = '" << central_or_shift << "' !!";
}

namespace
{
  unsigned
  compGenMatch(const RecoHadTau * hadTau)
  {
    unsigned genMatch = 0;
    if ( hadTau->genHadTau() )
    {
      if ( hadTau->charge() == hadTau->genHadTau()->charge() ) genMatch = 1;
      else genMatch = 2;
    }
    else if ( hadTau->genLepton() && std::abs(hadTau->genLepton()->pdgId()) == 11 )
    {
      if ( hadTau->charge() == hadTau->genLepton()->charge() ) genMatch = 4;
      else genMatch = 8;
    }
    else if ( hadTau->genLepton() && std::abs(hadTau->genLepton()->pdgId()) == 13 )
    {
      if ( hadTau->charge() == hadTau->genLepton()->charge() ) genMatch = 16;
      else genMatch = 32;
    }
    else
    {
      genMatch = 64;
    }
    return genMatch;
  }
}

void
RecoHadTauWriter::write(const Event & event)
{
  assert(current_central_or_shiftEntry_);
  const RecoHadTauPtrCollection& hadTaus = event.fakeableHadTaus();
  auto it = current_central_or_shiftEntry_;
  it->nHadTaus_ = hadTaus.size();
  for ( size_t idxHadTau = 0; idxHadTau < max_nHadTaus_; ++idxHadTau )
  {
    if ( idxHadTau < it->nHadTaus_ )
    {
      const RecoHadTau * hadTau = hadTaus[idxHadTau];
      it->pt_[idxHadTau] = hadTau->pt();
      it->eta_[idxHadTau] = hadTau->eta();
      it->phi_[idxHadTau] = hadTau->phi();
      it->mass_[idxHadTau] = hadTau->mass();
      it->decayMode_[idxHadTau] = hadTau->decayMode();
      it->charge_[idxHadTau] = hadTau->charge();
      it->isFakeable_[idxHadTau] = hadTau->isFakeable();
      it->isTight_[idxHadTau] = hadTau->isTight();
      it->genMatch_[idxHadTau] = compGenMatch(hadTau);
      it->isFake_[idxHadTau] = it->genMatch_[idxHadTau] == 64;
      it->isFlip_[idxHadTau] = it->genMatch_[idxHadTau] == 2 || it->genMatch_[idxHadTau] == 8 || it->genMatch_[idxHadTau] == 32;
    }
    else
    {
      it->pt_[idxHadTau] = 0.;
      it->eta_[idxHadTau] = 0.;
      it->phi_[idxHadTau] = 0.;
      it->mass_[idxHadTau] = 0.;
      it->decayMode_[idxHadTau] = 0;
      it->charge_[idxHadTau] = 0;
      it->isFakeable_[idxHadTau] = false;
      it->isTight_[idxHadTau] = false;
      it->genMatch_[idxHadTau] = 0;
      it->isFake_[idxHadTau] = false;
      it->isFlip_[idxHadTau] = false;
    }
  }
}

std::vector<std::string>
RecoHadTauWriter::get_supported_systematics()
{
  
}

DEFINE_EDM_PLUGIN(WriterPluginFactory, RecoHadTauWriter, "RecoHadTauWriter");
