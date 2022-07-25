#ifndef TallinnNtupleProducer_Readers_RecoMEtReader_h
#define TallinnNtupleProducer_Readers_RecoMEtReader_h

#include "TallinnNtupleProducer/Objects/interface/RecoMEt.h"                     // RecoMEt
#include "TallinnNtupleProducer/Readers/interface/ReaderBase.h"                  // ReaderBase

#include <map>                                                                   // std::map
#include <string>                                                                // std::string

// forward declarations
class TTree;
class GenMEtReader;

enum class Era;

class RecoMEtReader
  : public ReaderBase
{
 public:
  RecoMEtReader(const edm::ParameterSet & cfg);
  ~RecoMEtReader();

  /**
   * @brief Call tree->SetBranchAddress for all RecoMEt branches
   */
  std::vector<std::string>
  setBranchAddresses(TTree * inputTree) override;

  /**
   * @brief Read branches from tree and use information to fill RecoMEt object
   * @return RecoMEt object
   */
  RecoMEt
  read() const;

  /**
    * @brief Return list of systematic uncertainties supported by RecoMEtReader class
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

  bool isMC_;
  std::string branchName_obj_;

  std::string branchName_sumEt_;
  std::string branchName_unclDeltaX_;
  std::string branchName_unclDeltaY_;
  std::string branchName_covXX_;
  std::string branchName_covXY_;
  std::string branchName_covYY_;

  GenMEtReader * genMEtReader_;

  Float_t met_sumEt_;
  Float_t met_unclDeltaX_;
  Float_t met_unclDeltaY_;
  Float_t met_covXX_;
  Float_t met_covXY_;
  Float_t met_covYY_;

  // CV: make sure that only one RecoMEtReader instance exists for a given branchName,
  //     as ROOT cannot handle multiple TTree::SetBranchAddress calls for the same branch.
  static std::map<std::string, int> numInstances_;
  static std::map<std::string, RecoMEtReader *> instances_;
};

#endif // TallinnNtupleProducer_Readers_RecoMEtReader_h
