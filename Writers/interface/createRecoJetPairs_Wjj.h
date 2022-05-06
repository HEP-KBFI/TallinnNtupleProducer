#ifndef TallinnNtupleProducer_Writers_createRecoJetPairs_Wjj_h
#define TallinnNtupleProducer_Writers_createRecoJetPairs_Wjj_h

#include "TallinnNtupleProducer/Objects/interface/RecoJetAK4.h"      // RecoJetPtrCollectionAK4
#include "TallinnNtupleProducer/Objects/interface/RecoJetAK8.h"      // RecoJetPtrCollectionAK8
#include "TallinnNtupleProducer/Writers/interface/RecoJetPair_Wjj.h" // RecoJetPairCollection_Wjj

RecoJetPairCollection_Wjj
createRecoJetPairs_Wjj(const RecoJetPtrCollectionAK4 & jetsAK4, const RecoJetPtrCollectionAK8 & jetsAK8_Wjj, size_t numNominalJetPairs);

#endif // TallinnNtupleProducer_Writers_createRecoJetPairs_Wjj_h
