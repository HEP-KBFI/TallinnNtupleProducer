#include "TallinnNtupleProducer/Writers/interface/RecoJetPair_Wjj.h"

#include <algorithm> // std::sort()
#include <cmath>     // std::fabs()

namespace
{
  std::vector<const RecoJetBase *>
  get_sorted_jets(const RecoJetBase * jet1, const RecoJetBase * jet2)
  {
    std::vector<const RecoJetBase *> sorted_jets;
    if ( jet1 )
    {
      sorted_jets.push_back(jet1);
    }
    if ( jet2 )
    {
      sorted_jets.push_back(jet2);
    }
    std::sort(sorted_jets.begin(), sorted_jets.end());
    return sorted_jets;
  }

  const RecoJetBase *
  get_jet(const std::vector<const RecoJetBase *> & jets, size_t idx)
  {
    if ( idx < jets.size() )
    {
      return jets.at(idx);
    }
    else
    {
      return nullptr;
    }
  }

  const RecoJetBase *
  get_leading_jet(const RecoJetBase * jet1, const RecoJetBase * jet2)
  {
    std::vector<const RecoJetBase *> sorted_jets = get_sorted_jets(jet1, jet2);
    return get_jet(sorted_jets, 0);
  }

  const RecoJetBase *
  get_subleading_jet(const RecoJetBase * jet1, const RecoJetBase * jet2)
  {
    std::vector<const RecoJetBase *> sorted_jets = get_sorted_jets(jet1, jet2);
    return get_jet(sorted_jets, 1);
  }
}

RecoJetPair_Wjj::RecoJetPair_Wjj(const RecoJetAK4 * jet1, const RecoJetAK4 * jet2)
  : jet_lead_(get_leading_jet(jet1, jet2))
  , jet_sublead_(get_subleading_jet(jet1, jet2))
  , type_(Type::kResolved)
{
  update();
}

RecoJetPair_Wjj::RecoJetPair_Wjj(const RecoJetAK8 * fatjet)
  : jet_lead_(get_leading_jet(fatjet->subJet1(), fatjet->subJet2()))
  , jet_sublead_(get_subleading_jet(fatjet->subJet1(), fatjet->subJet2()))
  , type_(Type::kBoosted)
{
  update();
}

RecoJetPair_Wjj::~RecoJetPair_Wjj()
{}

void
RecoJetPair_Wjj::update()
{
  p4_ = Particle::LorentzVector(0., 0., 0., 0.);
  if ( jet_lead_ )
  {
    p4_ += jet_lead_->p4();
  }
  if ( jet_sublead_ )
  {
    p4_ += jet_sublead_->p4();
  }
  pt_ = p4_.pt();
  eta_ = p4_.eta();
  phi_ = p4_.phi();
  mass_ = p4_.mass();
  absEta_ = std::fabs(eta_);
  isValid_ = true;
}

const RecoJetBase *
RecoJetPair_Wjj::jet_lead() const
{
  return jet_lead_;
}

const RecoJetBase *
RecoJetPair_Wjj::jet_sublead() const
{
  return jet_sublead_;
}

std::vector<const RecoJetBase *>
RecoJetPair_Wjj::jets() const
{
  std::vector<const RecoJetBase *> jets;
  if ( jet_lead_    ) jets.push_back(jet_lead_);
  if ( jet_sublead_ ) jets.push_back(jet_sublead_);
  return jets;
}

const Particle::LorentzVector & 
RecoJetPair_Wjj::p4() const
{
  return p4_;
}

double
RecoJetPair_Wjj::pt() const
{
  return p4_.pt();
}

double
RecoJetPair_Wjj::eta() const
{
  return p4_.eta();
}

double
RecoJetPair_Wjj::phi() const
{
  return p4_.phi();
}

double
RecoJetPair_Wjj::mass() const
{
  return p4_.mass();
}

RecoJetPair_Wjj::Type
RecoJetPair_Wjj::type() const
{
  return type_;
}
