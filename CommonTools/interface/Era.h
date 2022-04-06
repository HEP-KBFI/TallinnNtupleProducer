#ifndef TallinnNtupleProducer_CommonTools_Era_h
#define TallinnNtupleProducer_CommonTools_Era_h

#include <string> // std::string

//--- declare data-taking periods
enum class Era
{
  kUndefined, k2016, k2017, k2018
};

Era
get_era(const std::string & eraString);

std::string
get_era(Era era);

#endif // TallinnNtupleProducer_CommonTools_Era_h
