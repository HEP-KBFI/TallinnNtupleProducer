#ifndef TallinnNtupleProducer_Writers_RecoJetPair_Wjj_h
#define TallinnNtupleProducer_Writers_RecoJetPair_Wjj_h

#include "TallinnNtupleProducer/Objects/interface/Particle.h"    // Particle::LorentzVector
#include "TallinnNtupleProducer/Objects/interface/RecoJetAK4.h"  // RecoJetAK4
#include "TallinnNtupleProducer/Objects/interface/RecoJetAK8.h"  // RecoJetAK8
#include "TallinnNtupleProducer/Objects/interface/RecoJetBase.h" // RecoJetBase

#include <vector>                                                // std::vector

class RecoJetPair_Wjj
{
 public:
  RecoJetPair_Wjj(const RecoJetAK4 * jet1, const RecoJetAK4 * jet2);
  RecoJetPair_Wjj(const RecoJetAK8 * fatjet);

  ~RecoJetPair_Wjj();

  const RecoJetBase *
  jet_lead() const;

  const RecoJetBase *
  jet_sublead() const;

  std::vector<const RecoJetBase *>
  jets() const;

  const Particle::LorentzVector & 
  p4() const;

  double
  pt() const;

  double
  eta() const;

  double
  phi() const;

  double
  mass() const;

  enum Type { kBoosted, kResolved };
  Type
  type() const;

 protected:
  void
  update();

  const RecoJetBase * jet_lead_;
  const RecoJetBase * jet_sublead_;

  Particle::LorentzVector p4_; ///< four-vector of jet pair (i.e. of hadronic W candidate)

  Type type_;                  ///< flag indicating whether jet pair has been build from one AK8 jet (boosted case) or from two AK4 jets (resolved case)
};

typedef std::vector<RecoJetPair_Wjj>  RecoJetPairCollection_Wjj;
typedef std::vector<const RecoJetPair_Wjj *>  RecoJetPairPtrCollection_Wjj;

#endif // TallinnNtupleProducer_Writers_RecoJetPair_Wjj_h
