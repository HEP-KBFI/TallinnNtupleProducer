#include "tthAnalysis/HiggsToTauTau/interface/TauESTool.h"

#include "tthAnalysis/HiggsToTauTau/interface/generalAuxFunctions.h"      // get_fullpath()
#include "tthAnalysis/HiggsToTauTau/interface/analysisAuxFunctions.h" // Era::k*

#include <TString.h> // Form()
#include <TFile.h>   // TFile
#include <TH1.h>     // TH1

TauESTool::TauESTool(Era era,
                     TauID tauID,
                     int central_or_shift,
                     bool debug)
  : era_(era)
  , tauID_(tauID)
  , central_or_shift_(central_or_shift)
  , debug_(debug)
  , lowPt_(34.)
  , highPt_(170.)
{
  load_sf();
}

void
TauESTool::set_central_or_shift(int central_or_shift)
{
  central_or_shift_ = central_or_shift;
}

void
TauESTool::set_tauID(TauID tauID)
{
  // we don't have to load new tau ES SF if the old and new tau IDs are MVA-based, since we use the same SF for both
  if(tauID != tauID_)
  {
    const bool load_new_sf = tauID == TauID::DeepTau2017v2VSjet || tauID_ == TauID::DeepTau2017v2VSjet;
    tauID_ = tauID;
    if(load_new_sf)
    {
      load_sf();
    }
  }
}

double
TauESTool::getTES(double pt,
                  int dm,
                  int genmatch) const
{
  if(genmatch != 5 || central_or_shift_ == kHadTauPt_uncorrected)
  {
    return 1.0;
  }
  if(! sf_lowPt_.count(dm) || ! sf_highPt_.count(dm))
  {
    throw cmsException(this, __func__, __LINE__) << "Invalid decay mode: " << dm;
  }

  // use low-pt SF for the whole pt range
  const double sf = sf_lowPt_.at(dm);
  const double sfErr_lowPt = sfErr_lowPt_.at(dm);
  const double sfErr_highPt = sfErr_highPt_.at(dm);

  double sfErr = 0.;
  if(pt >= highPt_)
  {
    sfErr = sfErr_highPt;
  }
  else if(pt >= lowPt_)
  {
    // interpolate
    sfErr = sfErr_lowPt + (sfErr_highPt - sfErr_lowPt) / (highPt_ - lowPt_) * (pt - lowPt_);
  }
  else
  {
    sfErr = sfErr_lowPt;
  }
  switch(central_or_shift_)
  {
    case kHadTauPt_central:   return sf;
    case kHadTauPt_shiftUp:   return sf + sfErr;
    case kHadTauPt_shiftDown: return sf - sfErr;
    default: throw cmsException(this, __func__, __LINE__) << "Invalid central_or_shift option: " << central_or_shift_;
  }
}

std::string
TauESTool::get_era_str() const
{
  switch(era_)
  {
    case Era::kUndefined: throw cmsException(this, __func__, __LINE__) << "Undefined era!";
    case Era::k2016: return "2016Legacy";
    case Era::k2017: return "2017ReReco";
    case Era::k2018: return "2018ReReco";
    default: throw cmsException(this, __func__, __LINE__) << "Invalid era: " << static_cast<int>(era_);
  }
}

std::string
TauESTool::get_tauId_str() const
{
  if(tauID_ == TauID::MVAoldDM2017v2 || tauID_ == TauID::MVAoldDMdR032017v2)
  {
    return "MVAoldDM2017v2";
  }
  else if(tauID_ == TauID::DeepTau2017v2VSjet)
  {
    return "DeepTau2017v2p1VSjet";
  }
  throw cmsException(this, __func__, __LINE__) << "Invalid tau ID: " << as_integer(tauID_);
}

void
TauESTool::load_sf()
{
  const std::string base_dir = "TauPOG/TauIDSFs/data";
  const std::string era_str = get_era_str();
  const std::string tauId_str = get_tauId_str();
  const std::string input_name_lowPt = Form(
    "%s/TauES_dm_%s_%s.root", base_dir.data(), tauId_str.data(), era_str.data()
  );
  const std::string input_name_highPt = Form(
    "%s/TauES_dm_%s_%s_ptgt100.root", base_dir.data(), tauId_str.data(), era_str.data()
  );
  load_sf(input_name_lowPt, sf_lowPt_, sfErr_lowPt_);
  load_sf(input_name_highPt, sf_highPt_, sfErr_highPt_);
}

void
TauESTool::load_sf(const std::string & input_name,
                   std::map<int, double> & sf,
                   std::map<int, double> & sfErr)
{
  const std::string input_name_full = get_fullpath(input_name);
  TFile * input = TFile::Open(input_name_full.data(), "read");
  if(! input || input -> IsZombie())
  {
    throw cmsException(this, __func__, __LINE__) << "Cannot open file: " << input_name;
  }

  const std::string hist_name = "tes";
  TH1 * tes = static_cast<TH1 *>(input->Get(hist_name.data()));
  if(! tes)
  {
    throw cmsException(this, __func__, __LINE__)
      << "Unable to load histogram '" << hist_name << "' from file" << input_name_full
    ;
  }

  const int ndm = tes->GetNbinsX();
  for(int dm_idx = 0; dm_idx < ndm; ++dm_idx)
  {
    const double tes_val = tes->GetBinContent(dm_idx + 1);
    const double tes_err = tes->GetBinError(dm_idx + 1);
    const bool is_valid = tes_val > 0. && tes_err > 0.;
    // the tau ES is measured only for DM 0, 1, 10, 11 => set tau ES and its uncertainty to 1 and 0, respectively,
    // for other decay modes
    sf[dm_idx] = is_valid ? tes_val : 1.;
    sfErr[dm_idx] = is_valid ? tes_err : 0.;
  }
  input->Close();

  if(debug_)
  {
    std::cout << "Loaded the following tau ES SF from file '" << input_name_full << "' for era " << static_cast<int>(era_) << ":\n";
    for(const auto & kv: sf)
    {
      std::cout << "  DM = " << kv.first << " -> SF = " << kv.second << " +/- " << sfErr[kv.first] << '\n';
    }
  }
}
