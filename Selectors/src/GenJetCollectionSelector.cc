#include "TallinnNtupleProducer/Selectors/interface/GenJetCollectionSelector.h"

#include "TallinnNtupleProducer/CommonTools/interface/cmsException.h" // get_human_line()
#include "TallinnNtupleProducer/CommonTools/interface/Era.h"          // Era

GenJetSelector::GenJetSelector(Era era,
			       int index,
			       bool debug)
  : era_(era)
  , min_pt_(25.)
  , max_absEta_(2.4)
  , debug_(debug)
{}

void
GenJetSelector::set_min_pt(double min_pt)
{
  min_pt_ = min_pt;
}

void
GenJetSelector::set_max_absEta(double max_absEta)
{
  max_absEta_ = max_absEta;
}

double
GenJetSelector::get_min_pt() const
{
  return min_pt_;
}

double
GenJetSelector::get_max_absEta() const
{
  return max_absEta_;
}

bool
GenJetSelector::operator()(const GenJet & jet) const
{
  if(debug_)
  {
    std::cout << get_human_line(this, __func__) << ":\n jet: " << jet << '\n';
  }

  if(jet.pt() < min_pt_)
  {
    if(debug_)
    {
      std::cout << "FAILS pT >= " << min_pt_ << " cut\n";
    }
    return false;
  }
  if(max_absEta_ > 0. && jet.absEta() > max_absEta_)
  {
    if(debug_)
    {
      std::cout << "FAILS abs(eta) <= " << max_absEta_ << " cut\n";
    }
    return false;
  }
  // jet passes all cuts
  return true;
}
