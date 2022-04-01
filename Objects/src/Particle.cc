#include "TallinnNtupleProducer/Objects/interface/Particle.h"

#include "DataFormats/Math/interface/deltaR.h" // deltaR()

#include <cmath>                               // std::fabs()

Particle::Particle()
  : Particle(0., 0., 0., 0.)
{}

Particle::Particle(Double_t pt,
                   Double_t eta,
                   Double_t phi,
                   Double_t mass)
  : pt_(pt)
  , eta_(eta)
  , phi_(phi)
  , mass_(mass)
  , absEta_(std::fabs(eta_))
  , p4_{pt_, eta_, phi_, mass_}
  , isValid_(pt_ > 0.)
{}

Particle::Particle(const Particle::LorentzVector & p4)
  : pt_(p4.pt())
  , eta_(p4.eta())
  , phi_(p4.phi())
  , mass_(p4.mass())
  , absEta_(std::fabs(eta_))
  , p4_(p4)
  , isValid_(true)
{}

Double_t
Particle::pt() const
{
  return pt_;
}

Double_t
Particle::eta() const
{
  return eta_;
}

Double_t
Particle::phi() const
{
  return phi_;
}

Double_t
Particle::mass() const
{
  return mass_;
}

Double_t
Particle::absEta() const
{
  return absEta_;
}

const Particle::LorentzVector &
Particle::p4() const
{
  return p4_;
}

double
Particle::deltaR(const Particle & particle) const
{
  return this->deltaR(&particle);
}

double
Particle::deltaR(const Particle * const particle) const
{
  return ::deltaR(eta_, phi_, particle->eta(), particle->phi());
}

bool
Particle::isValid() const
{
  return isValid_;
}

void
Particle::set_p4(const Particle::LorentzVector & p4)
{
  return set_ptEtaPhiMass(p4.pt(), p4.eta(), p4.phi(), p4.mass());
}

void
Particle::set_ptEtaPhiMass(Double_t pt,
                           Double_t eta,
                           Double_t phi,
                           Double_t mass)
{
  pt_ = pt;
  eta_ = eta;
  phi_ = phi;
  mass_ = mass;
  absEta_ = std::fabs(eta_);
  p4_ = { pt_, eta_, phi_, mass_ };
  isValid_ = pt_ > 0.;
}

std::ostream &
operator<<(std::ostream & stream,
           const Particle & particle)
{
  stream << " pT = "   << particle.pt()          << ","
            " eta = "  << particle.eta()         << ","
            " phi = "  << particle.phi()         << ","
            " mass = " << particle.mass()        << ","
            " E = "    << particle.p4().energy() << ","
            " |p| = "  << particle.p4().P();
  return stream;
}
