#ifndef TallinnNtupleProducer_CommonTools_genTools_h
#define TallinnNtupleProducer_CommonTools_genTools_h

#include "TallinnNtupleProducer/Objects/interface/GenParticle.h" // GenParticleCollection

Int_t
getHiggsDecayMode(const GenParticleCollection & genParticles);

double
topPtRwgtSF(const GenParticleCollection & genParticles);

#endif // TallinnNtupleProducer_CommonTools_genTools_h
