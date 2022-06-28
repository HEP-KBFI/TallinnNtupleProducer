#ifndef TallinnNtupleProducer_Readers_RecoLeptonReader_h
#define TallinnNtupleProducer_Readers_RecoLeptonReader_h

#include "TallinnNtupleProducer/Objects/interface/RecoLepton.h"      // RecoLepton
#include "TallinnNtupleProducer/Readers/interface/GenHadTauReader.h" // GenHadTauReader
#include "TallinnNtupleProducer/Readers/interface/GenJetReader.h"    // GenJetReader
#include "TallinnNtupleProducer/Readers/interface/GenLeptonReader.h" // GenLeptonReader
#include "TallinnNtupleProducer/Readers/interface/GenPhotonReader.h" // GenPhotonReader
#include "TallinnNtupleProducer/Readers/interface/ReaderBase.h"      // ReaderBase

#include <map>                                                       // std::map
#include <string>                                                    // std::string

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

  /**
   * @brief Read branches containing information on matching of RecoElectrons and RecoMuons
   *        to generator level electrons, muons, hadronic taus, and jets from tree
   *        and add this information to collection of RecoElectron and RecoMuon objects given as function argument
   */
  template<typename T,
           typename = std::enable_if<std::is_base_of<RecoLepton, T>::value>>
  void
  readGenMatching(std::vector<T> & leptons) const
  {
    if(readGenMatching_)
    {
      assert(genLeptonReader_ && genHadTauReader_ && genPhotonReader_ && genJetReader_);

      const std::size_t nLeptons = leptons.size();
      std::vector<GenLepton> matched_genLeptons = genLeptonReader_->read();
      assert(matched_genLeptons.size() == nLeptons);
      std::vector<GenHadTau> matched_genHadTaus = genHadTauReader_->read();
      assert(matched_genHadTaus.size() == nLeptons);
      std::vector<GenPhoton> matched_genPhotons = genPhotonReader_->read(true);
      assert(matched_genPhotons.size() == nLeptons);
      std::vector<GenJet> matched_genJets = genJetReader_->read();
      assert(matched_genJets.size() == nLeptons);

      for(std::size_t idxLepton = 0; idxLepton < nLeptons; ++idxLepton)
      {
        T & lepton = leptons[idxLepton];

        const GenLepton & matched_genLepton = matched_genLeptons[idxLepton];
        if(matched_genLepton.isValid()) lepton.set_genLepton(new GenLepton(matched_genLepton));

        const GenHadTau & matched_genHadTau = matched_genHadTaus[idxLepton];
        if(matched_genHadTau.isValid()) lepton.set_genHadTau(new GenHadTau(matched_genHadTau));

        const GenPhoton & matched_genPhoton = matched_genPhotons[idxLepton];
        if(matched_genPhoton.isValid()) lepton.set_genPhoton(new GenPhoton(matched_genPhoton));

        const GenJet & matched_genJet = matched_genJets[idxLepton];
        if(matched_genJet.isValid()) lepton.set_genJet(new GenJet(matched_genJet));
      }
    }
  }

  GenLeptonReader * genLeptonReader_;
  GenHadTauReader * genHadTauReader_;
  GenPhotonReader * genPhotonReader_;
  GenJetReader * genJetReader_;
  bool readGenMatching_;

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
  std::string branchName_genMatchIdx_;

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
  Int_t * genMatchIdx_;

  std::map<Btag, Float_t *> assocJetBtagCSVs_;

  // CV: make sure that only one RecoLeptonReader instance exists for a given branchName,
  //     as ROOT cannot handle multiple TTree::SetBranchAddress calls for the same branch.
  static std::map<std::string, int> numInstances_;
  static std::map<std::string, RecoLeptonReader *> instances_;
};

#endif // TallinnNtupleProducer_Readers_RecoLeptonReader_h
