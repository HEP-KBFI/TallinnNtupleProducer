#ifndef TallinnNtupleProducer_Writers_LowMassLeptonPairVetoWriter_h
#define TallinnNtupleProducer_Writers_LowMassLeptonPairVetoWriter_h

#include "FWCore/ParameterSet/interface/ParameterSet.h"         // edm::ParameterSet

#include "TallinnNtupleProducer/Writers/interface/WriterBase.h" // WriterBase
#include "TallinnNtupleProducer/Objects/interface/Event.h"      // Event

#include <string>                                               // std::string
#include <vector>                                               // std::vector

// forward declarations
class TTree;

class LowMassLeptonPairVetoWriter : public WriterBase
{
 public:
  LowMassLeptonPairVetoWriter(const edm::ParameterSet & cfg);
  ~LowMassLeptonPairVetoWriter();

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
  std::string branchName_;

  bool requireSF_;
  bool requireOS_;

  Bool_t passesLowMassLeptonPairVeto_;
};

#endif // TallinnNtupleProducer_Writers_LowMassLeptonPairVetoWriter_h
