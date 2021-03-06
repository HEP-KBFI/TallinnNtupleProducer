#ifndef TallinnNtupleProducer_EvtWeightTools_data_to_MC_corrections_auxFunctions_h
#define TallinnNtupleProducer_EvtWeightTools_data_to_MC_corrections_auxFunctions_h

#include "TallinnNtupleProducer/EvtWeightTools/interface/lutAuxFunctions.h"    // lutWrapperBase, vLutWrapperBase
#include "TallinnNtupleProducer/EvtWeightTools/interface/TauTriggerSFValues.h" // TauTriggerSFValues

#include "correction.h"

#include <map>                                                                 // std::map
#include <string>                                                              // std::string
#include <vector>                                                              // std::vector

// forward declarations
class TFile;

enum class TriggerSFsys;

namespace aux
{
  std::string
  getEtaBinLabel(double etaMin,
                 double etaMax,
                 bool replace_period = false);

  std::string
  getHadTauIdxLabel(int idx);

  std::string
  getHadTauSelectionLabel(const std::string & hadTauSelection);

  std::string
  getHadTauDecayModeLabel(int hadTauDecayMode);

  void
  clearCollection(std::vector<lutWrapperBase *> & collection);

  void
  clearCollection(std::map<int, std::vector<lutWrapperBase *>> & collection);

  double
  get_from_lut(const std::map<int, std::vector<lutWrapperBase *>> & corrections,
               double hadTau_pt,
               double hadTau_eta,
               int hadTau_decayMode,
               bool isDEBUG);

  double
  compSF(double eff_data,
         double eff_mc);

  double
  compSF(const TauTriggerSFValues & eff_data,
         const TauTriggerSFValues & eff_mc,
         TriggerSFsys central_or_shift);

  bool
  hasDecayMode(const std::vector<int> & allowedDecayModes,
               int hadTau_decayMode);

  void
  loadTriggerEff_1e_2016(vLutWrapperBase & effTrigger_1e_data,
                         vLutWrapperBase & effTrigger_1e_mc,
                         std::map<std::string, TFile *> & inputFiles);

  void
  loadTriggerEff_1e_1tau_lepLeg_2016(vLutWrapperBase & effTrigger_1e1tau_lepLeg_data,
                                     vLutWrapperBase & effTrigger_1e1tau_lepLeg_mc,
                                     std::map<std::string, TFile *> & inputFiles);

  void
  loadTriggerEff_1m_2016(vLutWrapperBase & effTrigger_1m_data,
                         vLutWrapperBase & effTrigger_1m_mc,
                         std::map<std::string, TFile *> & inputFiles);

  void
  loadTriggerEff_1m_1tau_lepLeg_2016(vLutWrapperBase & effTrigger_1m1tau_lepLeg_data,
                                     vLutWrapperBase & effTrigger_1m1tau_lepLeg_mc,
                                     std::map<std::string, TFile *> & inputFiles);

  void 
  loadTriggerEff_1e_2017(vLutWrapperBase & effTrigger_1e_data,
                         vLutWrapperBase & effTrigger_1e_mc, 
                         std::map<std::string, TFile *> & inputFiles);

  void
  loadTriggerEff_1e_1tau_lepLeg_2017(vLutWrapperBase & effTrigger_1e1tau_lepLeg_data,
                                     vLutWrapperBase & effTrigger_1e1tau_lepLeg_mc,
                                     std::map<std::string, TFile *> & inputFiles);

  void 
  loadTriggerEff_1m_2017(vLutWrapperBase & effTrigger_1m_data,
                         vLutWrapperBase & effTrigger_1m_mc,
                         std::map<std::string, TFile *> & inputFiles);
 
  void
  loadTriggerEff_1m_1tau_lepLeg_2017(vLutWrapperBase & effTrigger_1m1tau_lepLeg_data,
                                     vLutWrapperBase & effTrigger_1m1tau_lepLeg_mc,
                                     std::map<std::string, TFile *> & inputFiles);

  void 
  loadTriggerEff_1e_2018(vLutWrapperBase & effTrigger_1e_data,
                         vLutWrapperBase & effTrigger_1e_mc,
                         std::map<std::string, TFile *> & inputFiles);

  void
  loadTriggerEff_1e_1tau_lepLeg_2018(vLutWrapperBase & effTrigger_1e1tau_lepLeg_data,
                                     vLutWrapperBase & effTrigger_1e1tau_lepLeg_mc,
                                     std::map<std::string, TFile *> & inputFiles);

  void 
  loadTriggerEff_1m_2018(vLutWrapperBase & effTrigger_1m_data,
                         vLutWrapperBase & effTrigger_1m_mc,
                         std::map<std::string, TFile *> & inputFiles);
 
  void
  loadTriggerEff_1m_1tau_lepLeg_2018(vLutWrapperBase & effTrigger_1m1tau_lepLeg_data,
                                     vLutWrapperBase & effTrigger_1m1tau_lepLeg_mc,
                                     std::map<std::string, TFile *> & inputFiles);

  double
  tau_leg_efficiency(const double pt, 
                     const int dm, 
                     const std::string trigger_type, 
                     const std::string wp, 
                     const std::string data_type, 
                     const correction::Correction::Ref cset,
                     const std::string & sys);

  TauTriggerSFValues
  tau_leg_efficiency(const double pt, 
                     const int dm, 
                     const std::string trigger_type, 
                     const std::string wp, 
                     const std::string data_type, 
                     const correction::Correction::Ref cset);

  int
  get_btv_flavor(int hadronFlav);

  bool
  is_relevant_shape_sys(int hadronFlavor,
                        int central_or_shift);
}

#endif // TallinnNtupleProducer_EvtWeightTools_data_to_MC_corrections_auxFunctions_h
