#ifndef TallinnNtupleProducer_EvtWeightTools_BtagSFRatioInterface_h
#define TallinnNtupleProducer_EvtWeightTools_BtagSFRatioInterface_h

#include <FWCore/ParameterSet/interface/ParameterSet.h> // edm::ParameterSet

class BtagSFRatioInterface
{
 public:
  BtagSFRatioInterface(const edm::ParameterSet & cfg);
  ~BtagSFRatioInterface();

  double
  get_btagSFRatio(const std::string & central_or_shift, int nselJets) const;

 protected:
  double
  get_btagSFRatio(const std::vector<double> & btagSFRatios, int nselJets) const;

  std::map<std::string, std::vector<double>> btagSFRatios_;
  bool has_central_;
  int max_njets_;
};

#endif // TallinnNtupleProducer_EvtWeightTools_BtagSFRatioInterface_h
