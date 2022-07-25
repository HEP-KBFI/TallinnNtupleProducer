#include "TallinnNtupleProducer/Objects/interface/RecoJetAK8.h"

RecoJetAK8::RecoJetAK8(const GenJet & jet,
                       Float_t msoftdrop,
                       const RecoSubjetAK8* subJet1,
                       const RecoSubjetAK8* subJet2,
                       Float_t tau1,
                       Float_t tau2,
                       Float_t tau3,
                       Float_t tau4,
                       Int_t jetId,
                       Float_t rawFactor,
                       Float_t area,
                       UInt_t idx)
  : RecoJetBase(jet, rawFactor, idx)
  , msoftdrop_(msoftdrop)
  , subJet1_(subJet1)
  , subJet2_(subJet2)
  , tau1_(tau1)
  , tau2_(tau2)
  , tau3_(tau3)
  , tau4_(tau4)
  , jetId_(jetId)
  , area_(area)
{}

RecoJetAK8::~RecoJetAK8()
{}

Float_t
RecoJetAK8::msoftdrop() const
{
  return msoftdrop_;
}

const RecoSubjetAK8*
RecoJetAK8::subJet1() const
{
  return subJet1_.get();
}

const RecoSubjetAK8*
RecoJetAK8::subJet2() const
{
  return subJet2_.get();
}

Float_t
RecoJetAK8::tau1() const
{
  return tau1_;
}

Float_t
RecoJetAK8::tau2() const
{
  return tau2_;
}

Float_t
RecoJetAK8::tau3() const
{
  return tau3_;
}

Float_t
RecoJetAK8::tau4() const
{
  return tau4_;
}

Int_t
RecoJetAK8::jetId() const
{
  return jetId_;
}

Float_t
RecoJetAK8::area() const
{
  return area_;
}

std::ostream &
operator<<(std::ostream & stream,
           const RecoJetAK8 & jet)
{
  stream << static_cast<const RecoJetBase &>(jet)            << ",\n"
            " jet ID = "    << jet.jetId()                   << ","
            " msoftdrop = " << jet.msoftdrop()               << ","
            " tau1 = "      << jet.tau1()                    << ","
            " tau2 = "      << jet.tau2()                    << ","
            " tau3 = "      << jet.tau3()                    << ",\n"
            " subjets:";
  stream << "\n  idx1 = " << jet.subJet1();
  if(jet.subJet1())
  {
    stream << ": " << *(jet.subJet1());
  }
  else
  {
    stream << '\n';
  }
  stream << "  idx2 = " << jet.subJet2();
  if(jet.subJet2())
  {
    stream << ": " << *(jet.subJet2());
  }
  else
  {
    stream << '\n';
  }
  return stream;
}
