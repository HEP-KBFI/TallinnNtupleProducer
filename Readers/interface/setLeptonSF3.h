#ifndef TallinnNtupleProducer_Readers_setLeptonSF3_h
#define TallinnNtupleProducer_Readers_setLeptonSF3_h

#include "TallinnNtupleProducer/Objects/interface/RecoJetAK8.h"

#include <vector>                                                // std::vector

template <typename T>
void
setLeptonSF3(std::vector<T> & particles,
                const std::vector<RecoJetAK8> & jets)
{
  if ( particles.size() )
  {
    bool is_electron = std::fabs(particles.at(0).pdgId()) == 11;

    for ( unsigned int lepidx=0; lepidx<particles.size(); lepidx++ )
    {
      particles.at(lepidx).setBoostedLepton(false);
      particles.at(lepidx).setlsf3(-1);
      for ( const RecoJetAK8 & jet : jets)
      {
        int jet_leptonSF3Idx = (is_electron) ? jet.electronIdx3SJ() : jet.muonIdx3SJ();
        if ( jet_leptonSF3Idx == (int)lepidx ) 
        {
          particles.at(lepidx).setBoostedLepton( true );
          particles.at(lepidx).setlsf3( jet.lsf3() );
          break;
        }
      }
    }
  }
}

#endif // TallinnNtupleProducer_setLeptonSF3_h
