#ifndef TallinnNtupleProducer_EvtWeightTools_LHEVpt_LOtoNLO_h
#define TallinnNtupleProducer_EvtWeightTools_LHEVpt_LOtoNLO_h

#include "TallinnNtupleProducer/CommonTools/interface/AnalysisConfig.h" // AnalysisConfig
#include "TallinnNtupleProducer/Readers/interface/ReaderBase.h"         // ReaderBase

#include <Rtypes.h>                                                     // UChar_t, Float_t

// forward declarations
enum class LHEVptSys;

/**
 * @brief Apply LO-to-NLO corrections to LO DY and W+jets samples,
 *        parametrized as a function of vector boson pT at the LHE level.
 *
 * For further details, see section 4.4.3 in AN2019/229v6, in particular table 12.
 */
class LHEVpt_LOtoNLO : public ReaderBase
{
 public:
  LHEVpt_LOtoNLO(const AnalysisConfig & config,
                 bool isDEBUG = false);
  ~LHEVpt_LOtoNLO();

  std::vector<std::string>
  setBranchAddresses(TTree * tree) override;

  double
  getWeight(LHEVptSys option) const;

 protected:
  struct IdParams {
  public:
    IdParams();
    IdParams(double slope,
             double slope_err,
             double intercept,
             double intercept_err);

    double
    nominal(double pt) const;

    double
    up(double pt) const;

    double
    down(double pt) const;

  protected:
    double slope_;
    double slope_err_;
    double intercept_;
    double intercept_err_;
  };

  std::string branchName_lhe_nb_;
  std::string branchName_lhe_vpt_;

  UChar_t lhe_nb_;
  Float_t lhe_vpt_;

  std::map<UChar_t, IdParams> coefs_;
  bool isDEBUG_;

  static std::map<UChar_t, IdParams> wjets_;
  static std::map<UChar_t, IdParams> dy_;
};

#endif // TallinnNtupleProducer_EvtWeightTools_LHEVpt_LOtoNLO_h
