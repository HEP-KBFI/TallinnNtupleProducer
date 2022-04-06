#include "TallinnNtupleProducer/Selectors/interface/RecoMuonCollectionSelectorCMSPOG.h"

#include "TallinnNtupleProducer/CommonTools/interface/cmsException.h" // get_human_line()
#include "TallinnNtupleProducer/CommonTools/interface/Era.h"          // Era

RecoMuonSelectorCMSPOG::RecoMuonSelectorCMSPOG(Era era,
                                               int index,
                                               bool debug,
                                               bool set_selection_flags)
  : era_(era)
  , debug_(debug)
  , set_selection_flags_(set_selection_flags)
  , min_pt_(15.)
  , max_absEta_(2.4)
  , max_PFrelIso_(0.15)
{}

void
RecoMuonSelectorCMSPOG::set_min_pt(double min_pt)
{
  min_pt_ = min_pt;
}

void
RecoMuonSelectorCMSPOG::set_max_absEta(double max_absEta)
{
  max_absEta_ = max_absEta;
}

double
RecoMuonSelectorCMSPOG::get_min_pt() const
{
  return min_pt_;
}

double
RecoMuonSelectorCMSPOG::get_max_absEta() const
{
  return max_absEta_;
}

bool
RecoMuonSelectorCMSPOG::operator()(const RecoMuon & muon) const
{
  if(debug_)
  {
    std::cout << get_human_line(this, __func__) << ":\n" << muon;
  }

  if(muon.pt() < min_pt_)
  {
    if(debug_)
    {
      std::cout << "FAILS pT = " << muon.pt() << " >= " << min_pt_ << " CMS POG cut\n";
    }
    return false;
  }

  if(muon.absEta() > max_absEta_)
  {
    if(debug_)
    {
      std::cout << "FAILS abs(eta) = " << muon.absEta() << " <= " << max_absEta_ << " CMS POG cut\n";
    }
    return false;
  }

  if(muon.relIso() > max_PFrelIso_)
  {
    if(debug_)
    {
      std::cout << "FAILS PFrelIso = " << muon.relIso() << " <= " << max_PFrelIso_ << " CMS POG cut\n";
    }
    return false;
  }

  if(set_selection_flags_)
  {
    muon.set_isCMSPOG();
  }

  return true;
}
