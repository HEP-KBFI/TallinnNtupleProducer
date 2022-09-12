#ifndef TallinnNtupleProducer_Readers_setLeptonSF3_h
#define TallinnNtupleProducer_Readers_setLeptonSF3_h

#include "TallinnNtupleProducer/Objects/interface/RecoJetAK8.h"

#include <vector>                                                // std::vector

template <typename T>
void
setLeptonSF3(std::vector<T> & leptons,
             const std::vector<RecoJetAK8> & jets) // leptons -> only electron and muon
{
  if ( leptons.size() )
  {
    bool is_electron = std::fabs(leptons.at(0).pdgId()) == 11;

    for ( unsigned int lepidx=0; lepidx<leptons.size(); ++lepidx )
    {
      for ( const RecoJetAK8 & jet : jets)
      {
        int jet_leptonSF3Idx = is_electron ? jet.electronIdx3SJ() : jet.muonIdx3SJ();
        if ( jet_leptonSF3Idx == (int)lepidx ) 
        {
          leptons.at(lepidx).setBoostedLepton( true );
          leptons.at(lepidx).setlsf3( jet.lsf3() );
          break;
        }
      }
    }
  }
}

#endif // TallinnNtupleProducer_setLeptonSF3_h
