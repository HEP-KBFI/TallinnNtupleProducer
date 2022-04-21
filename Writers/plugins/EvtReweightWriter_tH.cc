#include "TallinnNtupleProducer/Writers/plugins/EvtReweightWriter_tH.h"

#include "TallinnNtupleProducer/CommonTools/interface/cmsException.h"         // cmsException()
#include "TallinnNtupleProducer/Objects/interface/RecoLepton.h"               // RecoLeptonPtrCollection
#include "TallinnNtupleProducer/Readers/interface/BranchAddressInitializer.h" // BranchAddressInitializer

#include "TString.h"                                                          // Form()
#include "TTree.h"                                                            // TTree

#include <assert.h>                                                           // assert()
#include <cstdlib>                                                            // std::abs()

EvtReweightWriter_tH::EvtReweightWriter_tH(const edm::ParameterSet & cfg)
  : WriterBase(cfg)
  , tHweights_(cfg.getParameterSetVector("tHweights"))
  , eventInfo_isInitialized_(false)
  , tree_(nullptr)
  , branchAddresses_isInitialized_(false)
{}

EvtReweightWriter_tH::~EvtReweightWriter_tH()
{}

void
EvtReweightWriter_tH::setBranches(TTree * tree)
{
  tree_ = tree;
}

void
EvtReweightWriter_tH::write(const Event & event, const EvtWeightRecorder & evtWeightRecorder, const std::string & central_or_shift)
{
  const EventInfo& eventInfo = event.eventInfo();
  const AnalysisConfig& analysisConfig = eventInfo.analysisConfig();
  if ( eventInfo.isMC_tH() && !tHweights_.empty() )
  {
    const std::string central_or_shift_tH = eventInfo.has_central_or_shift(central_or_shift) ? central_or_shift : "central";
    eventInfo.set_central_or_shift(central_or_shift_tH);
    if ( !eventInfo_isInitialized_ )
    {
      eventInfo.loadWeight_tH(tHweights);
    }
    std::vector<std::string> couplingScenarios = eventInfo.getWeight_tH_str(central_or_shift_tH);
    if ( !branchAddresses_isInitialized_[central_or_shift_tH] )
    {
      assert(tree_);
      BranchAddressInitializer bai(tree_);
      for ( auto couplingScenario : couplingScenarios )
      {
        branchNames_[central_or_shift_tH][couplingScenario] = Form("thReweight_%s", couplingScenario.data());
        thReweights_[central_or_shift_tH][couplingScenario] = 1.;
        bai.setBranch(thReweights_[central_or_shift_tH][couplingScenario], branchNames_[central_or_shift_tH][couplingScenario]);
      }
      branchAddresses_isInitialized_[central_or_shift_tH] = true;
    }
    const double evtWeight_tH_nom = evtWeightRecorder.get_nom_tH_weight(central_or_shift_tH);
    for ( auto couplingScenario : couplingScenarios )
    {
      thReweights_[central_or_shift_tH][couplingScenario] = (evtWeight/evtWeight_tH_nom)*eventInfo.genWeight_tH(central_or_shift_tH, couplingScenario);
    }
  }
}

std::vector<std::string>
EvtReweightWriter_tH::get_supported_systematics()
{
  return std::vector<std::string>();
}

DEFINE_EDM_PLUGIN(WriterPluginFactory, EvtReweightWriter_tH, "EvtReweightWriter_tH");
