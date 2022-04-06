#include "TallinnNtupleProducer/CommonTools/interface/jetDefinitions.h"

double
get_BtagWP(Era era,
           Btag btag,
           BtagWP wp)
{
  return BtagWP_map.at(era).at(btag).at(wp);
}
