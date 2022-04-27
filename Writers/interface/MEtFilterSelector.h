#ifndef TallinnNtupleProducer_Writers_MEtFilterSelector_h
#define TallinnNtupleProducer_Writers_MEtFilterSelector_h

#include <FWCore/ParameterSet/interface/ParameterSet.h>        // edm::ParameterSet

#include "TallinnNtupleProducer/Objects/interface/MEtFilter.h" // MEtFilter

class MEtFilterSelector 
{
 public:
  explicit MEtFilterSelector(const edm::ParameterSet & cfg,
                             bool isMC,
                             bool verbose = false);

  virtual ~MEtFilterSelector() {}

  // Returns value true if the event passes all the filters that are applied 
  bool
  operator()(const MEtFilter & metFilter) const;

 protected:
  std::array<bool, MEtFilterFlag::LAST> apply_;
  bool verbose_;
};

#endif // TallinnNtupleProducer_Writers_MEtFilterSelector_h
