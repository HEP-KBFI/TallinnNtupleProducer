#ifndef TallinnNtupleProducer_EvtWeightTools_Data_to_MC_CorrectionInterface_hh_0l_4tau_trigger_h
#define TallinnNtupleProducer_EvtWeightTools_Data_to_MC_CorrectionInterface_hh_0l_4tau_trigger_h

#include "TallinnNtupleProducer/EvtWeightTools/interface/Data_to_MC_CorrectionInterface_0l_2tau_trigger.h"

class Data_to_MC_CorrectionInterface_hh_0l_4tau_trigger
  : public Data_to_MC_CorrectionInterface_0l_2tau_trigger
{
public:
  Data_to_MC_CorrectionInterface_hh_0l_4tau_trigger(const edm::ParameterSet & cfg);
  ~Data_to_MC_CorrectionInterface_hh_0l_4tau_trigger();

  //-----------------------------------------------------------------------------
  // set hadTau pT, eta and decay mode
  // (to be called once per event, before calling any of the getSF.. functions)

  void
  setHadTaus(const RecoHadTau * const hadTau1,
             const RecoHadTau * const hadTau2);

  void
  setHadTaus(const RecoHadTau * const hadTau1,
             const RecoHadTau * const hadTau2,
             const RecoHadTau * const hadTau3,
             const RecoHadTau * const hadTau4);
  //-----------------------------------------------------------------------------

  //-----------------------------------------------------------------------------
  // data/MC correction for trigger efficiency 
  double
  getSF_triggerEff(TriggerSFsys central_or_shift) const;
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

  int hadTau3_genPdgId_;
  double hadTau3_pt_;
  double hadTau3_eta_;
  double hadTau3_phi_;
  int hadTau3_decayMode_;

  int hadTau4_genPdgId_;
  double hadTau4_pt_;
  double hadTau4_eta_;
  double hadTau4_phi_;
  int hadTau4_decayMode_;
};

#endif // TallinnNtupleProducer_EvtWeightTools_data_to_MC_corrections_hh_0l_4tau_trigger_h
