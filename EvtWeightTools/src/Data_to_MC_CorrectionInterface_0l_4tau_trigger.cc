#include "TallinnNtupleProducer/EvtWeightTools/interface/Data_to_MC_CorrectionInterface_0l_4tau_trigger.h"
#include "TallinnNtupleProducer/EvtWeightTools/interface/data_to_MC_corrections_auxFunctions.h" // aux::
#include "TallinnNtupleProducer/Objects/interface/RecoHadTau.h"                                 // RecoHadTau::filterBits()
#include "TallinnNtupleProducer/CommonTools/interface/cmsException.h"                           // cmsException()
#include "TallinnNtupleProducer/CommonTools/interface/sysUncertOptions.h"                       // TriggerSFsys, getTriggerSF_option()

#include <TString.h> // Form()

#include <boost/algorithm/string/predicate.hpp> // boost::ends_with()

#include <cassert> // assert()

Data_to_MC_CorrectionInterface_0l_4tau_trigger::Data_to_MC_CorrectionInterface_0l_4tau_trigger(const edm::ParameterSet & cfg)
  : Data_to_MC_CorrectionInterface_0l_2tau_trigger(cfg)
{}

Data_to_MC_CorrectionInterface_0l_4tau_trigger::~Data_to_MC_CorrectionInterface_0l_4tau_trigger()
{}

double
Data_to_MC_CorrectionInterface_0l_4tau_trigger::getSF_triggerEff(TriggerSFsys central_or_shift) const
{
  if(isDEBUG_)
  {
    std::cout << get_human_line(this, __func__, __LINE__) << '\n';
  }
  assert(check_triggerSFsys_opt(central_or_shift));

  std::string sys;
  if ( central_or_shift == TriggerSFsys::central )
  {
    sys = "nom";
  }
  else if ( central_or_shift == TriggerSFsys::shiftUp || central_or_shift == TriggerSFsys::shift_0l2tauUp )
  {
    sys = "up";
  }
  else
  {
    sys = "down";
  }

  std::vector<const RecoHadTau*> matched_hadTaus;

  for (auto hadTau_recObj : hadTau_recObjs)
  {
    //can we use 64, 128 filterbits only as they are related to di-tau trigger
    //https://cmssdt.cern.ch/lxr/source/PhysicsTools/NanoAOD/python/triggerObjects_cff.py?v=CMSSW_12_6_X_2022-09-08-2300 #Line-0122
    if ( hadTau_recObj->filterBits() ) matched_hadTaus.push_back(hadTau_recObj);
  }

  assert( matched_hadTaus.size() >= 2 );

  double prob_data(1);
  double prob_mc(1);
  for( auto matched_hadTau : matched_hadTaus)
  {
    prob_data *= tau_leg_efficiency(matched_hadTau->pt(), matched_hadTau->decayMode(), "ditau", wp_str_, "eff_data", sys);
    prob_mc *= tau_leg_efficiency(matched_hadTau->pt(), matched_hadTau->decayMode(), "ditau", wp_str_, "eff_mc", sys);
  }

  if ( isDEBUG_ )
    std::cout << "prob_data: " << prob_data << "\t prob_mc: " << prob_mc << std::endl;

  const double sf = aux::compSF(prob_data, prob_mc);
  return sf;
}
 
bool
Data_to_MC_CorrectionInterface_0l_4tau_trigger::check_triggerSFsys_opt(TriggerSFsys central_or_shift) const
{
  return
    central_or_shift == TriggerSFsys::central          ||
    central_or_shift == TriggerSFsys::shiftUp          ||
    central_or_shift == TriggerSFsys::shiftDown        ||
    central_or_shift == TriggerSFsys::shift_0l2tauUp   ||
    central_or_shift == TriggerSFsys::shift_0l2tauDown
  ;
}
