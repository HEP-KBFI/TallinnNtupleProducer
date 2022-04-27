#ifndef TallinnNtupleProducer_Writers_EvtWeightWriter_h
#define TallinnNtupleProducer_Writers_EvtWeightWriter_h

#include "FWCore/ParameterSet/interface/ParameterSet.h"                       // edm::ParameterSet

#include "TallinnNtupleProducer/EvtWeightTools/interface/EvtWeightRecorder.h" // EvtWeightRecorder
#include "TallinnNtupleProducer/Objects/interface/Event.h"                    // Event
#include "TallinnNtupleProducer/Writers/interface/WriterBase.h"               // WriterBase

#include <string>                                                             // std::string
#include <vector>                                                             // std::vector

// forward declarations
class TTree;

class EvtWeightWriter : public WriterBase
{
 public:
  EvtWeightWriter(const edm::ParameterSet & cfg);
  ~EvtWeightWriter();

  /**
   * @brief Call tree->Branch for all branches
   */
  void
  setBranches(TTree * outputTree);
 
  /**
   * @brief Switch branches to those for the central value or for systematic shifts.
   *        This method needs to be called before each call to the "write" method in the event loop.
   */
  void
  set_central_or_shift(const std::string & central_or_shift) const;

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

  struct central_or_shiftEntry
  {
    Float_t evtWeight_;
  };
  std::map<std::string, central_or_shiftEntry> central_or_shiftEntries_; // key = central_or_shift
  mutable central_or_shiftEntry * current_central_or_shiftEntry_;
};

#endif // TallinnNtupleProducer_Writers_EvtWeightWriter_h
