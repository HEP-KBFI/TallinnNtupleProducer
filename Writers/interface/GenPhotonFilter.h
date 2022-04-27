#ifndef TallinnNtupleProducer_Writers_GenPhotonFilter_h
#define TallinnNtupleProducer_Writers_GenPhotonFilter_h

#include "TallinnNtupleProducer/Objects/interface/GenPhoton.h" // GenPhoton

/*
  Resolve overlap between MC events contained in W+jets samples with either ISR or FSR
  and MC events contained in W+gamma sample, as discussed in this Hypernews thread:
    https://hypernews.cern.ch/HyperNews/CMS/get/generators/4947.html
  When processing the W+jets sample, set the mode parameter to kInverted,
  when processing the W+gamma sample, set the mode parameter to kEnabled,
  and when processing any other MC sample (or data), set it to kDisabled.
  The return value is true (false) if the event passes (fails) the filter.
*/

class GenPhotonFilter
{
 public:
  enum Mode { kDisabled, kEnabled, kInverted };

  GenPhotonFilter(const std::string & mode,
                  double minPt = 20.,
                  double maxAbsEta = 2.6,
                  double minDeltaR = 0.06);
  ~GenPhotonFilter();

  bool
  operator()(const std::vector<GenPhoton> & genPhotons,
             const std::vector<GenPhoton> & genProxyPhotons,
             const std::vector<GenParticle> & genFromHardProcess) const;

 private:
  int
  getNumPassingPhotons(const std::vector<GenPhoton> & genPhotons,
                       const std::vector<GenParticle> & genFromHardProcess) const;

  Mode mode_;

  double minPt_;
  double maxAbsEta_;
  double minDeltaR_;
};

#endif // TallinnNtupleProducer_Writers_GenPhotonFilter_h

