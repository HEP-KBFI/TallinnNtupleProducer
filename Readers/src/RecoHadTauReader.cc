#include "TallinnNtupleProducer/Readers/interface/RecoHadTauReader.h" // RecoHadTauReader

#include "TallinnNtupleProducer/CommonTools/interface/BranchAddressInitializer.h" // BranchAddressInitializer
#include "TallinnNtupleProducer/CommonTools/interface/cmsException.h"             // cmsException()
#include "TallinnNtupleProducer/CommonTools/interface/Era.h"                      // Era, get_era()
#include "TallinnNtupleProducer/CommonTools/interface/hadTauDefinitions.h"        // TauID
#include "TallinnNtupleProducer/Readers/interface/TauESTool.h"                    // TauESTool

#include "TMath.h"
#include "TTree.h"                                                                // TTree
#include "TString.h"                                                              // Form()

std::map<std::string, int> RecoHadTauReader::numInstances_;
std::map<std::string, RecoHadTauReader *> RecoHadTauReader::instances_;

RecoHadTauReader::RecoHadTauReader(const edm::ParameterSet & cfg)
  : ReaderBase(cfg)
  , era_(Era::kUndefined)
  , max_nHadTaus_(36)
  , branchName_num_("")
  , branchName_obj_("")
  , isMC_(false)
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
  , hadTau_jetIdx_(nullptr)
  , hadTau_genPartFlav_(nullptr)
  , hadTau_genPartIdx_(nullptr)
{
  era_ = get_era(cfg.getParameter<std::string>("era"));
  branchName_obj_ = cfg.getParameter<std::string>("branchName"); // default = "Tau"
  branchName_num_ = Form("n%s", branchName_obj_.data());
  isMC_ = cfg.getParameter<bool>("isMC");
  tauESTool_ = ( isMC_ ) ? new TauESTool(era_, tauID_, kHadTauPt_central, false) : nullptr;
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
    delete[] gInstance->hadTau_jetIdx_;
    delete[] gInstance->hadTau_genPartFlav_;
    delete[] gInstance->hadTau_genPartIdx_;

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
      branchNames_idMVA_[kv.first]  = Form("%s_%s", branchName_obj_.data(), Form("id%s", mvaString.data()));
      branchNames_rawMVA_[kv.first] = Form("%s_%s", branchName_obj_.data(), Form("raw%s", mvaString.data()));
    }
    branchName_idAgainstElec_ = Form("%s_%s", branchName_obj_.data(), "idAntiEle");
    branchName_idAgainstMu_ = Form("%s_%s", branchName_obj_.data(), "idAntiMu");
    branchName_jetIdx_ = Form("%s_%s", branchName_obj_.data(), "jetIdx");
    branchName_genPartFlav_ = Form("%s_%s", branchName_obj_.data(), "genPartFlav");
    branchName_genPartIdx_ = Form("%s_%s", branchName_obj_.data(), "genPartIdx");
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
    bai.setBranchAddress(hadTau_jetIdx_, branchName_jetIdx_);
    bai.setBranchAddress(hadTau_genPartFlav_, isMC_ ? branchName_genPartFlav_ : "");
    bai.setBranchAddress(hadTau_genPartIdx_, isMC_ ? branchName_genPartIdx_ : "");

    const std::vector<std::string> recoHadTauBranches = bai.getBoundBranchNames_read();
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
        (int)TMath::Log2(gInstance->hadTau_idAgainstElec_[idxHadTau]+1),
        (int)TMath::Log2(gInstance->hadTau_idAgainstMu_[idxHadTau]+1),
        gInstance->hadTau_jetIdx_[idxHadTau],
        gInstance->hadTau_genPartFlav_[idxHadTau],
        gInstance->hadTau_genPartIdx_[idxHadTau],
      });

      RecoHadTau & hadTau = hadTaus.back();

      for(const auto & kv: gInstance->hadTau_idMVAs_)
      {
        const int idMVA = (int)TMath::Log2(gInstance->hadTau_idMVAs_.at(kv.first)[idxHadTau]+1);
        hadTau.tauID_ids_[kv.first] = idMVA;
      }
      for(const auto & kv: gInstance->hadTau_rawMVAs_)
      {
        hadTau.tauID_raws_[kv.first] = gInstance->hadTau_rawMVAs_.at(kv.first)[idxHadTau];
      }
    }
  }
  return hadTaus;
}

std::vector<std::string>
RecoHadTauReader::get_supported_systematics(const edm::ParameterSet & cfg)
{
  return { "CMS_ttHl_tauESUp", "CMS_ttHl_tauESDown" };
}
