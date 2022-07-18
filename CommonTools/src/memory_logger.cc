#include "TallinnNtupleProducer/CommonTools/interface/memory_logger.h"

#include <vector> // std::vector<>
#include <sstream> // std::stringstream
#include <ios> // std::ios::fixed
#include <fstream> // std::ifstream

#include <unistd.h> // sysconf(), _SC_PAGESIZE

std::string
display_memory(int value_in_bytes,
               bool use_si_units)
{
  const static std::vector<std::string> units_si     = { "B", "KB",  "MB",  "GB"  };
  const static std::vector<std::string> units_binary = { "B", "KiB", "MiB", "GiB" };

  double converted_value = value_in_bytes;
  const double division_unit = use_si_units ? 1000. : 1024;
  const std::vector<std::string> & units = use_si_units ? units_si : units_binary;
  unsigned int order = 0;
  while(converted_value > division_unit && order < (units.size() - 1))
  {
    ++order;
    converted_value /= division_unit;
  }

  std::stringstream ss;
  ss.setf(std::ios::fixed);
  ss.precision(2);
  ss << converted_value << ' ' << units.at(order);
  return ss.str();
}

std::pair<int, int>
log_memory()
{
  unsigned long long vsize = 0;
  unsigned long long rss = 0;
  // see http://man7.org/linux/man-pages/man5/proc.5.html
  std::ifstream buffer("/proc/self/statm");
  buffer >> vsize >> rss;
  // sizes are given in number of pages -> multiply by the page size in bytes
  const long page_size = sysconf(_SC_PAGESIZE);
  vsize *= page_size;
  rss *= page_size;
  return std::make_pair(vsize, rss);
}
