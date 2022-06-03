#include "TallinnNtupleProducer/Readers/interface/RecoJetReaderAK4.h"

#include "TallinnNtupleProducer/CommonTools/interface/BranchAddressInitializer.h" // BranchAddressInitializer
#include "TallinnNtupleProducer/CommonTools/interface/cmsException.h"             // cmsException()
#include "TallinnNtupleProducer/CommonTools/interface/Era.h"                      // Era, get_era()
#include "TallinnNtupleProducer/CommonTools/interface/jetDefinitions.h"           // Btag, kBtag_*
#include "TallinnNtupleProducer/CommonTools/interface/merge_systematic_shifts.h"  // merge_systematic_shifts()
#include "TallinnNtupleProducer/CommonTools/interface/sysUncertOptions.h"         // getBranchName_jetMET(), getBranchName_bTagWeight(), getBranchName_jetPtMass()
#include "TallinnNtupleProducer/Readers/interface/GenHadTauReader.h"              // GenHadTauReader
#include "TallinnNtupleProducer/Readers/interface/GenLeptonReader.h"              // GenLeptonReader
#include "TallinnNtupleProducer/Readers/interface/GenJetReader.h"                 // GenJetReader

#include "TTree.h"                                                                // TTree
#include "TString.h"                                                              // Form()

std::map<std::string, int> RecoJetReaderAK4::numInstances_;
std::map<std::string, RecoJetReaderAK4 *> RecoJetReaderAK4::instances_;

RecoJetReaderAK4::RecoJetReaderAK4(const edm::ParameterSet & cfg)
  : ReaderBase(cfg)
  , era_(Era::kUndefined)
  , isMC_(false)
  , max_nJets_(256)
  , branchName_num_("")
  , branchName_obj_("")
  , genLeptonReader_(nullptr)
  , genHadTauReader_(nullptr)
  , genJetReader_(nullptr)
  , readGenMatching_(false)
  , btag_(Btag::kDeepJet)
  , btag_central_or_shift_(kBtag_central)
  , ptMassOption_central_(-1)
  , ptMassOption_(-1)
  , jet_eta_(nullptr)
  , jet_phi_(nullptr)
  , jet_charge_(nullptr)
  , jet_QGDiscr_(nullptr)
  , jet_bRegCorr_(nullptr)
  , jet_bRegRes_(nullptr)
  , jet_pullEta_(nullptr)
  , jet_pullPhi_(nullptr)
  , jet_pullMag_(nullptr)
  , jet_BtagScore_(nullptr)
  , jet_jetId_(nullptr)
  , jet_puId_(nullptr)
  , jet_genMatchIdx_(nullptr)
{
  era_ = get_era(cfg.getParameter<std::string>("era"));
  branchName_obj_ = cfg.getParameter<std::string>("branchName"); // default = "Jet"
  branchName_num_ = Form("n%s", branchName_obj_.data());
  isMC_ = cfg.getParameter<bool>("isMC");
  ptMassOption_central_ = ( isMC_ ) ? kJetMET_central : kJetMET_central_nonNominal;
  ptMassOption_ = ptMassOption_central_;
  readGenMatching_ = isMC_ && !cfg.getParameter<bool>("redoGenMatching");
  if(readGenMatching_)
  {
    edm::ParameterSet cfg_genLepton = makeReader_cfg(era_, Form("%s_genLepton", branchName_obj_.data()), true);
    cfg_genLepton.addParameter<unsigned int>("max_nLeptons", max_nJets_);
    genLeptonReader_ = new GenLeptonReader(cfg_genLepton);
    edm::ParameterSet cfg_genTau = makeReader_cfg(era_, Form("%s_genTau", branchName_obj_.data()), true);
    cfg_genTau.addParameter<unsigned int>("max_nHadTaus", max_nJets_);
    genHadTauReader_ = new GenHadTauReader(cfg_genTau);
    edm::ParameterSet cfg_genJet = makeReader_cfg(era_, Form("%s_genJet", branchName_obj_.data()), true);
    cfg_genJet.addParameter<unsigned int>("max_nJets", max_nJets_);
    genJetReader_ = new GenJetReader(cfg_genJet);
  }
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
    delete gInstance->genLeptonReader_;
    delete gInstance->genHadTauReader_;
    delete gInstance->genJetReader_;
    delete[] gInstance->jet_eta_;
    delete[] gInstance->jet_phi_;
    delete[] gInstance->jet_charge_;
    delete[] gInstance->jet_QGDiscr_;
    delete[] gInstance->jet_bRegCorr_;
    delete[] gInstance->jet_bRegRes_;
    delete[] gInstance->jet_pullEta_;
    delete[] gInstance->jet_pullPhi_;
    delete[] gInstance->jet_pullMag_;
    delete[] gInstance->jet_BtagScore_;
    delete[] gInstance->jet_jetId_;
    delete[] gInstance->jet_puId_;
    delete[] gInstance->jet_genMatchIdx_;
    for(auto & kv: gInstance->jet_pt_systematics_)
    {
      delete[] kv.second;
    }
    for(auto & kv: gInstance->jet_mass_systematics_)
    {
      delete[] kv.second;
    }
    for(auto & kv: gInstance->jet_BtagWeights_systematics_)
    {
      for(auto & kw: kv.second)
      {
        delete[] kw.second;
      }
    }
    instances_[branchName_obj_] = nullptr;
  }
}

void
RecoJetReaderAK4::setPtMass_central_or_shift(int central_or_shift)
{
  if(! isMC_ && central_or_shift != kJetMET_central_nonNominal)
  {
    throw cmsException(this, __func__, __LINE__) << "Data has only non-nominal pt and mass";
  }
  if(! isValidJESsource(era_, central_or_shift))
  {
    ptMassOption_ = ptMassOption_central_;
  }
  ptMassOption_ = central_or_shift;
}

void
RecoJetReaderAK4::setBtagWeight_central_or_shift(int central_or_shift)
{
  if(! isMC_ && central_or_shift != kBtag_central)
  {
    throw cmsException(this, __func__, __LINE__)
      << "No systematic uncertainties on b-tagging SFs available for data"
    ;
  }
  assert(central_or_shift >= kBtag_central && central_or_shift <= kBtag_jesRelativeSample_EraDown);
  btag_central_or_shift_ = central_or_shift;
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
    branchName_eta_ = Form("%s_%s", branchName_obj_.data(), "eta");
    branchName_phi_ = Form("%s_%s", branchName_obj_.data(), "phi");
    for(int idxShift = kJetMET_central_nonNominal; idxShift <= kJetMET_jerForwardHighPtDown; ++idxShift)
    {
      if( (idxShift == ptMassOption_central_) || (isMC_ && isValidJESsource(era_, idxShift)) )
      {
        branchNames_pt_systematics_[idxShift]   = getBranchName_jetMET(branchName_obj_, era_, idxShift, true);
        branchNames_mass_systematics_[idxShift] = getBranchName_jetMET(branchName_obj_, era_, idxShift, false);
      }
    }

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
    for(auto & kv: BtagWP_map.at(era_))
    {
      if(kv.first != Btag::kDeepJet)
      {
        continue;
      }
      branchNames_BtagWeight_systematics_[kv.first] = {};
      for(int idxShift = kBtag_central; idxShift <= kBtag_jesRelativeSample_EraDown; ++idxShift)
      {
        branchNames_BtagWeight_systematics_[kv.first][idxShift] = getBranchName_bTagWeight(
          kv.first, era_, branchName_obj_, idxShift
        );
      }
    }
    branchName_pullEta_ = Form("%s_%s", branchName_obj_.data(), "pullEta");
    branchName_pullPhi_ = Form("%s_%s", branchName_obj_.data(), "pullPhi");
    branchName_pullMag_ = Form("%s_%s", branchName_obj_.data(), "pullMag");
    branchName_BtagScore_ = Form("%s_%s", branchName_obj_.data(), "btagDeepFlavB");
    branchName_jetId_ = Form("%s_%s", branchName_obj_.data(), "jetId");
    branchName_puId_ = Form("%s_%s", branchName_obj_.data(), "puId");
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
RecoJetReaderAK4::setBranchAddresses(TTree * tree)
{
  std::vector<std::string> bound_branches;
  if(instances_[branchName_obj_] == this)
  {
    BranchAddressInitializer bai(tree, max_nJets_);
    if(readGenMatching_)
    {
      const std::vector<std::string> genLeptonBranches = genLeptonReader_->setBranchAddresses(tree);
      const std::vector<std::string> genHadTauBranches = genHadTauReader_->setBranchAddresses(tree);
      const std::vector<std::string> genJetBranches = genJetReader_->setBranchAddresses(tree);

      bound_branches.insert(bound_branches.end(), genLeptonBranches.begin(), genLeptonBranches.end());
      bound_branches.insert(bound_branches.end(), genHadTauBranches.begin(), genHadTauBranches.end());
      bound_branches.insert(bound_branches.end(), genJetBranches.begin(), genJetBranches.end());
    }
    for(int idxShift = kJetMET_central_nonNominal; idxShift <= kJetMET_jerForwardHighPtDown; ++idxShift)
    {
      if( (idxShift == ptMassOption_central_) || (isMC_ && isValidJESsource(era_, idxShift)) )
      {
        bai.setBranchAddress(jet_pt_systematics_[idxShift],   branchNames_pt_systematics_[idxShift]);
        bai.setBranchAddress(jet_mass_systematics_[idxShift], branchNames_mass_systematics_[idxShift]);
      }
    }
    bai.setBranchAddress(nJets_, branchName_num_);
    bai.setBranchAddress(jet_eta_, branchName_eta_);
    bai.setBranchAddress(jet_phi_, branchName_phi_);
    bai.setBranchAddress(jet_charge_, branchName_jetCharge_);

    for(const auto & kv: branchNames_BtagWeight_systematics_)
    {
      for(int idxShift = kBtag_central; idxShift <= kBtag_jesRelativeSample_EraDown; ++idxShift)
      {
        if( idxShift == btag_central_or_shift_ || isMC_ )
        {
          bai.setBranchAddress(
            jet_BtagWeights_systematics_[kv.first][idxShift], isMC_ ? branchNames_BtagWeight_systematics_[kv.first][idxShift] : "", 1.
          );
        }
      }
    }

    bai.setBranchAddress(jet_QGDiscr_, branchName_QGDiscr_, 1.);
    bai.setBranchAddress(jet_bRegCorr_, branchName_bRegCorr_, 1.);
    bai.setBranchAddress(jet_bRegRes_, branchName_bRegRes_, 0.);
    bai.setBranchAddress(jet_BtagScore_, branchName_BtagScore_);
    bai.setBranchAddress(jet_jetId_, branchName_jetId_);
    bai.setBranchAddress(jet_puId_, branchName_puId_);
    bai.setBranchAddress(jet_genMatchIdx_, isMC_ && branchName_obj_ == "Jet" ? branchName_genMatchIdx_ : "", -1);

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
      double qgl = gInstance->jet_QGDiscr_[idxJet];
      if(std::isnan(qgl))
      {
        qgl = -1.;
      }
      qgl = std::max(-1., qgl);

      // According to the log files of NanoAOD production, some jets might have nan as the value for DeepJet
      // b-tagging discriminator. In the past we've seen similar issues with DeepCSV score as well. Will set the value
      // of b-tagging discriminant to -2. in case the score is nan (as we already did with lepton-to-jet variables).
      // Not sure how nan values affect the b-tagging scale factors, though.
      double btagCSV = gInstance->jet_BtagScore_[idxJet];
      if(std::isnan(btagCSV))
      {
        btagCSV = -2.;
      }

      double jet_pt = gInstance->jet_pt_systematics_.at(ptMassOption_)[idxJet];
      const double jet_eta = gInstance->jet_eta_[idxJet];
      const double jet_phi = gInstance->jet_phi_[idxJet];
      double jet_mass = gInstance->jet_mass_systematics_.at(ptMassOption_)[idxJet];
      const int jet_id = gInstance->jet_jetId_[idxJet];

      jets.push_back({
        {
          jet_pt,
          jet_eta,
          jet_phi,
          jet_mass,
        },
        gInstance->jet_charge_[idxJet],
        btagCSV,
        gInstance->jet_BtagWeights_systematics_.at(btag_).at(btag_central_or_shift_)[idxJet],
        qgl,
        gInstance->jet_bRegCorr_[idxJet],
        gInstance->jet_bRegRes_[idxJet],
        jet_id,
        gInstance->jet_puId_[idxJet],
        idxJet,
        gInstance->jet_genMatchIdx_[idxJet],
        btag_,
        ptMassOption_,
      });
      RecoJetAK4 & jet = jets.back();
      if(isMC_)
      {
        for(const auto & kv: gInstance->jet_BtagWeights_systematics_)
        {
          for(int idxShift = kBtag_central; idxShift <= kBtag_jesRelativeSample_EraDown; ++idxShift)
          {
            if(gInstance->jet_BtagWeights_systematics_.at(kv.first).count(idxShift))
            {
              jet.BtagWeight_systematics_[kv.first][idxShift] = gInstance->jet_BtagWeights_systematics_.at(kv.first).at(idxShift)[idxJet];
            }
          } // idxShift
        } // jet_BtagWeights_systematics_
      } // isMC_
    } // idxJet

    readGenMatching(jets);
  } // nJets > 0
  return jets;
}

void
RecoJetReaderAK4::readGenMatching(std::vector<RecoJetAK4> & jets) const
{
  if(readGenMatching_)
  {
    assert(genLeptonReader_ && genHadTauReader_ && genJetReader_);
    std::size_t nJets = jets.size();

    std::vector<GenLepton> matched_genLeptons = genLeptonReader_->read();
    assert(matched_genLeptons.size() == nJets);

    std::vector<GenHadTau> matched_genHadTaus = genHadTauReader_->read();
    assert(matched_genHadTaus.size() == nJets);

    std::vector<GenJet> matched_genJets = genJetReader_->read();
    assert(matched_genJets.size() == nJets);

    for(std::size_t idxJet = 0; idxJet < nJets; ++idxJet)
    {
      RecoJetAK4 & jet = jets[idxJet];

      const GenLepton & matched_genLepton = matched_genLeptons[idxJet];
      if(matched_genLepton.isValid()) jet.set_genLepton(new GenLepton(matched_genLepton));

      const GenHadTau & matched_genHadTau = matched_genHadTaus[idxJet];
      if(matched_genHadTau.isValid()) jet.set_genHadTau(new GenHadTau(matched_genHadTau));

      const GenJet & matched_genJet = matched_genJets[idxJet];
      if(matched_genJet.isValid()) jet.set_genJet(new GenJet(matched_genJet));
    }
  }
}

std::vector<std::string>
RecoJetReaderAK4::get_supported_systematics(const edm::ParameterSet & cfg)
{
  std::vector<std::string> supported_systematics = {  
    "CMS_ttHl_JESAbsoluteUp",           "CMS_ttHl_JESAbsoluteDown",
    "CMS_ttHl_JESAbsolute_EraUp",       "CMS_ttHl_JESAbsolute_EraDown",
    "CMS_ttHl_JESBBEC1Up",              "CMS_ttHl_JESBBEC1Down",
    "CMS_ttHl_JESBBEC1_EraUp",          "CMS_ttHl_JESBBEC1_EraDown",
    "CMS_ttHl_JESEC2Up",                "CMS_ttHl_JESEC2Down",
    "CMS_ttHl_JESEC2_EraUp",            "CMS_ttHl_JESEC2_EraDown",
    "CMS_ttHl_JESFlavorQCDUp",          "CMS_ttHl_JESFlavorQCDDown",
    "CMS_ttHl_JESHFUp",                 "CMS_ttHl_JESHFDown",
    "CMS_ttHl_JESHF_EraUp",             "CMS_ttHl_JESHF_EraDown",
    "CMS_ttHl_JESRelativeBalUp",        "CMS_ttHl_JESRelativeBalDown",
    "CMS_ttHl_JESRelativeSample_EraUp", "CMS_ttHl_JESRelativeSample_EraDown"
  };
  Era era = get_era(cfg.getParameter<std::string>("era"));
  if ( era == Era::k2018 ) 
  {
    // KE: add systematic uncertainty on jet energy correction that addresses the HEM15/16 issue,
    //     cf. https://hypernews.cern.ch/HyperNews/CMS/get/JetMET/2000.html
    supported_systematics.push_back("CMS_ttHl_JESHEMDown");
  }
  return supported_systematics;
}
