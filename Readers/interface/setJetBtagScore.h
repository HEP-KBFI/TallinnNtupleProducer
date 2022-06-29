#ifndef TallinnNtupleProducer_Readers_setJetBtagScore_h
#define TallinnNtupleProducer_Readers_setJetBtagScore_h



#include "TallinnNtupleProducer/Objects/interface/RecoJetAK4.h"
#include "TallinnNtupleProducer/Objects/interface/RecoLepton.h"

#include <assert.h>                                              // assert()
#include <vector>                                                // std::vector

template <typename T>
void
setJetBtagScore(std::vector<T> & particles, const std::vector<RecoJetAK4> & jets)
{
  Int_t njets = jets.size();

  for ( T & particle : particles )
  {
    T * particlePtr = &particle;
    Int_t jetIdx = particlePtr->jetIdx();
    if ( jetIdx >=0 &&  jetIdx <njets)
    {
      double btag = jets.at(jetIdx).BtagCSV();
      particlePtr->setjetBtagCSV(btag);
    }
    else
    {
      particlePtr->setjetBtagCSV(-1);
    }
  }
}

#endif // TallinnNtupleProducer_Readers_setJetBtagScore_h
