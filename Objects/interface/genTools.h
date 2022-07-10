#ifndef TallinnNtupleProducer_Objects_genTools_h
#define TallinnNtupleProducer_Objects_genTools_h

#include "TallinnNtupleProducer/Objects/interface/GenParticle.h" // GenParticleCollection

Int_t
getHiggsDecayMode(const GenParticleCollection & genParticles);

double
topPtRwgtSF(const GenParticleCollection & genParticles);

#endif // TallinnNtupleProducer_Objects_genTools_h
