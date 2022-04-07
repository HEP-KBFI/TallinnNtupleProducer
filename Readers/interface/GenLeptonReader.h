#ifndef TallinnNtupleProducer_Readers_GenLeptonReader_h
#define TallinnNtupleProducer_Readers_GenLeptonReader_h

#include "TallinnNtupleProducer/Objects/interface/GenLepton.h"  // GenLepton
#include "TallinnNtupleProducer/Readers/interface/ReaderBase.h" // ReaderBase

#include <map>                                                  // std::map

// forward declarations
class TTree;

class GenLeptonReader : public ReaderBase
{
 public:
  GenLeptonReader(const edm::ParameterSet & cfg);
  ~GenLeptonReader();

  /**
   * @brief Call tree->SetBranchAddress for all GenLepton branches
   */
  std::vector<std::string>
  setBranchAddresses(TTree * tree) override;

  /**
   * @brief Read branches from tree and use information to fill collection of GenLepton objects
   * @return Collection of GenLepton objects
   */
  std::vector<GenLepton>
  read() const;

  /**
   * @brief enable/disable read genPartFlav branch
   * @param flag If true, reads genPartFlav branch; if false, does not read genPartFlav branch
   */
  void
  readGenPartFlav(bool flag);

 protected:
 /**
   * @brief Initialize names of branches to be read from tree
   */
  void
  setBranchNames();

  // all stable electrons and muons
  const unsigned int max_nPromptLeptons_;
  std::string branchName_nPromptLeptons_;
  std::string branchName_promptLeptons_;

  bool readGenPartFlav_;

  std::string branchName_promptLepton_pt_;
  std::string branchName_promptLepton_eta_;
  std::string branchName_promptLepton_phi_;
  std::string branchName_promptLepton_mass_;
  std::string branchName_promptLepton_pdgId_;
  std::string branchName_promptLepton_status_;
  std::string branchName_promptLepton_statusFlags_;
  std::string branchName_promptLepton_genPartFlav_;

  UInt_t nPromptLeptons_;
  Float_t * promptLepton_pt_;
  Float_t * promptLepton_eta_;
  Float_t * promptLepton_phi_;
  Float_t * promptLepton_mass_;
  Int_t * promptLepton_pdgId_;
  Int_t * promptLepton_status_;
  Int_t * promptLepton_statusFlags_;
  UChar_t * promptLepton_genPartFlav_;

  // CV: make sure that only one GenLeptonReader instance exists for a given branchName,
  //     as ROOT cannot handle multiple TTree::SetBranchAddress calls for the same branch.
  static std::map<std::string, int> numInstances_;
  static std::map<std::string, GenLeptonReader *> instances_;
};

#endif // tthAnalysis_HiggsToTauTau_GenLeptonReader_h

