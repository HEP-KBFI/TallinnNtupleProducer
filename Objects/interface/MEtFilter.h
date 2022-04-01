#ifndef TallinnNtupleProducer_Objects_MEtFilter_h
#define TallinnNtupleProducer_Objects_MEtFilter_h

#include "TallinnNtupleProducer/Objects/interface/MEtFilterFlag.h" // MEtFilterFlag::

#include <array>                                                   // std::array

class MEtFilter
{
 public:
  MEtFilter();

  bool
  getFlag(MEtFilterFlag flag) const;

  friend class MEtFilterReader;

 protected:
  // for MEtFilterReader
  bool &
  getFlagRef(MEtFilterFlag flag);

  std::array<bool, MEtFilterFlag::LAST> flags_;
};

#endif // TallinnNtupleProducer_Objects_MEtFilter_h
