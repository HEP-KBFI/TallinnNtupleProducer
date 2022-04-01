#include "TallinnNtupleProducer/Objects/interface/RecoVertex.h"

RecoVertex::RecoVertex()
  : RecoVertex(0., 0., 0., 0., 0., 0., 0, 0)
{}

RecoVertex::RecoVertex(Double_t x,
                       Double_t y,
                       Double_t z,
                       Double_t ndof,
                       Double_t chi2,
                       Double_t score,
                       Int_t npvs,
                       Int_t npvsGood)
  : position_x_(x)
  , position_y_(y)
  , position_z_(z)
  , ndof_(ndof)
  , chi2_(chi2)
  , score_(score)
  , npvs_(npvs)
  , npvsGood_(npvsGood)
{}

RecoVertex::~RecoVertex()
{}

const Particle::Point
RecoVertex::position() const
{
  return { position_x_, position_y_, position_z_ };
}

Double_t 
RecoVertex::x() const
{
  return position_x_;
}

Double_t 
RecoVertex::y() const
{
  return position_y_;
}

Double_t 
RecoVertex::z() const
{
  return position_z_;
}

Double_t 
RecoVertex::ndof() const
{
  return ndof_;
}

Double_t 
RecoVertex::chi2() const
{
  return chi2_;
}

Double_t 
RecoVertex::score() const
{
  return score_;
}

Int_t 
RecoVertex::npvs() const
{
  return npvs_;
}

Int_t 
RecoVertex::npvsGood() const
{
  return npvsGood_;
}

std::ostream &
operator<<(std::ostream & stream,
           const RecoVertex & vertex)
{
  stream << "position: x = " << vertex.x() << ", y = " << vertex.y() << ", z = " << vertex.z() << "\n";
  stream << "nDoF = " << vertex.ndof() << ", chi^2 = " << vertex.chi2() << ", sum(pT^2) = " << vertex.score() << "\n";
  stream << "(#vertices = " << vertex.npvs() << ", #good vertices = " << vertex.npvsGood() << ")\n";
  return stream;
}
