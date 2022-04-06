#ifndef TallinnNtupleProducer_Selectors_RecoJetCollectionSelectorAK4_btag_h
#define TallinnNtupleProducer_Selectors_RecoJetCollectionSelectorAK4_btag_h

#include "TallinnNtupleProducer/Objects/interface/RecoJetAK4.h"                      // RecoJetAK4
#include "TallinnNtupleProducer/Selectors/interface/ParticleCollectionSelector.h"    // ParticleCollectionSelector
#include "TallinnNtupleProducer/Selectors/interface/RecoJetCollectionSelectorAK4.h"  // RecoJetSelectorAK4

enum class Era;

class RecoJetSelectorAK4_btag : public RecoJetSelectorAK4
{
 public:
  explicit RecoJetSelectorAK4_btag(Era era,
                                   int index,
                                   bool debug);

  /**
   * @brief Get cut thresholds
   */
  double get_min_BtagCSV() const;

  /**
   * @brief Check if jet given as function argument passes the selection cuts
   * @return True if jet passes selection; false otherwise
   */
  bool
  operator()(const RecoJetAK4 & jet) const;

 protected:
  Double_t min_BtagCSV_; ///< lower cut threshold on CSV b-tagging discriminator value
};

class RecoJetSelectorAK4_btagLoose : public RecoJetSelectorAK4_btag
{
 public:
  explicit RecoJetSelectorAK4_btagLoose(Era era, int index = -1, bool debug = false);
};

typedef ParticleCollectionSelector<RecoJetAK4, RecoJetSelectorAK4_btagLoose> RecoJetCollectionSelectorAK4_btagLoose;

class RecoJetSelectorAK4_btagMedium : public RecoJetSelectorAK4_btag
{
 public:
  explicit RecoJetSelectorAK4_btagMedium(Era era, int index = -1, bool debug = false);
};

typedef ParticleCollectionSelector<RecoJetAK4, RecoJetSelectorAK4_btagMedium> RecoJetCollectionSelectorAK4_btagMedium;

#endif // TallinnNtupleProducer_Selectors_RecoJetCollectionSelectorAK4_btag_h


