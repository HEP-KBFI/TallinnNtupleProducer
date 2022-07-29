#ifndef TallinnNtupleProducer_Readers_JMECorrector_h
#define TallinnNtupleProducer_Readers_JMECorrector_h

#include "TallinnNtupleProducer/CommonTools/interface/as_integer.h" // EnumClassHash
#include "TallinnNtupleProducer/Objects/interface/RecoJetAK4.h"     // RecoJetAK4
#include "TallinnNtupleProducer/Objects/interface/CorrT1METJet.h"   // CorrT1METJet
#include "TallinnNtupleProducer/Objects/interface/RecoJetAK8.h"     // RecoJetAK8
#include "TallinnNtupleProducer/Objects/interface/RecoMEt.h"        // RecoMEt

#include "correction.h"                                             // correction::CorrectionSet, correction::Correction::Ref

#include "FWCore/ParameterSet/interface/ParameterSet.h"             // edm::ParameterSet

#include <random>                                                   // std::random_device, std::mt19937

// forward declarations
class EventInfo;
class RecoVertex;

enum class Era;

class JMECorrector
{
public:
  JMECorrector(const edm::ParameterSet & cfg);
  ~JMECorrector();

  correction::CorrectionSet *
  get_JMARcset() const;

  void
  set_info(const EventInfo * info);

  void
  reset();

  void
  set_opt(int jet_opt,
          int met_opt,
          int fatJet_opt);

  void
  correct(RecoJetAK4 & jet,
          const std::vector<const GenJet *> & genJets,
          bool force_recalibration = false);

  void
  correct(const CorrT1METJet & jet,
          const std::vector<const GenJet *> & genJets,
          const std::vector<const RecoJetAK4 *> & recoJets);

  void
  correct(RecoJetAK8 & jet,
          const std::vector<const Particle *> & genJetsAK8,
          const std::vector<const Particle *> & genSubJetsAK8) const;

  void
  correct(RecoMEt & met,
          const GenMEt & rawmet,
          const RecoVertex * const recoVertex) const;

protected:
  enum class JetAlgo {
    AK4,
    AK8,
  };

  struct JetParams
  {
    JetParams() = default;
    JetParams(const RecoJetAK4 & jet);
    JetParams(const RecoJetAK8 & jet);

    double pt;
    double eta;
    double area;
    double rawFactor;
    JetAlgo algo;
  };

  double
  calibrate(const JetParams & jetParams,
            bool include_residual,
            int max_level) const;

  double
  jec_unc(double jet_pt,
          double jet_eta,
          double jet_phi,
          int jet_id,
          JetAlgo jet_algo) const;

  double
  smear_pt(const Particle::LorentzVector & jet,
           const Particle::LorentzVector & genJet,
           JetAlgo jet_algo) const;

  double
  smear_mass(const Particle::LorentzVector & jet,
             const Particle::LorentzVector & genJet) const;

  double
  smear_stochastic(double jet_eta,
                   double sigma,
                   double sf) const;

  double
  jms(double jet_pt,
      double jet_eta) const;

  bool isDEBUG_;
  bool isMC_;
  bool reapply_JEC_;
  bool apply_smearing_;
  Era era_;
  std::string era_str_;
  std::string globalJECTag_;
  std::string globalJERTag_;
  std::string ak4_jetType_;
  std::string ak8_jetType_;

  int jet_sys_;
  int met_sys_;
  int fatJet_sys_;
  int fatJet_corr_;
  bool enable_phiModulationCorr_;

  double rho_;
  unsigned rle_;
  const EventInfo * info_;
  std::vector<double> met_T1Smear_px_;
  std::vector<double> met_T1Smear_py_;

  mutable std::mt19937 generator_;
  bool use_deterministic_seed_;

  std::unique_ptr<correction::CorrectionSet> jmar_cset_;
  std::unique_ptr<correction::CorrectionSet> jet_jerc_cset_;
  std::unique_ptr<correction::CorrectionSet> fatJet_jerc_cset_;

  std::map<int, correction::Correction::Ref> jmar_sf_;
  std::map<int, double> jmr_sf_;
  std::map<bool, std::vector<double>> jmr_sd_reso_;
  std::map<bool, std::vector<double>> jmr_sd_corr_;
  std::map<JetAlgo, std::vector<correction::Correction::Ref>> jec_compound_;
  std::map<JetAlgo, std::map<int, correction::Correction::Ref>> jec_uncs_;
  std::map<JetAlgo, correction::Correction::Ref> jet_reso_;
  std::map<JetAlgo, correction::Correction::Ref> jet_jer_sf_;
};

#endif // TallinnNtupleProducer_Readers_JMECorrector_h
