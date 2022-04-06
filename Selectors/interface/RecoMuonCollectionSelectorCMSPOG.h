#ifndef TallinnNtupleProducer_Selectors_RecoMuonCollectionSelectorCMSPOG_h
#define TallinnNtupleProducer_Selectors_RecoMuonCollectionSelectorCMSPOG_h

#include "TallinnNtupleProducer/Objects/interface/RecoMuon.h"                     // RecoMuon
#include "TallinnNtupleProducer/Selectors/interface/ParticleCollectionSelector.h" // ParticleCollectionSelector

enum class Era;

class RecoMuonSelectorCMSPOG
{
 public:
  explicit
  RecoMuonSelectorCMSPOG(Era era,
                         int index = -1,
                         bool debug = false,
                         bool set_selection_flags = true);

  /**
   * @brief Set cut thresholds
   */
  void set_min_pt(double min_pt);
  void set_max_absEta(double max_absEta);

  /**
   * @brief Get cut thresholds
   */
  double get_min_pt() const;
  double get_max_absEta() const;

  /**
   * @brief Check if muon given as function argument passes "CMS POG" muon selection, defined in
   *        https://git.rwth-aachen.de/3pia/cms_analyses/bbww_dl/-/blob/33d3a9b255044a8655f2d47e43d5d3ca4350671c/recipes/selection.py#L179-186
   * @return True if muon passes selection; false otherwise
   */
  bool
  operator()(const RecoMuon & muon) const;

 protected:
  const Era era_;
  bool debug_;
  bool set_selection_flags_;

  Double_t min_pt_;             ///< lower cut threshold on reco::Muon pT
  Double_t max_absEta_;         ///< upper cut threshold on absolute value of eta
  const Double_t max_PFrelIso_; ///< upper cut threshold on relative PF isolation
};

typedef ParticleCollectionSelector<RecoMuon, RecoMuonSelectorCMSPOG> RecoMuonCollectionSelectorCMSPOG;

#endif // TallinnNtupleProducer_Selectors_RecoMuonCollectionSelectorCMSPOG_h
