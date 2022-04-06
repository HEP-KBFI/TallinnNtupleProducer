#include "TallinnNtupleProducer/Selectors/interface/RecoMuonCollectionSelectorLoose.h"

#include "TallinnNtupleProducer/CommonTools/interface/cmsException.h" // get_human_line()
#include "TallinnNtupleProducer/CommonTools/interface/Era.h"          // Era

RecoMuonSelectorLoose::RecoMuonSelectorLoose(Era era,
                                             int index,
                                             bool debug,
                                             bool set_selection_flags) 
  : era_(era)
  , debug_(debug)
  , set_selection_flags_(set_selection_flags)
  , min_pt_(5.)
  , max_absEta_(2.4)
  , max_dxy_(0.05)
  , max_dz_(0.1)
  , max_relIso_(0.4)
  , max_sip3d_(8.)
  , apply_looseIdPOG_(true)
  , apply_mediumIdPOG_(false)
  , apply_tightCharge_(false)
{}

void
RecoMuonSelectorLoose::set_min_pt(double min_pt)
{
  min_pt_ = min_pt;
}

void
RecoMuonSelectorLoose::set_max_absEta(double max_absEta)
{
  max_absEta_ = max_absEta;
}

double
RecoMuonSelectorLoose::get_min_pt() const
{
  return min_pt_;
}

double
RecoMuonSelectorLoose::get_max_absEta() const
{
  return max_absEta_;
}

bool
RecoMuonSelectorLoose::operator()(const RecoMuon & muon) const
{
  if(debug_)
  {
    std::cout << get_human_line(this, __func__) << ":\n" << muon;
  }

  if(muon.pt() < min_pt_)
  {
    if(debug_)
    {
      std::cout << "FAILS pT = " << muon.pt() << " >= " << min_pt_ << " loose cut\n";
    }
    return false;
  }

  if(muon.absEta() > max_absEta_)
  {
    if(debug_)
    {
      std::cout << "FAILS abs(eta) = " << muon.absEta() << " <= " << max_absEta_ << " loose cut\n";
    }
    return false;
  }

  if(std::fabs(muon.dxy()) > max_dxy_)
  {
    if(debug_)
    {
      std::cout << "FAILS abs(dxy) = " << std::fabs(muon.dxy()) << " <= " << max_dxy_ << " loose cut\n";
    }
    return false;
  }

  if(std::fabs(muon.dz()) > max_dz_)
  {
    if(debug_)
    {
      std::cout << "FAILS abs(dz) = " << std::fabs(muon.dz()) << " <= " << max_dz_ << " loose cut\n";
    }
    return false;
  }

  if(muon.relIso() > max_relIso_)
  {
    if(debug_)
    {
      std::cout << "FAILS relIso = " << muon.relIso() << " <= " << max_relIso_ << " loose cut\n";
    }
    return false;
  }

  if(muon.sip3d() > max_sip3d_)
  {
    if(debug_)
    {
      std::cout << "FAILS sip3d = " << muon.sip3d() << " <= " << max_sip3d_ << " loose cut\n";
    }
    return false;
  }

  if(! muon.passesLooseIdPOG() && apply_looseIdPOG_)
  {
    if(debug_)
    {
      std::cout << "FAILS loose POG loose cut\n";
    }
    return false;
  }

  if(! muon.passesMediumIdPOG() && apply_mediumIdPOG_)
  {
    if(debug_)
    {
      std::cout << "FAILS medium POG loose cut\n";
    }
    return false;
  }

  if(set_selection_flags_)
  {
    muon.set_isLoose();
  }

  return true;
}
