#include "TallinnNtupleProducer/Writers/interface/GenPhotonFilter.h"

#include "TallinnNtupleProducer/CommonTools/interface/cmsException.h" // cmsException()

#include <algorithm> // std::find()
#include <map> // std::map<,>

GenPhotonFilter::GenPhotonFilter(const std::string & mode,
                                 double minPt,
                                 double maxAbsEta,
                                 double minDeltaR)
  : minPt_(minPt)
  , maxAbsEta_(maxAbsEta)
  , minDeltaR_(minDeltaR)
  , hardProcessPdgIds_({ 1, 2, 3, 4, 5, 11, 13, 15, 21 }) // TODO: why gluons though?
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
  std::map<int, std::vector<int>> genPromptLeptons;

  // collect prompt photons, hard process particles and prompt leptons (which are needed for constructing proxy photons)
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
      else if(genParticle.absPdgId() == 11 ||
              genParticle.absPdgId() == 13 ||
              genParticle.absPdgId() == 15)
      {
        genPromptLeptons[genParticle.pdgId()].push_back(genPartIdx);
      }
    }
  }

  // form all possible SFOS pairs
  std::vector<std::vector<int>> sfosPairCandidates;
  for(int leptonId: { 11, 13, 15 })
  {
    if(genPromptLeptons.count(leptonId))
    {
      for(std::size_t lepIdx = 0; lepIdx < genPromptLeptons.at(leptonId).size(); ++lepIdx)
      {
        if(genPromptLeptons.count(-leptonId))
        {
          for(std::size_t antiLepIdx = 0; antiLepIdx < genPromptLeptons.at(-leptonId).size(); ++antiLepIdx)
          {
            sfosPairCandidates.push_back({
              genPromptLeptons.at(leptonId).at(lepIdx),
              genPromptLeptons.at(-leptonId).at(antiLepIdx)
            });
          }
        }
      }
    }
  }

  // for every lepton in each pair, find the parent that doesn't decay to a single particle
  std::vector<std::vector<int>> sfosPairs;
  for(const std::vector<int> & sfosPairCandidate: sfosPairCandidates)
  {
    std::vector<int> sfosPair;
    for(int leptonIdx: sfosPairCandidate)
    {
      int currentIdx = -1;
      std::vector<int> leptonIdxs { leptonIdx };
      while(! leptonIdxs.empty())
      {
        currentIdx = leptonIdxs.back();
        leptonIdxs.pop_back();

        const int momIdx = genParticles.at(currentIdx).momIdx();
        assert(momIdx != currentIdx);

        const GenParticle & mom = genParticles.at(momIdx);
        const GenParticle & currentParticle = genParticles.at(currentIdx);

        if(momIdx >= 0 && mom.pdgId() == currentParticle.pdgId() && mom.dauIdxs().size() == 1)
        {
          // mom and daughter are the same -> keep looking
          leptonIdxs.push_back(momIdx);
        }
      }
      assert(currentIdx >= 0);
      sfosPair.push_back(currentIdx);
    }
    assert(sfosPair.size() == 2);
    sfosPairs.push_back(sfosPair);
  }
  
  // find out the SFOS leptons pairs with a common mother that is coming from a lepton
  std::map<int, std::vector<std::vector<int>>> sfosPairsWithCommonMom;
  std::vector<std::vector<int>> sfosPairsWithNoMom;
  for(const std::vector<int> & sfosPair: sfosPairs)
  {
    std::vector<int> momIdxs;
    for(int leptonIdx: sfosPair)
    {
      momIdxs.push_back(genParticles.at(leptonIdx).momIdx());
    }
    if(momIdxs.at(0) == momIdxs.at(1))
    {
      const int momIdx = momIdxs.at(0);
      const int momPdgId = genParticles.at(momIdx).absPdgId();

      if(momIdx < 0)
      {
        sfosPairsWithNoMom.push_back(sfosPair);
      }
      else if(momPdgId == 11 || momPdgId == 13 || momPdgId == 15)
      {
        sfosPairsWithCommonMom[momIdx].push_back(sfosPair);
      }
    }
  }

  // assume that parentless SFOS pairs come from different photons
  std::vector<std::vector<int>> sfosFinalPairs = sfosPairsWithNoMom;

  // for each mother, keep only the SFOS daughters that have the lowest energy
  for(const auto & kv: sfosPairsWithCommonMom)
  {
    const std::vector<std::vector<int>> & sfosPairArray = kv.second;
    const int nof_sfosPairs = sfosPairArray.size();
    if(nof_sfosPairs > 1)
    {
      double sfosPairMinEnergy = 1e6;
      int sfosPairMinEnergyIdx = -1;
      for(std::size_t pairIdx = 0; pairIdx < sfosPairArray.size(); ++pairIdx)
      {
        const std::vector<int> & sfosPair = sfosPairArray.at(pairIdx);
        const GenParticle & firstParticle = genParticles.at(sfosPair.at(0));
        const GenParticle & secondParticle = genParticles.at(sfosPair.at(1));
        const double currentPairEnergy = (firstParticle.p4() + secondParticle.p4()).E();
        if(currentPairEnergy < sfosPairMinEnergy)
        {
          sfosPairMinEnergy = currentPairEnergy;
          sfosPairMinEnergyIdx = pairIdx;
        }
      }
      assert(sfosPairMinEnergyIdx >= 0);
      sfosFinalPairs.push_back(sfosPairArray.at(sfosPairMinEnergyIdx));
    }
    else if(nof_sfosPairs == 1)
    {
      // no siblings
      sfosFinalPairs.push_back(sfosPairArray.at(0));
    }
    else
    {
      assert(false); // should never happen
    }
  }

  // All this work just to build proxy photons -- because ISR/FSR photons are apparently dropped already at MiniAOD-level
  // https://hypernews.cern.ch/HyperNews/CMS/get/physTools/3766.html?inline=-1
  std::vector<const GenParticle *> genProxyPhotons;
  for(const std::vector<int> & sfosFinalPair: sfosFinalPairs)
  {
    const GenParticle & firstParticle = genParticles.at(sfosFinalPair.at(0));
    const GenParticle & secondParticle = genParticles.at(sfosFinalPair.at(1));
    const math::PtEtaPhiMLorentzVector p4 = firstParticle.p4() + secondParticle.p4();
    const GenParticle * proxyPhoton = new GenParticle(
      p4.pt(), p4.eta(), p4.phi(), p4.mass(), 22, 201, (1 << static_cast<int>(StatusFlag::isPrompt)), -1
    ); // Pythia status codes that are >= 201 are free to use
    genProxyPhotons.push_back(proxyPhoton);
  }

  const int numSelPhotons =
    getNumPassingPhotons(genPromptPhotons, genFromHardProcess) +
    getNumPassingPhotons(genProxyPhotons, genFromHardProcess)
  ;
  
  for(auto & proxyPhoton: genProxyPhotons)
  {
    delete proxyPhoton;
  }

  return
    (mode_ == Mode::kEnabled  && numSelPhotons  > 0) ||
    (mode_ == Mode::kInverted && numSelPhotons == 0)
  ;
}
