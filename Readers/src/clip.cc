#include "TallinnNtupleProducer/Readers/interface/clip.h"

#include <algorithm> // std::clamp()

double
clip(double value, double min_value, double max_value)
{
  return std::clamp(value, min_value, max_value);
}
