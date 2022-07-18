#ifndef TallinnNtupleProducer_Readers_setFilterBits_h
#define TallinnNtupleProducer_Readers_setFilterBits_h

#include "TallinnNtupleProducer/Objects/interface/TriggerInfo.h"  // TriggerInfo
#include "TallinnNtupleProducer/Objects/interface/RecoElectron.h" // RecoElectron
#include "TallinnNtupleProducer/Objects/interface/RecoMuon.h"     // RecoMuon
#include "TallinnNtupleProducer/Objects/interface/RecoHadTau.h"   // RecoHadTau

#include <DataFormats/Math/interface/deltaR.h>                    // deltaR()

#include <assert.h>                                               // assert()
#include <vector>                                                 // std::vector

template <typename T>
void
setFilterBits(std::vector<T> & particles, const TriggerInfo& triggerInfo)
{
  const std::vector<trigger::Object> & triggerObjects = triggerInfo.objects();
  for ( T & particle : particles )
  {
    const T * particlePtr = &particle;
    Int_t filterBits = 0;
    for ( const trigger::Object & triggerObject : triggerObjects )
    {
      if ( (triggerObject.is_electron() && dynamic_cast<const RecoElectron *>(particlePtr)) ||
           (triggerObject.is_muon()     && dynamic_cast<const RecoMuon *>(particlePtr)    ) ||
           (triggerObject.is_hadTau()   && dynamic_cast<const RecoHadTau *>(particlePtr)  ) ) {
        if ( deltaR(particle.eta(), particle.phi(), triggerObject.eta(), triggerObject.phi()) < 0.05 )
        {
          filterBits |= triggerObject.filterBits();
        }
      }
    }
    particle.set_filterBits(filterBits);
  }
}

#endif // TallinnNtupleProducer_Readers_setFilterBits_h
