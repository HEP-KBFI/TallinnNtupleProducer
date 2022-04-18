#ifndef TallinnNtupleProducer_CommonTools_to_string_with_precision_h
#define TallinnNtupleProducer_CommonTools_to_string_with_precision_h

#include <iomanip> // std::fixed
#include <sstream> // std::ostringstream

template <typename T>
std::string
to_string_with_precision(const T a_value, const int n = 2)
{
  std::ostringstream out;
  out.precision(n);
  out << std::fixed << a_value;
  return out.str();
}

#endif // TallinnNtupleProducer_CommonTools_to_string_with_precision_h
