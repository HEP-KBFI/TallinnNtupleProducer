#ifndef TallinnNtupleProducer_Selectors_RecoElectronCollectionSelectorLoose_h
#define TallinnNtupleProducer_Selectors_RecoElectronCollectionSelectorLoose_h

#include "TallinnNtupleProducer/Objects/interface/RecoElectron.h"                 // RecoElectron
#include "TallinnNtupleProducer/Selectors/interface/ParticleCollectionSelector.h" // ParticleCollectionSelector

enum class Era;

class RecoElectronSelectorLoose
{
 public:
  explicit
  RecoElectronSelectorLoose(Era era,
                            int index = -1,
                            bool debug = false,
                            bool set_selection_flags = true);
  ~RecoElectronSelectorLoose() {}

  /**
   * @brief Set cut thresholds
   */
  void set_selection_flags(bool selection_flag);
  void set_min_pt(double min_pt);
  void set_max_absEta(double max_absEta);
  void invert_max_nLostHits(Int_t min_nLostHits_fornLostHitsInversion); // for conversion bkg CR

  /**
   * @brief Get cut thresholds
   */
  double get_min_pt() const;
  double get_max_absEta() const;

  /**
   * @brief Check if electron given as function argument passes "loose" electron selection, defined in Table 13 of AN-2015/321
   * @return True if electron passes selection; false otherwise
   */
  bool operator()(const RecoElectron & electron) const;

  void print_selection_conditions();
  
 protected:
  bool set_selection_flags_;
  bool debug_;                        ///< enable printout for debugging purposes
  const Era era_;

  Double_t min_pt_;                         ///< lower cut threshold on reco::GSFElectron pT
  Double_t max_absEta_;                     ///< upper cut threshold on absolute value of eta
  const Double_t max_dxy_;                  ///< upper cut threshold on d_{xy}, distance in the transverse plane w.r.t PV
  const Double_t max_dz_;                   ///< upper cut threshold on d_{z}, distance on the z axis w.r.t PV
  const Double_t max_relIso_;               ///< upper cut threshold on relative isolation
  const Double_t max_sip3d_;                ///< upper cut threshold on significance of IP
//-------------------------------------------------------------------------------
  const bool apply_tightCharge_;            ///< apply (True) or do not apply (False) tight charge cut
  const bool apply_conversionVeto_;         ///< apply (True) or do not apply (False) conversion veto
  const Int_t max_nLostHits_;               ///< upper cut threshold on lost hits in the innermost layer of the tracker (electrons with lost_hits equal to cut threshold pass)
  bool invert_nLostHits_;                     /// for conversion bkg CR
  Int_t min_nLostHits_fornLostHitsInversion_; ///<fornLostHitsInversion: lower cut threshold on lost hits in the innermost layer of the tracker (electrons with lost_hits equal to cut threshold pass)
//-------------------------------------------------------------------------------
};

class RecoElectronCollectionSelectorLoose : public ParticleCollectionSelector<RecoElectron, RecoElectronSelectorLoose>
{
 public:
  explicit
  RecoElectronCollectionSelectorLoose(Era era,
				      int index = -1,
				      bool debug = false,
				      bool set_selection_flags = true);
  ~RecoElectronCollectionSelectorLoose() {}

  void invert_max_nLostHits(Int_t min_nLostHits_fornLostHitsInversion); // for conversion bkg CR

  void print_selection_conditions();
};

#endif // TallinnNtupleProducer_Selectors_RecoElectronCollectionSelectorLoose_h
