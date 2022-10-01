#ifndef TallinnNtupleProducer_EvtWeightTools_Data_to_MC_CorrectionInterface_0l_4tau_trigger_h
#define TallinnNtupleProducer_EvtWeightTools_Data_to_MC_CorrectionInterface_0l_4tau_trigger_h

#include "TallinnNtupleProducer/EvtWeightTools/interface/Data_to_MC_CorrectionInterface_0l_2tau_trigger.h"

class Data_to_MC_CorrectionInterface_0l_4tau_trigger
  : public Data_to_MC_CorrectionInterface_0l_2tau_trigger
{
public:
  Data_to_MC_CorrectionInterface_0l_4tau_trigger(const edm::ParameterSet & cfg);
  ~Data_to_MC_CorrectionInterface_0l_4tau_trigger();

  //-----------------------------------------------------------------------------
  // data/MC correction for trigger efficiency 
  virtual double
  getSF_triggerEff(TriggerSFsys central_or_shift) const override;
  //-----------------------------------------------------------------------------

protected:

  bool
  check_triggerSFsys_opt(TriggerSFsys central_or_shift) const;

  // data/MC corrections for trigger efficiencies in 2017 ReReco data and Summer17 MC

};

#endif // TallinnNtupleProducer_EvtWeightTools_Data_to_MC_CorrectionInterface_0l_4tau_trigger_h
