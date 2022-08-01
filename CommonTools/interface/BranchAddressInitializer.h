#ifndef TallinnNtupleProducer_CommonTools_BranchAddressInitializer_h
#define TallinnNtupleProducer_CommonTools_BranchAddressInitializer_h

/** \class BranchAddressInitializer
 *
 * This class provides an interface for reading information from and writing information to a TTree.
 * The information is written to branches, the name of which are passed as arguments to the 'setBranch' and 'setBranchAddress' functions.
 * The function 'setBranchAddress' is to be used for reading and the function 'setBranch' for writing.
 * The functions handle the reading and writing of simple variables (Bool_t, Double_t, Float_t, Int_t, UInt_t),
 * as well as of (C-type) vectors of variables (Bool_t[N], Double_t[N], Float_t[N], Int_t[N], UInt_t[N]).
 * In the latter case, the length of the vector N is specified by the lenVar argument passed to the 'setBranch' and 'setBranchAddress' functions.
 * The code checks that the branch read exists in the input tree.
 * It also checks that branches read from the input tree or written to the output tree are not configured for reading/writing more than once.
 * This check is necessary, because ROOT cannot handle the case of multiple memory addresses "bound" to the same branch and will fail silently in that case,
 * i.e. the information read from the input tree or the information written to the output tree will be incorrect in case branches with the same name
 * are "bound" to the input/output tree more than once.
 *
 * \authors Karl Ehataht and Christian Veelken, Tallinn
 *
 */

#include "TallinnNtupleProducer/CommonTools/interface/TypeTraits.h"   // Traits<>

#include <TTree.h>                                                    // TTree

#include <algorithm>                                                  // std::sort(), std::set_union(), std::fill_n()
#include <iostream>                                                   // std::cout
#include <set>                                                        // std::set
#include <string>                                                     // std::string
#include <type_traits>                                                // std::enable_if, std::is_arithmetic
#include <vector>                                                     // std::vector

class BranchAddressInitializer
{
 public:
  BranchAddressInitializer(TTree * tree = nullptr,
                           int lenVar = -1,
                           const std::string & branchName_n = "");

  ~BranchAddressInitializer();

  void
  ignoreErrors(bool flag);

  template<typename T,
           typename = std::enable_if<std::is_arithmetic<T>::value>>
  void
  setBranch(T & value,
            const std::string & branchName)
  {
    if ( std::string(Traits<T>::TYPE_NAME) != "" )
    {
      tree_ -> Branch(branchName.data(), &value, Form("%s/%s", branchName.data(), Traits<T>::TYPE_NAME));
    }
    else
    {
      tree_ -> Branch(branchName.data(), &value);
    }
    recordBoundBranchName_write(branchName);
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
    recordBoundBranchName_write(branchName);
  }

  template<typename T,
           typename U = T,
           typename = typename std::enable_if<std::is_arithmetic<T>::value>>
  void
  setBranchAddress(T & value,
                   const std::string & branchName,
                   U default_value = 0)
  {
    value = static_cast<T>(default_value);
    if(! branchName.empty() && ! (ignoreErrors_ && ! hasBranchName(branchName)))
    {
      tree_ -> SetBranchAddress(branchName.data(), &value);
      recordBoundBranchName_read(branchName);
    }
  }

  template<typename T,
           typename U = T,
           typename = std::enable_if<std::is_arithmetic<T>::value && std::is_arithmetic<U>::value>>
  void
  setBranchAddress(T * & address,
                   const std::string & branchName,
                   U default_value = 0)
  {
    if(lenVar_ > 0)
    {
      address = new T[lenVar_];
      std::fill_n(address, lenVar_, static_cast<T>(default_value));
    }
    if(! branchName.empty() && ! (ignoreErrors_ && ! hasBranchName(branchName)))
    {
      tree_ -> SetBranchAddress(branchName.data(), address);
      recordBoundBranchName_read(branchName);
    }
  }

  BranchAddressInitializer &
  setLenVar(int lenVar);

  std::vector<std::string>
  getBoundBranchNames_read() const;

  std::vector<std::string>
  getBoundBranchNames_write() const;

  static
  void
  print(std::ostream & stream);

  static void
  reset_inputs();

 protected:
  bool
  hasBranchName(const std::string & branchName);

  void
  recordBoundBranchName_read(const std::string & branchName);

  void
  recordBoundBranchName_write(const std::string & branchName);

  TTree * tree_;
  int lenVar_;
  std::string branchName_n_;
  bool ignoreErrors_;
  bool inputBranchNames_initialized_;
  std::vector<std::string> inputBranchNames_;
  std::vector<std::string> boundBranchNames_read_;       // branchnames read from input tree (just for this BranchAddressInitializer instance)
  static std::set<std::string> gBoundBranchNames_read_;  // branchnames read from input tree (for all BranchAddressInitializer instances)
  std::vector<std::string> boundBranchNames_write_;      // branchnames written to output tree (just for this BranchAddressInitializer instance)
  static std::set<std::string> gBoundBranchNames_write_; // branchnames written to output tree (for all BranchAddressInitializer instances)
};

#endif // TallinnNtupleProducer_CommonTools_BranchAddressInitializer_h
