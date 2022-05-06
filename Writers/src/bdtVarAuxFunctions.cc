#include "TallinnNtupleProducer/Writers/interface/bdtVarAuxFunctions.h"

#include "DataFormats/Math/interface/deltaR.h" // deltaR()

#include <algorithm>                           // std::max(), std::min()
#include <cmath>                               // std::cos(), std::sin(), std::sqrt()

double 
comp_min_dR_jet(const Particle & particle, const std::vector<const RecoJetAK4 *> & jets_cleaned)
{
  double dRmin = 1.e+3;
  for ( auto jet : jets_cleaned )
  {
    const double dR = deltaR(particle.eta(), particle.phi(), jet->eta(), jet->phi());
    dRmin = std::min(dR, dRmin);
  }
  return dRmin;
}

namespace
{
  double 
  square(double x)
  {
    return x*x;
  }

  double
  comp_mT(const Particle::LorentzVector & leptonP4, double met_pt, double met_phi)
  {
    const double met_px = met_pt*std::cos(met_phi);
    const double met_py = met_pt*std::sin(met_phi);
    const double mT = std::sqrt(
      std::max(
        0.,
        square(leptonP4.Et() + met_pt) - (square(leptonP4.px() + met_px) + square(leptonP4.py() + met_py)))
    );
    return mT;
  }
}

double
comp_mT(const RecoLepton * lepton, const RecoMEt * met)
{
  return comp_mT(lepton->cone_p4(), met->pt(), met->phi());
}

double
comp_mT(const RecoHadTau * hadTau, const RecoMEt * met)
{
  return comp_mT(hadTau->p4(), met->pt(), met->phi());
}


