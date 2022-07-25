#include "TallinnNtupleProducer/Readers/interface/GenMEtReader.h"

#include "TallinnNtupleProducer/CommonTools/interface/BranchAddressInitializer.h" // BranchAddressInitializer
#include "TallinnNtupleProducer/CommonTools/interface/cmsException.h"             // cmsException()

#include "TTree.h"                                                                // TTree
#include "TString.h"                                                              // Form()

std::map<std::string, int> GenMEtReader::numInstances_;
std::map<std::string, GenMEtReader *> GenMEtReader::instances_;

GenMEtReader::GenMEtReader(const edm::ParameterSet & cfg)
  : ReaderBase(cfg)
  , branchName_obj_(cfg.getParameter<std::string>("branchName"))
{
  setBranchNames();
}

GenMEtReader::~GenMEtReader()
{
  --numInstances_[branchName_obj_];
  assert(numInstances_[branchName_obj_] >= 0);
  if(numInstances_[branchName_obj_] == 0)
    {
      GenMEtReader * const gInstance = instances_[branchName_obj_];
      assert(gInstance);
      instances_[branchName_obj_] = nullptr;
    }
}

void
GenMEtReader::setBranchNames()
{
  if(numInstances_[branchName_obj_] == 0)
  {
    branchName_pt_ = Form("%s_%s", branchName_obj_.data(), "pt");
    branchName_phi_ = Form("%s_%s", branchName_obj_.data(), "phi");
    instances_[branchName_obj_] = this;
  }
  ++numInstances_[branchName_obj_];
}

std::vector<std::string>
GenMEtReader::setBranchAddresses(TTree * tree)
{
  if(instances_[branchName_obj_] == this)
  {
    BranchAddressInitializer bai(tree);
    bai.setBranchAddress(pt_,  branchName_pt_);
    bai.setBranchAddress(phi_, branchName_phi_);
    return bai.getBoundBranchNames_read();
  }
  return {};
}

GenMEt
GenMEtReader::read() const
{
  const GenMEtReader * const gInstance = instances_[branchName_obj_];
  assert(gInstance);
  return { gInstance->pt_, gInstance->phi_ };
}
