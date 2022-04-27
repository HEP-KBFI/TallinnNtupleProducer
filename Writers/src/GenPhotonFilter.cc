#include "TallinnNtupleProducer/Writers/interface/GenPhotonFilter.h"

#include "TallinnNtupleProducer/CommonTools/interface/cmsException.h" // cmsException()

GenPhotonFilter::GenPhotonFilter(const std::string & mode,
                                 double minPt,
                                 double maxAbsEta,
                                 double minDeltaR)
  : minPt_(minPt)
  , maxAbsEta_(maxAbsEta)
  , minDeltaR_(minDeltaR)
{
  if      ( mode == "disabled" ) mode_ = Mode::kDisabled;
  else if ( mode == "enabled"  ) mode_ = Mode::kEnabled;
  else if ( mode == "inverted" ) mode_ = Mode::kInverted;
  else throw cmsException(this, __func__, __LINE__) << "Invalid Configuration parameter 'mode' = " << mode;
}

GenPhotonFilter::~GenPhotonFilter()
{}

int
GenPhotonFilter::getNumPassingPhotons(const std::vector<GenPhoton> & genPhotons,
                                      const std::vector<GenParticle> & genFromHardProcess) const
{
  int numSelPhotons = 0;
  for(const GenPhoton & genPhoton: genPhotons)
  {
    double deltaR = +1e6;
    for(const GenParticle & genParticle: genFromHardProcess)
    {
      deltaR = std::min(deltaR, genPhoton.deltaR(genParticle));
    }
    // see https://github.com/HEP-KBFI/hh-multilepton/issues/36
    if(genPhoton.checkStatusFlag(StatusFlag::isPrompt) && genPhoton.pt() > minPt_ && genPhoton.absEta() < maxAbsEta_ && deltaR > minDeltaR_)
    {
      ++numSelPhotons;
    }
  }
  return numSelPhotons;
}

bool
GenPhotonFilter::operator()(const std::vector<GenPhoton> & genPhotons,
                            const std::vector<GenPhoton> & genProxyPhotons,
                            const std::vector<GenParticle> & genFromHardProcess) const
{
  if ( mode_ == Mode::kDisabled ) return true;

  const int numSelPhotons =
    getNumPassingPhotons(genPhotons, genFromHardProcess) +
    getNumPassingPhotons(genProxyPhotons, genFromHardProcess)
  ;

  return
    (mode_ == Mode::kEnabled  && numSelPhotons  > 0) ||
    (mode_ == Mode::kInverted && numSelPhotons == 0)
  ;
}
