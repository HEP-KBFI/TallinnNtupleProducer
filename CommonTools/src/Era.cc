#include "TallinnNtupleProducer/CommonTools/interface/Era.h"

#include "TallinnNtupleProducer/CommonTools/interface/cmsException.h" // cmsException()

Era
get_era(const std::string & eraString)
{
  if(eraString == "2016")
  {
    return Era::k2016;
  }
  else if(eraString == "2017")
  {
    return Era::k2017;
  }
  else if(eraString == "2018")
  {
    return Era::k2018;
  }
  throw cmsException(__func__) << "Invalid Configuration parameter 'era' = " << eraString;
}

std::string
get_era(Era era)
{
  switch(era)
  {
    case Era::kUndefined: throw cmsException(__func__, __LINE__) << "Undefined era!";
    case Era::k2016: return "2016";
    case Era::k2017: return "2017";
    case Era::k2018: return "2018";
    default: throw cmsException(__func__, __LINE__) << "Invalid era: " << static_cast<int>(era);
  }
}
