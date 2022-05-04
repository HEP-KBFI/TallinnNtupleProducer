#include "TallinnNtupleProducer/CommonTools/interface/contains.h"

#include <algorithm> // std::find()

bool
contains(const std::vector<std::string> & list_of_strings,
         const std::string & keyWord)
{
  return std::find(list_of_strings.begin(), list_of_strings.end(), keyWord) != list_of_strings.end();
}

bool
contains(const std::set<std::string> & list_of_strings,
         const std::string & keyWord)
{
  return list_of_strings.find(keyWord) != list_of_strings.end();
}
