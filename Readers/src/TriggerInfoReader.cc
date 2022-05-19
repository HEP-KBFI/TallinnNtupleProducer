#include "TallinnNtupleProducer/Readers/interface/TriggerInfoReader.h"

#include "TallinnNtupleProducer/CommonTools/interface/BranchAddressInitializer.h" // BranchAddressInitializer
#include "TallinnNtupleProducer/CommonTools/interface/cmsException.h"             // cmsException()

#include <set>                                                                    // std::set
#include <string>                                                                 // std::string

TriggerInfoReader::TriggerInfoReader(const edm::ParameterSet & cfg)
  : ReaderBase(cfg)
  , triggerInfo_(cfg)
{}

TriggerInfoReader::~TriggerInfoReader()
{}

std::vector<std::string>
TriggerInfoReader::setBranchAddresses(TTree * tree)
{
  const std::vector<std::string> available_branches = this->get_available_branches(tree);
  std::set<std::string> used_branches;
  BranchAddressInitializer bai(tree);
  for ( trigger::Entry & entry : triggerInfo_.entries_ )
  {
    for ( trigger::HLTPath & hltPath : entry.hltPaths_ )
    {
      if ( std::find(available_branches.cbegin(), available_branches.cend(), hltPath.branchName()) != available_branches.cend() )
      {
        if ( used_branches.find(hltPath.branchName_) != used_branches.end() )
          throw cmsException(__func__, __LINE__) 
            << "Branch '" << hltPath.branchName() << "' cannot be read more than once !!";
        bai.setBranchAddress(hltPath.status_, hltPath.branchName_);         
        used_branches.insert(hltPath.branchName());
      }
      else
      {
        std::cerr << "Warning in <TriggerInfoReader::setBranchAddresses>:" 
                  << " Branch '" << hltPath.branchName_ << "' not available, defaulting to false" << std::endl;
        hltPath.status_ = false;
      }
    }
  }
  return bai.getBoundBranchNames_read();
}

const TriggerInfo &
TriggerInfoReader::read() const
{
  return triggerInfo_;
}

std::vector<std::string>
TriggerInfoReader::get_available_branches(TTree * tree) const
{
  TObjArray * arr = tree->GetListOfBranches();
  TIter it(arr);
  TObject * obj = nullptr;
  std::vector<std::string> available_branches;
  while( (obj = it.Next()) )
  {
    available_branches.push_back(obj->GetName());
  }
  delete obj;
  return available_branches;
}

std::vector<std::string>
TriggerInfoReader::get_supported_systematics(const edm::ParameterSet & cfg)
{
  return {};
}
