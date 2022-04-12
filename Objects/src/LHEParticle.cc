#include "TallinnNtupleProducer/Objects/interface/LHEParticle.h" // LHEParticle

LHEParticle::LHEParticle()
  : LHEParticle(0., 0., 0., 0., 0, -1)
{}

LHEParticle::LHEParticle(const LHEParticle & genParticle)
  : LHEParticle(
      genParticle.pt_
    , genParticle.eta_
    , genParticle.phi_
    , genParticle.mass_
    , genParticle.pdgId_
    , genParticle.status_
  )
{}

LHEParticle::LHEParticle(Double_t pt,
                         Double_t eta,
                         Double_t phi,
                         Double_t mass,
                         Int_t pdgId,
                         Int_t status)
  : Particle(pt, eta, phi, mass)
  , pdgId_(pdgId)
  , status_(status)
{}

LHEParticle::LHEParticle(const math::PtEtaPhiMLorentzVector & p4,
                         Int_t pdgId,
                         Int_t status)
  : Particle(p4)
  , pdgId_(pdgId)
  , status_(status)
{}


Int_t
LHEParticle::pdgId() const
{
  return pdgId_;
}
Int_t
LHEParticle::status() const
{
  return status_;
}

std::ostream &
operator<<(std::ostream & stream,
           const LHEParticle & particle)
{
  stream << static_cast<const Particle &>(particle) << ","
            " pdgId = "  << particle.pdgId()               << ","
            " status = " << particle.status()
  ;
  return stream;
}
