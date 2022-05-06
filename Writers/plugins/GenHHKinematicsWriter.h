#ifndef TallinnNtupleProducer_Writers_GenHHKinematicsWriter_h
#define TallinnNtupleProducer_Writers_GenHHKinematicsWriter_h

#include "FWCore/ParameterSet/interface/ParameterSet.h"                       // edm::ParameterSet

#include "TallinnNtupleProducer/EvtWeightTools/interface/EvtWeightRecorder.h" // EvtWeightRecorder
#include "TallinnNtupleProducer/Objects/interface/Event.h"                    // Event
#include "TallinnNtupleProducer/Writers/interface/WriterBase.h"               // WriterBase

#include <string>                                                             // std::string
#include <vector>                                                             // std::vector

// forward declarations
class TTree;

class GenHHKinematicsWriter : public WriterBase
{
 public:
  GenHHKinematicsWriter(const edm::ParameterSet & cfg);
  ~GenHHKinematicsWriter();

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

  Float_t gen_mHH_;
  Float_t gen_cosThetaStar_;
};

#endif // TallinnNtupleProducer_Writers_GenHHKinematicsWriter_h
