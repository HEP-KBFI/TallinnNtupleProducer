#ifndef TallinnNtupleProducer_Readers_RecoElectronReader_h
#define TallinnNtupleProducer_Readers_RecoElectronReader_h

#include "FWCore/ParameterSet/interface/ParameterSet.h"           // edm::ParameterSet

#include "TallinnNtupleProducer/Objects/interface/RecoElectron.h" // RecoElectron
#include "TallinnNtupleProducer/Readers/interface/ReaderBase.h"   // ReaderBase

#include <map>                                                    // std::map

// forward declarations
class TTree;
class RecoLeptonReader;

enum class EGammaID;
enum class EGammaWP;
enum class Era;

class RecoElectronReader : public ReaderBase
{
 public:
  RecoElectronReader(const edm::ParameterSet & cfg);
  ~RecoElectronReader();

  /**
   * @brief Call tree->SetBranchAddress for all lepton branches specific to RecoElectrons
   */
  std::vector<std::string>
  setBranchAddresses(TTree * tree) override;

  /**
   * @brief Read branches from tree and use information to fill collection of RecoElectron objects
   * @return Collection of RecoElectron objects
   */
  std::vector<RecoElectron>
  read() const;

  void
  set_mvaTTH_wp(double mvaTTH_wp);

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

  std::string branchName_eCorr_;
  std::string branchName_sigmaEtaEta_;
  std::string branchName_HoE_;
  std::string branchName_deltaEta_;
  std::string branchName_deltaPhi_;
  std::string branchName_OoEminusOoP_;
  std::string branchName_lostHits_;
  std::string branchName_conversionVeto_;
  std::string branchName_cutbasedID_HLT_;

  std::map<EGammaID, std::string> branchNames_mvaRaw_POG_;
  std::map<EGammaID, std::map<EGammaWP, std::string>> branchNames_mvaID_POG_;

  Float_t * eCorr_;
  Float_t * sigmaEtaEta_;
  Float_t * HoE_;
  Float_t * deltaEta_;
  Float_t * deltaPhi_;
  Float_t * OoEminusOoP_;
  UChar_t * lostHits_;
  Bool_t * conversionVeto_;
  Int_t * cutbasedID_HLT_;

  double mvaTTH_wp_;

  std::map<EGammaID, Float_t *> rawMVAs_POG_;
  std::map<EGammaID, std::map<EGammaWP, Bool_t *>> mvaIDs_POG_;

  // CV: make sure that only one RecoElectronReader instance exists for a given branchName,
  //     as ROOT cannot handle multiple TTree::SetBranchAddress calls for the same branch.
  static std::map<std::string, int> numInstances_;
  static std::map<std::string, RecoElectronReader *> instances_;
};

#endif // TallinnNtupleProducer_Readers_RecoElectronReader_h

