#include "TallinnNtupleProducer/Writers/interface/GenPhotonFilter.h"

#include "TallinnNtupleProducer/CommonTools/interface/cmsException.h" // cmsException()

#include <algorithm> // std::find()

GenPhotonFilter::GenPhotonFilter(const std::string & mode,
                                 double minPt,
                                 double maxAbsEta,
                                 double minDeltaR)
  : minPt_(minPt)
  , maxAbsEta_(maxAbsEta)
  , minDeltaR_(minDeltaR)
  , hardProcessPdgIds_({ 1, 2, 3, 4, 5, 11, 13, 15, 21 })
{
  if      ( mode == "disabled" ) mode_ = Mode::kDisabled;
  else if ( mode == "enabled"  ) mode_ = Mode::kEnabled;
  else if ( mode == "inverted" ) mode_ = Mode::kInverted;
  else throw cmsException(this, __func__, __LINE__) << "Invalid Configuration parameter 'mode' = " << mode;
}

GenPhotonFilter::~GenPhotonFilter()
{}

int
GenPhotonFilter::getNumPassingPhotons(const std::vector<const GenParticle *> & genPhotons,
                                      const std::vector<const GenParticle *> & genFromHardProcess) const
{
  int numSelPhotons = 0;
  for(const GenParticle * genPhoton: genPhotons)
  {
    double deltaR = +1e6;
    for(const GenParticle * genParticle: genFromHardProcess)
    {
      deltaR = std::min(deltaR, genPhoton->deltaR(*genParticle));
    }
    if(genPhoton->checkStatusFlag(StatusFlag::isPrompt) &&
       genPhoton->pt() > minPt_ &&
       genPhoton->absEta() < maxAbsEta_ &&
       deltaR > minDeltaR_)
    {
      ++numSelPhotons;
    }
  }
  return numSelPhotons;
}

bool
GenPhotonFilter::operator()(const std::vector<GenParticle> & genParticles) const
{
  if ( mode_ == Mode::kDisabled ) return true;

  std::vector<const GenParticle *> genFromHardProcess;
  std::vector<const GenParticle *> genPromptPhotons;
  std::vector<int> genPromptLeptons;

  for(std::size_t genPartIdx = 0; genPartIdx < genParticles.size(); ++genPartIdx)
  {
    const GenParticle & genParticle = genParticles.at(genPartIdx);
    if(genParticle.checkStatusFlag(StatusFlag::isHardProcess) &&
       std::find(hardProcessPdgIds_.begin(), hardProcessPdgIds_.end(), genParticle.absPdgId()) != hardProcessPdgIds_.end() &&
       ((genParticle.absPdgId() == 21 || genParticle.absPdgId() < 6) ? genParticle.status() != 21 : true))
    {
      genFromHardProcess.push_back(&genParticle);
    }
    else if(genParticle.checkStatusFlag(StatusFlag::isPrompt))
    {
      if(genParticle.absPdgId() == 22)
      {
        genPromptPhotons.push_back(&genParticle);
      }
      else
      {
        genPromptLeptons.push_back(genPartIdx);
      }
    }
  }
  //TODO still need to implement the proxy photons

  std::vector<const GenParticle *> genProxyPhotons;
  const int numSelPhotons =
    getNumPassingPhotons(genPromptPhotons, genFromHardProcess) +
    getNumPassingPhotons(genProxyPhotons, genFromHardProcess)
  ;

  return
    (mode_ == Mode::kEnabled  && numSelPhotons  > 0) ||
    (mode_ == Mode::kInverted && numSelPhotons == 0)
  ;
}
