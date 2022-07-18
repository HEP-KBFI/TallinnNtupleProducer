#include "TallinnNtupleProducer/CommonTools/interface/hadTauDefinitions.h"

#include "TallinnNtupleProducer/CommonTools/interface/cmsException.h" // cmsException()
#include "TallinnNtupleProducer/CommonTools/interface/as_integer.h"   // as_integer()

#include <algorithm>                                                  // std::find()
#include <assert.h>                                                   // assert()

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
  { TauID::DeepTau2017v2VSe,   "DeepTau2017v2p1VSe"   },
  { TauID::DeepTau2017v2VSmu,  "DeepTau2017v2p1VSmu"  },
  { TauID::DeepTau2017v2VSjet, "DeepTau2017v2p1VSjet" },
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
get_tau_id_enum(const std::string & tauId_str)
{
  const std::string wp_type = tauId_str.substr(0, 7);
  if(! TauID_PyMap.count(wp_type))
  {
    throw cmsException(__func__, __LINE__) << "Unrecognizable tau ID selection: " << tauId_str;
  }
  return TauID_PyMap.at(wp_type);
}

int
get_tau_id_wp_int(const std::string & tauId_str)
{
  if ( tauId_str != "" ) return get_tau_id_wp_int(get_tau_id_enum(tauId_str), tauId_str.substr(7));
  else                   return -1;
}

int
get_tau_id_wp_int(TauID tauID,
                  const std::string & wp_str)
{
  if ( wp_str != "" )
  {
    assert(TauID_levels.count(tauID));
    const int nof_levels = TauID_levels.at(tauID);
    assert(TauID_level_strings.count(nof_levels));
    const std::vector<std::string> & levels = TauID_level_strings.at(nof_levels);
    const auto wp_str_it = std::find(levels.begin(), levels.end(), wp_str);
    if(wp_str_it == levels.end())
    {
      throw cmsException(__func__, __LINE__)
        << "Invalid tau ID string requested for tau ID " << as_integer(tauID) << ": " << wp_str
      ;
    }
    return static_cast<int>(std::distance(levels.begin(), wp_str_it)) + 1;
  }
  else
  {
    return -1;
  }
}

std::string
get_tau_id_wp_str(TauID tauID,
                  int wp_int)
{
  if ( wp_int != -1 )
  {
    assert(TauID_levels.count(tauID));
    const int nof_levels = TauID_levels.at(tauID);
    if(wp_int < 1 || wp_int > nof_levels)
    {
      throw cmsException(__func__, __LINE__)
        << "Invalid level requested for tau ID " << as_integer(tauID) << ": " << wp_int
      ;
    }
    assert(TauID_level_strings.count(nof_levels));
    const std::vector<std::string> & levels = TauID_level_strings.at(nof_levels);
    assert(static_cast<int>(levels.size()) == nof_levels);
    return levels.at(wp_int - 1);
  }
  else
  {
    return "";
  }
}
