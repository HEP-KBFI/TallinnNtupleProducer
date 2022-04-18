#ifndef TallinnNtupleProducer_EvtWeightTools_HHWeightInterfaceCouplings_h
#define TallinnNtupleProducer_EvtWeightTools_HHWeightInterfaceCouplings_h

#include <FWCore/ParameterSet/interface/ParameterSet.h>                // edm::ParameterSet

#include "TallinnNtupleProducer/EvtWeightTools/interface/HHCoupling.h" // HHCoupling

enum class HHWeightInterfaceNLOMode { none, v1, v2, v3 };

// forward declarations
class TH2;

class HHWeightInterfaceCouplings
{
 public:
  HHWeightInterfaceCouplings(const edm::ParameterSet & cfg);
  ~HHWeightInterfaceCouplings() {}

  /**
   * @brief Add a scan point
   * @param couplings (kl, kt, c2, cg, c2g, name)
   * @param name Unique label for the coupling array
   */
  void
  add(const HHCoupling & coupling);

  std::map<std::string, HHCoupling>
  getCouplings() const;

  HHCoupling
  getCoupling(const std::string & name) const;

  std::vector<std::string>
  get_bm_names() const;

  std::vector<std::string>
  get_weight_names() const;

  HHWeightInterfaceNLOMode
  nlo_mode() const;

  std::string
  denominator_file_lo() const;

  std::string
  denominator_file_nlo() const;

  std::string
  histtitle() const;

  static TH2 *
  loadDenominatorHist(const std::string & fileName,
                      const std::string & histTitle);

  static double
  getBinContent(const TH2 * const hist,
                double mHH,
                double cosThetaStar);

 private:
  void
  loadScanFile(const std::string & filePath,
               bool isDEBUG);

  std::map<std::string, HHCoupling> couplings_;
  static const std::map<std::string, double> couplings_sm_;

  HHWeightInterfaceNLOMode nlo_mode_;
  const std::string denominator_file_lo_;
  const std::string denominator_file_nlo_;
  const std::string histtitle_;
};

#endif // TallinnNtupleProducer_EvtWeightTools_HHWeightInterfaceCouplings_h
