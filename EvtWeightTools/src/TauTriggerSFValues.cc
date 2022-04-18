#include "TallinnNtupleProducer/EvtWeightTools/interface/TauTriggerSFValues.h"

#include <algorithm> // std::max()
#include <assert.h>  // assert()
#include <iostream>  // std::ostream

TauTriggerSFValues::TauTriggerSFValues()
  : TauTriggerSFValues(0., 0., 0.)
{}

TauTriggerSFValues::TauTriggerSFValues(double min_,
                                       double central_,
                                       double max_)
  : min(min_)
  , central(central_)
  , max(max_)
{
  assert(min <= central && central <= max);
}

TauTriggerSFValues &
TauTriggerSFValues::max_of(double value)
{
  // if min <= central <= max, then taking max is guaranteed to preserve the order
  min     = std::max(min,     value);
  central = std::max(central, value);
  max     = std::max(max,     value);
  return *this;
}

bool
TauTriggerSFValues::is_ordered() const
{
  return min <= central && central <= max;
}

void
TauTriggerSFValues::reset_to_central()
{
  min = central;
  max = central;
}

TauTriggerSFValues &
TauTriggerSFValues::operator+=(const TauTriggerSFValues & values)
{
  min     += values.min;
  central += values.central;
  max     += values.max;
  return *this;
}

TauTriggerSFValues
operator*(const TauTriggerSFValues & lhs,
          const TauTriggerSFValues & rhs)
{
  const double central = lhs.central * rhs.central;
  const double min_min = lhs.min * rhs.min;
  const double min_max = lhs.min * rhs.max;
  const double max_min = lhs.max * rhs.min;
  const double max_max = lhs.max * rhs.max;
  const double true_min = std::min(std::min(min_min, min_max), std::min(max_min, max_max));
  const double true_max = std::max(std::max(min_min, min_max), std::max(max_min, max_max));
  assert(true_min <= central && central <= true_max);
  return { true_min, central, true_max };
}

TauTriggerSFValues
operator*(double factor,
          const TauTriggerSFValues & values)
{
  return values * factor;
}

TauTriggerSFValues
operator*(const TauTriggerSFValues & values,
          double factor)
{
  const double min_value     = values.min     * factor;
  const double central_value = values.central * factor;
  const double max_value     = values.max     * factor;
  if(min_value <= central_value && central_value <= max_value)
  {
    return { min_value, central_value, max_value };
  }
  else if(min_value >= central_value && central_value >= max_value)
  {
    return { max_value, central_value, min_value };
  }
  else
  {
    assert(0);
  }
}

TauTriggerSFValues
operator-(double minuend,
          const TauTriggerSFValues & values)
{
  const double min_value     = minuend - values.min;
  const double central_value = minuend - values.central;
  const double max_value     = minuend - values.max;
  if(min_value <= central_value && central_value <= max_value)
  {
    return { min_value, central_value, max_value };
  }
  else if(min_value >= central_value && central_value >= max_value)
  {
    return { max_value, central_value, min_value };
  }
  else
  {
    assert(false);
  }
}

TauTriggerSFValues
operator-(const TauTriggerSFValues & minuend,
          const TauTriggerSFValues & values)
{
  const double central_value = minuend.central - values.central;
  const double min_min       = minuend.min - values.min;
  const double min_max       = minuend.min - values.max;
  const double max_min       = minuend.max - values.min;
  const double max_max       = minuend.max - values.max;
  const double min_value     = std::min(std::min(min_min, min_max), std::min(max_min, max_max));
  const double max_value     = std::max(std::max(min_min, min_max), std::max(max_min, max_max));
  return { min_value, central_value, max_value };
}

std::ostream &
operator<<(std::ostream & stream,
           const TauTriggerSFValues & values)
{
  stream << values.min << " / " << values.central << " / " << values.max;
  return stream;
}

namespace aux
{
  TauTriggerSFValues
  min(const TauTriggerSFValues & lhs,
      const TauTriggerSFValues & rhs)
  {
    const double min_value     = std::min(lhs.min,     rhs.min);
    const double central_value = std::min(lhs.central, rhs.central);
    const double max_value     = std::min(lhs.max,     rhs.max);
    return { min_value, central_value, max_value };
  }

  TauTriggerSFValues
  max(const TauTriggerSFValues & lhs,
      const TauTriggerSFValues & rhs)
  {
    const double min_value     = std::max(lhs.min,     rhs.min);
    const double central_value = std::max(lhs.central, rhs.central);
    const double max_value     = std::max(lhs.max,     rhs.max);
    return { min_value, central_value, max_value };
  }
}
