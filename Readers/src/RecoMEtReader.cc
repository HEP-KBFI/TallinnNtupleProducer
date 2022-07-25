#include "TallinnNtupleProducer/Readers/interface/RecoMEtReader.h"

#include "TallinnNtupleProducer/CommonTools/interface/BranchAddressInitializer.h" // BranchAddressInitializer
#include "TallinnNtupleProducer/CommonTools/interface/merge_systematic_shifts.h"  // merge_systematic_shifts()
#include "TallinnNtupleProducer/CommonTools/interface/map_keys.h"                 // map_keys()
#include "TallinnNtupleProducer/CommonTools/interface/sysUncertOptions.h"         // metSysMap

#include "TString.h"                                                              // Form()
#include "TTree.h"                                                                // TTree

std::map<std::string, int> RecoMEtReader::numInstances_;
std::map<std::string, RecoMEtReader *> RecoMEtReader::instances_;

RecoMEtReader::RecoMEtReader(const edm::ParameterSet & cfg)
  : ReaderBase(cfg)
  , isMC_(false)
  , branchName_obj_("")
  , branchName_cov_("")
{
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
RecoMEtReader::setBranchNames()
{
  if(numInstances_[branchName_obj_] == 0)
  {
    branchName_pt_ = Form("%s_%s", branchName_obj_.data(), "pt");
    branchName_phi_ = Form("%s_%s", branchName_obj_.data(), "phi");
    branchName_sumEt_ = Form("%s_%s", branchName_obj_.data(), "sumEt");
    branchName_unclDeltaX_ = Form("%s_%s", branchName_obj_.data(), "MetUnclustEnUpDeltaX");
    branchName_unclDeltaY_ = Form("%s_%s", branchName_obj_.data(), "MetUnclustEnUpDeltaY");
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
    bai.setBranchAddress(met_pt_, branchName_pt_);
    bai.setBranchAddress(met_phi_, branchName_phi_);
    bai.setBranchAddress(met_sumEt_, branchName_sumEt_);
    bai.setBranchAddress(met_unclDeltaX_, branchName_unclDeltaX_);
    bai.setBranchAddress(met_unclDeltaY_, branchName_unclDeltaY_);
    bai.setBranchAddress(met_covXX_, branchName_covXX_);
    bai.setBranchAddress(met_covXY_, branchName_covXY_);
    bai.setBranchAddress(met_covYY_, branchName_covYY_);
    return bai.getBoundBranchNames_read();
  }
  return {};
}

RecoMEt
RecoMEtReader::read() const
{
  const RecoMEtReader * const gInstance = instances_[branchName_obj_];
  assert(gInstance);

  RecoMEt met(
    gInstance->met_pt_,
    gInstance->met_phi_,
    gInstance->met_sumEt_,
    gInstance->met_unclDeltaX_,
    gInstance->met_unclDeltaY_,
    gInstance->met_covXX_,
    gInstance->met_covXY_,
    gInstance->met_covYY_
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
