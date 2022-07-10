#include "TallinnNtupleProducer/Objects/interface/GenParticle.h" // GenParticle, Particle

#include "TDatabasePDG.h" // TDatabasePDG

#include <map> // std::map<,>

int
PdgTable::get_charge(int pdgId)
{
  static std::map<int, int> ids;
  if(ids.find(pdgId) == ids.end())
  {
    ids[pdgId] = TDatabasePDG::Instance()->GetParticle(pdgId)->Charge();
  }
  return ids.at(pdgId);
}

GenParticle::GenParticle()
  : GenParticle(0., 0., 0., 0., 0, -1, -1)
{}

GenParticle::GenParticle(const GenParticle & genParticle)
  : GenParticle(
      genParticle.pt_
    , genParticle.eta_
    , genParticle.phi_
    , genParticle.mass_
    , genParticle.pdgId_
    , genParticle.status_
    , genParticle.statusFlags_
  )
{}

GenParticle::GenParticle(Double_t pt,
                         Double_t eta,
                         Double_t phi,
                         Double_t mass,
                         Int_t pdgId,
                         Int_t status,
                         Int_t statusFlags)
  : ChargedParticle(pt, eta, phi, mass, pdgId, PdgTable::get_charge(pdgId))
  , status_(status)
  , statusFlags_(statusFlags)
  , isMatchedToReco_(false)
{}

GenParticle::GenParticle(const math::PtEtaPhiMLorentzVector & p4,
                         Int_t pdgId,
                         Int_t status,
                         Int_t statusFlags)
  : ChargedParticle(p4, pdgId, PdgTable::get_charge(pdgId))
  , status_(status)
  , statusFlags_(statusFlags)
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
            " statusFlags = " << particle.statusFlags()
  ;
  return stream;
}
