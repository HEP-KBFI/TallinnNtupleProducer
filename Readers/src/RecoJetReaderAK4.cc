#include "TallinnNtupleProducer/Readers/interface/RecoJetReaderAK4.h"

#include "TallinnNtupleProducer/CommonTools/interface/BranchAddressInitializer.h" // BranchAddressInitializer
#include "TallinnNtupleProducer/CommonTools/interface/cmsException.h"             // cmsException()
#include "TallinnNtupleProducer/CommonTools/interface/Era.h"                      // Era, get_era()
#include "TallinnNtupleProducer/CommonTools/interface/jetDefinitions.h"           // Btag, kBtag_*
#include "TallinnNtupleProducer/CommonTools/interface/merge_systematic_shifts.h"  // merge_systematic_shifts()
#include "TallinnNtupleProducer/CommonTools/interface/map_keys.h"                 // map_keys()
#include "TallinnNtupleProducer/CommonTools/interface/sysUncertOptions.h"         // jesAK4SysMap, jerAK4SysMap, jesSplitAK4SysMap, jerSplitAK4SysMap

#include "TTree.h"                                                                // TTree
#include "TString.h"                                                              // Form()

std::map<std::string, int> RecoJetReaderAK4::numInstances_;
std::map<std::string, RecoJetReaderAK4 *> RecoJetReaderAK4::instances_;

RecoJetReaderAK4::RecoJetReaderAK4(const edm::ParameterSet & cfg)
  : ReaderBase(cfg)
  , era_(get_era(cfg.getParameter<std::string>("era")))
  , isMC_(cfg.getParameter<bool>("isMC"))
  , max_nJets_(256)
  , branchName_obj_(cfg.getParameter<std::string>("branchName"))
  , branchName_num_(Form("n%s", branchName_obj_.data()))
  , btag_(Btag::kDeepJet)
  , jet_pt_(nullptr)
  , jet_eta_(nullptr)
  , jet_phi_(nullptr)
  , jet_mass_(nullptr)
  , jet_QGDiscr_(nullptr)
  , jet_bRegCorr_(nullptr)
  , jet_bRegRes_(nullptr)
  , jet_BtagScore_(nullptr)
  , jet_jetId_(nullptr)
  , jet_puId_(nullptr)
  , jet_genJetIdx_(nullptr)
  , jet_partonFlavour_(nullptr)
  , jet_hadronFlavour_(nullptr)
  , jet_rawFactor_(nullptr)
  , jet_area_(nullptr)
  , jet_neEmEF_(nullptr)
  , jet_chEmEF_(nullptr)
  , jet_muonSubtrFactor_(nullptr)
{
  setBranchNames();
}

RecoJetReaderAK4::~RecoJetReaderAK4()
{
  --numInstances_[branchName_obj_];
  assert(numInstances_[branchName_obj_] >= 0);
  if(numInstances_[branchName_obj_] == 0)
  {
    RecoJetReaderAK4 * const gInstance = instances_[branchName_obj_];
    assert(gInstance);
    delete[] gInstance->jet_pt_;
    delete[] gInstance->jet_eta_;
    delete[] gInstance->jet_phi_;
    delete[] gInstance->jet_mass_;
    delete[] gInstance->jet_QGDiscr_;
    delete[] gInstance->jet_bRegCorr_;
    delete[] gInstance->jet_bRegRes_;
    delete[] gInstance->jet_BtagScore_;
    delete[] gInstance->jet_jetId_;
    delete[] gInstance->jet_puId_;
    delete[] gInstance->jet_genJetIdx_;
    delete[] gInstance->jet_partonFlavour_;
    delete[] gInstance->jet_hadronFlavour_;
    delete[] gInstance->jet_rawFactor_;
    delete[] gInstance->jet_area_;
    delete[] gInstance->jet_neEmEF_;
    delete[] gInstance->jet_chEmEF_;
    delete[] gInstance->jet_muonSubtrFactor_;
    instances_[branchName_obj_] = nullptr;
  }
}

void
RecoJetReaderAK4::read_Btag(Btag btag)
{
  btag_ = btag;
}

void
RecoJetReaderAK4::setBranchNames()
{
  if(numInstances_[branchName_obj_] == 0)
  {
    branchName_pt_ = Form("%s_%s", branchName_obj_.data(), "pt");
    branchName_eta_ = Form("%s_%s", branchName_obj_.data(), "eta");
    branchName_phi_ = Form("%s_%s", branchName_obj_.data(), "phi");
    branchName_mass_ = Form("%s_%s", branchName_obj_.data(), "mass");

    for(const auto & kv: BtagWP_map.at(era_))
    {
      std::string btagName;
      switch(kv.first)
      {
        case Btag::kDeepCSV: btagName = "DeepB";     break;
        case Btag::kDeepJet: btagName = "DeepFlavB"; break;
        case Btag::kCSVv2:   btagName = "CSVV2";     break;
      }
      assert(! btagName.empty());
      branchNames_btag_[kv.first] = Form("%s_btag%s", branchName_obj_.data(), btagName.data());
    }
    assert(! branchNames_btag_.empty());

    branchName_QGDiscr_ = Form("%s_%s", branchName_obj_.data(), "qgl");
    branchName_bRegCorr_ = Form("%s_%s", branchName_obj_.data(), "bRegCorr");
    branchName_bRegRes_ = Form("%s_%s", branchName_obj_.data(), "bRegRes");
    branchName_BtagScore_ = Form("%s_%s", branchName_obj_.data(), "btagDeepFlavB");
    branchName_jetId_ = Form("%s_%s", branchName_obj_.data(), "jetId");
    branchName_puId_ = Form("%s_%s", branchName_obj_.data(), "puId");
    branchName_genJetIdx_ = Form("%s_%s", branchName_obj_.data(), "genJetIdx");
    branchName_partonFlavour_ = Form("%s_%s", branchName_obj_.data(), "partonFlavour");
    branchName_hadronFlavour_ = Form("%s_%s", branchName_obj_.data(), "hadronFlavour");
    branchName_rawFactor_ = Form("%s_%s", branchName_obj_.data(), "rawFactor");
    branchName_area_ = Form("%s_%s", branchName_obj_.data(), "area");
    branchName_neEmEF_ = Form("%s_%s", branchName_obj_.data(), "neEmEF");
    branchName_chEmEF_ = Form("%s_%s", branchName_obj_.data(), "chEmEF");
    branchName_muonSubtrFactor_ = Form("%s_%s", branchName_obj_.data(), "muonSubtrFactor");

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
RecoJetReaderAK4::setBranchAddresses(TTree * tree)
{
  std::vector<std::string> bound_branches;
  if(instances_[branchName_obj_] == this)
  {
    BranchAddressInitializer bai(tree, max_nJets_);
    bai.setBranchAddress(nJets_, branchName_num_);
    bai.setBranchAddress(jet_pt_, branchName_pt_);
    bai.setBranchAddress(jet_eta_, branchName_eta_);
    bai.setBranchAddress(jet_phi_, branchName_phi_);
    bai.setBranchAddress(jet_mass_, branchName_mass_);
    bai.setBranchAddress(jet_QGDiscr_, branchName_QGDiscr_, 1.);
    bai.setBranchAddress(jet_bRegCorr_, branchName_bRegCorr_, 1.);
    bai.setBranchAddress(jet_bRegRes_, branchName_bRegRes_, 0.);
    bai.setBranchAddress(jet_BtagScore_, branchName_BtagScore_);
    bai.setBranchAddress(jet_jetId_, branchName_jetId_);
    bai.setBranchAddress(jet_puId_, branchName_puId_);
    bai.setBranchAddress(jet_genJetIdx_, isMC_ && branchName_obj_ == "Jet" ? branchName_genJetIdx_ : "", -1);
    bai.setBranchAddress(jet_partonFlavour_, branchName_partonFlavour_);
    bai.setBranchAddress(jet_hadronFlavour_, branchName_hadronFlavour_);
    bai.setBranchAddress(jet_rawFactor_, branchName_rawFactor_);
    bai.setBranchAddress(jet_area_, branchName_area_);
    bai.setBranchAddress(jet_neEmEF_, branchName_neEmEF_);
    bai.setBranchAddress(jet_chEmEF_, branchName_chEmEF_);
    bai.setBranchAddress(jet_muonSubtrFactor_, branchName_muonSubtrFactor_);

    const std::vector<std::string> recoJetBranches = bai.getBoundBranchNames_read();
    bound_branches.insert(bound_branches.end(), recoJetBranches.begin(), recoJetBranches.end());
  }
  return bound_branches;
}

std::vector<RecoJetAK4>
RecoJetReaderAK4::read() const
{
  const RecoJetReaderAK4 * const gInstance = instances_[branchName_obj_];
  assert(gInstance);

  std::vector<RecoJetAK4> jets;
  const UInt_t nJets = gInstance->nJets_;
  if(nJets > max_nJets_)
  {
    throw cmsException(this)
      << "Number of jets stored in Ntuple = " << nJets << ", "
         "exceeds max_nJets = " << max_nJets_ << " !!\n";
  }

  if(nJets > 0)
  {
    jets.reserve(nJets);
    for(UInt_t idxJet = 0; idxJet < nJets; ++idxJet)
    {
      // set QGL to -1. if:
      // 1) the value is nan
      // 2) the value is invalid: https://twiki.cern.ch/twiki/bin/view/CMS/QuarkGluonLikelihood#Return_codes
      float qgl = gInstance->jet_QGDiscr_[idxJet];
      if(std::isnan(qgl))
      {
        qgl = -1.;
      }
      qgl = std::max(-1.f, qgl);

      // According to the log files of NanoAOD production, some jets might have nan as the value for DeepJet
      // b-tagging discriminator. In the past we've seen similar issues with DeepCSV score as well. Will set the value
      // of b-tagging discriminant to -2. in case the score is nan (as we already did with lepton-to-jet variables).
      // Not sure how nan values affect the b-tagging scale factors, though.
      float btagCSV = gInstance->jet_BtagScore_[idxJet];
      if(std::isnan(btagCSV))
      {
        btagCSV = -2.;
      }

      jets.push_back({
        {
          gInstance->jet_pt_[idxJet],
          gInstance->jet_eta_[idxJet],
          gInstance->jet_phi_[idxJet],
          gInstance->jet_mass_[idxJet],
          gInstance->jet_partonFlavour_[idxJet],
          gInstance->jet_hadronFlavour_[idxJet],
        },
        btagCSV,
        qgl,
        gInstance->jet_bRegCorr_[idxJet],
        gInstance->jet_bRegRes_[idxJet],
        gInstance->jet_jetId_[idxJet],
        gInstance->jet_puId_[idxJet],
        gInstance->jet_rawFactor_[idxJet],
        gInstance->jet_area_[idxJet],
        gInstance->jet_neEmEF_[idxJet],
        gInstance->jet_chEmEF_[idxJet],
        gInstance->jet_muonSubtrFactor_[idxJet],
        idxJet,
        gInstance->jet_genJetIdx_[idxJet],
        btag_,
      });
    } // idxJet
  } // nJets > 0
  return jets;
}

std::vector<std::string>
RecoJetReaderAK4::get_supported_systematics(const edm::ParameterSet & cfg)
{
  static std::vector<std::string> supported_systematics;
  if(supported_systematics.empty())
  {
    merge_systematic_shifts(supported_systematics, map_keys(jesAK4SysMap));
    const bool split_jes = cfg.getParameter<bool>("split_jes");
    if(split_jes)
    {
      std::vector<std::string> jesSplitAK4SysStrs = map_keys(jesSplitAK4SysMap);
      const Era era = get_era(cfg.getParameter<std::string>("era"));
      if ( era != Era::k2018 ) 
      {
        // KE: add systematic uncertainty on jet energy correction that addresses the HEM15/16 issue,
        //     cf. https://hypernews.cern.ch/HyperNews/CMS/get/JetMET/2000.html
        jesSplitAK4SysStrs.erase(
          std::remove_if(
            jesSplitAK4SysStrs.begin(), jesSplitAK4SysStrs.end(),
            [](const std::string & sysStr) -> bool {
              return sysStr.find("HEM") != std::string::npos;
            }
          ), jesSplitAK4SysStrs.end()
        );
      }
      merge_systematic_shifts(supported_systematics, jesSplitAK4SysStrs);
    }
    if(cfg.getParameter<bool>("apply_smearing"))
    {
      merge_systematic_shifts(supported_systematics, map_keys(jerAK4SysMap));
      if(cfg.getParameter<bool>("split_jer"))
      {
        merge_systematic_shifts(supported_systematics, map_keys(jerSplitAK4SysMap));
      }
    }
  }
  return supported_systematics;
}
