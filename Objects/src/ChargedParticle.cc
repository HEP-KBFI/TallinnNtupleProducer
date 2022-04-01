#include "TallinnNtupleProducer/Objects/interface/ChargedParticle.h" // ChargedParticle, Particle

ChargedParticle::ChargedParticle()
  : ChargedParticle(0., 0., 0., 0., 0, 0)
{}

ChargedParticle::ChargedParticle(Double_t pt,
                         Double_t eta,
                         Double_t phi,
                         Double_t mass,
                         Int_t pdgId,
                         Int_t charge)
  : Particle(pt, eta, phi, mass)
  , pdgId_(pdgId)
  , charge_(charge)
{}

ChargedParticle::ChargedParticle(const math::PtEtaPhiMLorentzVector & p4,
                         Int_t pdgId,
                         Int_t charge)
  : Particle(p4)
  , pdgId_(pdgId)
  , charge_(charge)
{}

Int_t
ChargedParticle::pdgId() const
{
  return pdgId_;
}

Int_t
ChargedParticle::charge() const
{
  return charge_;
}

std::ostream &
operator<<(std::ostream & stream,
           const ChargedParticle & particle)
{
  stream << static_cast<const Particle &>(particle) << ","
            " pdgId = "  << particle.pdgId()        << ","
            " charge = " << particle.charge()
  ;
  return stream;
}
