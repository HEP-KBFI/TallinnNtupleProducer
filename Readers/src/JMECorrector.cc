#include "TallinnNtupleProducer/Readers/interface/JMECorrector.h"

#include "TallinnNtupleProducer/CommonTools/interface/sysUncertOptions.h"

JMECorrector::JMECorrector(const edm::ParameterSet & cfg)
  : central_or_shift_(kJetMET_central)
{
  // TODO read the following options from cfg: apply JEC, apply JER, isMC
}

JMECorrector::~JMECorrector()
{}

void
JMECorrector::reset(int central_or_shift)
{
  central_or_shift_ = central_or_shift;
}

void
JMECorrector::correct(RecoJetAK4 & jet)
{
  //
}
