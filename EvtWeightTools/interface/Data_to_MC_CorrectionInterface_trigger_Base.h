#ifndef TallinnNtupleProducer_EvtWeightTools_Data_to_MC_CorrectionInterface_trigger_Base_h
#define TallinnNtupleProducer_EvtWeightTools_Data_to_MC_CorrectionInterface_trigger_Base_h

#include "TallinnNtupleProducer/EvtWeightTools/interface/TauTriggerSFValues.h" // TauTriggerSFValues

#include "FWCore/ParameterSet/interface/ParameterSet.h"                        // edm::ParameterSet

#include "correction.h"                                                        // correction::CorrectionSet, correction::Correction::Ref

#include <iostream>                                                            // std::cout

// forward declarations
class RecoHadTau;
class RecoLepton;

enum class Era;
enum class TauID;
enum class TriggerSFsys;

class Data_to_MC_CorrectionInterface_trigger_Base
{
 public:
  Data_to_MC_CorrectionInterface_trigger_Base(const edm::ParameterSet & cfg);
  virtual ~Data_to_MC_CorrectionInterface_trigger_Base() = default;

  //-----------------------------------------------------------------------------

  //-----------------------------------------------------------------------------
  // set hadTau pT, eta and decay mode
  // (to be called once per event, before calling any of the getSF.. functions)
  void
  setHadTaus(const std::vector<const RecoHadTau * > & hadTaus);
  //-----------------------------------------------------------------------------

  // set lepton type, pT and eta as well as hadTau pT, eta and decay mode
  // (to be called once per event, before calling any of the getSF.. functions)
  void
  setLeptons(const std::vector<const RecoLepton *> & leptons);

  // data/MC correction for trigger efficiency
  virtual double
  getSF_triggerEff(TriggerSFsys central_or_shift) const = 0;
  //----------------------------------------------------------------------------- 

 protected:

  static const std::map<std::string, double> tau_trigger_ptThresholds_;

  double
  tau_leg_efficiency(double pt,
                     int dm,
                     const std::string & trigger_type,
                     const std::string & wp,
                     const std::string & data_type,
                     const std::string & sys) const;

  TauTriggerSFValues
  tau_leg_efficiency(double pt,
                     int dm,
                     const std::string & trigger_type,
                     const std::string & wp,
                     const std::string & data_type) const;

  std::string era_str_;
  Era era_;
  std::string hadTauSelection_;
  bool isDEBUG_;
  std::vector<int> allowedDecayModes_;

  int lepton_type_;
  double lepton_pt_;
  double lepton_eta_;

  int hadTau1_genPdgId_;
  double hadTau1_pt_;
  double hadTau1_eta_;
  double hadTau1_phi_;
  int hadTau1_decayMode_;

  int hadTau2_genPdgId_;
  double hadTau2_pt_;
  double hadTau2_eta_;
  double hadTau2_phi_;
  int hadTau2_decayMode_;

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

  const TauID tauId_;
  const std::string wp_str_;

  std::vector<const RecoHadTau*> hadTau_recObjs;

  std::unique_ptr<correction::CorrectionSet> tau_tigger_cset_;
  correction::Correction::Ref sf_trigger_;
  //-----------------------------------------------------------------------------
  // data/MC corrections for trigger efficiencies
  //-----------------------------------------------------------------------------
};

#endif // TallinnNtupleProducer_EvtWeightTools_Data_to_MC_CorrectionInterface_trigger_Base_h
