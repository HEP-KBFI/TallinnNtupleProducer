#include "TallinnNtupleProducer/CommonTools/interface/deltaR.h"
#include "TMath.h"

double
deltaR(double eta1, double eta2, double phi1, double phi2)
{
  return TMath::Sqrt( pow( (eta1 - eta2), 2 ) + pow( (phi1 - phi2), 2 ) );
}
