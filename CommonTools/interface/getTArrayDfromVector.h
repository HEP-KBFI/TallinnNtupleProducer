#ifndef TallinnNtupleProducer_CommonTools_getTArrayDfromVector_h
#define TallinnNtupleProducer_CommonTools_getTArrayDfromVector_h

#include "TArrayD.h" // TArrayD

#include <vector>    // std::vector

TArrayD
getTArrayDfromVector(const std::vector<double> & histogramBinning);

#endif // TallinnNtupleProducer_CommonTools_getTArrayDfromVector_h
