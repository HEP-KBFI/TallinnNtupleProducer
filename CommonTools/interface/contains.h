#ifndef TallinnNtupleProducer_CommonTools_contains_h
#define TallinnNtupleProducer_CommonTools_contains_h

#include <string> // std::string
#include <vector> // std::vector

/**
 * @brief Check if a certain string is contained in a given vector of strings
 */
bool
contains(const std::vector<std::string> & list_of_strings,
         const std::string & keyWord);

#endif // TallinnNtupleProducer_CommonTools_contains_h
