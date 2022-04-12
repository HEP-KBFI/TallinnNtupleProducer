#include "TallinnNtupleProducer/Readers/interface/RecoJetReaderAK4.h"

#include "TallinnNtupleProducer/CommonTools/interface/cmsException.h"         // cmsException()
#include "TallinnNtupleProducer/CommonTools/interface/jetDefinitions.h"       // Btag, kBtag_*
#include "TallinnNtupleProducer/CommonTools/interface/Era.h"                  // Era
#include "TallinnNtupleProducer/CommonTools/interface/sysUncertOptions.h"     // getBranchName_jetMET(), getBranchName_bTagWeight(), getBranchName_jetPtMass()
#include "TallinnNtupleProducer/Readers/interface/BranchAddressInitializer.h" // BranchAddressInitializer
#include "TallinnNtupleProducer/Readers/interface/GenHadTauReader.h"          // GenHadTauReader
#include "TallinnNtupleProducer/Readers/interface/GenLeptonReader.h"          // GenLeptonReader
#include "TallinnNtupleProducer/Readers/interface/GenJetReader.h"             // GenJetReader

#include "TTree.h"                                                            // TTree
#include "TString.h"                                                          // Form()

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
  , ptMassOption_(-1)
  , read_ptMass_systematics_(false)
  , read_btag_systematics_(false)
  , jet_eta_(nullptr)
  , jet_phi_(nullptr)
  , jet_charge_(nullptr)
  , jet_QGDiscr_(nullptr)
  , jet_bRegCorr_(nullptr)
  , jet_bRegRes_(nullptr)
  , jet_pullEta_(nullptr)
  , jet_pullPhi_(nullptr)
  , jet_pullMag_(nullptr)
  , jet_jetId_(nullptr)
  , jet_puId_(nullptr)
  , jet_jetIdx_(nullptr)
  , jet_genMatchIdx_(nullptr)
{
  era_ = get_era(cfg.getParameter<std::string>("era"));
  branchName_obj_ = cfg.getParameter<std::string>("branchName"); // default = "Jet"
  branchName_num_ = Form("n%s", branchName_obj_.data());
  isMC_ = cfg.getParameter<bool>("isMC");
  ptMassOption_ = ( isMC_ ) ? kJetMET_central : kJetMET_central_nonNominal;
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
    delete[] gInstance->jet_jetId_;
    delete[] gInstance->jet_puId_;
    delete[] gInstance->jet_jetIdx_;
    delete[] gInstance->jet_genMatchIdx_;
    for(auto & kv: gInstance->jet_pt_systematics_)
    {
      delete[] kv.second;
    }
    for(auto & kv: gInstance->jet_mass_systematics_)
    {
      delete[] kv.second;
    }
    for(auto & kv: gInstance->jet_BtagCSVs_)
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
    throw cmsException(this, __func__, __LINE__) << "Invalid option for the era = " << static_cast<int>(era_) << ": " << central_or_shift;
  }
  ptMassOption_ = central_or_shift;
}

void
RecoJetReaderAK4::setBranchName_BtagWeight(int central_or_shift)
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
RecoJetReaderAK4::read_ptMass_systematics(bool flag)
{
  read_ptMass_systematics_ = flag;
}

void
RecoJetReaderAK4::read_btag_systematics(bool flag)
{
  read_btag_systematics_ = flag;
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
      if(! isValidJESsource(era_, idxShift))
      {
        continue;
      }
      branchNames_pt_systematics_[idxShift]   = getBranchName_jetMET(branchName_obj_, era_, idxShift, true);
      branchNames_mass_systematics_[idxShift] = getBranchName_jetMET(branchName_obj_, era_, idxShift, false);
    }
    branchName_jetCharge_ = Form("%s_%s", branchName_obj_.data(), "jetCharge");

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
    branchName_jetId_ = Form("%s_%s", branchName_obj_.data(), "jetId");
    branchName_puId_ = Form("%s_%s", branchName_obj_.data(), "puId");
    branchName_jetIdx_ = Form("%s_%s", branchName_obj_.data(), "jetIdx");
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
    bai.setBranchAddress(jet_pt_systematics_[ptMassOption_],   branchNames_pt_systematics_[ptMassOption_]);
    bai.setBranchAddress(jet_mass_systematics_[ptMassOption_], branchNames_mass_systematics_[ptMassOption_]);
    if(isMC_ && read_ptMass_systematics_)
    {
      for(int idxShift = kJetMET_central_nonNominal; idxShift <= kJetMET_jerForwardHighPtDown; ++idxShift)
      {
        if(! isValidJESsource(era_, idxShift))
        {
          continue;
        }
        if(idxShift == ptMassOption_)
        {
          continue; // do not bind the same branch twice
        }
        bai.setBranchAddress(jet_pt_systematics_[idxShift],   branchNames_pt_systematics_[idxShift]);
        bai.setBranchAddress(jet_mass_systematics_[idxShift], branchNames_mass_systematics_[idxShift]);
      }
    }
    bai.setBranchAddress(nJets_, branchName_num_);
    bai.setBranchAddress(jet_eta_, branchName_eta_);
    bai.setBranchAddress(jet_phi_, branchName_phi_);
    bai.setBranchAddress(jet_charge_, branchName_jetCharge_);
    for(const auto & kv: branchNames_btag_)
    {
      bai.setBranchAddress(jet_BtagCSVs_[kv.first], kv.second, -3.);
    }

    for(const auto & kv: branchNames_BtagWeight_systematics_)
    {
      for(int idxShift = kBtag_central; idxShift <= kBtag_jesRelativeSample_EraDown; ++idxShift)
      {
        if(read_btag_systematics_ || (! read_btag_systematics_ && idxShift == btag_central_or_shift_))
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
    bai.setBranchAddress(jet_pullEta_, branchName_pullEta_);
    bai.setBranchAddress(jet_pullPhi_, branchName_pullPhi_);
    bai.setBranchAddress(jet_pullMag_, branchName_pullMag_);
    bai.setBranchAddress(jet_jetId_, branchName_jetId_);
    bai.setBranchAddress(jet_puId_, branchName_puId_);
    bai.setBranchAddress(jet_jetIdx_, branchName_jetIdx_);
    bai.setBranchAddress(jet_genMatchIdx_, isMC_ && branchName_obj_ == "Jet" ? branchName_genMatchIdx_ : "", -1);

    const std::vector<std::string> recoJetBranches = bai.getBoundBranchNames();
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
      double btagCSV = gInstance->jet_BtagCSVs_.at(btag_)[idxJet];
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
        gInstance->jet_pullEta_[idxJet],
        gInstance->jet_pullPhi_[idxJet],
        gInstance->jet_pullMag_[idxJet],
        jet_id,
        gInstance->jet_puId_[idxJet],
        gInstance->jet_genMatchIdx_[idxJet],
        gInstance->jet_jetIdx_[idxJet],
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
            if(! read_btag_systematics_ && idxShift != btag_central_or_shift_)
            {
              continue;
            }
            if(gInstance->jet_BtagWeights_systematics_.at(kv.first).count(idxShift))
            {
              jet.BtagWeight_systematics_[kv.first][idxShift] = gInstance->jet_BtagWeights_systematics_.at(kv.first).at(idxShift)[idxJet];
            }
          } // idxShift
        } // jet_BtagWeights_systematics_
      } // isMC_

      for(const auto & kv: gInstance->jet_BtagCSVs_)
      {
        double btagCSV_tmp = gInstance->jet_BtagCSVs_.at(kv.first)[idxJet];
        if(std::isnan(btagCSV_tmp))
        {
          btagCSV_tmp = -2;
        }
        jet.BtagCSVs_[kv.first] = btagCSV_tmp;
      }

      if(isMC_ && read_ptMass_systematics_)
      {
        for(int idxShift = kJetMET_central_nonNominal; idxShift <= kJetMET_jerForwardHighPtDown; ++idxShift)
        {
          if(! isValidJESsource(era_, idxShift))
          {
            continue;
          }
          // we want to save all pT-s and masses that have been shifted by systematic uncertainties to the maps,
          // including the central nominal and central non-nominal values; crucial for RecoJetWriter
          jet.pt_systematics_[idxShift]   = gInstance->jet_pt_systematics_.at(idxShift)[idxJet];
          jet.mass_systematics_[idxShift] = gInstance->jet_mass_systematics_.at(idxShift)[idxJet];
        } // idxShift
      }
      else
      {
        // fill the maps with only the central values (either nominal or non-nominal if data)
        jet.pt_systematics_[ptMassOption_]   = gInstance->jet_pt_systematics_.at(ptMassOption_)[idxJet];
        jet.mass_systematics_[ptMassOption_] = gInstance->jet_mass_systematics_.at(ptMassOption_)[idxJet];
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
