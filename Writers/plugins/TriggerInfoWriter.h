#ifndef TallinnNtupleProducer_Writers_TriggerInfoWriter_h
#define TallinnNtupleProducer_Writers_TriggerInfoWriter_h

#include "FWCore/ParameterSet/interface/ParameterSet.h"         // edm::ParameterSet

#include "TallinnNtupleProducer/Writers/interface/WriterBase.h" // WriterBase
#include "TallinnNtupleProducer/Objects/interface/Event.h"      // Event

#include <string>                                               // std::string
#include <vector>                                               // std::vector

// forward declarations
class TTree;

class TriggerInfoWriter : public WriterBase
{
 public:
  TriggerInfoWriter(const edm::ParameterSet & cfg);
  ~TriggerInfoWriter();

  /**
   * @brief Call tree->Branch for all branches
   */
  void
  setBranches(TTree * outputTree);
 
  /**
   * @brief Return list of systematic uncertainties supported by this plugin
   */
  std::vector<std::string>
  get_supported_systematics();

 private:
  /**
   * @brief Write relevant information to tree
   */
  void
  writeImp(const Event & event, const EvtWeightRecorder & evtWeightRecorder);

  int PD_;
  std::vector<int> PD_priority_;
  bool passesTrigger_;
};

#endif // TallinnNtupleProducer_Writers_TriggerInfoWriter_h
