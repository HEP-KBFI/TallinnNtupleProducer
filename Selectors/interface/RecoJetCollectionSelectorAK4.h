#ifndef TallinnNtupleProducer_Selectors_RecoJetCollectionSelectorAK4_h
#define TallinnNtupleProducer_Selectors_RecoJetCollectionSelectorAK4_h

#include "TallinnNtupleProducer/Objects/interface/RecoJetAK4.h"                   // RecoJetAK4
#include "TallinnNtupleProducer/Selectors/interface/ParticleCollectionSelector.h" // ParticleCollectionSelector

enum class Era;
enum class pileupJetID;

class RecoJetSelectorAK4
{
 public:
  explicit RecoJetSelectorAK4(Era era,
                              int index = -1,
                              bool debug = false);
  ~RecoJetSelectorAK4() {}

  /**
   * @brief Set cut thresholds
   */
  void set_min_pt(double min_pt);
  void set_max_absEta(double max_absEta);
  void set_min_jetId(int min_jetId);
  void set_pileupJetId(pileupJetID apply_pileupJetId);

  /**
   * @brief Get cut thresholds
   */
  double get_min_pt() const;
  double get_max_absEta() const;
  int get_min_jetId() const;
  pileupJetID get_pileupJetId() const;

  /**
   * @brief Check if jet given as function argument passes the selection cuts
   * @return True if jet passes selection; false otherwise
   */
  bool
  operator()(const RecoJetAK4 & jet) const;

 protected:
  Era era_;
  Double_t min_pt_;     ///< lower cut threshold on pT
  Double_t max_absEta_; ///< upper cut threshold on absolute value of eta
  Int_t min_jetId_;     ///< lower cut threshold on jet ID value
  pileupJetID apply_pileupJetId_;
  bool debug_;
};

typedef ParticleCollectionSelector<RecoJetAK4, RecoJetSelectorAK4> RecoJetCollectionSelectorAK4;

#endif // TallinnNtupleProducer_Selectors_RecoJetCollectionSelectorAK4_h

