#include "TallinnNtupleProducer/Objects/interface/RecoSubjetAK8.h"

RecoSubjetAK8::RecoSubjetAK8(const GenJet & jet,
                             Double_t BtagCSV,
                             Int_t idx)
  : RecoJetBase(jet, idx)
  , BtagCSV_(BtagCSV)
{}

RecoSubjetAK8::~RecoSubjetAK8()
{}

Double_t
RecoSubjetAK8::BtagCSV() const
{
  return BtagCSV_;
}

std::ostream &
operator<<(std::ostream & stream,
           const RecoSubjetAK8 & jet)
{
  stream << static_cast<const GenJet &>(jet) << ","
            " CSV = "    << jet.BtagCSV()    << ",\n";
  return stream;
}
