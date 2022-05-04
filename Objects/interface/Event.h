#ifndef TallinnNtupleProducer_Objects_Event_h
#define TallinnNtupleProducer_Objects_Event_h

#include "TallinnNtupleProducer/Objects/interface/EventInfo.h"    // EventInfo
#include "TallinnNtupleProducer/Objects/interface/GenHadTau.h"    // GenHadTauCollection
#include "TallinnNtupleProducer/Objects/interface/GenLepton.h"    // GenLeptonCollection
#include "TallinnNtupleProducer/Objects/interface/GenJet.h"       // GenJetCollection
#include "TallinnNtupleProducer/Objects/interface/GenPhoton.h"    // GenPhotonCollection
#include "TallinnNtupleProducer/Objects/interface/MEtFilter.h"    // MEtFilter
#include "TallinnNtupleProducer/Objects/interface/TriggerInfo.h"  // TriggerInfo
#include "TallinnNtupleProducer/Objects/interface/RecoElectron.h" // RecoElectronCollection, RecoElectronPtrCollection
#include "TallinnNtupleProducer/Objects/interface/RecoHadTau.h"   // RecoHadTauCollection, RecoHadTauPtrCollection
#include "TallinnNtupleProducer/Objects/interface/RecoJetAK4.h"   // RecoJetCollectionAK4, RecoJetPtrCollectionAK4
#include "TallinnNtupleProducer/Objects/interface/RecoJetAK8.h"   // RecoJetCollectionAK8, RecoJetPtrCollectionAK8
#include "TallinnNtupleProducer/Objects/interface/RecoLepton.h"   // RecoLeptonPtrCollection
#include "TallinnNtupleProducer/Objects/interface/RecoMEt.h"      // RecoMEt
#include "TallinnNtupleProducer/Objects/interface/RecoMuon.h"     // RecoMuonCollection, RecoMuonPtrCollection
#include "TallinnNtupleProducer/Objects/interface/RecoVertex.h"   // RecoVertex

class Event
{
 public:
  Event();
  Event(const EventInfo& eventInfo, const TriggerInfo& triggerInfo);

  ~Event();

  /**
   * @brief Funtions to access collections of particles
   * @return Collections of particles
   */
  const EventInfo& eventInfo() const;

  const TriggerInfo& triggerInfo() const;

  const RecoMuonPtrCollection& looseMuons() const;
  const RecoMuonPtrCollection& fakeableMuons() const;
  const RecoMuonPtrCollection& tightMuons()const;

  const RecoElectronPtrCollection& looseElectrons() const;
  const RecoElectronPtrCollection& fakeableElectrons() const;
  const RecoElectronPtrCollection& tightElectrons() const;

  const RecoLeptonPtrCollection& looseLeptons() const;
  const RecoLeptonPtrCollection& fakeableLeptons() const;
  const RecoLeptonPtrCollection& tightLeptons() const;

  const RecoHadTauPtrCollection& fakeableHadTaus() const;
  const RecoHadTauPtrCollection& tightHadTaus() const;

  const RecoJetPtrCollectionAK4& selJetsAK4() const;
  const RecoJetPtrCollectionAK4& selJetsAK4_btagLoose() const;
  const RecoJetPtrCollectionAK4& selJetsAK4_btagMedium() const;

  const RecoJetPtrCollectionAK8& selJetsAK8_Hbb() const;
  const RecoJetPtrCollectionAK8& selJetsAK8_Wjj() const;

  const RecoMEt& met() const;
  const MEtFilter& metFilters() const;

  const RecoVertex& vertex() const;

  friend class EventReader;

 protected:
  const EventInfo * eventInfo_;

  const TriggerInfo * triggerInfo_;

  // CV: non-Ptr collections of RecoMuons, RecoElectrons, RecoHadTaus, RecoJetsAK4, RecoJetsAK8
  //     and of GenLeptons, GenHadTaus, GenPhotons, and GenJets need to be stored in the Event,
  //     so that these objects don get out of scope and the Ptr collections become invalid !!

  RecoMuonCollection muons_;
  RecoMuonPtrCollection muon_ptrs_;
  RecoMuonPtrCollection looseMuonsFull_;
  RecoMuonPtrCollection fakeableMuonsFull_;
  RecoMuonPtrCollection tightMuonsFull_;
  RecoMuonPtrCollection fakeableMuons_;
  RecoMuonPtrCollection tightMuons_;

  RecoElectronCollection electrons_;
  RecoElectronPtrCollection electron_ptrs_;
  RecoElectronPtrCollection looseElectronsUncleaned_;
  RecoElectronPtrCollection fakeableElectronsUncleaned_;
  RecoElectronPtrCollection tightElectronsUncleaned_;
  RecoElectronPtrCollection looseElectronsFull_;
  RecoElectronPtrCollection fakeableElectronsFull_;
  RecoElectronPtrCollection tightElectronsFull_;
  RecoElectronPtrCollection fakeableElectrons_;
  RecoElectronPtrCollection tightElectrons_;

  RecoLeptonPtrCollection looseLeptonsFull_;
  RecoLeptonPtrCollection fakeableLeptonsFull_;
  RecoLeptonPtrCollection tightLeptonsFull_;
  RecoLeptonPtrCollection fakeableLeptons_;
  RecoLeptonPtrCollection tightLeptons_;

  RecoHadTauCollection hadTaus_;
  RecoHadTauPtrCollection hadTau_ptrs_;
  RecoHadTauPtrCollection fakeableHadTausUncleaned_;
  RecoHadTauPtrCollection tightHadTausUncleaned_;
  RecoHadTauPtrCollection fakeableHadTausFull_;
  RecoHadTauPtrCollection tightHadTausFull_;
  RecoHadTauPtrCollection fakeableHadTaus_;
  RecoHadTauPtrCollection tightHadTaus_;

  RecoJetCollectionAK4 jetsAK4_;
  RecoJetPtrCollectionAK4 jet_ptrsAK4_;
  RecoJetPtrCollectionAK4 selJetsUncleanedAK4_;
  RecoJetPtrCollectionAK4 selJetsUncleanedAK4_btagLoose_;
  RecoJetPtrCollectionAK4 selJetsUncleanedAK4_btagMedium_;
  RecoJetPtrCollectionAK4 selJetsAK4_;
  RecoJetPtrCollectionAK4 selJetsAK4_btagLoose_;
  RecoJetPtrCollectionAK4 selJetsAK4_btagMedium_;

  GenLeptonCollection genLeptons_;
  GenLeptonCollection genElectrons_;
  GenLeptonCollection genMuons_;
  GenHadTauCollection genHadTaus_;
  GenPhotonCollection genPhotons_;
  GenJetCollection genJets_;

  RecoJetCollectionAK8 jetsAK8_Hbb_;
  RecoJetPtrCollectionAK8 jet_ptrsAK8_Hbb_;
  RecoJetPtrCollectionAK8 selJetsUncleanedAK8_Hbb_;
  RecoJetPtrCollectionAK8 selJetsAK8_Hbb_;
  RecoJetCollectionAK8 jetsAK8_Wjj_;
  RecoJetPtrCollectionAK8 jet_ptrsAK8_Wjj_;
  RecoJetPtrCollectionAK8 selJetsAK8_Wjj_;

  RecoMEt met_;
  MEtFilter metFilters_;

  RecoVertex vertex_;
};

std::ostream &
operator<<(std::ostream & stream,
           const Event & event);

#endif // TallinnNtupleProducer_Objects_Event_h
