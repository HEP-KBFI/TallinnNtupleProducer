#ifndef TallinnNtupleProducer_EvtWeightTools_hadTauFakeRateAuxFunctions_h
#define TallinnNtupleProducer_EvtWeightTools_hadTauFakeRateAuxFunctions_h

#include <string>                                               // std::string
#include <vector>                                               // std::vector

std::string
getEtaBin(double minAbsEta,
          double maxAbsEta);

std::string
getPtBin(double minPt,
         double maxPt);

int
getTrigMatchingOption_2016(const std::string& trigMatching);
int
getTrigMatchingOption_2017and2018(const std::string& trigMatching);

#endif // TallinnNtupleProducer_EvtWeightTools_hadTauFakeRateAuxFunctions_h
