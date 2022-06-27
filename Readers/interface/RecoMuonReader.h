#ifndef TallinnNtupleProducer_Readers_RecoMuonReader_h
#define TallinnNtupleProducer_Readers_RecoMuonReader_h

#include "FWCore/ParameterSet/interface/ParameterSet.h"           // edm::ParameterSet

#include "TallinnNtupleProducer/Objects/interface/RecoMuon.h"     // RecoMuon
#include "TallinnNtupleProducer/Readers/interface/ReaderBase.h"   // ReaderBase

#include <map>                                                    // std::map

// forward declarations
class TTree;
class RecoLeptonReader;

enum class Era;

class RecoMuonReader : public ReaderBase
{
 public:
  RecoMuonReader(const edm::ParameterSet & cfg);
  ~RecoMuonReader();

  /**
   * @brief Call tree->SetBranchAddress for all lepton branches specific to RecoMuons
   */
  std::vector<std::string>
  setBranchAddresses(TTree * tree) override;

  /**
   * @brief Read branches from tree and use information to fill collection of RecoMuon objects
   * @return Collection of RecoMuon objects
   */
  std::vector<RecoMuon>
  read() const;

  /**
   * @brief Set threshold that a muon need to pass in order to qualify as tight lepton
   *        cf. Table 7 in AN/2020-032 v14.pdf
   */
  void
  set_mvaTTH_wp(double mvaTTH_wp);

  /**
    * @brief Return list of systematic uncertainties supported by RecoMuonReader class
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

  Era era_;
  std::string branchName_num_;
  std::string branchName_obj_;

  RecoLeptonReader * leptonReader_;

  std::string branchName_mediumIdPOG_;
  std::string branchName_segmentCompatibility_;
  std::string branchName_ptErr_;

  Bool_t  * mediumIdPOG_;
  Float_t * segmentCompatibility_;
  Float_t * ptErr_;

  double mvaTTH_wp_;

  // CV: make sure that only one RecoMuonReader instance exists for a given branchName,
  //     as ROOT cannot handle multiple TTree::SetBranchAddress calls for the same branch.
  static std::map<std::string, int> numInstances_;
  static std::map<std::string, RecoMuonReader *> instances_;
};

#endif // TallinnNtupleProducer_Readers_RecoMuonReader_h

