#ifndef TallinnNtupleProducer_CommonTools_topPtRwgt_h
#define TallinnNtupleProducer_CommonTools_topPtRwgt_h

#include <string> // std::string

enum class TopPtRwgtOption
{
  Disable,
  TOP16011,
  Linear,
  Quadratic,
  HighPt,
};

TopPtRwgtOption
get_topPtRwgtOpt(const std::string & str);

#endif // TallinnNtupleProducer_CommonTools_topPtRwgt_h
