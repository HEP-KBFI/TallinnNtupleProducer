#ifndef TallinnNtupleProducer_Writers_TriggerInfoWriter_h
#define TallinnNtupleProducer_Writers_TriggerInfoWriter_h

#include "TallinnNtupleProducer/Writers/interface/WriterBase.h" // WriterBase
#include "TallinnNtupleProducer/Objects/interface/Event.h"      // Event

#include <string>                                               // std::string
#include <vector>                                               // std::vector

class TTree; // forward declaration

class TriggerInfoWriter : public WriterBase
{
 public:
  TriggerInfoWriter(const edm::ParameterSet & cfg);
  ~TriggerInfoWriter();

  /**
   * @brief Call tree->Branch for all branches
   */
  void
  setBranches(TTree * tree);

  /**
   * @brief Write relevant information to tree
   */
  void
  write(const Event & event);
 
 private:
  int PD_;
  std::vector<int> PD_priority_;

  std::string branchName_passesTrigger_;
  bool passesTrigger_;
};

#endif // TallinnNtupleProducer_Writers_TriggerInfoWriter_h
