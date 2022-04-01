#include "TallinnNtupleProducer/CommonTools/interface/HTXS.h"

#include <cassert> // assert()
#include <cmath>   // std::fabs()

HTXS::HTXS()
  : HTXS(-1., -999.)
{}

HTXS::HTXS(Float_t pt,
           Float_t y)
  : pt_(pt)
  , y_(y)
{}

HTXS::HTXS(const HTXS & other)
{
  pt_ = other.pt_;
  y_ = other.y_;
}

int
HTXS::get_category() const
{
  assert(pt_ >= 0.);
  int category = HTXSCategory::kUnknown;

  if(std::fabs(y_) < 2.5)
  {
    if(pt_ < 60.)
    {
      category = HTXSCategory::kPt0to60;
    }
    else if(pt_ < 120.)
    {
      category = HTXSCategory::kPt60to120;
    }
    else if(pt_ < 200.)
    {
      category = HTXSCategory::kPt120to200;
    }
    else if(pt_ < 300.)
    {
      category = HTXSCategory::kPt200to300;
    }
    else if(pt_ < 450.)
    {
      category = HTXSCategory::kPt300to450;
    }
    else
    {
      category = HTXSCategory::kPtGt450;
    }
  }
  else
  {
    category = HTXSCategory::kForward;
  }

  return category;
}
