#include "TallinnNtupleProducer/Writers/plugins/RecoLeptonWriter.h"

#include "TallinnNtupleProducer/Framework/interface/BranchAddressInitializer.h" // BranchAddressInitializer
#include "TallinnNtupleProducer/Framework/interface/cmsException.h"             // cmsException()

#include "TString.h"                                                            // Form()

RecoLeptonWriter::RecoLeptonWriter(const edm::ParameterSet & cfg)
  : branchName_num_("nlep")
  , branchName_obj_("lep")
{
  nLeptons_ = cfg.getParameter<unsigned>("numNominalLeptons");
  pt_ = new Float_t[nLeptons_];
  eta_ = new Float_t[nLeptons_];
  phi_ = new Float_t[nLeptons_];
  mass_ = new Float_t[nLeptons_];
  pdgId_ = new Int_t[nLeptons_];
  charge_ = new Int_t[nLeptons_];
  mvaRawTTH_ = new Float_t[nLeptons_];
  isFakeable_ = new Bool_t[nLeptons_];
  isTight_ = new Bool_t[nLeptons_];
  tightCharge_ = new Int_t[nLeptons_];
  genMatch_ = new Int_t[nLeptons_];
  isFake_ = new Bool_t[nLeptons_];
  isFlip_ = new Bool_t[nLeptons_];
}

RecoLeptonWriter::~RecoLeptonWriter()
{
  delete[] pt_;
  delete[] eta_;
  delete[] phi_;
  delete[] mass_;
  delete[] pdgId_;
  delete[] charge_;
  delete[] mvaRawTTH_;
  delete[] isFakeable_;
  delete[] isTight_;
  delete[] tightCharge_;
  delete[] genMatch_;
  delete[] isFake_;
  delete[] isFlip_;
}

void
RecoLeptonWriter::setBranches(TTree * tree)
{
  BranchAddressInitializer bai(tree);
  bai.setBranch(nLeptons_, branchName_num_);
  for ( size_t idxLepton = 0; idxLepton < nLeptons_; ++idxLepton )
  {
    bai.setBranch(pt_[idxLepton], Form("%s%i_%s", branchName_obj_.data(), idxLepton, "pt"));
    bai.setBranch(eta_[idxLepton], Form("%s%i_%s", branchName_obj_.data(), idxLepton, "eta"));
    bai.setBranch(phi_[idxLepton], Form("%s%i_%s", branchName_obj_.data(), idxLepton, "phi"));
    bai.setBranch(mass_[idxLepton], Form("%s%i_%s", branchName_obj_.data(), idxLepton, "mass"));
    bai.setBranch(pdgId_[idxLepton], Form("%s%i_%s", branchName_obj_.data(), idxLepton, "pdgId"));
    bai.setBranch(charge_[idxLepton], Form("%s%i_%s", branchName_obj_.data(), idxLepton, "charge"));
    bai.setBranch(mvaRawTTH_[idxLepton], Form("%s%i_%s", branchName_obj_.data(), idxLepton, "mvaRawTTH"));
    bai.setBranch(isFakeable_[idxLepton], Form("%s%i_%s", branchName_obj_.data(), idxLepton, "isFakeable"));
    bai.setBranch(isTight_[idxLepton], Form("%s%i_%s", branchName_obj_.data(), idxLepton, "isTight"));    
    bai.setBranch(tightCharge_[idxLepton], Form("%s%i_%s", branchName_obj_.data(), idxLepton, "tightCharge"));
    bai.setBranch(genMatch_[idxLepton], Form("%s%i_%s", branchName_obj_.data(), idxLepton, "genMatch"));
    bai.setBranch(isFake_[idxLepton], Form("%s%i_%s", branchName_obj_.data(), idxLepton, "isFake"));
    bai.setBranch(isFlip_[idxLepton], Form("%s%i_%s", branchName_obj_.data(), idxLepton, "isFlip"));
  }
}

namespace
{
  unsigned
  compGenMatch(const RecoLepton * lepton)
  {
    unsigned genMatch = 0;
    if ( lepton->genLepton() )
    {
      if ( lepton->charge() == lepton->genLepton()->charge() ) genMatch = 1;
      else genMatch = 2;
    }
    else if ( lepton->is_electron() && lepton->genPhoton() && lepton->genPhoton()->pt() > (0.50*lepton->pt()) )
    {
      genMatch = 4;
    }
    else if ( lepton->genHadTau())
    {
      genMatch = 8;
    }
    else
    {
      genMatch = 16;
    }
    return genMatch;
  }
}

void
RecoLeptonWriter::write(const Event & event)
{
  // Warning: The following code assumes that only those events get written to the "plain" Ntuple
  //          for which the number of fakeable leptons is equal to the number of "nominal" leptons 
  //         (where the number of "nominal" leptons is specific to a given channel)
  const RecoLeptonPtrCollection& leptons = event.fakeableLeptons();
  for ( size_t idxLepton = 0; idxLepton < nLeptons_; ++idxLepton )
    const RecoLepton * lepton = leptons[idxLepton];
    pt_[idxLepton] = lepton->cone_pt();
    eta_[idxLepton] = lepton->eta();
    phi_[idxLepton] = lepton->phi();
    mass_[idxLepton] = lepton->mass();
    pdgId_[idxLepton] = lepton->pdgId();
    charge_[idxLepton] = lepton->charge();
    mvaRawTTH_[idxLepton] = lepton->mvaRawTTH();
    isFakeable_[idxLepton] = lepton->isFakeable();
    isTight_[idxLepton] = lepton->isTight();
    tightCharge_[idxLepton] = lepton->tightCharge();
    genMatch_[idxLepton] = compGenMatch(lepton);
    isFake_[idxLepton] = genMatch_[idxLepton] == 16;
    isFlip_[idxLepton] = genMatch_[idxLepton] == 2;
  }
}

DEFINE_EDM_PLUGIN(WriterPluginFactory, RecoLeptonWriter, "RecoLeptonWriter");
