#include "TallinnNtupleProducer/Objects/interface/GenParticle.h" // GenParticle, Particle

#include "TallinnNtupleProducer/CommonTools/interface/format_vT.h" // format_vint()

#include "TDatabasePDG.h" // TDatabasePDG

#include <map> // std::map<,>

int
PdgTable::get_charge(int pdgId)
{
  static std::map<int, int> ids;
  if(ids.find(pdgId) == ids.end())
  {
    const TParticlePDG * const pdgParticle = TDatabasePDG::Instance()->GetParticle(pdgId);
    ids[pdgId] = pdgParticle ? pdgParticle->Charge() : 0; // assume neutral if not available in DB
  }
  return ids.at(pdgId);
}

double
PdgTable::get_mass(double mass,
                   int pdgId)
{
  static std::map<int, double> masses;
  const int absPdgId = std::abs(pdgId);
  // https://github.com/cms-sw/cmssw/blob/CMSSW_12_3_1/PhysicsTools/NanoAOD/python/genparticles_cff.py#L44
  if((absPdgId>=1 && absPdgId<=5) || (absPdgId>=11 && absPdgId<=16) || pdgId==21 || pdgId==111 ||
     absPdgId==211 || absPdgId==421 || absPdgId==411 || (pdgId==22 && mass<1))
  {
    if(masses.find(absPdgId) == masses.end())
    {
      const TParticlePDG * const pdgParticle = TDatabasePDG::Instance()->GetParticle(pdgId);
      masses[absPdgId] = pdgParticle ? pdgParticle->Mass() : 0; // assume massless if not available in DB
    }
    return masses.at(absPdgId);
  }
  return mass;
}

GenParticle::GenParticle()
  : GenParticle(0., 0., 0., 0., 0, -1, -1, -1)
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
    , genParticle.momIdx_
  )
{}

GenParticle::GenParticle(Double_t pt,
                         Double_t eta,
                         Double_t phi,
                         Double_t mass,
                         Int_t pdgId,
                         Int_t status,
                         Int_t statusFlags,
                         Int_t momIdx)
  : ChargedParticle(pt, eta, phi, PdgTable::get_mass(mass, pdgId), pdgId, PdgTable::get_charge(pdgId))
  , status_(status)
  , statusFlags_(statusFlags)
  , momIdx_(momIdx)
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

Int_t
GenParticle::momIdx() const
{
  return momIdx_;
}

std::vector<int>
GenParticle::dauIdxs() const
{
  return dauIdxs_;
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

void
GenParticle::addDaughter(int idx)
{
  dauIdxs_.push_back(idx);
}

std::ostream &
operator<<(std::ostream & stream,
           const GenParticle & particle)
{
  stream << static_cast<const ChargedParticle &>(particle)                << ","
            " status = "      << particle.status()                        << ","
            " statusFlags = " << particle.statusFlags()                   << ","
            " momIdx = "      << particle.momIdx()                        << ","
            " dauIdxs = "     << format_vint(particle.dauIdxs())
  ;
  return stream;
}
