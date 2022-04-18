#ifndef TallinnNtupleProducer_EvtWeightTools_LeptonFakeRateInterface_h
#define TallinnNtupleProducer_EvtWeightTools_LeptonFakeRateInterface_h

#include "FWCore/ParameterSet/interface/ParameterSet.h"                   // edm::ParameterSet

#include "TallinnNtupleProducer/CommonTools/interface/sysUncertOptions.h" // kFRl_*

#include <map>                                                            // std::map

// forward declarations
class lutWrapperBase;
class TFile;

class LeptonFakeRateInterface
{
 public:
  LeptonFakeRateInterface(const edm::ParameterSet & cfg);
  ~LeptonFakeRateInterface();

  double
  getWeight_e(double electronPt,
              double electronAbsEta,
              int central_or_shift) const;
  double
  getWeight_mu(double muonPt,
               double muonAbsEta,
               int central_or_shift) const;

 private:
  double jetToEleFakeRateCorr_;
  double jetToMuFakeRateCorr_;
  bool applyNonClosureCorrection_;
  std::map<std::string, TFile *> inputFiles_;

  std::map<int, lutWrapperBase *> lutFakeRate_e_;
  std::map<int, lutWrapperBase *> lutFakeRate_mu_;

  bool isDEBUG_;
};

#endif // TallinnNtupleProducer_EvtWeightTools_LeptonFakeRateInterface_h
