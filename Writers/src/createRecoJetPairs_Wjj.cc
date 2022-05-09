#include "TallinnNtupleProducer/Writers/interface/createRecoJetPairs_Wjj.h"

#include "DataFormats/Math/interface/deltaR.h"                             // deltaR()

#include "TallinnNtupleProducer/CommonTools/interface/constants.h"         // w_mass
#include "TallinnNtupleProducer/CommonTools/interface/pickFirstNobjects.h" // pickFirstNobjects()

#include <algorithm>                                                       // std::sort()
#include <cmath>                                                           // std::fabs()

namespace
{
  bool 
  isOverlap(const RecoJetAK4 * jet, const RecoJetPairCollection_Wjj & jetPairs)
  {
    bool isOverlap = false;
    for ( const RecoJetPair_Wjj & jetPair : jetPairs )
    {
      if ( (jetPair.jet_lead()    && deltaR(jet->p4(), jetPair.jet_lead()->p4())    < 0.3) ||
           (jetPair.jet_sublead() && deltaR(jet->p4(), jetPair.jet_sublead()->p4()) < 0.3) ) {
        isOverlap = true;
        break;
      }
    }
    return isOverlap;
  }

  bool
  isHigherPriority(const RecoJetPair_Wjj & jetPair1,
                   const RecoJetPair_Wjj & jetPair2)
  {
    // CV: Give preference to boosted over resolved jet pairs.
    //     If both jet pairs are boosted, give preference to the jet pair (i.e. to the AK8 jet) of higher pT.
    //     If both jet pairs are resolved, give preference to jet pairs containing two AK4 jets over jet pairs containing only one AK4 jet;
    //     in case both jet pairs contain two AK4 jets, give preference to the jet pair that has mass closer to mW.
    if ( jetPair1.type() == RecoJetPair_Wjj::Type::kBoosted && jetPair2.type() == RecoJetPair_Wjj::Type::kResolved )
    { 
      return true;
    }
    else if ( jetPair1.type() == RecoJetPair_Wjj::Type::kResolved && jetPair2.type() == RecoJetPair_Wjj::Type::kBoosted )
    {
      return false;
    }
    else if ( jetPair1.type() == RecoJetPair_Wjj::Type::kBoosted && jetPair2.type() == RecoJetPair_Wjj::Type::kBoosted )
    { 
      return jetPair1.pt() > jetPair2.pt();
    }
    else if ( jetPair1.type() == RecoJetPair_Wjj::Type::kResolved && jetPair2.type() == RecoJetPair_Wjj::Type::kResolved )
    {
      if (  jetPair1.jet_lead()    && !jetPair2.jet_lead()    ) return true;
      if ( !jetPair1.jet_lead()    &&  jetPair2.jet_lead()    ) return false;
      if (  jetPair1.jet_sublead() && !jetPair2.jet_sublead() ) return true;
      if ( !jetPair1.jet_sublead() &&  jetPair2.jet_sublead() ) return false;
      double diff1_massW = std::fabs(jetPair1.mass() - w_mass);
      double diff2_massW = std::fabs(jetPair2.mass() - w_mass);
      return diff1_massW < diff2_massW;
    } else assert(0);
    return false;
  }
}

RecoJetPairCollection_Wjj
createRecoJetPairs_Wjj(const RecoJetPtrCollectionAK4 & jetsAK4, const RecoJetPtrCollectionAK8 & jetsAK8_Wjj, size_t numNominalJetPairs)
{
  RecoJetPairCollection_Wjj jetPairs_Wjj;

  for ( const RecoJetAK8 * fatjet : jetsAK8_Wjj )
  {
    if ( fatjet->subJet1() && fatjet->subJet2() )
    {
      jetPairs_Wjj.push_back(RecoJetPair_Wjj(fatjet));
    }
  }

  for ( RecoJetPtrCollectionAK4::const_iterator jet1 = jetsAK4.begin(); jet1 != jetsAK4.end(); ++jet1 )
  {
    if ( isOverlap(*jet1, jetPairs_Wjj) ) continue;
    for ( RecoJetPtrCollectionAK4::const_iterator jet2 = jet1 + 1; jet2 != jetsAK4.end(); ++jet2 )
    {
      if ( isOverlap(*jet2, jetPairs_Wjj) ) continue;
      jetPairs_Wjj.push_back(RecoJetPair_Wjj(*jet1, *jet2));
    }
  }
  
  // Siddhesh: Add up to one unpaired AK4 jet to collection of jet pairs,
  //           in case number of boosted and resolved jet pairs is below nominal number of jets pairs
  if ( jetPairs_Wjj.size() < numNominalJetPairs )
  {
    for ( const RecoJetAK4 * jet : jetsAK4 )
    {
      if ( isOverlap(jet, jetPairs_Wjj) ) continue;
      jetPairs_Wjj.push_back(RecoJetPair_Wjj(jet, nullptr));
    }
  }

  std::sort(jetPairs_Wjj.begin(), jetPairs_Wjj.end(), isHigherPriority);

  return pickFirstNobjects(jetPairs_Wjj, numNominalJetPairs);
}
