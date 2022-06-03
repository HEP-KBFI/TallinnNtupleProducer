#include "TallinnNtupleProducer/Writers/plugins/EvtReweightWriter_HH.h"

#include "TallinnNtupleProducer/CommonTools/interface/AnalysisConfig.h"           // AnalysisConfig
#include "TallinnNtupleProducer/CommonTools/interface/BranchAddressInitializer.h" // BranchAddressInitializer
#include "TallinnNtupleProducer/CommonTools/interface/cmsException.h"             // cmsException()
#include "TallinnNtupleProducer/CommonTools/interface/Era.h"                      // Era, get_era()
#include "TallinnNtupleProducer/Objects/interface/EventInfo.h"                    // EventInfo

#include "TString.h"                                                              // Form()
#include "TTree.h"                                                                // TTree

#include <assert.h>                                                               // assert()

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
    edm::ParameterSet cfg_hhCoupling = cfg.getParameterSet("hhCoupling");
    hhWeightInterface_couplings_ = new HHWeightInterfaceCouplings(cfg_hhCoupling);
    bmNames_ = hhWeightInterface_couplings_->get_bm_names();
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
EvtReweightWriter_HH::setBranches(TTree * outputTree)
{
  BranchAddressInitializer bai(outputTree);
  for ( auto bmName : bmNames_ )
  {
    bai.setBranch(hhReweights_[bmName], branchNames_[bmName]);
  }
}

void
EvtReweightWriter_HH::writeImp(const Event & event, const EvtWeightRecorder & evtWeightRecorder)
{
  const EventInfo& eventInfo = event.eventInfo();
  const AnalysisConfig& analysisConfig = eventInfo.analysisConfig();
  if ( analysisConfig.isMC_HH_nonresonant() && analysisConfig.isHH_rwgt_allowed() && (apply_HH_rwgt_lo_ || apply_HH_rwgt_nlo_) )
  {
    double hhReweight_sm = 1.;
    if ( apply_HH_rwgt_lo_ )
    {
      hhReweight_sm *= hhWeightInterfaceLO_->getWeight("SM", evtWeightRecorder.gen_mHH(), eventInfo.gen_cosThetaStar());
    }
    if ( apply_HH_rwgt_nlo_ )
    {
      hhReweight_sm *= hhWeightInterfaceNLO_->getWeight_LOtoNLO("SM", evtWeightRecorder.gen_mHH(), eventInfo.gen_cosThetaStar());
    }
    for ( auto bmName : bmNames_ )
    {
      double hhReweight_bm = 1.;
      if ( apply_HH_rwgt_lo_ )
      {
        assert(hhWeightInterfaceLO_);
        hhReweight_bm *= hhWeightInterfaceLO_->getRelativeWeight(bmName, evtWeightRecorder.gen_mHH(), evtWeightRecorder.gen_cosThetaStar());
      }
      if ( apply_HH_rwgt_nlo_ )
      {
        assert(hhWeightInterfaceNLO_);
        hhReweight_bm *= hhWeightInterfaceNLO_->getRelativeWeight_LOtoNLO(bmName, evtWeightRecorder.gen_mHH(), evtWeightRecorder.gen_cosThetaStar());
      }
      hhReweights_[bmName] = hhReweight_sm*hhReweight_bm;
    }
  }
}

std::vector<std::string>
EvtReweightWriter_HH::get_supported_systematics(const edm::ParameterSet & cfg)
{
  return std::vector<std::string>();
}

DEFINE_EDM_PLUGIN(WriterPluginFactory, EvtReweightWriter_HH, "EvtReweightWriter_HH");
