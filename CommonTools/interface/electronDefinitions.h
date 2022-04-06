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
const std::map<EGammaID, std::string> EGammaID_map = {
//  { EGammaID::Fall17V1noIso, "mvaFall17V1noIso" },
//  { EGammaID::Fall17V1Iso,   "mvaFall17V1Iso"   },
  { EGammaID::Fall17V2noIso, "mvaFall17V2noIso" },
  { EGammaID::Fall17V2Iso,   "mvaFall17V2Iso"   },
};

const std::map<EGammaWP, std::string> EGammaWP_map = {
  { EGammaWP::WP90, "WP90" },
  { EGammaWP::WP80, "WP80" },
  { EGammaWP::WPL,  "WPL"  },
};

#endif // TallinnNtupleProducer_CommonTools_electronDefinitions_h
