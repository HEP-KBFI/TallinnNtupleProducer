#include "TallinnNtupleProducer/Objects/interface/Event.h"

#include <TString> // TString

#include <string>  // std::string

const EventInfo&
Event::eventInfo() const
{
  return eventInfo_;
}

const TriggerInfo&
Event::triggerInfo() const
{
  return triggerInfo_;
}

const RecoMuonPtrCollection&
Event::looseMuons() const
{
  return looseMuons_;
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
  return looseElectrons_;
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
Event::looseLeptons() const
{
  return looseLeptons_;
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

const RecoJetPtrCollectionAK4&
Event::selJetsAK4() const
{
  return selJetsAK4_;
}

const RecoJetPtrCollectionAK4&
Event::selBJetsAK4_loose() const
{
  return selBJetsAK4_loose_;
}

const RecoJetPtrCollectionAK4&
Event::selBJetsAK4_medium() const
{
  return selBJetsAK4_medium_;
}

const RecoJetPtrCollectionAK8&
Event::selJetsAK8() const
{
  return selJetsAK8_;
}

const RecoMEt&
Event::met() const
{
  return met_;
}

const RecoVertex&
Event::vertex() const
{
  return vertex_;
}

namespace
{
  template <typename T>
  void 
  printCollection(const std::string & label, const std::vector<const T*> & collection)
  {
    std::string collectionName = TString(label.data()).ReplaceAll("[s]", "s").Data();
    std::string objectName = TString(label.data()).ReplaceAll("[s]", "").Data();
    stream << "#" << collectionName << " = " << collection.size() << std::endl;
    size_t numObjects = objects.size();
    for ( size_t idxObject = 0; idxObject < numObjects; ++idxObject )
    {
      const T* object = collection[idxObject];
      std::cout << objectName << " #" << idxObject << ": " << *object;
    }
  }
}

std::ostream &
operator << (std::ostream & stream, const Event & event)
{
  stream << event.eventInfo();

  stream << event.triggerInfo();

  printCollection("preselMuon[s]", preselMuons_);
  printCollection("fakeableMuon[s]", fakeableMuons_);
  printCollection("tightMuon[s]", tightMuons_);

  printCollection("preselElectron[s]", preselElectrons_);
  printCollection("fakeableElectron[s]", fakeableElectrons_);
  printCollection("tightElectron[s]", tightElectrons_);

  printCollection("preselHadTau[s]", preselHadTaus_);
  printCollection("fakeableHadTau[s]", fakeableHadTaus_);
  printCollection("tightHadTau[s]", tightHadTaus_);

  printCollection("selJet[s]AK4", selJetsAK4_);

  printCollection("selBJet[s]AK4_loose", selBJetsAK4_loose_);
  printCollection("selBJet[s]AK4_medium", selBJetsAK4_medium_);

  printCollection("selJet[s]AK8", selJetsAK8_);

  stream << " met: " << met_;

  stream << " vertex: " << vertex_;
}
