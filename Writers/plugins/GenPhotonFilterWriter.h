#ifndef TallinnNtupleProducer_Writers_GenPhotonFilterWriter_h
#define TallinnNtupleProducer_Writers_GenPhotonFilterWriter_h

#include "FWCore/ParameterSet/interface/ParameterSet.h"                       // edm::ParameterSet

#include "TallinnNtupleProducer/EvtWeightTools/interface/EvtWeightRecorder.h" // EvtWeightRecorder
#include "TallinnNtupleProducer/Objects/interface/Event.h"                    // Event
#include "TallinnNtupleProducer/Writers/interface/WriterBase.h"               // WriterBase

// forward declarations
class GenParticleReader;
class GenPhotonFilter;
class TTree;
class TTreeWrapper;

class GenPhotonFilterWriter : public WriterBase
{
 public:
  GenPhotonFilterWriter(const edm::ParameterSet & cfg);
  ~GenPhotonFilterWriter();

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

  /// name of branch in outputTree
  std::string branchName_;

  GenPhotonFilter * genPhotonFilter_;
  bool apply_genPhotonFilter_;

  bool passesGenPhotonFilter_;
};

#endif // TallinnNtupleProducer_Writers_GenPhotonFilterFilterWriter_h
