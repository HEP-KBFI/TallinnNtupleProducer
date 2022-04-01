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

GenMEt::GenMEt(const math::PtEtaPhiMLorentzVector & p4)
  : pt_(p4.pt())
  , phi_(p4.phi())
  , p4_(p4)
{}

GenMEt &
GenMEt::operator=(const GenMEt & other)
{

  pt_ = other.pt_;
  phi_ = other.phi_;
  update();
  return *this;
}

void
GenMEt::update()
{
  update_p4();
}

void
GenMEt::update_p4()
{
  p4_ = math::PtEtaPhiMLorentzVector(pt_, 0., phi_, 0.);
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

std::ostream& operator<<(std::ostream& stream,
                         const GenMEt& met)
{
  stream << " pT = " << met.pt() << ","
    " phi = " << met.phi() << "\n";
  return stream;
}
