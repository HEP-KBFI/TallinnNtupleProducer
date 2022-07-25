#include "TallinnNtupleProducer/Readers/interface/JMECorrector.h"

#include "TallinnNtupleProducer/CommonTools/interface/Era.h"              // get_era()
#include "TallinnNtupleProducer/CommonTools/interface/sysUncertOptions.h" // kJetMET_*
#include "TallinnNtupleProducer/Readers/interface/metPhiModulation.h"     // METXYCorr_Met_MetPhi
#include "TallinnNtupleProducer/CommonTools/interface/cmsException.h"     // get_human_line()

#define _USE_MATH_DEFINES // M_PI

namespace
{
  double square(double x)
  {
    return x*x;
  }
}

JMECorrector::JMECorrector(const edm::ParameterSet & cfg)
  : isDEBUG_(cfg.getParameter<bool>("isDEBUG"))
  , era_(get_era(cfg.getParameter<std::string>("era")))
  , jet_sys_(kJetMET_central)
  , met_sys_(kJetMET_central)
  , fatJet_sys_(kFatJet_central)
  , enable_phiModulationCorr_(cfg.getParameter<bool>("enable_phiModulationCorr"))
  , rho_(0.)
{
  // TODO read the following options from cfg: apply JEC, apply JER, isMC, disable_ak8_corr
}

JMECorrector::~JMECorrector()
{}

void
JMECorrector::set_rho(double rho)
{
  rho_ = rho;
}

void
JMECorrector::set_jet_opt(int central_or_shift)
{
  jet_sys_ = central_or_shift;
}

void
JMECorrector::set_met_opt(int central_or_shift)
{
  met_sys_ = central_or_shift;
}

void
JMECorrector::set_fatJet_opt(int central_or_shift)
{
  fatJet_sys_ = central_or_shift;
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

void
JMECorrector::correct(const RecoJetAK8 & jet,
                      const std::vector<GenJet> & genJets)
{
  //
}

void
JMECorrector::correct(RecoMEt & met,
                      const GenMEt & rawmet,
                      const EventInfo * const eventInfo,
                      const RecoVertex * const recoVertex) const
{
  if(enable_phiModulationCorr_)
  {
    double met_pt = met.pt();
    double met_phi = met.phi();

    const std::pair<double, double> met_pxpyCorr = METXYCorr_Met_MetPhi(eventInfo, recoVertex, era_);
    double met_px = met_pt * std::cos(met_phi) + met_pxpyCorr.first;
    double met_py = met_pt * std::sin(met_phi) + met_pxpyCorr.second;

    met_pt = std::sqrt(square(met_px) + square(met_py));
    if     (met_px > 0) { met_phi = std::atan(met_py / met_px); }
    else if(met_px < 0) { met_phi = std::atan(met_py / met_px) + ((met_py > 0. ? +1. : -1.) * M_PI);  }
    else                { met_phi = (met_py > 0. ? +1. : -1.) * M_PI; }

    met.set(met_pt, met_phi);
  }
}
