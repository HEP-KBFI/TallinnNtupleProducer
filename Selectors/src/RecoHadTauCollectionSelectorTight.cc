#include "TallinnNtupleProducer/Selectors/interface/RecoHadTauCollectionSelectorTight.h"

#include "TallinnNtupleProducer/CommonTools/interface/cmsException.h"      // get_human_line()
#include "TallinnNtupleProducer/CommonTools/interface/Era.h"               // Era
#include "TallinnNtupleProducer/CommonTools/interface/hadTauDefinitions.h" // TauID

RecoHadTauSelectorTight::RecoHadTauSelectorTight(Era era,
                                                 int index,
                                                 bool debug,
                                                 bool set_selection_flags)
  : RecoHadTauSelectorBase(era, index, debug, set_selection_flags)
{}

void
RecoHadTauSelectorTight::set_selection_flags(const RecoHadTau & hadTau) const
{
  hadTau.set_isTight();
}

bool
RecoHadTauSelectorTight::set_if_looser(const std::string & cut)
{
  throw cmsException(this, __func__, __LINE__) << "Invalid call";
}

bool
RecoHadTauSelectorTight::operator()(const RecoHadTau & hadTau) const
{
  if(debug_)
  {
    std::cout << get_human_line(this, __func__) << ":\n hadTau: " << hadTau << '\n';
  }
  return RecoHadTauSelectorBase::operator()(hadTau);
}
