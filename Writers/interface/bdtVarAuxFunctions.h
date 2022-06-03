#ifndef TallinnNtupleProducer_Writers_bdtVarAuxFunctions_h
#define TallinnNtupleProducer_Writers_bdtVarAuxFunctions_h

#include "TallinnNtupleProducer/Objects/interface/Particle.h"   // Particle
#include "TallinnNtupleProducer/Objects/interface/RecoHadTau.h" // RecoHadTau
#include "TallinnNtupleProducer/Objects/interface/RecoJetAK4.h" // RecoJetAK4
#include "TallinnNtupleProducer/Objects/interface/RecoLepton.h" // RecoLepton
#include "TallinnNtupleProducer/Objects/interface/RecoMEt.h"    // RecoMEt

#include <vector>                                               // std::vector

double 
comp_min_dR_jet(const Particle & particle, const std::vector<const RecoJetAK4 *> & jets_cleaned);

double
comp_mT(const RecoLepton * lepton, const RecoMEt * met);
double
comp_mT(const RecoHadTau * hadTau, const RecoMEt * met);

template <typename T>
Particle::LorentzVector
comp_sumP4(const std::vector<const T *> & particles)
{
  Particle::LorentzVector sumP4;
  for ( const T * particle : particles )
  {
    //const RecoLepton * lepton = dynamic_cast<const RecoLepton *>(particle);
    //if ( lepton ) sumP4 += lepton->cone_p4();
    //else          sumP4 += particle->p4();
    sumP4 += particle->p4(); // CV: only for testing (backwards compatibility with old HH->multilepton 2lss code) !!
  }
  return sumP4;
}

#endif // TallinnNtupleProducer_Writers_bdtVarAuxFunctions_h
