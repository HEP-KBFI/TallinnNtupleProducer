#ifndef TallinnNtupleProducer_CommonTools_memory_logger_h
#define TallinnNtupleProducer_CommonTools_memory_logger_h

#include <string> // std::string
#include <utility> // std::pair<,>

std::string
display_memory(int value_in_bytes,
               bool use_si_units = true);

/**
 * @brief Logs memory
 * @return Pair of VSIZE and RSS (in this order)
 */
std::pair<int, int>
log_memory();

#endif // TallinnNtupleProducer_CommonTools_memory_logger_h
