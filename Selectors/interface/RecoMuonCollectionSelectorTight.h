#ifndef TallinnNtupleProducer_Selectors_RecoMuonCollectionSelectorTight_h
#define TallinnNtupleProducer_Selectors_RecoMuonCollectionSelectorTight_h

#include "TallinnNtupleProducer/Objects/interface/RecoMuon.h"                     // RecoMuon
#include "TallinnNtupleProducer/Selectors/interface/ParticleCollectionSelector.h" // ParticleCollectionSelector

enum class Era;

class RecoMuonSelectorTight
{
 public:
  explicit
  RecoMuonSelectorTight(Era era,
                        int index = -1,
                        bool debug = false,
                        bool set_selection_flags = true);

  /**
   * @brief Set cut thresholds
   */
  void set_min_lepton_pt(double min_lepton_pt);
  void set_min_cone_pt(double min_cone_pt);
  void set_max_absEta(double max_absEta);

  void set_selection_flags(bool selection_flags);

  /**
   * @brief Get cut thresholds
   */
  double get_min_lepton_pt() const;
  double get_min_cone_pt() const;
  double get_max_absEta() const;

  /**
   * @brief Check if muon given as function argument passes "tight" muon selection, defined in Table 12 of AN-2015/321
   * @return True if muon passes selection; false otherwise
   */
  bool
  operator()(const RecoMuon & muon) const;

 protected:
  const Era era_;
  bool set_selection_flags_;
  int index_;
  bool debug_;

  Double_t min_lepton_pt_;        ///< lower cut threshold on reco::Muon pT
  Double_t min_cone_pt_;          ///< lower cut threshold on cone pT
  Double_t max_absEta_;           ///< upper cut threshold on absolute value of eta
  const Double_t max_dxy_;        ///< upper cut threshold on d_{xy}, distance in the transverse plane w.r.t PV
  const Double_t max_dz_;         ///< upper cut threshold on d_{z}, distance on the z axis w.r.t PV
  const Double_t max_relIso_;     ///< upper cut threshold on relative isolation
  const Double_t max_sip3d_;      ///< upper cut threshold on significance of IP
  const bool apply_looseIdPOG_;   ///< apply (True) or do not apply (False) loose PFMuon id selection
  const bool apply_mediumIdPOG_;  ///< apply (True) or do not apply (False) medium PFMuon id selection
  const Double_t max_jetBtagCSV_; ///< upper cut threshold on CSV b-tagging discriminator value of nearby jet
};

class RecoMuonCollectionSelectorTight : public ParticleCollectionSelector<RecoMuon, RecoMuonSelectorTight>
{
 public:
  explicit
  RecoMuonCollectionSelectorTight(Era era,
                                  int index = -1,
                                  bool debug = false,
                                  bool set_selection_flags = true);
  ~RecoMuonCollectionSelectorTight() {}
};

#endif // TallinnNtupleProducer_Selectors_RecoMuonCollectionSelectorTight_h
