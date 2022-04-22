#include "TallinnNtupleProducer/Selectors/interface/RecoJetCollectionSelectorAK4_btag.h"

#include "TallinnNtupleProducer/CommonTools/interface/cmsException.h"   // get_human_line()
#include "TallinnNtupleProducer/CommonTools/interface/Era.h"            // Era
#include "TallinnNtupleProducer/CommonTools/interface/jetDefinitions.h" // BtagWP, get_BtagWP()

RecoJetSelectorAK4_btag::RecoJetSelectorAK4_btag(Era era,
                                                 int index,
                                                 bool debug)
  : RecoJetSelectorAK4(era, index, debug)
  , min_BtagCSV_(-1.e+3)
  , BtagWP_(BtagWP::kUndefined)
{}

double
RecoJetSelectorAK4_btag::get_min_BtagCSV() const
{
  return min_BtagCSV_;
}

RecoJetSelectorAK4_btagLoose::RecoJetSelectorAK4_btagLoose(Era era,
                                                           int index,
                                                           bool debug)
  : RecoJetSelectorAK4_btag(era, index, debug)
{
  min_BtagCSV_ = get_BtagWP(era_, Btag::kDeepJet, BtagWP::kLoose);
  BtagWP_ = BtagWP::kLoose;
}

RecoJetSelectorAK4_btagMedium::RecoJetSelectorAK4_btagMedium(Era era,
                                                             int index,
                                                             bool debug)
  : RecoJetSelectorAK4_btag(era, index, debug)
{
  min_BtagCSV_ = get_BtagWP(era_, Btag::kDeepJet, BtagWP::kMedium);
  BtagWP_ = BtagWP::kMedium;
}

bool
RecoJetSelectorAK4_btag::operator()(const RecoJetAK4 & jet) const
{
  if(debug_)
  {
    std::cout << get_human_line(this, __func__) << ":\n jet: " << jet << '\n';
  }
  if(! RecoJetSelectorAK4::operator()(jet))
  {
    return false;
  }
  if(jet.BtagCSV() < min_BtagCSV_)
  {
    if(debug_)
    {
      std::cout << "FAILS b-tagging CSV = " << jet.BtagCSV() << " >= " << min_BtagCSV_ << " cut\n";
    }
    return false;
  }
  // jet passes all cuts
  if ( BtagWP_ == BtagWP::kLoose  ) jet.set_isBJet_loose();
  if ( BtagWP_ == BtagWP::kMedium ) jet.set_isBJet_medium();
  return true;
}
