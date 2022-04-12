#ifndef TallinnNtupleProducer_Readers_GenHadTauReader_h
#define TallinnNtupleProducer_Readers_GenHadTauReader_h

#include "TallinnNtupleProducer/Objects/interface/GenHadTau.h"  // GenHadTau
#include "TallinnNtupleProducer/Readers/interface/ReaderBase.h" // ReaderBase

#include <map>                                                  // std::map

// forward declarations
class TTree;

class GenHadTauReader : public ReaderBase
{
 public:
  GenHadTauReader(const edm::ParameterSet & cfg);
  ~GenHadTauReader();

  /**
   * @brief Call tree->SetBranchAddress for all GenHadTau branches
   */
  std::vector<std::string>
  setBranchAddresses(TTree * tree) override;

  /**
   * @brief Read branches from tree and use information to fill collection of GenHadTau objects
   * @return Collection of GenHadTau objects
   */
  std::vector<GenHadTau>
  read() const;

 protected:
 /**
   * @brief Initialize names of branches to be read from tree
   */
  void
  setBranchNames();

  unsigned int max_nHadTaus_;
  std::string branchName_num_;
  std::string branchName_obj_;

  std::string branchName_pt_;
  std::string branchName_eta_;
  std::string branchName_phi_;
  std::string branchName_mass_;
  std::string branchName_charge_;
  std::string branchName_status_;

  UInt_t nHadTaus_;
  Float_t * hadTau_pt_;
  Float_t * hadTau_eta_;
  Float_t * hadTau_phi_;
  Float_t * hadTau_mass_;
  Int_t * hadTau_charge_;
  Int_t * hadTau_status_;

  // CV: make sure that only one GenHadTauReader instance exists for a given branchName,
  //     as ROOT cannot handle multiple TTree::SetBranchAddress calls for the same branch.
  static std::map<std::string, int> numInstances_;
  static std::map<std::string, GenHadTauReader*> instances_;
};

#endif // TallinnNtupleProducer_Readers_GenHadTauReader_h

