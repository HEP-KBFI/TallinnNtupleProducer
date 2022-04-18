#ifndef TallinnNtupleProducer_CommonTools_format_vT_h
#define TallinnNtupleProducer_CommonTools_format_vT_h

#include <string> // std::string
#include <vector> // std::vector

std::string
format_vstring(const std::vector<std::string>&);

std::string
format_vdouble(const std::vector<double>&);

std::string
format_vfloat(const std::vector<float>&);

std::string
format_vint(const std::vector<int>&);

std::string
format_vunsigned(const std::vector<unsigned>&);

std::string
format_vbool(const std::vector<bool>&);

#endif // TallinnNtupleProducer_CommonTools_format_vT_h
