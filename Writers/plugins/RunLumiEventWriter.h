#ifndef TallinnNtupleProducer_Writers_RunLumiEventWriter_h
#define TallinnNtupleProducer_Writers_RunLumiEventWriter_h

#include "FWCore/ParameterSet/interface/ParameterSet.h"                       // edm::ParameterSet

#include "TallinnNtupleProducer/Objects/interface/Event.h"                    // Event
#include "TallinnNtupleProducer/Writers/interface/WriterBase.h"               // WriterBase

#include <string>                                                             // std::string
#include <vector>                                                             // std::vector

// forward declarations
class TTree;

class RunLumiEventWriter : public WriterBase
{
 public:
  RunLumiEventWriter(const edm::ParameterSet & cfg);
  ~RunLumiEventWriter();

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

  UInt_t run_;
  UInt_t lumi_;
  ULong64_t event_;
};

#endif // TallinnNtupleProducer_Writers_RunLumiEventWriter_h


