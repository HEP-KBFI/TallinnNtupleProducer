#include "TallinnNtupleProducer/CommonTools/interface/leptonTypes.h"

#include <assert.h> // assert()
#include <cmath>    // std::abs()

int
getLeptonType(int pdgId)
{
  const int abs_pdgId = std::abs(pdgId);
  switch(abs_pdgId)
  {
    case 11: return kElectron;
    case 13: return kMuon;
    default: assert(0);
  }
}
