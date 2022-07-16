#ifndef TallinnNtupleProducer_EvtWeightTools_Data_to_MC_CorrectionInterface_Base_h
#define TallinnNtupleProducer_EvtWeightTools_Data_to_MC_CorrectionInterface_Base_h

#include "FWCore/ParameterSet/interface/ParameterSet.h"                   // edm::ParameterSet

#include "TallinnNtupleProducer/CommonTools/interface/sysUncertOptions.h" // FRet, FRmt

#include <string>                                                         // std::string
#include <vector>                                                         // std::vector
#include "correction.h"

// forward declarations
class lutWrapperBase;
class RecoHadTau;
class RecoLepton;
class RecoJetAK4;
class TauIDSFTool;
class TFile;

enum class Era;
enum class pileupJetID;
enum class pileupJetIDSFsys;
enum class TauID;

class Data_to_MC_CorrectionInterface_Base
{
 public:
  Data_to_MC_CorrectionInterface_Base(Era era, const edm::ParameterSet & cfg);
  virtual ~Data_to_MC_CorrectionInterface_Base();

  //-----------------------------------------------------------------------------
  // overwrite configuration parameters (needed by analyze_jetToTauFakeRate.cc)
  void
  setHadTauSelection(const std::string & hadTauSelection);

  void
  set_hadTauID_and_Iso_cset(correction::Correction::Ref cset);

  void
  set_eToTauFakeRate_cset(correction::Correction::Ref cset);

  void
  set_muToTauFakeRate_cset(correction::Correction::Ref cset);

  //-----------------------------------------------------------------------------

  //-----------------------------------------------------------------------------
  // set leptons, taus, and jets
  // (to be called once per event, before calling any of the getSF.. functions)
  void
  setLeptons(const std::vector<const RecoLepton *> & leptons,
             bool requireChargeMatch = false);

  void
  setHadTaus(const std::vector<const RecoHadTau *> & hadTaus);

  void
  setJets(const std::vector<const RecoJetAK4 *> & jets);
  //-----------------------------------------------------------------------------

  //-----------------------------------------------------------------------------
  // data/MC correction for electron and muon trigger efficiency
  virtual double
  getSF_leptonTriggerEff(TriggerSFsys central_or_shift) const;
  //-----------------------------------------------------------------------------

  //-----------------------------------------------------------------------------
  // data/MC corrections for electron and muon identification and isolation efficiency,
  // including the cut on the ttH multilepton MVA
  double
  getSF_leptonID_and_Iso_loose(LeptonIDSFsys central_or_shift) const;

  double
  getSF_leptonID_and_Iso_looseToFakeable() const;

  double
  getSF_leptonID_and_Iso_tight_to_loose_woTightCharge(LeptonIDSFsys central_or_shift) const;

  double
  getSF_leptonID_and_Iso_tight_to_loose_wTightCharge(LeptonIDSFsys central_or_shift) const;
  //-----------------------------------------------------------------------------

  //-----------------------------------------------------------------------------
  // data/MC corrections for hadronic tau identification efficiency,
  // and for e->tau and mu->tau misidentification rates
  virtual double
  getSF_hadTauID_and_Iso(TauIDSFsys central_or_shift) const;

  virtual double
  getSF_eToTauFakeRate(FRet central_or_shift) const;

  virtual double
  getSF_muToTauFakeRate(FRmt central_or_shift) const;
  //-----------------------------------------------------------------------------

  //-----------------------------------------------------------------------------
  // data/MC corrections for jets to pass pileup jet ID
  double
  getSF_pileupJetID(pileupJetIDSFsys central_or_shift) const;
  //-----------------------------------------------------------------------------

 protected:
  double
  getSF_leptonID_and_Iso(std::size_t numLeptons,
                         const std::vector<double> & lepton_pt,
                         const std::vector<double> & lepton_eta,
                         const std::vector<bool> & lepton_isGenMatched,
                         const std::vector<bool> & lepton_isTight,
                         bool sfForTightSelection,
                         const std::vector<lutWrapperBase *> & corrections,
                         int error_shift,
                         double recompSF) const;
  bool
  check_triggerSFsys_opt(TriggerSFsys central_or_shift) const;

  double
  comp_triggerSFsys_opt(double sf,
                        double sfErr,
                        TriggerSFsys central_or_shift) const;

  //-----------------------------------------------------------------------------
  // data/MC corrections for electron and muon identification and isolation efficiency,
  // including the cut on the ttH multilepton MVA

  // loose electron selection (RecoElectronSelectorLoose)
  std::vector<lutWrapperBase *> sfElectronID_and_Iso_loose_;
  // tight electron selection used in all channels except 2lss_1tau (RecoElectronSelectorTight with tightCharge_cut disabled)
  std::vector<lutWrapperBase *> sfElectronID_and_Iso_tight_to_loose_woTightCharge_;
  // tight electron selection specific to 2lss_1tau channel (RecoElectronSelectorTight with tightCharge_cut enabled)
  std::vector<lutWrapperBase *> sfElectronID_and_Iso_tight_to_loose_wTightCharge_;
  // errors for the above
  std::vector<lutWrapperBase *> sfElectronID_and_Iso_tight_to_loose_errors_up_;
  std::vector<lutWrapperBase *> sfElectronID_and_Iso_tight_to_loose_errors_down_;

  // loose muon selection (RecoMuonSelectorLoose)
  std::vector<lutWrapperBase *> sfMuonID_and_Iso_loose_;
  // tight muon selection used in all channels except 2lss_1tau (RecoMuonSelectorTight with tightCharge_cut disabled)
  std::vector<lutWrapperBase *> sfMuonID_and_Iso_tight_to_loose_woTightCharge_;
  // tight muon selection specific to 2lss_1tau channel (RecoMuonSelectorTight with tightCharge_cut enabled)
  std::vector<lutWrapperBase *> sfMuonID_and_Iso_tight_to_loose_wTightCharge_;
  // errors for the above
  std::vector<lutWrapperBase *> sfMuonID_and_Iso_tight_to_loose_errors_up_;
  std::vector<lutWrapperBase *> sfMuonID_and_Iso_tight_to_loose_errors_down_;
  //-----------------------------------------------------------------------------

  //-----------------------------------------------------------------------------
  // data/MC corrections for pileup jet ID
  lutWrapperBase * effPileupJetID_;
  lutWrapperBase * sfPileupJetID_eff_;
  lutWrapperBase * sfPileupJetID_eff_errors_;
  lutWrapperBase * mistagPileupJetID_;
  lutWrapperBase * sfPileupJetID_mistag_;
  lutWrapperBase * sfPileupJetID_mistag_errors_;
  //-----------------------------------------------------------------------------

  Era era_;

  std::map<std::string, TFile *> inputFiles_;

  int hadTauSelection_;
  TauID hadTauId_;
  std::string tauIDSF_str_;
  std::string tauIDSF_level_str_;

  int hadTauSelection_antiElectron_[4];
  int hadTauSelection_antiMuon_[4];

  correction::Correction::Ref hadTauID_and_Iso_cset_;
  correction::Correction::Ref eToTauFakeRate_cset_;
  correction::Correction::Ref muToTauFakeRate_cset_;

  bool applyHadTauSF_;
  bool isDEBUG_;

  pileupJetID pileupJetId_;

  bool recompTightSF_;
  double recompTightSF_el_woTightCharge_;
  double recompTightSF_mu_woTightCharge_;
  double recompTightSF_el_wTightCharge_;
  double recompTightSF_mu_wTightCharge_;

  std::size_t numLeptons_;
  std::vector<int> lepton_type_;
  std::vector<double> lepton_pt_;
  std::vector<double> lepton_cone_pt_;
  std::vector<double> lepton_eta_;
  std::size_t numElectrons_;
  std::vector<double> electron_pt_;
  std::vector<double> electron_cone_pt_;
  std::vector<double> electron_eta_;
  std::vector<bool> electron_isGenMatched_;
  std::vector<bool> electron_isTight_;
  std::size_t numMuons_;
  std::vector<double> muon_pt_;
  std::vector<double> muon_cone_pt_;
  std::vector<double> muon_eta_;
  std::vector<bool> muon_isGenMatched_;
  std::vector<bool> muon_isTight_;
  std::size_t numHadTaus_;
  std::vector<int> hadTau_genPdgId_;
  std::vector<double> hadTau_pt_;
  std::vector<double> hadTau_absEta_;
  std::size_t numJets_;
  std::vector<double> jet_pt_;
  std::vector<double> jet_eta_;
  std::vector<bool> jet_isPileup_;
  std::vector<bool> jet_passesPileupJetId_;
};

#endif // TallinnNtupleProducer_EvtWeightTools_Data_to_MC_CorrectionInterface_Base_h
