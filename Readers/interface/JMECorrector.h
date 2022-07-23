#ifndef TallinnNtupleProducer_Readers_JMECorrector_h
#define TallinnNtupleProducer_Readers_JMECorrector_h

#include "TallinnNtupleProducer/Objects/interface/RecoJetAK4.h" // RecoJetAK4

#include "FWCore/ParameterSet/interface/ParameterSet.h"         // edm::ParameterSet

class JMECorrector
{
public:
  JMECorrector(const edm::ParameterSet & cfg);
  ~JMECorrector();

  void
  set_rho(double rho);

  void
  reset(int central_or_shift);

  void
  correct(RecoJetAK4 & jet);

  // TODO set run/lumi/evt number for reproducible smearing

protected:
  bool isDEBUG_;
  int central_or_shift_;
  double rho_;
};

#endif // TallinnNtupleProducer_Readers_JMECorrector_h
