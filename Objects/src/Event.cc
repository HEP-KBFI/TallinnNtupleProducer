#include "TallinnNtupleProducer/Objects/interface/Event.h"

#include "TallinnNtupleProducer/Objects/interface/genTools.h" // ::topPtRwgtSF()

#include <TString.h> // TString

#include <string>    // std::string

Event::Event()
  : eventInfo_(nullptr)
  , triggerInfo_(nullptr)
  , isValid_(false)
{}

Event::Event(const EventInfo& eventInfo, const TriggerInfo& triggerInfo)
  : eventInfo_(&eventInfo)
  , triggerInfo_(&triggerInfo)
  , isValid_(false)
{}

Event::~Event()
{}

const EventInfo&
Event::eventInfo() const
{
  assert(eventInfo_);
  return *eventInfo_;
}

double
Event::topPtRwgtSF() const
{
  return ::topPtRwgtSF(genParticles_, eventInfo_->analysisConfig().apply_topPtReweighting());
}

const TriggerInfo&
Event::triggerInfo() const
{
  assert(triggerInfo_);
  return *triggerInfo_;
}

const RecoMuonPtrCollection&
Event::looseMuons() const
{
  return looseMuonsFull_;
}

const RecoMuonPtrCollection&
Event::fakeableMuons() const
{
  return fakeableMuons_;
}

const RecoMuonPtrCollection&
Event::tightMuons()const
{
  return tightMuons_;
}

const RecoElectronPtrCollection&
Event::looseElectrons() const
{
  return looseElectronsFull_;
}

const RecoElectronPtrCollection&
Event::fakeableElectrons() const
{
  return fakeableElectrons_;
}

const RecoElectronPtrCollection&
Event::tightElectrons() const
{
  return tightElectrons_;
}

const RecoLeptonPtrCollection&
Event::fakeableLeptons() const
{
  return fakeableLeptons_;
}

const RecoLeptonPtrCollection&
Event::tightLeptons() const
{
  return tightLeptons_;
}

const RecoLeptonPtrCollection&
Event::looseLeptonsUncleaned() const
{
  return looseLeptonsUncleaned_;
}

const RecoLeptonPtrCollection&
Event::looseLeptonsFull() const
{
  return looseLeptonsFull_;
}

const RecoLeptonPtrCollection&
Event::fakeableLeptonsFull() const
{
  return fakeableLeptonsFull_;
}

const RecoLeptonPtrCollection&
Event::tightLeptonsFull() const
{
  return tightLeptonsFull_;
}

const RecoHadTauPtrCollection&
Event::fakeableHadTaus() const
{
  return fakeableHadTaus_;
}

const RecoHadTauPtrCollection&
Event::tightHadTaus() const
{
  return tightHadTaus_;
}

const RecoHadTauPtrCollection&
Event::fakeableHadTausFull() const
{
  return fakeableHadTausFull_;
}

const RecoHadTauPtrCollection&
Event::tightHadTausFull() const
{
  return tightHadTausFull_;
}

const RecoJetPtrCollectionAK4&
Event::selJetsAK4() const
{
  return selJetsAK4_;
}

const RecoJetPtrCollectionAK4&
Event::selJetsAK4_btagLoose() const
{
  return selJetsAK4_btagLoose_;
}

const RecoJetPtrCollectionAK4&
Event::selJetsAK4_btagMedium() const
{
  return selJetsAK4_btagMedium_;
}

const RecoJetPtrCollectionAK8&
Event::selJetsAK8_Hbb() const
{
  return selJetsAK8_Hbb_;
}

const RecoJetPtrCollectionAK8&
Event::selJetsAK8_Wjj() const
{
  return selJetsAK8_Wjj_;
}

const RecoMEt&
Event::met() const
{
  return met_;
}

const MEtFilter& 
Event::metFilters() const
{
  return metFilters_;
}

const RecoVertex&
Event::vertex() const
{
  return vertex_;
}

const GenParticleCollection&
Event::genParticles() const
{
  return genParticles_;
}

namespace
{
  template <typename T>
  void 
  printCollection(std::ostream & stream, const std::string & label, const std::vector<const T*> & collection)
  {
    std::string collectionName = TString(label.data()).ReplaceAll("[s]", "s").Data();
    std::string objectName = TString(label.data()).ReplaceAll("[s]", "").Data();
    stream << "#" << collectionName << " = " << collection.size() << std::endl;
    size_t numObjects = collection.size();
    for ( size_t idxObject = 0; idxObject < numObjects; ++idxObject )
    {
      const T* object = collection[idxObject];
      stream << objectName << " #" << idxObject << ": " << *object;
    }
  }
}

bool 
Event::isInvalid() const
{
  return !isValid_;
}

std::ostream &
operator << (std::ostream & stream, const Event & event)
{
  stream << event.eventInfo();

  stream << event.triggerInfo();

  printCollection(stream, "looseMuon[s]", event.looseMuons());
  printCollection(stream, "fakeableMuon[s]", event.fakeableMuons());
  printCollection(stream, "tightMuon[s]", event.tightMuons());

  printCollection(stream, "looseElectron[s]", event.looseElectrons());
  printCollection(stream, "fakeableElectron[s]", event.fakeableElectrons());
  printCollection(stream, "tightElectron[s]", event.tightElectrons());

  printCollection(stream, "fakeableHadTau[s]", event.fakeableHadTaus());
  printCollection(stream, "tightHadTau[s]", event.tightHadTaus());

  printCollection(stream, "selJet[s]AK4", event.selJetsAK4());
  printCollection(stream, "selJet[s]AK4_btagLoose", event.selJetsAK4_btagLoose());
  printCollection(stream, "selJet[s]AK4_btagMedium", event.selJetsAK4_btagMedium());

  printCollection(stream, "selJet[s]AK8_Hbb", event.selJetsAK8_Hbb());
  printCollection(stream, "selJet[s]AK8_Wjj", event.selJetsAK8_Wjj());

  stream << " met: " << event.met();

  stream << " vertex: " << event.vertex();

  return stream;
}
