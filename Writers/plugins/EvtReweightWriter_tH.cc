#include "TallinnNtupleProducer/Writers/plugins/EvtReweightWriter_tH.h"

#include "TallinnNtupleProducer/CommonTools/interface/BranchAddressInitializer.h" // BranchAddressInitializer
#include "TallinnNtupleProducer/CommonTools/interface/cmsException.h"             // cmsException()

#include "TString.h"                                                              // Form()
#include "TTree.h"                                                                // TTree

#include <assert.h>                                                               // assert()

EvtReweightWriter_tH::EvtReweightWriter_tH(const edm::ParameterSet & cfg)
  : WriterBase(cfg)
  , tHweights_(cfg.getParameterSetVector("tHweights"))
  , eventInfo_isInitialized_(false)
  , outputTree_(nullptr)
{}

EvtReweightWriter_tH::~EvtReweightWriter_tH()
{}

void
EvtReweightWriter_tH::setBranches(TTree * outputTree)
{
  outputTree_ = outputTree;
}

namespace
{
  std::string
  get_branchName(const std::string & branchName, const std::string & couplingScenario, const std::string & central_or_shift)
  {
    if ( central_or_shift == "central" ) return Form("%s_%s",    branchName.data(), couplingScenario.data());
    else                                 return Form("%s_%s_%s", branchName.data(), central_or_shift.data(), couplingScenario.data());
  }
}

void
EvtReweightWriter_tH::writeImp(const Event & event, const EvtWeightRecorder & evtWeightRecorder)
{
  const EventInfo& eventInfo = event.eventInfo();
  if ( eventInfo.analysisConfig().isMC_tH() && !tHweights_.empty() )
  {
    const std::string central_or_shift_tH = eventInfo.has_central_or_shift(current_central_or_shift_) ? current_central_or_shift_ : "central";
    eventInfo.set_central_or_shift(central_or_shift_tH);
    if ( !eventInfo_isInitialized_ )
    {
      (const_cast<EventInfo*>(&eventInfo))->loadWeight_tH(tHweights_);
    }
    std::vector<std::string> couplingScenarios = eventInfo.getWeight_tH_str(central_or_shift_tH);
    if ( !branchAddresses_isInitialized_[central_or_shift_tH] )
    {
      assert(outputTree_);
      BranchAddressInitializer bai(outputTree_);
      central_or_shiftEntry it;
      for ( auto couplingScenario : couplingScenarios )
      {
        it.thReweights_[couplingScenario] = 1.;
      }
      central_or_shiftEntries_[central_or_shift_tH] = it;
      current_central_or_shiftEntry_ = &central_or_shiftEntries_[central_or_shift_tH];
      for ( auto couplingScenario : couplingScenarios )
      {
        bai.setBranch(current_central_or_shiftEntry_->thReweights_[couplingScenario], get_branchName("thReweight", couplingScenario, central_or_shift_tH));
      }
      branchAddresses_isInitialized_[central_or_shift_tH] = true;
    }
    const double evtWeight = evtWeightRecorder.get(central_or_shift_tH);
    const double evtWeight_tH_nom = evtWeightRecorder.get_nom_tH_weight(central_or_shift_tH);
    for ( auto couplingScenario : couplingScenarios )
    {
      current_central_or_shiftEntry_->thReweights_[couplingScenario] = (evtWeight/evtWeight_tH_nom)*eventInfo.genWeight_tH(central_or_shift_tH, couplingScenario);
    }
  }
}

std::vector<std::string>
EvtReweightWriter_tH::get_supported_systematics(const edm::ParameterSet & cfg)
{
  return std::vector<std::string>(); // CV: to be implemented !!
}

DEFINE_EDM_PLUGIN(WriterPluginFactory, EvtReweightWriter_tH, "EvtReweightWriter_tH");
