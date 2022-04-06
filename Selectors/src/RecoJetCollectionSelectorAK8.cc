#include "TallinnNtupleProducer/Selectors/interface/RecoJetCollectionSelectorAK8.h"

#include "TallinnNtupleProducer/CommonTools/interface/cmsException.h" // get_human_line()
#include "TallinnNtupleProducer/CommonTools/interface/Era.h"          // Era

RecoJetSelectorAK8::RecoJetSelectorAK8(Era era,
                                       int index,
                                       bool debug)
  : min_pt_(100.)
  , max_absEta_(2.4)
  , min_msoftdrop_(50.)
  , debug_(debug)
{}

void
RecoJetSelectorAK8::set_min_pt(double min_pt)
{
  min_pt_ = min_pt;
}

void
RecoJetSelectorAK8::set_max_absEta(double max_absEta)
{
  max_absEta_ = max_absEta;
}

void
RecoJetSelectorAK8::set_min_msoftdrop(double min_msoftdrop)
{
  min_msoftdrop_ = min_msoftdrop;
}

double
RecoJetSelectorAK8::get_min_pt() const
{
  return min_pt_;
}

double
RecoJetSelectorAK8::get_max_absEta() const
{
  return max_absEta_;
}

double
RecoJetSelectorAK8::get_min_msoftdrop() const
{
  return min_msoftdrop_;
}

bool
RecoJetSelectorAK8::operator()(const RecoJetAK8 & jet) const
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
  if (jet.msoftdrop() < min_msoftdrop_)
  {
    if(debug_)
    {
      std::cout << "FAILS softdrop mass = " << jet.msoftdrop() << " >= " << min_msoftdrop_ << " cut\n";
    }
    return false;
  }
  // jet passes all cuts
  return true;
}
