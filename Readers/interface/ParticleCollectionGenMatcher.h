#ifndef TallinnNtupleProducer_Readers_ParticleCollectionGenMatcher_h
#define TallinnNtupleProducer_Readers_ParticleCollectionGenMatcher_h

#include <DataFormats/Math/interface/deltaR.h>                        // deltaR()

#include "TallinnNtupleProducer/CommonTools/interface/cmsException.h" // cmsException
#include "TallinnNtupleProducer/Objects/interface/GenHadTau.h"        // GenHadTau
#include "TallinnNtupleProducer/Objects/interface/GenJet.h"           // GenJet
#include "TallinnNtupleProducer/Objects/interface/GenLepton.h"        // GenLepton
#include "TallinnNtupleProducer/Objects/interface/GenPhoton.h"        // GenPhoton
#include "TallinnNtupleProducer/Objects/interface/RecoElectron.h"     // RecoElectron
#include "TallinnNtupleProducer/Objects/interface/RecoHadTau.h"       // RecoHadTau
#include "TallinnNtupleProducer/Objects/interface/RecoJetAK4.h"       // RecoJetAK4
#include "TallinnNtupleProducer/Objects/interface/RecoMuon.h"         // RecoMuon

#include <algorithm>                                                  // std::find()

enum class GenParticleType
{
  kGenElectron,
  kGenMuon,
  kGenPhoton,
  kGenAnyLepton,
  kGenAny,
};

template <typename Trec>
class ParticleCollectionGenMatcher
{
 public:
  ParticleCollectionGenMatcher(bool isDEBUG = false)
    : isDEBUG_(isDEBUG)
  {}
  ~ParticleCollectionGenMatcher() {}

  /**
   * @brief Match reconstructed particles to generator level electrons and muons by dR
   */
  void
  addGenLeptonMatch(const std::vector<const Trec *> & recParticles,
                    const std::vector<GenLepton> & genLeptons,
                    double dRmax = 0.3,
                    double minDPtRel = -0.5,
                    double maxDPtRel = +0.5,
                    int status = 1) const
  {
    return addGenMatch<GenLepton, GenLeptonLinker>(recParticles, genLeptons, dRmax, minDPtRel, maxDPtRel, genLeptonLinker_, status);
  }

  void
  addGenLeptonMatchByIndex(const std::vector<const Trec *> & recParticles,
                           const std::vector<GenParticle> & genParticles,
                           GenParticleType genParticleType) const
  {
    if(genParticleType == GenParticleType::kGenPhoton)
    {
      throw cmsException(this, __func__, __LINE__) << "Cannot match to gen photons in this function";
    }
    return addGenMatchByIndex<GenLeptonLinker>(recParticles, genParticles, genLeptonLinker_, genParticleType);
  }

  /**
   * @brief Match reconstructed particles to generator level hadronic tau decays by dR
   */
  void
  addGenHadTauMatch(const std::vector<const Trec *> & recParticles,
                    const std::vector<GenHadTau> & genHadTaus,
                    double dRmax = 0.3,
                    double maxDPtRel = 1.0) const
  {
    std::vector<unsigned char> genPartFlavs;
    if(typeid(Trec) == typeid(RecoHadTau))
    {
      genPartFlavs = { 5 };
    }
    return addGenMatch<GenHadTau, GenHadTauLinker>(recParticles, genHadTaus, dRmax, maxDPtRel, genHadTauLinker_, -1, genPartFlavs);
  }

  void
  addGenPhotonMatch(const std::vector<const Trec *> & recParticles,
                    const std::vector<GenPhoton> & genPhotons,
                    double dRmax = 0.3,
                    double maxDPtRel = 1.0, // 0 < pt(reco) < 2 * pt(gen)
                    int status = 1) const
  {
    return addGenMatch<GenPhoton, GenPhotonLinker>(recParticles, genPhotons, dRmax, maxDPtRel, genPhotonLinker_, status);
  }

  void
  addGenPhotonMatchByIndex(const std::vector<const Trec *> & recParticles,
                           const std::vector<GenParticle> & genParticles) const
  {
    return addGenMatchByIndex<GenPhotonLinker>(recParticles, genParticles, genPhotonLinker_, GenParticleType::kGenPhoton);
  }

  /**
   * @brief Match reconstructed particles to generator level jets by dR
   */
  void
  addGenJetMatch(const std::vector<const Trec *> & recParticles,
                 const std::vector<GenJet> & genJets,
                 double dRmax = 0.3,
                 double maxDPtRel = 0.5) const
  {
    return addGenMatch<GenJet, GenJetLinker>(recParticles, genJets, dRmax, maxDPtRel, genJetLinker_);
  }

  void
  addGenJetMatchByIndex(const std::vector<const Trec *> & recParticles,
                        const std::vector<GenParticle> & genJets) const
  {
    return addGenMatchByIndex<GenJetLinker>(recParticles, genJets, genJetLinker_, GenParticleType::kGenAny);
  }

 protected:
  /**
   * @brief Match reconstructed particles to generator level particles by dR
   */
  template <typename Tgen,
            typename Tlinker>
  void
  addGenMatch(const std::vector<const Trec *> & recParticles,
              const std::vector<Tgen> & genParticles,
              double dRmax,
              double maxDPtRel,
              const Tlinker & linker,
              int status = -1,
              const std::vector<unsigned char> & genPartFlavs = {}) const
  {
    return addGenMatch<Tgen, Tlinker>(
      recParticles, genParticles, dRmax, -maxDPtRel, maxDPtRel, linker, status, genPartFlavs
    );
  }

  template <typename Tgen,
            typename Tlinker>
  void
  addGenMatch(const std::vector<const Trec *> & recParticles,
              const std::vector<Tgen> & genParticles,
              double dRmax,
              double minDPtRel,
              double maxDPtRel,
              const Tlinker & linker,
              int status = -1,
              const std::vector<unsigned char> & genPartFlavs = {}) const
  {
    assert(minDPtRel < 0. && maxDPtRel > 0.);
    for(const Trec * recParticle: recParticles)
    {
      if(recParticle->hasAnyGenMatch())
      {
        // the reco particle has already been matched to a gen particle
        continue;
      }
      Tgen * bestMatch = nullptr;
      double dR_bestMatch = 1.e+3;
      double dPtRel_bestMatch = 1.e+3;

      for(const Tgen & genParticle: genParticles)
      {
        const double dR = deltaR(
          recParticle->eta(), recParticle->phi(), genParticle.eta(), genParticle.phi()
        );
        const double dPtRel = std::fabs(recParticle->pt() - genParticle.pt()) / genParticle.pt();
        bool passesConstraints = minDPtRel < dPtRel && dPtRel < maxDPtRel;
        if(passesConstraints && typeid(Trec) != typeid(RecoJetAK4) && ! genPartFlavs.empty())
        {
          passesConstraints &=
            std::find(genPartFlavs.begin(), genPartFlavs.end(), recParticle->genPartFlav()) != genPartFlavs.end()
          ;
        }
        if(status > 0)
        {
          passesConstraints &= genParticle.status() == status;
        }
        if(dR < dRmax && dR < dR_bestMatch && passesConstraints && ! genParticle.isMatchedToReco())
        {
          bestMatch = const_cast<Tgen *>(&genParticle);
          dR_bestMatch = dR;
          dPtRel_bestMatch = dPtRel;
        }
      }

      if(bestMatch)
      {
        if(isDEBUG_)
        {
          std::cout
            << "Found gen match with dR = " << dR_bestMatch << " and dPtRel = " << dPtRel_bestMatch << " between "
               "reconstructed object...\n" << *recParticle << "\n... and generator level object...\n" << *bestMatch
            << '\n'
          ;
        }
        // forbid the same gen particle to be matched to another reco particle
        bestMatch->setMatchedToReco();
        Trec * recParticle_nonconst = const_cast<Trec *>(recParticle);
        linker(*recParticle_nonconst, bestMatch);
      }
      else if(isDEBUG_)
      {
        std::cout
          << "Did not find gen match for reconstructed object in gen particle collection '" << typeid (Tgen).name()
          << "' (size = " << genParticles.size() << "):\n" << *recParticle << '\n'
        ;
      }
    }
  }

  template <typename Tlinker,
            typename Tgen = GenParticle>
  void
  addGenMatchByIndex(const std::vector<const Trec *> & recParticles,
                     const std::vector<Tgen> & genParticles,
                     const Tlinker & linker,
                     GenParticleType genParticleType) const
  {
    const int genParticleSize = genParticles.size();
    for(const Trec * recParticle: recParticles)
    {
      if(recParticle->hasAnyGenMatch())
      {
        // the reco particle has already been matched to a gen particle
        continue;
      }

      const int genMatchIdx = recParticle->genMatchIdx();

      if(genMatchIdx >= 0)
      {
        if(genMatchIdx >= genParticleSize)
        {
          throw cmsException(this, __func__, __LINE__)
            << "Expected gen matching index = " << genMatchIdx << " but gen collection has size of " << genParticleSize
          ;
        }
        Tgen * genMatch = const_cast<Tgen *>(&genParticles.at(genMatchIdx));
        if(genMatch->genPartFlav() != recParticle->genPartFlav())
        {
          throw cmsException(this, __func__, __LINE__)
            << "Parton flavor of generator level object = " << static_cast<int>(genMatch->genPartFlav())
            << " does not equal to parton flavor of reconstructed object = " << static_cast<int>(recParticle->genPartFlav())
          ;
        }

        std::vector<unsigned int> genAbsPdgIds;
        std::vector<unsigned int> genPartFlavs;
        if(typeid(Trec) == typeid(RecoMuon) && genParticleType == GenParticleType::kGenMuon)
        {
          genPartFlavs = { 1, 15 };
        }
        else if(typeid(Trec) == typeid(RecoElectron))
        {
          if(genParticleType == GenParticleType::kGenPhoton)
          {
            genPartFlavs = { 22 };
          }
          else if(genParticleType == GenParticleType::kGenElectron)
          {
            genPartFlavs = { 1, 15 };
          }
        }
        else if(typeid(Trec) == typeid(RecoHadTau))
        {
          if(genParticleType == GenParticleType::kGenMuon)
          {
            genPartFlavs = { 2, 4 };
          }
          else if(genParticleType == GenParticleType::kGenElectron)
          {
            genPartFlavs = { 1, 3 };
          }
          else if(genParticleType == GenParticleType::kGenAnyLepton)
          {
            genPartFlavs = { 1, 2, 3, 4 };
          }
        }
        switch(genParticleType)
        {
          case GenParticleType::kGenElectron:  genAbsPdgIds = { 11 };     break;
          case GenParticleType::kGenMuon:      genAbsPdgIds = { 13 };     break;
          case GenParticleType::kGenAnyLepton: genAbsPdgIds = { 11, 13 }; break;
          case GenParticleType::kGenPhoton:    genAbsPdgIds = { 22 };     break;
          case GenParticleType::kGenAny:                                  break;
        }

        const bool hasGenAbsPdgIdMatch = genAbsPdgIds.empty() || (
          ! genAbsPdgIds.empty() &&
          std::find(genAbsPdgIds.begin(), genAbsPdgIds.end(), std::abs(genMatch->pdgId())) != genAbsPdgIds.end()
        );
        const bool hasGenPartFlavMatch = genPartFlavs.empty() || (
          ! genPartFlavs.empty() &&
          std::find(genPartFlavs.begin(), genPartFlavs.end(), std::abs(genMatch->genPartFlav())) != genPartFlavs.end()
        );

        if(hasGenAbsPdgIdMatch && hasGenPartFlavMatch)
        {
          if(isDEBUG_)
          {
            const double dR_bestMatch = deltaR(recParticle->eta(), recParticle->phi(), genMatch->eta(), genMatch->phi());
            const double dPtRel_bestMatch = std::fabs(recParticle->pt() - genMatch->pt()) / genMatch->pt();
            std::cout
              << "Found gen match with dR = " << dR_bestMatch << " and dPtRel = " << dPtRel_bestMatch << " between "
                 "reconstructed object...\n" << recParticle << "\n... and generator level object...\n" << *genMatch
              << '\n'
            ;
          }
          Trec * recParticle_nonconst = const_cast<Trec *>(recParticle);
          linker(*recParticle_nonconst, genMatch);
        }
        else if(isDEBUG_)
        {
          std::cout
            << "Did not find gen match for reconstructed object in gen particle collection '" << typeid (Tgen).name()
            << "' (size = " << genParticles.size() << ":\n" << *recParticle << '\n'
          ;
        }
      }
    }
  }

  struct GenLeptonLinker
  {
    void
    operator()(Trec & recParticle,
               const GenLepton * genLepton) const
    {
      recParticle.set_genLepton(new GenLepton(*genLepton));
    }

    void
    operator()(Trec & recParticle,
               const GenParticle * genLepton) const
    {
      recParticle.set_genLepton(new GenLepton(*genLepton));
    }
  };
  GenLeptonLinker genLeptonLinker_;

  struct GenHadTauLinker
  {
    void operator()(Trec & recParticle,
                    const GenHadTau * genHadTau) const
    {
      recParticle.set_genHadTau(new GenHadTau(*genHadTau));
    }
  };
  GenHadTauLinker genHadTauLinker_;

  struct GenPhotonLinker
  {
    void operator()(Trec & recParticle,
                    const GenPhoton * genPhoton) const
    {
      recParticle.set_genPhoton(new GenPhoton(*genPhoton));
    }

    void operator()(Trec & recParticle,
                    const GenParticle * genPhoton) const
    {
      recParticle.set_genPhoton(new GenPhoton(*genPhoton));
    }
  };
  GenPhotonLinker genPhotonLinker_;

  struct GenJetLinker
  {
    void operator()(Trec & recParticle,
                    const GenJet * genJet) const
    {
      recParticle.set_genJet(new GenJet(*genJet));
    }

    void operator()(Trec & recParticle,
                    const GenParticle * genJet) const
    {
      recParticle.set_genJet(new GenJet(*genJet));
    }
  };
  GenJetLinker genJetLinker_;

  bool isDEBUG_;
};

typedef ParticleCollectionGenMatcher<RecoElectron> RecoElectronCollectionGenMatcher;
typedef ParticleCollectionGenMatcher<RecoMuon> RecoMuonCollectionGenMatcher;
typedef ParticleCollectionGenMatcher<RecoHadTau> RecoHadTauCollectionGenMatcher;
typedef ParticleCollectionGenMatcher<RecoJetAK4> RecoJetCollectionGenMatcherAK4;

#endif // TallinnNtupleProducer_Readers_ParticleCollectionGenMatcher_h
