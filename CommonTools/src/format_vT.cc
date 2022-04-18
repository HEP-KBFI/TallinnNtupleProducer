#include "TallinnNtupleProducer/CommonTools/interface/format_vT.h"

#include <sstream> // std::ostringstream

template <typename T>
std::string
format_vT(const std::vector<T> & vT)
{
  std::ostringstream os;
  const unsigned numEntries = vT.size();

  os << "{ ";
  for(unsigned iEntry = 0; iEntry < numEntries; ++iEntry)
  {
    os << vT[iEntry];
    if(iEntry < numEntries - 1)
    {
      os << ", ";
    }
  }
  os << " }";

  return os.str();
}

std::string
format_vstring(const std::vector<std::string> & vs)
{
  return format_vT(vs);
}

std::string
format_vdouble(const std::vector<double> & vd)
{
  return format_vT(vd);
}

std::string
format_vfloat(const std::vector<float> & vf)
{
  return format_vT(vf);
}

std::string
format_vint(const std::vector<int> & vi)
{
  return format_vT(vi);
}

std::string
format_vunsigned(const std::vector<unsigned> & vu)
{
  return format_vT(vu);
}

std::string
format_vbool(const std::vector<bool> & vb)
{
  return format_vT(vb);
}
