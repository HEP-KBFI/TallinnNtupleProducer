#ifndef TallinnNtupleProducer_EvtWeightTools_TauTriggerSFValues_h
#define TallinnNtupleProducer_EvtWeightTools_TauTriggerSFValues_h

#include <iosfwd> // std::ostream

struct TauTriggerSFValues
{
  /**
   * @brief Always ensures that the member variables min, central and max obey min <= central <= max and
   * that the central value always remains central after every linear operation (such as addition, subtraction and
   * multiplication with a scalar). Multiplication of two instances of this class yields another instance of this class.
   */
  TauTriggerSFValues();
  TauTriggerSFValues(double min_,
                     double central_,
                     double max_);

  TauTriggerSFValues &
  max_of(double value);

  bool
  is_ordered() const;

  void
  reset_to_central();

  TauTriggerSFValues &
  operator+=(const TauTriggerSFValues & values);

  double min;
  double central;
  double max;
};

TauTriggerSFValues
operator*(const TauTriggerSFValues & lhs,
          const TauTriggerSFValues & rhs);

TauTriggerSFValues
operator*(double factor,
          const TauTriggerSFValues & values);

TauTriggerSFValues
operator*(const TauTriggerSFValues & values,
          double factor);

TauTriggerSFValues
operator-(double minuend,
          const TauTriggerSFValues & values);

TauTriggerSFValues
operator-(const TauTriggerSFValues & minuend,
          const TauTriggerSFValues & values);

std::ostream &
operator<<(std::ostream & stream,
           const TauTriggerSFValues & values);

namespace aux
{
  TauTriggerSFValues
  min(const TauTriggerSFValues & lhs,
      const TauTriggerSFValues & rhs);

  TauTriggerSFValues
  max(const TauTriggerSFValues & lhs,
      const TauTriggerSFValues & rhs);
}

#endif // TallinnNtupleProducer_EvtWeightTools_TauTriggerSFValues_h
