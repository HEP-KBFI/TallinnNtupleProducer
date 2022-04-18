#include "TallinnNtupleProducer/EvtWeightTools/interface/LeptonFakeRateInterface.h"

#include "TallinnNtupleProducer/CommonTools/interface/cmsException.h"       // cmsException()
#include "TallinnNtupleProducer/CommonTools/interface/Era.h"                // Era, get_era()
#include "TallinnNtupleProducer/EvtWeightTools/interface/lutAuxFunctions.h" // lutWrapperTH2

#include <TFile.h>                                                          // TFile

#include <iostream>                                                         // std::cout

LeptonFakeRateInterface::LeptonFakeRateInterface(const edm::ParameterSet & cfg)
  : jetToEleFakeRateCorr_(1.)
  , jetToMuFakeRateCorr_(1.)
  , applyNonClosureCorrection_(cfg.getParameter<bool>("applyNonClosureCorrection"))
  , isDEBUG_(cfg.exists("debug") ? cfg.getParameter<bool>("debug") : false)
{
  const std::string inputFileName    = cfg.getParameter<std::string>("inputFileName");
  const std::string histogramName_e  = cfg.getParameter<std::string>("histogramName_e");
  const std::string histogramName_mu = cfg.getParameter<std::string>("histogramName_mu");

  std::vector<int> central_or_shift_enums;
  if(cfg.exists("central_or_shifts"))
  {
    const std::vector<std::string> central_or_shifts = cfg.getParameter<std::vector<std::string>>("central_or_shifts");
    for(const std::string & central_or_shift: central_or_shifts)
    {
      const int jetToLeptonFR_option = getJetToLeptonFR_option(central_or_shift);
      if(std::find(central_or_shift_enums.cbegin(), central_or_shift_enums.cend(), jetToLeptonFR_option) == central_or_shift_enums.cend())
      {
        central_or_shift_enums.push_back(jetToLeptonFR_option);
      }
    }
    assert(! central_or_shift_enums.empty());
  }

  if(applyNonClosureCorrection_)
  {
    const Era era = get_era(cfg.getParameter<std::string>("era"));
    switch(era)
    {
      // Slide 16 in https://indico.cern.ch/event/885279/contributions/3789360/attachments/2004535/3347458/mar15.pdf
      case Era::kUndefined: throw cmsException(this) << "Undefined era!";
      case Era::k2016: jetToEleFakeRateCorr_ = 1.376; jetToMuFakeRateCorr_ = 1.050; break;
      case Era::k2017: jetToEleFakeRateCorr_ = 1.252; jetToMuFakeRateCorr_ = 1.157; break;
      case Era::k2018: jetToEleFakeRateCorr_ = 1.325; jetToMuFakeRateCorr_ = 1.067; break;
    }
  }
  std::cout << "Non-closure corr to FR of muons = " << jetToMuFakeRateCorr_ << ", electrons = " << jetToEleFakeRateCorr_ << '\n';

  for(int FR_option = kFRl_central; FR_option <= kFRm_shape_eta_barrelDown; ++FR_option)
  {
    if(! central_or_shift_enums.empty() &&
       std::find(central_or_shift_enums.cbegin(), central_or_shift_enums.cend(), FR_option) == central_or_shift_enums.cend())
    {
      continue;
    }
    std::string suffix = "";
    if(FR_option == kFRl_central || (FR_option >= kFRe_shape_ptUp && FR_option <= kFRe_shape_eta_barrelDown))
    {
      switch(FR_option)
      {
        case kFRl_central:              suffix = "";      break;
        case kFRe_shape_ptUp:           suffix = "_pt1";  break;
        case kFRe_shape_ptDown:         suffix = "_pt2";  break;
        case kFRe_shape_normUp:         suffix = "_up";   break;
        case kFRe_shape_normDown:       suffix = "_down"; break;
        case kFRe_shape_eta_barrelUp:   suffix = "_be1";  break;
        case kFRe_shape_eta_barrelDown: suffix = "_be2";  break;
        default: throw cmsException(this, __func__, __LINE__) << "Invalid option: " << FR_option;
      }
      const std::string histogramName = histogramName_e + suffix;
      lutFakeRate_e_[FR_option]  = new lutWrapperTH2(
        inputFiles_, inputFileName, histogramName,  lut::kXptYabsEta, -1., 100.
      );
      std::cout << "Loaded histogram '" << histogramName << "' from file " << inputFileName << '\n';
    }
    if(FR_option == kFRl_central || (FR_option >= kFRm_shape_ptUp && FR_option <= kFRm_shape_eta_barrelDown))
    {
      switch(FR_option)
      {
        case kFRl_central:              suffix = "";      break;
        case kFRm_shape_ptUp:           suffix = "_pt1";  break;
        case kFRm_shape_ptDown:         suffix = "_pt2";  break;
        case kFRm_shape_normUp:         suffix = "_up";   break;
        case kFRm_shape_normDown:       suffix = "_down"; break;
        case kFRm_shape_eta_barrelUp:   suffix = "_be1";  break;
        case kFRm_shape_eta_barrelDown: suffix = "_be2";  break;
        default: throw cmsException(this, __func__, __LINE__) << "Invalid option: " << FR_option;
      }
      const std::string histogramName = histogramName_mu + suffix;
      lutFakeRate_mu_[FR_option] = new lutWrapperTH2(
        inputFiles_, inputFileName, histogramName, lut::kXptYabsEta, -1., 100.
      );
      std::cout << "Loaded histogram '" << histogramName << "' from file " << inputFileName << '\n';
    }
  }
}

LeptonFakeRateInterface::~LeptonFakeRateInterface()
{
  for(auto & kv: inputFiles_)
  {
    delete kv.second;
  }
}

namespace
{
  double
  capLeptonFakeRate(double leptonFakeRate,
                    double cap_threshold,
                    bool isDEBUG)
  {
    double leptonFakeRate_capped = -1.;

    if(leptonFakeRate > cap_threshold)
    {
      leptonFakeRate_capped = cap_threshold;
      if(isDEBUG){
        std::cout << "leptonFakeRate: " << leptonFakeRate 
                  << " has exceeded cap threshold: "
                  << cap_threshold
                  << " hence fixing it to "
                  << cap_threshold
                  << '\n';
      }
    }else{
      leptonFakeRate_capped = leptonFakeRate;
    }

    return leptonFakeRate_capped;
  }
}

double
LeptonFakeRateInterface::getWeight_e(double electronPt,
                                     double electronAbsEta,
                                     int central_or_shift) const
{
  if(! applyNonClosureCorrection_ && (central_or_shift == kFRe_shape_corrUp || central_or_shift == kFRe_shape_corrDown))
  {
    throw cmsException(this, __func__, __LINE__) << "Requested non-closure systematics while non-closure corrections disabled";
  }
  int central_or_shift_e = central_or_shift;
  if(central_or_shift_e == kFRe_shape_corrUp || central_or_shift_e == kFRe_shape_corrDown)
  {
    central_or_shift_e = kFRl_central;
  }
  if(! lutFakeRate_e_.count(central_or_shift_e))
  {
    throw cmsException(this, __func__, __LINE__) << "Invalid option: " << central_or_shift_e;
  }
  const double jetToEleFakeRate = lutFakeRate_e_.at(central_or_shift_e)->getSF(electronPt, electronAbsEta);
  double jetToEleFakeRate_tmp = -1.0;
  if(central_or_shift == kFRe_shape_corrUp)
  {
    jetToEleFakeRate_tmp = (jetToEleFakeRate * jetToEleFakeRateCorr_ * jetToEleFakeRateCorr_);
  }
  else if(central_or_shift == kFRe_shape_corrDown)
  {
    jetToEleFakeRate_tmp = jetToEleFakeRate;
  }else{
    jetToEleFakeRate_tmp = (jetToEleFakeRate * jetToEleFakeRateCorr_);
  }
  const double jetToEleFakeRate_final = capLeptonFakeRate(jetToEleFakeRate_tmp, 0.80, isDEBUG_);  
  if(isDEBUG_)
  {
    std::cout
      << get_human_line(this, __func__, __LINE__)
      << "FR(e pT = " << electronPt << ", |eta| = " << electronAbsEta << ") = " << jetToEleFakeRate
      << " @ central_or_shift = " << central_or_shift_e << " => FR(jet->e) = " << jetToEleFakeRate_final << '\n';
    ;
  }
  return jetToEleFakeRate_final;
}

double
LeptonFakeRateInterface::getWeight_mu(double muonPt,
                                      double muonAbsEta,
                                      int central_or_shift) const
{
  if(! applyNonClosureCorrection_ && (central_or_shift == kFRm_shape_corrUp || central_or_shift == kFRm_shape_corrDown))
  {
    throw cmsException(this, __func__, __LINE__) << "Requested non-closure systematics while non-closure corrections disabled";
  }
  int central_or_shift_m = central_or_shift;
  if(central_or_shift_m == kFRm_shape_corrUp || central_or_shift_m == kFRm_shape_corrDown)
  {
    central_or_shift_m = kFRl_central;
  }
  if(! lutFakeRate_mu_.count(central_or_shift_m))
  {
    throw cmsException(this, __func__, __LINE__) << "Invalid option: " << central_or_shift_m;
  }
  const double jetToMuFakeRate = lutFakeRate_mu_.at(central_or_shift_m)->getSF(muonPt, muonAbsEta);
  double jetToMuFakeRate_tmp = -1.0;
  if(central_or_shift == kFRm_shape_corrUp)
  {
    jetToMuFakeRate_tmp = (jetToMuFakeRate * jetToMuFakeRateCorr_ * jetToMuFakeRateCorr_);
  }
  else if(central_or_shift == kFRm_shape_corrDown)
  {
    jetToMuFakeRate_tmp = jetToMuFakeRate;
  }else{
    jetToMuFakeRate_tmp = (jetToMuFakeRate * jetToMuFakeRateCorr_);
  }
  const double jetToMuFakeRate_final = capLeptonFakeRate(jetToMuFakeRate_tmp, 0.80, isDEBUG_);  
  if(isDEBUG_)
  {
    std::cout
      << get_human_line(this, __func__, __LINE__)
      << "FR(mu pT = " << muonPt << ", |eta| = " << muonAbsEta << ") = " << jetToMuFakeRate
      << " @ central_or_shift = " << central_or_shift_m << " => FR(jet->mu) = " << jetToMuFakeRate_final << '\n';
    ;
  }
  return jetToMuFakeRate_final;
}
