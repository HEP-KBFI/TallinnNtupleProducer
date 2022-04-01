#include "TallinnNtupleProducer/Objects/interface/RecoJetBase.h"

#include "TallinnNtupleProducer/Objects/interface/GenLepton.h" // GenLepton
#include "TallinnNtupleProducer/Objects/interface/GenHadTau.h" // GenHadTau

RecoJetBase::RecoJetBase(const GenJet & jet,
                         Int_t idx)
  : GenJet(jet)
  , idx_(idx)
  , genLepton_(nullptr)
  , genHadTau_(nullptr)
  , genJet_(nullptr)
{}

RecoJetBase::~RecoJetBase()
{}

void
RecoJetBase::set_genLepton(const GenLepton * genLepton)
{
  genLepton_.reset(genLepton);
}

void
RecoJetBase::set_genHadTau(const GenHadTau *  genHadTau)
{
  genHadTau_.reset(genHadTau);
}

void
RecoJetBase::set_genJet(const GenJet * genJet)
{
  genJet_.reset(genJet);
}

Int_t
RecoJetBase::idx() const
{
  return idx_;
}

const GenLepton *
RecoJetBase::genLepton() const
{
  return genLepton_.get();
}

const GenHadTau *
RecoJetBase::genHadTau() const
{
  return genHadTau_.get();
}

const GenJet *
RecoJetBase::genJet() const
{
  return genJet_.get();
}

bool
RecoJetBase::isGenMatched() const
{
  return !! genJet_;
}

bool
RecoJetBase::hasAnyGenMatch() const
{
  return !! genLepton_ || !! genHadTau_ || !! genJet_;
}

std::ostream &
operator<<(std::ostream & stream,
           const RecoJetBase & jet)
{
  stream << static_cast<const GenJet &>(jet)      << ","
            " charge = "          << jet.charge() << ","
            "\n"
            " gen. matching:";
  stream << ",\n  lepton = " << jet.genLepton();
  if(jet.genLepton())
  {
    stream << ": " << *(jet.genLepton());
  }
  stream << ",\n  hadTau = " << jet.genHadTau();
  if(jet.genHadTau())
  {
    stream << ": " << *(jet.genHadTau());
  }
  stream << ",\n  jet    = " << jet.genJet();
  if(jet.genJet())
  {
    stream << ": " << *(jet.genJet());
  }
  return stream;
}

