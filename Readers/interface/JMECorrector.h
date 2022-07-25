#ifndef TallinnNtupleProducer_Readers_JMECorrector_h
#define TallinnNtupleProducer_Readers_JMECorrector_h

#include "TallinnNtupleProducer/Objects/interface/RecoJetAK4.h"   // RecoJetAK4
#include "TallinnNtupleProducer/Objects/interface/CorrT1METJet.h" // CorrT1METJet
#include "TallinnNtupleProducer/Objects/interface/RecoJetAK8.h"   // RecoJetAK8
#include "TallinnNtupleProducer/Objects/interface/RecoMEt.h"      // RecoMEt

#include "FWCore/ParameterSet/interface/ParameterSet.h"         // edm::ParameterSet

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
  set_rho(double rho);

  void
  set_jet_opt(int central_or_shift);

  void
  set_met_opt(int central_or_shift);

  void
  set_fatJet_opt(int central_or_shift);

  void
  correct(RecoJetAK4 & jet,
          const std::vector<GenJet> & genJets);

  void
  correct(const CorrT1METJet & jet,
          const std::vector<GenJet> & genJets);

  void
  correct(const RecoJetAK8 & jet,
          const std::vector<Particle> & genJetsAK8,
          const std::vector<Particle> & genSubJetsAK8);

  void
  correct(RecoMEt & met,
          const GenMEt & rawmet,
          const EventInfo * const eventInfo,
          const RecoVertex * const recoVertex) const;

  // TODO set run/lumi/evt number for reproducible smearing

protected:
  bool isDEBUG_;
  Era era_;
  int jet_sys_;
  int met_sys_;
  int fatJet_sys_;
  bool enable_phiModulationCorr_;

  double rho_;
};

#endif // TallinnNtupleProducer_Readers_JMECorrector_h
