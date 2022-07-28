#ifndef TallinnNtupleProducer_Readers_JMECorrector_h
#define TallinnNtupleProducer_Readers_JMECorrector_h

#include "TallinnNtupleProducer/Objects/interface/RecoJetAK4.h"   // RecoJetAK4
#include "TallinnNtupleProducer/Objects/interface/CorrT1METJet.h" // CorrT1METJet
#include "TallinnNtupleProducer/Objects/interface/RecoJetAK8.h"   // RecoJetAK8
#include "TallinnNtupleProducer/Objects/interface/RecoMEt.h"      // RecoMEt

#include "correction.h"                                           // correction::CorrectionSet, correction::Correction::Ref

#include "FWCore/ParameterSet/interface/ParameterSet.h"           // edm::ParameterSet

#include <random>                                                 // std::random_device, std::mt19937

// forward declarations
class EventInfo;
class RecoVertex;

enum class Era;

class JMECorrector
{
public:
  JMECorrector(const edm::ParameterSet & cfg);
  ~JMECorrector();

  void
  set_info(const EventInfo * info);

  void
  set_jet_opt(int central_or_shift);

  void
  set_met_opt(int central_or_shift);

  void
  set_fatJet_opt(int central_or_shift);

  void
  correct(RecoJetAK4 & jet,
          const std::vector<const GenJet *> & genJets);

  void
  correct(const CorrT1METJet & jet,
          const std::vector<const GenJet *> & genJets);

  void
  correct(RecoJetAK8 & jet,
          const std::vector<const Particle *> & genJetsAK8,
          const std::vector<const Particle *> & genSubJetsAK8) const;

  void
  correct(RecoMEt & met,
          const GenMEt & rawmet,
          const RecoVertex * const recoVertex) const;

  // TODO set run/lumi/evt number for reproducible smearing

protected:
  struct JetParams
  {
    JetParams() = default;
    JetParams(const RecoJetAK4 & jet);

    double pt;
    double eta;
    double phi;
    double mass;
    double area;
    double rawFactor;
  };

  static double
  kahan_sum(const std::vector<double> & input);

  double
  calibrate(const JetParams & jetParams,
            bool include_residual,
            int max_level) const;

  double
  jec_unc(double jet_pt,
          double jet_eta,
          double jet_phi,
          int jet_id) const;

  double
  smear(const Particle::LorentzVector & jet,
        const Particle::LorentzVector & genJet);

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
  bool enable_phiModulationCorr_;

  double rho_;
  unsigned rle_;
  const EventInfo * info_;
  std::vector<double> delta_x_T1Jet_;
  std::vector<double> delta_y_T1Jet_;
  std::vector<double> delta_x_rawJet_;
  std::vector<double> delta_y_rawJet_;
  std::vector<double> met_T1Smear_px_;
  std::vector<double> met_T1Smear_py_;

  std::mt19937 generator_;
  bool use_deterministic_seed_;

  std::unique_ptr<correction::CorrectionSet> jet_cset_;
  std::vector<correction::Correction::Ref> jet_compound_;
  std::map<int, correction::Correction::Ref> jet_uncs_;
  correction::Correction::Ref jet_reso_;
  correction::Correction::Ref jet_jer_sf_;

  std::unique_ptr<correction::CorrectionSet> fatJet_cset_;
  std::unique_ptr<correction::CorrectionSet> jmar_cset_;
};

#endif // TallinnNtupleProducer_Readers_JMECorrector_h
