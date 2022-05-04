#ifndef TallinnNtupleProducer_Readers_RunLumiEventReader_h
#define TallinnNtupleProducer_Readers_RunLumiEventReader_h

#include "TallinnNtupleProducer/Readers/interface/ReaderBase.h" // ReaderBase

// forward declarations
class TTree;
class RunLumiEvent;

class RunLumiEventReader : public ReaderBase
{
 public:
  explicit RunLumiEventReader(const edm::ParameterSet & cfg);
  ~RunLumiEventReader() override;

  std::vector<std::string>
  setBranchAddresses(TTree * inputTree) override;

  /**
   * @brief Read branches from tree and use information to fill EventInfo object
   * @return reference to EventInfo object
   */
  const RunLumiEvent &
  read() const;

 protected:
  RunLumiEvent * info_;

  const std::string branchName_run_;
  const std::string branchName_lumi_;
  const std::string branchName_event_;

  // CV: make sure that only one RunLumiEventReader instance exists,
  //     as ROOT cannot handle multiple TTree::SetBranchAddress calls for the same branch.
  static int numInstances_;
  static RunLumiEventReader * gInstance_;
};

#endif // TallinnNtupleProducer_Readers_RunLumiEventReader_h
