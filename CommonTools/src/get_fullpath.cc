#include "TallinnNtupleProducer/CommonTools/interface/get_fullpath.h"

#include <FWCore/Utilities/interface/FileInPath.h> // edm::FileInPath

std::string
get_fullpath(const std::string & path)
{
  return edm::FileInPath(path).fullPath();
}
