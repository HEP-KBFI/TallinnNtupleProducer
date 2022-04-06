#include "TallinnNtupleProducer/Selectors/interface/GenLeptonCollectionSelector.h"

#include "TallinnNtupleProducer/CommonTools/interface/cmsException.h" // get_human_line()

GenLeptonSelector::GenLeptonSelector(Era era,
                                     int index,
                                     bool debug)
  : era_(era)
  , min_pt_muon_(5.)
  , max_absEta_muon_(2.4)
  , min_pt_electron_(7.)
  , max_absEta_electron_(2.5)
  , debug_(debug)
{}

void
GenLeptonSelector::set_min_pt_muon(double min_pt)
{
  min_pt_muon_ = min_pt;
}

void
GenLeptonSelector::set_max_absEta_muon(double max_absEta)
{
  max_absEta_muon_ = max_absEta;
}

void
GenLeptonSelector::set_min_pt_electron(double min_pt)
{
  min_pt_electron_ = min_pt;
}

void
GenLeptonSelector::set_max_absEta_electron(double max_absEta)
{
  max_absEta_electron_ = max_absEta;
}

double
GenLeptonSelector::get_min_pt_muon() const
{
  return min_pt_muon_;
}

double
GenLeptonSelector::get_max_absEta_muon() const
{
  return max_absEta_muon_;
}

double
GenLeptonSelector::get_min_pt_electron() const
{
  return min_pt_electron_;
}

double
GenLeptonSelector::get_max_absEta_electron() const
{
  return max_absEta_electron_;
}

bool
GenLeptonSelector::operator()(const GenLepton & lepton) const
{
  if(debug_)
  {
    std::cout << get_human_line(this, __func__) << ":\n lepton: " << lepton << '\n';
  }

  double min_pt, max_absEta;
  if(lepton.is_muon())
  {
     min_pt = min_pt_muon_;
     max_absEta = max_absEta_muon_;
  } 
  else if(lepton.is_electron())
  {
     min_pt = min_pt_electron_;
     max_absEta = max_absEta_electron_;
  }
  else
  {
    assert(0);
  }

  if(lepton.pt() < min_pt)
  {
    if(debug_)
    {
      std::cout << "FAILS pT >= " << min_pt << " cut\n";
    }
    return false;
  }
  if(max_absEta > 0. && lepton.absEta() > max_absEta)
  {
    if(debug_)
    {
      std::cout << "FAILS abs(eta) <= " << max_absEta << " cut\n";
    }
    return false;
  }
  // lepton passes all cuts
  return true;
}
