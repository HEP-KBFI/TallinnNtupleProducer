#include "TallinnNtupleProducer/Readers/interface/L1PreFiringWeightReader.h"

#include "TallinnNtupleProducer/CommonTools/interface/BranchAddressInitializer.h" // BranchAddressInitializer
#include "TallinnNtupleProducer/CommonTools/interface/cmsException.h"             // cmsException()
#include "TallinnNtupleProducer/CommonTools/interface/Era.h"                      // Era
#include "TallinnNtupleProducer/CommonTools/interface/sysUncertOptions.h"         // L1PreFiringWeightSys

#include "TString.h"                                                              // Form()
#include "TTree.h"                                                                // TTree

#include <assert.h>                                                               // assert()

std::map<std::string, int> L1PreFiringWeightReader::numInstances_;
std::map<std::string, L1PreFiringWeightReader*> L1PreFiringWeightReader::instances_;

L1PreFiringWeightReader::L1PreFiringWeightReader(const edm::ParameterSet & cfg)
  : ReaderBase(cfg)
  , era_(Era::kUndefined)
  , branchName_("L1PreFiringWeight")
  , l1PreFiringWeight_nominal_(1.)
{
  era_ = get_era(cfg.getParameter<std::string>("era"));
  setBranchNames();
}

L1PreFiringWeightReader::~L1PreFiringWeightReader()
{
  --numInstances_[branchName_];
  assert(numInstances_[branchName_] >= 0);

  if(numInstances_[branchName_] == 0)
  {
    L1PreFiringWeightReader * const gInstance = instances_[branchName_];
    assert(gInstance);
    instances_[branchName_] = nullptr;
  }
}

void
L1PreFiringWeightReader::setBranchNames()
{
  if(numInstances_[branchName_] == 0)
  {
    instances_[branchName_] = this;
  }
  ++numInstances_[branchName_];
}

std::vector<std::string>
L1PreFiringWeightReader::setBranchAddresses(TTree * inputTree)
{
  if(era_ != Era::k2018)
  {
    BranchAddressInitializer bai(inputTree);
    bai.setBranchAddress(l1PreFiringWeight_nominal_, Form("%s_Nom", branchName_.data()), 1.);
    bai.setBranchAddress(l1PreFiringWeight_up_,      Form("%s_Up",  branchName_.data()), 1.);
    bai.setBranchAddress(l1PreFiringWeight_down_,    Form("%s_Dn",  branchName_.data()), 1.);
    return bai.getBoundBranchNames_read();
  }
  return {};
}

double
L1PreFiringWeightReader::getWeight(L1PreFiringWeightSys option) const
{
  switch(option)
  {
    case L1PreFiringWeightSys::nominal: return l1PreFiringWeight_nominal_; break;
    case L1PreFiringWeightSys::up:      return l1PreFiringWeight_up_;      break;
    case L1PreFiringWeightSys::down:    return l1PreFiringWeight_down_;    break;
    default: throw cmsException(this, __func__, __LINE__) << "Invalid option";
  }
}
