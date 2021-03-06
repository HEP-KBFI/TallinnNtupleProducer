#include "TallinnNtupleProducer/Readers/interface/ReaderBase.h"

#include "TallinnNtupleProducer/CommonTools/interface/Era.h" // Era, get_era()

edm::ParameterSet
makeReader_cfg(Era era, const std::string& branchName, bool isMC, bool redoGenMatching)
{
  edm::ParameterSet cfg;
  cfg.addParameter<std::string>("era", get_era(era));
  cfg.addParameter<std::string>("branchName", branchName);
  cfg.addParameter<bool>("isMC", isMC);
  cfg.addParameter<bool>("redoGenMatching", redoGenMatching);
  return cfg;
}
