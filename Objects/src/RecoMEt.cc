#include "TallinnNtupleProducer/Objects/interface/RecoMEt.h"

#include "TallinnNtupleProducer/CommonTools/interface/cmsException.h" // cmsException()

#define _USE_MATH_DEFINES // M_PI

namespace 
{
  double constexpr
  pow2(double x)
  {
    return x * x;
  }
}

RecoMEt::RecoMEt()
  : RecoMEt(0., 0., 0., 0., 0., 0., 0., 0.)
{}

RecoMEt::RecoMEt(Float_t pt,
                 Float_t phi,
                 Float_t sumEt,
                 Float_t unclEnDeltaX,
                 Float_t unclEnDeltaY,
                 Float_t covXX,
                 Float_t covXY,
                 Float_t covYY)
  : pt_(pt)
  , phi_(phi)
  , sumEt_(sumEt)
  , unclEnDeltaX_(unclEnDeltaX)
  , unclEnDeltaY_(unclEnDeltaY)
  , covXX_(covXX)
  , covXY_(covXY)
  , covYY_(covYY)
  , p4_({ pt_, 0., phi_, 0. })
  , cov_(2,2)
  , genPt_(0.)
  , genPhi_(0.)
{
  cov_(0,0) = static_cast<Double_t>(covXX_);
  cov_(0,1) = static_cast<Double_t>(covXY_);
  cov_(1,0) = static_cast<Double_t>(covXY_);
  cov_(1,1) = static_cast<Double_t>(covYY_);
}

RecoMEt::RecoMEt(const math::PtEtaPhiMLorentzVector & p4,
                 Float_t sumEt,
                 Float_t unclEnDeltaX,
                 Float_t unclEnDeltaY,
                 const TMatrixD& cov)
  : pt_(p4.pt())
  , phi_(p4.phi())
  , sumEt_(sumEt)
  , unclEnDeltaX_(unclEnDeltaX)
  , unclEnDeltaY_(unclEnDeltaY)
  , covXX_(static_cast<Float_t>(cov(0,0)))
  , covXY_(static_cast<Float_t>(cov(0,1)))
  , covYY_(static_cast<Float_t>(cov(1,1)))
  , p4_(p4)
  , cov_(cov)
  , genPt_(0.)
  , genPhi_(0.)
{}

RecoMEt &
RecoMEt::operator=(const RecoMEt & other)
{
  pt_ = other.pt_;
  phi_ = other.phi_;
  unclEnDeltaX_ = other.unclEnDeltaX_;
  unclEnDeltaY_ = other.unclEnDeltaY_;
  sumEt_ = other.sumEt_;
  covXX_ = other.covXX_;
  covXY_ = other.covXY_;
  covYY_ = other.covYY_;
  p4_ = other.p4_;
  cov_ = other.cov_;
  return *this;
}

Double_t
RecoMEt::pt() const
{
  return static_cast<Double_t>(pt_);
}

Double_t
RecoMEt::phi() const
{
  return static_cast<Double_t>(phi_);
}

Double_t 
RecoMEt::sumEt() const
{
  return static_cast<Double_t>(sumEt_);
}

Double_t
RecoMEt::unclEnDeltaX() const
{
  return unclEnDeltaX_;
}
Double_t
RecoMEt::unclEnDeltaY() const
{
  return unclEnDeltaY_;
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
RecoMEt::set(Double_t pt,
             Double_t phi)
{
  pt_ = pt;
  phi_ = phi;
  p4_ = { pt_, 0., phi_, 0. };
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
