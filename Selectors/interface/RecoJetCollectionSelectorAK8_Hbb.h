#ifndef TallinnNtupleProducer_Selectors_RecoJetCollectionSelectorAK8_Hbb_h
#define TallinnNtupleProducer_Selectors_RecoJetCollectionSelectorAK8_Hbb_h

#include "TallinnNtupleProducer/Objects/interface/RecoJetAK8.h"                   // RecoJetAK8
#include "TallinnNtupleProducer/Selectors/interface/ParticleCollectionSelector.h" // ParticleCollectionSelector

enum class Era;

class RecoJetSelectorAK8_Hbb
{
 public:
  explicit RecoJetSelectorAK8_Hbb(Era era,
                                  int index = -1,
                                  bool debug = false);
  ~RecoJetSelectorAK8_Hbb() {}

  /**
   * @brief Set cut thresholds
   */
  void set_min_pt(double min_pt);
  void set_max_absEta(double max_absEta);
  void set_min_msoftdrop(double min_msoftdrop);
  void set_max_msoftdrop(double max_msoftdrop);
  void set_max_tau2_div_tau1(double max_tau2_div_tau1);
  void set_min_subJet1_pt(double min_pt);
  void set_min_subJet1_pt_forBtag(double min_pt);
  void set_max_subJet1_absEta(double max_absEta);
  void set_min_subJet2_pt(double min_pt);
  void set_min_subJet2_pt_forBtag(double min_pt);
  void set_max_subJet2_absEta(double max_absEta);
  void set_min_btagCSV_loose(double min_btagCSV_loose);
  void set_min_btagCSV_medium(double min_btagCSV_medium);
  void set_min_numSubJets_btagLoose(int min_numSubJets_btagLoose);
  void set_min_numSubJets_btagMedium(int min_numSubJets_btagMedium);
  void set_min_jetId(int min_jetId);

  /**
   * @brief Get cut thresholds
   */
  double get_min_pt() const;
  double get_max_absEta() const;
  double get_min_msoftdrop() const;
  double get_max_msoftdrop() const;
  double get_max_tau2_div_tau1() const;
  double get_min_subJet1_pt() const;
  double get_max_subJet1_absEta() const;
  double get_min_subJet2_pt() const;
  double get_max_subJet2_absEta() const;
  double get_min_btagCSV_loose() const;
  double get_min_btagCSV_medium() const;
  int get_min_numSubJets_btagLoose() const;
  int get_min_numSubJets_btagMedium() const;
  int get_min_jetId() const;

  /**
   * @brief Check if jet given as function argument passes pT and eta cuts (pT > 25 GeV and |eta| < 2.4, cf. Section 3.1 of AN-2015/321)
   * @return True if jet passes selection; false otherwise
   */
  bool
  operator()(const RecoJetAK8 & jet) const;

 protected:
  Double_t min_pt_;                 ///< lower cut threshold on pT of "fat" (AK8) jet
  Double_t max_absEta_;             ///< upper cut threshold on absolute value of eta of "fat" (AK8) jet
  Int_t min_jetId_;                 ///< lower cut threshold on jet ID value 
  Double_t min_msoftdrop_;          ///< lower cut threshold on mass of the two subjets
  Double_t max_msoftdrop_;          ///< upper cut threshold on mass of the two subjets
  Double_t max_tau2_div_tau1_;      ///< upper cut threshold on value of N-subjettiness ratio tau2/tau1
  Double_t min_subJet1_pt_;         ///< lower cut threshold on pT of first subjet
  Double_t max_subJet1_absEta_;     ///< upper cut threshold on absolute value of eta of first subjet
  Double_t min_subJet2_pt_;         ///< lower cut threshold on pT of second subjet
  Double_t max_subJet2_absEta_;     ///< upper cut threshold on absolute value of eta of second subjet
  Double_t min_btagCSV_loose_;      ///< lower cut threshold on CSV b-tagging discriminator value (loose WP)
  Double_t min_btagCSV_medium_;     ///< lower cut threshold on CSV b-tagging discriminator value (medium WP)
  Int_t min_numSubJets_btagLoose_;  ///< lower cut threshold on number of subjets passing loose WP of CSV b-tagging discriminator
  Int_t min_numSubJets_btagMedium_; ///< lower cut threshold on number of subjets passing medium WP of CSV b-tagging discriminator
  bool debug_;
};

typedef ParticleCollectionSelector<RecoJetAK8, RecoJetSelectorAK8_Hbb> RecoJetCollectionSelectorAK8_Hbb;

#endif // TallinnNtupleProducer_Selectors_RecoJetCollectionSelectorAK8_Hbb_h

