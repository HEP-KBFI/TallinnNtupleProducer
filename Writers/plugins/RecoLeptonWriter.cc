#include "TallinnNtupleProducer/Writers/plugins/RecoLeptonWriter.h"

#include "TallinnNtupleProducer/CommonTools/interface/cmsException.h"         // cmsException()
#include "TallinnNtupleProducer/Objects/interface/GenHadTau.h"                // GenHadTau
#include "TallinnNtupleProducer/Objects/interface/GenLepton.h"                // GenLepton
#include "TallinnNtupleProducer/Objects/interface/GenPhoton.h"                // GenPhoton
#include "TallinnNtupleProducer/Readers/interface/BranchAddressInitializer.h" // BranchAddressInitializer

#include "TString.h"                                                          // Form()
#include "TTree.h"                                                            // TTree

#include <assert.h>                                                           // assert()

RecoLeptonWriter::RecoLeptonWriter(const edm::ParameterSet & cfg)
  : WriterBase(cfg)
  , branchName_num_("nlep")
  , branchName_obj_("lep")
{
  max_nLeptons_ = cfg.getParameter<unsigned>("numNominalLeptons");
  assert(max_nLeptons_ >= 1);
  pt_ = new Float_t[max_nLeptons_];
  eta_ = new Float_t[max_nLeptons_];
  phi_ = new Float_t[max_nLeptons_];
  mass_ = new Float_t[max_nLeptons_];
  pdgId_ = new Int_t[max_nLeptons_];
  charge_ = new Int_t[max_nLeptons_];
  mvaRawTTH_ = new Float_t[max_nLeptons_];
  isFakeable_ = new Bool_t[max_nLeptons_];
  isTight_ = new Bool_t[max_nLeptons_];
  tightCharge_ = new Int_t[max_nLeptons_];
  genMatch_ = new Int_t[max_nLeptons_];
  isFake_ = new Bool_t[max_nLeptons_];
  isFlip_ = new Bool_t[max_nLeptons_];
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
RecoLeptonWriter::setBranches(TTree * outputTree)
{
  BranchAddressInitializer bai(outputTree);
  bai.setBranch(nLeptons_, branchName_num_);
  for ( size_t idxLepton = 0; idxLepton < nLeptons_; ++idxLepton )
  {
    bai.setBranch(pt_[idxLepton], Form("%s%i_%s", branchName_obj_.data(), (int)idxLepton, "pt"));
    bai.setBranch(eta_[idxLepton], Form("%s%i_%s", branchName_obj_.data(), (int)idxLepton, "eta"));
    bai.setBranch(phi_[idxLepton], Form("%s%i_%s", branchName_obj_.data(), (int)idxLepton, "phi"));
    bai.setBranch(mass_[idxLepton], Form("%s%i_%s", branchName_obj_.data(), (int)idxLepton, "mass"));
    bai.setBranch(pdgId_[idxLepton], Form("%s%i_%s", branchName_obj_.data(), (int)idxLepton, "pdgId"));
    bai.setBranch(charge_[idxLepton], Form("%s%i_%s", branchName_obj_.data(), (int)idxLepton, "charge"));
    bai.setBranch(mvaRawTTH_[idxLepton], Form("%s%i_%s", branchName_obj_.data(), (int)idxLepton, "mvaRawTTH"));
    bai.setBranch(isFakeable_[idxLepton], Form("%s%i_%s", branchName_obj_.data(), (int)idxLepton, "isFakeable"));
    bai.setBranch(isTight_[idxLepton], Form("%s%i_%s", branchName_obj_.data(), (int)idxLepton, "isTight"));    
    bai.setBranch(tightCharge_[idxLepton], Form("%s%i_%s", branchName_obj_.data(), (int)idxLepton, "tightCharge"));
    bai.setBranch(genMatch_[idxLepton], Form("%s%i_%s", branchName_obj_.data(), (int)idxLepton, "genMatch"));
    bai.setBranch(isFake_[idxLepton], Form("%s%i_%s", branchName_obj_.data(), (int)idxLepton, "isFake"));
    bai.setBranch(isFlip_[idxLepton], Form("%s%i_%s", branchName_obj_.data(), (int)idxLepton, "isFlip"));
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
RecoLeptonWriter::writeImp(const Event & event, const EvtWeightRecorder & evtWeightRecorder)
{
  // Warning: The following code assumes that only those events get written to the "plain" Ntuple
  //          for which the number of fakeable leptons is equal to the number of "nominal" leptons 
  //         (where the number of "nominal" leptons is specific to a given channel)
  const RecoLeptonPtrCollection& leptons = event.fakeableLeptons();
  for ( size_t idxLepton = 0; idxLepton < max_nLeptons_; ++idxLepton )
  {
    if ( idxLepton < nLeptons_ )
    {
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
    else
    {
      pt_[idxLepton] = 0.;
      eta_[idxLepton] = 0.;
      phi_[idxLepton] = 0.;
      mass_[idxLepton] = 0.;
      pdgId_[idxLepton] = 0;
      charge_[idxLepton] = 0;
      mvaRawTTH_[idxLepton] = 0.;
      isFakeable_[idxLepton] = false;
      isTight_[idxLepton] = false;
      tightCharge_[idxLepton] = 0;
      genMatch_[idxLepton] = 0;
      isFake_[idxLepton] = false;
      isFlip_[idxLepton] = false;
    }
  }
}

std::vector<std::string>
RecoLeptonWriter::get_supported_systematics()
{
  return std::vector<std::string>();
}

DEFINE_EDM_PLUGIN(WriterPluginFactory, RecoLeptonWriter, "RecoLeptonWriter");
