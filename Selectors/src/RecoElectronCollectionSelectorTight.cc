#include "TallinnNtupleProducer/Selectors/interface/RecoElectronCollectionSelectorTight.h"

#include "TallinnNtupleProducer/CommonTools/interface/electronDefinitions.h" // EGammaWP
#include "TallinnNtupleProducer/CommonTools/interface/cmsException.h"        // get_human_line()
#include "TallinnNtupleProducer/CommonTools/interface/Era.h"                 // Era
#include "TallinnNtupleProducer/CommonTools/interface/jetDefinitions.h"      // Btag, BtagWP, get_BtagWP()

RecoElectronSelectorTight::RecoElectronSelectorTight(Era era,
                                                     int index,
                                                     bool debug,
                                                     bool set_selection_flags)
  : era_(era)
  , set_selection_flags_(set_selection_flags)
  , apply_offline_e_trigger_cuts_(true)
  , debug_(debug)
  , min_lepton_pt_(7.) // L
  , min_cone_pt_(10) // F
  , max_absEta_(2.5) // F
  , max_dxy_(0.05) // F
  , max_dz_(0.1) // F
  , max_relIso_(0.4) // F
  , max_sip3d_(8.) // F
  , binning_absEta_(1.479) // F
  , min_sigmaEtaEta_trig_(0.011) // F
  , max_sigmaEtaEta_trig_(0.019) // F
  , max_HoE_trig_(0.10) // F
  , min_OoEminusOoP_trig_(-0.04) // F
  , max_jetBtagCSV_(get_BtagWP(era_, Btag::kDeepJet, BtagWP::kMedium)) // F
  , max_nLostHits_(0) // F
  , apply_conversionVeto_(true) // F
  , invert_nLostHits_(false)
  , min_nLostHits_fornLostHitsInversion_(1)
  , invert_conversionVeto_(false)
{
  // L -- inherited from the preselection (loose cut)
  // F -- inherited from the fakeable selection
  // T -- additional tight cut not applied in the fakeable selection
}

void
RecoElectronSelectorTight::enable_offline_e_trigger_cuts()
{
  apply_offline_e_trigger_cuts_ = true;
}

void
RecoElectronSelectorTight::disable_offline_e_trigger_cuts()
{
  apply_offline_e_trigger_cuts_ = false;
}

// enable/disable photon conversion veto
void
RecoElectronSelectorTight::enable_conversionVeto()
{
  apply_conversionVeto_ = true;
}

void
RecoElectronSelectorTight::disable_conversionVeto()
{
  apply_conversionVeto_ = false;
}

void
RecoElectronSelectorTight::invert_max_nLostHits(Int_t min_nLostHits_fornLostHitsInversion)
{
  invert_nLostHits_ = true;
  min_nLostHits_fornLostHitsInversion_ = min_nLostHits_fornLostHitsInversion;
  std::cout << "RecoElectronSelectorTight::invert_max_nLostHits():: invert_nLostHits_ with min_nLostHits " << min_nLostHits_fornLostHitsInversion_ << ".\n";
}

void
RecoElectronSelectorTight::invert_conversionVeto()
{
  apply_conversionVeto_  = false;
  invert_conversionVeto_ = true;
}


void
RecoElectronSelectorTight::set_min_lepton_pt(double min_lepton_pt)
{
  min_lepton_pt_ = min_lepton_pt;
}

void
RecoElectronSelectorTight::set_min_cone_pt(double min_cone_pt)
{
  min_cone_pt_ = min_cone_pt;
}

void
RecoElectronSelectorTight::set_max_absEta(double max_absEta)
{
  max_absEta_ = max_absEta;
}
 
double
RecoElectronSelectorTight::get_min_lepton_pt() const
{
  return min_lepton_pt_;
}

double
RecoElectronSelectorTight::get_min_cone_pt() const
{
  return min_cone_pt_;
}

double
RecoElectronSelectorTight::get_max_absEta() const
{
  return max_absEta_;
}

void
RecoElectronSelectorTight::set_selection_flags(bool selection_flag)
{
  set_selection_flags_ = selection_flag;
}

void
RecoElectronSelectorTight::print_selection_conditions()
{
  std::cout << "RecoElectronSelectorTight::print_selection_conditions():: " << "\n";
  std::cout << " \t set_selection_flags:" << set_selection_flags_ << "\n";
  std::cout << " \t apply_offline_e_trigger_cuts_:" << apply_offline_e_trigger_cuts_ << "\n";
  std::cout << " \t min_lepton_pt_:" << min_lepton_pt_ << "\n";
  std::cout << " \t min_cone_pt_:" << min_cone_pt_ << "\n";
  std::cout << " \t max_absEta_:" << max_absEta_ << "\n";
  std::cout << " \t max_dxy_:" << max_dxy_ << "\n";
  std::cout << " \t max_dz_:" << max_dz_ << "\n";  
  std::cout << " \t max_relIso_:" << max_relIso_ << "\n";
  std::cout << " \t max_sip3d_:" << max_sip3d_ << "\n";
  std::cout << " \t binning_absEta_:" << binning_absEta_ << "\n";
  std::cout << " \t min_sigmaEtaEta_trig_:" << min_sigmaEtaEta_trig_ << "\n";
  std::cout << " \t max_sigmaEtaEta_trig_:" << max_sigmaEtaEta_trig_ << "\n";
  std::cout << " \t max_HoE_trig_:" << max_HoE_trig_ << "\n";
  std::cout << " \t min_OoEminusOoP_trig_:" << min_OoEminusOoP_trig_ << "\n";
  std::cout << " \t max_jetBtagCSV_:" << max_jetBtagCSV_ << "\n";
  std::cout << " \t apply_conversionVeto_:" << apply_conversionVeto_ << "\n";
  std::cout << " \t max_nLostHits_:" << max_nLostHits_ << "\n";
  std::cout << " \t invert_conversionVeto_:" << invert_conversionVeto_ << "\n";
  std::cout << " \t invert_nLostHits_:" << invert_nLostHits_ << "\n";
  std::cout << " \t min_nLostHits_fornLostHitsInversion_:" << min_nLostHits_fornLostHitsInversion_ << "\n";
}

bool
RecoElectronSelectorTight::operator()(const RecoElectron & electron) const
{
  if(debug_)
  {
    std::cout << get_human_line(this, __func__) << ":\n" << electron;
  }

  if(electron.cone_pt() < min_cone_pt_)
  {
    if(debug_)
    {
      std::cout << "FAILS cone pT = " << electron.cone_pt() << " >= " << min_cone_pt_ << " tight cut\n";
    }
    return false;
  }
  if(electron.pt() < min_lepton_pt_)
  {
    if(debug_)
    {
      std::cout << "FAILS pT = " << electron.pt() << " >= " << min_lepton_pt_ << " tight cut\n";
    }
    return false;
  }
  if(electron.absEta() > max_absEta_)
  {
    if(debug_)
    {
      std::cout << "FAILS abs(eta) = " << electron.absEta() << " <= " << max_absEta_ << " tight cut\n";
    }
    return false;
  }
  if(std::fabs(electron.dxy()) > max_dxy_)
  {
    if(debug_)
    {
      std::cout << "FAILS abs(dxy) = " << std::fabs(electron.dxy()) << " <= " << max_dxy_ << " tight cut\n";
    }
    return false;
  }
  if(std::fabs(electron.dz()) > max_dz_)
  {
    if(debug_)
    {
      std::cout << "FAILS max(dz) = " << std::fabs(electron.dz()) << " <= " << max_dz_ << " tight cut\n";
    }
    return false;
  }
  if(electron.relIso() > max_relIso_)
  {
    if(debug_)
    {
      std::cout << "FAILS relIso = " << electron.relIso() << " <= " << max_relIso_ << " tight cut\n";
    }
    return false;
  }
  if(electron.sip3d() > max_sip3d_)
  {
    if(debug_)
    {
      std::cout << "FAILS sip3d = " << electron.sip3d() << " <= " << max_sip3d_ << " tight cut\n";
    }
    return false;
  }
  if(electron.jetBtagCSV() > max_jetBtagCSV_)
  {
    if(debug_)
    {
      std::cout << "FAILS jetBtagCSV = " << electron.jetBtagCSV() << " <= " << max_jetBtagCSV_ << " tight cut\n";
    }
    return false;
  }
  if ( ! invert_nLostHits_ )
  {
    if(electron.nLostHits() > max_nLostHits_)
    {
      if(debug_)
      {
	std::cout << "FAILS nLostHits = " << electron.nLostHits() << " <= " << max_nLostHits_ << " loose cut\n";
      }
      return false;
    }
  }
  else
  {  // for conversion bkg CR
    if(electron.nLostHits() < min_nLostHits_fornLostHitsInversion_) 
    {
      if(debug_)
      {
	std::cout << "FAILS nLostHits = " << electron.nLostHits() << " >= " << min_nLostHits_fornLostHitsInversion_ << " (invert_nLostHits) loose cut\n";
      }
      return false;
    }
  }
  if(apply_conversionVeto_ && ! electron.passesConversionVeto())
  {
    if(debug_)
    {
      std::cout << "FAILS conversion veto tight cut\n";
    }
    return false;
  }
  if(invert_conversionVeto_ && electron.passesConversionVeto()) // for conversion bkg CR
  {
    if(debug_)
    {
      std::cout << "PASSES conversion veto tight cut while running on invert_conversionVeto mode\n";
    }
    return false;
  }
  if(electron.mvaRawTTH() < electron.mvaRawTTH_cut())
  {
    if(debug_)
    {
      std::cout << "FAILS mvaTTH = " << electron.mvaRawTTH() << " >= " << electron.mvaRawTTH_cut() << " tight cut\n";
    }
    return false;
  }
  if(! electron.mvaID_POG(EGammaWP::WPL))
  {
    if(debug_)
    {
      std::cout << "FAILS loose EGamma POG MVA tight cut\n";
    }
    return false;
  }

  // extra cuts for electrons passing pT threshold of single electron trigger,
  // as explained in section 3.3.4 of AN-2015/321
  if(apply_offline_e_trigger_cuts_)
  {
    const double max_sigmaEtaEta_trig = min_sigmaEtaEta_trig_ +
                                        max_sigmaEtaEta_trig_ * (electron.absEtaSC() > binning_absEta_);
    if(electron.sigmaEtaEta() > max_sigmaEtaEta_trig)
    {
      if(debug_)
      {
        std::cout << "FAILS sigmaEtaEta = " << electron.sigmaEtaEta() << " <= " << max_sigmaEtaEta_trig << " tight cut\n";
      }
      return false;
    }
    if(electron.HoE() > max_HoE_trig_)
    {
      if(debug_)
      {
        std::cout << "FAILS HoE = " << electron.HoE() << " <= " << max_HoE_trig_ << " tight cut\n";
      }
      return false;
    }
    if(electron.OoEminusOoP() < min_OoEminusOoP_trig_)
    {
      if(debug_)
      {
        std::cout << "FAILS OoEminusOoP = " << electron.OoEminusOoP() << " >= " << min_OoEminusOoP_trig_ << " tight cut\n";
      }
      return false;
    }
  }

  // electron passes all cuts
  if(set_selection_flags_)
  {
    electron.set_isTight();
  }
  return true;
}

RecoElectronCollectionSelectorTight::RecoElectronCollectionSelectorTight(Era era,
                                                                         int index,
                                                                         bool debug,
                                                                         bool set_selection_flags)
  : ParticleCollectionSelector<RecoElectron, RecoElectronSelectorTight>(era, index, debug)
{
  selector_.set_selection_flags(set_selection_flags);
}

void
RecoElectronCollectionSelectorTight::enable_offline_e_trigger_cuts()
{
  selector_.enable_offline_e_trigger_cuts();
}

void
RecoElectronCollectionSelectorTight::disable_offline_e_trigger_cuts()
{
  selector_.disable_offline_e_trigger_cuts();
}

// enable/disable photon conversion veto
void
RecoElectronCollectionSelectorTight::enable_conversionVeto()
{
  selector_.enable_conversionVeto();
}

void
RecoElectronCollectionSelectorTight::disable_conversionVeto()
{
  selector_.disable_conversionVeto();
}

void
RecoElectronCollectionSelectorTight::invert_max_nLostHits(Int_t min_nLostHits_fornLostHitsInversion)
{
  selector_.invert_max_nLostHits(min_nLostHits_fornLostHitsInversion);
}

void
RecoElectronCollectionSelectorTight::invert_conversionVeto()
{
  selector_.invert_conversionVeto();
}

void
RecoElectronCollectionSelectorTight::print_selection_conditions()
{
  selector_.print_selection_conditions();
}
