#ifndef TallinnNtupleProducer_Objects_CorrT1METJetReader_h
#define TallinnNtupleProducer_Objects_CorrT1METJetReader_h

#include "TallinnNtupleProducer/Objects/interface/CorrT1METJet.h" // CorrT1METJet
#include "TallinnNtupleProducer/Readers/interface/ReaderBase.h"   // ReaderBase

#include <map>                                                    // std::map

// forward declarations
class TTree;

class CorrT1METJetReader
  : public ReaderBase
{
 public:
  explicit CorrT1METJetReader(const edm::ParameterSet & cfg);
  ~CorrT1METJetReader();

  /**
   * @brief Call tree->SetBranchAddress for all CorrT1METJet branches
   */
  std::vector<std::string>
  setBranchAddresses(TTree * tree) override;

  /**
   * @brief Read branches from tree and use information to fill collection of CorrT1METJet objects
   * @return Collection of CorrT1METJet objects
   */
  std::vector<CorrT1METJet>
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

  std::string branchName_rawPt_;
  std::string branchName_eta_;
  std::string branchName_phi_;
  std::string branchName_area_;
  std::string branchName_muonSubtrFactor_;

  UInt_t nJets_;
  Float_t * jet_rawPt_;
  Float_t * jet_eta_;
  Float_t * jet_phi_;
  Float_t * jet_area_;
  Float_t * jet_muonSubtrFactor_;

  // CV: make sure that only one CorrT1METJetReader instance exists for a given branchName,
  //     as ROOT cannot handle multiple TTree::SetBranchAddress calls for the same branch.
  static std::map<std::string, int> numInstances_;
  static std::map<std::string, CorrT1METJetReader *> instances_;
};

#endif // TallinnNtupleProducer_Objects_CorrT1METJetReader_h
