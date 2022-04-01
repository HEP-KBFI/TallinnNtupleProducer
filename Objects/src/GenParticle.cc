#include "TallinnNtupleProducer/Objects/interface/GenParticle.h" // GenParticle, Particle

GenParticle::GenParticle()
  : GenParticle(0., 0., 0., 0., 0, 0, -1, -1, 0)
{}

GenParticle::GenParticle(const GenParticle & genParticle)
  : GenParticle(
      genParticle.pt_
    , genParticle.eta_
    , genParticle.phi_
    , genParticle.mass_
    , genParticle.pdgId_
    , genParticle.charge_
    , genParticle.status_
    , genParticle.statusFlags_
    , genParticle.genPartFlav_
  )
{}

GenParticle::GenParticle(Double_t pt,
                         Double_t eta,
                         Double_t phi,
                         Double_t mass,
                         Int_t pdgId,
                         Int_t charge,
                         Int_t status,
                         Int_t statusFlags,
                         UChar_t genPartFlav)
  : ChargedParticle(pt, eta, phi, mass, pdgId, charge)
  , status_(status)
  , statusFlags_(statusFlags)
  , genPartFlav_(genPartFlav)
  , isMatchedToReco_(false)
{}

GenParticle::GenParticle(const math::PtEtaPhiMLorentzVector & p4,
                         Int_t pdgId,
                         Int_t charge,
                         Int_t status,
                         Int_t statusFlags,
                         UChar_t genPartFlav)
  : ChargedParticle(p4, pdgId, charge)
  , status_(status)
  , statusFlags_(statusFlags)
  , genPartFlav_(genPartFlav)
  , isMatchedToReco_(false)
{}

Int_t
GenParticle::status() const
{
  return status_;
}

Int_t
GenParticle::statusFlags() const
{
  return statusFlags_;
}

UChar_t
GenParticle::genPartFlav() const
{
  return genPartFlav_;
}

void
GenParticle::setMatchedToReco()
{
  isMatchedToReco_ = true;
}

bool
GenParticle::isMatchedToReco() const
{
  return isMatchedToReco_;
}

bool
GenParticle::checkStatusFlag(StatusFlag statusFlag) const
{
  return statusFlags_ < 0 ? false : statusFlags_ & (1 << static_cast<int>(statusFlag));
}

std::ostream &
operator<<(std::ostream & stream,
           const GenParticle & particle)
{
  stream << static_cast<const ChargedParticle &>(particle)                << ","
            " status = "      << particle.status()                        << ","
            " statusFlags = " << particle.statusFlags()                   << ","
            " genPartFlav = " << static_cast<int>(particle.genPartFlav())
  ;
  return stream;
}
