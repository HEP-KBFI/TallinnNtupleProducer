#include "TallinnNtupleProducer/Readers/interface/RecoMEtReader.h"

#include "TallinnNtupleProducer/CommonTools/interface/BranchAddressInitializer.h" // BranchAddressInitializer
#include "TallinnNtupleProducer/CommonTools/interface/cmsException.h"             // cmsException()
#include "TallinnNtupleProducer/CommonTools/interface/Era.h"                      // Era, get_era()
#include "TallinnNtupleProducer/CommonTools/interface/merge_systematic_shifts.h"  // merge_systematic_shifts()
#include "TallinnNtupleProducer/CommonTools/interface/map_keys.h"                 // map_keys()
#include "TallinnNtupleProducer/CommonTools/interface/sysUncertOptions.h"         // getBranchName_jetMET(), kJetMET_*
#include "TallinnNtupleProducer/Readers/interface/metPhiModulation.h"             // METXYCorr_Met_MetPhi()

#include "TString.h"                                                              // Form()
#include "TTree.h"                                                                // TTree

#include <cmath>                                                                  // std::atan(), std::cos(), std::sin()

#define _USE_MATH_DEFINES // M_PI

std::map<std::string, int> RecoMEtReader::numInstances_;
std::map<std::string, RecoMEtReader *> RecoMEtReader::instances_;

RecoMEtReader::RecoMEtReader(const edm::ParameterSet & cfg)
  : ReaderBase(cfg)
  , era_(Era::kUndefined)
  , isMC_(false)
  , branchName_obj_("")
  , branchName_cov_("")
  , ptPhiOption_central_(-1)
  , ptPhiOption_(-1)
  , eventInfo_(nullptr)
  , recoVertex_(nullptr)
  , enable_phiModulationCorr_(false)
{
  era_ = get_era(cfg.getParameter<std::string>("era"));
  if ( cfg.exists("branchName_met") && cfg.exists("branchName_metCov") )
  {
    branchName_obj_ = cfg.getParameter<std::string>("branchName_met");
    branchName_cov_ = cfg.getParameter<std::string>("branchName_metCov");
  }
  else
  {
    branchName_obj_ = cfg.getParameter<std::string>("branchName"); // default = "MET"
    branchName_cov_ = branchName_obj_;
  }
  isMC_ = cfg.getParameter<bool>("isMC");
  ptPhiOption_central_ = ( isMC_ ) ? kJetMET_central : kJetMET_central_nonNominal;
  ptPhiOption_ = ptPhiOption_central_; 
  setBranchNames();
}

RecoMEtReader::~RecoMEtReader()
{
  --numInstances_[branchName_obj_];
  assert(numInstances_[branchName_obj_] >= 0);
  if(numInstances_[branchName_obj_] == 0)
  {
    RecoMEtReader * const gInstance = instances_[branchName_obj_];
    assert(gInstance);
    instances_[branchName_obj_] = nullptr;
  }
}

void
RecoMEtReader::setMEt_central_or_shift(int central_or_shift)
{
  if(! isMC_ && central_or_shift != kJetMET_central_nonNominal)
  {
    throw cmsException(this, __func__, __LINE__) << "Nominal MET available only in MC";
  }
  if(! isValidJESsource(era_, central_or_shift))
  {
    ptPhiOption_ = ptPhiOption_central_;
  }
  if(central_or_shift <= kJetMET_UnclusteredEnDown)
  {
    ptPhiOption_ = central_or_shift;
  }
  else
  {
    std::cout
        << get_human_line(this, __func__, __LINE__)
        << "Not setting the systematics option to " << central_or_shift
        << " but keeping it at " << ptPhiOption_ << std::endl;
    ;
  }
}

void
RecoMEtReader::setBranchNames()
{
  if(numInstances_[branchName_obj_] == 0)
  {
    for(int idxShift = kJetMET_central_nonNominal; idxShift <= kJetMET_UnclusteredEnDown; ++idxShift)
    {
      if( (idxShift == ptPhiOption_central_) || (isMC_ && isValidJESsource(era_, idxShift)) )
      {
        branchName_pt_[idxShift]  = getBranchName_jetMET(branchName_obj_, era_, idxShift, true);
        branchName_phi_[idxShift] = getBranchName_jetMET(branchName_obj_, era_, idxShift, false);
      }
    }
    branchName_sumEt_ = Form("%s_%s", branchName_obj_.data(), "sumEt");
    branchName_covXX_ = Form("%s_%s", branchName_cov_.data(), "covXX");
    branchName_covXY_ = Form("%s_%s", branchName_cov_.data(), "covXY");
    branchName_covYY_ = Form("%s_%s", branchName_cov_.data(), "covYY");
    instances_[branchName_obj_] = this;
  }
  ++numInstances_[branchName_obj_];
}

std::vector<std::string>
RecoMEtReader::setBranchAddresses(TTree * inputTree)
{
  if(instances_[branchName_obj_] == this)
  {
    BranchAddressInitializer bai(inputTree);
    for(int idxShift = kJetMET_central_nonNominal; idxShift <= kJetMET_UnclusteredEnDown; ++idxShift)
    {
      if( (idxShift == ptPhiOption_central_) || (isMC_ && isValidJESsource(era_, idxShift)) )
      {
        met_pt_systematics_[idxShift] = 0.;
        bai.setBranchAddress(met_pt_systematics_[idxShift], branchName_pt_[idxShift]);
        met_phi_systematics_[idxShift] = 0.;
        bai.setBranchAddress(met_phi_systematics_[idxShift], branchName_phi_[idxShift]);
      }
    }
    bai.setBranchAddress(met_sumEt_, branchName_sumEt_);
    bai.setBranchAddress(met_covXX_, branchName_covXX_);
    bai.setBranchAddress(met_covXY_, branchName_covXY_);
    bai.setBranchAddress(met_covYY_, branchName_covYY_);

    return bai.getBoundBranchNames_read();
  }
  return {};
}

void
RecoMEtReader::set_phiModulationCorrDetails(const EventInfo * const eventInfo,
                                            const RecoVertex * const recoVertex,
                                            bool enable_phiModulationCorr)
{
  eventInfo_ = eventInfo;
  recoVertex_ = recoVertex;
  enable_phiModulationCorr_ = enable_phiModulationCorr;
}

namespace
{
  double square(double x)
  {
    return x*x;
  }
}

RecoMEt
RecoMEtReader::read() const
{
  const RecoMEtReader * const gInstance = instances_[branchName_obj_];
  assert(gInstance);

  double met_pt = gInstance->met_pt_systematics_.at(ptPhiOption_);
  double met_phi = gInstance->met_phi_systematics_.at(ptPhiOption_);
  if(enable_phiModulationCorr_)
  {
    const std::pair<double, double> met_pxpyCorr = METXYCorr_Met_MetPhi(eventInfo_, recoVertex_, era_);
    double met_px = met_pt*std::cos(met_phi) + met_pxpyCorr.first;
    double met_py = met_pt*std::sin(met_phi) + met_pxpyCorr.second;
    met_pt = std::sqrt(square(met_px) + square(met_py));
    if     (met_px > 0) { met_phi = std::atan(met_py / met_px); }
    else if(met_px < 0) { met_phi = std::atan(met_py / met_px) + ((met_py > 0. ? +1. : -1.) * M_PI);  }
    else                { met_phi = (met_py > 0. ? +1. : -1.) * M_PI; }
    // CV: force another call to set_phiModulationCorrDetails() 
    //     before applying MEt x/y shift correction to next event
    enable_phiModulationCorr_ = false;
  }

  RecoMEt met(
    met_pt,
    met_phi,
    met_sumEt_,
    met_covXX_,
    met_covXY_,
    met_covYY_
  );

  return met;
}

std::vector<std::string>
RecoMEtReader::get_supported_systematics(const edm::ParameterSet & cfg)
{
  static std::vector<std::string> supported_systematics;
  if(supported_systematics.empty())
  {
    merge_systematic_shifts(supported_systematics, RecoJetReaderAK4::get_supported_systematics(cfg));
    merge_systematic_shifts(supported_systematics, map_keys(metSysMap));
  }
  return supported_systematics;
}
