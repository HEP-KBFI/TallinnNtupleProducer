#include "TallinnNtupleProducer/Readers/interface/JMECorrector.h"

#include "TallinnNtupleProducer/CommonTools/interface/sysUncertOptions.h"

JMECorrector::JMECorrector(const edm::ParameterSet & cfg)
  : isDEBUG_(cfg.getParameter<bool>("isDEBUG"))
  , rho_(0.)
  , central_or_shift_(kJetMET_central)
{
  // TODO read the following options from cfg: apply JEC, apply JER, isMC
}

JMECorrector::~JMECorrector()
{}

void
JMECorrector::set_rho(double rho)
{
  rho_ = rho;
}

void
JMECorrector::reset(int central_or_shift)
{
  central_or_shift_ = central_or_shift;
}

void
JMECorrector::correct(RecoJetAK4 & jet)
{
  const double pt = jet.pt();
  const double mass = jet.mass();
  const double rawFactor = jet.rawFactor();

  const double rawpt = pt * (1 - rawFactor);
  const double rawmass = mass * (1 - rawFactor);
  
  //TODO implement jet calibrator
}
