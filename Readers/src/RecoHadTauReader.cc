#include "TallinnNtupleProducer/Readers/interface/RecoHadTauReader.h" // RecoHadTauReader

#include "TallinnNtupleProducer/CommonTools/interface/cmsException.h"         // cmsException()
#include "TallinnNtupleProducer/CommonTools/interface/Era.h"                  // Era, get_era()
#include "TallinnNtupleProducer/CommonTools/interface/hadTauDefinitions.h"    // TauID
#include "TallinnNtupleProducer/Readers/interface/BranchAddressInitializer.h" // BranchAddressInitializer
#include "TallinnNtupleProducer/Readers/interface/GenHadTauReader.h"          // GenHadTauReader
#include "TallinnNtupleProducer/Readers/interface/GenLeptonReader.h"          // GenLeptonReader
#include "TallinnNtupleProducer/Readers/interface/GenJetReader.h"             // GenJetReader
#include "TallinnNtupleProducer/Readers/interface/TauESTool.h"                // TauESTool

#include "TTree.h"                                                            // TTree
#include "TString.h"                                                          // Form()

std::map<std::string, int> RecoHadTauReader::numInstances_;
std::map<std::string, RecoHadTauReader *> RecoHadTauReader::instances_;

RecoHadTauReader::RecoHadTauReader(const edm::ParameterSet & cfg)
  : ReaderBase(cfg)
  , era_(Era::kUndefined)
  , max_nHadTaus_(36)
  , branchName_num_("")
  , branchName_obj_("")
  , isMC_(false)
  , genLeptonReader_(nullptr)
  , genHadTauReader_(nullptr)
  , genJetReader_(nullptr)
  , readGenMatching_(false)
  , tauID_(TauID::DeepTau2017v2VSjet)
  , tauESTool_(nullptr)
  , hadTau_pt_(nullptr)
  , hadTau_eta_(nullptr)
  , hadTau_phi_(nullptr)
  , hadTau_mass_(nullptr)
  , hadTau_charge_(nullptr)
  , hadTau_dxy_(nullptr)
  , hadTau_dz_(nullptr)
  , hadTau_decayMode_(nullptr)
  , hadTau_idDecayMode_(nullptr)
  , hadTau_idAgainstElec_(nullptr)
  , hadTau_idAgainstMu_(nullptr)
  , hadTau_filterBits_(nullptr)
  , hadTau_jetIdx_(nullptr)
  , hadTau_genPartFlav_(nullptr)
  , hadTau_genMatchIdx_(nullptr)
{
  era_ = get_era(cfg.getParameter<std::string>("era"));
  branchName_obj_ = cfg.getParameter<std::string>("branchName"); // default = "Tau"
  branchName_num_ = Form("n%s", branchName_obj_.data());
  isMC_ = cfg.getParameter<bool>("isMC");
  readGenMatching_ = isMC_ && !cfg.getParameter<bool>("redoGenMatching");
  tauESTool_ = ( isMC_ ) ? new TauESTool(era_, tauID_, kHadTauPt_central, false) : nullptr;
  if(readGenMatching_)
  {
    edm::ParameterSet cfg_genLepton = makeReader_cfg(era_, Form("%s_genLepton", branchName_obj_.data()), true);
    cfg_genLepton.addParameter<unsigned int>("max_nLeptons", max_nHadTaus_);
    genLeptonReader_ = new GenLeptonReader(cfg_genLepton);
    edm::ParameterSet cfg_genTau = makeReader_cfg(era_, Form("%s_genTau", branchName_obj_.data()), true);
    cfg_genTau.addParameter<unsigned int>("max_nHadTaus", max_nHadTaus_);
    genHadTauReader_ = new GenHadTauReader(cfg_genTau);
    edm::ParameterSet cfg_genJet = makeReader_cfg(era_, Form("%s_genJet", branchName_obj_.data()), true);
    cfg_genJet.addParameter<unsigned int>("max_nJets", max_nHadTaus_);
    genJetReader_ = new GenJetReader(cfg_genJet);
  }
  setBranchNames();
}

RecoHadTauReader::~RecoHadTauReader()
{
  --numInstances_[branchName_obj_];
  assert(numInstances_[branchName_obj_] >= 0);

  if(numInstances_[branchName_obj_] == 0)
  {
    numInstances_.erase(branchName_obj_);

    RecoHadTauReader * const gInstance = instances_[branchName_obj_];
    assert(gInstance);

    delete gInstance->genLeptonReader_;
    delete gInstance->genHadTauReader_;
    delete gInstance->genJetReader_;
    delete[] gInstance->hadTau_pt_;
    delete[] gInstance->hadTau_eta_;
    delete[] gInstance->hadTau_phi_;
    delete[] gInstance->hadTau_mass_;
    delete[] gInstance->hadTau_dxy_;
    delete[] gInstance->hadTau_dz_;
    delete[] gInstance->hadTau_decayMode_;
    delete[] gInstance->hadTau_idDecayMode_;
    delete[] gInstance->hadTau_idAgainstElec_;
    delete[] gInstance->hadTau_idAgainstMu_;
    delete[] gInstance->hadTau_charge_;
    delete[] gInstance->hadTau_filterBits_;
    delete[] gInstance->hadTau_jetIdx_;
    delete[] gInstance->hadTau_genPartFlav_;
    delete[] gInstance->hadTau_genMatchIdx_;

    for(auto & kv: gInstance->hadTau_idMVAs_)
    {
      delete[] kv.second;
    }
    for(auto & kv: gInstance->hadTau_rawMVAs_)
    {
      delete[] kv.second;
    }

    instances_.erase(branchName_obj_);
  }
}

void
RecoHadTauReader::setHadTauPt_central_or_shift(int hadTauPt_option)
{
  if(tauESTool_)
  {
    tauESTool_->set_central_or_shift(hadTauPt_option);
  }
}

void
RecoHadTauReader::set_default_tauID(TauID tauId)
{
  tauID_ = tauId;
  if(tauESTool_)
  {
    tauESTool_->set_tauID(tauID_);
  }
}

void
RecoHadTauReader::setBranchNames()
{
  if(numInstances_[branchName_obj_] == 0)
  {
    branchName_pt_ = Form("%s_%s", branchName_obj_.data(), "pt");
    branchName_eta_ = Form("%s_%s", branchName_obj_.data(), "eta");
    branchName_phi_ = Form("%s_%s", branchName_obj_.data(), "phi");
    branchName_mass_ = Form("%s_%s", branchName_obj_.data(), "mass");
    branchName_charge_ = Form("%s_%s", branchName_obj_.data(), "charge");
    branchName_dxy_ = Form("%s_%s", branchName_obj_.data(), "dxy");
    branchName_dz_ = Form("%s_%s", branchName_obj_.data(), "dz");
    branchName_decayMode_ = Form("%s_%s", branchName_obj_.data(), "decayMode");
    branchName_idDecayMode_ = Form("%s_%s", branchName_obj_.data(), "idDecayMode");
    for(const auto & kv: TauID_levels)
    {
      const std::string & mvaString = TauID_names.at(kv.first);
      branchNames_idMVA_[kv.first]  = Form("%s_%s", branchName_obj_.data(), Form("id%s_log", mvaString.data()));
      branchNames_rawMVA_[kv.first] = Form("%s_%s", branchName_obj_.data(), Form("raw%s", mvaString.data()));
    }
    branchName_idAgainstElec_ = Form("%s_%s", branchName_obj_.data(), "idAntiEle_log");
    branchName_idAgainstMu_ = Form("%s_%s", branchName_obj_.data(), "idAntiMu_log");
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
           " present association 'branchName_num' = " << branchName_num_ << " with 'branchName_obj' = " << branchName_obj_
        << " does not match previous association 'branchName_num' = " << instances_[branchName_obj_]->branchName_num_
        << " with 'branchName_obj' = " << instances_[branchName_obj_]->branchName_obj_ << " !!\n";
    }
  }
  ++numInstances_[branchName_obj_];
}

std::vector<std::string>
RecoHadTauReader::setBranchAddresses(TTree * tree)
{
  std::vector<std::string> bound_branches;
  if(instances_[branchName_obj_] == this)
  {
    if(readGenMatching_)
    {
      const std::vector<std::string> genLeptonBranches = genLeptonReader_->setBranchAddresses(tree);
      const std::vector<std::string> genHadTauBranches = genHadTauReader_->setBranchAddresses(tree);
      const std::vector<std::string> genJetBranches = genJetReader_->setBranchAddresses(tree);

      bound_branches.insert(bound_branches.end(), genLeptonBranches.begin(), genLeptonBranches.end());
      bound_branches.insert(bound_branches.end(), genHadTauBranches.begin(), genHadTauBranches.end());
      bound_branches.insert(bound_branches.end(), genJetBranches.begin(), genJetBranches.end());
    }
    BranchAddressInitializer bai(tree, max_nHadTaus_);
    bai.setBranchAddress(nHadTaus_, branchName_num_);
    bai.setBranchAddress(hadTau_pt_, branchName_pt_);
    bai.setBranchAddress(hadTau_eta_, branchName_eta_);
    bai.setBranchAddress(hadTau_phi_, branchName_phi_);
    bai.setBranchAddress(hadTau_mass_, branchName_mass_);
    bai.setBranchAddress(hadTau_charge_, branchName_charge_);
    bai.setBranchAddress(hadTau_dxy_, branchName_dxy_);
    bai.setBranchAddress(hadTau_dz_, branchName_dz_);
    bai.setBranchAddress(hadTau_decayMode_, branchName_decayMode_);
    bai.setBranchAddress(hadTau_idDecayMode_, branchName_idDecayMode_);
    for(const auto & kv: TauID_levels)
    {
      bai.setBranchAddress(hadTau_idMVAs_[kv.first], branchNames_idMVA_[kv.first]);
      bai.setBranchAddress(hadTau_rawMVAs_[kv.first], branchNames_rawMVA_[kv.first]);
    }
    bai.setBranchAddress(hadTau_idAgainstElec_, branchName_idAgainstElec_);
    bai.setBranchAddress(hadTau_idAgainstMu_, branchName_idAgainstMu_);
    bai.setBranchAddress(hadTau_filterBits_, branchName_filterBits_);
    bai.setBranchAddress(hadTau_jetIdx_, branchName_jetIdx_);
    bai.setBranchAddress(hadTau_genPartFlav_, isMC_ ? branchName_genPartFlav_ : "");
    bai.setBranchAddress(hadTau_genMatchIdx_, isMC_ ? branchName_genMatchIdx_ : "");

    const std::vector<std::string> recoHadTauBranches = bai.getBoundBranchNames();
    bound_branches.insert(bound_branches.end(), recoHadTauBranches.begin(), recoHadTauBranches.end());
  }
  return bound_branches;
}

std::vector<RecoHadTau>
RecoHadTauReader::read() const
{
  const RecoHadTauReader * const gInstance = instances_[branchName_obj_];
  assert(gInstance);

  std::vector<RecoHadTau> hadTaus;
  const UInt_t nHadTaus = gInstance->nHadTaus_;
  if(nHadTaus > max_nHadTaus_)
  {
    throw cmsException(this)
      << "Number of hadronic taus stored in Ntuple = " << nHadTaus
      << ", exceeds max_nHadTaus = " << max_nHadTaus_ << " !!\n";
  }

  if(nHadTaus > 0)
  {
    hadTaus.reserve(nHadTaus);
    for(UInt_t idxHadTau = 0; idxHadTau < nHadTaus; ++idxHadTau)
    {
      const double corrFactor = tauESTool_ ? tauESTool_->getTES(
          gInstance->hadTau_pt_[idxHadTau],
          gInstance->hadTau_decayMode_[idxHadTau],
          gInstance->hadTau_genPartFlav_[idxHadTau]
        ) : 1.
      ;
      const double hadTau_pt   = gInstance->hadTau_pt_  [idxHadTau] * corrFactor;
      const double hadTau_mass = gInstance->hadTau_mass_[idxHadTau] * corrFactor;

      hadTaus.push_back({
        {
          hadTau_pt,
          gInstance->hadTau_eta_[idxHadTau],
          gInstance->hadTau_phi_[idxHadTau],
          hadTau_mass,
          gInstance->hadTau_charge_[idxHadTau]
        },
        corrFactor,
        gInstance->hadTau_dxy_[idxHadTau],
        gInstance->hadTau_dz_[idxHadTau],
        gInstance->hadTau_decayMode_[idxHadTau],
        gInstance->hadTau_idDecayMode_[idxHadTau],
        gInstance->hadTau_idMVAs_.at(tauID_)[idxHadTau],
        gInstance->hadTau_rawMVAs_.at(tauID_)[idxHadTau],
        gInstance->hadTau_idAgainstElec_[idxHadTau],
        gInstance->hadTau_idAgainstMu_[idxHadTau],
        gInstance->hadTau_filterBits_[idxHadTau],
        gInstance->hadTau_jetIdx_[idxHadTau],
        gInstance->hadTau_genPartFlav_[idxHadTau],
        gInstance->hadTau_genMatchIdx_[idxHadTau],
      });

      RecoHadTau & hadTau = hadTaus.back();

      for(const auto & kv: gInstance->hadTau_idMVAs_)
      {
        hadTau.tauID_ids_[kv.first] = gInstance->hadTau_idMVAs_.at(kv.first)[idxHadTau];
      }
      for(const auto & kv: gInstance->hadTau_rawMVAs_)
      {
        hadTau.tauID_raws_[kv.first] = gInstance->hadTau_rawMVAs_.at(kv.first)[idxHadTau];
      }
    }
    readGenMatching(hadTaus);
  }
  return hadTaus;
}

void
RecoHadTauReader::readGenMatching(std::vector<RecoHadTau> & hadTaus) const
{
  if(readGenMatching_)
  {
    assert(genLeptonReader_ && genHadTauReader_ && genJetReader_);
    const std::size_t nHadTaus = hadTaus.size();

    std::vector<GenLepton> matched_genLeptons = genLeptonReader_->read();
    assert(matched_genLeptons.size() == nHadTaus);

    std::vector<GenHadTau> matched_genHadTaus = genHadTauReader_->read();
    assert(matched_genHadTaus.size() == nHadTaus);

    std::vector<GenJet> matched_genJets = genJetReader_->read();
    assert(matched_genJets.size() == nHadTaus);

    for(std::size_t idxHadTau = 0; idxHadTau < nHadTaus; ++idxHadTau)
    {
      RecoHadTau & hadTau = hadTaus[idxHadTau];

      const GenLepton & matched_genLepton = matched_genLeptons[idxHadTau];
      if(matched_genLepton.isValid()) hadTau.set_genLepton(new GenLepton(matched_genLepton));

      const GenHadTau & matched_genHadTau = matched_genHadTaus[idxHadTau];
      if(matched_genHadTau.isValid()) hadTau.set_genHadTau(new GenHadTau(matched_genHadTau));

      const GenJet & matched_genJet = matched_genJets[idxHadTau];
      if(matched_genJet.isValid()) hadTau.set_genJet(new GenJet(matched_genJet));
    }
  }
}
