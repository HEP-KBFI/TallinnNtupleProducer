#ifndef TallinnNtupleProducer_Selectors_ParticleCollectionSelector_h
#define TallinnNtupleProducer_Selectors_ParticleCollectionSelector_h

#include "TallinnNtupleProducer/CommonTools/interface/Era.h" // Era

#include <vector>                                            // std::vector
#include <algorithm>                                         // std::sort()
#include <type_traits>                                       // std::enable_if, std::is_base_of

template <typename Tobj,
          typename Tsel>
class ParticleCollectionSelector
{
 public:
  ParticleCollectionSelector(Era era,
                             int index = -1,
                             bool debug = false)
    : selIndex_(index)
    , selector_(era, index, debug)
  {}

  /**
   * @brief Select subset of particles passing selection, by applying selector specified as
   *        template parameter to each particle in collection passed as function argument
   * @return Collection of selected particles
   */
  std::vector<const Tobj *>
  operator()(const std::vector<const Tobj *> & particles) const
  {
    std::vector<const Tobj *> selParticles;
    int idx = 0;
    for(const Tobj * particle: particles)
    {
      if(selector_(*particle))
      {
        if(idx == selIndex_ || selIndex_ == -1)
        {
          selParticles.push_back(particle);
        }
        ++idx;
      }
    }
    return selParticles;
  }

  template <typename T,
            typename = typename std::enable_if<std::is_base_of<T, Tobj>::value>::type>
  std::vector<const Tobj *>
  operator()(const std::vector<const Tobj *> & particles,
             bool (*sortFunction)(const T *, const T *)) const
  {
    std::vector<const Tobj *> selParticles = (*this)(particles);
    std::sort(selParticles.begin(), selParticles.end(), sortFunction);
    return selParticles;
  }

  Tsel &
  getSelector() 
  {
    return selector_;
  }
  Tsel const &
  getSelector() const
  {
    return selector_;
  }

 protected:
  int selIndex_;
  Tsel selector_;
};

#endif // TallinnNtupleProducer_Selectors_ParticleCollectionSelector_h

