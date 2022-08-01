#ifndef TallinnNtupleProducer_Readers_RecoSubjetReaderAK8_h
#define TallinnNtupleProducer_Readers_RecoSubjetReaderAK8_h

#include "FWCore/ParameterSet/interface/ParameterSet.h"            // edm::ParameterSet

#include "TallinnNtupleProducer/Objects/interface/RecoSubjetAK8.h" // RecoSubjetAK8
#include "TallinnNtupleProducer/Readers/interface/ReaderBase.h"    // ReaderBase

#include <map>                                                     // std::map

// forward declarations
class TTree;

enum class Btag;
enum class Era;

class RecoSubjetReaderAK8
  : public ReaderBase
{
 public:
  RecoSubjetReaderAK8(const edm::ParameterSet & cfg);
  ~RecoSubjetReaderAK8() override;

  /**
   * @brief Call tree->SetBranchAddress for all RecoSubjetAK8 branches
   */
  std::vector<std::string>
  setBranchAddresses(TTree * tree) override;

  /**
   * @brief Read branches from tree and use information to fill collection of RecoSubjetAK8 objects
   * @return Collection of RecoSubjetAK8 objects
   */
  std::vector<RecoSubjetAK8>
  read() const;

 protected:
 /**
   * @brief Initialize names of branches to be read from tree
   */
  void
  setBranchNames();

  Era era_;
  Btag btag_;
  const unsigned int max_nJets_;
  std::string branchName_obj_;
  std::string branchName_num_;
  std::string branchName_btag_;

  std::string branchName_pt_;
  std::string branchName_eta_;
  std::string branchName_phi_;
  std::string branchName_mass_;
  std::string branchName_rawFactor_;
  std::string branchName_BtagCSV_;

  UInt_t nJets_;
  Float_t * jet_pt_;
  Float_t * jet_eta_;
  Float_t * jet_phi_;
  Float_t * jet_mass_;
  Float_t * jet_rawFactor_;
  Float_t * jet_BtagCSV_;

  // CV: make sure that only one RecoSubjetReaderAK8 instance exists for a given branchName,
  //     as ROOT cannot handle multiple TTree::SetBranchAddress calls for the same branch.
  static std::map<std::string, int> numInstances_;
  static std::map<std::string, RecoSubjetReaderAK8 *> instances_;
};

#endif // TallinnNtupleProducer_Readers_RecoSubjetReaderAK8_h
