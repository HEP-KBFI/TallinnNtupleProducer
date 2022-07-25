#ifndef TallinnNtupleProducer_Readers_ParticleCollectionGenMatcher_h
#define TallinnNtupleProducer_Readers_ParticleCollectionGenMatcher_h

#include <DataFormats/Math/interface/deltaR.h>                        // deltaR()

#include "TallinnNtupleProducer/CommonTools/interface/cmsException.h" // cmsException
#include "TallinnNtupleProducer/Objects/interface/GenHadTau.h"        // GenHadTau
#include "TallinnNtupleProducer/Objects/interface/GenJet.h"           // GenJet
#include "TallinnNtupleProducer/Objects/interface/GenParticle.h"      // GenParticle
#include "TallinnNtupleProducer/Objects/interface/RecoElectron.h"     // RecoElectron
#include "TallinnNtupleProducer/Objects/interface/RecoHadTau.h"       // RecoHadTau
#include "TallinnNtupleProducer/Objects/interface/RecoJetAK4.h"       // RecoJetAK4
#include "TallinnNtupleProducer/Objects/interface/RecoMuon.h"         // RecoMuon
#include "TallinnNtupleProducer/Objects/interface/CorrT1METJet.h"     // CorrT1METJet

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
   * @brief Matches reconstructed electrons, muons and taus to gen particles as designated by NanoAOD
   *        Ignoring matches to gen quarks
   */
  void
  addGenMatchByIdx(const std::vector<const Trec *> & recParticles,
                   const std::vector<GenParticle> & genParticles) const
  {
    const int genParticleSize = genParticles.size();
    for(const Trec * recParticle: recParticles)
    {
      if(recParticle->hasAnyGenMatch())
      {
        // the reco particle has already been matched to a gen particle
        continue;
      }

      const UChar_t genPartFlav = recParticle->genPartFlav();
      if(genPartFlav == 0 ||
         ((typeid(Trec) == typeid(RecoMuon) || typeid(Trec) == typeid(RecoElectron)) &&
          (genPartFlav != 1 && genPartFlav != 15 && genPartFlav != 22)) ||
         (typeid(Trec) == typeid(RecoHadTau) && genPartFlav == 5))
      {
        // ignore reco lepton matches to generator-level quarks
        continue;
      }
      const int genPartIdx = recParticle->genPartIdx();

      if(genPartIdx >= 0)
      {
        if(genPartIdx >= genParticleSize)
        {
          throw cmsException(this, __func__, __LINE__)
            << "Expected gen matching index = " << genPartIdx << " but gen collection has size of " << genParticleSize
          ;
        }
        GenParticle * genMatch = const_cast<GenParticle *>(&genParticles.at(genPartIdx));
        Trec * recParticle_nonconst = const_cast<Trec *>(recParticle);

        const int genPart_absPdgId = genMatch->absPdgId();
        if(genPart_absPdgId == 11 || genPart_absPdgId == 13)
        {
          genLeptonLinker_(*recParticle_nonconst, genMatch);
          //assert(! genMatch->isMatchedToReco()); // should not happen ...
          genMatch->setMatchedToReco();
        }
        else if(genPart_absPdgId == 22)
        {
          genPhotonLinker_(*recParticle_nonconst, genMatch, std::is_same<Trec, RecoElectron>());
          //assert(! genMatch->isMatchedToReco());
          genMatch->setMatchedToReco();
        }
        else
        {
          throw cmsException(this, __func__, __LINE__) << "Found gen particle with unexpected abs(PDG ID) = " << genPart_absPdgId;
        }
      }
    }
  }
  
  /**
   * @brief Matches reco jets to gen jets
   *        Could also be used to match AK8 reco jets to AK8 gen jets
   */
  void
  addGenJetMatchByIdx(const std::vector<const Trec *> & recParticles,
                      const std::vector<GenJet> & genJets) const
  {
    const int genJetSize = genJets.size();
    for(const Trec * recParticle: recParticles)
    {
      if(recParticle->hasAnyGenMatch())
      {
        // the reco particle has already been matched to a gen particle
        continue;
      }

      const int genJetIdx = recParticle->genJetIdx();
      if(genJetIdx >= 0)
      {
        if(genJetIdx >= genJetSize)
        {
          // In NanoAOD, matching is done before the pT threshold is applied to GenJets
          // See https://hypernews.cern.ch/HyperNews/CMS/get/physics-validation/3311.html?inline=-1
          // And NanoAOD documentation: https://twiki.cern.ch/twiki/bin/view/CMSPublic/WorkBookNanoAOD#The_Events_TTree
          // We could perhaps replace the actual gen jet with just a boolean that basically tells if the recojet is from PU
          continue;
        }
        GenJet * genMatch = const_cast<GenJet *>(&genJets.at(genJetIdx));
        Trec * recParticle_nonconst = const_cast<Trec *>(recParticle);
        genJetLinker_(*recParticle_nonconst, genMatch);
        //assert(! genMatch->isMatchedToReco());
        genMatch->setMatchedToReco();
      }
    }
  }

  /**
   * @brief Match reconstructed particles to generator level electrons and muons by dR
   */
  void
  addGenLeptonMatch(const std::vector<const Trec *> & recParticles,
                    const std::vector<GenParticle> & genParticles,
                    double dRmax = 0.3,
                    double minDPtRel = -0.5,
                    double maxDPtRel = +0.5,
                    int status = 1) const
  {
    return addGenMatch<GenParticle, GenLeptonLinker>(recParticles, genParticles, dRmax, minDPtRel, maxDPtRel, genLeptonLinker_, status, { 11, 13 });
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
                    const std::vector<GenParticle> & genPhotons,
                    double dRmax = 0.3,
                    double maxDPtRel = 1.0, // 0 < pt(reco) < 2 * pt(gen)
                    int status = 1) const
  {
    return addGenMatch<GenParticle, GenPhotonLinker>(recParticles, genPhotons, dRmax, maxDPtRel, genPhotonLinker_, status);
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
        if(! checkPartonFlavs(genParticle, genPartFlavs, std::is_same<Trec, RecoJetAK4>()))
        {
          continue;
        }
        if(genParticle.status() != status)
        {
          continue;
        }
        if(! genParticle.isMatchedToReco())
        {
          continue;
        }
        if(! checkPartonFlavs(recParticle, genPartFlavs, std::is_same<Trec, RecoJetAK4>()))
        {
          continue;
        }
        const double dPtRel = std::fabs(recParticle->pt() - genParticle.pt()) / genParticle.pt();
        if(! (minDPtRel < dPtRel && dPtRel < maxDPtRel))
        {
          continue;
        }

        const double dR = deltaR(
          recParticle->eta(), recParticle->phi(), genParticle.eta(), genParticle.phi()
        );
        if(dR < dRmax && dR < dR_bestMatch)
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

  bool
  checkPartonFlavs(const Trec * recParticle,
                   const std::vector<unsigned char> & genPartFlavs,
                   std::false_type) const
  {
    return std::find(genPartFlavs.begin(), genPartFlavs.end(), recParticle->genPartFlav()) != genPartFlavs.end();
  }

  bool
  checkPartonFlavs(const Trec * recParticle,
                   const std::vector<unsigned char> & genPartFlavs,
                   std::true_type) const
  {
    return true;
  }

  template <typename Tgen>
  bool
  checkPartonFlavs(const Tgen & genParticle,
                   const std::vector<unsigned char> & genPartFlavs,
                   std::false_type) const
  {
    return true;
  }

  template <typename Tgen>
  bool
  checkPartonFlavs(const Tgen & genParticle,
                   const std::vector<unsigned char> & genPartFlavs,
                   std::true_type) const
  {
    return std::find(genPartFlavs.begin(), genPartFlavs.end(), genParticle.absPdgId()) != genPartFlavs.end();
  }

  struct GenLeptonLinker
  {
    void
    operator()(Trec & recParticle,
               const GenParticle * genParticle) const
    {
      recParticle.set_genLepton(genParticle);
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
                    const GenParticle * genPhoton,
                    std::true_type) const
    {
      recParticle.set_genPhoton(genPhoton);
    }
    
    void operator()(Trec & recParticle,
                    const GenParticle * genPhoton,
                    std::false_type) const
    {}
  };
  GenPhotonLinker genPhotonLinker_;

  struct GenJetLinker
  {
    void operator()(Trec & recParticle,
                    const GenJet * genJet) const
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
typedef ParticleCollectionGenMatcher<CorrT1METJet> CorrT1METJetCollectionGenMatcher;

#endif // TallinnNtupleProducer_Readers_ParticleCollectionGenMatcher_h
