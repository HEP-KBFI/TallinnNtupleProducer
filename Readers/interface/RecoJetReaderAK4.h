#ifndef TallinnNtupleProducer_Readers_RecoJetReaderAK4_h
#define TallinnNtupleProducer_Readers_RecoJetReaderAK4_h

#include "FWCore/ParameterSet/interface/ParameterSet.h"         // edm::ParameterSet

#include "TallinnNtupleProducer/Objects/interface/RecoJetAK4.h" // RecoJetAK4
#include "TallinnNtupleProducer/Readers/interface/ReaderBase.h" // ReaderBase

#include <map>                                                  // std::map

// forward declarations
class TTree;

enum class Btag;
enum class Era;

class RecoJetReaderAK4 : public ReaderBase
{
 public:
  RecoJetReaderAK4(const edm::ParameterSet & cfg);
  ~RecoJetReaderAK4() override;

  void
  setPtMass_central_or_shift(int central_or_shift);

  void
  read_Btag(Btag btag);

  /**
   * @brief Call tree->SetBranchAddress for all RecoJet branches
   */
  std::vector<std::string>
  setBranchAddresses(TTree * tree) override;

  /**
   * @brief Read branches from tree and use information to fill collection of RecoJet objects
   * @return Collection of RecoJet objects
   */
  std::vector<RecoJetAK4>
  read() const;

  /**
    * @brief Return list of systematic uncertainties supported by RecoJetReaderAK4 class
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
  bool isMC_;
  unsigned int max_nJets_;
  std::string branchName_num_;
  std::string branchName_obj_;
 
  std::string branchName_eta_;
  std::string branchName_phi_;
  std::string branchName_QGDiscr_;
  std::string branchName_bRegCorr_;
  std::string branchName_bRegRes_;
  std::string branchName_BtagScore_;
  std::string branchName_jetId_;
  std::string branchName_puId_;
  std::string branchName_genJetIdx_;
  std::string branchName_partonFlavour_;
  std::string branchName_hadronFlavour_;

  std::map<int, std::string> branchNames_pt_systematics_;
  std::map<int, std::string> branchNames_mass_systematics_;
  std::map<Btag, std::string> branchNames_btag_;

  Btag btag_;
  int btag_central_or_shift_;
  int ptMassOption_central_;
  int ptMassOption_;

  UInt_t nJets_;
  Float_t * jet_eta_;
  Float_t * jet_phi_;
  Float_t * jet_QGDiscr_;
  Float_t * jet_bRegCorr_;
  Float_t * jet_bRegRes_;
  Float_t * jet_BtagScore_;
  Int_t * jet_jetId_;
  Int_t * jet_puId_;
  Int_t * jet_genJetIdx_;
  Int_t * jet_partonFlavour_;
  Int_t * jet_hadronFlavour_;

  std::map<int, Float_t *> jet_pt_systematics_;
  std::map<int, Float_t *> jet_mass_systematics_;

  // CV: make sure that only one RecoJetReader instance exists for a given branchName,
  //     as ROOT cannot handle multiple TTree::SetBranchAddress calls for the same branch.
  static std::map<std::string, int> numInstances_;
  static std::map<std::string, RecoJetReaderAK4 *> instances_;
};

#endif // TallinnNtupleProducer_Readers_RecoJetReaderAK4_h
