#ifndef TallinnNtupleProducer_Readers_ReaderBase_h
#define TallinnNtupleProducer_Readers_ReaderBase_h

#include "FWCore/ParameterSet/interface/ParameterSet.h" // edm::ParameterSet

// forward declarations
class TTree;
enum class Era;

class ReaderBase
{
 public:
  ReaderBase(const edm::ParameterSet & cfg) {}
  virtual ~ReaderBase() {}

  virtual std::vector<std::string>
  setBranchAddresses(TTree * tree) = 0;
};

edm::ParameterSet
makeReader_cfg(Era era, const std::string& branchName, bool isMC);

#endif // TallinnNtupleProducer_Readers_ReaderBase_h
