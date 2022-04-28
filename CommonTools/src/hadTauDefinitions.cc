#include "TallinnNtupleProducer/CommonTools/interface/hadTauDefinitions.h"

#include "TallinnNtupleProducer/CommonTools/interface/cmsException.h" // cmsException()
#include "TallinnNtupleProducer/CommonTools/interface/as_integer.h"   // as_integer()

#include <algorithm>                                                  // std::find()
#include <assert.h>                                                   // assert()

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
