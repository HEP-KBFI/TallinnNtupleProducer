#include "TallinnNtupleProducer/Readers/interface/RecoLeptonReader.h"

#include "TallinnNtupleProducer/CommonTools/interface/BranchAddressInitializer.h" // BranchAddressInitializer
#include "TallinnNtupleProducer/CommonTools/interface/cmsException.h"             // cmsException
#include "TallinnNtupleProducer/CommonTools/interface/Era.h"                      // Era, get_era()
#include "TallinnNtupleProducer/CommonTools/interface/jetDefinitions.h"           // Btag

#include <TString.h>                                                              // Form
#include <TTree.h>                                                                // TTree

std::map<std::string, int> RecoLeptonReader::numInstances_;
std::map<std::string, RecoLeptonReader *> RecoLeptonReader::instances_;

RecoLeptonReader::RecoLeptonReader(const edm::ParameterSet & cfg)
  : ReaderBase(cfg)
  , max_nLeptons_(64)
  , branchName_num_("")
  , branchName_obj_("")
  , isMC_(false)
  , era_(Era::kUndefined)
  , genLeptonReader_(nullptr)
  , genHadTauReader_(nullptr)
  , genPhotonReader_(nullptr)
  , genJetReader_(nullptr)
  , readGenMatching_(false)
  , pt_(nullptr)
  , eta_(nullptr)
  , phi_(nullptr)
  , mass_(nullptr)
  , pdgId_(nullptr)
  , dxy_(nullptr)
  , dz_(nullptr)
  , relIso_all_(nullptr)
  , pfRelIso03_all_(nullptr)
  , relIso_chg_(nullptr)
  , relIso_neu_(nullptr)
  , sip3d_(nullptr)
  , mvaRawTTH_(nullptr)
  , jetRelIso_(nullptr)
  , jetPtRel_(nullptr)
  , jetNDauChargedMVASel_(nullptr)
  , tightCharge_(nullptr)
  , charge_(nullptr)
  , filterBits_(nullptr)
  , jetIdx_(nullptr)
  , genPartFlav_(nullptr)
{
  era_ = get_era(cfg.getParameter<std::string>("era"));
  branchName_obj_ = cfg.getParameter<std::string>("branchName");
  branchName_num_ = Form("n%s", branchName_obj_.data());
  isMC_ = cfg.getParameter<bool>("isMC");
  readGenMatching_ = isMC_ && !cfg.getParameter<bool>("redoGenMatching");
  if(readGenMatching_)
  {
    edm::ParameterSet cfg_genLepton = makeReader_cfg(era_, Form("%s_genLepton", branchName_obj_.data()), true);
    cfg_genLepton.addParameter<unsigned int>("max_nLeptons", max_nLeptons_);
    genLeptonReader_ = new GenLeptonReader(cfg_genLepton);
    edm::ParameterSet cfg_genTau = makeReader_cfg(era_, Form("%s_genTau", branchName_obj_.data()), true);
    cfg_genTau.addParameter<unsigned int>("max_nHadTaus", max_nLeptons_);
    genHadTauReader_ = new GenHadTauReader(cfg_genTau);
    edm::ParameterSet cfg_genPhoton = makeReader_cfg(era_, Form("%s_genPhoton", branchName_obj_.data()), true);
    cfg_genPhoton.addParameter<unsigned int>("max_nPhotons", max_nLeptons_);
    genPhotonReader_ = new GenPhotonReader(cfg_genPhoton);
    edm::ParameterSet cfg_genJet = makeReader_cfg(era_, Form("%s_genJet", branchName_obj_.data()), true);
    cfg_genJet.addParameter<unsigned int>("max_nJets", max_nLeptons_);
    genJetReader_ = new GenJetReader(cfg_genJet);
  }
  setBranchNames();
}

RecoLeptonReader::~RecoLeptonReader()
{
  --numInstances_[branchName_obj_];
  assert(numInstances_[branchName_obj_] >= 0);

  if(numInstances_[branchName_obj_] == 0)
  {
    RecoLeptonReader * const gInstance = instances_[branchName_obj_];
    assert(gInstance);
    delete gInstance->genLeptonReader_;
    delete gInstance->genHadTauReader_;
    delete gInstance->genPhotonReader_;
    delete gInstance->genJetReader_;
    delete[] gInstance->pt_;
    delete[] gInstance->eta_;
    delete[] gInstance->phi_;
    delete[] gInstance->mass_;
    delete[] gInstance->pdgId_;
    delete[] gInstance->dxy_;
    delete[] gInstance->dz_;
    delete[] gInstance->relIso_all_;
    delete[] gInstance->pfRelIso03_all_;
    delete[] gInstance->relIso_chg_;
    delete[] gInstance->relIso_neu_;
    delete[] gInstance->sip3d_;
    delete[] gInstance->mvaRawTTH_;
    delete[] gInstance->jetRelIso_;
    delete[] gInstance->jetPtRel_;
    delete[] gInstance->jetNDauChargedMVASel_;
    delete[] gInstance->tightCharge_;
    delete[] gInstance->charge_;
    delete[] gInstance->filterBits_;
    delete[] gInstance->jetIdx_;
    delete[] gInstance->genPartFlav_;

    for(auto & kv: gInstance->jetBtagCSVs_)
    {
      delete[] kv.second;
    }
    for(auto & kv: gInstance->assocJetBtagCSVs_)
    {
      delete[] kv.second;
    }

    instances_[branchName_obj_] = nullptr;
  }
}

void
RecoLeptonReader::setBranchNames()
{
  if(numInstances_[branchName_obj_] == 0)
  {
    branchName_pt_ = Form("%s_%s", branchName_obj_.data(), "pt");
    branchName_eta_ = Form("%s_%s", branchName_obj_.data(), "eta");
    branchName_phi_ = Form("%s_%s", branchName_obj_.data(), "phi");
    branchName_mass_ = Form("%s_%s", branchName_obj_.data(), "mass");
    branchName_pdgId_ = Form("%s_%s", branchName_obj_.data(), "pdgId");
    branchName_dxy_ = Form("%s_%s", branchName_obj_.data(), "dxy");
    branchName_dz_ = Form("%s_%s", branchName_obj_.data(), "dz");
    branchName_relIso_all_ = Form("%s_%s", branchName_obj_.data(), "miniPFRelIso_all");
    branchName_pfRelIso03_all_ = Form("%s_%s", branchName_obj_.data(), "pfRelIso03_all");
    branchName_relIso_chg_ = Form("%s_%s", branchName_obj_.data(), "miniPFRelIso_chg");
    branchName_relIso_neu_ = Form("%s_%s", branchName_obj_.data(), "miniPFRelIso_neu");
    branchName_sip3d_ = Form("%s_%s", branchName_obj_.data(), "sip3d");
    branchName_mvaRawTTH_ = Form("%s_%s", branchName_obj_.data(), "mvaTTH");
    branchName_jetRelIso_ = Form("%s_%s", branchName_obj_.data(), "jetRelIso");
    branchName_jetPtRel_ = Form("%s_%s", branchName_obj_.data(), "jetPtRelv2");
    branchName_jetNDauChargedMVASel_ = Form("%s_%s", branchName_obj_.data(), "jetNDauChargedMVASel");
    for(Btag btag: { Btag::kCSVv2, Btag::kDeepCSV, Btag::kDeepJet })
    {
      if(btag == Btag::kCSVv2 && era_ == Era::k2018)
      {
        continue;
      }
      std::string btag_str = "";
      switch(btag)
      {
        case Btag::kCSVv2:   btag_str = "CSV"; break;
        case Btag::kDeepCSV: btag_str = "DeepCSV"; break;
        case Btag::kDeepJet: btag_str = "DeepJet"; break;
      }
      branchNames_jetBtagCSV_[btag] = Form("%s_jetBTag%s", branchName_obj_.data(), btag_str.data());
      branchNames_assocJetBtagCSV_[btag] = Form("%s_assocJetBtag_%s", branchName_obj_.data(), btag_str.data());
    }
    branchName_tightCharge_ = Form("%s_%s", branchName_obj_.data(), "tightCharge");
    branchName_charge_ = Form("%s_%s", branchName_obj_.data(), "charge");
    branchName_filterBits_ = Form("%s_%s", branchName_obj_.data(), "filterBits");
    branchName_jetIdx_ = Form("%s_%s", branchName_obj_.data(), "jetIdx");
    branchName_genPartFlav_ = Form("%s_%s", branchName_obj_.data(), "genPartFlav");
    branchName_genMatchIdx_ = Form("%s_%s", branchName_obj_.data(), "genMatchIdx");
    instances_[branchName_obj_] = this;
  }
  else
  {
    if(branchName_num_ != instances_[branchName_obj_]->branchName_num_)
    {
      throw cmsException(this)
        << "Association between configuration parameters 'branchName_num' and 'branchName_obj' must be unique:"
        << " present association 'branchName_num' = " << branchName_num_ << " with 'branchName_obj' = " << branchName_obj_
        << " does not match previous association 'branchName_num' = " << instances_[branchName_obj_]->branchName_num_
        << " with 'branchName_obj' = " << instances_[branchName_obj_]->branchName_obj_ << " !!\n";
    }
  }
  ++numInstances_[branchName_obj_];
}

std::vector<std::string>
RecoLeptonReader::setBranchAddresses(TTree * tree)
{
  std::vector<std::string> bound_branches;
  if(instances_[branchName_obj_] == this)
  {
    if(readGenMatching_)
    {
      const std::vector<std::string> genLeptonBranches = genLeptonReader_->setBranchAddresses(tree);
      const std::vector<std::string> genHadTauBranches = genHadTauReader_->setBranchAddresses(tree);
      const std::vector<std::string> genPhotonBranches = genPhotonReader_->setBranchAddresses(tree);
      const std::vector<std::string> genJetBranches = genJetReader_->setBranchAddresses(tree);

      bound_branches.insert(bound_branches.end(), genLeptonBranches.begin(), genLeptonBranches.end());
      bound_branches.insert(bound_branches.end(), genHadTauBranches.begin(), genHadTauBranches.end());
      bound_branches.insert(bound_branches.end(), genPhotonBranches.begin(), genPhotonBranches.end());
      bound_branches.insert(bound_branches.end(), genJetBranches.begin(), genJetBranches.end());
    }
    BranchAddressInitializer bai(tree, max_nLeptons_);
    bai.setBranchAddress(nLeptons_, branchName_num_);
    bai.setBranchAddress(pt_, branchName_pt_);
    bai.setBranchAddress(eta_, branchName_eta_);
    bai.setBranchAddress(phi_, branchName_phi_);
    bai.setBranchAddress(mass_, branchName_mass_);
    bai.setBranchAddress(pdgId_, branchName_pdgId_);
    bai.setBranchAddress(dxy_, branchName_dxy_);
    bai.setBranchAddress(dz_, branchName_dz_);
    bai.setBranchAddress(pfRelIso03_all_, branchName_pfRelIso03_all_);
    bai.setBranchAddress(relIso_all_, branchName_relIso_all_);
    bai.setBranchAddress(relIso_chg_, branchName_relIso_chg_);
    bai.setBranchAddress(sip3d_, branchName_sip3d_);
    bai.setBranchAddress(mvaRawTTH_, branchName_mvaRawTTH_);
    bai.setBranchAddress(jetRelIso_, branchName_jetRelIso_);
    for(const auto & kv: branchNames_assocJetBtagCSV_)
    {
      bai.setBranchAddress(assocJetBtagCSVs_[kv.first], kv.second);
    }
    bai.setBranchAddress(tightCharge_, branchName_tightCharge_);
    bai.setBranchAddress(charge_, branchName_charge_);
    bai.setBranchAddress(jetIdx_, branchName_jetIdx_);
    bai.setBranchAddress(genPartFlav_, isMC_ ? branchName_genPartFlav_ : "");

    const std::vector<std::string> recoLeptonBranches = bai.getBoundBranchNames_read();
    bound_branches.insert(bound_branches.end(), recoLeptonBranches.begin(), recoLeptonBranches.end());
  }
  return bound_branches;
}
