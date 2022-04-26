#include "TallinnNtupleProducer/CommonTools/interface/get_htxs_binning.h"

#include "TallinnNtupleProducer/CommonTools/interface/HTXS.h" // HTXSCategory::

std::vector<std::pair<std::string, int>>
get_htxs_binning(bool isMC_ttH)
{
  std::vector<std::pair<std::string, int>> binning;
  if(isMC_ttH)
  {
    binning = {
      { "fwd",        HTXSCategory::kForward    },
      { "pt0to60",    HTXSCategory::kPt0to60    },
      { "pt60to120",  HTXSCategory::kPt60to120  },
      { "pt120to200", HTXSCategory::kPt120to200 },
      { "pt200to300", HTXSCategory::kPt200to300 },
      { "ptGt300",    HTXSCategory::kPt300to450 | HTXSCategory::kPtGt450 },
      { "pt300to450", HTXSCategory::kPt300to450 },
      { "ptGt450",    HTXSCategory::kPtGt450    },
    };
  }
  return binning;
}
