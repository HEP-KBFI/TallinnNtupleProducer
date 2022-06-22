#ifndef TallinnNtupleProducer_Readers_RecoHadTauReader_h
#define TallinnNtupleProducer_Readers_RecoHadTauReader_h

#include "TallinnNtupleProducer/Objects/interface/RecoHadTau.h" // RecoHadTau
#include "TallinnNtupleProducer/Readers/interface/ReaderBase.h" // ReaderBase

#include <map>                                                  // std::map

// forward declarations
class TTree;
class GenLeptonReader;
class GenHadTauReader;
class GenJetReader;
class TauESTool;
class TriggerInfo;

enum class Era;

class RecoHadTauReader : public ReaderBase
{
 public:
  RecoHadTauReader(const edm::ParameterSet & cfg);
  ~RecoHadTauReader();

  void
  setHadTauPt_central_or_shift(int hadTauPt_option);

  void
  set_default_tauID(TauID tauId);

  /**
   * @brief Call tree->SetBranchAddress for all RecoHadTau branches
   */
  std::vector<std::string>
  setBranchAddresses(TTree * tree) override;

  /**
   * @brief Read branches from tree and use information to fill collection of RecoHadTau objects
   * @return Collection of RecoHadTau objects
   */
  std::vector<RecoHadTau>
    read(const TriggerInfo& triggerInfo) const;

  /**
    * @brief Return list of systematic uncertainties supported by RecoHadTauReader class
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
  unsigned int max_nHadTaus_;
  std::string branchName_num_;
  std::string branchName_obj_;
  bool isMC_;

  /**
   * @brief Read branches containing information on matching of RecoHadTau objects
   *        to generator level electrons, muons, hadronic taus, and jets from tree
   *        and add this information to collection of RecoHadTau objects given as function argument
   */
  void
  readGenMatching(std::vector<RecoHadTau> & hadTaus) const;

  GenLeptonReader * genLeptonReader_;
  GenHadTauReader * genHadTauReader_;
  GenJetReader * genJetReader_;
  bool readGenMatching_;

  std::string branchName_pt_;
  std::string branchName_eta_;
  std::string branchName_phi_;
  std::string branchName_mass_;
  std::string branchName_charge_;
  std::string branchName_dxy_;
  std::string branchName_dz_;
  std::string branchName_decayMode_;
  std::string branchName_idDecayMode_;
  std::string branchName_idAgainstElec_;
  std::string branchName_idAgainstMu_;
  std::string branchName_filterBits_;
  std::string branchName_jetIdx_;
  std::string branchName_genPartFlav_;
  std::string branchName_genMatchIdx_;

  std::map<TauID, std::string> branchNames_idMVA_;
  std::map<TauID, std::string> branchNames_rawMVA_;

  TauID tauID_;
  TauESTool * tauESTool_;

  UInt_t nHadTaus_;
  Float_t * hadTau_pt_;
  Float_t * hadTau_eta_;
  Float_t * hadTau_phi_;
  Float_t * hadTau_mass_;
  Int_t * hadTau_charge_;
  Float_t * hadTau_dxy_;
  Float_t * hadTau_dz_;
  Int_t * hadTau_decayMode_;
  Bool_t * hadTau_idDecayMode_;
  UChar_t * hadTau_idAgainstElec_;
  UChar_t * hadTau_idAgainstMu_;
  UInt_t * hadTau_filterBits_;
  Int_t * hadTau_jetIdx_;
  UChar_t * hadTau_genPartFlav_;
  Int_t * hadTau_genMatchIdx_;

  std::map<TauID, UChar_t *> hadTau_idMVAs_;
  std::map<TauID, Float_t *> hadTau_rawMVAs_;

  // CV: make sure that only one RecoHadronicTauReader instance exists for a given branchName,
  //     as ROOT cannot handle multiple TTree::SetBranchAddress calls for the same branch.
  static std::map<std::string, int> numInstances_;
  static std::map<std::string, RecoHadTauReader *> instances_;
};

#endif // TallinnNtupleProducer_Readers_RecoHadTauReader_h

