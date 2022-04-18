#include "TallinnNtupleProducer/EvtWeightTools/interface/hadTauTriggerTurnOnCurves.h"

#include <cmath>  // std::fabs(), std::exp(), std::pow(), std::erf()
#include <limits> // std::numeric_limits<>

/**
 * @brief Integral of Crystal Ball function for fitting trigger efficiency turn-on curves (code from Pascal Paganini)
 * @param m: pT of reconstructed hadronic tau candidates;
 *           the other parameters refer to the shape of the Crystal Ball function, cf. Section 6.2.3 of AN-2016/027
 * @return efficiency for passing trigger, per hadronic tau leg
 */

double
integralCrystalBall(double m,
                    double m0,
                    double sigma,
                    double alpha,
                    double n,
                    double norm)
{
  const double sqrtPiOver2 = 1.2533141373;
  const double sqrt2 = 1.4142135624;

  const double sig = std::fabs(static_cast<double>(sigma));

  double t = (m - m0) / sig;
  if(alpha < 0)
  {
    t = -t;
  }

  const double absAlpha = std::fabs(alpha / sig);
  const double a = std::pow(n / absAlpha, n) * std::exp(-0.5 * absAlpha * absAlpha);
  const double b = absAlpha - n / absAlpha;

  if(a >= std::numeric_limits<double>::max())
  {
    return -1.;
  }

  double ApproxErf;
  double arg = absAlpha / sqrt2;
  if     (arg >  5.) ApproxErf =  1;
  else if(arg < -5.) ApproxErf = -1;
  else               ApproxErf = std::erf(arg);

  const double leftArea  = (1 + ApproxErf) * sqrtPiOver2;
  const double rightArea = (a / std::pow(absAlpha - b, n - 1)) / (n - 1);
  const double area = leftArea + rightArea;

  if(t <= absAlpha)
  {
    arg = t / sqrt2;
    if     (arg >  5.) ApproxErf =  1;
    else if(arg < -5.) ApproxErf = -1;
    else               ApproxErf = std::erf(arg);
    return norm * (1 + ApproxErf) * sqrtPiOver2 / area;
  }
  else
  {
    return norm * (leftArea +  a * (1 / std::pow(t-b, n - 1) - 1 / std::pow(absAlpha - b, n - 1)) / (1 - n)) / area;
  }
}
