#include "TallinnNtupleProducer/Readers/interface/RecoVertexReader.h"

#include "TallinnNtupleProducer/CommonTools/interface/cmsException.h"         // cmsException()
#include "TallinnNtupleProducer/Readers/interface/BranchAddressInitializer.h" // BranchAddressInitializer

#include "TTree.h"                                                            // TTree
#include "TString.h"                                                          // Form()

std::map<std::string, int> RecoVertexReader::numInstances_;
std::map<std::string, RecoVertexReader *> RecoVertexReader::instances_;

RecoVertexReader::RecoVertexReader(const edm::ParameterSet & cfg)
{
  branchName_ = cfg.getParameter<std::string>("branchName"); // default = "PV"
  setBranchNames();
}

RecoVertexReader::~RecoVertexReader()
{
  --numInstances_[branchName_];
  assert(numInstances_[branchName_] >= 0);
  if(numInstances_[branchName_] == 0)
  {
    RecoVertexReader * const gInstance = instances_[branchName_];
    assert(gInstance);
    instances_[branchName_] = nullptr;
  }
}

void
RecoVertexReader::setBranchNames()
{
  if(numInstances_[branchName_] == 0)
  {
    branchName_x_ = Form("%s_%s", branchName_.data(), "x");
    branchName_y_ = Form("%s_%s", branchName_.data(), "y");
    branchName_z_ = Form("%s_%s", branchName_.data(), "z");
    branchName_ndof_ = Form("%s_%s", branchName_.data(), "ndof");
    branchName_chi2_ = Form("%s_%s", branchName_.data(), "chi2");
    branchName_score_ = Form("%s_%s", branchName_.data(), "score");
    branchName_npvs_ = Form("%s_%s", branchName_.data(), "npvs");
    branchName_npvsGood_ = Form("%s_%s", branchName_.data(), "npvsGood");
    instances_[branchName_] = this;
  }
  ++numInstances_[branchName_];
}

std::vector<std::string>
RecoVertexReader::setBranchAddresses(TTree * tree)
{
  if(instances_[branchName_] == this)
  {
    BranchAddressInitializer bai(tree);
    bai.setBranchAddress(recoVertex_.position_x_, branchName_x_);
    bai.setBranchAddress(recoVertex_.position_y_, branchName_y_);
    bai.setBranchAddress(recoVertex_.position_z_, branchName_z_);
    bai.setBranchAddress(recoVertex_.ndof_, branchName_ndof_);
    bai.setBranchAddress(recoVertex_.chi2_, branchName_chi2_);
    bai.setBranchAddress(recoVertex_.score_, branchName_score_);
    bai.setBranchAddress(recoVertex_.npvs_, branchName_npvs_);
    bai.setBranchAddress(recoVertex_.npvsGood_, branchName_npvsGood_);
    return bai.getBoundBranchNames();
  }
  return {};
}

RecoVertex
RecoVertexReader::read() const
{
  return recoVertex_;
}
