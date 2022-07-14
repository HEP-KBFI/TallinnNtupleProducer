#ifndef TallinnNtupleProducer_Selectors_GenLeptonCollectionSelector_h
#define TallinnNtupleProducer_Selectors_GenLeptonCollectionSelector_h

#include "TallinnNtupleProducer/Selectors/interface/ParticleCollectionSelector.h" // ParticleCollectionSelector
#include "TallinnNtupleProducer/Objects/interface/GenParticle.h"                  // GenParticle

enum class Era;

class GenLeptonSelector
{
 public:
  explicit GenLeptonSelector(Era era,
			     int index = -1,
			     bool debug = false);
  ~GenLeptonSelector() {}

  /**
   * @brief Set cut thresholds
   */
  void set_min_pt_muon(double min_pt_muon);
  void set_max_absEta_muon(double max_absEta_muon);
  void set_min_pt_electron(double min_pt_electron);
  void set_max_absEta_electron(double max_absEta_electron);

  /**
   * @brief Get cut thresholds
   */
  double get_min_pt_muon() const;
  double get_max_absEta_muon() const;
  double get_min_pt_electron() const;
  double get_max_absEta_electron() const;

  /**
   * @brief Check if generator-level lepton given as function argument passes pT and eta cuts (pT > 5 GeV and |eta| < 2.4 for muons, pT > 7 GeV and |eta| < 2.5 for electrons)
   * @return True if lepton passes selection; false otherwise
   */
  bool
  operator()(const GenParticle & lepton) const;

 protected:
  Era era_;
  Double_t min_pt_muon_;         ///< lower cut threshold on pT for muons
  Double_t max_absEta_muon_;     ///< upper cut threshold on absolute value of eta for muons
  Double_t min_pt_electron_;     ///< lower cut threshold on pT for electrons
  Double_t max_absEta_electron_; ///< upper cut threshold on absolute value of eta for electrons
  bool debug_;
};

typedef ParticleCollectionSelector<GenParticle, GenLeptonSelector> GenLeptonCollectionSelector;

#endif // TallinnNtupleProducer_Selectors_GenLeptonCollectionSelector_h

