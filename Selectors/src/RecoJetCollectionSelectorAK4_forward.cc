#include "TallinnNtupleProducer/Selectors/interface/RecoJetCollectionSelectorAK4_forward.h"

#include "TallinnNtupleProducer/CommonTools/interface/cmsException.h" // get_human_line()
#include "TallinnNtupleProducer/CommonTools/interface/Era.h"          // Era

RecoJetSelectorAK4_forward::RecoJetSelectorAK4_forward(Era era,
                                                       int index,
                                                       bool debug)
  : RecoJetSelectorAK4(era, index, debug)
  , min_absEta_(2.4)
{
  RecoJetSelectorAK4::set_max_absEta(5.0);
}

double
RecoJetSelectorAK4_forward::get_min_absEta() const
{
  return min_absEta_;
}

bool
RecoJetSelectorAK4_forward::operator()(const RecoJetAK4 & jet) const
{
  if(debug_)
  {
    std::cout << get_human_line(this, __func__) << ":\n jet: " << jet << '\n';
  }
  if(! RecoJetSelectorAK4::operator()(jet))
  {
    return false;
  }
  if(min_absEta_ > 0. && jet.absEta() < min_absEta_)
  {
    if(debug_)
    {
      std::cout << "FAILS abs(eta) = " << jet.absEta() << " >= " << min_absEta_ << " cut\n";
    }
    return false;
  }
  // jet passes all cuts
  return true;
}
