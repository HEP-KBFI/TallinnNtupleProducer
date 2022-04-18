#ifndef TallinnNtupleProducer_EvtWeightTools_DYMCNormScaleFactors_h
#define TallinnNtupleProducer_EvtWeightTools_DYMCNormScaleFactors_h

#include "TallinnNtupleProducer/Objects/interface/GenParticle.h" // GenParticle

#include <vector>                                                // std::vector

// forward declarations
class TFile;

enum class Era;

/**
 * @brief SFs applied to Drell-Yan MC, derived by Christian in Z->ll+jets events
 */
class DYMCNormScaleFactors
{
 public:
  DYMCNormScaleFactors(Era era,
                       bool debug = false);
  ~DYMCNormScaleFactors();

  double
  getWeight(const std::vector<GenParticle> & genTauLeptons,
            int nJets,
            int nBLoose,
            int nBMedium,
            int central_or_shift) const;

 protected:
  Era era_;
  bool debug_;

};

#endif // TallinnNtupleProducer_EvtWeightTools_DYMCNormScaleFactors_h
