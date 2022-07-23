#ifndef TallinnNtupleProducer_Objects_EventInfo_h
#define TallinnNtupleProducer_Objects_EventInfo_h

#include "TallinnNtupleProducer/CommonTools/interface/AnalysisConfig.h" // AnalysisConfig
#include "TallinnNtupleProducer/CommonTools/interface/HTXS.h"           // HTXS

#include "FWCore/ParameterSet/interface/ParameterSet.h"                 // edm::ParameterSet

#include <iostream>                                                     // std::ostream
#include <string>                                                       // std::string
#include <map>                                                          // std::map

class EventInfo
{
 public:
  EventInfo();
  EventInfo(const AnalysisConfig & analysisConfig);
  EventInfo(const EventInfo & eventInfo);
  EventInfo &
  operator=(const EventInfo & eventInfo);

  ~EventInfo();

  UInt_t
  run() const;

  UInt_t
  lumi() const;

  ULong64_t
  event() const;

  Float_t
  genWeight() const;

  Float_t
  nof_pileup() const;

  Float_t
  rho() const;

  const AnalysisConfig &
  analysisConfig() const;

  double
  genWeight_tH() const;

  double
  genWeight_tH(double kv,
               double kt,
               double cosa) const;

  double
  genWeight_tH(const std::string & central_or_shift) const;

  double
  genWeight_tH(const std::string & central_or_shift,
               const std::string & name) const;

  void
  loadWeight_tH(const std::vector<edm::ParameterSet> & cfg);

  std::vector<std::string>
  getWeight_tH_str(const std::string & central_or_shift,
                   bool include_sm = false) const;

  std::vector<std::string>
  getWeight_tH_str(bool include_sm = false) const;

  void
  set_refGetWeight(double refGenWeight);

  double
  get_refGenWeight() const;

  void
  set_central_or_shift(const std::string & central_or_shift) const;

  bool
  has_central_or_shift(const std::string & central_or_shift) const;

  bool
  read_htxs() const;

  void
  read_htxs(bool flag);

  int
  get_htxs_category() const;

  bool
  is_initialized() const;

  std::string
  getProductionModeString() const;

  void
  reset_productionMode();

  void
  set_productionMode(int productionMode);

  std::string
  str() const;

  friend std::ostream &
  operator<<(std::ostream & os,
             const EventInfo & info);

  friend class EventInfoReader;

 protected:
  const AnalysisConfig * analysisConfig_;

  mutable std::string central_or_shift_;
  std::string process_string_;

  UInt_t    run_;                 ///< run number
  UInt_t    lumi_;                ///< luminosity
  ULong64_t event_;               ///< event number
  Float_t   genWeight_;           ///< generator-level weight (only if MC)
  Float_t   nof_pileup_;          ///< true number of pileup interactions (only if MC)
  Float_t   rho_;                 ///< rho from all PF candidates

  UInt_t nLHEReweightingWeight_;
  Float_t * LHEReweightingWeight_;

  const unsigned int LHEReweightingWeight_max_;
  bool is_owner_;

  HTXS htxs_;
  bool read_htxs_;

  std::map<std::string, std::map<std::string, std::pair<int, double>>> tH_sf_;
  static const std::map<Int_t, std::string> gProductionMode_idString_singleHiggs;

  double refGenWeight_;
  int productionMode_;

  void
  copy(const EventInfo & eventInfo);
};

#endif // TallinnNtupleProducer_Objects_EventInfo_h
