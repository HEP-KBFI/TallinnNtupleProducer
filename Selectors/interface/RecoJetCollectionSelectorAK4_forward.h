#ifndef TallinnNtupleProducer_Selectors_RecoJetCollectionSelectorAK4_forward_h
#define TallinnNtupleProducer_Selectors_RecoJetCollectionSelectorAK4_forward_h

#include "TallinnNtupleProducer/Objects/interface/RecoJetAK4.h"                      // RecoJetAK4
#include "TallinnNtupleProducer/Selectors/interface/ParticleCollectionSelector.h"    // ParticleCollectionSelector
#include "TallinnNtupleProducer/Selectors/interface/RecoJetCollectionSelectorAK4.h"  // RecoJetSelectorAK4

enum class Era;

class RecoJetSelectorAK4_forward : public RecoJetSelectorAK4
{
 public:
  explicit RecoJetSelectorAK4_forward(Era era,
                                      int index = -1,
                                      bool debug = false);
  ~RecoJetSelectorAK4_forward() {}

  /**
   * @brief Get cut thresholds
   */
  double get_min_absEta() const;

  /**
   * @brief Check if jet given as function argument passes the selection cuts
   * @return True if jet passes selection; false otherwise
   */
  bool
  operator()(const RecoJetAK4 & jet) const;

 protected:
  Double_t min_absEta_; ///< upper cut threshold on absolute value of eta
};

typedef ParticleCollectionSelector<RecoJetAK4, RecoJetSelectorAK4_forward> RecoJetCollectionSelectorAK4_forward;

#endif // TallinnNtupleProducer_Selectors_RecoJetCollectionSelectorAK4_forward_h
