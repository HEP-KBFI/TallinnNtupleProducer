#include "TallinnNtupleProducer/CommonTools/interface/topPtRwgt.h"

#include "TallinnNtupleProducer/CommonTools/interface/cmsException.h" // cmsException()

TopPtRwgtOption
get_topPtRwgtOpt(const std::string & str)
{
  if(! str.empty())
  {
    if     (str == "TOP16011")  return TopPtRwgtOption::TOP16011;
    else if(str == "Linear")    return TopPtRwgtOption::Linear;
    else if(str == "Quadratic") return TopPtRwgtOption::Quadratic;
    else if(str == "HighPt")    return TopPtRwgtOption::HighPt;
    else throw cmsException(__func__, __LINE__) << "Invalid reweighting option: " << str;
  }
  return TopPtRwgtOption::Disable;
}
