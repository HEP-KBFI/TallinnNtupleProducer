#ifndef TallinnNtupleProducer_CommonTools_hadTauDefinitions_h
#define TallinnNtupleProducer_CommonTools_hadTauDefinitions_h

#include <map>    // std::map
#include <string> // std::string
#include <vector> // std::vector

#define TAU_WP_SEPARATOR   "&"
#define TAU_WP_SEPARATOR_C '&'

//--- declare tau ID discriminators

enum class TauID {
  MVAnewDM2017v2, MVAoldDM, MVAoldDMdR032017v2, MVAoldDM2017v1, MVAoldDM2017v2,
  DeepTau2017v2VSe, DeepTau2017v2VSmu, DeepTau2017v2VSjet
};

const std::map<TauID, int> TauID_levels = {
  { TauID::MVAnewDM2017v2,     7 }, // VVLoose  - 1, VVTight - 7
  { TauID::MVAoldDM,           6 }, // VLoose   - 1, VVTight - 7
  { TauID::MVAoldDMdR032017v2, 7 }, // VVLoose  - 1, VVTight - 7
  { TauID::MVAoldDM2017v1,     7 }, // VVLoose  - 1, VVTight - 7
  { TauID::MVAoldDM2017v2,     7 }, // VVLoose  - 1, VVTight - 7
  { TauID::DeepTau2017v2VSe,   8 }, // VVVLoose - 1, VVTight - 8
  { TauID::DeepTau2017v2VSmu,  4 }, // VLoose   - 1, Tight   - 4
  { TauID::DeepTau2017v2VSjet, 8 }, // VVVLoose - 1, VVTight - 8
};

const std::map<int, std::vector<std::string>> TauID_level_strings = {
  { 1, {                                                     "Tight"                      } },
  { 2, {                                  "Loose",           "Tight"                      } },
  { 4, {                        "VLoose", "Loose", "Medium", "Tight"                      } },
  { 5, {                        "VLoose", "Loose", "Medium", "Tight", "VTight"            } },
  { 6, {                        "VLoose", "Loose", "Medium", "Tight", "VTight", "VVTight" } },
  { 7, {             "VVLoose", "VLoose", "Loose", "Medium", "Tight", "VTight", "VVTight" } },
  { 8, { "VVVLoose", "VVLoose", "VLoose", "Loose", "Medium", "Tight", "VTight", "VVTight" } },
};

const std::map<TauID, std::string> TauID_names = {
  { TauID::MVAnewDM2017v2,     "MVAnewDM2017v2"     },
  { TauID::MVAoldDM,           "MVAoldDM"           },
  { TauID::MVAoldDMdR032017v2, "MVAoldDMdR032017v2" },
  { TauID::MVAoldDM2017v1,     "MVAoldDM2017v1"     },
  { TauID::MVAoldDM2017v2,     "MVAoldDM2017v2"     },
  { TauID::DeepTau2017v2VSe,   "DeepTau2017v2VSe"   },
  { TauID::DeepTau2017v2VSmu,  "DeepTau2017v2VSmu"  },
  { TauID::DeepTau2017v2VSjet, "DeepTau2017v2VSjet" },
};

// NB! must be exactly 7 characters!
const std::map<std::string, TauID> TauID_PyMap = {
  { "dR03mva", TauID::MVAoldDMdR032017v2 },
  { "dR05mva", TauID::MVAoldDM2017v2     },
  { "deepVSj", TauID::DeepTau2017v2VSjet },
  { "deepVSe", TauID::DeepTau2017v2VSe   },
  { "deepVSm", TauID::DeepTau2017v2VSmu  },
};

TauID
get_tau_id_enum(const std::string & tauId_str);

int
get_tau_id_wp_int(const std::string & tauId_str);

int
get_tau_id_wp_int(TauID tauID,
                  const std::string & wp_str);

std::string
get_tau_id_wp_str(TauID tauID,
                  int wp_int);

#endif // TallinnNtupleProducer_CommonTools_hadTauDefinitions_h
