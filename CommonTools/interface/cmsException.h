#ifndef TallinnNtupleProducer_CommonTools_cmsException_h
#define TallinnNtupleProducer_CommonTools_cmsException_h

#include "FWCore/Utilities/interface/Exception.h" // cms::Exception

#include <cxxabi.h>                               // abi::__cxa_demangle()
#include <type_traits>                            // std::enable_if<>, std::is_null_pointer

template <typename T,
          typename = typename std::enable_if<! std::is_null_pointer<T>::value>>
std::string
get_human_line(const T * const this_,
               const std::string & func = "",
               int line = -1)
{
  return
    std::string(abi::__cxa_demangle(typeid(T).name(), nullptr, nullptr, nullptr)) +
      (func.empty() ? "" : "::" + func)                                           +
      (line >= 0 ? ":" + std::to_string(line) : "")                               +
      " "
  ;
}

template <typename T,
          typename = typename std::enable_if<! std::is_null_pointer<T>::value>>
cms::Exception
cmsException(const T * const this_,
             const std::string & func = "",
             int line = -1)
{
  return cms::Exception(get_human_line(this_, func, line));
}

cms::Exception
cmsException(const std::string & func,
             int line = -1);

#endif // TallinnNtupleProducer_CommonTools_cmsException_h
