#ifndef TallinnNtupleProducer_CommonTools_merge_systematic_shifts_h
#define TallinnNtupleProducer_CommonTools_merge_systematic_shifts_h

#include <string> // std::string
#include <vector> // std::vector

void
merge_systematic_shifts(std::vector<std::string> & systematic_shifts,
                        const std::vector<std::string> & to_add);

#endif // TallinnNtupleProducer_CommonTools_merge_systematic_shifts_h
