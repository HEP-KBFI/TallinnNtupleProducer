#ifndef TallinnNtupleProducer_Selectors_RecoJetCollectionSelectorAK8_h
#define TallinnNtupleProducer_Selectors_RecoJetCollectionSelectorAK8_h

#include "TallinnNtupleProducer/Objects/interface/RecoJetAK8.h"                   // RecoJetAK8
#include "TallinnNtupleProducer/Selectors/interface/ParticleCollectionSelector.h" // ParticleCollectionSelector

enum class Era;

class RecoJetSelectorAK8
{
 public:
  explicit RecoJetSelectorAK8(Era era,
                              int index = -1,
                              bool debug = false);
  ~RecoJetSelectorAK8() {}

  /**
   * @brief Set cut thresholds
   */
  void set_min_pt(double min_pt);
  void set_max_absEta(double max_absEta);
  void set_min_msoftdrop(double min_msoftdrop);

  /**
   * @brief Get cut thresholds
   */
  double get_min_pt() const;
  double get_max_absEta() const;
  double get_min_msoftdrop() const;

  /**
   * @brief Check if jet given as function argument passes pT and eta cuts (pT > 25 GeV and |eta| < 2.4, cf. Section 3.1 of AN-2015/321)
   * @return True if jet passes selection; false otherwise
   */
  bool
  operator()(const RecoJetAK8 & jet) const;

 protected:
  Double_t min_pt_;     ///< lower cut threshold on pT
  Double_t max_absEta_; ///< upper cut threshold on absolute value of eta
  Double_t min_msoftdrop_;
  bool debug_;
};

typedef ParticleCollectionSelector<RecoJetAK8, RecoJetSelectorAK8> RecoJetCollectionSelectorAK8;

#endif // TallinnNtupleProducer_Selectors_RecoJetCollectionSelectorAK8_h
