#ifndef TallinnNtupleProducer_Selectors_RecoHadTauSelectorBase_h
#define TallinnNtupleProducer_Selectors_RecoHadTauSelectorBase_h

#include "TallinnNtupleProducer/CommonTools/interface/Era.h"    // Era
#include "TallinnNtupleProducer/Objects/interface/RecoHadTau.h" // RecoHadTau

#define DEFAULT_TAUID_ID_VALUE -1000
#define DEFAULT_TAUID_RAW_VALUE -1.e+6

class RecoHadTauSelectorBase
{
 public:
  explicit
  RecoHadTauSelectorBase(Era era,
                         int index = -1,
                         bool debug = false,
                         bool set_selection_flags = true);
  virtual ~RecoHadTauSelectorBase() {}

  void
  set_min_pt(double min_pt);

  void
  set_max_absEta(double max_absEta);

  double
  get_min_pt() const;

  double
  get_max_absEta() const;

  void
  set_min_id_mva(TauID tauId,
                 int min_id_mva);
  void
  set_min_raw_mva(TauID tauId,
                  double min_raw_mva);

  void
  set_min_antiElectron(int min_antiElectron);

  void
  set_min_antiMuon(int min_antiMuon);

  int
  get_min_antiElectron() const;

  int
  get_min_antiMuon() const;

  bool
  get_deeptau_lepton() const;

  void
  disable_deeptau_lepton();

  void
  set(const std::string & cut);

  const std::string &
  get() const;

  bool
  set_if_looser(const std::string & cut);

  /**
   * @brief Check if hadronic tau given as function argument passes nominal selection criteria defined in Section 3.5 of AN-2015/321
   * @return True if hadronic tau passes selection; false otherwise
   */
  bool operator()(const RecoHadTau & hadTau) const;

  friend class RecoHadTauCollectionSelectorBase;

 protected:
  virtual void set_selection_flags(const RecoHadTau & hadTau) const = 0;
  void reset();

  bool set_selection_flags_;
  bool debug_;                            ///< enable printout for debugging purposes
  Double_t min_pt_;                       ///< lower cut threshold on pT
  Double_t max_absEta_;                   ///< upper cut threshold on absolute value of eta
  Double_t max_dxy_;                      ///< upper cut threshold on d_{xy}, distance on the xy plane w.r.t PV
  Double_t max_dz_;                       ///< upper cut threshold on d_{z}, distance on the z axis w.r.t PV
  Bool_t apply_decayModeFinding_;         ///< lower cut threshold on old decayModeFinding discriminator
  std::vector<int> decayMode_blacklist_;  ///< decay mode IDs that the tau is required to fail
  std::vector<int> decayMode_whitelist_;  ///< decay mode IDs that the tau is required to pass
  std::map<TauID, Int_t> min_id_mva_;     ///< lower cut threshold on MVA-based tau id
  std::map<TauID, Double_t> min_raw_mva_; ///< lower cut threshold on raw output of MVA-based tau id
  Int_t min_antiElectron_;                ///< lower cut threshold on discriminator against electrons
  Int_t min_antiMuon_;                    ///< lower cut threshold on discriminator against muons
  bool apply_deeptau_lepton_;             ///< apply loosest cut of DeepTau anti-e and anti-mu discriminators
  bool disable_deeptau_lepton_;           ///< flag to completely turn of anti-e and anti-mu discriminators
  std::string cut_;

  ///< nominal WP for each tau ID discriminator in fakeable selection
  static const std::map<TauID, std::string> nominal_fakeable_wp_;
};

#endif // TallinnNtupleProducer_Selectors_RecoHadTauSelectorBase_h

