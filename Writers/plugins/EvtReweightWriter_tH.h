#ifndef TallinnNtupleProducer_Writers_EvtReweightWriter_tH_h
#define TallinnNtupleProducer_Writers_EvtReweightWriter_tH_h

#include "FWCore/ParameterSet/interface/ParameterSet.h"         // edm::ParameterSet

#include "TallinnNtupleProducer/Writers/interface/WriterBase.h" // WriterBase
#include "TallinnNtupleProducer/Objects/interface/Event.h"      // Event

#include <string>                                               // std::string
#include <vector>                                               // std::vector

// forward declarations
class TTree;

class EvtReweightWriter_tH : public WriterBase
{
 public:
  EvtReweightWriter_tH(const edm::ParameterSet & cfg);
  ~EvtReweightWriter_tH();

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
 
  /**
   * @brief Return list of systematic uncertainties supported by this plugin
   */
  std::vector<std::string>
  get_supported_systematics();

 private:
  std::map<std::string, std::map<std::string, std::string>> branchNames_; // key = central_or_shift, tH coupling scenario
  std::map<std::string, std::map<std::string, Float_t>> thReweights_;     // key = central_or_shift, tH coupling scenario

  std::vector<edm::ParameterSet> tHweights_; 
  bool eventInfo_isInitialized_;
  TTree* tree_;
  std::map<std::string, bool> branchAddresses_isInitialized_;             // key = central_or_shift
};

#endif // TallinnNtupleProducer_Writers_EvtReweightWriter_tH_h
