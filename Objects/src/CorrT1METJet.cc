#include "TallinnNtupleProducer/Objects/interface/CorrT1METJet.h"

CorrT1METJet::CorrT1METJet(Double_t rawPt,
                           Double_t eta,
                           Double_t phi,
                           Double_t area,
                           Double_t muonSubtrFactor)
  : Particle(rawPt, eta, phi, 0.)
  , area_(area)
  , muonSubtrFactor_(muonSubtrFactor)
{}

Double_t
CorrT1METJet::area() const
{
  return area_;
}

Double_t
CorrT1METJet::muonSubtrFactor() const
{
  return muonSubtrFactor_;
}

std::ostream &
operator<<(std::ostream & stream,
           const CorrT1METJet & jet)
{
  stream << static_cast<const Particle &>(jet)             << ","
            " area = "            << jet.area()            << ","
            " muonSubtrFactor = " << jet.muonSubtrFactor()
  ;
  return stream;
}
