#include "TallinnNtupleProducer/CommonTools/interface/dumpHistogram.h"

#include "TallinnNtupleProducer/CommonTools/interface/compIntegral.h" // compIntegral(), compIntegralErr()

#include <TAxis.h>                                                    // TAxis
#include <TH1.h>                                                      // TH1

#include <assert.h>                                                   // assert()
#include <iostream>                                                   // std::cout

void
dumpHistogram(const TH1 * histogram)
{
  double integral = compIntegral(histogram, false, false);
  double integralErr = compIntegralErr(histogram, false, false);
  std::cout << "<dumpHistogram>:\n"
               "histogram = '" << histogram->GetName() << "'\n"
               "integral = " << integral << " +/- " << integralErr << '\n';

  const int firstBinX = 1;
  const int lastBinX = histogram->GetNbinsX();
  const TAxis * const xAxis = histogram->GetXaxis();

  int firstBinY = 1;
  int lastBinY = 1;
  const TAxis * yAxis = nullptr;
  bool isHistogram2d = ( histogram->GetDimension() == 2 ) ? true : false;
  if ( isHistogram2d ) 
  {
    firstBinY = 1;
    lastBinY = histogram->GetNbinsY();
    yAxis = histogram->GetYaxis();
  }

  for(int idxBinX = firstBinX; idxBinX <= lastBinX; ++idxBinX)
  {
    for(int idxBinY = firstBinY; idxBinY <= lastBinY; ++idxBinY)
    {
      double binContent = 0.;
      double binError = 0.;
      if(isHistogram2d)
      {
        binContent = histogram->GetBinContent(idxBinX, idxBinY);
        binError = histogram->GetBinError(idxBinX, idxBinY);
      }
      else
      {
        assert(idxBinY == firstBinY);
        binContent = histogram->GetBinContent(idxBinX);
        binError = histogram->GetBinError(idxBinX);
      }
      std::cout << "binX #" << idxBinX << " (x =  " << xAxis->GetBinCenter(idxBinX) << ")";
      if(isHistogram2d)
      {
        std::cout << ", binY #" << idxBinY << " (y =  " << yAxis->GetBinCenter(idxBinY) << ")";
      }
      std::cout << ": " 
                   "binContent = " << binContent << " +/- " << binError << '\n';
    }
  }
}
