#ifndef TallinnNtupleProducer_EvtWeightTools_ChargeMisIdRateInterface_h
#define TallinnNtupleProducer_EvtWeightTools_ChargeMisIdRateInterface_h

#include <map>    // std::map
#include <string> // std::string

enum class Era;

// forward declarations
class lutWrapperBase;
class RecoLepton;
class TFile;

class ChargeMisIdRateInterface
{
 public:
  ChargeMisIdRateInterface(Era era,
                           const std::string & lep_mva_wp = "default",
                           bool wTightCharge = true,
                           bool isDEBUG = false);
  ~ChargeMisIdRateInterface();

  double
  get(const RecoLepton * const lepton) const;

  double
  get(const RecoLepton * const lepton_lead, const RecoLepton * const lepton_sublead) const;

 protected:
  std::map<std::string, TFile *> inputFiles_;
  lutWrapperBase * chargeMisId_;

  bool isDEBUG_;
};

#endif // TallinnNtupleProducer_EvtWeightTools_ChargeMisIdRateInterface_h
