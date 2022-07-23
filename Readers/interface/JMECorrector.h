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
  reset(int central_or_shift);

  void
  correct(RecoJetAK4 & jet);

  // TODO set run/lumi/evt number for reproducible smearing

protected:
  int central_or_shift_;
};

#endif // TallinnNtupleProducer_Readers_JMECorrector_h
