#include "TallinnNtupleProducer/CommonTools/interface/compIntegral.h"

#include <TH1.h>    // TH1

#include <assert.h> // assert()
#include <cmath>    // std::sqrt()

namespace
{
  double 
  square(double x)
  {
    return x*x;
  }
}

double
compIntegral(const TH1 * histogram,
             bool includeUnderflowBin,
             bool includeOverflowBin)
{
  const int numBinsX  = histogram->GetNbinsX();
  const int firstBinX = includeUnderflowBin ? 0            : 1;
  const int lastBinX  = includeOverflowBin  ? numBinsX + 1 : numBinsX;

  int firstBinY = 1;
  int lastBinY = 1;
  bool isHistogram2d = ( histogram->GetDimension() == 2 ) ? true : false;
  if ( isHistogram2d ) 
  {
    const int numBinsY  = histogram->GetNbinsY();
    firstBinY = includeUnderflowBin ? 0            : 1;
    lastBinY  = includeOverflowBin  ? numBinsY + 1 : numBinsY;
  }

  double sumBinContent = 0.;
  for ( int idxBinX = firstBinX; idxBinX <= lastBinX; ++idxBinX)
  {
    for ( int idxBinY = firstBinY; idxBinY <= lastBinY; ++idxBinY)
    {
      if (isHistogram2d)
      {
        sumBinContent += histogram->GetBinContent(idxBinX, idxBinY);
      }
      else
      {
        assert(idxBinY == firstBinY);
        sumBinContent += histogram->GetBinContent(idxBinX);
      }
    }
  }
  return sumBinContent;
}

double
compIntegralErr(const TH1 * histogram,
                bool includeUnderflowBin,
                bool includeOverflowBin)
{
  const int numBinsX  = histogram->GetNbinsX();
  const int firstBinX = includeUnderflowBin ? 0            : 1;
  const int lastBinX  = includeOverflowBin  ? numBinsX + 1 : numBinsX;

  int firstBinY = 1;
  int lastBinY = 1;
  bool isHistogram2d = ( histogram->GetDimension() == 2 ) ? true : false;
  if ( isHistogram2d ) 
  {
    const int numBinsY  = histogram->GetNbinsY();
    firstBinY = includeUnderflowBin ? 0            : 1;
    lastBinY  = includeOverflowBin  ? numBinsY + 1 : numBinsY;
  }

  double sumBinErr2 = 0.;
  for ( int idxBinX = firstBinX; idxBinX <= lastBinX; ++idxBinX)
  {
    for ( int idxBinY = firstBinY; idxBinY <= lastBinY; ++idxBinY)
    {
      if (isHistogram2d)
      {
        sumBinErr2 += square(histogram->GetBinError(idxBinX, idxBinY));
      }
      else
      {
        assert(idxBinY == firstBinY);
        sumBinErr2 += square(histogram->GetBinError(idxBinX));
      }
    }
  }
  return std::sqrt(sumBinErr2);
}



