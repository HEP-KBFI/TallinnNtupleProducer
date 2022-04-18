#include "TallinnNtupleProducer/EvtWeightTools/interface/LHEVpt_LOtoNLO.h"

#include "TallinnNtupleProducer/CommonTools/interface/as_integer.h"           // as_integer()
#include "TallinnNtupleProducer/CommonTools/interface/cmsException.h"         // cmsException()
#include "TallinnNtupleProducer/CommonTools/interface/sysUncertOptions.h"     // LHEVptSys::
#include "TallinnNtupleProducer/Readers/interface/BranchAddressInitializer.h" // BranchAddressInitializer

inline constexpr unsigned char
operator "" _uchar(unsigned long long arg) noexcept
{
  return static_cast<UChar_t>(arg);
}

LHEVpt_LOtoNLO::IdParams::IdParams()
  : LHEVpt_LOtoNLO::IdParams::IdParams(0, 0, 1, 0)
{}

LHEVpt_LOtoNLO::IdParams::IdParams(double slope,
                                   double slope_err,
                                   double intercept,
                                   double intercept_err)
  : slope_(slope)
  , slope_err_(slope_err)
  , intercept_(intercept)
  , intercept_err_(intercept_err)
{}

double
LHEVpt_LOtoNLO::IdParams::nominal(double pt) const
{
  return intercept_ - slope_ * pt * 1e-3;
}

double
LHEVpt_LOtoNLO::IdParams::up(double pt) const
{
  return (intercept_ + intercept_err_) - (slope_ - slope_err_) * pt / 1e3;
}

double
LHEVpt_LOtoNLO::IdParams::down(double pt) const
{
  return (intercept_ - intercept_err_) - (slope_ + slope_err_) * pt / 1e3;
}

std::map<UChar_t, LHEVpt_LOtoNLO::IdParams> LHEVpt_LOtoNLO::wjets_ = {
  { 0, { 1.339, 0.020, 1.628, 0.005 } },
  { 1, { 1.531, 0.112, 1.586, 0.027 } },
  { 2, { 0.925, 0.203, 1.440, 0.048 } },
};

std::map<UChar_t, LHEVpt_LOtoNLO::IdParams> LHEVpt_LOtoNLO::dy_ = {
  { 0, { 1.707, 0.020, 1.650, 0.002 } },
  { 1, { 1.485, 0.080, 1.534, 0.010 } },
  { 2, { 1.916, 0.140, 1.519, 0.019 } },
};

LHEVpt_LOtoNLO::LHEVpt_LOtoNLO(const AnalysisConfig & config,
                               bool isDEBUG)
  : ReaderBase(edm::ParameterSet())
  , branchName_lhe_nb_("LHE_Nb")
  , branchName_lhe_vpt_("LHE_Vpt")
  , isDEBUG_(isDEBUG)
{
  if(config.isMC_DY())
  {
    coefs_ = dy_;
  }
  else if(config.isMC_WJets())
  {
    coefs_ = wjets_;
  }
  else
  {
    throw cmsException(this, __func__, __LINE__) << "Corrections invoked for invalid sample";
  }
}

LHEVpt_LOtoNLO::~LHEVpt_LOtoNLO()
{}

std::vector<std::string>
LHEVpt_LOtoNLO::setBranchAddresses(TTree * tree)
{
  BranchAddressInitializer bai(tree);
  bai.setBranchAddress(lhe_nb_, branchName_lhe_nb_);
  bai.setBranchAddress(lhe_vpt_, branchName_lhe_vpt_);
  return bai.getBoundBranchNames();
}

double
LHEVpt_LOtoNLO::getWeight(LHEVptSys option) const
{
  const UChar_t nb = std::max(std::min(lhe_nb_, 2_uchar), 0_uchar);
  const LHEVpt_LOtoNLO::IdParams & params = coefs_.at(nb);
  const double sf = [&](double pt) -> double {
    switch(option)
    {
      case LHEVptSys::up:      return params.up(pt);
      case LHEVptSys::down:    return params.down(pt);
      case LHEVptSys::central: return params.nominal(pt);
    }
    assert(0);
  }(lhe_vpt_);

  if(isDEBUG_)
  {
    std::cout
      << get_human_line(this, __func__, __LINE__)
      << "LHE Nb = " << static_cast<int>(lhe_nb_) << ", "
         "Vpt = " << lhe_vpt_ << ", "
         "shift = " << as_integer(option) << " => "
         "SF = " << sf
      << '\n'
    ;
  }
  return std::max(sf, 1e-2);
}
