#ifndef TallinnNtupleProducer_Selectors_RecoJetCollectionSelectorAK8_Wjj_h
#define TallinnNtupleProducer_Selectors_RecoJetCollectionSelectorAK8_Wjj_h

#include "TallinnNtupleProducer/Objects/interface/RecoJetAK8.h"                   // RecoJetAK8
#include "TallinnNtupleProducer/Selectors/interface/ParticleCollectionSelector.h" // ParticleCollectionSelector

class RecoLepton;

enum class Era;

class RecoJetSelectorAK8_Wjj
{
 public:
  explicit RecoJetSelectorAK8_Wjj(Era era,
                                  int index = -1,
                                  bool debug = false);
  ~RecoJetSelectorAK8_Wjj() {}

  /**
   * @brief Set cut thresholds
   */
  void set_min_pt(double min_pt);
  void set_max_absEta(double max_absEta);
  void set_min_msoftdrop(double min_msoftdrop);
  void set_max_msoftdrop(double max_msoftdrop);
  void set_max_tau2_div_tau1(double max_tau2_div_tau1);
  void set_max_dR_lepton(double max_dR_lepton);
  void set_min_subJet1_pt(double min_pt);
  void set_max_subJet1_absEta(double max_absEta);
  void set_min_subJet2_pt(double min_pt);
  void set_max_subJet2_absEta(double max_absEta);
  void set_min_jetId(int min_jetId);

  void enable_dR_lepton_cut();
  void disable_dR_lepton_cut();

  void set_lepton(const RecoLepton * lepton) const;
  void set_leptons(const std::vector<const RecoLepton *> & leptons) const;

  /**
   * @brief Get cut thresholds
   */
  double get_min_pt() const;
  double get_max_absEta() const;
  double get_min_msoftdrop() const;
  double get_max_msoftdrop() const;
  double get_max_tau2_div_tau1() const;
  double get_max_dR_lepton() const;
  double get_min_subJet1_pt() const;
  double get_max_subJet1_absEta() const;
  double get_min_subJet2_pt() const;
  double get_max_subJet2_absEta() const;
  int get_min_jetId() const;

  /**
   * @brief Check if jet given as function argument passes pT and eta cuts (pT > 25 GeV and |eta| < 2.4, cf. Section 3.1 of AN-2015/321)
   * @return True if jet passes selection; false otherwise
   */
  bool
  operator()(const RecoJetAK8 & jet) const;

  bool
  operator()(const RecoJetAK8 & jet,
             std::string & returnType) const;
 
 protected:
  Double_t min_pt_;                        ///< lower cut threshold on pT of "fat" (AK8) jet
  Double_t max_absEta_;                    ///< upper cut threshold on absolute value of eta of "fat" (AK8) jet
  Int_t    min_jetId_;                     ///< lower cut threshold on jet ID value 
  Double_t min_msoftdrop_;                 ///< lower cut threshold on mass of the two subjets
  Double_t max_msoftdrop_;                 ///< upper cut threshold on mass of the two subjets
  Double_t max_tau2_div_tau1_;             ///< upper cut threshold on value of N-subjettiness ratio tau2/tau1
  Double_t max_dR_lepton_;                 ///< upper cut threshold on distance between "fat" (AK8) jet and electron or muon
  Double_t min_subJet1_pt_;                ///< lower cut threshold on pT of first subjet
  Double_t max_subJet1_absEta_;            ///< upper cut threshold on absolute value of eta of first subjet
  Double_t min_subJet2_pt_;                ///< lower cut threshold on pT of second subjet
  Double_t max_subJet2_absEta_;            ///< upper cut threshold on absolute value of eta of second subjet
  bool     apply_dR_lepton_cut_;           ///< set this flag to False when reading AK8 lepton-subtracted jet collection; otherwise, set this flag to true
  
  mutable std::vector<const RecoLepton *> leptons_; ///< pointer to electron or muon produced in H->WW*->jj lnu decay
  
  bool debug_;
};

typedef ParticleCollectionSelector<RecoJetAK8, RecoJetSelectorAK8_Wjj> RecoJetCollectionSelectorAK8_Wjj;

#endif // TallinnNtupleProducer_Selectors_RecoJetCollectionSelectorAK8_Wjj_h

