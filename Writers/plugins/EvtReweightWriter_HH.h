#ifndef TallinnNtupleProducer_Writers_EvtReweightWriter_HH_h
#define TallinnNtupleProducer_Writers_EvtReweightWriter_HH_h

#include "FWCore/ParameterSet/interface/ParameterSet.h"         // edm::ParameterSet

#include "TallinnNtupleProducer/Writers/interface/WriterBase.h" // WriterBase
#include "TallinnNtupleProducer/Objects/interface/Event.h"      // Event

#include <string>                                               // std::string
#include <vector>                                               // std::vector

// forward declarations
class HHWeightInterfaceCouplings;
class HHWeightInterfaceLO;
class HHWeightInterfaceNLO;
class TTree;

class EvtReweightWriter_HH : public WriterBase
{
 public:
  EvtReweightWriter_HH(const edm::ParameterSet & cfg);
  ~EvtReweightWriter_HH();

  /**
   * @brief Call tree->Branch for all branches
   */
  void
  setBranches(TTree * tree);

  /**
   * @brief Write relevant information to tree
   */
  void
  write(const Event & event, const EvtWeightRecorder & evtWeightRecorder, const std::string & central_or_shift);
 
  /**
   * @brief Return list of systematic uncertainties supported by this plugin
   */
  std::vector<std::string>
  get_supported_systematics();

 private:
  std::map<std::string, std::string> branchNames_; // key = HH coupling scenario
  std::map<std::string, Float_t> hhReweights_;     // key = HH coupling scenario

  bool apply_HH_rwgt_lo_;
  bool apply_HH_rwgt_nlo_;

  HHWeightInterfaceCouplings * hhWeightInterface_couplings_;
  HHWeightInterfaceLO * hhWeightInterfaceLO_;
  HHWeightInterfaceNLO * hhWeightInterfaceNLO_;

  std::vector<std::string> bmNames_;
};

#endif // TallinnNtupleProducer_Writers_EvtReweightWriter_HH_h
