#include "TallinnNtupleProducer/EvtWeightTools/interface/Data_to_MC_CorrectionInterface_1l_2tau_trigger.h"

#include "TauAnalysisTools/TauTriggerSFs/interface/TauTriggerSFs2017.h"                         // TauTriggerSFs2017

#include "TallinnNtupleProducer/CommonTools/interface/cmsException.h"                           // cmsException()
#include "TallinnNtupleProducer/CommonTools/interface/Era.h"                                    // Era, get_era()
#include "TallinnNtupleProducer/CommonTools/interface/leptonTypes.h"                            // getLeptonType()
#include "TallinnNtupleProducer/CommonTools/interface/sysUncertOptions.h"                       // TriggerSFsys
#include "TallinnNtupleProducer/EvtWeightTools/interface/data_to_MC_corrections_auxFunctions.h" // aux::
#include "TallinnNtupleProducer/Objects/interface/RecoHadTau.h"                                 // RecoHadTau
#include "TallinnNtupleProducer/Objects/interface/RecoLepton.h"                                 // RecoLepton
#include "TallinnNtupleProducer/CommonTools/interface/hadTauDefinitions.h"

#include <assert.h>                                                                             // assert()

Data_to_MC_CorrectionInterface_1l_2tau_trigger::Data_to_MC_CorrectionInterface_1l_2tau_trigger(const edm::ParameterSet & cfg)
  : era_str_(cfg.getParameter<std::string>("era"))
  , era_(get_era(era_str_))
  , hadTauSelection_(cfg.getParameter<std::string>("hadTauSelection"))
  , isDEBUG_(cfg.exists("isDEBUG") ? cfg.getParameter<bool>("isDEBUG") : false)
  , allowedDecayModes_({ 0, 1, 2, 10, 11 })
  , lepton_type_(-1)
  , lepton_pt_(0.)
  , lepton_eta_(0.)
  , hadTau1_genPdgId_(0)
  , hadTau1_pt_(0.)
  , hadTau1_eta_(0.)
  , hadTau1_phi_(0.)
  , hadTau1_decayMode_(0)
  , hadTau2_genPdgId_(0)
  , hadTau2_pt_(0.)
  , hadTau2_eta_(0.)
  , hadTau2_phi_(0.)
  , hadTau2_decayMode_(0)
  , tauId_(get_tau_id_enum(hadTauSelection_))
  , wp_str_(get_tau_id_wp_str(tauId_, get_tau_id_wp_int(hadTauSelection_)))
{
  if(era_ == Era::k2016)
  {
    aux::loadTriggerEff_1e_2016(effTrigger_1e_data_, effTrigger_1e_mc_, inputFiles_);
    aux::loadTriggerEff_1e_1tau_lepLeg_2016(effTrigger_1e1tau_lepLeg_data_, effTrigger_1e1tau_lepLeg_mc_, inputFiles_);

    aux::loadTriggerEff_1m_2016(effTrigger_1m_data_, effTrigger_1m_mc_, inputFiles_);
    aux::loadTriggerEff_1m_1tau_lepLeg_2016(effTrigger_1m1tau_lepLeg_data_, effTrigger_1m1tau_lepLeg_mc_, inputFiles_);
  }
  else if(era_ == Era::k2017)
  {
    aux::loadTriggerEff_1e_2017(effTrigger_1e_data_, effTrigger_1e_mc_, inputFiles_);
    aux::loadTriggerEff_1e_1tau_lepLeg_2017(effTrigger_1e1tau_lepLeg_data_, effTrigger_1e1tau_lepLeg_mc_, inputFiles_);

    aux::loadTriggerEff_1m_2017(effTrigger_1m_data_, effTrigger_1m_mc_, inputFiles_);
    aux::loadTriggerEff_1m_1tau_lepLeg_2017(effTrigger_1m1tau_lepLeg_data_, effTrigger_1m1tau_lepLeg_mc_, inputFiles_);
  }
  else if(era_ == Era::k2018)
  {
    aux::loadTriggerEff_1e_2018(effTrigger_1e_data_, effTrigger_1e_mc_, inputFiles_);
    aux::loadTriggerEff_1e_1tau_lepLeg_2018(effTrigger_1e1tau_lepLeg_data_, effTrigger_1e1tau_lepLeg_mc_, inputFiles_);

    aux::loadTriggerEff_1m_2018(effTrigger_1m_data_, effTrigger_1m_mc_, inputFiles_);
    aux::loadTriggerEff_1m_1tau_lepLeg_2018(effTrigger_1m1tau_lepLeg_data_, effTrigger_1m1tau_lepLeg_mc_, inputFiles_);
  }
  else
  {
    throw cmsException(this) << "Invalid era = " << static_cast<int>(era_);
  }
}

Data_to_MC_CorrectionInterface_1l_2tau_trigger::~Data_to_MC_CorrectionInterface_1l_2tau_trigger()
{
  aux::clearCollection(effTrigger_1e_data_);
  aux::clearCollection(effTrigger_1e_mc_);
  aux::clearCollection(effTrigger_1e1tau_lepLeg_data_);
  aux::clearCollection(effTrigger_1e1tau_lepLeg_mc_);

  aux::clearCollection(effTrigger_1m_data_);
  aux::clearCollection(effTrigger_1m_mc_);
  aux::clearCollection(effTrigger_1m1tau_lepLeg_data_);
  aux::clearCollection(effTrigger_1m1tau_lepLeg_mc_);
}

void
Data_to_MC_CorrectionInterface_1l_2tau_trigger::set_1l_2tau_trigger_sf(correction::Correction::Ref cset)
{
  sf_1l_2tau_trigger_ = cset;
}

void
Data_to_MC_CorrectionInterface_1l_2tau_trigger::setTriggerBits(bool isTriggered_1e,
                                                               bool isTriggered_1e1tau,
                                                               bool isTriggered_1m,
                                                               bool isTriggered_1m1tau)
{
  isTriggered_1e_     = isTriggered_1e;
  isTriggered_1e1tau_ = isTriggered_1e1tau;
  isTriggered_1m_     = isTriggered_1m;
  isTriggered_1m1tau_ = isTriggered_1m1tau;
}

void
Data_to_MC_CorrectionInterface_1l_2tau_trigger::setLepton(const RecoLepton * const lepton)
{
  lepton_type_ = getLeptonType(lepton->pdgId());
  lepton_pt_   = lepton->pt();
  lepton_eta_  = lepton->eta();
}

void
Data_to_MC_CorrectionInterface_1l_2tau_trigger::setHadTaus(const RecoHadTau * const hadTau1,
                                                           const RecoHadTau * const hadTau2)
{
  hadTau1_pt_ = hadTau1->pt();
  hadTau1_eta_ = hadTau1->eta();
  hadTau1_phi_ = hadTau1->phi();
  hadTau1_decayMode_ = hadTau1->decayMode();

  hadTau2_pt_ = hadTau2->pt();
  hadTau2_eta_ = hadTau2->eta();
  hadTau2_phi_ = hadTau2->phi();
  hadTau2_decayMode_ = hadTau2->decayMode();
}

double
Data_to_MC_CorrectionInterface_1l_2tau_trigger::getSF_triggerEff(TriggerSFsys central_or_shift) const
{
  if(isDEBUG_)
  {
    std::cout << get_human_line(this, __func__, __LINE__) << '\n';
  }
  assert(check_triggerSFsys_opt(central_or_shift));

  double eff_1l_data             = 0.;
  double eff_1l_mc               = 0.;
  double eff_1l1tau_lepLeg_data  = 0.;
  double eff_1l1tau_lepLeg_mc    = 0.;
  TauTriggerSFValues eff_1l1tau_tauLeg1_data;
  TauTriggerSFValues eff_1l1tau_tauLeg1_mc;
  TauTriggerSFValues eff_1l1tau_tauLeg2_data;
  TauTriggerSFValues eff_1l1tau_tauLeg2_mc;

  bool isTriggered_1l     = false;
  bool isTriggered_1l1tau = false;

  if(lepton_type_ == kElectron)
  {
    if(isDEBUG_)
    {
      std::cout << "electron: pT = " << lepton_pt_ << ", eta = " << lepton_eta_ << '\n';
    }
    eff_1l_data            = get_from_lut(effTrigger_1e_data_,            lepton_pt_, lepton_eta_, isDEBUG_);
    eff_1l_mc              = get_from_lut(effTrigger_1e_mc_,              lepton_pt_, lepton_eta_, isDEBUG_);
    eff_1l1tau_lepLeg_data = get_from_lut(effTrigger_1e1tau_lepLeg_data_, lepton_pt_, lepton_eta_, isDEBUG_);
    eff_1l1tau_lepLeg_mc   = get_from_lut(effTrigger_1e1tau_lepLeg_mc_,   lepton_pt_, lepton_eta_, isDEBUG_);

    if(std::fabs(hadTau1_eta_) <= 2.1 && aux::hasDecayMode(allowedDecayModes_, hadTau1_decayMode_))
    {
      eff_1l1tau_tauLeg1_data = aux::tau_leg_efficiency(hadTau1_pt_, hadTau1_decayMode_, "etau", wp_str_, "eff_data", sf_1l_2tau_trigger_);
        eff_1l1tau_tauLeg1_mc   = aux::tau_leg_efficiency(hadTau1_pt_, hadTau1_decayMode_, "etau", wp_str_, "eff_mc", sf_1l_2tau_trigger_);
    }

    if(std::fabs(hadTau2_eta_) <= 2.1 && aux::hasDecayMode(allowedDecayModes_, hadTau2_decayMode_))
    {
      eff_1l1tau_tauLeg2_data = aux::tau_leg_efficiency(hadTau2_pt_, hadTau2_decayMode_, "etau", wp_str_, "eff_data", sf_1l_2tau_trigger_);
      eff_1l1tau_tauLeg2_mc   = aux::tau_leg_efficiency(hadTau2_pt_, hadTau2_decayMode_, "etau", wp_str_, "eff_mc", sf_1l_2tau_trigger_);
    }

    isTriggered_1l     = isTriggered_1e_;
    isTriggered_1l1tau = isTriggered_1e1tau_;
  }
  else if(lepton_type_ == kMuon)
  {
    if(isDEBUG_)
    {
      std::cout << "muon: pT = " << lepton_pt_ << ", eta = " << lepton_eta_ << '\n';
    }
    eff_1l_data            = get_from_lut(effTrigger_1m_data_,            lepton_pt_, lepton_eta_, isDEBUG_);
    eff_1l_mc              = get_from_lut(effTrigger_1m_mc_,              lepton_pt_, lepton_eta_, isDEBUG_);
    eff_1l1tau_lepLeg_data = get_from_lut(effTrigger_1m1tau_lepLeg_data_, lepton_pt_, lepton_eta_, isDEBUG_);
    eff_1l1tau_lepLeg_mc   = get_from_lut(effTrigger_1m1tau_lepLeg_mc_,   lepton_pt_, lepton_eta_, isDEBUG_);

    if(std::fabs(hadTau1_eta_) <= 2.1 && aux::hasDecayMode(allowedDecayModes_, hadTau1_decayMode_))
    {
      eff_1l1tau_tauLeg1_data = aux::tau_leg_efficiency(hadTau1_pt_, hadTau1_decayMode_, "mtau", wp_str_, "eff_data", sf_1l_2tau_trigger_);
      eff_1l1tau_tauLeg1_mc = aux::tau_leg_efficiency(hadTau1_pt_,hadTau1_decayMode_, "mtau", wp_str_, "eff_mc", sf_1l_2tau_trigger_);
    }

    if(std::fabs(hadTau2_eta_) <= 2.1 && aux::hasDecayMode(allowedDecayModes_, hadTau2_decayMode_))
    {
      eff_1l1tau_tauLeg2_data = aux::tau_leg_efficiency(hadTau2_pt_, hadTau2_decayMode_, "mutau", wp_str_, "eff_data", sf_1l_2tau_trigger_);
      eff_1l1tau_tauLeg2_mc   = aux::tau_leg_efficiency(hadTau2_pt_, hadTau2_decayMode_, "mutau", wp_str_, "eff_mc", sf_1l_2tau_trigger_);
    }

    isTriggered_1l     = isTriggered_1m_;
    isTriggered_1l1tau = isTriggered_1m1tau_;
  }
  else
  {
    assert(0);
  }

  const TauTriggerSFValues eff_1l1tau_tauLegs_data = 1. - (1. - eff_1l1tau_tauLeg1_data) * (1. - eff_1l1tau_tauLeg2_data);
  const TauTriggerSFValues eff_1l1tau_tauLegs_mc   = 1. - (1. - eff_1l1tau_tauLeg1_mc)   * (1. - eff_1l1tau_tauLeg2_mc);
  if(isDEBUG_)
  {
    std::cout << "hadTau (lead):    pT = " << hadTau1_pt_ << ", eta = " << hadTau1_eta_ << "\n"
                 "hadTau (sublead): pT = " << hadTau2_pt_ << ", eta = " << hadTau2_eta_ << "\n"
                 "eff (L):      data = " << eff_1l_data             << ", MC = " << eff_1l_mc             << "\n"
                 "eff (X_lep):  data = " << eff_1l1tau_lepLeg_data  << ", MC = " << eff_1l1tau_lepLeg_mc  << "\n"
                 "eff (X_tau):  data = " << eff_1l1tau_tauLegs_data << ", MC = " << eff_1l1tau_tauLegs_mc << "\n"
                 "eff (X_tau1): data = " << eff_1l1tau_tauLeg1_data << ", MC = " << eff_1l1tau_tauLeg1_mc << "\n"
                 "eff (X_tau2): data = " << eff_1l1tau_tauLeg2_data << ", MC = " << eff_1l1tau_tauLeg2_mc << '\n'
    ;
  }

  //-------------------------------------------------------------------------------------------------------------------
  // CV: data/MC corrections are agreed as discussed on HTT working mailing list
  //     (email from Alexei Raspereza on February 22nd 2017)
  double sf = 1.;
  if(isTriggered_1l && isTriggered_1l1tau)
  {
    // case 4: both single lepton trigger and lepton+tau cross trigger fire

    const TauTriggerSFValues eff_data = std::min(eff_1l_data, eff_1l1tau_lepLeg_data) * eff_1l1tau_tauLegs_data;
    const TauTriggerSFValues eff_mc   = std::min(eff_1l_mc,   eff_1l1tau_lepLeg_mc)   * eff_1l1tau_tauLegs_mc;

    sf = aux::compSF(eff_data, eff_mc, central_or_shift);
    if(isDEBUG_)
    {
      std::cout << "case 4: both single lepton trigger and lepton+tau cross trigger fire\n"
                   " eff: data = " << eff_data << ", MC = " << eff_mc << " --> SF = " << sf << '\n';
    }
  }
  else if(isTriggered_1l1tau)
  {
    // case 3: lepton+tau cross trigger fires, single lepton trigger doesn't fire

    const TauTriggerSFValues eff_data = ((eff_1l1tau_lepLeg_data - eff_1l_data) * eff_1l1tau_tauLegs_data).max_of(1.e-2);
    const TauTriggerSFValues eff_mc   = ((eff_1l1tau_lepLeg_mc   - eff_1l_mc)   * eff_1l1tau_tauLegs_mc).max_of(1.e-2);

    sf = aux::compSF(eff_data, eff_mc, central_or_shift);
    if(isDEBUG_)
    {
      std::cout << "case 3: lepton+tau cross trigger fires, single lepton trigger doesn't fire\n"
                   " eff: data = " << eff_data << ", MC = " << eff_mc << " --> SF = " << sf << '\n';
    }
  }
  else if(isTriggered_1l)
  {
    // case 2: single lepton trigger fires, lepton+tau cross trigger doesn't fire

    const TauTriggerSFValues eff_data = (eff_1l_data - eff_1l1tau_tauLegs_data * std::min(eff_1l_data, eff_1l1tau_lepLeg_data)).max_of(1.e-2);
    const TauTriggerSFValues eff_mc   = (eff_1l_mc   - eff_1l1tau_tauLegs_mc   * std::min(eff_1l_mc,   eff_1l1tau_lepLeg_mc)).max_of(1.e-2);

    sf = aux::compSF(eff_data, eff_mc, central_or_shift);
    if(isDEBUG_)
    {
      std::cout << "case 2: single lepton trigger fires, lepton+tau cross trigger doesn't fire\n"
                   " eff: data = " << eff_data << ", MC = " << eff_mc << " --> SF = " << sf << '\n';
    }
  }
  else
  {
    // case 1: neither single lepton trigger nor lepton+tau cross trigger fires
    // (SF doesn't matter, as event does not pass event selection)

    sf = 0.;
    if(isDEBUG_)
    {
      std::cout << "case 1: neither single lepton trigger nor lepton+tau cross trigger fires\n"
                   "--> setting SF = " << sf << '\n';
    }
  } // isTriggered_*

  return sf;
}

bool
Data_to_MC_CorrectionInterface_1l_2tau_trigger::check_triggerSFsys_opt(TriggerSFsys central_or_shift) const
{
  return
    central_or_shift == TriggerSFsys::central          ||
    central_or_shift == TriggerSFsys::shiftUp          ||
    central_or_shift == TriggerSFsys::shiftDown        ||
    central_or_shift == TriggerSFsys::shift_1l2tauUp   ||
    central_or_shift == TriggerSFsys::shift_1l2tauDown
  ;
}
