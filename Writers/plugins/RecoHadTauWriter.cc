#include "TallinnNtupleProducer/Writers/plugins/RecoHadTauWriter.h"

#include "TallinnNtupleProducer/CommonTools/interface/cmsException.h"         // cmsException()
#include "TallinnNtupleProducer/Objects/interface/GenHadTau.h"                // GenHadTau
#include "TallinnNtupleProducer/Objects/interface/GenLepton.h"                // GenLepton
#include "TallinnNtupleProducer/Readers/interface/BranchAddressInitializer.h" // BranchAddressInitializer
#include "TallinnNtupleProducer/Readers/interface/RecoHadTauReader.h"         // RecoHadTauReader::get_supported_systematics()

#include "TString.h"                                                          // Form()
#include "TTree.h"                                                            // TTree

#include <assert.h>                                                           // assert()

RecoHadTauWriter::RecoHadTauWriter(const edm::ParameterSet & cfg)
  : WriterBase(cfg)
  , max_nHadTaus_(cfg.getParameter<unsigned>("numNominalHadTaus"))
  , branchName_num_("ntau")
  , branchName_obj_("tau")
  , current_central_or_shiftEntry_(nullptr)
{
  max_nHadTaus_ = cfg.getParameter<unsigned>("numNominalHadTaus");
  assert(max_nHadTaus_ >= 1);
  for ( auto shift : RecoHadTauReader::get_supported_systematics() )
  {
    systematic_shifts_.insert(shift);
  }
  // CV: add central value 
  systematic_shifts_.insert("central");
  for ( auto central_or_shift : systematic_shifts_ )
  {    
    central_or_shiftEntry it;
    it.nHadTaus_ = 0;
    it.pt_ = new Float_t[max_nHadTaus_];
    it.eta_ = new Float_t[max_nHadTaus_];
    it.phi_ = new Float_t[max_nHadTaus_];
    it.mass_ = new Float_t[max_nHadTaus_];
    it.decayMode_ = new Int_t[max_nHadTaus_];
    it.charge_ = new Int_t[max_nHadTaus_];
    it.isFakeable_ = new Bool_t[max_nHadTaus_];
    it.isTight_ = new Bool_t[max_nHadTaus_];
    it.genMatch_ = new Int_t[max_nHadTaus_];
    it.isFake_ = new Bool_t[max_nHadTaus_];
    it.isFlip_ = new Bool_t[max_nHadTaus_];
    central_or_shiftEntries_[central_or_shift] = it;
  }
  current_central_or_shiftEntry_ = &central_or_shiftEntries_["central"];
}

RecoHadTauWriter::~RecoHadTauWriter()
{
  for ( auto it : central_or_shiftEntries_ )
  {
    delete[] it.second.pt_;
    delete[] it.second.eta_;
    delete[] it.second.phi_;
    delete[] it.second.mass_;
    delete[] it.second.decayMode_;
    delete[] it.second.charge_;
    delete[] it.second.isFakeable_;
    delete[] it.second.isTight_;
    delete[] it.second.genMatch_;
    delete[] it.second.isFake_;
    delete[] it.second.isFlip_;
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
    if ( central_or_shift == "central" ) return Form("%s%i_%s",    branchName_obj.data(), (int)idx, suffix.data());
    else                                 return Form("%s%i_%s_%s", branchName_obj.data(), (int)idx, central_or_shift.data(), suffix.data());
  }
}

void
RecoHadTauWriter::setBranches(TTree * tree)
{
  BranchAddressInitializer bai(tree);
  for ( auto central_or_shift : systematic_shifts_ )
  {
    auto it = central_or_shiftEntries_.find(central_or_shift);
    assert(it != central_or_shiftEntries_.end());
    bai.setBranch(it->second.nHadTaus_, get_branchName_num(branchName_num_, central_or_shift));
    for ( size_t idxHadTau = 0; idxHadTau < it->second.nHadTaus_; ++idxHadTau )
    {
      bai.setBranch(it->second.pt_[idxHadTau], get_branchName_obj(branchName_obj_, (int)idxHadTau, "pt", central_or_shift));
      bai.setBranch(it->second.eta_[idxHadTau], get_branchName_obj(branchName_obj_, (int)idxHadTau, "eta", central_or_shift));
      bai.setBranch(it->second.phi_[idxHadTau], get_branchName_obj(branchName_obj_, (int)idxHadTau, "phi", central_or_shift));
      bai.setBranch(it->second.mass_[idxHadTau], get_branchName_obj(branchName_obj_, (int)idxHadTau, "mass", central_or_shift));
      bai.setBranch(it->second.decayMode_[idxHadTau], get_branchName_obj(branchName_obj_, (int)idxHadTau, "decayMode", central_or_shift));
      bai.setBranch(it->second.charge_[idxHadTau], get_branchName_obj(branchName_obj_, (int)idxHadTau, "charge", central_or_shift));
      bai.setBranch(it->second.isFakeable_[idxHadTau], get_branchName_obj(branchName_obj_, (int)idxHadTau, "isFakeable", central_or_shift));
      bai.setBranch(it->second.isTight_[idxHadTau], get_branchName_obj(branchName_obj_, (int)idxHadTau, "isTight", central_or_shift));    
      bai.setBranch(it->second.genMatch_[idxHadTau], get_branchName_obj(branchName_obj_, (int)idxHadTau, "genMatch", central_or_shift));
      bai.setBranch(it->second.isFake_[idxHadTau], get_branchName_obj(branchName_obj_, (int)idxHadTau, "isFake", central_or_shift));  
      bai.setBranch(it->second.isFlip_[idxHadTau], get_branchName_obj(branchName_obj_, (int)idxHadTau, "isFlip", central_or_shift));  
    }
  }
}

void
RecoHadTauWriter::set_central_or_shift(const std::string & central_or_shift) const
{
  auto it = central_or_shiftEntries_.find(central_or_shift);
  if ( it != central_or_shiftEntries_.end() )
  {
    current_central_or_shiftEntry_ = const_cast<central_or_shiftEntry *>(&it->second);
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
  return RecoHadTauReader::get_supported_systematics();
}

DEFINE_EDM_PLUGIN(WriterPluginFactory, RecoHadTauWriter, "RecoHadTauWriter");
