#ifndef TallinnNtupleProducer_EvtWeightTools_data_to_MC_corrections_auxFunctions_h
#define TallinnNtupleProducer_EvtWeightTools_data_to_MC_corrections_auxFunctions_h

#include "TallinnNtupleProducer/EvtWeightTools/interface/lutAuxFunctions.h"    // lutWrapperBase, vLutWrapperBase
#include "TallinnNtupleProducer/EvtWeightTools/interface/TauTriggerSFValues.h" // TauTriggerSFValues

#include <map>                                                                 // std::map
#include <string>                                                              // std::string
#include <vector>                                                              // std::vector

// forward declarations
class TFile;
class TauTriggerSFs2017;

enum class TriggerSFsys;

typedef double (TauTriggerSFs2017::*getTriggerEfficiencyFunc)(double, double, double, int) const;

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
}

#endif // TallinnNtupleProducer_EvtWeightTools_data_to_MC_corrections_auxFunctions_h
