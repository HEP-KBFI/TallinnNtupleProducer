#include "TallinnNtupleProducer/CommonTools/interface/AnalysisConfig.h"

#include "TallinnNtupleProducer/CommonTools/interface/topPtRwgt.h" // TopPtRwgtOption::, get_topPtRwgtOpt()

#include <TPRegexp.h>                           // TPRegexp
#include <TObjString.h>                         // TObjString
#include <TObjArray.h>                          // TObjArray

#include <boost/algorithm/string/predicate.hpp> // boost::starts_with()

AnalysisConfig::AnalysisConfig(const std::string & analysis, const edm::ParameterSet & cfg)
  : analysis_string_(analysis)
  , process_string_(cfg.getParameter<std::string>("process"))
  , process_string_hh_(cfg.exists("process_hh") ? cfg.getParameter<std::string>("process_hh") : process_string_)
  , mass_HH_resonant_(-1.)
  , apply_topPtReweighting_(TopPtRwgtOption::Disable)
{
  assert(boost::starts_with(process_string_hh_, process_string_) || process_string_.find("signal") != std::string::npos || process_string_ == "HH");
  isMC_       = cfg.getParameter<bool>("isMC");
  isData_     = !isMC_;
  isMC_DY_    = process_string_ == "DY";
  isMC_WJets_ = process_string_ == "W";
  isMC_WZ_    = process_string_ == "WZ";
  isMC_ZZ_    = process_string_ == "ZZ" || process_string_ == "ggZZ" || process_string_ == "qqZZ";
  isMC_EWK_   = isMC_WZ_ || isMC_ZZ_;
  isMC_QCD_   = process_string_ == "QCD";
  isMC_tH_    = process_string_ == "tHq" || process_string_ == "tHW" || process_string_ == "TH";
  isMC_ttH_   = process_string_ == "ttH" || process_string_ == "ttH_ctcvcp" || process_string_ == "TTH";
  isMC_ggH_   = process_string_ == "ggH";
  isMC_qqH_   = process_string_ == "qqH";
  isMC_VH_    = process_string_ == "VH";
  isMC_WH_    = process_string_ == "WH";
  isMC_ZH_    = process_string_ == "ZH";
  isMC_H_     = isMC_tH_ || isMC_ttH_ || isMC_ggH_ || isMC_qqH_ || isMC_VH_ || isMC_WH_ || isMC_ZH_ ||
                process_string_ == "TTWH" || process_string_ == "TTZH";

  decayModes_H_ = { "hww", "hzz", "htt", "hzg", "hmm", "hbb" };

  TPRegexp parser_HH_resonant_spin0("signal_(ggf|vbf)_spin0_[0-9]+_[0-9a-zA-Z]*");
  isMC_HH_resonant_spin0_ = parser_HH_resonant_spin0.Match(process_string_.data());
  TPRegexp parser_HH_resonant_spin2("signal_(ggf|vbf)_spin2_[0-9]+_[0-9a-zA-Z]*");
  isMC_HH_resonant_spin2_ = parser_HH_resonant_spin2.Match(process_string_.data());
  assert(!(isMC_HH_resonant_spin0_ && isMC_HH_resonant_spin2_));
  isMC_HH_resonant_ = isMC_HH_resonant_spin0_ || isMC_HH_resonant_spin2_;
  if ( isMC_HH_resonant_ )
  {
    TPRegexp parser_mass_HH_resonant("signal_(ggf|vbf)_spin(0|2)_([0-9]+)_*");
    const TObjArray * const subStrings = parser_mass_HH_resonant.MatchS(process_string_.data());
    if ( subStrings->GetEntries() == 4 )
    {
      mass_HH_resonant_ = (static_cast<TObjString *>(subStrings->At(3)))->GetString().Atof();
    }
  }
  TPRegexp parser_HH_nonresonant("signal_(ggf|vbf)_nonresonant_[0-9a-zA-Z]*");
  isMC_HH_nonresonant_ = parser_HH_nonresonant.Match(process_string_.data());
  assert(!(isMC_HH_resonant_ && isMC_HH_nonresonant_));
  isMC_HH_  = isMC_HH_resonant_ || isMC_HH_nonresonant_ || process_string_ == "HH";
  isHH_rwgt_allowed_ = (
    boost::starts_with(process_string_, "signal_ggf_nonresonant_") && process_string_.find("cHHH") == std::string::npos
  ) || process_string_ == "HH";

  const std::string apply_topPtReweighting_string = cfg.exists("apply_topPtReweighting") ? cfg.getParameter<std::string>("apply_topPtReweighting") : "";
  apply_topPtReweighting_ = get_topPtRwgtOpt(apply_topPtReweighting_string);
}

AnalysisConfig::~AnalysisConfig()
{}

const std::string & 
AnalysisConfig::analysis() const
{
  return analysis_string_;
}

const std::string & 
AnalysisConfig::process() const
{
  return process_string_;
}

const std::string &
AnalysisConfig::process_hh() const
{
  return process_string_hh_;
}

bool
AnalysisConfig::isData() const
{
  return isData_;
}

bool
AnalysisConfig::isMC() const
{
  return isMC_;
}

bool
AnalysisConfig::isMC_DY() const
{
  return isMC_DY_;
}

bool
AnalysisConfig::isMC_WJets() const
{
  return isMC_WJets_;
}
 
bool
AnalysisConfig::isMC_WZ() const
{
  return isMC_WZ_;
}
 
bool
AnalysisConfig::isMC_ZZ() const
{
  return isMC_ZZ_;
}

bool
AnalysisConfig::isMC_EWK() const
{
  return isMC_EWK_;
}

bool
AnalysisConfig::isMC_QCD() const
{
  return isMC_QCD_;
}

bool
AnalysisConfig::isMC_tH() const
{
  return isMC_tH_;
}
 
bool 
AnalysisConfig::isMC_ttH() const
{
  return isMC_ttH_;
}

bool
AnalysisConfig::isMC_ggH() const
{
  return isMC_ggH_;
}
 
bool
AnalysisConfig::isMC_qqH() const
{
  return isMC_qqH_;
}

bool
AnalysisConfig::isMC_VH() const
{
  return isMC_VH_;
}

bool
AnalysisConfig::isMC_WH() const
{
  return isMC_WH_;
}

bool
AnalysisConfig::isMC_ZH() const
{
  return isMC_ZH_;
}

bool
AnalysisConfig::isMC_H() const
{
  return isMC_H_;
}

bool
AnalysisConfig::isMC_HH() const
{
  return isMC_HH_;
}

bool 
AnalysisConfig::isMC_HH_resonant() const
{
  return isMC_HH_resonant_;
}

bool
AnalysisConfig::isMC_HH_resonant_spin0() const
{
  return isMC_HH_resonant_spin0_;
}

bool
AnalysisConfig::isMC_HH_resonant_spin2() const
{
  return isMC_HH_resonant_spin2_;
}

bool
AnalysisConfig::isMC_HH_nonresonant() const
{
  return isMC_HH_nonresonant_;
}

std::vector<std::string> 
AnalysisConfig::get_decayModes_H() const
{
  return decayModes_H_;
}

double
AnalysisConfig::get_HH_resonant_mass() const
{
  return mass_HH_resonant_;
}

bool
AnalysisConfig::isHH_rwgt_allowed() const
{
  return isHH_rwgt_allowed_;
}

TopPtRwgtOption
AnalysisConfig::apply_topPtReweighting() const
{
  return apply_topPtReweighting_;
}
