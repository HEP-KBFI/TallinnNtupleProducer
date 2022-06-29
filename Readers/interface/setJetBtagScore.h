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
  const Int_t njets = jets.size();

  for ( T & particle : particles )
  {
    const Int_t jetIdx = particle.jetIdx();
    double btag = ( jetIdx >=0 &&  jetIdx <njets ) ? jets.at(jetIdx).BtagCSV() : -1;
    if(std::isnan(btag))
    {
      btag = -2;
    }
    particle.setjetBtagCSV(btag);
  }
}

#endif // TallinnNtupleProducer_Readers_setJetBtagScore_h
