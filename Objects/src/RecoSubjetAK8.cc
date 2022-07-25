#include "TallinnNtupleProducer/Objects/interface/RecoSubjetAK8.h"

RecoSubjetAK8::RecoSubjetAK8(const GenJet & jet,
                             Float_t rawFactor,
                             Float_t BtagCSV,
                             Int_t idx)
  : RecoJetBase(jet, rawFactor, idx)
  , BtagCSV_(BtagCSV)
{}

RecoSubjetAK8::~RecoSubjetAK8()
{}

Float_t
RecoSubjetAK8::BtagCSV() const
{
  return BtagCSV_;
}

bool
RecoSubjetAK8::is_btaggable() const
{
  return pt_ >= 30.;
}

std::ostream &
operator<<(std::ostream & stream,
           const RecoSubjetAK8 & jet)
{
  stream << static_cast<const RecoJetBase &>(jet) << ","
            " CSV = "    << jet.BtagCSV()    << ",\n";
  return stream;
}
