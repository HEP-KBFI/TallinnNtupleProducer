#include "TallinnNtupleProducer/CommonTools/interface/getTArrayDfromVector.h"

TArrayD
getTArrayDfromVector(const std::vector<double> & histogramBinning)
{
  TArrayD binning_tarray(histogramBinning.size());
  for(std::size_t idxBin = 0; idxBin < histogramBinning.size(); ++idxBin)
  {
    binning_tarray[idxBin] = histogramBinning[idxBin];
  }
  return binning_tarray;
}
