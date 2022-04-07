#ifndef TallinnNtupleProducer_Readers_GenMEtReader_h
#define TallinnNtupleProducer_Readers_GenMEtReader_h

#include "TallinnNtupleProducer/Readers/interface/ReaderBase.h" // ReaderBase
#include "TallinnNtupleProducer/Objects/interface/GenMEt.h"     // GenMEt

#include <map>                                                  // std::map

// forward declarations
class TTree;

class GenMEtReader : public ReaderBase
{
 public:
  GenMEtReader(const edm::ParameterSet & cfg);
  ~GenMEtReader();

  /**
   * @brief Call tree->SetBranchAddress for all RecoMEt branches
   */
  std::vector<std::string>
  setBranchAddresses(TTree * tree) override;

  /**
   * @brief Read branches from tree and use information to fill RecoMEt object
   * @return RecoMEt object
   */
  GenMEt
  read() const;

 protected:
  /**
   * @brief Initialize names of branches to be read from tree
   */
  void
  setBranchNames();

  std::string branchName_obj_;

  std::string branchName_pt_;
  std::string branchName_phi_;

  GenMEt met_;

  // CV: make sure that only one GenMEtReader instance exists for a given branchName,
  //     as ROOT cannot handle multiple TTree::SetBranchAddress calls for the same branch.
  static std::map<std::string, int> numInstances_;
  static std::map<std::string, GenMEtReader *> instances_;
};

#endif // TallinnNtupleProducer_Readers_GenMEtReader_h  
