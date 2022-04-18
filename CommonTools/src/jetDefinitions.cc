#include "TallinnNtupleProducer/CommonTools/interface/jetDefinitions.h"

#include "TallinnNtupleProducer/CommonTools/interface/cmsException.h" // cmsException

pileupJetID
get_pileupJetID(const std::string & pileupJetID_str)
{
  if     (pileupJetID_str == "disabled") return pileupJetID::kPileupJetID_disabled;
  else if(pileupJetID_str == "loose"   ) return pileupJetID::kPileupJetID_loose;
  else if(pileupJetID_str == "medium"  ) return pileupJetID::kPileupJetID_medium;
  else if(pileupJetID_str == "tight"   ) return pileupJetID::kPileupJetID_tight;
  else throw cmsException(__func__, __LINE__) << "Invalid argument: " << pileupJetID_str;
}

double
get_BtagWP(Era era,
           Btag btag,
           BtagWP wp)
{
  return BtagWP_map.at(era).at(btag).at(wp);
}
