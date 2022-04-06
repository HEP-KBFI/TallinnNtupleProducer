#include "TallinnNtupleProducer/Selectors/interface/RecoElectronCollectionSelectorCMSPOG.h"

#include "TallinnNtupleProducer/CommonTools/interface/electronDefinitions.h" // EGammaID, EGammaWP
#include "TallinnNtupleProducer/CommonTools/interface/cmsException.h"        // get_human_line()
#include "TallinnNtupleProducer/CommonTools/interface/Era.h"                 // Era

RecoElectronSelectorCMSPOG::RecoElectronSelectorCMSPOG(Era era,
                                                       int index,
                                                       bool debug,
                                                       bool set_selection_flags)
  : set_selection_flags_(set_selection_flags)
  , debug_(debug)
  , era_(era)
  , min_pt_(15.)
  , max_absEta_(2.5)
{}

void
RecoElectronSelectorCMSPOG::set_min_pt(double min_pt)
{
  min_pt_ = min_pt;
}

void
RecoElectronSelectorCMSPOG::set_max_absEta(double max_absEta)
{
  max_absEta_ = max_absEta;
}

double
RecoElectronSelectorCMSPOG::get_min_pt() const
{
  return min_pt_;
}

double
RecoElectronSelectorCMSPOG::get_max_absEta() const
{
  return max_absEta_;
}

bool
RecoElectronSelectorCMSPOG::operator()(const RecoElectron & electron) const
{
  if(debug_)
  {
    std::cout << get_human_line(this, __func__) << ":\n" << electron;
  }

  if(electron.pt() < min_pt_)
  {
    if(debug_)
    {
      std::cout << "FAILS pT = " << electron.pt() << " >= " << min_pt_ << " CMS POG cut\n";
    }
    return false;
  }
  if(electron.absEta() > max_absEta_)
  {
    if(debug_)
    {
      std::cout << "FAILS abs(eta) = " << electron.absEta() << " <= " << max_absEta_ << " CMS POG cut\n";
    }
    return false;
  }

  if(! electron.mvaID_POG(EGammaID::Fall17V2Iso, EGammaWP::WP90))
  {
    if(debug_)
    {
      std::cout << "FAILS EGamma POG ISO MVA WP90 cut\n";
    }
    return false;
  }

  if(set_selection_flags_)
  {
    electron.set_isCMSPOG();
  }

  return true;
}
