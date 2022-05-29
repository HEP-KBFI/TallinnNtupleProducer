#ifndef TallinnNtupleProducer_Readers_GenJetReader_h
#define TallinnNtupleProducer_Readers_GenJetReader_h

#include "TallinnNtupleProducer/Objects/interface/GenJet.h"     // GenJet
#include "TallinnNtupleProducer/Readers/interface/ReaderBase.h" // ReaderBase

#include <map>                                                  // std::map

// forward declarations
class TTree;

class GenJetReader : public ReaderBase
{
 public:
  explicit GenJetReader(const edm::ParameterSet & cfg);
  ~GenJetReader();

  /**
   * @brief Call tree->SetBranchAddress for all GenJet branches
   */
  std::vector<std::string>
  setBranchAddresses(TTree * tree) override;

  /**
   * @brief Read branches from tree and use information to fill collection of GenJet objects
   * @return Collection of GenJet objects
   */
  std::vector<GenJet>
  read() const;

 protected:
 /**
   * @brief Initialize names of branches to be read from tree
   */
  void
  setBranchNames();

  unsigned int max_nJets_;
  std::string branchName_num_;
  std::string branchName_obj_;

  std::string branchName_pt_;
  std::string branchName_eta_;
  std::string branchName_phi_;
  std::string branchName_mass_;
  std::string branchName_partonFlavour_;
  std::string branchName_hadronFlavour_;

  UInt_t nJets_;
  Float_t * jet_pt_;
  Float_t * jet_eta_;
  Float_t * jet_phi_;
  Float_t * jet_mass_;
  Int_t * jet_partonFlavour_;
  UChar_t * jet_hadronFlavour_;

  // CV: make sure that only one RecoJetReader instance exists for a given branchName,
  //     as ROOT cannot handle multiple TTree::SetBranchAddress calls for the same branch.
  static std::map<std::string, int> numInstances_;
  static std::map<std::string, GenJetReader *> instances_;
};

#endif // TallinnNtupleProducer_Readers_GenJetReader_h

