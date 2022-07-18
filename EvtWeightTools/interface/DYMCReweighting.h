#ifndef TallinnNtupleProducer_EvtWeightTools_DYMCReweighting_h
#define TallinnNtupleProducer_EvtWeightTools_DYMCReweighting_h

#include "TallinnNtupleProducer/Objects/interface/GenParticle.h" // GenParticle

#include <map>                                                   // std::map
#include <vector>                                                // std::vector

// forward declarations
class lutWrapperBase;
class TFile;

enum class Era;

class DYMCReweighting
{
 public:
  DYMCReweighting(Era era,
                  bool debug = false);
  ~DYMCReweighting();

  // reweight (LO) Drell-Yan MC produced by Madgraph in order to improve modelling of dilepton mass and pT distribution,
  // cf. slides 7-11 of presentation by Alexei Raspereza in HTT meeting on October 10th 2018
  // (https://indico.cern.ch/event/762837/contributions/3172618/attachments/1731302/2798220/Recoils_20181010.pdf )
  double
  getWeight(const std::vector<GenParticle> & genParticles,
            int central_or_shift) const;

 protected:
  Era era_;
  bool debug_;
  std::map<std::string, TFile *> inputFiles_;
  lutWrapperBase * weights_;
};

#endif // TallinnNtupleProducer_EvtWeightTools_DYMCReweighting_h
