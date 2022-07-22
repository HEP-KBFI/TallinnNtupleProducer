#ifndef TallinnNtupleProducer_EvtWeightTools_Data_to_MC_CorrectionInterface_0l_2tau_trigger_h
#define TallinnNtupleProducer_EvtWeightTools_Data_to_MC_CorrectionInterface_0l_2tau_trigger_h

#include "TallinnNtupleProducer/EvtWeightTools/interface/lutAuxFunctions.h"       // lutWrapperBase, vLutWrapperBase
#include "TallinnNtupleProducer/EvtWeightTools/interface/Data_to_MC_CorrectionInterface_trigger_Base.h"
               // Data_to_MC_CorrectionInterface_trigger_Base
#include "correction.h"
// forward declarations
enum class TriggerSFsys;

class Data_to_MC_CorrectionInterface_0l_2tau_trigger : public Data_to_MC_CorrectionInterface_trigger_Base
{
 public:
  Data_to_MC_CorrectionInterface_0l_2tau_trigger(const edm::ParameterSet & cfg);
  ~Data_to_MC_CorrectionInterface_0l_2tau_trigger();

  //-----------------------------------------------------------------------------

  void
  set_0l_2tau_trigger_sf(correction::Correction::Ref cset);

  // set HLT trigger bits
  // (to be called once per event, before calling any of the getSF.. functions)

  void
  setTriggerBits(bool isTriggered_2tau);
  //-----------------------------------------------------------------------------

  //-----------------------------------------------------------------------------
  // data/MC correction for trigger efficiency 
  double
  getSF_triggerEff(TriggerSFsys central_or_shift) const;
  //-----------------------------------------------------------------------------

 protected:
  bool
  check_triggerSFsys_opt(TriggerSFsys central_or_shift) const;

  bool isTriggered_2tau_;

  //-----------------------------------------------------------------------------
  // data/MC corrections for trigger efficiencies

  correction::Correction::Ref sf_0l_2tau_trigger_;
  //-----------------------------------------------------------------------------
};

#endif // TallinnNtupleProducer_EvtWeightTools_Data_to_MC_CorrectionInterface_0l_2tau_trigger_h
