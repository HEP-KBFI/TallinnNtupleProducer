#ifndef TallinnNtupleProducer_EvtWeightTools_JetToTauFakeRateInterface_h
#define TallinnNtupleProducer_EvtWeightTools_JetToTauFakeRateInterface_h

#include "TallinnNtupleProducer/CommonTools/interface/sysUncertOptions.h"             // kFRjt_*
#include "TallinnNtupleProducer/EvtWeightTools/interface/HadTauFakeRateWeightEntry.h" // HadTauFakeRateWeightEntry

#include <vector>                                                                     // std::vector

class HadTauFakeRateInterface
{
 public:
  HadTauFakeRateInterface(const edm::ParameterSet & cfg, const std::string& trigMatching = "withoutTriggerMatching");
  ~HadTauFakeRateInterface();

  // jet->tau fake-rates (product of fake-rates in MC, determined in bins of tau candidate pT and eta, and data/MC scale factors)
  double
  getWeight(int idxHadTau,
            double hadTauPt,
            double hadTauAbsEta,
            int central_or_shift) const;

  // jet->tau fake-rate scale factors (ratio of jet->tau fake-rates in data and MC simulation);
  // to be applied to simulated events in case data-driven "fake" background estimation is applied to leptons only
  double
  getSF(int idxHadTau, 
        double hadTauPt,
        double hadTauAbsEta,
        int central_or_shift) const;

 protected:
  enum { kWeight, kSF };

  double
  getWeight_or_SF(double hadTauPt_lead,
                  double hadTauAbsEta_lead,
                  int mode,
                  int order,
                  int central_or_shift) const;

 private:
  TFile * inputFile_;
  std::map<int, std::vector<HadTauFakeRateWeightEntry*>> hadTauFakeRateWeights_lead_;
  bool isInitialized_lead_;
  std::map<int, std::vector<HadTauFakeRateWeightEntry*>> hadTauFakeRateWeights_sublead_;
  bool isInitialized_sublead_;
  std::map<int, std::vector<HadTauFakeRateWeightEntry*>> hadTauFakeRateWeights_third_;
  bool isInitialized_third_;
  std::map<int, std::vector<HadTauFakeRateWeightEntry*>> hadTauFakeRateWeights_fourth_;
  bool isInitialized_fourth_;
};

#endif // TallinnNtupleProducer_EvtWeightTools_HadTauFakeRateInterface_h
