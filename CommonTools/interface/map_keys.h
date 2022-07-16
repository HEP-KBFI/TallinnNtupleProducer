#ifndef TallinnNtupleProducer_CommonTools_map_keys_h
#define TallinnNtupleProducer_CommonTools_map_keys_h

#include <boost/range/adaptor/map.hpp> // boost::adaptors::map_keys
#include <boost/range/algorithm/copy.hpp> // boost::copy()

#include <vector> // std::vector<>
#include <map> // std::map<,>

template <typename K,
          typename V>
std::vector<K>
map_keys(const std::map<K, V> & map)
{
  std::vector<K> keys;
  keys.reserve(map.size());
  boost::copy(map | boost::adaptors::map_keys, std::back_inserter(keys));
  return keys;
}

#endif // TallinnNtupleProducer_CommonTools_map_keys_h
