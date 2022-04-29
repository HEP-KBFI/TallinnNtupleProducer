#ifndef TallinnNtupleProducer_Readers_EventInfoReader_h
#define TallinnNtupleProducer_Readers_EventInfoReader_h

#include "TallinnNtupleProducer/Readers/interface/ReaderBase.h" // ReaderBase

// forward declarations
class TTree;
class EventInfo;
class AnalysisConfig;

class EventInfoReader : public ReaderBase
{
 public:
  explicit EventInfoReader(const edm::ParameterSet & cfg);
  ~EventInfoReader() override;

  std::vector<std::string>
  setBranchAddresses(TTree * tree) override;

  void
  set_central_or_shift(const std::string& central_or_shift);

  void
  setTopPtRwgtBranchName(const std::string & branchName);

  std::string
  getTopPtRwgtBranchName() const;

  /**
   * @brief Read branches from tree and use information to fill EventInfo object
   * @return reference to EventInfo object
   */
  const EventInfo &
  read() const;

  /**
    * @brief Return list of systematic uncertainties supported by EventInfoReader class
    */
  static
  std::vector<std::string>
  get_supported_systematics(const edm::ParameterSet & cfg);

 protected:
  bool read_genHiggsDecayMode_;
  bool read_puWeight_;

  const AnalysisConfig * analysisConfig_;
  EventInfo * info_;

  std::string branchName_topPtRwgt;

 public:
  const std::string branchName_run;
  const std::string branchName_lumi;
  const std::string branchName_event;
  const std::string branchName_genHiggsDecayMode;
  const std::string branchName_genWeight;
  const std::string branchName_LHEReweightingWeight;
  const std::string branchName_nLHEReweightingWeight;
  const std::string branchName_gen_mHH;
  const std::string branchName_gen_cosThetaStar;
  const std::string branchName_htxs_pt;
  const std::string branchName_htxs_y;
};

#endif // TallinnNtupleProducer_Readers_EventInfoReader_h
