#ifndef TallinnNtupleProducer_EvtWeightTools_Data_to_MC_CorrectionInterface_1l_1tau_trigger_h
#define TallinnNtupleProducer_EvtWeightTools_Data_to_MC_CorrectionInterface_1l_1tau_trigger_h

#include "TallinnNtupleProducer/EvtWeightTools/interface/Data_to_MC_CorrectionInterface_trigger_Base.h" // Data_to_MC_CorrectionInterface_trigger_Base
#include "TallinnNtupleProducer/EvtWeightTools/interface/lutAuxFunctions.h"                             // lutWrapperBase, vLutWrapperBase
#include "correction.h"

// forward declarations
enum class TriggerSFsys;


class Data_to_MC_CorrectionInterface_1l_1tau_trigger
  : public Data_to_MC_CorrectionInterface_trigger_Base
{
 public:
  Data_to_MC_CorrectionInterface_1l_1tau_trigger(const edm::ParameterSet & cfg);
  ~Data_to_MC_CorrectionInterface_1l_1tau_trigger();

  //-----------------------------------------------------------------------------

  void
  set_1l_1tau_trigger_sf(correction::Correction::Ref cset);

  // set HLT trigger bits
  // (to be called once per event, before calling any of the getSF.. functions)
  void
  setTriggerBits(bool isTriggered_1e,
                 bool isTriggered_1e1tau,
                 bool isTriggered_1m,
                 bool isTriggered_1m1tau);
  //-----------------------------------------------------------------------------

  //-----------------------------------------------------------------------------
  // data/MC correction for trigger efficiency 
  double
  getSF_triggerEff(TriggerSFsys central_or_shift) const;
  //-----------------------------------------------------------------------------

 protected:
  bool
  check_triggerSFsys_opt(TriggerSFsys central_or_shift) const;

  std::map<std::string, TFile *> inputFiles_;

  bool isTriggered_1e_;
  bool isTriggered_1e1tau_;
  bool isTriggered_1m_;
  bool isTriggered_1m1tau_;

  //-----------------------------------------------------------------------------
  // data/MC corrections for trigger efficiencies in 2017 ReReco data and Summer17 MC

  vLutWrapperBase effTrigger_1e_data_;
  vLutWrapperBase effTrigger_1e_mc_;
  vLutWrapperBase effTrigger_1e1tau_lepLeg_data_;
  vLutWrapperBase effTrigger_1e1tau_lepLeg_mc_;

  vLutWrapperBase effTrigger_1m_data_;
  vLutWrapperBase effTrigger_1m_mc_;
  vLutWrapperBase effTrigger_1m1tau_lepLeg_data_;
  vLutWrapperBase effTrigger_1m1tau_lepLeg_mc_;

  correction::Correction::Ref sf_1l_1tau_trigger_;
  //-----------------------------------------------------------------------------
};

#endif // TallinnNtupleProducer_EvtWeightTools_Data_to_MC_CorrectionInterface_1l_1tau_trigger_h
