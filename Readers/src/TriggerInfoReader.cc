#include "TallinnNtupleProducer/Readers/interface/TriggerInfoReader.h"

#include "TallinnNtupleProducer/Readers/interface/BranchAddressInitializer.h" // BranchAddressInitializer
#include "TallinnNtupleProducer/Framework/interface/cmsException.h"           // cmsException()

#include <set>                                                                // std::set

TriggerInfoReader::TriggerInfoReader(const edm::ParameterSet & cfg)
  : triggerInfo_(cfg)
{}

TriggerInfoReader::~TriggerInfoReader()
{}

std::vector<std::string>
TriggerInfoReader::setBranchAddresses(TTree * tree)
{
  const std::vector<std::string> available_branches = this->get_available_branches(tree);
  std::set<string> used_branches;
  BranchAddressInitializer bai(tree);
  for ( auto entry : triggerInfo_.entries_ )
  {
    for ( auto hltPath : entry.hltPaths_ )
    {
      if ( std::find(available_branches.cbegin(), available_branches.cend(), hltPath.branchName_) != available_branches.cend() )
      {
        if ( used_branches.find(hltPath.branchName_) != used_branches.end() )
          throw cmsException(__func__, __LINE__) 
            << "Branch '" << hltPath.branchName_ << "' cannot be read more than once !!";
        bai.setBranchAddress(hltPath.status_, hltPath.branchName_);         
        used_branches.insert(hltPath.branchName_);
      }
      else
      {
        std::cerr << "Warning in <TriggerInfoReader::setBranchAddresses>:" 
                  << " Branch '" << hltPath.branchName_ << "' not available, defaulting to false" << std::endl;
        hltPath.status_ = false;
      }
    }
  }
  return bai.getBoundBranchNames();
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
