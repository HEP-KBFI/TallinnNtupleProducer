#include "TallinnNtupleProducer/Objects/interface/RecoMEt.h"

#include "TallinnNtupleProducer/CommonTools/interface/cmsException.h" // cmsException()

#define _USE_MATH_DEFINES // M_PI

namespace {
  double constexpr
  pow2(double x)
  {
    return x * x;
  }
}

RecoMEt::RecoMEt()
  : RecoMEt(0., 0., 0., 0., 0.)
{}

RecoMEt::RecoMEt(Float_t pt,
                 Float_t phi,
                 Float_t covXX,
                 Float_t covXY,
                 Float_t covYY)
  : default_(pt, phi)
  , default_systematics_(-1)
  , covXX_(covXX)
  , covXY_(covXY)
  , covYY_(covYY)
  , p4_{default_.pt_, 0., default_.phi_, 0.}
  , sumEt_(0.)
  , cov_(2,2)
  , genPt_(0.)
  , genPhi_(0.)
{
  update_cov();
}

RecoMEt::RecoMEt(const math::PtEtaPhiMLorentzVector & p4,
                 const TMatrixD& cov)
  : default_(p4.pt(), p4.phi())
  , default_systematics_(-1)
  , covXX_(static_cast<Float_t>(cov(0,0)))
  , covXY_(static_cast<Float_t>(cov(0,1)))
  , covYY_(static_cast<Float_t>(cov(1,1)))
  , p4_(p4)
  , sumEt_(0.)
  , cov_(cov)
  , genPt_(0.)
  , genPhi_(0.)
{}

RecoMEt &
RecoMEt::operator=(const RecoMEt & other)
{
  default_ = other.default_;
  default_systematics_ = other.default_systematics_;
  for(const auto & kv: other.systematics_)
  {
    systematics_[kv.first] = kv.second;
  }
  covXX_ = other.covXX_;
  covXY_ = other.covXY_;
  covYY_ = other.covYY_;
  sumEt_ = other.sumEt_;
  update();
  return *this;
}

void
RecoMEt::update()
{
  update_p4();
  update_cov();
}

void
RecoMEt::update_p4()
{
  p4_ = math::PtEtaPhiMLorentzVector(default_.pt_, 0., default_.phi_, 0.);
}

void
RecoMEt::update_cov()
{
  cov_(0,0) = static_cast<Double_t>(covXX_);
  cov_(0,1) = static_cast<Double_t>(covXY_);
  cov_(1,0) = static_cast<Double_t>(covXY_);
  cov_(1,1) = static_cast<Double_t>(covYY_);
}

Double_t
RecoMEt::pt() const
{
  return static_cast<Double_t>(default_.pt_);
}

Double_t
RecoMEt::phi() const
{
  return static_cast<Double_t>(default_.phi_);
}

Double_t 
RecoMEt::sumEt() const
{
  return static_cast<Double_t>(sumEt_);
}
Double_t
RecoMEt::covXX() const
{
  return static_cast<Double_t>(covXX_);
}

Double_t
RecoMEt::covXY() const
{
  return static_cast<Double_t>(covXY_);
}

Double_t
RecoMEt::covYY() const
{
  return static_cast<Double_t>(covYY_);
}

const Particle::LorentzVector &
RecoMEt::p4() const
{
  return p4_;
}

const TMatrixD &
RecoMEt::cov() const
{
  return cov_;
}

void 
RecoMEt::set_genPt(double genPt)
{
  genPt_ = genPt;
}

void 
RecoMEt::set_genPhi(double genPhi)
{
  genPhi_ = genPhi;
}

Double_t 
RecoMEt::genPt() const
{
  return genPt_;
}

Double_t 
RecoMEt::genPhi() const
{
  return genPhi_;
}

void
RecoMEt::set_default(int central_or_shift)
{
std::cout << "<RecoMEt::set_default>:" << std::endl;
std::cout << "central_or_shift = " << central_or_shift << std::endl;
  default_systematics_ = central_or_shift;
for ( const auto & systematic : systematics_ )
{
std::cout << "systematics_[" << systematic.first << "] = " << systematic.second << std::endl;
}
std::cout << "default_systematics_ = " << default_systematics_ << std::endl;
  default_ = systematics_.at(default_systematics_);
std::cout << "break-point F.1 reached" << std::endl;
  update();
std::cout << "break-point F.2 reached" << std::endl;
}

bool
RecoMEt::has_systematics(int central_or_shift) const
{
  return systematics_.count(central_or_shift);
}

void
RecoMEt::set_systematics(const RecoMEt::MEt & met,
                         int central_or_shift,
                         bool replace)
{
  if(! replace && systematics_.count(central_or_shift))
  {
    throw cmsException(this, __func__, __LINE__) << "Unable to set MEt for option " << central_or_shift;
  }
  systematics_[central_or_shift] = met;
}

RecoMEt::MEt
RecoMEt::get_systematics(int central_or_shift) const
{
  if(! systematics_.count(central_or_shift))
  {
    throw cmsException(this, __func__, __LINE__) << "Unable to retrive MEt for option " << central_or_shift;
  }
  return systematics_.at(central_or_shift);
}

int
RecoMEt::get_default_systematics() const
{
  return default_systematics_;
}

void
RecoMEt::shift_PxPy(const std::pair<double, double> & PxPyCorr)
{
  for(auto & kv: systematics_)
  {
    kv.second.shift_PxPy(PxPyCorr);
  }
  default_.shift_PxPy(PxPyCorr);
}

std::ostream &
operator<<(std::ostream & stream,
           const RecoMEt & met)
{
  stream << " pT = " << met.pt() << ","
            " phi = " << met.phi() << "\n"
            " cov:\n";
  met.cov().Print();
  return stream;
}

RecoMEt::MEt::MEt(Float_t pt, Float_t phi)
  : pt_(pt)
  , phi_(phi)
{}

RecoMEt::MEt::MEt()
  : pt_(0.)
  , phi_(0.)
{}

RecoMEt::MEt &
RecoMEt::MEt::operator=(const RecoMEt::MEt & other)
{
  pt_ = other.pt_;
  phi_ = other.phi_;
  return *this;
}

double
RecoMEt::MEt::px() const
{
  return pt_ * std::cos(phi_);
}

double
RecoMEt::MEt::py() const
{
  return pt_ * std::sin(phi_);
}

void
RecoMEt::MEt::shift_PxPy(const std::pair<double, double> & PxPyCorr)
{
  const double shifted_x = this->px() + PxPyCorr.first;
  const double shifted_y = this->py() + PxPyCorr.second;

  pt_ = std::sqrt(::pow2(shifted_x) + ::pow2(shifted_y));
  phi_ = 0.;
  if     (shifted_x > 0) { phi_ = std::atan(shifted_y / shifted_x); }
  else if(shifted_x < 0) { phi_ = std::atan(shifted_y / shifted_x) + ((shifted_y > 0. ? +1. : -1.) * M_PI);  }
  else                   { phi_ = (shifted_y > 0. ? +1. : -1.) * M_PI; }
}

std::ostream &
operator<<(std::ostream & stream,
           const RecoMEt::MEt & met)
{
  stream << "pT = " << met.pt_ << " phi = " << met.phi_;
  return stream;
}
