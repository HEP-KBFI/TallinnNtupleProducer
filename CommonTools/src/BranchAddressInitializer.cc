#include "TallinnNtupleProducer/CommonTools/interface/BranchAddressInitializer.h"

#include "TallinnNtupleProducer/CommonTools/interface/format_vT.h" // format_vstring()

#include <TString.h>                                               // Form()

#include <algorithm>                                               // std::sort()

std::set<std::string> BranchAddressInitializer::gBoundBranchNames_read_;
std::set<std::string> BranchAddressInitializer::gBoundBranchNames_write_;

BranchAddressInitializer::BranchAddressInitializer(TTree * tree, 
                                                   int lenVar,
                                                   const std::string & branchName_n)
  : tree_(tree)
  , lenVar_(lenVar)
  , branchName_n_(branchName_n)
  , ignoreErrors_(false)
  , inputBranchNames_initialized_(false)
{
  if(! tree_)
  {
    throw cmsException(this) << "No TTree provided!";
  }
}

BranchAddressInitializer::~BranchAddressInitializer()
{}

void
BranchAddressInitializer::ignoreErrors(bool flag)
{
  ignoreErrors_ = flag;
}

BranchAddressInitializer &
BranchAddressInitializer::setLenVar(int lenVar)
{
  lenVar_ = lenVar;
  return *this;
}

std::vector<std::string>
BranchAddressInitializer::getBoundBranchNames_read() const
{
  return boundBranchNames_read_;
}

std::vector<std::string>
BranchAddressInitializer::getBoundBranchNames_write() const
{
  return boundBranchNames_write_;
}

bool
BranchAddressInitializer::hasBranchName(const std::string & branchName)
{
  if(branchName.empty())
  {
    throw cmsException(this, __func__, __LINE__) << "Cannot search for an empty branch name";
  }
  if(! inputBranchNames_initialized_)
  {
    const TObjArray * const tree_branches = tree_->GetListOfBranches();
    const int tree_numBranches = tree_branches->GetEntries();
    for(int idxBranch = 0; idxBranch < tree_numBranches; ++idxBranch)
    {
      const TBranch * const tree_branch = dynamic_cast<const TBranch * const>(tree_branches->At(idxBranch));
      assert(tree_branch);
      const std::string tree_branchName = tree_branch->GetName();
      if(std::find(inputBranchNames_.cbegin(), inputBranchNames_.cend(), tree_branchName) != inputBranchNames_.cend())
      {
        std::cout << "Found duplicate branch names in input TTree: " << tree_branchName << '\n';
        continue;
      }
      inputBranchNames_.push_back(tree_branchName);
    }
    inputBranchNames_initialized_ = true;
  }
  return std::find(inputBranchNames_.cbegin(), inputBranchNames_.cend(), branchName) != inputBranchNames_.cend();
}

namespace
{
  void
  recordBoundBranchName(const std::string & branchName, 
                        std::vector<std::string> & boundBranchNames, std::set<std::string> & gBoundBranchNames,
                        const std::string & errorMessage)
  {
    if(std::find(boundBranchNames.cbegin(), boundBranchNames.cend(), branchName) == boundBranchNames.cend())
    {
      boundBranchNames.push_back(branchName);
    }
    if ( gBoundBranchNames.count(branchName) > 0 )
    {
      throw cmsException(__func__, __LINE__) << errorMessage;
    }
    gBoundBranchNames.insert(branchName);
  }
}

void
BranchAddressInitializer::recordBoundBranchName_read(const std::string & branchName)
{
  recordBoundBranchName(
    branchName, 
    boundBranchNames_read_, gBoundBranchNames_read_, 
    Form("Branch name '%s' is already being read from !!", branchName.data())
  );
}

void
BranchAddressInitializer::recordBoundBranchName_write(const std::string & branchName)
{
  recordBoundBranchName(
    branchName,
    boundBranchNames_write_, gBoundBranchNames_write_,
    Form("Branch name '%s' is already being written to !!", branchName.data())
  );
}

namespace
{
  std::vector<std::string>
  convert_to_vector(const std::set<std::string> & branchNames_set)
  {
    std::vector<std::string> branchNames_vector;
    branchNames_vector.insert(branchNames_vector.end(), branchNames_set.begin(), branchNames_set.end());
    std::sort(branchNames_vector.begin(), branchNames_vector.end());
    return branchNames_vector;
  }
}

void
BranchAddressInitializer::BranchAddressInitializer::print(std::ostream & stream)
{
  stream << "<BranchAddressInitializer::print>:" << std::endl;
  std::vector<std::string> branchNames_read = convert_to_vector(gBoundBranchNames_read_);
  stream << "branches read from input tree: " << branchNames_read.size() << std::endl;
  stream << " list of branches = " << format_vstring(branchNames_read) << std::endl;
  std::vector<std::string> branchNames_write = convert_to_vector(gBoundBranchNames_write_);
  stream << "branches written to output tree: " << branchNames_write.size() << std::endl;
  stream << " list of branches = " << format_vstring(branchNames_write) << std::endl;
  stream << std::endl;
}

void
BranchAddressInitializer::reset_inputs()
{
  BranchAddressInitializer::gBoundBranchNames_read_.clear();
}
