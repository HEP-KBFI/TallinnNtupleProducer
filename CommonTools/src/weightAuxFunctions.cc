#include "TallinnNtupleProducer/CommonTools/interface/weightAuxFunctions.h"

#include <TString.h> // Form()

#include <algorithm> // std::replace()
#include <cassert>   // assert()
#include <tuple>     // std::make_tuple()

std::string
get_tH_weight_str(double kt,
                  double kv,
                  double cosa)
{
  std::string kt_kv_cosa_str = Form("kt_%.3g_kv_%.6g", kt, kv);
  if(! std::isnan(cosa))
  {
    kt_kv_cosa_str += Form("_cosa_%.4g", cosa);
  }
  std::replace(kt_kv_cosa_str.begin(), kt_kv_cosa_str.end(), '.', 'p');
  std::replace(kt_kv_cosa_str.begin(), kt_kv_cosa_str.end(), '-', 'm');
  return kt_kv_cosa_str;
}

std::string
get_tH_SM_str()
{
  return get_tH_weight_str(1.0, 1.0, std::numeric_limits<double>::quiet_NaN());
}

std::tuple<double, double, double>
get_tH_params(const std::string & kt_kv_cosa_str)
{
  std::string kt_kv_cosa_str_copy = kt_kv_cosa_str;
  std::replace(kt_kv_cosa_str_copy.begin(), kt_kv_cosa_str_copy.end(),   'p', '.');
  std::replace(kt_kv_cosa_str_copy.begin(), kt_kv_cosa_str_copy.end(),   'm', '-');

  std::string kt_str = kt_kv_cosa_str_copy.substr(kt_kv_cosa_str_copy.find("kt_") + 3, kt_kv_cosa_str_copy.find("kv_") - 4);
  const int cosa_idx = kt_kv_cosa_str_copy.find("cosa_");
  const int kv_str_limit = cosa_idx < 0 ? kt_kv_cosa_str_copy.size() : cosa_idx - 9;
  std::string kv_str = kt_kv_cosa_str_copy.substr(kt_kv_cosa_str_copy.find("kv_") + 3, kv_str_limit);
  std::string cosa_str = cosa_idx < 0 ? "" : kt_kv_cosa_str_copy.substr(cosa_idx + 5);

  const double kt = std::stod(kt_str);
  const double kv = std::stod(kv_str);
  const double cosa = ! cosa_str.empty() ? std::stod(cosa_str) : std::numeric_limits<double>::quiet_NaN();

  // make sure that we can recover the encoding string
  const std::string kt_kv_cosa_str_re = get_tH_weight_str(kt, kv, cosa);
  assert(kt_kv_cosa_str == kt_kv_cosa_str_re);

  return std::make_tuple(kt, kv, cosa);
}
