#ifndef TallinnNtupleProducer_EvtWeightTools_TauTriggerSFInterface_h
#define TallinnNtupleProducer_EvtWeightTools_TauTriggerSFInterface_h

#include "TallinnNtupleProducer/EvtWeightTools/interface/TauTriggerSFValues.h" // TauTriggerSFValues

#include <functional>                                                          // std::function
#include <string>                                                              // std::string

// forward declarations
class TauTriggerSFs2017; // for MVAv2 tau ID
namespace tau_trigger 
{
  class SFProvider; // for DeepTau ID
}
enum class TriggerSFsys;

enum class TauTriggerType
{
  None, DiTau, ETau, MuTau
};

class TauTriggerSFInterface
{
 public:
  TauTriggerSFInterface(TauTriggerType triggerType = TauTriggerType::None);
  TauTriggerSFInterface(const std::string & era_str,
                        const std::string & hadTauSelection,
                        TauTriggerType triggerType);

  double
  getTauTriggerEvalData(TriggerSFsys central_or_shift,
                        double pt,
                        double eta,
                        double phi,
                        int dm) const;

  double
  getTauTriggerEvalMC(TriggerSFsys central_or_shift,
                      double pt,
                      double eta,
                      double phi,
                      int dm,
                      bool flip = true) const;

  TauTriggerSFValues
  getTauTriggerEvalData(double pt,
                        double eta,
                        double phi,
                        int dm) const;

  TauTriggerSFValues
  getTauTriggerEvalMC(double pt,
                      double eta,
                      double phi,
                      int dm) const;

 protected:
  static std::string
  tauTriggerType_toStr(TauTriggerType triggerType);

  TriggerSFsys
  getGenericTriggerSFsys(TriggerSFsys central_or_shift) const;

  TauTriggerSFs2017 * eff_mvav2_;
  tau_trigger::SFProvider * eff_deep_;
  TauTriggerType triggerType_;
};

#endif // TallinnNtupleProducer_EvtWeightTools_TauTriggerSFInterface_h
