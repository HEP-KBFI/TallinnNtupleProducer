#include "TallinnNtupleProducer/Readers/interface/RunLumiEventReader.h"

#include "TallinnNtupleProducer/CommonTools/interface/BranchAddressInitializer.h" // BranchAddressInitializer
#include "TallinnNtupleProducer/Objects/interface/RunLumiEvent.h"                 // RunLumiEvent

#include "TString.h"                                                              // Form()
#include "TTree.h"                                                                // TTree

#include <assert.h>                                                               // assert()

int RunLumiEventReader::numInstances_ = 0;
RunLumiEventReader * RunLumiEventReader::gInstance_ = nullptr;

RunLumiEventReader::RunLumiEventReader(const edm::ParameterSet & cfg)
  : ReaderBase(cfg)
  , info_(nullptr)
  , branchName_run_("run")
  , branchName_lumi_("luminosityBlock")
  , branchName_event_("event")
{
  if(numInstances_ == 0)
  {
    info_ = new RunLumiEvent();
    gInstance_ = this;
  }
}

RunLumiEventReader::~RunLumiEventReader()
{
  --numInstances_;
  assert(numInstances_ >= 0);
  if ( numInstances_ == 0 )
  {
    assert(gInstance_);
    delete gInstance_->info_;
  }
}

std::vector<std::string>
RunLumiEventReader::setBranchAddresses(TTree * inputTree)
{
  std::vector<std::string> bound_branches;
  if ( gInstance_ == this )
  {
    BranchAddressInitializer bai(inputTree);
    bai.setBranchAddress(gInstance_->info_->run_, branchName_run_);
    bai.setBranchAddress(gInstance_->info_->lumi_, branchName_lumi_);
    bai.setBranchAddress(gInstance_->info_->event_, branchName_event_);
    bound_branches = bai.getBoundBranchNames_read();
  }
  return bound_branches;
}

const RunLumiEvent &
RunLumiEventReader::read() const
{
  assert(gInstance_);
  return *gInstance_->info_;
}
