#ifndef TallinnNtupleProducer_Readers_EventReader_h
#define TallinnNtupleProducer_Readers_EventReader_h

#include "TallinnNtupleProducer/Cleaners/interface/ParticleCollectionCleaner.h"               // RecoElectronCollectionCleaner, RecoHadTauCollectionCleaner, RecoJetCollectionCleanerAK4, RecoMuonCollectionCleaner
#include "TallinnNtupleProducer/Cleaners/interface/RecoJetCollectionCleanerByIndexAK4.h"      // RecoJetCollectionCleanerByIndexAK4
#include "TallinnNtupleProducer/Readers/interface/ParticleCollectionGenMatcher.h"             // RecoElectronCollectionGenMatcher, RecoMuonCollectionGenMatcher, RecoHadTauCollectionGenMatcher, RecoJetCollectionGenMatcher
#include "TallinnNtupleProducer/Readers/interface/ReaderBase.h"                               // ReaderBase
#include "TallinnNtupleProducer/Objects/interface/Event.h"                                    // Event

#include "TallinnNtupleProducer/Selectors/interface/RecoMuonCollectionSelectorLoose.h"        // RecoMuonCollectionSelectorLoose
#include "TallinnNtupleProducer/Selectors/interface/RecoHadTauCollectionSelectorFakeable.h"   // RecoHadTauCollectionSelectorFakeable
#include "TallinnNtupleProducer/Selectors/interface/RecoHadTauCollectionSelectorLoose.h"      // RecoHadTauCollectionSelectorLoose
#include "TallinnNtupleProducer/Selectors/interface/RecoHadTauCollectionSelectorTight.h"      // RecoHadTauCollectionSelectorTight
#include "TallinnNtupleProducer/Selectors/interface/RecoJetCollectionSelectorAK4.h"           // RecoJetCollectionSelectorAK4
#include "TallinnNtupleProducer/Selectors/interface/RecoJetCollectionSelectorAK4_btag.h"      // RecoJetCollectionSelectorAK4_btag
#include "TallinnNtupleProducer/Selectors/interface/RecoJetCollectionSelectorAK8.h"           // RecoJetCollectionSelectorAK8
#include "TallinnNtupleProducer/Selectors/interface/RecoJetCollectionSelectorAK8_Hbb.h"       // RecoJetCollectionSSelectorAK8_Hbb
#include "TallinnNtupleProducer/Selectors/interface/RecoJetCollectionSelectorAK8_Wjj.h"       // RecoJetCollectionSSelectorAK8_Wjj

#include "correction.h"                                                                       // correction::Correction::Ref

#include <set>                                                                                // std::set

// forward declarations
class TTree;
class RunLumiEvent;
class RunLumiEventReader;
class TriggerInfoReader;
class EventInfoReader;
class GenHadTauReader;
class GenJetReader;
class CorrT1METJetReader;
class GenParticleReader;
class MEtFilterReader;
class RecoElectronCollectionSelectorFakeable;
class RecoElectronCollectionSelectorLoose;
class RecoElectronCollectionSelectorTight;
class RecoMuonCollectionSelectorFakeable;
class RecoMuonCollectionSelectorTight;
class RecoElectronReader;
class RecoHadTauReader;
class GenMEtReader;
class RecoMEtReader;
class RecoMuonReader;
class RecoVertexReader;
class RecoJetReaderAK4;
class RecoJetReaderAK8;
class JMECorrector;

enum class Era;

class EventReader : public ReaderBase
{
 public:
  EventReader(const edm::ParameterSet& cfg);
  ~EventReader();

  void
  set_central_or_shift(const std::string& central_or_shift);

  /**
   * @brief Call tree->SetBranchAddress for all particle-collection reader classes
   */
  std::vector<std::string>
  setBranchAddresses(TTree * inputTree) override;

  /**
   * @brief Read run, lumi, and event number branches from tree
   * @return Event object
   */
  const RunLumiEvent &
  read_runLumiEvent() const;

  /**
   * @brief Read branches from tree and use information to fill Event object
   * @return Event object
   */
  const Event &
  read() const;

  /**
    * @brief Return list of systematic uncertainties supported by EventReader class
    */
  static
  std::vector<std::string>
  get_supported_systematics(const edm::ParameterSet & cfg);

  void
  set_tauEScset(correction::Correction::Ref);

 protected:
  enum Level { kLepton, kHadTau };
  void
  clearEvent(Level level) const;

  edm::ParameterSet cfg_;

  bool isDEBUG_;

  unsigned numNominalLeptons_;
  bool applyNumNominalLeptonsCut_;
  unsigned numNominalHadTaus_;
  bool applyNumNominalHadTausCut_;

  Era era_;
  bool isMC_;
  bool jetCleaningByIndex_;
  bool genMatchRecoJets_;

  mutable UInt_t lastRun_;
  mutable UInt_t lastLumi_;
  mutable ULong64_t lastEvent_;
  std::string current_central_or_shift_;

  mutable Event event_;

  RunLumiEventReader * runLumiEventReader_;

  EventInfoReader * eventInfoReader_;
  std::set<std::string> eventInfo_supported_systematics_;
  std::string eventInfo_lastSystematic_;

  TriggerInfoReader * triggerInfoReader_;
  std::set<std::string> triggerInfo_supported_systematics_;
  std::string triggerInfo_lastSystematic_;

  RecoMuonReader * muonReader_;
  RecoMuonCollectionSelectorLoose * looseMuonSelector_;
  RecoMuonCollectionSelectorFakeable * fakeableMuonSelector_;
  RecoMuonCollectionSelectorTight * tightMuonSelector_;
  std::set<std::string> muon_supported_systematics_;
  mutable std::string muon_lastSystematic_;

  RecoElectronReader * electronReader_;
  RecoElectronCollectionCleaner electronCleaner_;
  RecoElectronCollectionSelectorLoose * looseElectronSelector_;
  RecoElectronCollectionSelectorFakeable * fakeableElectronSelector_;
  RecoElectronCollectionSelectorTight * tightElectronSelector_;
  std::set<std::string> electron_supported_systematics_;
  mutable std::string electron_lastSystematic_;

  RecoHadTauReader * hadTauReader_;
  RecoHadTauCollectionCleaner hadTauCleaner_;
  RecoHadTauCollectionSelectorLoose * looseHadTauSelector_;
  RecoHadTauCollectionSelectorFakeable * fakeableHadTauSelector_;
  RecoHadTauCollectionSelectorTight * tightHadTauSelector_;
  std::set<std::string> hadTau_supported_systematics_;
  mutable std::string hadTau_lastSystematic_;
  mutable bool hadTau_isInvalid_;

  JMECorrector * jmeCorrector_;

  RecoJetReaderAK4 * jetReaderAK4_;
  RecoJetCollectionCleanerAK4 jetCleanerAK4_dR04_; // used for cleaning AK4 jets wrt electrons, muons, and tauh
  RecoJetCollectionCleanerByIndexAK4 jetCleanerAK4ByIndex_dR04_;
  RecoJetCollectionCleanerAK4 jetCleanerAK4_dR12_; // used for cleaning AK4 jets wrt AK8 jets
  RecoJetCollectionSelectorAK4 * jetSelectorAK4_;
  RecoJetCollectionSelectorAK4_btagLoose * jetSelectorAK4_btagLoose_;
  RecoJetCollectionSelectorAK4_btagMedium * jetSelectorAK4_btagMedium_;
  std::set<std::string> jetsAK4_supported_systematics_;
  mutable std::string jetAK4_lastSystematic_;
  mutable bool jetAK4_isInvalid_;
  pileupJetID apply_pileupJetID_;

  GenParticleReader * genParticleReader_;
  GenHadTauReader * genHadTauReader_;
  GenJetReader * genJetReader_;
  CorrT1METJetReader * corrT1METJetReader_;
  RecoMuonCollectionGenMatcher * muonGenMatcher_;
  RecoElectronCollectionGenMatcher * electronGenMatcher_;
  RecoHadTauCollectionGenMatcher * hadTauGenMatcher_;
  RecoJetCollectionGenMatcherAK4 * jetGenMatcherAK4_;
  CorrT1METJetCollectionGenMatcher * corrT1METJetGenMatcher_;

  RecoJetReaderAK8 * jetReaderAK8_;
  RecoJetCollectionCleanerAK8 jetCleanerAK8_dR08_;  // used for cleaning AK8 jets wrt electrons, muons, and tauh
  RecoJetCollectionSelectorAK8_Hbb * jetSelectorAK8_Hbb_;
  RecoJetCollectionSelectorAK8_Wjj * jetSelectorAK8_Wjj_;
  std::set<std::string> jetsAK8_Hbb_supported_systematics_;
  mutable std::string jetAK8_Hbb_lastSystematic_;
  mutable bool jetAK8_Hbb_isInvalid_;
  std::set<std::string> jetsAK8_Wjj_supported_systematics_;
  mutable std::string jetAK8_Wjj_lastSystematic_;
  mutable bool jetAK8_Wjj_isInvalid_;

  GenMEtReader * rawmetReader_;
  RecoMEtReader * metReader_;
  std::set<std::string> met_supported_systematics_;
  mutable std::string met_lastSystematic_;
  mutable bool met_isInvalid_;

  MEtFilterReader * metFilterReader_;
  std::set<std::string> metFilter_supported_systematics_;
  mutable std::string metFilter_lastSystematic_;
  mutable bool metFilter_isInvalid_;

  RecoVertexReader * vertexReader_;
  std::set<std::string> vertex_supported_systematics_;
  mutable std::string vertex_lastSystematic_;
  mutable bool vertex_isInvalid_;
};

#endif // TallinnNtupleProducer_Readers_EventReader_h
