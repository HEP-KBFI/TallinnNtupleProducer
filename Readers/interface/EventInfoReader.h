#ifndef TallinnNtupleProducer_Readers_EventInfoReader_h
#define TallinnNtupleProducer_Readers_EventInfoReader_h

#include "TallinnNtupleProducer/Readers/interface/ReaderBase.h"         // ReaderBase
#include "TallinnNtupleProducer/Readers/interface/RunLumiEventReader.h" // RunLumiEventReader

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
  setBranchAddresses(TTree * inputTree) override;

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

  std::string branchName_topPtRwgt_;

  RunLumiEventReader * runLumiEventReader_;

  const std::string branchName_genHiggsDecayMode_;
  const std::string branchName_genWeight_;
  const std::string branchName_LHEReweightingWeight_;
  const std::string branchName_nLHEReweightingWeight_;
  const std::string branchName_gen_mHH_;
  const std::string branchName_gen_cosThetaStar_;
  const std::string branchName_htxs_pt_;
  const std::string branchName_htxs_y_;
};

#endif // TallinnNtupleProducer_Readers_EventInfoReader_h
