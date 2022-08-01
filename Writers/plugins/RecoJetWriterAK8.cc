#include "TallinnNtupleProducer/Writers/plugins/RecoJetWriterAK8.h"

#include "TallinnNtupleProducer/CommonTools/interface/BranchAddressInitializer.h" // BranchAddressInitializer
#include "TallinnNtupleProducer/CommonTools/interface/cmsException.h"             // cmsException()
#include "TallinnNtupleProducer/CommonTools/interface/merge_systematic_shifts.h"  // merge_systematic_shifts()
#include "TallinnNtupleProducer/Readers/interface/RecoJetReaderAK8.h"             // RecoJetReaderAK8::get_supported_systematics()

#include "TString.h"                                                              // Form()
#include "TTree.h"                                                                // TTree

#include <assert.h>                                                               // assert()

RecoJetWriterAK8::RecoJetWriterAK8(const edm::ParameterSet & cfg)
  : WriterBase(cfg)
  , jetCollection_(JetCollection::kUndefined)
  , branchName_num_("")
  , branchName_obj_("")
  , current_central_or_shiftEntry_(nullptr)
{
  std::string jetCollection_string = cfg.getParameter<std::string>("jetCollection");
  if      ( jetCollection_string == "selJetsAK8_Wjj" ) jetCollection_ = JetCollection::kSelJetsAK8_Wjj;
  else if ( jetCollection_string == "selJetsAK8_Hbb" ) jetCollection_ = JetCollection::kSelJetsAK8_Hbb;
  else throw cmsException(__func__, __LINE__) 
    << "Invalid Configuration parameter 'jetCollection' = " << jetCollection_string;
  branchName_obj_ = cfg.getParameter<std::string>("branchName");
  branchName_num_ = Form("n%s", branchName_obj_.data());
  max_nJets_ = cfg.getParameter<unsigned>("max_numJets");
  assert(max_nJets_ >= 1);
  merge_systematic_shifts(supported_systematics_, RecoJetWriterAK8::get_supported_systematics(cfg));
  merge_systematic_shifts(supported_systematics_, { "central" }); // CV: add central value
  for ( const std::string & central_or_shift : supported_systematics_ )
  {    
    central_or_shiftEntry it;
    it.nJets_ = 0;
    it.pt_ = new Float_t[max_nJets_];
    it.eta_ = new Float_t[max_nJets_];
    it.phi_ = new Float_t[max_nJets_];
    it.mass_ = new Float_t[max_nJets_];
    it.msoftdrop_ = new Float_t[max_nJets_];
    it.tau21_ = new Float_t[max_nJets_];
    it.jetId_ = new Int_t[max_nJets_];
    it.subjet1_pt_ = new Float_t[max_nJets_];
    it.subjet2_pt_ = new Float_t[max_nJets_];
    central_or_shiftEntries_[central_or_shift] = it;
  }
  current_central_or_shiftEntry_ = &central_or_shiftEntries_["central"];
}

RecoJetWriterAK8::~RecoJetWriterAK8()
{
  for ( auto & it : central_or_shiftEntries_ )
  {
    delete[] it.second.pt_;
    delete[] it.second.eta_;
    delete[] it.second.phi_;
    delete[] it.second.mass_;
    delete[] it.second.msoftdrop_;
    delete[] it.second.tau21_;
    delete[] it.second.jetId_;
    delete[] it.second.subjet1_pt_;
    delete[] it.second.subjet2_pt_;
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
    else                                 return Form("%s%i_%s_%s", branchName_obj.data(), (int)idx + 1, suffix.data(), central_or_shift.data());
  }
}

void
RecoJetWriterAK8::setBranches(TTree * outputTree)
{
  BranchAddressInitializer bai(outputTree);
  for ( const std::string & central_or_shift : supported_systematics_ )
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
      bai.setBranch(it->second.msoftdrop_[idxJet], get_branchName_obj(branchName_obj_, (int)idxJet, "msoftdrop", central_or_shift));
      bai.setBranch(it->second.tau21_[idxJet], get_branchName_obj(branchName_obj_, (int)idxJet, "tau21", central_or_shift));
      bai.setBranch(it->second.jetId_[idxJet], get_branchName_obj(branchName_obj_, (int)idxJet, "jetId", central_or_shift));
      bai.setBranch(it->second.subjet1_pt_[idxJet], get_branchName_obj(branchName_obj_, (int)idxJet, "subjet1_pt", central_or_shift));
      bai.setBranch(it->second.subjet2_pt_[idxJet], get_branchName_obj(branchName_obj_, (int)idxJet, "subjet2_pt", central_or_shift));
    }
  }
}

void
RecoJetWriterAK8::set_central_or_shift(const std::string & central_or_shift) const
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

void
RecoJetWriterAK8::writeImp(const Event & event, const EvtWeightRecorder & evtWeightRecorder)
{
  if ( current_central_or_shiftEntry_ )
  {
    const RecoJetPtrCollectionAK8* jets = nullptr;
    if      ( jetCollection_ == JetCollection::kSelJetsAK8_Wjj ) jets = &event.selJetsAK8_Wjj();
    else if ( jetCollection_ == JetCollection::kSelJetsAK8_Hbb ) jets = &event.selJetsAK8_Hbb();
    else assert(0);
    auto it = current_central_or_shiftEntry_;
    it->nJets_ = jets->size();
    for ( size_t idxJet = 0; idxJet < max_nJets_; ++idxJet )
    {
      if ( idxJet < it->nJets_ )
      {
        const RecoJetAK8 * jet = (*jets)[idxJet];
        it->pt_[idxJet] = jet->pt();
        it->eta_[idxJet] = jet->eta();
        it->phi_[idxJet] = jet->phi();
        it->mass_[idxJet] = jet->mass();
        it->msoftdrop_[idxJet] = jet->msoftdrop();
        it->tau21_[idxJet] = jet->tau1() > 0. ? jet->tau2()/jet->tau1() : 1.;
        it->jetId_[idxJet] = jet->jetId();
        it->subjet1_pt_[idxJet] = jet->subJet1() ? jet->subJet1()->pt() : 0.;
        it->subjet2_pt_[idxJet] = jet->subJet2() ? jet->subJet2()->pt() : 0.;
      }
      else
      {
        it->pt_[idxJet] = 0.;
        it->eta_[idxJet] = 0.;
        it->phi_[idxJet] = 0.;
        it->mass_[idxJet] = 0.;
        it->msoftdrop_[idxJet] = 0.;
        it->tau21_[idxJet] = 0.;
        it->jetId_[idxJet] = 0.;
        it->subjet1_pt_[idxJet] = 0.;
        it->subjet2_pt_[idxJet] = 0.;
      }
    }
  }
}

std::vector<std::string>
RecoJetWriterAK8::get_supported_systematics(const edm::ParameterSet & cfg)
{
  return RecoJetReaderAK8::get_supported_systematics(cfg);
}

DEFINE_EDM_PLUGIN(WriterPluginFactory, RecoJetWriterAK8, "RecoJetWriterAK8"); // clazy:skip
