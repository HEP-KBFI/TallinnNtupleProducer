#include "TallinnNtupleProducer/CommonTools/interface/get_ignore_ak8_sys.h"

#include "TallinnNtupleProducer/CommonTools/interface/cmsException.h"   // cmsException
#include "TallinnNtupleProducer/CommonTools/interface/jetDefinitions.h" // kFatJet*

int
get_ignore_ak8_sys(const std::vector<std::string> & corrections)
{
  int code = 0;
  for(const std::string & correction: corrections)
  {
    if(correction == "JMS")
    {
      code |= kFatJetJMS;
    }
    else if(correction == "JMR")
    {
      code |= kFatJetJMR;
    }
    else if(correction == "PUPPI")
    {
      code |= kFatJetPUPPI;
    }
    else
    {
      throw cmsException(__func__, __LINE__) << "Invalid correction name: " << correction;
    }
  }
  return code;
}
