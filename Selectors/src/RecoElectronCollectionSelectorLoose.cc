#include "TallinnNtupleProducer/Selectors/interface/RecoElectronCollectionSelectorLoose.h"

#include "TallinnNtupleProducer/CommonTools/interface/electronDefinitions.h" // EGammaWP
#include "TallinnNtupleProducer/CommonTools/interface/cmsException.h"        // get_human_line()
#include "TallinnNtupleProducer/CommonTools/interface/Era.h"                 // Era

RecoElectronSelectorLoose::RecoElectronSelectorLoose(Era era,
                                                     int index,
                                                     bool debug,
                                                     bool set_selection_flags)
  : set_selection_flags_(set_selection_flags)
  , debug_(debug)
  , era_(era)
  , min_pt_(7.)
  , max_absEta_(2.5)
  , max_dxy_(0.05)
  , max_dz_(0.1)
  , max_relIso_(0.4)
  , max_sip3d_(8.)
  , apply_tightCharge_(false)
  , apply_conversionVeto_(false)
  , max_nLostHits_(1)
  , invert_nLostHits_(false)
  , min_nLostHits_fornLostHitsInversion_(1)
{}

void
RecoElectronSelectorLoose::set_selection_flags(bool selection_flag)
{
  set_selection_flags_ = selection_flag;
}

void
RecoElectronSelectorLoose::set_min_pt(double min_pt)
{
  min_pt_ = min_pt;
}

void
RecoElectronSelectorLoose::set_max_absEta(double max_absEta)
{
  max_absEta_ = max_absEta;
}

void
RecoElectronSelectorLoose::invert_max_nLostHits(Int_t min_nLostHits_fornLostHitsInversion)
{
  invert_nLostHits_ = true;
  min_nLostHits_fornLostHitsInversion_ = min_nLostHits_fornLostHitsInversion;
  std::cout << "RecoElectronSelectorLoose::invert_max_nLostHits():: invert_nLostHits_ with min_nLostHits " << min_nLostHits_fornLostHitsInversion_ << ".\n";
}

double
RecoElectronSelectorLoose::get_min_pt() const
{
  return min_pt_;
}

double
RecoElectronSelectorLoose::get_max_absEta() const
{
  return max_absEta_;
}

void
RecoElectronSelectorLoose::print_selection_conditions()
{
  std::cout << "RecoElectronSelectorLoose::print_selection_conditions():: " << "\n";
  std::cout << " \t set_selection_flags_:" << set_selection_flags_ << "\n";
  std::cout << " \t min_pt_:" << min_pt_ << "\n";
  std::cout << " \t max_absEta_:" << max_absEta_ << "\n";
  std::cout << " \t max_dxy_:" << max_dxy_ << "\n";
  std::cout << " \t max_dz_:" << max_dz_ << "\n";  
  std::cout << " \t max_relIso_:" << max_relIso_ << "\n";
  std::cout << " \t max_sip3d_:" << max_sip3d_ << "\n";
  std::cout << " \t apply_tightCharge_:" << apply_tightCharge_ << "\n";
  std::cout << " \t apply_conversionVeto_:" << apply_conversionVeto_ << "\n";
  std::cout << " \t max_nLostHits_:" << max_nLostHits_ << "\n";
  std::cout << " \t invert_nLostHits_:" << invert_nLostHits_ << "\n";
  std::cout << " \t min_nLostHits_fornLostHitsInversion_:" << min_nLostHits_fornLostHitsInversion_ << "\n";
}


bool
RecoElectronSelectorLoose::operator()(const RecoElectron & electron) const
{
  if(debug_)
  {
    std::cout << get_human_line(this, __func__) << ":\n" << electron;
  }

  if(electron.pt() < min_pt_)
  {
    if(debug_)
    {
      std::cout << "FAILS pT = " << electron.pt() << " >= " << min_pt_ << " loose cut\n";
    }
    return false;
  }
  if(electron.absEta() > max_absEta_)
  {
    if(debug_)
    {
      std::cout << "FAILS abs(eta) = " << electron.absEta() << " <= " << max_absEta_ << " loose cut\n";
    }
    return false;
  }
  if(std::fabs(electron.dxy()) > max_dxy_)
  {
    if(debug_)
    {
      std::cout << "FAILS abs(dxy) = " << std::fabs(electron.dxy()) << " <= " << max_dxy_ << " loose cut\n";
    }
    return false;
  }
  if(std::fabs(electron.dz()) > max_dz_)
  {
    if(debug_)
    {
      std::cout << "FAILS max(dz) = " << std::fabs(electron.dz()) << " <= " << max_dz_ << " loose cut\n";
    }
    return false;
  }
  if(electron.relIso() > max_relIso_)
  {
    if(debug_)
    {
      std::cout << "FAILS relIso = " << electron.relIso() << " <= " << max_relIso_ << " loose cut\n";
    }
    return false;
  }
  if(electron.sip3d() > max_sip3d_)
  {
    if(debug_)
    {
      std::cout << "FAILS sip3d = " << electron.sip3d() << " <= " << max_sip3d_ << " loose cut\n";
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
      std::cout << "FAILS conversion veto loose cut\n";
    }
    return false;
  }
  if(apply_tightCharge_ && electron.tightCharge() < 2)
  {
    if(debug_)
    {
      std::cout << "FAILS tight charge loose cut\n";
    }
    return false;
  }

  if(! electron.mvaID_POG(EGammaWP::WPL))
  {
    if(debug_)
    {
      std::cout << "FAILS loose EGamma POG MVA loose cut\n";
    }
    return false;
  }

  if(set_selection_flags_)
  {
    electron.set_isLoose();
  }

  return true;
}



RecoElectronCollectionSelectorLoose::RecoElectronCollectionSelectorLoose(Era era,
									 int index,
									 bool debug,
									 bool set_selection_flags)
  : ParticleCollectionSelector<RecoElectron, RecoElectronSelectorLoose>(era, index, debug)
{
  selector_.set_selection_flags(set_selection_flags);
}

void
RecoElectronCollectionSelectorLoose::invert_max_nLostHits(Int_t min_nLostHits_fornLostHitsInversion)
{
  selector_.invert_max_nLostHits(min_nLostHits_fornLostHitsInversion);
}

void
RecoElectronCollectionSelectorLoose::print_selection_conditions()
{
  selector_.print_selection_conditions();
}
