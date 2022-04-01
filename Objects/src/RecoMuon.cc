#include "TallinnNtupleProducer/Objects/interface/RecoMuon.h"

RecoMuon::RecoMuon(const RecoLepton & lepton,
                   Bool_t passesLooseIdPOG,
                   Bool_t passesMediumIdPOG,
                   Float_t segmentCompatibility,
                   Float_t ptErr)
  : RecoLepton(lepton)
  , passesLooseIdPOG_(passesLooseIdPOG)
  , passesMediumIdPOG_(passesMediumIdPOG)
  , segmentCompatibility_(segmentCompatibility)
  , ptErr_(ptErr)
{
  set_mvaRawTTH_cut(0.85);
}

Bool_t
RecoMuon::passesLooseIdPOG() const
{
  return passesLooseIdPOG_;
}

Bool_t
RecoMuon::passesMediumIdPOG() const
{
  return passesMediumIdPOG_;
}

Float_t
RecoMuon::segmentCompatibility() const
{
  return segmentCompatibility_;
}

Float_t
RecoMuon::ptErr() const
{
  return ptErr_;
}

Float_t
RecoMuon::dpt_div_pt() const
{
  return ptErr() >= 0. ? ptErr() / pt() : -1.;
}

bool
RecoMuon::is_electron() const
{
  return false;
}

bool
RecoMuon::is_muon() const
{
  return true;
}

Double_t
RecoMuon::cone_pt() const
{
  return passesMediumIdPOG() ? RecoLepton::cone_pt() : assocJet_pt();
}

const Particle::LorentzVector &
RecoMuon::cone_p4() const
{
  return passesMediumIdPOG() ? RecoLepton::cone_p4() : assocJet_p4();
}

std::ostream &
operator<<(std::ostream & stream,
           const RecoMuon & muon)
{
  stream << static_cast<const RecoLepton &>(muon) << ",\n "
            "passesLooseIdPOG = "     << muon.passesLooseIdPOG()  << ", "
            "passesMediumIdPOG = "    << muon.passesMediumIdPOG() << ", "
            "segmentCompatibility = " << muon.segmentCompatibility() << ", "
            "ptErr = "                << muon.ptErr() << '\n'
  ;
  return stream;
}
