#ifndef TallinnNtupleProducer_CommonTools_electronDefinitions_h
#define TallinnNtupleProducer_CommonTools_electronDefinitions_h

#include <map>    // std::map
#include <string> // std::string

enum class EGammaID {
  Fall17V1noIso, Fall17V1Iso,
  Fall17V2noIso, Fall17V2Iso,
};

enum class EGammaWP {
  WP90, WP80, WPL
};

// read only Fall17V2noIso
extern const std::map<EGammaID, std::string> EGammaID_map;

extern const std::map<EGammaWP, std::string> EGammaWP_map;

#endif // TallinnNtupleProducer_CommonTools_electronDefinitions_h
