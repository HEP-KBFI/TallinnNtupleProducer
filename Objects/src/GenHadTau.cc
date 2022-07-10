#include "TallinnNtupleProducer/Objects/interface/GenHadTau.h" // GenHadTau, GenParticleBase

GenHadTau::GenHadTau(Double_t pt,
                     Double_t eta,
                     Double_t phi,
                     Double_t mass,
                     Int_t charge,
                     Int_t status)
  : GenParticle(pt, eta, phi, mass, -15 * charge, status, -1)
{}

std::ostream &
operator<<(std::ostream & stream,
           const GenHadTau & hadTau)
{
  stream << static_cast<const Particle &>(hadTau) << ","
            " charge = " << hadTau.charge()       << ","
            " status = " << hadTau.status()
  ;
  return stream;
}
