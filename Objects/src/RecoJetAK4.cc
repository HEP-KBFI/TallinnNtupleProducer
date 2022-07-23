#include "TallinnNtupleProducer/Objects/interface/RecoJetAK4.h"

#include "TallinnNtupleProducer/CommonTools/interface/as_integer.h"     // as_integer()
#include "TallinnNtupleProducer/CommonTools/interface/cmsException.h"   // cmsException()
#include "TallinnNtupleProducer/CommonTools/interface/jetDefinitions.h" // Btag, pileupJetID

RecoJetAK4::RecoJetAK4(const GenJet & jet,
                       Double_t BtagCSV,
                       Double_t QGDiscr,
                       Double_t bRegCorr,
                       Double_t bRegRes,
                       Int_t jetId,
                       Int_t puId,
                       Float_t rawFactor,
                       Float_t neEmEF,
                       Float_t chEmEF,
                       Float_t muonSubtrFactor,
                       UInt_t idx,
                       Int_t genJetIdx,
                       Btag btag,
                       Int_t central_or_shift)
: RecoJetBase(jet, idx)
, BtagCSV_(BtagCSV)
, QGDiscr_(QGDiscr)
, bRegCorr_(bRegCorr)
, bRegRes_(bRegRes)
, jetId_(jetId)
, puId_(puId)
, rawFactor_(rawFactor)
, neEmEF_(neEmEF)
, chEmEF_(chEmEF)
, muonSubtrFactor_(muonSubtrFactor)
, genJetIdx_(genJetIdx)
, btag_(btag)
, isBJet_loose_(false)
, isBJet_medium_(false)
{}

RecoJetAK4::~RecoJetAK4()
{}

void
RecoJetAK4::set_isBJet_loose() const
{
  isBJet_loose_ = true;
}

void
RecoJetAK4::set_isBJet_medium() const
{
  isBJet_medium_ = true;
}

Double_t
RecoJetAK4::BtagCSV() const
{
  return BtagCSV_;
}

Double_t
RecoJetAK4::BtagCSV(Btag btag) const
{
  if(! BtagCSV_systematics_.count(btag))
  {
    throw cmsException(this, __func__, __LINE__)
      << "No such b-tagging score available: " << as_integer(btag)
    ;
  }
  return BtagCSV_systematics_.at(btag);
}

Double_t
RecoJetAK4::QGDiscr() const
{
  return QGDiscr_;
}

Double_t
RecoJetAK4::bRegCorr() const
{
  return bRegCorr_;
}

Particle::LorentzVector
RecoJetAK4::p4_bRegCorr() const
{
  // scale both pT and mass, so that the pT and energy are scaled by the same amount
  // cf. https://twiki.cern.ch/twiki/bin/viewauth/CMS/HiggsWG/BJetRegression
  //     ("To get corrected jet one has to scale the pT and energy with this correction")
  return math::PtEtaPhiMLorentzVector(this->pt()*bRegCorr_, this->eta(), this->phi(), this->mass()*bRegCorr_);
}

Double_t
RecoJetAK4::bRegRes() const
{
  return bRegRes_;
}

Int_t
RecoJetAK4::jetId() const
{
  return jetId_;
}

Int_t
RecoJetAK4::puId() const
{
  return puId_;
}

Float_t
RecoJetAK4::rawFactor() const
{
  return rawFactor_;
}

Float_t
RecoJetAK4::neEmEF() const
{
  return neEmEF_;
}

Float_t
RecoJetAK4::chEmEF() const
{
  return chEmEF_;
}

Float_t
RecoJetAK4::muonSubtrFactor() const
{
  return muonSubtrFactor_;
}

Int_t
RecoJetAK4::genJetIdx() const
{
  return genJetIdx_;
}

bool
RecoJetAK4::passesPUID(pileupJetID puIdWP) const
{
  return this->puId() & static_cast<int>(puIdWP);
}

bool
RecoJetAK4::is_PUID_taggable() const
{
  // PU jet ID is applicable only to jets that have pT < 50. GeV
  // https://twiki.cern.ch/twiki/bin/viewauth/CMS/PileupJetID
  return this->pt() < 50.;
}

bool
RecoJetAK4::is_PU() const
{
  return genJetIdx_ < 0;
}

bool
RecoJetAK4::isBJet_loose() const
{
  return isBJet_loose_;
}

bool
RecoJetAK4::isBJet_medium() const
{
  return isBJet_medium_;
}

std::ostream &
operator<<(std::ostream & stream,
           const RecoJetAK4 & jet)
{
  stream << static_cast<const RecoJetBase &>(jet)                            << ",\n"
            " genJetIdx = "       << jet.genJetIdx()                         << ","
            " btag score = "      << jet.BtagCSV()                           << ","
            " jet ID = "          << jet.jetId()                             << ","
            " PU ID = "           << jet.puId()                              << ","
            " rawFactor = "       << jet.rawFactor()                         << ","
            " neEmEF = "          << jet.neEmEF()                            << ","
            " chEmEF = "          << jet.chEmEF()                            << ","
            " muonSubtrFactor = " << jet.muonSubtrFactor()                   << ","
            " QGL = "             << jet.QGDiscr()                           << ","
            " bReg corr (res) = " << jet.bRegCorr() << " (" << jet.bRegRes() << ")\n"
  ;
  return stream;
}
