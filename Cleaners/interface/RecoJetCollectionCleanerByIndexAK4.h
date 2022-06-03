#ifndef TallinnNtupleProducer_Cleaners_ParticleCollectionCleanerByIndexAK4_h
#define TallinnNtupleProducer_Cleaners_ParticleCollectionCleanerByIndexAK4_h

#include "TallinnNtupleProducer/Cleaners/interface/ParticleCollectionCleaner.h" // ParticleCollectionCleaner
#include "TallinnNtupleProducer/CommonTools/interface/cmsException.h"           // get_human_line()
#include "TallinnNtupleProducer/Objects/interface/RecoJetAK4.h"                 // RecoJetAK4

class RecoJetCollectionCleanerByIndexAK4
{
 public:
  RecoJetCollectionCleanerByIndexAK4(bool debug = false)
    : debug_(debug)
  {}
  ~RecoJetCollectionCleanerByIndexAK4() {}

  template <typename Toverlap>
  std::vector<const RecoJetAK4 *>
  operator()(const std::vector<const RecoJetAK4 *> & jets,
             const std::vector<const Toverlap *> & overlaps) const
  {
    if(debug_)
    {
      std::cout << get_human_line(this, __func__, __LINE__) << '\n';
    }

    std::vector<const RecoJetAK4 *> cleanedJets;
    for(const RecoJetAK4 * jet: jets)
    {
      const int jet_idx = jet->idx();
      bool isOverlap = false;
      for(const Toverlap * overlap: overlaps)
      {
        if(overlap->jetIdx() == jet_idx)
        {
          isOverlap = true;
          if(debug_)
          {
            std::cout << "Removed:\n"                    << *jet
                      << "because it overlapped with:\n" << *overlap
                      << " at index "                    << jet_idx
                      << '\n'
            ;
          }
          break;
        }
      }
      if(! isOverlap)
      {
        cleanedJets.push_back(jet);
      }
    }
    return cleanedJets;
  }

  template <typename Toverlap,
            typename... Args>
  std::vector<const RecoJetAK4 *>
  operator()(const std::vector<const RecoJetAK4 *> & jets,
             const std::vector<const Toverlap *> & overlaps,
             Args... args) const
  {
    std::vector<const RecoJetAK4 *> cleanedJets = (*this)(jets, overlaps);
    return (*this)(cleanedJets, args...);
  }

 protected:
  bool debug_;
}; 

#endif // TallinnNtupleProducer_Cleaners_ParticleCollectionCleanerByIndexAK4_h
