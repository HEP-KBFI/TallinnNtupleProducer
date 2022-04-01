#include "TallinnNtupleProducer/Objects/interface/GenPhoton.h"

GenPhoton::GenPhoton(Double_t pt,
                     Double_t eta,
                     Double_t phi,
                     Double_t mass,
                     Int_t pdgId,
                     Int_t status,
                     Int_t statusFlags,
                     UChar_t genPartFlav)
  : GenParticle(pt, eta, phi, mass, pdgId, 0, status, statusFlags, genPartFlav)
{}

GenPhoton::GenPhoton(const GenParticle & genParticle)
  : GenParticle(genParticle)
{}

std::ostream &
operator<<(std::ostream & stream,
           const GenPhoton & genPhoton)
{
  stream << static_cast<const GenParticle &>(genPhoton);
  return stream;
}
