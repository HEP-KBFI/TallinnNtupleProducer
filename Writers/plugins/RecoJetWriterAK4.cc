#include "TallinnNtupleProducer/Writers/plugins/RecoJetWriterAK4.h"

#include "TallinnNtupleProducer/CommonTools/interface/BranchAddressInitializer.h" // BranchAddressInitializer
#include "TallinnNtupleProducer/CommonTools/interface/cmsException.h"             // cmsException()
#include "TallinnNtupleProducer/CommonTools/interface/merge_systematic_shifts.h"  // merge_systematic_shifts()
#include "TallinnNtupleProducer/Objects/interface/GenHadTau.h"                    // GenHadTau
#include "TallinnNtupleProducer/Objects/interface/GenLepton.h"                    // GenLepton
#include "TallinnNtupleProducer/Readers/interface/RecoJetReaderAK4.h"             // RecoJetReaderAK4::get_supported_systematics()

#include "TString.h"                                                              // Form()
#include "TTree.h"                                                                // TTree

#include <assert.h>                                                               // assert()

RecoJetWriterAK4::RecoJetWriterAK4(const edm::ParameterSet & cfg)
  : WriterBase(cfg)
  , jetCollection_(JetCollection::kUndefined)
  , branchName_num_("")
  , branchName_obj_("")
  , current_central_or_shiftEntry_(nullptr)
{
  std::string jetCollection_string = cfg.getParameter<std::string>("jetCollection");
  if      ( jetCollection_string == "selJetsAK4"            ) jetCollection_ = JetCollection::kSelJetsAK4;
  else if ( jetCollection_string == "selJetsAK4_btagLoose"  ) jetCollection_ = JetCollection::kSelJetsAK4_btagLoose;
  else if ( jetCollection_string == "selJetsAK4_btagMedium" ) jetCollection_ = JetCollection::kSelJetsAK4_btagMedium;
  else throw cmsException(__func__, __LINE__) 
    << "Invalid Configuration parameter 'jetCollection' = " << jetCollection_string;
  branchName_obj_ = cfg.getParameter<std::string>("branchName");
  branchName_num_ = Form("n%s", branchName_obj_.data());
  max_nJets_ = cfg.getParameter<unsigned>("max_numJets");
  assert(max_nJets_ >= 1);
  merge_systematic_shifts(supported_systematics_, RecoJetWriterAK4::get_supported_systematics(cfg));
  merge_systematic_shifts(supported_systematics_, { "central" }); // CV: add central value
  for ( auto central_or_shift : supported_systematics_ )
  {    
    central_or_shiftEntry it;
    it.nJets_ = 0;
    it.pt_ = new Float_t[max_nJets_];
    it.eta_ = new Float_t[max_nJets_];
    it.phi_ = new Float_t[max_nJets_];
    it.mass_ = new Float_t[max_nJets_];
    it.charge_ = new Int_t[max_nJets_];
    it.qgDiscr_ = new Float_t[max_nJets_];
    it.bRegCorr_ = new Float_t[max_nJets_];
    it.jetId_ = new Int_t[max_nJets_];
    it.puId_ = new Int_t[max_nJets_];
    it.genMatch_ = new Int_t[max_nJets_];
    central_or_shiftEntries_[central_or_shift] = it;
  }
  current_central_or_shiftEntry_ = &central_or_shiftEntries_["central"];
}

RecoJetWriterAK4::~RecoJetWriterAK4()
{
  for ( auto it : central_or_shiftEntries_ )
  {
    delete[] it.second.pt_;
    delete[] it.second.eta_;
    delete[] it.second.phi_;
    delete[] it.second.mass_;
    delete[] it.second.charge_;
    delete[] it.second.qgDiscr_;
    delete[] it.second.bRegCorr_;
    delete[] it.second.jetId_;
    delete[] it.second.puId_;
    delete[] it.second.genMatch_;
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
    if ( central_or_shift == "central" ) return Form("%s%i_%s",    branchName_obj.data(), (int)idx + 1, suffix.data());
    else                                 return Form("%s%i_%s_%s", branchName_obj.data(), (int)idx + 1, central_or_shift.data(), suffix.data());
  }
}

void
RecoJetWriterAK4::setBranches(TTree * outputTree)
{
  BranchAddressInitializer bai(outputTree);
  for ( auto central_or_shift : supported_systematics_ )
  {
    auto it = central_or_shiftEntries_.find(central_or_shift);
    assert(it != central_or_shiftEntries_.end());
    bai.setBranch(it->second.nJets_, get_branchName_num(branchName_num_, central_or_shift));
    for ( size_t idxJet = 0; idxJet < max_nJets_; ++idxJet )
    {
      bai.setBranch(it->second.pt_[idxJet], get_branchName_obj(branchName_obj_, (int)idxJet, "pt", central_or_shift));
      bai.setBranch(it->second.eta_[idxJet], get_branchName_obj(branchName_obj_, (int)idxJet, "eta", central_or_shift));
      bai.setBranch(it->second.phi_[idxJet], get_branchName_obj(branchName_obj_, (int)idxJet, "phi", central_or_shift));
      bai.setBranch(it->second.mass_[idxJet], get_branchName_obj(branchName_obj_, (int)idxJet, "mass", central_or_shift));
      bai.setBranch(it->second.charge_[idxJet], get_branchName_obj(branchName_obj_, (int)idxJet, "charge", central_or_shift));
      bai.setBranch(it->second.qgDiscr_[idxJet], get_branchName_obj(branchName_obj_, (int)idxJet, "qgDiscr", central_or_shift));
      bai.setBranch(it->second.bRegCorr_[idxJet], get_branchName_obj(branchName_obj_, (int)idxJet, "bRegCorr", central_or_shift));  
      bai.setBranch(it->second.jetId_[idxJet], get_branchName_obj(branchName_obj_, (int)idxJet, "jetId", central_or_shift));
      bai.setBranch(it->second.puId_[idxJet], get_branchName_obj(branchName_obj_, (int)idxJet, "puId", central_or_shift));  
      bai.setBranch(it->second.genMatch_[idxJet], get_branchName_obj(branchName_obj_, (int)idxJet, "genMatch", central_or_shift));
    }
  }
}

void
RecoJetWriterAK4::set_central_or_shift(const std::string & central_or_shift) const
{
  WriterBase::set_central_or_shift(central_or_shift);
  auto it = central_or_shiftEntries_.find(central_or_shift);
  if ( it != central_or_shiftEntries_.end() )
  {
    current_central_or_shiftEntry_ = const_cast<central_or_shiftEntry *>(&it->second);
  }
  else
  {
    current_central_or_shiftEntry_ = nullptr;
  }
}

namespace
{
  unsigned
  compGenMatch(const RecoJetAK4 * jet)
  {
    unsigned genMatch = 0;
    if ( jet->genLepton() )
    {
      genMatch = 2;
    }
    else if ( jet->genHadTau() )
    {
      genMatch = 4;
    }
    else
    {
      genMatch = 1;
    }
    return genMatch;
  }
}

void
RecoJetWriterAK4::writeImp(const Event & event, const EvtWeightRecorder & evtWeightRecorder)
{
  if ( current_central_or_shiftEntry_ )
  {
    const RecoJetPtrCollectionAK4* jets = nullptr;
    if      ( jetCollection_ == JetCollection::kSelJetsAK4            ) jets = &event.selJetsAK4();
    else if ( jetCollection_ == JetCollection::kSelJetsAK4_btagLoose  ) jets = &event.selJetsAK4_btagLoose();
    else if ( jetCollection_ == JetCollection::kSelJetsAK4_btagMedium ) jets = &event.selJetsAK4_btagMedium();
    else assert(0);
    auto it = current_central_or_shiftEntry_;
    it->nJets_ = jets->size();
    for ( size_t idxJet = 0; idxJet < max_nJets_; ++idxJet )
    {
      if ( idxJet < it->nJets_ )
      {
        const RecoJetAK4 * jet = (*jets)[idxJet];
        it->pt_[idxJet] = jet->pt();
        it->eta_[idxJet] = jet->eta();
        it->phi_[idxJet] = jet->phi();
        it->mass_[idxJet] = jet->mass();
        it->charge_[idxJet] = jet->charge();
        it->qgDiscr_[idxJet] = jet->QGDiscr();
        it->bRegCorr_[idxJet] = jet->bRegCorr();
        it->jetId_[idxJet] = jet->jetId();
        it->puId_[idxJet] = jet->puId();
        it->genMatch_[idxJet] = compGenMatch(jet);
      }
      else
      {
        it->pt_[idxJet] = 0.;
        it->eta_[idxJet] = 0.;
        it->phi_[idxJet] = 0.;
        it->mass_[idxJet] = 0.;
        it->charge_[idxJet] = 0;
        it->qgDiscr_[idxJet] = 0.;
        it->bRegCorr_[idxJet] = 0.;
        it->jetId_[idxJet] = 0;
        it->puId_[idxJet] = 0;
        it->genMatch_[idxJet] = 0;
      }
    }
  }
}

std::vector<std::string>
RecoJetWriterAK4::get_supported_systematics(const edm::ParameterSet & cfg)
{
  return RecoJetReaderAK4::get_supported_systematics(cfg);
}

DEFINE_EDM_PLUGIN(WriterPluginFactory, RecoJetWriterAK4, "RecoJetWriterAK4");
