#ifndef TallinnNtupleProducer_Selectors_RecoHadTauCollectionSelectorECalCrack_h
#define TallinnNtupleProducer_Selectors_RecoHadTauCollectionSelectorECalCrack_h

#include "TallinnNtupleProducer/Objects/interface/Particle.h"                       // Particle::Point
#include "TallinnNtupleProducer/Objects/interface/RecoHadTau.h"                     // RecoHadTau
#include "TallinnNtupleProducer/Objects/interface/RecoVertex.h"                     // RecoVertex
#include "TallinnNtupleProducer/Selectors/interface/RecoHadTauCollectionSelector.h" // RecoHadTauCollectionSelector

class RecoHadTauSelectorECalCrack
{
 public:
  explicit
  RecoHadTauSelectorECalCrack(Era era,
                              int index = -1,
                              bool debug = false);
  ~RecoHadTauSelectorECalCrack() {}

  /**
   * @brief Set primary event vertex
   */
  void set_vertex(const RecoVertex& vertex);

  /**
   * @brief Check if hadronic tau given as function argument is within the crack between ECAL barrel and endcap
   * @return True if hadronic tau passes selection; false otherwise
   */
  bool operator()(const RecoHadTau & hadTau) const;

 protected:
  bool debug_;        ///< enable printout for debugging purposes
  const Era era_;

  Double_t crack_eta_min_; ///< position of the crack between ECAL barrel and endcap,
  Double_t crack_eta_max_; ///< as defined in RecoTauTag/RecoTau/python/PFRecoTauDiscriminationAgainstElectron2_cfi.py

  Particle::Point vertex_position_; ///< position of primary event vertex
};

typedef ParticleCollectionSelector<RecoHadTau, RecoHadTauSelectorECalCrack> RecoHadTauCollectionSelectorECalCrack;

#endif // TallinnNtupleProducer_Selectors_RecoHadTauCollectionSelectorECalCrack_h
