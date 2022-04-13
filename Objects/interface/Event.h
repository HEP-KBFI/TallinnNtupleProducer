#ifndef TallinnNtupleProducer_Objects_Event_h
#define TallinnNtupleProducer_Objects_Event_h

#include "TallinnNtupleProducer/Objects/interface/EventInfo.h"    // EventInfo
#include "TallinnNtupleProducer/Objects/interface/MEtFilter.h"    // MEtFilter
#include "TallinnNtupleProducer/Objects/interface/TriggerInfo.h"  // TriggerInfo
#include "TallinnNtupleProducer/Objects/interface/RecoElectron.h" // RecoElectronPtrCollection
#include "TallinnNtupleProducer/Objects/interface/RecoHadTau.h"   // RecoHadTauPtrCollection
#include "TallinnNtupleProducer/Objects/interface/RecoJetAK4.h"   // RecoJetPtrCollectionAK4
#include "TallinnNtupleProducer/Objects/interface/RecoJetAK8.h"   // RecoJetPtrCollectionAK8
#include "TallinnNtupleProducer/Objects/interface/RecoLepton.h"   // RecoLeptonPtrCollection
#include "TallinnNtupleProducer/Objects/interface/RecoMEt.h"      // RecoMEt
#include "TallinnNtupleProducer/Objects/interface/RecoMuon.h"     // RecoMuonPtrCollection
#include "TallinnNtupleProducer/Objects/interface/RecoVertex.h"   // RecoVertex

class Event
{
 public:
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

  const RecoJetPtrCollectionAK8& selJetsAK8() const;
  const RecoJetPtrCollectionAK8& selJetsAK8_Hbb() const;
  const RecoJetPtrCollectionAK8& selJetsAK8_Wjj() const;

  const RecoMEt& met() const;
  const MEtFilter& metFilters() const;

  const RecoVertex& vertex() const;

  friend class EventReader;

 protected:
  const EventInfo & eventInfo_;

  const TriggerInfo & triggerInfo_;

  RecoMuonPtrCollection looseMuons_;
  RecoMuonPtrCollection fakeableMuons_;
  RecoMuonPtrCollection tightMuons_;

  RecoElectronPtrCollection looseElectrons_;
  RecoElectronPtrCollection fakeableElectrons_;
  RecoElectronPtrCollection tightElectrons_;

  RecoLeptonPtrCollection looseLeptons_;
  RecoLeptonPtrCollection fakeableLeptons_;
  RecoLeptonPtrCollection tightLeptons_;

  RecoHadTauPtrCollection fakeableHadTaus_;
  RecoHadTauPtrCollection tightHadTaus_;

  RecoJetPtrCollectionAK4 selJetsAK4_;
  RecoJetPtrCollectionAK4 selJetsAK4_btagLoose_;
  RecoJetPtrCollectionAK4 selJetsAK4_btagMedium_;

  RecoJetPtrCollectionAK8 selJetsAK8_;
  RecoJetPtrCollectionAK8 selJetsAK8_Hbb_;
  RecoJetPtrCollectionAK8 selJetsAK8_Wjj_;

  RecoMEt met_;
  MEtFilter metFilters_;

  RecoVertex vertex_;
};

std::ostream &
operator<<(std::ostream & stream,
           const Event & event);

#endif // TallinnNtupleProducer_Objects_Event_h
