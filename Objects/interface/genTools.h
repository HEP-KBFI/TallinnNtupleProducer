#ifndef TallinnNtupleProducer_Objects_genTools_h
#define TallinnNtupleProducer_Objects_genTools_h

#include "TallinnNtupleProducer/Objects/interface/GenParticle.h" // GenParticleCollection

#include <utility> // std::pair<,>

// forward declarations
enum class TopPtRwgtOption;

Int_t
getHiggsDecayMode(const GenParticleCollection & genParticles);

double
topPtRwgtSF(const GenParticleCollection & genParticles,
            TopPtRwgtOption option);

std::pair<const GenParticle *, const GenParticle *>
findTauLeptonPair(const GenParticleCollection & genParticles);

#endif // TallinnNtupleProducer_Objects_genTools_h
