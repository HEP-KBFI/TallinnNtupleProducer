#ifndef TallinnNtupleProducer_CommonTools_weightAuxFunctions_h
#define TallinnNtupleProducer_CommonTools_weightAuxFunctions_h

#include <string>  // std::string
#include <utility> // std::pair

std::string
get_tH_weight_str(double kt,
                  double kv,
                  double cosa);

std::string
get_tH_SM_str();

std::tuple<double, double, double>
get_tH_params(const std::string & kt_kv_cosa_str);

#endif
