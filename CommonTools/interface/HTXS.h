#ifndef TallinnNtupleProducer_CommonTools_HTXS_h
#define TallinnNtupleProducer_CommonTools_HTXS_h

#include <Rtypes.h> // Float_t

enum HTXSCategory
{
  kUnknown    = 0,
  kForward    = (1 << 0),
  kPt0to60    = (1 << 1),
  kPt60to120  = (1 << 2),
  kPt120to200 = (1 << 3),
  kPt200to300 = (1 << 4),
  kPt300to450 = (1 << 5),
  kPtGt450    = (1 << 6),
};

class HTXS
{
public:
  HTXS();
  HTXS(Float_t pt,
       Float_t y);
  HTXS(const HTXS & other);

  int
  get_category() const;

  Float_t pt_;
  Float_t y_;
};

#endif // TallinnNtupleProducer_CommonTools_HTXS_h
