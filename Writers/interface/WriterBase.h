#ifndef TallinnNtupleProducer_Writers_WriterBase_h
#define TallinnNtupleProducer_Writers_WriterBase_h

#include "FWCore/ParameterSet/interface/ParameterSet.h"                       // edm::ParameterSet

#include "TallinnNtupleProducer/EvtWeightTools/interface/EvtWeightRecorder.h" // EvtWeightRecorder
#include "TallinnNtupleProducer/Objects/interface/Event.h"                    // Event

#include <string>                                                             // std::string

// forward declarations
class TTree;
class TTreeWrapper;

class WriterBase
{
 public:
  WriterBase(const edm::ParameterSet & cfg);
  virtual ~WriterBase();

  /**
   * @brief Call inputTree->registerReader(reader)
   *        in case writer plugin uses "private" reader instances to read information from inputTree
   *       (this concerns information not handled via the EventReader class)
   */
  virtual
  void
  registerReaders(TTreeWrapper * inputTree);

  /**
   * @brief Call tree->Branch for all branches
   */
  virtual
  void
  setBranches(TTree * outputTree);

  /**
   * @brief Switch branches to those for the central value or for systematic shifts.
   *        This method needs to be called before each call to the "write" method in the event loop.
   */
  virtual
  void
  set_central_or_shift(const std::string & central_or_shift) const;

  /**
   * @brief Write relevant information to tree
   */
  virtual
  void
  write(const Event & event, const EvtWeightRecorder & evtWeightRecorder);

 protected:
  virtual
  void
  writeImp(const Event & event, const EvtWeightRecorder & evtWeightRecorder) = 0;

  std::vector<std::string> supported_systematics_;
  mutable std::string current_central_or_shift_;
};

#include "FWCore/PluginManager/interface/PluginFactory.h"  // edmplugin::PluginFactory

typedef edmplugin::PluginFactory<WriterBase*(const edm::ParameterSet&)> WriterPluginFactory;

#endif // TallinnNtupleProducer_Writers_WriterBase_h
