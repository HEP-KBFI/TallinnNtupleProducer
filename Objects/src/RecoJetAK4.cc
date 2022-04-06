#include "TallinnNtupleProducer/Objects/interface/RecoJetAK4.h"

#include "TallinnNtupleProducer/CommonTools/interface/as_integer.h"     // as_integer()
#include "TallinnNtupleProducer/CommonTools/interface/cmsException.h"   // cmsException()
#include "TallinnNtupleProducer/CommonTools/interface/jetDefinitions.h" // Btag, pileupJetID

RecoJetAK4::RecoJetAK4(const GenJet & jet,
                       Double_t charge,
                       Double_t BtagCSV,
                       Double_t BtagWeight,
                       Double_t QGDiscr,
                       Double_t bRegCorr,
                       Double_t bRegRes,
                       Double_t pullEta,
                       Double_t pullPhi,
                       Double_t pullMag,
                       Int_t jetId,
                       Int_t puId,
                       Int_t genMatchIdx,
                       Int_t idx,
                       Btag btag,
                       Int_t central_or_shift)
  : RecoJetBase(jet, idx)
  , jetCharge_(charge)
  , BtagCSV_(BtagCSV)
  , BtagWeight_(BtagWeight)
  , QGDiscr_(QGDiscr)
  , bRegCorr_(bRegCorr)
  , bRegRes_(bRegRes)
  , pullEta_(pullEta)
  , pullPhi_(pullPhi)
  , pullMag_(pullMag)
  , jetId_(jetId)
  , puId_(puId)
  , genMatchIdx_(genMatchIdx)
  , btag_(btag)
  , default_systematics_(central_or_shift)
{}

RecoJetAK4::~RecoJetAK4()
{}

Double_t
RecoJetAK4::charge() const
{
  return jetCharge_;
}

Double_t
RecoJetAK4::BtagCSV() const
{
  return BtagCSV_;
}

Double_t
RecoJetAK4::BtagCSV(Btag btag) const
{
  if(! BtagCSVs_.count(btag))
  {
    throw cmsException(this, __func__, __LINE__)
      << "No such b-tagging score available: " << as_integer(btag)
    ;
  }
  return BtagCSVs_.at(btag);
}

Double_t
RecoJetAK4::BtagWeight() const
{
  return BtagWeight_;
}

Double_t
RecoJetAK4::BtagWeight(Btag btag,
                       int central_or_shift) const
{
  return BtagWeight_systematics_.at(btag).at(central_or_shift);
}

Double_t
RecoJetAK4::BtagWeight(int central_or_shift) const
{
  return BtagWeight(btag_, central_or_shift);
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

Double_t
RecoJetAK4::pullEta() const
{
  return pullEta_;
}

Double_t
RecoJetAK4::pullPhi() const
{
  return pullPhi_;
}

Double_t
RecoJetAK4::pullMag() const
{
  return pullMag_;
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

Int_t
RecoJetAK4::genMatchIdx() const
{
  return genMatchIdx_;
}

Double_t
RecoJetAK4::maxPt() const
{
  double max_Pt = this->pt();
  for(const auto & kv: pt_systematics_)
  {
    if(kv.second > max_Pt)
    {
      max_Pt = kv.second;
    }
  }
  return max_Pt;
}

bool
RecoJetAK4::hasBtag(Btag btag) const
{
  return BtagCSVs_.count(btag);
}

bool
RecoJetAK4::passesPUID(pileupJetID puIdWP) const
{
  return this->puId() & (int)puIdWP;
}

bool
RecoJetAK4::is_PUID_taggable() const
{
  // PU jet ID is applicable only to jets that have pT < 50. GeV
  // https://twiki.cern.ch/twiki/bin/viewauth/CMS/PileupJetID
  return this->pt() < 50.;
}

int
RecoJetAK4::get_default_systematics() const
{
  return default_systematics_;
}

const Particle::LorentzVector
RecoJetAK4::get_systematics_p4(int central_or_shift) const
{
  if(! pt_systematics_.count(central_or_shift) && ! mass_systematics_.count(central_or_shift))
  {
    throw cmsException(this, __func__, __LINE__) << "No such systematics available: " << central_or_shift;
  }
  return { pt_systematics_.at(central_or_shift), eta_, phi_, mass_systematics_.at(central_or_shift) };
}

std::ostream &
operator<<(std::ostream & stream,
           const RecoJetAK4 & jet)
{
  stream << static_cast<const RecoJetBase &>(jet)                            << ",\n"
            " CSV = "             << jet.BtagCSV()                           << ","
            " jet ID = "          << jet.jetId()                             << ","
            " PU ID = "           << jet.puId()                              << ","
            " QGL = "             << jet.QGDiscr()                           << ","
            " bReg corr (res) = " << jet.bRegCorr() << " (" << jet.bRegRes() << "),"
            " sysunc = "          << jet.get_default_systematics()           << '\n'
  ;
  return stream;
}
