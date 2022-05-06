#ifndef TallinnNtupleProducer_CommonTools_pickFirstNobjects_h
#define TallinnNtupleProducer_CommonTools_pickFirstNobjects_h

#include <algorithm> // std::copy_n(), std::min()
#include <iterator>  // std::back_inserter
#include <vector>    // std::vector

/**
 * @brief Return first N objects from collection given as function argument. In case the input
 *        collection contains fewer than N objects, the whole input collection is returned
 */
template <typename T>
std::vector<T>
pickFirstNobjects(const std::vector<T> & objects_input,
                  std::size_t N)
{
  const std::size_t N_input = std::min(objects_input.size(), N);
  std::vector<T> objects_output;
  std::copy_n(objects_input.begin(), N_input, std::back_inserter(objects_output));
  return objects_output;
}

#endif // TallinnNtupleProducer_CommonTools_pickFirstNobjects_h
