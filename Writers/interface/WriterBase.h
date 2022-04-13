#ifndef TallinnNtupleProducer_Writers_WriterBase_h
#define TallinnNtupleProducer_Writers_WriterBase_h

#include "FWCore/ParameterSet/interface/ParameterSet.h"    // edm::ParameterSet

#include "TallinnNtupleProducer/Objects/interface/Event.h" // Event

// forward declarations
class TTree;

class WriterBase
{
 public:
  WriterBase(const edm::ParameterSet & cfg);
  virtual ~WriterBase();

  /**
   * @brief Call tree->Branch for all branches
   */
  virtual
  void
  setBranches(TTree * tree) = 0;

  /**
   * @brief Write relevant information to tree
   */
  virtual
  void
  write(const Event & event) = 0;

  /**
   * @brief Return list of systematic uncertainties supported by this plugin
   */
  virtual
  std::vector<std::string>
  get_supported_systematics() = 0;
};

#include "FWCore/PluginManager/interface/PluginFactory.h"  // edmplugin::PluginFactory

typedef edmplugin::PluginFactory<WriterBase*(const edm::ParameterSet&)> WriterPluginFactory;

#endif // TallinnNtupleProducer_Writers_WriterBase_h
