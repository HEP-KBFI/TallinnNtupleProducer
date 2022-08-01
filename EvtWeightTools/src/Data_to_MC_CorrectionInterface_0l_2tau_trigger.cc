#include "TallinnNtupleProducer/EvtWeightTools/interface/Data_to_MC_CorrectionInterface_0l_2tau_trigger.h"

#include "TallinnNtupleProducer/CommonTools/interface/cmsException.h"                           // get_human_line()
#include "TallinnNtupleProducer/CommonTools/interface/sysUncertOptions.h"                       // TriggerSFsys
#include "TallinnNtupleProducer/EvtWeightTools/interface/data_to_MC_corrections_auxFunctions.h" // aux::

#include <assert.h>                                                                             // assert()
#include <cmath>                                                                                // std::fabs()

Data_to_MC_CorrectionInterface_0l_2tau_trigger::Data_to_MC_CorrectionInterface_0l_2tau_trigger(const edm::ParameterSet & cfg)
  : Data_to_MC_CorrectionInterface_trigger_Base(cfg)
{}

Data_to_MC_CorrectionInterface_0l_2tau_trigger::~Data_to_MC_CorrectionInterface_0l_2tau_trigger()
{}

void
Data_to_MC_CorrectionInterface_0l_2tau_trigger::setTriggerBits(bool isTriggered_2tau)
{
  isTriggered_2tau_ = isTriggered_2tau;
}

double
Data_to_MC_CorrectionInterface_0l_2tau_trigger::getSF_triggerEff(TriggerSFsys central_or_shift) const
{
  if(isDEBUG_)
  {
    std::cout << get_human_line(this, __func__, __LINE__) << '\n';
  }
  assert(check_triggerSFsys_opt(central_or_shift));

  double eff_2tau_tauLeg1_data = 0.;
  double eff_2tau_tauLeg1_mc   = 0.;
  double eff_2tau_tauLeg2_data = 0.;
  double eff_2tau_tauLeg2_mc   = 0.;

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

  if(std::fabs(hadTau1_eta_) <= 2.1 && aux::hasDecayMode(allowedDecayModes_, hadTau1_decayMode_))
  {
    eff_2tau_tauLeg1_data = tau_leg_efficiency(hadTau1_pt_, hadTau1_decayMode_, "ditau", wp_str_, "eff_data", sys);
    eff_2tau_tauLeg1_mc   = tau_leg_efficiency(hadTau1_pt_, hadTau1_decayMode_, "ditau", wp_str_, "eff_mc", sys);
  }

  if(std::fabs(hadTau2_eta_) <= 2.1 && aux::hasDecayMode(allowedDecayModes_, hadTau2_decayMode_))
  {
    eff_2tau_tauLeg2_data = tau_leg_efficiency(hadTau2_pt_, hadTau2_decayMode_, "ditau", wp_str_, "eff_data", sys);
    eff_2tau_tauLeg2_mc   = tau_leg_efficiency(hadTau2_pt_, hadTau2_decayMode_, "ditau", wp_str_, "eff_mc", sys);
  }

  if(isDEBUG_)
  {
    std::cout << "hadTau (lead):    pT = " << hadTau1_pt_ << ", eta = " << hadTau1_eta_ << "\n"
                 "hadTau (sublead): pT = " << hadTau2_pt_ << ", eta = " << hadTau2_eta_ << "\n"
                 "eff (tau1): data = " << eff_2tau_tauLeg1_data << ", MC = " << eff_2tau_tauLeg1_mc << "\n"
                 "eff (tau2): data = " << eff_2tau_tauLeg2_data << ", MC = " << eff_2tau_tauLeg2_mc << '\n'
    ;
  }

  double sf = 1.;
  if(isTriggered_2tau_)
  {
    // case 2: ditau trigger fires

    const double eff_data = eff_2tau_tauLeg1_data * eff_2tau_tauLeg2_data;
    const double eff_mc   = eff_2tau_tauLeg1_mc   * eff_2tau_tauLeg2_mc;
    if ( eff_data < 1.e-3 && eff_mc < 1.e-3 )
    {
      std::cout << "tauLeg1: pT = " << hadTau1_pt_ << ", eta = " << hadTau1_eta_ << std::endl;
      std::cout << "tauLeg2: pT = " << hadTau2_pt_ << ", eta = " << hadTau2_eta_ << std::endl;
      std::cout << " eff_data = " << eff_data << ": eff_2tau_tauLeg1_data = " << eff_2tau_tauLeg1_data << ", eff_2tau_tauLeg1_data = " << eff_2tau_tauLeg2_data << std::endl;
      std::cout << " eff_mc = " << eff_mc << ": eff_2tau_tauLeg1_mc = " << eff_2tau_tauLeg1_mc << ", eff_2tau_tauLeg1_mc = " << eff_2tau_tauLeg2_mc << std::endl;
    }
    sf = aux::compSF(eff_data, eff_mc);
    if(isDEBUG_)
    {
      std::cout << "case 2: ditau trigger fires\n"
                   " eff: data = " << eff_data << ", MC = " << eff_mc << " --> SF = " << sf << '\n'
      ;
    }
  }
  else
  {
    // case 1: ditau trigger doesn't fire
    // (SF doesn't matter, as event does not pass event selection)

    sf = 0.;
    if(isDEBUG_)
    {
      std::cout << "case 1: ditau trigger doesn't fire\n"
                   "--> setting SF = " << sf << '\n'
      ;
    }
  }

  return sf;
}

bool
Data_to_MC_CorrectionInterface_0l_2tau_trigger::check_triggerSFsys_opt(TriggerSFsys central_or_shift) const
{
  return
    central_or_shift == TriggerSFsys::central          ||
    central_or_shift == TriggerSFsys::shiftUp          ||
    central_or_shift == TriggerSFsys::shiftDown        ||
    central_or_shift == TriggerSFsys::shift_0l2tauUp   ||
    central_or_shift == TriggerSFsys::shift_0l2tauDown
  ;
}
