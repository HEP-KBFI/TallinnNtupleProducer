#ifndef TallinnNtupleProducer_Selectors_RecoMuonCollectionSelectorLoose_h
#define TallinnNtupleProducer_Selectors_RecoMuonCollectionSelectorLoose_h

#include "TallinnNtupleProducer/Objects/interface/RecoMuon.h"                     // RecoMuon
#include "TallinnNtupleProducer/Selectors/interface/ParticleCollectionSelector.h" // ParticleCollectionSelector

enum class Era;

class RecoMuonSelectorLoose
{
 public:
  explicit
  RecoMuonSelectorLoose(Era era,
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
   * @brief Check if muon given as function argument passes "loose" muon selection, defined in Table 12 of AN-2015/321
   * @return True if muon passes selection; false otherwise
   */
  bool
  operator()(const RecoMuon & muon) const;

 protected:
  const Era era_;
  bool debug_;
  bool set_selection_flags_;

  Double_t min_pt_;              ///< lower cut threshold on reco::Muon pT
  Double_t max_absEta_;          ///< upper cut threshold on absolute value of eta
  const Double_t max_dxy_;       ///< upper cut threshold on d_{xy}, distance in the transverse plane w.r.t PV
  const Double_t max_dz_;        ///< upper cut threshold on d_{z}, distance on the z axis w.r.t PV
  const Double_t max_relIso_;    ///< upper cut threshold on relative isolation
  const Double_t max_sip3d_;     ///< upper cut threshold on significance of IP
  const bool apply_looseIdPOG_;  ///< apply (True) or do not apply (False) loose PFMuon id selection
  const bool apply_mediumIdPOG_; ///< apply (True) or do not apply (False) medium PFMuon id selection
  const bool apply_tightCharge_; ///< apply (True) or do not apply (False) tight charge cut
};

typedef ParticleCollectionSelector<RecoMuon, RecoMuonSelectorLoose> RecoMuonCollectionSelectorLoose;

#endif // TallinnNtupleProducer_Selectors_RecoMuonCollectionSelectorLoose_h
