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
  unsigned int max_nLeptons_;
  std::string branchName_num_;
  std::string branchName_obj_;

  bool readGenPartFlav_;

  std::string branchName_pt_;
  std::string branchName_eta_;
  std::string branchName_phi_;
  std::string branchName_mass_;
  std::string branchName_pdgId_;
  std::string branchName_status_;
  std::string branchName_statusFlags_;
  std::string branchName_genPartFlav_;

  UInt_t nLeptons_;
  Float_t * lepton_pt_;
  Float_t * lepton_eta_;
  Float_t * lepton_phi_;
  Float_t * lepton_mass_;
  Int_t * lepton_pdgId_;
  Int_t * lepton_status_;
  Int_t * lepton_statusFlags_;
  UChar_t * lepton_genPartFlav_;

  // CV: make sure that only one GenLeptonReader instance exists for a given branchName,
  //     as ROOT cannot handle multiple TTree::SetBranchAddress calls for the same branch.
  static std::map<std::string, int> numInstances_;
  static std::map<std::string, GenLeptonReader *> instances_;
};

#endif // TallinnNtupleProducer_Readers_GenLeptonReader_h


