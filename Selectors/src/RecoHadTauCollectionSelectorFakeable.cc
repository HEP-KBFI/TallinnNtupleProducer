#include "TallinnNtupleProducer/Selectors/interface/RecoHadTauCollectionSelectorFakeable.h"

#include "TallinnNtupleProducer/CommonTools/interface/cmsException.h"      // get_human_line()
#include "TallinnNtupleProducer/CommonTools/interface/Era.h"               // Era
#include "TallinnNtupleProducer/CommonTools/interface/hadTauDefinitions.h" // TauID

RecoHadTauSelectorFakeable::RecoHadTauSelectorFakeable(Era era,
                                                       int index,
                                                       bool debug,
                                                       bool set_selection_flags)
  : RecoHadTauSelectorBase(era, index, debug, set_selection_flags)
{
  const TauID tauId = TauID::DeepTau2017v2VSjet;
  std::string tauId_str = "";
  for(const auto & kv: TauID_PyMap)
  {
    if(kv.second == tauId)
    {
      tauId_str = kv.first;
      break;
    }
  }
  assert(! tauId_str.empty());
  set(tauId_str + nominal_fakeable_wp_.at(tauId));
}

void
RecoHadTauSelectorFakeable::set_selection_flags(const RecoHadTau & hadTau) const
{
  hadTau.set_isFakeable();
}

bool
RecoHadTauSelectorFakeable::operator()(const RecoHadTau & hadTau) const
{
  if(debug_)
  {
    std::cout << get_human_line(this, __func__) << ":\n hadTau: " << hadTau << '\n';
  }
  return RecoHadTauSelectorBase::operator()(hadTau);
}
