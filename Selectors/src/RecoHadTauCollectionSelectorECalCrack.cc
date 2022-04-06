#include "TallinnNtupleProducer/Selectors/interface/RecoHadTauCollectionSelectorECalCrack.h"

#include "DataFormats/Math/interface/Vector3D.h"                      // math::XYZVector

#include "TallinnNtupleProducer/CommonTools/interface/cmsException.h" // cmsException()
#include "TallinnNtupleProducer/CommonTools/interface/Era.h"          // Era

#include <TMath.h>                                                    // TMath::Cos, TMath::Sin, TMath::Tan

#include <assert.h>                                                   // assert()

RecoHadTauSelectorECalCrack::RecoHadTauSelectorECalCrack(Era era,
                                                         int index,
                                                         bool debug)
  : debug_(debug)
  , era_(era)
  , crack_eta_min_(1.460)
  , crack_eta_max_(1.558)
  , vertex_position_(0., 0., -1.e+3)
{}

void 
RecoHadTauSelectorECalCrack::set_vertex(const RecoVertex& vertex)
{
  vertex_position_ = vertex.position();
}

Particle::Point
compPositionAtECalEntrance(const RecoHadTau& hadTau, const Particle::Point& vertex_position)
{
  Particle::Point positionAtECalEntrance;
    
  const double rECal_barrel = 129.0; // radius of ECal barrel
  const double zECal_barrel = 320.9; // lenght of ECal barrel (= position of ECal endcap)
  
  // CV: ignore curvature of track, as we are interested in eta@ECalEntrance only 
  double xAtECalEntrance = rECal_barrel*TMath::Cos(hadTau.phi()); 
  double yAtECalEntrance = rECal_barrel*TMath::Sin(hadTau.phi());
  double zAtECalEntrance = vertex_position.z() + rECal_barrel/TMath::Tan(hadTau.p4().theta());
  if ( TMath::Abs(zAtECalEntrance) < zECal_barrel ) {      
    positionAtECalEntrance = Particle::Point(
      xAtECalEntrance, 
      yAtECalEntrance, 
      zAtECalEntrance);
  } else {
    zAtECalEntrance = 0.;
    if      ( hadTau.eta() > 0. ) zAtECalEntrance = +zECal_barrel;
    else if ( hadTau.eta() < 0. ) zAtECalEntrance = -zECal_barrel;
    else assert(0);
    double rAtECalEntrance = TMath::Tan(hadTau.p4().theta())*(zAtECalEntrance - vertex_position.z());
    // CV: ignore curvature of track, as we are interested in eta@ECalEntrance only 
    xAtECalEntrance = rAtECalEntrance*TMath::Cos(hadTau.phi()); 
    yAtECalEntrance = rAtECalEntrance*TMath::Sin(hadTau.phi());
    positionAtECalEntrance = Particle::Point(
      xAtECalEntrance, 
      yAtECalEntrance, 
      zAtECalEntrance);
  }
  return positionAtECalEntrance;
}

bool
RecoHadTauSelectorECalCrack::operator()(const RecoHadTau & hadTau) const
{
  if(debug_)
  {
    std::cout << get_human_line(this, __func__) << ":\n" << hadTau;
  }

  if ( vertex_position_.z() == -1.e+3 )
    throw cmsException(__func__, __LINE__) << "No vertex position set !!";

  Particle::Point positionAtECalEntrance = compPositionAtECalEntrance(hadTau, vertex_position_);
  double absEta = TMath::Abs(positionAtECalEntrance.eta());
  bool isInECalCrack = (absEta >= crack_eta_min_ && absEta <= crack_eta_max_);
  if( isInECalCrack )
  {
    if(debug_)
    {
      std::cout << "FAILS crack veto\n";
    }
    return false;
  }
  return true;
}
