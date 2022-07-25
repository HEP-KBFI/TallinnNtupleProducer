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
JMECorrector::correct(RecoJetAK4 & jet,
                      const std::vector<GenJet> & genJets)
{
  //
}

void
JMECorrector::correct(const CorrT1METJet & jet,
                      const std::vector<GenJet> & genJets)
{
  //
}
