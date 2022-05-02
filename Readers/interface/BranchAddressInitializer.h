#ifndef TallinnNtupleProducer_Readers_BranchAddressInitializer_h
#define TallinnNtupleProducer_Readers_BranchAddressInitializer_h

#include "TallinnNtupleProducer/CommonTools/interface/cmsException.h" // cmsException()
#include "TallinnNtupleProducer/Readers/interface/TypeTraits.h"       // Traits<>

#include <TTree.h>                                                    // TTree

#include <type_traits>                                                // std::enable_if, std::is_arithmetic
#include <string>                                                     // std::string
#include <algorithm>                                                  // std::sort(), std::set_union(), std::fill_n()
#include <iostream>                                                   // std::cout

struct BranchAddressInitializer
{
 public:
  BranchAddressInitializer(TTree * tree = nullptr,
                           int lenVar = -1,
                           const std::string & branchName_n = "")
    : tree_(tree)
    , lenVar_(lenVar)
    , branchName_n_(branchName_n)
    , ignoreErrors_(false)
    , inputBranchNamesFound_(false)
  {
    if(! tree_)
    {
      throw cmsException(this) << "No TTree provided!";
    }
  }

  void
  ignoreErrors(bool flag)
  {
    ignoreErrors_ = flag;
  }

  template<typename T,
           typename = std::enable_if<std::is_arithmetic<T>::value>>
  void
  setBranch(T & value,
            const std::string & branchName)
  {
    tree_ -> Branch(branchName.data(), &value, Form("%s/%s", branchName.data(), Traits<T>::TYPE_NAME));
  }

  template<typename T,
           typename = std::enable_if<std::is_arithmetic<T>::value>>
  void
  setBranch(T * & address,
            const std::string & branchName)
  {
    if(lenVar_ > 0)
    {
      address = new T[lenVar_];
    }
    tree_ -> Branch(branchName.data(), address, Form(
      "%s%s/%s", branchName.data(), Form("[%s]", branchName_n_.data()), Traits<T>::TYPE_NAME)
    );
  }

  template<typename T,
           typename U = T,
           typename = typename std::enable_if<std::is_arithmetic<T>::value>>
  void
  setBranchAddress(T & value,
                   const std::string & branchName,
                   //U default_value = 0)
                   U default_value = 77)
  {
std::cout << "<BranchAddressInitializer::setBranchAddress>:" << std::endl;
std::cout << "branchName = '" << branchName << "'" << std::endl;
std::cout << "ignoreErrors = " << ignoreErrors_ << std::endl;
std::cout << "hasBranchName(branchName) = " << hasBranchName(branchName) << std::endl;
    value = static_cast<T>(default_value);
std::cout << "break.point A.1 reached" << std::endl;
    if(! branchName.empty() && ! (ignoreErrors_ && ! hasBranchName(branchName)))
    {
std::cout << "break.point A.2 reached" << std::endl;
std::cout << "&value = " << &value << std::endl;
      tree_ -> SetBranchAddress(branchName.data(), &value);
      recordBoundBranchName(branchName);
    }
std::cout << "break.point A.3 reached" << std::endl;
  }

  template<typename T,
           typename U = T,
           typename = std::enable_if<std::is_arithmetic<T>::value && std::is_arithmetic<U>::value>>
  void
  setBranchAddress(T * & address,
                   const std::string & branchName,
                   //U default_value = 0)
                   U default_value = 88)
  {
std::cout << "<BranchAddressInitializer::setBranchAddress>:" << std::endl;
std::cout << "branchName = '" << branchName << "'" << std::endl;
std::cout << "ignoreErrors = " << ignoreErrors_ << std::endl;
std::cout << "hasBranchName(branchName) = " << hasBranchName(branchName) << std::endl;
std::cout << "break.point B.1 reached" << std::endl;
    if(lenVar_ > 0)
    {
std::cout << "break.point B.2 reached" << std::endl;
      address = new T[lenVar_];
      std::fill_n(address, lenVar_, static_cast<T>(default_value));
    }
    if(! branchName.empty() && ! (ignoreErrors_ && ! hasBranchName(branchName)))
    {
std::cout << "break.point B.3 reached" << std::endl;
      tree_ -> SetBranchAddress(branchName.data(), address);
      recordBoundBranchName(branchName);
    }
std::cout << "break.point B.4 reached" << std::endl;
  }

  BranchAddressInitializer &
  setLenVar(int lenVar)
  {
    lenVar_ = lenVar;
    return *this;
  }

  std::vector<std::string>
  getBoundBranchNames() const
  {
    return boundBranchNames_;
  }

 protected:
  bool
  hasBranchName(const std::string & branchName)
  {
    if(branchName.empty())
    {
      throw cmsException(this, __func__, __LINE__) << "Cannot search for an empty branch name";
    }
    if(! inputBranchNamesFound_)
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
      inputBranchNamesFound_ = true;
    }
    return std::find(inputBranchNames_.cbegin(), inputBranchNames_.cend(), branchName) != inputBranchNames_.cend();
  }

  void
  recordBoundBranchName(const std::string & branchName)
  {
    if(std::find(boundBranchNames_.cbegin(), boundBranchNames_.cend(), branchName) != boundBranchNames_.cend())
    {
      throw cmsException(this, __func__, __LINE__) << "Branch name '" << branchName << "' already bound";
    }
    boundBranchNames_.push_back(branchName);
  }

  TTree * tree_;
  int lenVar_;
  std::string branchName_n_;
  bool ignoreErrors_;
  bool inputBranchNamesFound_;
  std::vector<std::string> inputBranchNames_;
  std::vector<std::string> boundBranchNames_;
};

#endif // TallinnNtupleProducer_Readers_BranchAddressInitializer_h
