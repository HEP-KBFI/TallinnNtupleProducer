#ifndef TallinnNtupleProducer_EvtWeightTools_Data_to_MC_CorrectionInterface_1l_1tau_trigger_h
#define TallinnNtupleProducer_EvtWeightTools_Data_to_MC_CorrectionInterface_1l_1tau_trigger_h

#include "TallinnNtupleProducer/EvtWeightTools/interface/lutAuxFunctions.h"       // lutWrapperBase, vLutWrapperBase
#include "TallinnNtupleProducer/EvtWeightTools/interface/TauTriggerSFInterface.h" // TauTriggerSFInterface, TriggerSFsys

// forward declarations
class RecoHadTau;
class RecoLepton;

enum class Era;

class Data_to_MC_CorrectionInterface_1l_1tau_trigger
{
 public:
  Data_to_MC_CorrectionInterface_1l_1tau_trigger(const edm::ParameterSet & cfg);
  ~Data_to_MC_CorrectionInterface_1l_1tau_trigger();

  //-----------------------------------------------------------------------------
  // set HLT trigger bits
  // (to be called once per event, before calling any of the getSF.. functions)
  void
  setTriggerBits(bool isTriggered_1e,
                 bool isTriggered_1e1tau,
                 bool isTriggered_1m,
                 bool isTriggered_1m1tau);
  //-----------------------------------------------------------------------------

  //-----------------------------------------------------------------------------
  // set lepton type, pT and eta as well as hadTau pT, eta and decay mode
  // (to be called once per event, before calling any of the getSF.. functions)
  void
  setLepton(const RecoLepton * const lepton);

  void
  setHadTau(const RecoHadTau * const lepton);

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

  std::string era_str_;
  Era era_;
  std::string hadTauSelection_;
  bool isDEBUG_;
  std::vector<int> allowedDecayModes_;

  bool isTriggered_1e_;
  bool isTriggered_1e1tau_;
  bool isTriggered_1m_;
  bool isTriggered_1m1tau_;

  int lepton_type_;
  double lepton_pt_;
  double lepton_eta_;

  int hadTau_genPdgId_;
  double hadTau_pt_;
  double hadTau_eta_;
  double hadTau_phi_;
  int hadTau_decayMode_;

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

  TauTriggerSFInterface effTrigger_1e1tau_tauLeg_;
  TauTriggerSFInterface effTrigger_1m1tau_tauLeg_;
  //-----------------------------------------------------------------------------
};

#endif // TallinnNtupleProducer_EvtWeightTools_Data_to_MC_CorrectionInterface_1l_1tau_trigger_h