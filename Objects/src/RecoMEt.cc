#include "TallinnNtupleProducer/Objects/interface/RecoMEt.h"

RecoMEt::RecoMEt()
  : RecoMEt({0., 0.}, 0., 0., 0., 0., 0., 0.)
{}

RecoMEt::RecoMEt(const GenMEt & genMEt,
                 Float_t sumEt,
                 Float_t unclEnDeltaX,
                 Float_t unclEnDeltaY,
                 Float_t covXX,
                 Float_t covXY,
                 Float_t covYY)
  : GenMEt(genMEt)
  , sumEt_(sumEt)
  , unclEnDeltaX_(unclEnDeltaX)
  , unclEnDeltaY_(unclEnDeltaY)
  , covXX_(covXX)
  , covXY_(covXY)
  , covYY_(covYY)
  , cov_(2,2)
{
  cov_(0,0) = static_cast<Double_t>(covXX_);
  cov_(0,1) = static_cast<Double_t>(covXY_);
  cov_(1,0) = static_cast<Double_t>(covXY_);
  cov_(1,1) = static_cast<Double_t>(covYY_);
}

RecoMEt &
RecoMEt::operator=(const RecoMEt & other)
{
  GenMEt::operator=(other);
  unclEnDeltaX_ = other.unclEnDeltaX_;
  unclEnDeltaY_ = other.unclEnDeltaY_;
  sumEt_ = other.sumEt_;
  covXX_ = other.covXX_;
  covXY_ = other.covXY_;
  covYY_ = other.covYY_;
  cov_ = other.cov_;
  return *this;
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

const TMatrixD &
RecoMEt::cov() const
{
  return cov_;
}

std::ostream &
operator<<(std::ostream & stream,
           const RecoMEt & met)
{
  stream
    << static_cast<const GenMEt &>(met)
    << "\n cov:\n"
  ;
  met.cov().Print();
  return stream;
}
