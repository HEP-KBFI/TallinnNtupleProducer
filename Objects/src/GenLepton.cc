#include "TallinnNtupleProducer/Objects/interface/GenLepton.h"

#include <boost/math/special_functions/sign.hpp> // boost::math::sign()

GenLepton::GenLepton(Double_t pt,
                     Double_t eta,
                     Double_t phi,
                     Double_t mass,
                     Int_t pdgId,
                     Int_t status,
                     Int_t statusFlags,
                     UChar_t genPartFlav)
  : GenLepton(pt, eta, phi, mass, pdgId, -boost::math::sign(pdgId), status, statusFlags, genPartFlav)
{}

GenLepton::GenLepton(Double_t pt,
                     Double_t eta,
                     Double_t phi,
                     Double_t mass,
                     Int_t pdgId,
                     Int_t charge,
                     Int_t status,
                     Int_t statusFlags,
                     UChar_t genPartFlav)
  : GenParticle(pt, eta, phi, mass, pdgId, charge, status, statusFlags, genPartFlav)
{}

GenLepton::GenLepton(const math::PtEtaPhiMLorentzVector & p4,
                     Int_t pdgId,
                     Int_t status,
                     Int_t statusFlags,
                     UChar_t genPartFlav)
  : GenParticle(p4, pdgId, -pdgId / std::abs(pdgId), status, statusFlags, genPartFlav)
{}

GenLepton::GenLepton(const GenParticle & genParticle)
  : GenParticle(genParticle)
{}

bool
GenLepton::is_electron() const
{
  return (std::abs(pdgId_) == 11);
}

bool
GenLepton::is_muon() const
{
  return (std::abs(pdgId_) == 13);
}

std::ostream &
operator<<(std::ostream & stream,
           const GenLepton & lepton)
{
  stream << static_cast<GenParticle>(lepton);
  return stream;
}

