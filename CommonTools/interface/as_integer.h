#ifndef TallinnNtupleProducer_CommonTools_as_integer_h
#define TallinnNtupleProducer_CommonTools_as_integer_h

#include <type_traits> // std::underlying_type<>

/**
 * @brief Converts enum class value to corresponding integer value
 *        which is determined by the order in which the enums are declared
 * @param value Enum class value
 * @return Corresponding integer value
 *
 * Taken from: https://stackoverflow.com/a/11421471
 */
template <typename Enumeration>
auto as_integer(Enumeration const value)
  -> typename std::underlying_type<Enumeration>::type
{
  return static_cast<typename std::underlying_type<Enumeration>::type>(value);
}

#endif // TallinnNtupleProducer_CommonTools_as_integer_h
