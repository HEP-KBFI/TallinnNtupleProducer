#ifndef TallinnNtupleProducer_Writers_ZbosonMassVetoWriter_h
#define TallinnNtupleProducer_Writers_ZbosonMassVetoWriter_h

#include "FWCore/ParameterSet/interface/ParameterSet.h"         // edm::ParameterSet

#include "TallinnNtupleProducer/Writers/interface/WriterBase.h" // WriterBase
#include "TallinnNtupleProducer/Objects/interface/Event.h"      // Event

#include <string>                                               // std::string
#include <vector>                                               // std::vector

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

  double z_mass_;
  double z_window_;
  bool requireOS_;

  Bool_t passesZbosonMassVeto_;
};

#endif // TallinnNtupleProducer_Writers_ZbosonMassVetoWriter_h
