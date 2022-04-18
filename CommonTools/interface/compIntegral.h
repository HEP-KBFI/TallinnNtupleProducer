#ifndef TallinnNtupleProducer_CommonTools_compIntegral_h
#define TallinnNtupleProducer_CommonTools_compIntegral_h

// forward declarations
class TH1;

double
compIntegral(const TH1 * histogram,
             bool includeUnderflowBin,
             bool includeOverflowBin);

double
compIntegralErr(const TH1 * histogram,
                bool includeUnderflowBin,
                bool includeOverflowBin);

#endif // TallinnNtupleProducer_CommonTools_compIntegral_h
