#include "TallinnNtupleProducer/Writers/plugins/RecoLeptonWriter.h"

#include "TallinnNtupleProducer/CommonTools/interface/BranchAddressInitializer.h" // BranchAddressInitializer
#include "TallinnNtupleProducer/CommonTools/interface/cmsException.h"             // cmsException()
#include "TallinnNtupleProducer/CommonTools/interface/merge_systematic_shifts.h"  // merge_systematic_shifts()
#include "TallinnNtupleProducer/Objects/interface/GenHadTau.h"                    // GenHadTau
#include "TallinnNtupleProducer/Readers/interface/RecoElectronReader.h"           // RecoElectronReader::get_supported_systematics()
#include "TallinnNtupleProducer/Readers/interface/RecoMuonReader.h"               // RecoMuonReader::get_supported_systematics()

#include "TString.h"                                                              // Form()
#include "TTree.h"                                                                // TTree

#include <assert.h>                                                               // assert()
#include <cmath>                                                                  // std::abs()

RecoLeptonWriter::RecoLeptonWriter(const edm::ParameterSet & cfg)
  : WriterBase(cfg)
  , branchName_num_("nlep")
  , branchName_obj_("lep")
{
  max_nLeptons_ = cfg.getParameter<unsigned>("numNominalLeptons");
  assert(max_nLeptons_ >= 1);
  nLeptons_ = 0;
  pt_ = new Float_t[max_nLeptons_];
  conePt_ = new Float_t[max_nLeptons_];
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
  isBoosted_ = new Bool_t[max_nLeptons_];
  lsf3_ = new Float_t[max_nLeptons_];
}

RecoLeptonWriter::~RecoLeptonWriter()
{
  delete[] pt_;
  delete[] conePt_;
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
  delete[] isBoosted_;
  delete[] lsf3_;
}

namespace
{
  std::string
  get_branchName_num(const std::string & branchName_num)
  {
    return branchName_num;
  }
  std::string
  get_branchName_obj(const std::string & branchName_obj, int idx, const std::string & suffix)
  {
    return Form("%s%i_%s", branchName_obj.data(), (int)idx + 1, suffix.data());
  }
}

void
RecoLeptonWriter::setBranches(TTree * outputTree)
{
  BranchAddressInitializer bai(outputTree);
  bai.setBranch(nLeptons_, get_branchName_num(branchName_num_));
  for ( size_t idxLepton = 0; idxLepton < max_nLeptons_; ++idxLepton )
  {
    bai.setBranch(pt_[idxLepton], get_branchName_obj(branchName_obj_.data(), (int)idxLepton, "pt"));
    bai.setBranch(conePt_[idxLepton], get_branchName_obj(branchName_obj_.data(), (int)idxLepton, "conePt"));
    bai.setBranch(eta_[idxLepton], get_branchName_obj(branchName_obj_.data(), (int)idxLepton, "eta"));
    bai.setBranch(phi_[idxLepton], get_branchName_obj(branchName_obj_.data(), (int)idxLepton, "phi"));
    bai.setBranch(mass_[idxLepton], get_branchName_obj(branchName_obj_.data(), (int)idxLepton, "mass"));
    bai.setBranch(pdgId_[idxLepton], get_branchName_obj(branchName_obj_.data(), (int)idxLepton, "pdgId"));
    bai.setBranch(charge_[idxLepton], get_branchName_obj(branchName_obj_.data(), (int)idxLepton, "charge"));
    bai.setBranch(mvaRawTTH_[idxLepton], get_branchName_obj(branchName_obj_.data(), (int)idxLepton, "mvaRawTTH"));
    bai.setBranch(isFakeable_[idxLepton], get_branchName_obj(branchName_obj_.data(), (int)idxLepton, "isFakeable"));
    bai.setBranch(isTight_[idxLepton], get_branchName_obj(branchName_obj_.data(), (int)idxLepton, "isTight"));    
    bai.setBranch(tightCharge_[idxLepton], get_branchName_obj(branchName_obj_.data(), (int)idxLepton, "tightCharge"));
    bai.setBranch(genMatch_[idxLepton], get_branchName_obj(branchName_obj_.data(), (int)idxLepton, "genMatch"));
    bai.setBranch(isFake_[idxLepton], get_branchName_obj(branchName_obj_.data(), (int)idxLepton, "isFake"));
    bai.setBranch(isFlip_[idxLepton], get_branchName_obj(branchName_obj_.data(), (int)idxLepton, "isFlip"));
    bai.setBranch(isBoosted_[idxLepton], get_branchName_obj(branchName_obj_.data(), (int)idxLepton, "isBoosted"));
    bai.setBranch(lsf3_[idxLepton], get_branchName_obj(branchName_obj_.data(), (int)idxLepton, "lsf3"));
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
      if ( lepton->charge()*lepton->genLepton()->charge() >= 0 ) genMatch = 1;
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
  const RecoLeptonPtrCollection& leptons = event.fakeableLeptons();
  nLeptons_ = leptons.size();
  for ( size_t idxLepton = 0; idxLepton < max_nLeptons_; ++idxLepton )
  {
    if ( idxLepton < nLeptons_ )
    {
      const RecoLepton * lepton = leptons[idxLepton];
      pt_[idxLepton] = lepton->pt();
      conePt_[idxLepton] = lepton->cone_pt();
      eta_[idxLepton] = lepton->eta();
      phi_[idxLepton] = lepton->phi();
      mass_[idxLepton] = lepton->mass();
      pdgId_[idxLepton] = std::abs(lepton->pdgId());
      charge_[idxLepton] = lepton->charge();
      mvaRawTTH_[idxLepton] = lepton->mvaRawTTH();
      isFakeable_[idxLepton] = lepton->isFakeable();
      isTight_[idxLepton] = lepton->isTight();
      tightCharge_[idxLepton] = lepton->tightCharge();
      genMatch_[idxLepton] = compGenMatch(lepton);
      isFake_[idxLepton] = (genMatch_[idxLepton] == 16);
      isFlip_[idxLepton] = (genMatch_[idxLepton] == 2);
      isBoosted_[idxLepton] = lepton->isBoostedLepton();
      lsf3_[idxLepton] = lepton->getlsf3();
    }
    else
    {
      pt_[idxLepton] = 0.;
      conePt_[idxLepton] = 0.;
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
      isBoosted_[idxLepton] = false;
      lsf3_[idxLepton] = -1;
    }
  }
}

std::vector<std::string>
RecoLeptonWriter::get_supported_systematics(const edm::ParameterSet & cfg)
{
  std::vector<std::string> supported_systematics;
  merge_systematic_shifts(supported_systematics, RecoElectronReader::get_supported_systematics(cfg));
  merge_systematic_shifts(supported_systematics, RecoMuonReader::get_supported_systematics(cfg));
  return supported_systematics;
}

DEFINE_EDM_PLUGIN(WriterPluginFactory, RecoLeptonWriter, "RecoLeptonWriter"); // clazy:skip
