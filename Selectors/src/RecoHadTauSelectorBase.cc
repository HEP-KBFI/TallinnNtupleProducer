#include "TallinnNtupleProducer/Selectors/interface/RecoHadTauSelectorBase.h"

#include "FWCore/Utilities/interface/Parse.h"                              // edm::tokenize()

#include "TallinnNtupleProducer/CommonTools/interface/as_integer.h"        // as_integer()
#include "TallinnNtupleProducer/CommonTools/interface/cmsException.h"      // cmsException()
#include "TallinnNtupleProducer/CommonTools/interface/Era.h"               // Era
#include "TallinnNtupleProducer/CommonTools/interface/hadTauDefinitions.h" // get_tau_id_wp_int()

#include <boost/algorithm/string/join.hpp>                                 // boost::algorithm::join()

const std::map<TauID, std::string> RecoHadTauSelectorBase::nominal_fakeable_wp_ = {
  { TauID::MVAoldDM2017v2,     "VLoose"  },
  { TauID::MVAoldDMdR032017v2, "VLoose"  },
  { TauID::DeepTau2017v2VSjet, "VVLoose" },
};

RecoHadTauSelectorBase::RecoHadTauSelectorBase(Era era,
                                               int index,
                                               bool debug,
                                               bool set_selection_flags)
  : set_selection_flags_(set_selection_flags)
  , debug_(debug)
  , min_pt_(20.)
  , max_absEta_(2.3)
  , max_dxy_(1000.)
  , max_dz_(0.2)
  , apply_decayModeFinding_(true)
  , min_antiElectron_(DEFAULT_TAUID_ID_VALUE)
  , min_antiMuon_(DEFAULT_TAUID_ID_VALUE)
  , apply_deeptau_lepton_(false)
  , disable_deeptau_lepton_(false)
{
  reset();
}

void
RecoHadTauSelectorBase::set_min_pt(double min_pt)
{
  min_pt_ = min_pt;
}

void
RecoHadTauSelectorBase::set_max_absEta(double max_absEta)
{
  max_absEta_ = max_absEta;
}

double
RecoHadTauSelectorBase::get_min_pt() const
{
  return min_pt_;
}

double
RecoHadTauSelectorBase::get_max_absEta() const
{
  return max_absEta_;
}

void
RecoHadTauSelectorBase::set_min_id_mva(TauID tauId,
                                       int min_id_mva)
{
  if(! min_id_mva_.count(tauId))
  {
    throw cmsException(this, __func__, __LINE__)
      << "Unrecognizable tau ID = " << as_integer(tauId)
    ;
  }
  min_id_mva_[tauId] = min_id_mva;
}

void
RecoHadTauSelectorBase::set_min_raw_mva(TauID tauId,
                                        double min_raw_mva)
{
  if(! min_raw_mva_.count(tauId))
  {
    throw cmsException(this, __func__, __LINE__)
      << "Unrecognizable tau ID = " << as_integer(tauId)
    ;
  }
  min_raw_mva_[tauId] = min_raw_mva;
}

void
RecoHadTauSelectorBase::set_min_antiElectron(int min_antiElectron)
{
  min_antiElectron_ = min_antiElectron;
}

void
RecoHadTauSelectorBase::set_min_antiMuon(int min_antiMuon)
{
  min_antiMuon_ = min_antiMuon;
}

int
RecoHadTauSelectorBase::get_min_antiElectron() const
{
  return min_antiElectron_;
}

int
RecoHadTauSelectorBase::get_min_antiMuon() const
{
  return min_antiMuon_;
}

bool
RecoHadTauSelectorBase::get_deeptau_lepton() const
{
  return apply_deeptau_lepton_;
}

void
RecoHadTauSelectorBase::disable_deeptau_lepton()
{
  disable_deeptau_lepton_ = true;
}

void
RecoHadTauSelectorBase::set(const std::string & cut)
{
  if(cut.empty())
  {
    throw cmsException(this, __func__, __LINE__) << "Empty cut supplied";
  }

  apply_deeptau_lepton_ = false;
  const std::vector<std::string> cut_parts = edm::tokenize(cut, TAU_WP_SEPARATOR);
  const std::size_t nof_cut_parts = cut_parts.size();
  assert(nof_cut_parts);
  reset();
  for(const std::string & cut_part: cut_parts)
  {
    const std::string mva_type = cut_part.substr(0, 7);
    if(! TauID_PyMap.count(mva_type))
    {
      throw cmsException(this, __func__, __LINE__)
        << "Unrecognizable tau discriminator '" << mva_type << "' found in string: " << cut
      ;
    }
    const TauID tauId = TauID_PyMap.at(mva_type);
    const std::string wp = cut_part.substr(7);
    const int wp_int = get_tau_id_wp_int(tauId, wp);
    set_min_id_mva(tauId, wp_int);
    if(tauId == TauID::DeepTau2017v2VSe ||
       tauId == TauID::DeepTau2017v2VSmu ||
       tauId == TauID::DeepTau2017v2VSjet)
    {
      apply_decayModeFinding_ = false;
      decayMode_blacklist_ = { 5, 6 }; // exclude DMs 5 & 6
    }
    // Apply anti-e and anti-mu DeepTauID discriminators if cut on anti-jet DeepTauID is requested
    apply_deeptau_lepton_ |= tauId == TauID::DeepTau2017v2VSjet;
  }

  apply_deeptau_lepton_ &= nof_cut_parts == 1 && ! disable_deeptau_lepton_;
  if(nof_cut_parts > 1)
  {
    // Reset DM whitelist and blacklist if tau is required to pass an OR of multiple tau IDs
    // This is relevant only in Ntuple post-production and MEM workflows
    decayMode_whitelist_.clear();
    decayMode_blacklist_.clear();
  }

  if(apply_deeptau_lepton_)
  {
    // If the DeepTau ID discriminator is the only tau ID we're cutting on, then we should also cut on the loosest
    // WP of antt-e and anti-mu tau ID discriminators; if there are more tau IDs than just DeepTau ID, then we should
    // avoid cutting on these extra discriminators.
    std::cout << "Applying additional loosest anti-e and anti-mu DeepTau discriminator cuts\n";
  }

  cut_ = cut;
}

const std::string &
RecoHadTauSelectorBase::get() const
{
  return cut_;
}

bool
RecoHadTauSelectorBase::set_if_looser(const std::string & cut)
{
  if(cut.empty())
  {
    return false;
  }
  std::cout << "Attempting to replace cut from '" << cut_ << "' to '" << cut << "'\n";

  // 0) if the old cut is an OR of multiple WPs, then throw an error
  // 1) if the new cut is an OR of multiple WPs, then override the old WP with the new WPs (use case: Ntuple production)
  // 2) if the MVA IDs of old and new cut are the same (default use case):
  //   i) if the MVA WP from the new cut is strictly looser than the old WP, then replace it w/ the new WP (use case: BDT training)
  //   ii) if the MVA WP from the new cut is not strictly looser than the old WP, then keep the cut as is (default use case)
  // 3) if the MVA IDs of old and new cut are different (use case: switching from MVAv2 to DeepTau ID):
  //   i) if the new cut is strictly looser than the nominal fakeable WP of the new tau ID, then override the old WP (use case: BDT training)
  //   ii) if the new cut is the same or tighter than the nominal fakeable WP of the new tau ID, then override the old WP
  //       with the nominal fakeable WP of the new tau ID (default use case)

  const std::vector<std::string> cut_parts_old = edm::tokenize(cut_, TAU_WP_SEPARATOR);
  if(cut_parts_old.size() != 1)
  {
    // case 0:
    throw cmsException(this, __func__, __LINE__)
        << "Can overwrite a single tau ID WP but got more of them: " << boost::algorithm::join(cut_parts_old, ", ")
     ;
  }
  const std::string mva_old = cut_.substr(0, 7);
  const TauID tauId_old = TauID_PyMap.at(mva_old);
  const std::string wp_old  = cut_.substr(7);
  const int wp_int_old = get_tau_id_wp_int(tauId_old, wp_old);

  std::string cut_new = "";
  const std::vector<std::string> cut_parts = edm::tokenize(cut, TAU_WP_SEPARATOR);
  if(cut_parts.size() > 1)
  {
    // case 1:
    cut_new = cut;
  }
  else if(cut_parts.size() == 1)
  {
    const std::string mva_new = cut.substr(0, 7);
    const TauID tauId_new = TauID_PyMap.at(mva_new);
    const std::string wp_new  = cut.substr(7);
    const int wp_int_new = get_tau_id_wp_int(tauId_new, wp_new);

    if(tauId_new == tauId_old)
    {
      // case 2
      if(wp_int_new < wp_int_old)
      {
        // case 2.i
        cut_new = mva_old + wp_new;
        std::cout << "Relaxing old cut for '" << mva_new << "' from '" << wp_old << "' to '" << wp_new << "'\n";
      }
      else
      {
        // case 2.ii
        cut_new = mva_old + wp_old;
        std::cout << "Keeping old cut for '" << mva_new << "' at '" << wp_old << "'\n";
      }
    }
    else
    {
      // case 3
      if(! nominal_fakeable_wp_.count(tauId_new))
      {
        throw cmsException(this, __func__, __LINE__)
          << "Cannot change tau ID WP because fakeable nominal WP missing for tau ID: " << mva_new
        ;
      }
      std::cout << "Changing from '" << mva_new << "' to '" << mva_old << "'\n";
      const std::string new_nominal_fakeable_wp = nominal_fakeable_wp_.at(tauId_new);
      const int new_nominal_fakeable_wp_int = get_tau_id_wp_int(tauId_new, new_nominal_fakeable_wp);
      if(new_nominal_fakeable_wp_int < wp_int_new)
      {
        // case 3.i
        std::cout << "Relaxing the nominal fakeable WP of '" << mva_new << "' from '" << wp_new << "' to '" << new_nominal_fakeable_wp << "'\n";

        cut_new = mva_new + new_nominal_fakeable_wp;
      }
      else
      {
        // case 3.ii
        std::cout << "Keeping the nominal fakeable WP of '" << mva_new << "' at '" << new_nominal_fakeable_wp << "'\n";
        cut_new = mva_new + wp_new;
      }
    }
  }
  else
  {
    throw cmsException(this, __func__, __LINE__) << "Empty cut supplied";
  }
  assert(! cut_new.empty());

  if(cut_ != cut_new)
  {
    std::cout << "Changed tau ID WP from '" << cut_ << "' to '" << cut_new << "'\n";
    set(cut_new);
    return true;
  }

  return false;
}

bool
RecoHadTauSelectorBase::operator()(const RecoHadTau & hadTau) const
{
  assert(! cut_.empty());

  if(hadTau.pt() < min_pt_)
  {
    if(debug_)
    {
      std::cout << "FAILS pT = " << hadTau.pt() << " >= " << min_pt_ << " cut\n";
    }
    return false;
  }
  if(hadTau.absEta() > max_absEta_)
  {
    if(debug_)
    {
      std::cout << "FAILS abs(eta) = " << hadTau.absEta() << " <= " << max_absEta_ << " cut\n";
    }
    return false;
  }
  if(std::fabs(hadTau.dz()) > max_dz_)
  {
    if(debug_)
    {
      std::cout << "FAILS abs(dz) = " << std::fabs(hadTau.dz()) << " <= " << max_dz_ << " cut\n";
    }
    return false;
  }
  if(apply_decayModeFinding_ && ! hadTau.idDecayMode())
  {
    if(debug_)
    {
      std::cout << "FAILS old decayModeFinding cut\n";
    }
    return false;
  }
  if(std::find(decayMode_blacklist_.begin(), decayMode_blacklist_.end(), hadTau.decayMode()) != decayMode_blacklist_.end())
  {
    if(debug_)
    {
      std::cout << "FAILS decay mode cut DM (=" << hadTau.decayMode() << ") in ";
      for(unsigned decayModeIdx = 0; decayModeIdx < decayMode_blacklist_.size(); ++decayModeIdx)
      {
        std::cout << decayMode_blacklist_.at(decayModeIdx);
        if(decayModeIdx < decayMode_blacklist_.size() - 1)
        {
          std::cout << ", ";
        }
        else
        {
          std::cout << '\n';
        }
      }
    }
    return false;
  }
  if(! decayMode_whitelist_.empty() &&
     std::find(decayMode_whitelist_.begin(), decayMode_whitelist_.end(), hadTau.decayMode()) == decayMode_whitelist_.end())
  {
    if(debug_)
    {
      std::cout << "FAILS decay mode cut DM (=" << hadTau.decayMode() << ") NOT in ";
      for(unsigned decayModeIdx = 0; decayModeIdx < decayMode_whitelist_.size(); ++decayModeIdx)
      {
        std::cout << decayMode_whitelist_.at(decayModeIdx);
        if(decayModeIdx < decayMode_whitelist_.size() - 1)
        {
          std::cout << ", ";
        }
        else
        {
          std::cout << '\n';
        }
      }
    }
    return false;
  }

  std::vector<TauID> min_id_mva_cuts;
  for(const auto & kv: min_id_mva_)
  {
    if(kv.second > DEFAULT_TAUID_ID_VALUE)
    {
      min_id_mva_cuts.push_back(kv.first);
    }
  }
  std::vector<TauID> min_raw_mva_cuts;
  for(const auto & kv: min_raw_mva_)
  {
    if(kv.second > DEFAULT_TAUID_RAW_VALUE)
    {
      min_raw_mva_cuts.push_back(kv.first);
    }
  }
  assert(! min_id_mva_cuts.empty() || ! min_raw_mva_cuts.empty());

  if(! min_id_mva_cuts.empty() &&
     std::none_of(min_id_mva_cuts.begin(), min_id_mva_cuts.end(),
       [this, &hadTau](TauID tauId) -> bool
        {
          return hadTau.id_mva(tauId) >= min_id_mva_.at(tauId);
        }
      )
    )
  {
    if(debug_)
    {
      std::cout << "FAILS id_mva cuts: ";
      for(std::size_t min_id_mva_idx = 0; min_id_mva_idx < min_id_mva_cuts.size(); ++min_id_mva_idx)
      {
        const TauID tauId = min_id_mva_cuts[min_id_mva_idx];
        std::cout << "id" << TauID_names.at(tauId) << " (= " << hadTau.id_mva(tauId) << ") >= " << min_id_mva_.at(tauId);
        if(min_id_mva_idx < min_id_mva_cuts.size() - 1)
        {
          std::cout << " OR ";
        }
        else
        {
          std::cout << '\n';
        }
      }
    }
    return false;
  }

  if(! min_raw_mva_cuts.empty() &&
     std::none_of(min_raw_mva_cuts.begin(), min_raw_mva_cuts.end(),
       [this, &hadTau](TauID tauId) -> bool
        {
          return hadTau.raw_mva(tauId) >= min_raw_mva_.at(tauId);
        }
      )
    )
  {
    if(debug_)
    {
      std::cout << "FAILS raw_mva cuts: ";
      for(std::size_t min_raw_mva_idx = 0; min_raw_mva_idx < min_raw_mva_cuts.size(); ++min_raw_mva_idx)
      {
        const TauID tauId = min_raw_mva_cuts[min_raw_mva_idx];
        std::cout << "raw" << TauID_names.at(tauId) << " (= " << hadTau.raw_mva(tauId) << ") >= " << min_raw_mva_.at(tauId);
        if(min_raw_mva_idx < min_raw_mva_cuts.size() - 1)
        {
          std::cout << " OR ";
        }
        else
        {
          std::cout << '\n';
        }
      }
    }
    return false;
  }

  if(apply_deeptau_lepton_)
  {
    if(hadTau.id_mva(TauID::DeepTau2017v2VSmu) < 1) // corresponds to cut on VLoose WP
    {
      if(debug_)
      {
        std::cout << "FAILS DeepTau anti-mu MVA cut (" << hadTau.id_mva(TauID::DeepTau2017v2VSmu) << ")\n";
      }
      return false;
    }
    if(hadTau.id_mva(TauID::DeepTau2017v2VSe) < 1) // corresponds to cut on VVVLoose WP
    {
      if(debug_)
      {
        std::cout << "FAILS DeepTau anti-e MVA cut (" << hadTau.id_mva(TauID::DeepTau2017v2VSe) << ")\n";
      }
      return false;
    }
  }

  if(hadTau.antiElectron() < min_antiElectron_)
  {
    if(debug_)
    {
      std::cout << "FAILS antiElectron = " << hadTau.antiElectron() << " >= " << min_antiElectron_ << " cut\n";
    }
    return false;
  }
  if(hadTau.antiMuon() < min_antiMuon_)
  {
    if(debug_)
    {
      std::cout << "FAILS antiMuon = " << hadTau.antiMuon() << " >= " << min_antiMuon_ << "cut\n";
    }
    return false;
  }

  // hadTau passes all cuts
  if(set_selection_flags_)
  {
    set_selection_flags(hadTau);
  }
  return true;
}

void
RecoHadTauSelectorBase::reset()
{
  for(const auto & kv: TauID_levels)
  {
    min_id_mva_[kv.first] = DEFAULT_TAUID_ID_VALUE;
    min_raw_mva_[kv.first] = DEFAULT_TAUID_RAW_VALUE;
  }
}
