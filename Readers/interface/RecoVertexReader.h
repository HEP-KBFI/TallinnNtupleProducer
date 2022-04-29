#ifndef TallinnNtupleProducer_Readers_RecoVertexReader_h
#define TallinnNtupleProducer_Readers_RecoVertexReader_h

#include "FWCore/ParameterSet/interface/ParameterSet.h"           // edm::ParameterSet

#include "TallinnNtupleProducer/Objects/interface/RecoVertex.h"   // RecoVertex
#include "TallinnNtupleProducer/Readers/interface/ReaderBase.h"   // ReaderBase

#include <map>                                                    // std::map

// forward declarations
class TTree;

class RecoVertexReader : public ReaderBase
{
 public:
  RecoVertexReader(const edm::ParameterSet & cfg);
  ~RecoVertexReader() override;

  /**
   * @brief Call tree->SetBranchAddress for all RecoVertex branches
   */
  std::vector<std::string>
  setBranchAddresses(TTree * tree) override;

  /**
   * @brief Read branches from tree and use information to fill RecoVertex object
   * @return RecoVertex object
   */
  RecoVertex
  read() const;

  /**
    * @brief Return list of systematic uncertainties supported by RecoVertexReader class
    */
  static
  std::vector<std::string>
  get_supported_systematics(const edm::ParameterSet & cfg);

 protected:
 /**
   * @brief Initialize names of branches to be read from tree
   */
  void
  setBranchNames();

  std::string branchName_;

  std::string branchName_x_;
  std::string branchName_y_;
  std::string branchName_z_;
  std::string branchName_ndof_;
  std::string branchName_chi2_;
  std::string branchName_score_;
  std::string branchName_npvs_;
  std::string branchName_npvsGood_;

  RecoVertex recoVertex_;

  // CV: make sure that only one RecoVertexReader instance exists for a given branchName,
  //     as ROOT cannot handle multiple TTree::SetBranchAddress calls for the same branch.
  static std::map<std::string, int> numInstances_;
  static std::map<std::string, RecoVertexReader *> instances_;
};

#endif // TallinnNtupleProducer_Readers_RecoVertexReader_h
