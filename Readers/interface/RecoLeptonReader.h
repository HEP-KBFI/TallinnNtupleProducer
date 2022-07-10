#ifndef TallinnNtupleProducer_Readers_RecoLeptonReader_h
#define TallinnNtupleProducer_Readers_RecoLeptonReader_h

#include "TallinnNtupleProducer/Objects/interface/RecoLepton.h"      // RecoLepton
#include "TallinnNtupleProducer/Readers/interface/ReaderBase.h"      // ReaderBase

// forward declarations
enum class Btag;
enum class Era;

class RecoLeptonReader : public ReaderBase
{
 public:
  RecoLeptonReader(const edm::ParameterSet & cfg);
  ~RecoLeptonReader();

  /**
   * @brief Call tree->SetBranchAddress for all lepton branches common to RecoElectrons and RecoMuons
   */
  std::vector<std::string>
  setBranchAddresses(TTree * tree) override;

  friend class RecoElectronReader;
  friend class RecoMuonReader;

 protected:
 /**
   * @brief Initialize names of branches to be read from tree
   */
  void
  setBranchNames();

  unsigned int max_nLeptons_;
  std::string branchName_num_;
  std::string branchName_obj_;
  bool isMC_;
  Era era_;

  std::string branchName_pt_;
  std::string branchName_eta_;
  std::string branchName_phi_;
  std::string branchName_mass_;
  std::string branchName_pdgId_;
  std::string branchName_dxy_;
  std::string branchName_dz_;
  std::string branchName_relIso_all_;
  std::string branchName_pfRelIso03_all_;
  std::string branchName_relIso_chg_;
  std::string branchName_relIso_neu_;
  std::string branchName_sip3d_;
  std::string branchName_mvaRawTTH_;
  std::string branchName_jetRelIso_;
  std::string branchName_jetNDauChargedMVASel_;
  std::string branchName_tightCharge_;
  std::string branchName_charge_;
  std::string branchName_jetIdx_;
  std::string branchName_genPartFlav_;
  std::string branchName_genPartIdx_;

  std::map<Btag, std::string> branchNames_assocJetBtagCSV_;

  UInt_t nLeptons_;
  Float_t * pt_;
  Float_t * eta_;
  Float_t * phi_;
  Float_t * mass_;
  Int_t * pdgId_;
  Float_t * dxy_;
  Float_t * dz_;
  Float_t * relIso_all_;
  Float_t * pfRelIso03_all_;
  Float_t * relIso_chg_;
  Float_t * relIso_neu_;
  Float_t * sip3d_;
  Float_t * mvaRawTTH_;
  Float_t * jetRelIso_;
  Int_t * jetNDauChargedMVASel_;
  Int_t * tightCharge_;
  Int_t * charge_;
  Int_t * jetIdx_;
  UChar_t * genPartFlav_;
  Int_t * genPartIdx_;

  std::map<Btag, Float_t *> assocJetBtagCSVs_;

  // CV: make sure that only one RecoLeptonReader instance exists for a given branchName,
  //     as ROOT cannot handle multiple TTree::SetBranchAddress calls for the same branch.
  static std::map<std::string, int> numInstances_;
  static std::map<std::string, RecoLeptonReader *> instances_;
};

#endif // TallinnNtupleProducer_Readers_RecoLeptonReader_h
