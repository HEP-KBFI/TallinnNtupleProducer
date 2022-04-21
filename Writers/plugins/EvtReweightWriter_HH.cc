#include "TallinnNtupleProducer/Writers/plugins/EvtReweightWriter_HH.h"

#include "TallinnNtupleProducer/CommonTools/interface/AnalysisConfig.h"       // AnalysisConfig
#include "TallinnNtupleProducer/CommonTools/interface/cmsException.h"         // cmsException()
#include "TallinnNtupleProducer/CommonTools/interface/Era.h"                  // Era, get_era()
#include "TallinnNtupleProducer/Objects/interface/EventInfo.h"                // EventInfo
#include "TallinnNtupleProducer/Readers/interface/BranchAddressInitializer.h" // BranchAddressInitializer

#include "TString.h"                                                          // Form()
#include "TTree.h"                                                            // TTree

#include <assert.h>                                                           // assert()
#include <cstdlib>                                                            // std::abs()

EvtReweightWriter_HH::EvtReweightWriter_HH(const edm::ParameterSet & cfg)
  : WriterBase(cfg)
  , hhWeightInterface_couplings_(nullptr)
  , hhWeightInterfaceLO_(nullptr)
  , hhWeightInterfaceNLO_(nullptr)
  , apply_HH_rwgt_lo_(cfg.getParameter<bool>("apply_rwgt_lo"))
  , apply_HH_rwgt_nlo_(cfg.getParameter<bool>("apply_rwgt_nlo"))
{
  if ( apply_HH_rwgt_lo_ || apply_HH_rwgt_nlo_ )
  {
    hhWeightInterface_couplings_ = new HHWeightInterfaceCouplings(cfg);
    bmNames_ = hhWeight_couplings->get_bm_names();
    if ( apply_HH_rwgt_lo_ )
    {
      hhWeightInterfaceLO_ = new HHWeightInterfaceLO(hhWeightInterface_couplings_, cfg);
    }
    if ( apply_HH_rwgt_nlo_ )
    {
      Era era = get_era(cfg.getParameter<std::string>("era"));
      hhWeightInterfaceNLO_ = new HHWeightInterfaceNLO(hhWeightInterface_couplings_, era);
    }
    for ( auto bmName : bmNames_ )
    {
      branchNames_[bmName] = Form("hhReweight_%s", bmName.data());
      hhReweights_[bmName] = 1.;
    }
  }
}

EvtReweightWriter_HH::~EvtReweightWriter_HH()
{
  delete hhWeightInterface_couplings_;
  delete hhWeightInterfaceLO_;
  delete hhWeightInterfaceNLO_;
}

void
EvtReweightWriter_HH::setBranches(TTree * tree)
{
  BranchAddressInitializer bai(tree);
  for ( auto bmName : bmNames_ )
  {
    bai.setBranch(hhReweights_[bmName], branchNames_[bmName]);
  }
}

void
EvtReweightWriter_HH::write(const Event & event, const EvtWeightRecorder & evtWeightRecorder)
{
  const EventInfo& eventInfo = event.eventInfo();
  const AnalysisConfig& analysisConfig = eventInfo.analysisConfig();
  if ( analysisConfig.isMC_HH_nonresonant() && analysisConfig.isHH_rwgt_allowed() && (apply_HH_rwgt_lo_ || apply_HH_rwgt_nlo_) )
  {
    for ( auto bmName : bmNames_ )
    {
      double hhReweight = 1.;
      if ( apply_HH_rwgt_lo_ )
      {
        assert(HHWeightLO_calc_);
        hhReweight = HHWeightLO_calc_->getRelativeWeight(bmName, eventInfo_.gen_mHH, eventInfo_.gen_cosThetaStar);
      }
      if ( apply_HH_rwgt_nlo_ )
      {
        assert(HHWeightNLO_calc_);
        hhReweight *= HHWeightNLO_calc_->getRelativeWeight_LOtoNLO(bmName, eventInfo_.gen_mHH, eventInfo_.gen_cosThetaStar);
      }
      hhReweights_[bmName] = hhReweight;
    }
  }
}

std::vector<std::string>
EvtReweightWriter_HH::get_supported_systematics()
{
  return std::vector<std::string>();
}

DEFINE_EDM_PLUGIN(WriterPluginFactory, EvtReweightWriter_HH, "EvtReweightWriter_HH");
