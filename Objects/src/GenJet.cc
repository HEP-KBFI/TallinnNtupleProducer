#include "TallinnNtupleProducer/Objects/interface/GenJet.h"

GenJet::GenJet(Double_t pt,
               Double_t eta,
               Double_t phi,
               Double_t mass)
  : GenJet(pt, eta, phi, mass, 0, 0)
{}

GenJet::GenJet(Double_t pt,
               Double_t eta,
               Double_t phi,
               Double_t mass,
               Int_t partonFlavour,
               Int_t hadronFlavour)
  : GenParticle(pt, eta, phi, mass, -1, 0, -1, -1, partonFlavour)
{}

GenJet::GenJet(const GenParticle & genParticle)
  : GenParticle(genParticle)
{}

std::ostream &
operator<<(std::ostream & stream,
           const GenJet & genJet)
{
  stream << static_cast<const GenParticle &>(genJet);
  return stream;
}

UChar_t
GenJet::genHadronFlav() const
{
  return genHadronFlav_;
}
