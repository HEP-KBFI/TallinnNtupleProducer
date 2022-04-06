#include "TallinnNtupleProducer/Selectors/interface/RecoJetCollectionSelectorAK4.h"

#include "TallinnNtupleProducer/CommonTools/interface/as_integer.h"     // as_integer()
#include "TallinnNtupleProducer/CommonTools/interface/cmsException.h"   // get_human_line()
#include "TallinnNtupleProducer/CommonTools/interface/Era.h"            // Era
#include "TallinnNtupleProducer/CommonTools/interface/jetDefinitions.h" // pileupJetID

RecoJetSelectorAK4::RecoJetSelectorAK4(Era era,
                                       int index,
                                       bool debug)
  : era_(era)
  , min_pt_(25.)
  , max_absEta_(2.4)
  , min_jetId_(-1.e+3)
  , apply_pileupJetId_(pileupJetID::kPileupJetID_disabled)
  , debug_(debug)
{
  switch(era)
  {
    case Era::kUndefined: throw cmsException(this) << "Undefined era!";
    case Era::k2016: min_jetId_ = 1; break; // 1 means loose
    case Era::k2017:
    case Era::k2018: min_jetId_ = 2; break; // 2 means tight (loose jet ID deprecated since 94x)
    default: throw cmsException(this) << "Implement me!";
  }
}

void
RecoJetSelectorAK4::set_min_pt(double min_pt)
{
  min_pt_ = min_pt;
}

void
RecoJetSelectorAK4::set_max_absEta(double max_absEta)
{
  max_absEta_ = max_absEta;
}

void
RecoJetSelectorAK4::set_min_jetId(int min_jetId)
{
  min_jetId_ = min_jetId;
}

void 
RecoJetSelectorAK4::set_pileupJetId(pileupJetID apply_pileupJetId)
{
  apply_pileupJetId_ = apply_pileupJetId;
}

double
RecoJetSelectorAK4::get_min_pt() const
{
  return min_pt_;
}

double
RecoJetSelectorAK4::get_max_absEta() const
{
  return max_absEta_;
}

int
RecoJetSelectorAK4::get_min_jetId() const
{
  return min_jetId_;
}

pileupJetID
RecoJetSelectorAK4::get_pileupJetId() const
{
  return apply_pileupJetId_;
}

bool
RecoJetSelectorAK4::operator()(const RecoJetAK4 & jet) const
{
  if(debug_)
  {
    std::cout << get_human_line(this, __func__) << ":\n jet: " << jet << '\n';
  }

  if(jet.pt() < min_pt_)
  {
    if(debug_)
    {
      std::cout << "FAILS pT = " << jet.pt() << " >= " << min_pt_ << " cut\n";
    }
    return false;
  }
  if(max_absEta_ > 0. && jet.absEta() > max_absEta_)
  {
    if(debug_)
    {
      std::cout << "FAILS abs(eta) = " << jet.absEta() << " <= " << max_absEta_ << " cut\n";
    }
    return false;
  }
  // CV: Ignore jets with 2.65 < |eta| < 3.139 and pT < 50 GeV when selecting jets in 2017 data and MC,
  //     in order to reduce effect of noise in ECAL endcap,
  //     cf. slide 13 of presentation by Alexei Raspereza in HTT meeting on October 10th 2018
  //    (https://indico.cern.ch/event/762837/contributions/3172618/attachments/1731302/2798220/Recoils_20181010.pdf )
  // Karl: "rounded" the eta thresholds, increased the pT threshold and applied it equally to all eras, see:
  // https://gitlab.cern.ch/ttH_leptons/doc/blob/01f187e5b2efb39f93004acb7606a149c765ea18/Legacy/objects.md#23-jets
  if(jet.pt() < 60. && jet.absEta() > 2.7 && jet.absEta() < 3.)
  {
    if(debug_)
    {
      std::cout << "FAILS EE noise cut\n";
    }
    return false;
  }
  if(jet.jetId() < min_jetId_)
  {
    if(debug_)
    {
      std::cout << "FAILS PF-jet ID = " << jet.jetId() << " >= " << min_jetId_ << " cut\n";
    }
    return false;
  }
  if ( apply_pileupJetId_ != pileupJetID::kPileupJetID_disabled && jet.is_PUID_taggable() )
  {
    if ( ! jet.passesPUID(apply_pileupJetId_) )
    {
      if(debug_)
      {
        std::cout << "FAILS PU-jet ID = " << jet.puId() << " & " << as_integer(apply_pileupJetId_) << " cut\n";
      }
      return false;
    }
  }
  // jet passes all cuts
  return true;
}
