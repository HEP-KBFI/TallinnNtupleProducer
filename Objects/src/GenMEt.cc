#include "TallinnNtupleProducer/Objects/interface/GenMEt.h"

GenMEt::GenMEt()
  : pt_(0.)
  , phi_(0.)
  , p4_{0., 0., 0., 0.}
{}

GenMEt::GenMEt(Float_t pt,
               Float_t phi)
  : pt_(pt)
  , phi_(phi)
  , p4_{pt, 0., phi, 0.}
{}

GenMEt &
GenMEt::operator=(const GenMEt & other)
{

  set(other.pt_, other.phi_);
  return *this;
}

Double_t
GenMEt::pt() const
{
  return pt_;
}

Double_t
GenMEt::phi() const
{
  return phi_;
}

const Particle::LorentzVector &
GenMEt::p4() const
{
  return p4_;
}

void
GenMEt::set(Double_t pt,
            Double_t phi)
{
  pt_ = pt;
  phi_ = phi;
  p4_ = { pt_, 0., phi_, 0. };
}

std::ostream &
operator<<(std::ostream & stream,
           const GenMEt & met)
{
  stream <<
    " pT = "  << met.pt()  << ","
    " phi = " << met.phi()
  ;
  return stream;
}
