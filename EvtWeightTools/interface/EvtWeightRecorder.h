#ifndef TallinnNtupleProducer_EvtWeightTools_EvtWeightRecorder_h
#define TallinnNtupleProducer_EvtWeightTools_EvtWeightRecorder_h

#include "FWCore/ParameterSet/interface/ParameterSet.h"          // edm::VParameterSet

#include "TallinnNtupleProducer/Objects/interface/GenParticle.h" // GenParticle
#include "TallinnNtupleProducer/Objects/interface/LHEParticle.h"

// forward declarations
class L1PreFiringWeightReader;
class PSWeightReader;
class EventInfo;
class LHEInfoReader;
class LHEParticleReader;
class RecoJetAK4;
class Data_to_MC_CorrectionInterface_Base;
class HadTauFakeRateInterface;
class LeptonFakeRateInterface;
class RecoLepton;
class RecoHadTau;
class EvtWeightManager;
class DYMCReweighting;
class DYMCNormScaleFactors;
class Data_to_MC_CorrectionInterface_trigger_Base;
class HHWeightInterfaceLO;
class HHWeightInterfaceNLO;
class LHEVpt_LOtoNLO;
class SubjetBtagSFInterface;

enum class L1PreFiringWeightSys;
enum class PDFSys;
enum class PUsys;
enum class TriggerSFsys;
enum class LeptonIDSFsys;
enum class TauIDSFsys;
enum class FRet;
enum class FRmt;
enum class EWKJetSys;
enum class EWKBJetSys;
enum class pileupJetIDSFsys;
enum class LHEVptSys;
enum class SubjetBtagSys;

class EvtWeightRecorder
{
 public:
  EvtWeightRecorder();
  EvtWeightRecorder(const std::vector<std::string> & central_or_shifts,
                    const std::string & central_or_shift,
                    bool isMC);
  virtual ~EvtWeightRecorder() {}

  double
  get(const std::string & central_or_shift,
      const std::string & bin = "") const;

  virtual double
  get_inclusive(const std::string & central_or_shift,
                const std::string & bin = "") const;

  double
  get_genWeight() const;

  double
  get_hhWeight() const;

  double
  get_hhWeight_lo() const;

  double
  get_hhWeight_nlo() const;

  double
  get_rescaling() const;

  double
  get_auxWeight(const std::string & central_or_shift) const;

  double
  get_lumiScale(const std::string & central_or_shift,
                const std::string & bin = "") const;

  double
  get_prescaleWeight() const; 
  
  double
  get_nom_tH_weight(const std::string & central_or_shift) const;

  double
  get_puWeight(const std::string & central_or_shift) const;

  double
  get_pileupJetIDSF(const std::string & central_or_shift) const;

  double
  get_l1PreFiringWeight(const std::string & central_or_shift) const;

  double
  get_lheScaleWeight(const std::string & central_or_shift) const;

  double
  get_pdfWeight(const std::string & central_or_shift) const;

  double
  get_pdfMemberWeight(const std::string & central_or_shift) const;

  double
  get_psWeight(const std::string & central_or_shift) const;

  double
  get_leptonSF() const;

  double
  get_leptonIDSF_recoToLoose(const std::string & central_or_shift) const;

  double
  get_leptonIDSF_looseToTight(const std::string & central_or_shift) const;

  double
  get_leptonIDSF(const std::string & central_or_shift) const;

  double
  get_chargeMisIdProb() const;

  double
  get_dyBgrWeight() const; // CV: weight for data-driven Drell-Yan background estimation used in HH->bbWW dilepton channel

  double
  get_data_to_MC_correction(const std::string & central_or_shift) const;

  double
  get_btag(const std::string & central_or_shift) const;

  double
  get_ewk_jet(const std::string & central_or_shift) const;

  double
  get_ewk_bjet(const std::string & central_or_shift) const;

  double
  get_dy_rwgt(const std::string & central_or_shift) const;

  double
  get_dy_norm(const std::string & central_or_shift) const;

  double
  get_toppt_rwgt(const std::string & central_or_shift) const;

  double
  get_LHEVpt(const std::string & central_or_shift) const;

  double
  get_subjetBtagSF(const std::string & central_or_shift) const;
  
  double
  get_sf_triggerEff(const std::string & central_or_shift) const;

  virtual double
  get_tauSF(const std::string & central_or_shift) const;

  double
  get_FR(const std::string & central_or_shift) const;

  void
  record_genWeight(const EventInfo & eventInfo,
                   bool use_sign_only = false);

  void
  record_auxWeight(const EvtWeightManager * const evtWeightManager);

  void
  record_dy_rwgt(const DYMCReweighting * const dyReweighting,
                 const std::vector<GenParticle> & genParticles);

  void
  record_dy_norm(const DYMCNormScaleFactors * const dyNormScaleFactors,
                 const std::vector<GenParticle> & genParticles,
                 int nJets,
                 int nBLoose,
                 int nBMedium);

  void
  record_toppt_rwgt(double sf);
  
  void
  record_lumiScale(const edm::VParameterSet & lumiScales);

  void
  record_rescaling(double rescaling);

  void
  record_nom_tH_weight(const EventInfo * const eventInfo);

  void
  record_leptonSF(double weight);

  void
  record_leptonIDSF_recoToLoose(const Data_to_MC_CorrectionInterface_Base * const dataToMCcorrectionInterface);

  void
  record_leptonIDSF_looseToTight(const Data_to_MC_CorrectionInterface_Base * const dataToMCcorrectionInterface,
                                 bool woTightCharge = true);

  void
  record_chargeMisIdProb(double weight);

  // CV: weight for data-driven Drell-Yan background estimation used in HH->bbWW dilepton channel
  void
  record_dyBgrWeight(double weight); 

  void
  record_prescale(double weight);

  std::pair<std::vector<int>, std::vector<int>>
  findHiggs(const LHEParticleCollection & lheParticles, std::vector<int> & exclude) const;

  void
  record_gen_mHH_cosThetaStar(const LHEParticleCollection & lheParticles);

  // CV: weight specific to LO HH MC samples, computed by HHWeightInterfaceLO->getWeight("SM",...) method
  void
  record_hhWeight_lo(double weight);

  void
  record_hhWeight_lo(const HHWeightInterfaceLO * const HHWeightLO_calc,
                     bool isDEBUG = false);

  // CV: weight specific to LO HH MC samples, computed by HHWeightInterfaceNLO->getWeight("SM",...) method
  void
  record_hhWeight_nlo(double weight);

  void
  record_hhWeight_nlo(const HHWeightInterfaceNLO * const HHWeightNLO_calc,
                      bool isDEBUG = false);

  void
  record_l1PrefireWeight(const L1PreFiringWeightReader * const l1PreFiringWeightReader);

  void
  record_lheScaleWeight(const LHEInfoReader * const lheInfoReader);

  void
  record_pdfWeight(const LHEInfoReader * const lheInfoReader);

  void
  record_pdfMembers(const LHEInfoReader * const lheInfoReader,
                    const std::map<std::string, int> & pdf_map);

  void
  record_psWeight(const PSWeightReader * const psWeightReader);

  void
  record_puWeight(const Data_to_MC_CorrectionInterface_Base * const dataToMCcorrectionInterface,
                  const EventInfo * const eventInfo);

  void
  record_pileupJetIDSF(const Data_to_MC_CorrectionInterface_Base * const dataToMCcorrectionInterface,
                       const std::vector<const RecoJetAK4 *> & jets);

  void
  record_btagWeight(const Data_to_MC_CorrectionInterface_Base * const dataToMCcorrectionInterface,
                    const std::vector<const RecoJetAK4 *> & jets);

  void
  record_ewk_jet(const std::vector<const RecoJetAK4 *> & jets);

  void
  record_ewk_bjet(const std::vector<const RecoJetAK4 *> & bjets);

  void
  record_LHEVpt(const LHEVpt_LOtoNLO * const lhe_vpt);

  void
  record_subjetBtagSF(SubjetBtagSFInterface * const subjetBtagSFInterface);

  void
  record_leptonTriggerEff(const Data_to_MC_CorrectionInterface_Base * const dataToMCcorrectionInterface);

  void
  record_tauTriggerEff(const Data_to_MC_CorrectionInterface_trigger_Base * const dataToMCcorrectionInterface_trigger);

  void
  record_hadTauID_and_Iso(const Data_to_MC_CorrectionInterface_Base * const dataToMCcorrectionInterface);

  void
  record_eToTauFakeRate(const Data_to_MC_CorrectionInterface_Base * const dataToMCcorrectionInterface);

  void
  record_muToTauFakeRate(const Data_to_MC_CorrectionInterface_Base * const dataToMCcorrectionInterface);

  void
  record_jetToTauFakeRate(const HadTauFakeRateInterface * const hadTauFakeRateInterface,
                          const std::vector<const RecoHadTau *> & hadTaus);

  void
  record_jetToTauSF(const HadTauFakeRateInterface * const hadTauFakeRateInterface,
                    const std::vector<const RecoHadTau *> & hadTaus);

  void
  record_jetToLeptonFakeRate(const LeptonFakeRateInterface * const leptonFakeRateInterface,
                             const std::vector<const RecoLepton *> & leptons);

  void
  compute_FR();

  double
  gen_mHH() const;

  double
  gen_cosThetaStar() const;

  friend std::ostream &
  operator<<(std::ostream & os,
             const EvtWeightRecorder & evtWeightRecorder);

 protected:
  void
  record_jetToLepton_FR(const LeptonFakeRateInterface * const leptonFakeRateInterface,
                        const RecoLepton * const lepton,
                        std::map<int, double> & weights_FR_lepton);

  bool isMC_;
  double genWeight_;
  std::map<std::string, double> auxWeight_;
  std::map<std::string, std::map<std::string, double>> lumiScale_;
  std::map<std::string, double> nom_tH_weight_;
  double leptonSF_;
  double chargeMisIdProb_;
  double dyBgrWeight_;
  double prescale_;
  double hhWeight_lo_;
  double hhWeight_nlo_;
  double rescaling_;
  double gen_mHH_;
  double gen_cosThetaStar_;
  std::string central_or_shift_;
  std::vector<std::string> central_or_shifts_;

  std::map<L1PreFiringWeightSys, double> weights_l1PreFiring_;
  std::map<int, double> weights_lheScale_;
  std::map<PDFSys, double> weights_pdf_;
  std::map<std::string, double> weights_pdf_members_;
  std::map<int, double> weights_partonShower_;
  std::map<PUsys, double> weights_pu_;
  std::map<int, double> weights_dy_norm_;
  std::map<int, double> weights_dy_rwgt_;
  std::map<int, double> weights_toppt_rwgt_;
  std::map<LHEVptSys, double> weights_lhe_vpt_;
  std::map<SubjetBtagSys, double> weights_subjet_btag_;
  std::map<int, double> weights_btag_;
  std::map<pileupJetIDSFsys, double> weights_puJetIDSF_;
  std::map<TriggerSFsys, double> weights_leptonTriggerEff_;
  std::map<TriggerSFsys, double> weights_tauTriggerEff_;
  std::map<LeptonIDSFsys, double> weights_leptonID_and_Iso_recoToLoose_;
  std::map<LeptonIDSFsys, double> weights_leptonID_and_Iso_looseToTight_;
  std::map<TauIDSFsys, double> weights_hadTauID_and_Iso_;
  std::map<FRet, double> weights_eToTauFakeRate_;
  std::map<FRmt, double> weights_muToTauFakeRate_;
  std::map<int, double> weights_jetToTauFakeRate_;
  std::map<int, double> weights_jetToTauSF_;
  std::map<int, double> weights_jetToLeptonFakeRate_;
  std::map<std::string, double> weights_FR_;
  std::map<EWKJetSys, double> weights_ewk_jet_;
  std::map<EWKBJetSys, double> weights_ewk_bjet_;
};

#endif
