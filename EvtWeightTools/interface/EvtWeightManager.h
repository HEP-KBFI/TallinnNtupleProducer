#ifndef TallinnNtupleProducer_EvtWeightTools_EvtWeightManager_h
#define TallinnNtupleProducer_EvtWeightTools_EvtWeightManager_h

#include <FWCore/ParameterSet/interface/ParameterSet.h>         // edm::ParameterSet

#include "TallinnNtupleProducer/Readers/interface/ReaderBase.h" // ReaderBase

#include <Rtypes.h>                                             // UChar_t, Float_t

// forward declarations
class TFile;
class TH1;
class TH2;

class EvtWeightManager : public ReaderBase
{
 public:
  EvtWeightManager(const edm::ParameterSet & cfg, bool isDebug = false);
  ~EvtWeightManager();

  std::vector<std::string>
  setBranchAddresses(TTree * tree) override;

  double
  getWeight(const std::string & central_or_shift) const;

  double
  getWeight() const;

  int
  getDimension() const;

  TH1 *
  getHistogram_1D(const std::string & central_or_shift) const;

  TH1 *
  getHistogram_1D() const;

  TH2 *
  getHistogram_2D(const std::string & central_or_shift) const;

  TH2 *
  getHistogram_2D() const;

  int
  getBinIdx_1D(const std::string & central_or_shift) const;

  int
  getBinIdx_1D() const;

  std::pair<int, int>
  getBinIdx_2D(const std::string & central_or_shift) const;

  std::pair<int, int>
  getBinIdx_2D() const;

  void
  set_central_or_shift(const std::string & central_or_shift);

  bool
  has_central_or_shift(const std::string & central_or_shift) const;

  std::string
  get_x_var() const;

  std::string
  get_y_var() const;

protected:
  std::string binnedHistogram_varName_x_;
  std::string binnedHistogram_varName_y_;
  std::string binnedHistogram_varType_x_;
  std::string binnedHistogram_varType_y_;

  UChar_t var_x_uchar_;
  Float_t var_x_float_;
  UChar_t var_y_uchar_;
  Float_t var_y_float_;

  TFile * histogram_file_;
  std::map<std::string, TH1 *> binnedHistogram_1var_;
  std::map<std::string, TH2 *> binnedHistogram_2var_;

  bool isDebug_;
  std::string central_or_shift_;
};

#endif // EVTWEIGHTMANAGER_H
