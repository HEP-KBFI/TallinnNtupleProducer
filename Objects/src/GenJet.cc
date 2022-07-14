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
  : GenParticle(pt, eta, phi, mass, 0, -1, -1, -1)
  , partonFlav_(partonFlavour)
  , hadronFlav_(hadronFlavour)
{}

GenJet::GenJet(const GenParticle & genParticle)
  : GenParticle(genParticle)
{}

Int_t
GenJet::partonFlav() const
{
  return partonFlav_;
}

UChar_t
GenJet::hadronFlav() const
{
  return hadronFlav_;
}

std::ostream &
operator<<(std::ostream & stream,
           const GenJet & genJet)
{
  stream << static_cast<const GenParticle &>(genJet) << ","
            " partonFlav = " << genJet.partonFlav()  << ","
            " hadronFlav = " << static_cast<int>(genJet.hadronFlav())
  ;
  return stream;
}
