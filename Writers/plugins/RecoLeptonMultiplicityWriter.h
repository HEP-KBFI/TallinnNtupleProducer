#ifndef TallinnNtupleProducer_Writers_RecoLeptonMultiplicityWriter_h
#define TallinnNtupleProducer_Writers_RecoLeptonMultiplicityWriter_h

#include "FWCore/ParameterSet/interface/ParameterSet.h"                       // edm::ParameterSet

#include "TallinnNtupleProducer/EvtWeightTools/interface/EvtWeightRecorder.h" // EvtWeightRecorder
#include "TallinnNtupleProducer/Objects/interface/Event.h"                    // Event
#include "TallinnNtupleProducer/Writers/interface/WriterBase.h"               // WriterBase

#include <string>                                                             // std::string
#include <vector>                                                             // std::vector

// forward declarations
class TTree;

class RecoLeptonMultiplicityWriter : public WriterBase
{
 public:
  RecoLeptonMultiplicityWriter(const edm::ParameterSet & cfg);
  ~RecoLeptonMultiplicityWriter();

  /**
   * @brief Call tree->Branch for all branches
   */
  void
  setBranches(TTree * outputTree);
 
  /**
   * @brief Return list of systematic uncertainties supported by this plugin
   */
  std::vector<std::string>
  get_supported_systematics(const edm::ParameterSet & cfg);

 private:
  /**
   * @brief Write relevant information to tree
   */
  void
  writeImp(const Event & event, const EvtWeightRecorder & evtWeightRecorder);

  std::string branchName_numLooseLeptonsFull_;
  std::string branchName_numFakeableLeptonsFull_;
  std::string branchName_numTightLeptonsFull_;

  UInt_t nLooseLeptonsFull_;
  UInt_t nFakeableLeptonsFull_;
  UInt_t nTightLeptonsFull_;
};

#endif // TallinnNtupleProducer_Writers_RecoLeptonMultiplicityWriter_h
