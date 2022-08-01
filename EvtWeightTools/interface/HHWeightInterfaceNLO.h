#ifndef TallinnNtupleProducer_EvtWeightTools_HHWeightInterfaceNLO_h
#define TallinnNtupleProducer_EvtWeightTools_HHWeightInterfaceNLO_h

/**
 * @brief Compute weights to reweight leading-order (LO) non-resonant HH signal MC samples to next-to-leading order (NLO).
 *        The code for the reweighting is based on code ( https://github.com/pmandrik/VSEVA/tree/master/HHWWgg/reweight ) 
 *        developed for the CMS HH->WWgammagamma analysis, using LO and NLO cross-sections 
 *        documented in the arXiv paper https://arxiv.org/pdf/1806.05162.pdf
 *
 */

#include "TallinnNtupleProducer/CommonTools/interface/Era.h"             // Era

#include <FWCore/Utilities/interface/FileInPath.h>                       // edm::FileInPath

#include <map>                                                           // std::map

// forward declarations
class HHWeightInterfaceCouplings;
class TH1;
class TH2;

enum class HHWeightInterfaceNLOMode;

class HHWeightInterfaceNLO
{
 public:
  HHWeightInterfaceNLO(const HHWeightInterfaceCouplings * const couplings,
                       Era era,
                       bool apply_coupling_fix_CMS = false,
                       double max_weight = 10.,
                       bool isDEBUG = false);
  ~HHWeightInterfaceNLO();

  /**
   * @brief Get single Weight
   *
   * The purpose of this function is to reweight the kinematics of events in the LO HH MC samples to the distribution
   * in gen_mHH and cos(theta*) computed at NLO accuracy.
   *
   * The function getWeight_V1(...) returns the weights documented in the arXiv paper https://arxiv.org/pdf/1806.05162.pdf,
   * which are parametrized as function of the observable gen_mHH only, i.e. the distribution in cos(theta*) is not reweighted to NLO accuracy.
   * The function getWeight_V2(...) extends the reweighting to both observables gen_mHH and cos(theta*),
   * and extendds the reweighting in gen_mHH to higher mass (up to 5 TeV, compared to 1040 GeV in the getWeight_V1(...) function)
   *
   * Note: The weights returned by the functions getWeight_LOtoNLO_V1(...) and getWeight_LOtoNLO_V2(...) are meant to be used in combination 
   *       with the weights obtained by the function HHWeightInterfaceLO::getWeight(...).
   */
  double
  getWeight_LOtoNLO(const std::string & bmName,
                    double mHH,
                    double cosThetaStar,
                    bool isDEBUG = false) const;

  double
  getWeight_LOtoNLO_V1(const std::string & bmName,
                       double mHH,
                       double cosThetaStar,
                       bool isDEBUG = false) const;

  double
  getWeight_LOtoNLO_V2(const std::string & bmName,
                       double mHH,
                       double cosThetaStar,
                       bool isDEBUG = false) const;

  double
  getWeight_LOtoNLO_V3(const std::string & bmName,
                       double mHH,
                       double cosThetaStar,
                       bool isDEBUG = false) const;

  /**
   * @brief Get single reWeight
   *
   * The purpose of this function is to switch the NLO corrections from the case of SM kinematics 
   * to the kinematics corresponding to one of the 12 BM scenarios documented in the arXiv paper https://arxiv.org/abs/1507.02245
   *
   * Note: The weights returned by the functions getRelativeWeight_LOtoNLO_V1(...) and getRelativeWeight_LOtoNLO_V2(...) are meant to be used in combination 
   *       with the weights obtained by the functions
   *         HHWeightInterfaceLO::getWeight(...) * HHWeightInterfaceLO::getRelativeWeight(...) * HHWeightInterfaceNLO::getWeight_LOtoNLO(...).
   *
   */
  double
  getRelativeWeight_LOtoNLO(const std::string & bmName,
                            double mHH,
                            double cosThetaStar,
                            bool isDEBUG = false) const;

  double
  getRelativeWeight_LOtoNLO_V1(const std::string & bmName,
                               double mHH,
                               double cosThetaStar,
                               bool isDEBUG = false) const;

  double
  getRelativeWeight_LOtoNLO_V2(const std::string & bmName,
                               double mHH,
                               double cosThetaStar,
                               bool isDEBUG = false) const;

  double
  getRelativeWeight_LOtoNLO_V3(const std::string & bmName,
                               double mHH,
                               double cosThetaStar,
                               bool isDEBUG = false) const;

 /**
   * @brief Get single reWeight
   *
   * The purpose of this function is to reweight the events in the NLO HH MC samples
   * from SM kinematics to one of the 12 benchmark (BM) scenarios discussed on the paper https://arxiv.org/abs/1507.02245
   *
   * The function getRelativeWeight_V1(...) returns the weights documented in the arXiv paper https://arxiv.org/pdf/1806.05162.pdf,
   * which are parametrized as function of the observable gen_mHH only, i.e. the distribution in cos(theta*) is not reweighted to NLO accuracy.
   * The function getRelativeWeight_V2(...) extends the reweighting to both observables gen_mHH and cos(theta*),
   * and extendds the reweighting in gen_mHH to higher mass (up to 5 TeV, compared to 1040 GeV in the getRelativeWeight_NLOtoNLO_V1(...) function)
   *
   * Note: The weights returned by the functions getRelativeWeight_NLOtoNLO_V1(...) and getRelativeWeight_NLOtoNLO_V2(...) 
   *       are meant to be used for studies of the HH reweighting only, NOT in regular analysis code !!
   */
  double
  getRelativeWeight_NLOtoNLO(const std::string & bmName,
                             double mHH,
                             double cosThetaStar,
                             bool isDEBUG = false) const;

  double
  getRelativeWeight_NLOtoNLO_V1(const std::string & bmName,
                                double mHH,
                                double cosThetaStar,
                                bool isDEBUG = false) const;

  double
  getRelativeWeight_NLOtoNLO_V2(const std::string & bmName,
                                double mHH,
                                double cosThetaStar,
                                bool isDEBUG = false) const;

  /**
   * @brief Get histograms used to compute the weights
   *
   * Note: The histograms returned by the functions get_dXsec_V1_lo(), get_dXsec_V1_nlo(), get_dXsec_V2_lo(), and get_dXsec_V2_nlo()
   *       are meant to be used for studies of the HH reweighting only, NOT in regular analysis code !!
   */
  const TH1*
  get_dXsec_V1_lo(const std::string & bmName) const;

  const TH1*
  get_dXsec_V1_nlo(const std::string & bmName) const;

  const TH2*
  get_dXsec_V2_lo(const std::string & bmName) const;

  const TH2*
  get_dXsec_V2_nlo(const std::string & bmName) const;

  double
  get_totalXsec_V1_lo(const std::string & bmName) const;

  double
  get_totalXsec_V1_nlo(const std::string & bmName) const;

  double
  get_totalXsec_V2_lo(const std::string & bmName) const;

  double
  get_totalXsec_V2_nlo(const std::string & bmName) const;

  double
  get_totalXsec_lo(const std::string & bmName) const;

  double
  get_totalXsec_nlo(const std::string & bmName) const;

 private:
  double
  getDenom(double mHH,
           double cosThetaStar) const;

  static double
  get_totalXsec(const std::string & bmName,
                const std::map<std::string, double> & totalXsec);

  const HHWeightInterfaceCouplings * const couplings_;
  const HHWeightInterfaceNLOMode mode_;

  edm::FileInPath xsecFileName_V1_lo_;
  edm::FileInPath xsecFileName_V1_nlo_;

  std::map<std::string, double>      totalXsec_V1_lo_;     // key = bmName
  std::map<std::string, double>      totalXsec_V1_nlo_;    // key = bmName
  std::map<std::string, const TH1 *> dXsec_V1_lo_;         // key = bmName
  std::map<std::string, const TH1 *> dXsec_V1_nlo_;        // key = bmName
  std::map<std::string, const TH1 *> weights_LOtoNLO_V1_;  // key = bmName
  std::map<std::string, const TH1 *> weights_NLOtoNLO_V1_; // key = bmName

  edm::FileInPath xsecFileName_V2_lo_;
  edm::FileInPath xsecFileName_V2_nlo_;

  std::map<std::string, double>      totalXsec_V2_lo_;     // key = bmName
  std::map<std::string, double>      totalXsec_V2_nlo_;    // key = bmName
  std::map<std::string, const TH2 *> dXsec_V2_lo_;         // key = bmName
  std::map<std::string, const TH2 *> dXsec_V2_nlo_;        // key = bmName
  std::map<std::string, const TH2 *> weights_LOtoNLO_V2_;  // key = bmName
  std::map<std::string, const TH2 *> weights_NLOtoNLO_V2_; // key = bmName

  TH2 * sumEvt_;
  int nof_sumEvt_entries_;

  Era era_;
  bool apply_coupling_fix_CMS_;
  double max_weight_;
  
  bool isDEBUG_;
};

#endif // TallinnNtupleProducer_EvtWeightTools_HHWeightInterfaceNLO_h
