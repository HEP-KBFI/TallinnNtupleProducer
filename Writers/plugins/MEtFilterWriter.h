#ifndef TallinnNtupleProducer_Writers_MEtFilterWriter_h
#define TallinnNtupleProducer_Writers_MEtFilterWriter_h

#include "FWCore/ParameterSet/interface/ParameterSet.h"                       // edm::ParameterSet

#include "TallinnNtupleProducer/EvtWeightTools/interface/EvtWeightRecorder.h" // EvtWeightRecorder
#include "TallinnNtupleProducer/Objects/interface/Event.h"                    // Event
#include "TallinnNtupleProducer/Writers/interface/WriterBase.h"               // WriterBase

#include <string>                                                             // std::string
#include <vector>                                                             // std::vector

// forward declarations
class MEtFilterSelector;
class TTree;

class MEtFilterWriter : public WriterBase
{
 public:
  MEtFilterWriter(const edm::ParameterSet & cfg);
  ~MEtFilterWriter();

  /**
   * @brief Call tree->Branch for all branches
   */
  void
  setBranches(TTree * outputTree);
 
 private:
  /**
   * @brief Write relevant information to tree
   */
  void
  writeImp(const Event & event, const EvtWeightRecorder & evtWeightRecorder);
  
  std::string branchName_;

  MEtFilterSelector* metFilterSelector_;

  bool passesMEtFilters_;
};

#endif // TallinnNtupleProducer_Writers_MEtFilterWriter_h
