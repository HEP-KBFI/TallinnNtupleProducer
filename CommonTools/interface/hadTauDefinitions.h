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

extern const std::map<TauID, int> TauID_levels;
extern const std::map<int, std::vector<std::string>> TauID_level_strings;
extern const std::map<TauID, std::string> TauID_names;
extern const std::map<std::string, TauID> TauID_PyMap;

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
