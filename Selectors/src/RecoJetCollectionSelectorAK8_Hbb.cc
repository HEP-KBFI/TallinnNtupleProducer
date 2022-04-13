#include "TallinnNtupleProducer/Selectors/interface/RecoJetCollectionSelectorAK8_Hbb.h"

#include "TallinnNtupleProducer/CommonTools/interface/cmsException.h"   // get_human_line()
#include "TallinnNtupleProducer/CommonTools/interface/Era.h"            // Era
#include "TallinnNtupleProducer/CommonTools/interface/jetDefinitions.h" // Btag, BtagWP, get_BtagWP()

RecoJetSelectorAK8_Hbb::RecoJetSelectorAK8_Hbb(Era era,
                                               int index,
                                               bool debug)
  : min_pt_(200.)
  , max_absEta_(2.4)
  , min_msoftdrop_(30.)
  , max_msoftdrop_(210.)
  , max_tau2_div_tau1_(0.75)
  , min_subJet1_pt_(20.)
  , max_subJet1_absEta_(2.4)
  , min_subJet2_pt_(20.)
  , max_subJet2_absEta_(2.4)
  , min_btagCSV_loose_(get_BtagWP(era, Btag::kDeepCSV, BtagWP::kLoose))
  , min_btagCSV_medium_(get_BtagWP(era, Btag::kDeepCSV, BtagWP::kMedium))
  , min_numSubJets_btagLoose_(-1)
  , min_numSubJets_btagMedium_(1)
  , debug_(debug)
{
  switch(era)
  {
    case Era::kUndefined: throw cmsException(this) << "Undefined era!";
    case Era::k2016: min_jetId_ = 1; break; // 1 means loose
    case Era::k2018:
    case Era::k2017: min_jetId_ = 2; break; // 2 means tight (loose jet ID deprecated since 94x)
    default: throw cmsException(this) << "Implement me!";
  }
}

void
RecoJetSelectorAK8_Hbb::set_min_pt(double min_pt)
{
  min_pt_ = min_pt;
}

void
RecoJetSelectorAK8_Hbb::set_max_absEta(double max_absEta)
{
  max_absEta_ = max_absEta;
}

void
RecoJetSelectorAK8_Hbb::set_min_msoftdrop(double min_msoftdrop)
{
  min_msoftdrop_ = min_msoftdrop;
}

void
RecoJetSelectorAK8_Hbb::set_max_msoftdrop(double max_msoftdrop)
{
  max_msoftdrop_ = max_msoftdrop;
}

void
RecoJetSelectorAK8_Hbb::set_max_tau2_div_tau1(double max_tau2_div_tau1)
{
  max_tau2_div_tau1_ = max_tau2_div_tau1;
}

void
RecoJetSelectorAK8_Hbb::set_min_subJet1_pt(double min_pt)
{
  min_subJet1_pt_ = min_pt;
}

void
RecoJetSelectorAK8_Hbb::set_max_subJet1_absEta(double max_absEta)
{
  max_subJet1_absEta_ = max_absEta;
}

void
RecoJetSelectorAK8_Hbb::set_min_subJet2_pt(double min_pt)
{
  min_subJet2_pt_ = min_pt;
}

void
RecoJetSelectorAK8_Hbb::set_max_subJet2_absEta(double max_absEta)
{
  max_subJet2_absEta_ = max_absEta;
}

void
RecoJetSelectorAK8_Hbb::set_min_btagCSV_loose(double min_btagCSV_loose) 
{
  min_btagCSV_loose_ = min_btagCSV_loose;
}
 
void 
RecoJetSelectorAK8_Hbb::set_min_btagCSV_medium(double min_btagCSV_medium) 
{
  min_btagCSV_medium_ = min_btagCSV_medium;
}

void
RecoJetSelectorAK8_Hbb::set_min_numSubJets_btagLoose(int min_numSubJets_btagLoose) 
{
  min_numSubJets_btagLoose_ = min_numSubJets_btagLoose;
}
 
void
RecoJetSelectorAK8_Hbb::set_min_numSubJets_btagMedium(int min_numSubJets_btagMedium) 
{
  min_numSubJets_btagMedium_ = min_numSubJets_btagMedium;
}

void 
RecoJetSelectorAK8_Hbb::set_min_jetId(int min_jetId)
{
  min_jetId_ = min_jetId;
}

double
RecoJetSelectorAK8_Hbb::get_min_pt() const
{
  return min_pt_;
}

double
RecoJetSelectorAK8_Hbb::get_max_absEta() const
{
  return max_absEta_;
}

double
RecoJetSelectorAK8_Hbb::get_min_msoftdrop() const
{
  return min_msoftdrop_;
}

double
RecoJetSelectorAK8_Hbb::get_max_msoftdrop() const
{
  return max_msoftdrop_;
}

double
RecoJetSelectorAK8_Hbb::get_max_tau2_div_tau1() const
{
  return max_tau2_div_tau1_;
}

double
RecoJetSelectorAK8_Hbb::get_min_subJet1_pt() const
{
  return min_subJet1_pt_;
}

double
RecoJetSelectorAK8_Hbb::get_max_subJet1_absEta() const
{
  return max_subJet1_absEta_;
}

double
RecoJetSelectorAK8_Hbb::get_min_subJet2_pt() const
{
  return min_subJet2_pt_;
}

double
RecoJetSelectorAK8_Hbb::get_max_subJet2_absEta() const
{
  return max_subJet2_absEta_;
}

double 
RecoJetSelectorAK8_Hbb::get_min_btagCSV_loose() const
{
  return min_btagCSV_loose_;
}
 
double 
RecoJetSelectorAK8_Hbb::get_min_btagCSV_medium() const
{
  return min_btagCSV_medium_;
}

int 
RecoJetSelectorAK8_Hbb::get_min_numSubJets_btagLoose() const
{
  return min_numSubJets_btagLoose_;
}
 
int 
RecoJetSelectorAK8_Hbb::get_min_numSubJets_btagMedium() const
{
  return min_numSubJets_btagMedium_;
}

bool
RecoJetSelectorAK8_Hbb::operator()(const RecoJetAK8 & jet) const
{
  int numSubJets_btagLoose  = 0;
  int numSubJets_btagMedium = 0;
  if ( jet.subJet1() && jet.subJet1()->is_btagable() )
  {
    const double subJet1_btagCSV = jet.subJet1()->BtagCSV();
    if ( subJet1_btagCSV >= min_btagCSV_loose_  ) ++numSubJets_btagLoose;
    if ( subJet1_btagCSV >= min_btagCSV_medium_ ) ++numSubJets_btagMedium;
  }

  if ( jet.subJet2() && jet.subJet2()->is_btagable() )
  {
    const double subJet2_btagCSV = jet.subJet2()->BtagCSV();
    if ( subJet2_btagCSV >= min_btagCSV_loose_  ) ++numSubJets_btagLoose;
    if ( subJet2_btagCSV >= min_btagCSV_medium_ ) ++numSubJets_btagMedium;
  }
  const bool passes =
    jet.pt()                  >= min_pt_                   &&
    jet.absEta()              <= max_absEta_               &&
    jet.jetId()               >= min_jetId_                &&
    jet.msoftdrop()           >= min_msoftdrop_            &&
    jet.msoftdrop()           <= max_msoftdrop_            &&
    jet.tau2() / jet.tau1()   <= max_tau2_div_tau1_        &&
    jet.subJet1() && jet.subJet2()                         &&
    ((jet.subJet1()->pt()     >= min_subJet1_pt_           && 
      jet.subJet1()->absEta() <= max_subJet1_absEta_       && 
      jet.subJet2()->pt()     >= min_subJet2_pt_           && 
      jet.subJet2()->absEta() <= max_subJet2_absEta_)      && 
     (jet.subJet1()->pt()     >= min_subJet2_pt_           && 
      jet.subJet1()->absEta() <= max_subJet2_absEta_       && 
      jet.subJet2()->pt()     >= min_subJet1_pt_           && 
      jet.subJet2()->absEta() <= max_subJet1_absEta_))     &&
     numSubJets_btagLoose     >= min_numSubJets_btagLoose_ &&
     numSubJets_btagMedium    >= min_numSubJets_btagMedium_
  ;
  if(debug_)
  {
    std::cout << get_human_line(this, __func__, __LINE__) << "\n"
                 "jet: " << jet << " (" << (passes ? "passes" : "fails") << ")\n"
    ;
  }
  return passes;
}
