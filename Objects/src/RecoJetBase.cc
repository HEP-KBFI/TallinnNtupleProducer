#include "TallinnNtupleProducer/Objects/interface/RecoJetBase.h"

#include "TallinnNtupleProducer/Objects/interface/GenHadTau.h" // GenHadTau
#include <iostream>
RecoJetBase::RecoJetBase(const GenJet & jet,
                         Float_t rawFactor,
                         UInt_t idx)
  : GenJet(jet)
  , rawFactor_(rawFactor)
  , idx_(idx)
  , genLepton_(nullptr)
  , genHadTau_(nullptr)
  , genJet_(nullptr)
{}

RecoJetBase::~RecoJetBase()
{}

void
RecoJetBase::set_genLepton(const GenParticle * genLepton)
{
  genLepton_ = genLepton;
}

void
RecoJetBase::set_genHadTau(const GenHadTau *  genHadTau)
{
  genHadTau_ = genHadTau;
}

void
RecoJetBase::set_genJet(const GenJet * genJet)
{
  genJet_ = genJet;
}

Float_t
RecoJetBase::rawFactor() const
{
  return rawFactor_;
}

UInt_t
RecoJetBase::idx() const
{
  return idx_;
}

const GenParticle *
RecoJetBase::genLepton() const
{
  return genLepton_;
}

const GenHadTau *
RecoJetBase::genHadTau() const
{
  return genHadTau_;
}

const GenJet *
RecoJetBase::genJet() const
{
  return genJet_;
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
  stream << static_cast<const GenJet &>(jet)   << ","
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

