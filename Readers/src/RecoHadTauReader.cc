#include "TallinnNtupleProducer/Readers/interface/RecoHadTauReader.h" // RecoHadTauReader

#include "TallinnNtupleProducer/CommonTools/interface/BranchAddressInitializer.h" // BranchAddressInitializer
#include "TallinnNtupleProducer/CommonTools/interface/cmsException.h"             // cmsException()
#include "TallinnNtupleProducer/CommonTools/interface/map_keys.h"                 // map_keys()
#include "TallinnNtupleProducer/CommonTools/interface/Era.h"                      // Era, get_era()
#include "TallinnNtupleProducer/CommonTools/interface/hadTauDefinitions.h"        // TauID
#include "TallinnNtupleProducer/CommonTools/interface/sysUncertOptions.h"         // kHadTauPt_central, kHadTauPt_shiftUp, kHadTauPt_shiftDown

#include "TTree.h"                                                                // TTree
#include "TString.h"                                                              // Form()

namespace {
  int
  intlog2(unsigned input)
  {
    unsigned input_copy = input + 1;
    int log2val = 0;
    while(input_copy >>= 1)
    {
      ++log2val;
    }
    return log2val;
  }
};

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
  switch ( hadTauPt_option )
  {
  case kHadTauPt_central: systematic_ = "nom"; break;
  case kHadTauPt_shiftUp: systematic_ = "up"; break;
  case kHadTauPt_shiftDown: systematic_ = "down"; break;
  default: throw cmsException(__func__, __LINE__) << "Invalid systematic shift = " << hadTauPt_option;
  }
}

void
RecoHadTauReader::set_default_tauID(TauID tauId)
{
  tauID_ = tauId;
}

void
RecoHadTauReader::set_tauEScset(correction::Correction::Ref cset)
{
  tauEScset_ = cset;
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
      const bool valid_dm =
        (gInstance->hadTau_decayMode_[idxHadTau] >= 0 && gInstance->hadTau_decayMode_[idxHadTau] <= 2 ) ||
        gInstance->hadTau_decayMode_[idxHadTau] == 10 ||
        gInstance->hadTau_decayMode_[idxHadTau] == 11
      ;
      const double corrFactor = valid_dm ? tauEScset_->evaluate({
          gInstance->hadTau_pt_[idxHadTau],
          gInstance->hadTau_eta_[idxHadTau],
          gInstance->hadTau_decayMode_[idxHadTau],
          gInstance->hadTau_genPartFlav_[idxHadTau],
          "DeepTau2017v2p1",
           systematic_,
        }) : 1
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
        ::intlog2(gInstance->hadTau_idAgainstElec_[idxHadTau]),
        ::intlog2(gInstance->hadTau_idAgainstMu_[idxHadTau]),
        gInstance->hadTau_jetIdx_[idxHadTau],
        gInstance->hadTau_genPartFlav_[idxHadTau],
        gInstance->hadTau_genPartIdx_[idxHadTau],
      });

      RecoHadTau & hadTau = hadTaus.back();

      for(const auto & kv: gInstance->hadTau_idMVAs_)
      {
        hadTau.tauID_ids_[kv.first] = ::intlog2(gInstance->hadTau_idMVAs_.at(kv.first)[idxHadTau]);
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
  static std::vector<std::string> supported_systematics;
  if(supported_systematics.empty())
  {
    supported_systematics = map_keys(hadTauESSysMap);
  }
  return supported_systematics;
}
