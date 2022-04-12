#include "TallinnNtupleProducer/Readers/interface/MEtFilterReader.h"

#include "TallinnNtupleProducer/CommonTools/interface/Era.h"                  // Era, get_era()
#include "TallinnNtupleProducer/Objects/interface/MEtFilter.h"                // MEtFilter
#include "TallinnNtupleProducer/Readers/interface/BranchAddressInitializer.h" // BranchAddressInitializer

#include <TString.h>                                                          // Form
#include <TTree.h>                                                            // TTree

#include <assert.h>                                                           // assert()

int MEtFilterReader::numInstances_ = 0;
MEtFilterReader * MEtFilterReader::instance_ = nullptr;

MEtFilterReader::MEtFilterReader(const edm::ParameterSet & cfg)
  : ReaderBase(cfg)
{
  era_ = get_era(cfg.getParameter<std::string>("era"));
  setBranchNames();
}

MEtFilterReader::~MEtFilterReader()
{
  --numInstances_;
  assert(numInstances_ >= 0);
  if(numInstances_ == 0)
  {
    instance_ = nullptr;
  }
}

void
MEtFilterReader::setBranchNames()
{
  if(numInstances_ == 0)
  {
    for(int flag = 0; flag < MEtFilterFlag::LAST; ++flag)
    {
      branchNames_[flag] = getMEtFilterFlagString(static_cast<MEtFilterFlag>(flag));
    }
    instance_ = this;
  }
  ++numInstances_;
}

std::vector<std::string>
MEtFilterReader::setBranchAddresses(TTree * tree)
{
  if(instance_ == this)
  {
    BranchAddressInitializer bai(tree);
    for(int flag = 0; flag < MEtFilterFlag::LAST; ++flag)
    {
      const MEtFilterFlag flag_enum = static_cast<MEtFilterFlag>(flag);
      std::string metFilterFlagString = getMEtFilterFlagString(flag_enum);
      if(flag == MEtFilterFlag::ecalBadCalibFilterV2)
      {
        if(era_ == Era::k2016)
        {
          // not present in 2016
          continue;
        }
        else
        {
          // this may or may not be an issue
          //metFilterFlagString += "_bool";
        }
      }
      bai.setBranchAddress(metFilter_.getFlagRef(flag_enum), metFilterFlagString);
    }
    return bai.getBoundBranchNames();
  }
  return {};
}

const MEtFilter &
MEtFilterReader::read() const
{
  return metFilter_;
}
