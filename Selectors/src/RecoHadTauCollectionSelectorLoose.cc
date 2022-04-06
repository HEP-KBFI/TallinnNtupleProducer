#include "TallinnNtupleProducer/Selectors/interface/RecoHadTauCollectionSelectorLoose.h"

#include "TallinnNtupleProducer/CommonTools/interface/cmsException.h"      // get_human_line()
#include "TallinnNtupleProducer/CommonTools/interface/Era.h"               // Era
#include "TallinnNtupleProducer/CommonTools/interface/hadTauDefinitions.h" // TauID

RecoHadTauSelectorLoose::RecoHadTauSelectorLoose(Era era,
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
  // Up to this point the selection coincides with the fakeable tau selection. In order to preserve the event
  // categorization, we need to keep the 2-prong taus (DMs 5 and 6) as well as eliminate the cuts on the loosests WPs
  // of anti-e and anti-mu DeepTau ID. The decision was made in https://indico.cern.ch/event/867117/?showSession=51
  // so that the new fakeable tau definition would not interfere with the event selection defined with the previous
  // tau definition as implemented in this class. The jet multiplicity changes if we don't use dedicated tau
  // collection used in the jet cleaning (ie the collection returned by this class), which in turn gives rise to
  // the migration of events between event categories -- an unwanted effect for people who already have performed
  // the event skimming at this point in time.
  reset_cuts();
}

void
RecoHadTauSelectorLoose::set_selection_flags(const RecoHadTau & hadTau) const
{
  hadTau.set_isLoose();
}

bool
RecoHadTauSelectorLoose::set_if_looser(const std::string & cut)
{
  const bool hasChanged = RecoHadTauSelectorBase::set_if_looser(cut);
  if(hasChanged)
  {
    reset_cuts();
  }
  return hasChanged;
}

void
RecoHadTauSelectorLoose::set(const std::string & cut)
{
  RecoHadTauSelectorBase::set(cut);
  reset_cuts();
}

void
RecoHadTauSelectorLoose::reset_cuts()
{
  decayMode_blacklist_.clear(); // keep 2-prong taus
  apply_deeptau_lepton_ = false; // disable the cuts on the loosest WPs of anti-e/mu DeepTau ID
}

bool
RecoHadTauSelectorLoose::operator()(const RecoHadTau & hadTau) const
{
  if(debug_)
  {
    std::cout << get_human_line(this, __func__) << ":\n hadTau: " << hadTau << '\n';
  }
  return RecoHadTauSelectorBase::operator()(hadTau);
}
