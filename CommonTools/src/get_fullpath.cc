#include "TallinnNtupleProducer/CommonTools/interface/get_fullpath.h"

#include "TallinnNtupleProducer/CommonTools/interface/LocalFileInPath.h" // LocalFileInPath

std::string
get_fullpath(const std::string & path)
{
  return LocalFileInPath(path).fullPath();
}
