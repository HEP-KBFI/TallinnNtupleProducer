#ifndef TallinnNtupleProducer_Readers_EventReader_h
#define TallinnNtupleProducer_Readers_EventReader_h

#include "FWCore/ParameterSet/interface/ParameterSet.h"                                       // edm::ParameterSet

#include "TallinnNtupleProducer/Cleaners/interface/ParticleCollectionCleaner.h"               // RecoElectronCollectionCleaner, RecoHadTauCollectionCleaner, RecoJetCollectionCleanerAK4, RecoMuonCollectionCleaner
#include "TallinnNtupleProducer/Cleaners/interface/RecoJetCollectionCleanerByIndexAK4.h"      // RecoJetCollectionCleanerByIndexAK4
#include "TallinnNtupleProducer/Objects/interface/Event.h"                                    // Event
#include "TallinnNtupleProducer/Readers/interface/EventInfoReader.h"                          // EventInfoReader
#include "TallinnNtupleProducer/Readers/interface/GenHadTauReader.h"                          // GenHadTauReader
#include "TallinnNtupleProducer/Readers/interface/GenJetReader.h"                             // GenJetReader
#include "TallinnNtupleProducer/Readers/interface/GenLeptonReader.h"                          // GenLeptonReader
#include "TallinnNtupleProducer/Readers/interface/GenPhotonReader.h"                          // GenPhotonReader
#include "TallinnNtupleProducer/Readers/interface/MEtFilterReader.h"                          // MEtFilterReader
#include "TallinnNtupleProducer/Readers/interface/ParticleCollectionGenMatcher.h"             // RecoElectronCollectionGenMatcher, RecoMuonCollectionGenMatcher, RecoHadTauCollectionGenMatcher, RecoJetCollectionGenMatcher
#include "TallinnNtupleProducer/Readers/interface/ReaderBase.h"                               // ReaderBase
#include "TallinnNtupleProducer/Readers/interface/RecoElectronReader.h"                       // RecoElectronReader
#include "TallinnNtupleProducer/Readers/interface/RecoHadTauReader.h"                         // RecoHadTauReader
#include "TallinnNtupleProducer/Readers/interface/RecoJetReaderAK4.h"                         // RecoJetReaderAK4
#include "TallinnNtupleProducer/Readers/interface/RecoJetReaderAK8.h"                         // RecoJetReaderAK8
#include "TallinnNtupleProducer/Readers/interface/RecoMEtReader.h"                            // RecoMEtReader
#include "TallinnNtupleProducer/Readers/interface/RecoMuonReader.h"                           // RecoMuonReader
#include "TallinnNtupleProducer/Readers/interface/RecoVertexReader.h"                         // RecoVertexReader
#include "TallinnNtupleProducer/Readers/interface/TriggerInfoReader.h"                        // TriggerInfoReader
#include "TallinnNtupleProducer/Selectors/interface/RecoElectronCollectionSelectorFakeable.h" // RecoElectronCollectionSelectorFakeable
#include "TallinnNtupleProducer/Selectors/interface/RecoElectronCollectionSelectorLoose.h"    // RecoElectronCollectionSelectorLoose
#include "TallinnNtupleProducer/Selectors/interface/RecoElectronCollectionSelectorTight.h"    // RecoElectronCollectionSelectorTight
#include "TallinnNtupleProducer/Selectors/interface/RecoHadTauCollectionSelectorFakeable.h"   // RecoHadTauCollectionSelectorFakeable
#include "TallinnNtupleProducer/Selectors/interface/RecoHadTauCollectionSelectorLoose.h"      // RecoHadTauCollectionSelectorLoose
#include "TallinnNtupleProducer/Selectors/interface/RecoHadTauCollectionSelectorTight.h"      // RecoHadTauCollectionSelectorTight
#include "TallinnNtupleProducer/Selectors/interface/RecoJetCollectionSelectorAK4.h"           // RecoJetCollectionSelectorAK4
#include "TallinnNtupleProducer/Selectors/interface/RecoJetCollectionSelectorAK4_btag.h"      // RecoJetCollectionSelectorAK4_btag
#include "TallinnNtupleProducer/Selectors/interface/RecoJetCollectionSelectorAK8.h"           // RecoJetCollectionSelectorAK8
#include "TallinnNtupleProducer/Selectors/interface/RecoJetCollectionSelectorAK8_Hbb.h"       // RecoJetCollectionSSelectorAK8_Hbb
#include "TallinnNtupleProducer/Selectors/interface/RecoJetCollectionSelectorAK8_Wjj.h"       // RecoJetCollectionSSelectorAK8_Wjj
#include "TallinnNtupleProducer/Selectors/interface/RecoMuonCollectionSelectorFakeable.h"     // RecoMuonCollectionSelectorFakeable
#include "TallinnNtupleProducer/Selectors/interface/RecoMuonCollectionSelectorLoose.h"        // RecoMuonCollectionSelectorLoose
#include "TallinnNtupleProducer/Selectors/interface/RecoMuonCollectionSelectorTight.h"        // RecoMuonCollectionSelectorTight

#include <set>                                                                                // std::set
#include <string>                                                                             // std::string
#include <vector>                                                                             // std::vector

// forward declarations
class TTree;

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
   * @brief Read branches from tree and use information to fill Event object
   * @return Event object
   */
  Event
  read() const;

  /**
    * @brief Return list of systematic uncertainties supported by EventReader class
    */
  static
  std::vector<std::string>
  get_supported_systematics(const edm::ParameterSet & cfg);

 protected:
  edm::ParameterSet cfg_;

  unsigned numNominalLeptons_;
  unsigned numNominalHadTaus_;

  Era era_;
  bool isMC_;
  bool readGenMatching_;

  EventInfoReader * eventInfoReader_;

  TriggerInfoReader * triggerInfoReader_;

  RecoMuonReader * muonReader_;
  RecoMuonCollectionSelectorLoose * looseMuonSelector_;
  RecoMuonCollectionSelectorFakeable * fakeableMuonSelector_;
  RecoMuonCollectionSelectorTight * tightMuonSelector_;

  RecoElectronReader * electronReader_;
  RecoElectronCollectionCleaner * electronCleaner_;
  RecoElectronCollectionSelectorLoose * looseElectronSelector_;
  RecoElectronCollectionSelectorFakeable * fakeableElectronSelector_;
  RecoElectronCollectionSelectorTight * tightElectronSelector_;

  RecoHadTauReader * hadTauReader_;
  RecoHadTauCollectionCleaner * hadTauCleaner_;
  RecoHadTauCollectionSelectorLoose * looseHadTauSelector_;
  RecoHadTauCollectionSelectorFakeable * fakeableHadTauSelector_;
  RecoHadTauCollectionSelectorTight * tightHadTauSelector_;

  RecoJetReaderAK4 * jetReaderAK4_;
  RecoJetCollectionCleanerAK4 * jetCleanerAK4_dR04_; // used for cleaning AK4 jets wrt electrons, muons, and tauh
  RecoJetCollectionCleanerAK4 * jetCleanerAK4_dR12_; // used for cleaning AK4 jets wrt AK8 jets
  RecoJetCollectionSelectorAK4 * jetSelectorAK4_;
  RecoJetCollectionSelectorAK4_btagLoose * jetSelectorAK4_btagLoose_;
  RecoJetCollectionSelectorAK4_btagMedium * jetSelectorAK4_btagMedium_;

  GenLeptonReader * genLeptonReader_;
  GenHadTauReader * genHadTauReader_;
  GenPhotonReader * genPhotonReader_;
  GenJetReader * genJetReader_;
  RecoMuonCollectionGenMatcher * muonGenMatcher_;
  RecoElectronCollectionGenMatcher * electronGenMatcher_;
  RecoHadTauCollectionGenMatcher * hadTauGenMatcher_;
  RecoJetCollectionGenMatcherAK4 * jetGenMatcherAK4_;

  RecoJetReaderAK8 * jetReaderAK8_Hbb_;
  RecoJetReaderAK8 * jetReaderAK8_Wjj_;
  RecoJetCollectionCleanerAK8 * jetCleanerAK8_dR08_;  // used for cleaning AK8 jets wrt electrons, muons, and tauh
  RecoJetCollectionSelectorAK8_Hbb * jetSelectorAK8_Hbb_;
  RecoJetCollectionSelectorAK8_Wjj * jetSelectorAK8_Wjj_;
  
  RecoMEtReader * metReader_;
  MEtFilterReader * metFilterReader_;

  RecoVertexReader * vertexReader_;

  bool isDEBUG_;
};

#endif // TallinnNtupleProducer_Readers_EventReader_h
