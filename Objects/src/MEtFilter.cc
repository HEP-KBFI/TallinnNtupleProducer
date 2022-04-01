#include "TallinnNtupleProducer/Objects/interface/MEtFilter.h"

MEtFilter::MEtFilter()
{
  flags_.fill(false);
}

bool
MEtFilter::getFlag(MEtFilterFlag flag) const
{
  return flags_[flag];
}

bool &
MEtFilter::getFlagRef(MEtFilterFlag flag)
{
  return flags_[flag];
}
