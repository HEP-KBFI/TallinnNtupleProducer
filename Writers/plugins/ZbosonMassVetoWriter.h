#ifndef TallinnNtupleProducer_Writers_ZbosonMassVetoWriter_h
#define TallinnNtupleProducer_Writers_ZbosonMassVetoWriter_h

#include "FWCore/ParameterSet/interface/ParameterSet.h"                       // edm::ParameterSet

#include "TallinnNtupleProducer/EvtWeightTools/interface/EvtWeightRecorder.h" // EvtWeightRecorder
#include "TallinnNtupleProducer/Objects/interface/Event.h"                    // Event
#include "TallinnNtupleProducer/Writers/interface/WriterBase.h"               // WriterBase

#include <string>                                                             // std::string
#include <vector>                                                             // std::vector

// forward declarations
class TTree;

class ZbosonMassVetoWriter : public WriterBase
{
 public:
  ZbosonMassVetoWriter(const edm::ParameterSet & cfg);
  ~ZbosonMassVetoWriter();

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

  std::string branchName_;

  double z_mass_;
  double z_window_;
  bool requireOS_;

  Bool_t passesZbosonMassVeto_;
};

#endif // TallinnNtupleProducer_Writers_ZbosonMassVetoWriter_h
