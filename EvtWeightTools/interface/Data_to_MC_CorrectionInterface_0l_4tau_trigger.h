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

  // define states of tau leg of ditau trigger for each reconstructed tau
  enum { k2tau, kNot2tau };

  // define auxiliary functions
  double
  getProb_tau(int tau_status,
              double eff_2tau_tauLeg) const;

  //-----------------------------------------------------------------------------
  // data/MC corrections for trigger efficiencies in 2017 ReReco data and Summer17 MC

};

#endif // TallinnNtupleProducer_EvtWeightTools_Data_to_MC_CorrectionInterface_0l_4tau_trigger_h
