#include "TallinnNtupleProducer/CommonTools/interface/genTools.h"

Int_t
getHiggsDecayMode(const GenParticleCollection & genParticles)
{
  // TODO implement me!
  
  // hww -> 24, hzz -> 23, htt -> 15, hzg -> 220023, hmm -> 13, hbb -> 5
  // tttt -> 15, zzzz -> 23, wwww -> 24, bbtt -> 5000015, bbzz -> 5000023, bbww -> 5000024, ttzz -> 15000023, ttww -> 15000024, zzww -> 23000024
  return 0;
}

double
topPtRwgtSF(const GenParticleCollection & genParticles)
{
  // TODO implement me!
  return 0.;
}
