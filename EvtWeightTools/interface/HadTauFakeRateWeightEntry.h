#ifndef TallinnNtupleProducer_EvtWeightTools_HadTauFakeRateWeightEntry_h
#define TallinnNtupleProducer_EvtWeightTools_HadTauFakeRateWeightEntry_h

#include <FWCore/ParameterSet/interface/ParameterSet.h> // edm::ParameterSet

// forward declarations
class TF1;
class TFile;
class TGraphAsymmErrors;

class HadTauFakeRateWeightEntry
{
 public:
  HadTauFakeRateWeightEntry(double absEtaMin,
                            double absEtaMax,
                            const std::string & hadTauSelection,
                            TFile * inputFile,
                            const edm::ParameterSet & cfg,
                            const std::string& trigMatching,
                            int central_or_shift);
  ~HadTauFakeRateWeightEntry();

  // jet->tau fake-rates (product of fake-rates in MC, determined in bins of tau candidate pT and eta, and data/MC scale factors)
  double getWeight(double pt) const;

  // jet->tau fake-rate scale factors (ratio of jet->tau fake-rates in data and MC simulation);
  // to be applied to simulated events in case data-driven "fake" background estimation is applied to leptons only
  double getSF(double pt) const;

  double absEtaMin() const;
  double absEtaMax() const;

 private:
  double absEtaMin_;
  double absEtaMax_;
  std::string hadTauSelection_;
  std::string graphName_;
  TGraphAsymmErrors * graph_;
  double graph_xMin_;
  double graph_xMax_;
  bool applyGraph_;
  std::string fitFunctionName_;
  TF1 * fitFunction_;
  double fitFunction_xMin_;
  double fitFunction_xMax_;
  bool applyFitFunction_;
};

#endif // TallinnNtupleProducer_EvtWeightTools_HadTauFakeRateWeightEntry_h
