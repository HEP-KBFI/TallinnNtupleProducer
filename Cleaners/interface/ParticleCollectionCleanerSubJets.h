#ifndef TallinnNtupleProducer_Cleaners_ParticleCollectionCleanerSubJets_h
#define TallinnNtupleProducer_Cleaners_ParticleCollectionCleanerSubJets_h

#include "DataFormats/Math/interface/deltaR.h"                        // deltaR()

#include "TallinnNtupleProducer/CommonTools/interface/cmsException.h" // get_human_line()

template <typename T>
class ParticleCollectionCleanerSubJets
{
public:
  ParticleCollectionCleanerSubJets(double dR = 0.4,
                            bool debug = false)
    : dR_(dR)
    , debug_(debug)
  {}
  ~ParticleCollectionCleanerSubJets() {}

  /**
   * @brief Select subset of particles not overlapping with any of the other particles passed as function argument
   * @return Collection of non-overlapping particles
   */
  template <typename Toverlap>
  std::vector<const T *> operator()(const std::vector<const T *> & particles,
                                    const std::vector<const Toverlap *> & overlaps) const
  {
    std::vector<const T *> cleanedParticles;
    for(const T * particle: particles)
    {
      bool isOverlap = false;
      if(! (particle->subJet1() && particle->subJet2()))
      {
        isOverlap = true;
        if(debug_)
        {
          std::cout << "Jet of type '" << get_human_name<T>() << "' "
                    << "removed:\n"    << *particle
                    << "because it does not have subjets\n"
          ;
        }
      }
      for(const Toverlap * overlap: overlaps)
      {
        if(isOverlap)
        {
          break;
        }

        const double dRoverlap1 = deltaR(
          particle->subJet1()->eta(), particle->subJet1()->phi(), overlap->eta(), overlap->phi()
        );
        const double dRoverlap2 = deltaR(
          particle->subJet2()->eta(), particle->subJet2()->phi(), overlap->eta(), overlap->phi()
        );

        if(dRoverlap1 < dR_ || dRoverlap2 < dR_)
        {
          isOverlap = true;
          if(debug_)
          {
            std::cout
              << "Jet of type '" << get_human_name<T>() << "' (that has 3rd subjet =" << subJet3Exists<T>(0) << ") "
                 "removed:\n" << *particle << "because it overlapped with:\n" << *overlap << " "
                 "within dR1 = " << dRoverlap1 << " dR2 = " << dRoverlap2 << " dR3 = " << dRoverlap3 << '\n'
            ;
          }
          break;
        }
      }
      if(! isOverlap)
      {
        cleanedParticles.push_back(particle);
      }
    }
    return cleanedParticles;
  }

  template <typename Toverlap,
            typename... Args>
  std::vector<const T *> operator()(const std::vector<const T *> & particles,
                                    const std::vector<const Toverlap *> & overlaps,
                                    Args... args) const
  {

    std::vector<const T *> cleanedParticles = (*this)(particles, overlaps);
    return (*this)(cleanedParticles, args...);
  }

protected:
  double dR_;
  bool debug_;
};

#include "tthAnalysis/HiggsToTauTau/interface/RecoJetAK8.h"

typedef ParticleCollectionCleanerSubJets<RecoJetAK8> RecoJetCollectionCleanerAK8SubJets;

#endif // TallinnNtupleProducer_Cleaners_ParticleCollectionCleanerSubJets_h
