#include "TallinnNtupleProducer/Writers/interface/RecoHadTauWriter.h"

#include "TallinnNtupleProducer/Framework/interface/BranchAddressInitializer.h" // BranchAddressInitializer
#include "TallinnNtupleProducer/Framework/interface/cmsException.h"             // cmsException()

#include "TString.h"                                                            // Form()

RecoHadTauWriter::RecoHadTauWriter(const edm::ParameterSet & cfg)
  : branchName_num_("ntau")
  , branchName_obj_("tau")
{
  nHadTaus_ = cfg.getParameter<unsigned>("numNominalHadTaus");
  pt_ = new Float_t[nHadTaus_];
  eta_ = new Float_t[nHadTaus_];
  phi_ = new Float_t[nHadTaus_];
  mass_ = new Float_t[nHadTaus_];
  decayMode_ = new Int_t[nHadTaus_];
  charge_ = new Int_t[nHadTaus_];
  isFakeable_ = new Bool_t[nHadTaus_];
  isTight_ = new Bool_t[nHadTaus_];
  genMatch_ = new Int_t[nHadTaus_];
  isFake_ = new Bool_t[nHadTaus_];
  isFlip_ = new Bool_t[nHadTaus_];
}

RecoHadTauWriter::~RecoHadTauWriter()
{
  delete[] pt_;
  delete[] eta_;
  delete[] phi_;
  delete[] mass_;
  delete[] decayMode_;
  delete[] charge_;
  delete[] isFakeable_;
  delete[] isTight_;
  delete[] genMatch_;
  delete[] isFake_;
  delete[] isFlip_;
}

void
RecoHadTauWriter::setBranches(TTree * tree)
{
  BranchAddressInitializer bai(tree);
  bai.setBranch(nHadTaus_, branchName_num_);
  for ( size_t idxHadTau = 0; idxHadTau < nHadTaus_; ++idxHadTau )
  {
    bai.setBranch(pt_[idxHadTau], Form("%s%i_%s", branchName_obj_.data(), idxHadTau, "pt"));
    bai.setBranch(eta_[idxHadTau], Form("%s%i_%s", branchName_obj_.data(), idxHadTau, "eta"));
    bai.setBranch(phi_[idxHadTau], Form("%s%i_%s", branchName_obj_.data(), idxHadTau, "phi"));
    bai.setBranch(mass_[idxHadTau], Form("%s%i_%s", branchName_obj_.data(), idxHadTau, "mass"));
    bai.setBranch(decayMode_[idxHadTau], Form("%s%i_%s", branchName_obj_.data(), idxHadTau, "decayMode"));
    bai.setBranch(charge_[idxHadTau], Form("%s%i_%s", branchName_obj_.data(), idxHadTau, "charge"));
    bai.setBranch(isFakeable_[idxHadTau], Form("%s%i_%s", branchName_obj_.data(), idxHadTau, "isFakeable"));
    bai.setBranch(isTight_[idxHadTau], Form("%s%i_%s", branchName_obj_.data(), idxHadTau, "isTight"));    
    bai.setBranch(genMatch_[idxHadTau], Form("%s%i_%s", branchName_obj_.data(), idxHadTau, "genMatch"));
    bai.setBranch(isFake_[idxHadTau], Form("%s%i_%s", branchName_obj_.data(), idxHadTau, "isFake"));  
    bai.setBranch(isFlip_[idxHadTau], Form("%s%i_%s", branchName_obj_.data(), idxHadTau, "isFlip"));  
  }
}

namespace
{
  unsigned
  compGenMatch(const RecoHadTau * hadTau)
  {
    unsigned genMatch = 0;
    if ( hadTau->genHadTau() )
    {
      if ( hadTau->charge() == hadTau->genHadTau()->charge() ) genMatch = 1;
      else genMatch = 2;
    }
    else if ( hadTau->genLepton() && std::abs(hadTau->genLepton()->pdgId()) == 11 )
    {
      if ( hadTau->charge() == hadTau->genLepton()->charge() ) genMatch = 4;
      else genMatch = 8;
    }
    else if ( hadTau->genLepton() && std::abs(hadTau->genLepton()->pdgId()) == 13 )
    {
      if ( hadTau->charge() == hadTau->genLepton()->charge() ) genMatch = 16;
      else genMatch = 32;
    }
    else
    {
      genMatch = 64;
    }
    return genMatch;
  }
}

void
RecoHadTauWriter::write(const Event & event)
{
  // Warning: The following code assumes that only those events get written to the "plain" Ntuple
  //          for which the number of fakeable hadronic taus is equal to the number of "nominal" hadronic taus 
  //         (where the number of "nominal" hadronic taus is specific to a given channel)
  const RecoHadTauPtrCollection& hadTaus = event.fakeableHadTaus();
  for ( size_t idxHadTau = 0; idxHadTau < nHadTaus_; ++idxHadTau )
    const RecoHadTau * hadTau = hadTaus[idxHadTau];
    pt_[idxHadTau] = hadTau->pt();
    eta_[idxHadTau] = hadTau->eta();
    phi_[idxHadTau] = hadTau->phi();
    mass_[idxHadTau] = hadTau->mass();
    decayMode_[idxHadTau] = hadTau->decayMode();
    charge_[idxHadTau] = lepton->charge();
    isFakeable_[idxHadTau] = hadTau->isFakeable();
    isTight_[idxHadTau] = hadTau->isTight();
    genMatch_ = compGenMatch(hadTau);
    isFake_[idxHadTau] = genMatch_[idxHadTau] == 64;
    isFlip_[idxHadTau] = genMatch_[idxHadTau] == 2 || genMatch_[idxHadTau] == 8 || genMatch_[idxHadTau] == 32;
  }
}

DEFINE_EDM_PLUGIN(WriterPluginFactory, RecoHadTauWriter, "RecoHadTauWriter");
