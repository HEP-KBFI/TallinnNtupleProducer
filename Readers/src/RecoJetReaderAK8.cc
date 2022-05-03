#include "TallinnNtupleProducer/Readers/interface/RecoJetReaderAK8.h"

#include "TallinnNtupleProducer/CommonTools/interface/BranchAddressInitializer.h" // BranchAddressInitializer
#include "TallinnNtupleProducer/CommonTools/interface/cmsException.h"             // cmsException()
#include "TallinnNtupleProducer/CommonTools/interface/Era.h"                      // Era, get_era()
#include "TallinnNtupleProducer/CommonTools/interface/get_ignore_ak8_sys.h"       // get_ignore_ak8_sys()
#include "TallinnNtupleProducer/CommonTools/interface/jetDefinitions.h"           // Btag, kBtag_*
#include "TallinnNtupleProducer/CommonTools/interface/merge_systematic_shifts.h"  // merge_systematic_shifts()
#include "TallinnNtupleProducer/CommonTools/interface/sysUncertOptions.h"         // getBranchName_fatJet(), kFatJet_*
#include "TallinnNtupleProducer/Objects/interface/RecoSubjetAK8.h"                // RecoSubjetAK8
#include "TallinnNtupleProducer/Readers/interface/RecoSubjetReaderAK8.h"          // RecoSubjetReaderAK8

#include "TTree.h"                                                                // TTree
#include "TString.h"                                                              // Form()

std::map<std::string, int> RecoJetReaderAK8::numInstances_;
std::map<std::string, RecoJetReaderAK8 *> RecoJetReaderAK8::instances_;

RecoJetReaderAK8::RecoJetReaderAK8(const edm::ParameterSet & cfg)
  : ReaderBase(cfg)
  , era_(Era::kUndefined)
  , isMC_(false)
  , max_nJets_(32)
  , branchName_num_("")
  , branchName_obj_("")
  , subjetReader_(nullptr)
  , sysOption_central_(-1)
  , sysOption_(-1)
  , ignoreSys_(-1)
  , pt_str_("pt")
  , mass_str_("mass")
  , msoftdrop_str_("msoftdrop")
  , jet_eta_(nullptr)
  , jet_phi_(nullptr)
  , subjet_idx1_(nullptr)
  , subjet_idx2_(nullptr)
  , jet_tau1_(nullptr)
  , jet_tau2_(nullptr)
  , jet_tau3_(nullptr)
  , jet_tau4_(nullptr)
  , jet_jetId_(nullptr)
{
  era_ = get_era(cfg.getParameter<std::string>("era"));
  branchName_obj_ = cfg.getParameter<std::string>("branchName_jet"); // default = "FatJet"
  branchName_num_ = Form("n%s", branchName_obj_.data());
  isMC_ = cfg.getParameter<bool>("isMC");
  sysOption_central_ = ( isMC_ ) ? kFatJet_central : kFatJet_central_nonNominal;
  sysOption_ = sysOption_central_;
  ignoreSys_ = ( isMC_ ) ? kFatJetJMS + kFatJetJMR + kFatJetPUPPI : kFatJetNone;
  std::string branchName_subjet = cfg.getParameter<std::string>("branchName_subjet"); // default = "SubJet"
  subjetReader_ = new RecoSubjetReaderAK8(cfg);
  setBranchNames();
}

RecoJetReaderAK8::~RecoJetReaderAK8()
{
  --numInstances_[branchName_obj_];
  assert(numInstances_[branchName_obj_] >= 0);
  if(numInstances_[branchName_obj_] == 0)
  {
    RecoJetReaderAK8 * const gInstance = instances_[branchName_obj_];
    assert(gInstance);
    delete gInstance->subjetReader_;
    delete[] gInstance->jet_eta_;
    delete[] gInstance->jet_phi_;
    delete[] gInstance->subjet_idx1_;
    delete[] gInstance->subjet_idx2_;
    delete[] gInstance->jet_tau1_;
    delete[] gInstance->jet_tau2_;
    delete[] gInstance->jet_tau3_;
    delete[] gInstance->jet_tau4_;
    delete[] gInstance->jet_jetId_;
    for(auto & kv: gInstance->jet_pt_systematics_)
    {
      delete[] kv.second;
    }
    for(auto & kv: gInstance->jet_mass_systematics_)
    {
      delete[] kv.second;
    }
    for(auto & kv: gInstance->jet_msoftdrop_systematics_)
    {
      delete[] kv.second;
    }
    for(auto & kv: gInstance->jet_mass_corr_)
    {
      delete[] kv.second;
    }
    for(auto & kv: gInstance->jet_sd_corr_)
    {
      delete[] kv.second;
    }
    instances_[branchName_obj_] = nullptr;
  }
}

void
RecoJetReaderAK8::set_central_or_shift(int central_or_shift)
{
  if(! isMC_ && central_or_shift != kFatJet_central_nonNominal)
  {
    throw cmsException(this, __func__, __LINE__) << "Data has only non-nominal attributes";
  }
  if(! isValidJESsource(era_, central_or_shift, true))
  {
    sysOption_ = sysOption_central_;
  }
  if(((ignoreSys_ & kFatJetJMS) && (central_or_shift == kFatJet_jmsUp || central_or_shift == kFatJet_jmsDown)) ||
     ((ignoreSys_ & kFatJetJMR) && (central_or_shift == kFatJet_jmrUp || central_or_shift == kFatJet_jmrDown))  )
  {
    throw cmsException(this, __func__, __LINE__)
      << "Requested systematics " << central_or_shift << " while at the same time ignoring its corrections: " << ignoreSys_
    ;
  }
  sysOption_ = central_or_shift;
}

void
RecoJetReaderAK8::ignoreSys(int flag)
{
  if(((flag & kFatJetJMS) && (sysOption_ == kFatJet_jmsUp || sysOption_ == kFatJet_jmsDown)) ||
     ((flag & kFatJetJMR) && (sysOption_ == kFatJet_jmrUp || sysOption_ == kFatJet_jmrDown))  )
  {
    throw cmsException(this, __func__, __LINE__)
      << "Requested to ignore corrections (" << flag << ") while at the same time enabling its systematics: " << sysOption_
    ;
  }
  ignoreSys_ = flag;
}

namespace
{
  std::string
  getCorrectionString(int code)
  {
    switch(code)
    {
      case kFatJetJMS:   return "JMS";
      case kFatJetJMR:   return "JMR";
      case kFatJetPUPPI: return "PUPPI";
      default:           assert(false);
    }
    assert(false);
  }

  double
  updateWithCorrections(double input,
                        int flag,
                        const std::map<int, double> & corrections,
                        bool undo)
  {
    double factor = 1.;
    for(const auto & kv: corrections)
    {
      if(flag & kv.first)
      {
        factor *= kv.second;
      }
    }
    if(undo)
    {
      factor = std::fpclassify(factor) != FP_ZERO ? 1. / factor : 0.;
    }
    return input * factor;
  }
}

void
RecoJetReaderAK8::setBranchNames()
{
  if(numInstances_[branchName_obj_] == 0)
  {
    for(int idxShift = kFatJet_central_nonNominal; idxShift <= kFatJet_jmrDown; ++idxShift)
    {
      if(! isValidJESsource(era_, idxShift, true))
      {
        continue;
      }
      if(isValidFatJetAttribute(idxShift, pt_str_))
      {
        branchNames_pt_systematics_[idxShift] = getBranchName_fatJet(branchName_obj_, era_, pt_str_, idxShift);
      }
      if(isValidFatJetAttribute(idxShift, mass_str_))
      {
        branchNames_mass_systematics_[idxShift] = getBranchName_fatJet(branchName_obj_, era_, mass_str_, idxShift);
      }
      if(isValidFatJetAttribute(idxShift, msoftdrop_str_))
      {
        branchNames_msoftdrop_systematics_[idxShift] = getBranchName_fatJet(branchName_obj_, era_, msoftdrop_str_, idxShift);
      }
    }
    branchName_eta_ = Form("%s_%s", branchName_obj_.data(), "eta");
    branchName_phi_ = Form("%s_%s", branchName_obj_.data(), "phi");
    branchName_subJetIdx1_ = Form("%s_%s", branchName_obj_.data(), "subJetIdx1");
    branchName_subJetIdx2_ = Form("%s_%s", branchName_obj_.data(), "subJetIdx2");
    branchName_tau1_ = Form("%s_%s", branchName_obj_.data(), "tau1");
    branchName_tau2_ = Form("%s_%s", branchName_obj_.data(), "tau2");
    branchName_tau3_ = Form("%s_%s", branchName_obj_.data(), "tau3");
    branchName_tau4_ = Form("%s_%s", branchName_obj_.data(), "tau4");
    branchName_jetId_ = Form("%s_%s", branchName_obj_.data(), "jetId");

    for(int idxShift = 0; idxShift < 3; ++idxShift)
    {
      const int idxShift_pow = 1 << idxShift;
      const std::string suffix = getCorrectionString(idxShift_pow);
      branchNames_sd_corr_[idxShift_pow] = Form("%s_%s_corr_%s", branchName_obj_.data(), msoftdrop_str_.data(), suffix.data());

      if(! (idxShift_pow & kFatJetPUPPI))
      {
        branchNames_mass_corr_[idxShift_pow] = Form("%s_corr_%s", branchName_obj_.data(), suffix.data());
      }
    }

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
RecoJetReaderAK8::setBranchAddresses(TTree * tree)
{
  std::vector<std::string> bound_branches;
  if(instances_[branchName_obj_] == this)
  {
    BranchAddressInitializer bai(tree, max_nJets_);
    const std::vector<std::string> subjetBranches = subjetReader_->setBranchAddresses(tree);
    bound_branches.insert(bound_branches.end(), subjetBranches.begin(), subjetBranches.end());

    bai.setBranchAddress(nJets_, branchName_num_);
    bai.setBranchAddress(jet_pt_systematics_[sysOption_central_],        branchNames_pt_systematics_.at(sysOption_central_));
    bai.setBranchAddress(jet_mass_systematics_[sysOption_central_],      branchNames_mass_systematics_.at(sysOption_central_));
    bai.setBranchAddress(jet_msoftdrop_systematics_[sysOption_central_], branchNames_msoftdrop_systematics_.at(sysOption_central_));
    if(isMC_)
    {
      for(int idxShift = kFatJet_central_nonNominal; idxShift <= kFatJet_jmrDown; ++idxShift)
      {
        if(! isValidJESsource(era_, idxShift, true))
        {
          continue;
        }
        if(idxShift == sysOption_central_)
        {
          continue; // do not bind the same branch twice
        }
        if(branchNames_pt_systematics_.count(idxShift))
        {
          bai.setBranchAddress(jet_pt_systematics_[idxShift], branchNames_pt_systematics_.at(idxShift));
        }
        if(branchNames_mass_systematics_.count(idxShift))
        {
          bai.setBranchAddress(jet_mass_systematics_[idxShift], branchNames_mass_systematics_.at(idxShift));
        }
        if(branchNames_msoftdrop_systematics_.count(idxShift))
        {
          bai.setBranchAddress(jet_msoftdrop_systematics_[idxShift], branchNames_msoftdrop_systematics_.at(idxShift));
        }
      }

      for(int idxShift = 0; idxShift < 3; ++idxShift)
      {
        const int idxShift_pow = 1 << idxShift;
        if(branchNames_sd_corr_.count(idxShift_pow))
        {
          bai.setBranchAddress(jet_sd_corr_[idxShift_pow], branchNames_sd_corr_.at(idxShift_pow));
        }
        if(branchNames_mass_corr_.count(idxShift_pow))
        {
          bai.setBranchAddress(jet_mass_corr_[idxShift_pow], branchNames_mass_corr_.at(idxShift_pow));
        }
      }
    }
    assert(jet_pt_systematics_.count(sysOption_central_));
    assert(jet_mass_systematics_.count(sysOption_central_));
    assert(jet_msoftdrop_systematics_.count(sysOption_central_));
    bai.setBranchAddress(jet_eta_, branchName_eta_);
    bai.setBranchAddress(jet_phi_, branchName_phi_);
    bai.setBranchAddress(subjet_idx1_, branchName_subJetIdx1_);
    bai.setBranchAddress(subjet_idx2_, branchName_subJetIdx2_);
    bai.setBranchAddress(jet_tau1_, branchName_tau1_);
    bai.setBranchAddress(jet_tau2_, branchName_tau2_);
    bai.setBranchAddress(jet_tau3_, branchName_tau3_);
    bai.setBranchAddress(jet_tau4_, branchName_tau4_);
    bai.setBranchAddress(jet_jetId_, branchName_jetId_);

    const std::vector<std::string> recoFatJetBranches = bai.getBoundBranchNames_read();
    bound_branches.insert(bound_branches.end(), recoFatJetBranches.begin(), recoFatJetBranches.end());
  }
  return bound_branches;
}

namespace
{
  const RecoSubjetAK8 *
  getSubjet(const std::vector<RecoSubjetAK8> & subjets,
            int idx)
  {
    if(idx == -1)
    {
      return nullptr;
    }
    else if(idx >= 0 && idx < (int)subjets.size())
    {
      return &subjets[idx];
    }
    else
    {
      throw cmsException(__func__, __LINE__)
        << "Invalid subjet index = " << idx << ", given number of subjets = " << subjets.size() << " !!\n";
    }
  }
}

std::vector<RecoJetAK8>
RecoJetReaderAK8::read() const
{
  const RecoJetReaderAK8 * const gInstance = instances_[branchName_obj_];
  assert(gInstance);

  std::vector<RecoJetAK8> jets;
  const UInt_t nJets = gInstance->nJets_;
  if(nJets > max_nJets_)
  {
    throw cmsException(this)
      << "Number of jets stored in Ntuple = " << nJets << ", exceeds max_nJets = " << max_nJets_ << " !!\n";
  }

  if(nJets > 0)
  {
    jets.reserve(nJets);
    std::vector<RecoSubjetAK8> subjets = subjetReader_->read();
    for(UInt_t idxJet = 0; idxJet < nJets; ++idxJet)
    {
      const RecoSubjetAK8 * subJet1 = subjets.size() > 0 ? getSubjet(subjets, gInstance->subjet_idx1_[idxJet]) : nullptr;
      const RecoSubjetAK8 * subJet2 = subjets.size() > 1 ? getSubjet(subjets, gInstance->subjet_idx2_[idxJet]) : nullptr;
      const int jet_pt_sys        = gInstance->jet_pt_systematics_.count(sysOption_)        ? sysOption_ : sysOption_central_;
      const int jet_mass_sys      = gInstance->jet_mass_systematics_.count(sysOption_)      ? sysOption_ : sysOption_central_;
      const int jet_msoftdrop_sys = gInstance->jet_msoftdrop_systematics_.count(sysOption_) ? sysOption_ : sysOption_central_;

      std::map<int, double> jet_mass_corr;
      for(const auto & kv: gInstance->jet_mass_corr_)
      {
        jet_mass_corr[kv.first] = kv.second[idxJet];
      }
      std::map<int, double> jet_sd_corr;
      for(const auto & kv: gInstance->jet_sd_corr_)
      {
        jet_sd_corr[kv.first] = kv.second[idxJet];
      }
      const double jet_mass = updateWithCorrections(
        gInstance->jet_mass_systematics_.at(jet_mass_sys)[idxJet], ignoreSys_, jet_mass_corr, true
      );
      const double jet_msoftdrop = updateWithCorrections(
        gInstance->jet_msoftdrop_systematics_.at(jet_msoftdrop_sys)[idxJet], ignoreSys_, jet_sd_corr, true
      );

      jets.push_back({
        {
          gInstance->jet_pt_systematics_.at(jet_pt_sys)[idxJet],
          gInstance->jet_eta_[idxJet],
          gInstance->jet_phi_[idxJet],
          jet_mass,
        },
        jet_msoftdrop,
        subJet1 ? new RecoSubjetAK8(*subJet1) : nullptr,
        subJet2 ? new RecoSubjetAK8(*subJet2) : nullptr,
        gInstance->jet_tau1_[idxJet],
        gInstance->jet_tau2_[idxJet],
        gInstance->jet_tau3_[idxJet],
        gInstance->jet_tau4_[idxJet],
        gInstance->jet_jetId_[idxJet],
        static_cast<Int_t>(idxJet),
        sysOption_,
        ignoreSys_,
      });

      RecoJetAK8 & jet = jets.back();
      if(isMC_)
      {
        jet.mass_corrections_ = jet_mass_corr;
        jet.sd_corrections_ = jet_sd_corr;

        for(int idxShift = kFatJet_central_nonNominal; idxShift <= kFatJet_jmrDown; ++idxShift)
        {
          if(! isValidJESsource(era_, idxShift, true))
          {
            continue;
          }
          // we want to save all pT-s and masses that have been shifted by systematic uncertainties to the maps,
          // including the central nominal and central non-nominal values; crucial for RecoJetWriter
          const int jet_pt_sys_local        = gInstance->jet_pt_systematics_.count(idxShift)        ? idxShift : sysOption_central_;
          const int jet_mass_sys_local      = gInstance->jet_mass_systematics_.count(idxShift)      ? idxShift : sysOption_central_;
          const int jet_msoftdrop_sys_local = gInstance->jet_msoftdrop_systematics_.count(idxShift) ? idxShift : sysOption_central_;
          jet.pt_systematics_[idxShift]        = gInstance->jet_pt_systematics_.at(jet_pt_sys_local)[idxJet];
          jet.mass_systematics_[idxShift]      = gInstance->jet_mass_systematics_.at(jet_mass_sys_local)[idxJet];
          jet.msoftdrop_systematics_[idxShift] = gInstance->jet_msoftdrop_systematics_.at(jet_msoftdrop_sys_local)[idxJet];
        } // idxShift
      } // isMC_
    } // idxJet
  } // nJets > 0

  return jets;
}

std::vector<std::string>
RecoJetReaderAK8::get_supported_systematics(const edm::ParameterSet & cfg)
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
  const std::vector<std::string> disable_ak8_corr = cfg.getParameter<std::vector<std::string>>("disable_ak8_corr");
  const int ignore_ak8_sys = get_ignore_ak8_sys(disable_ak8_corr);
  if ( !(ignore_ak8_sys & kFatJetJMS) )
  {
    merge_systematic_shifts(supported_systematics, { "CMS_ttHl_AK8JMSUp", "CMS_ttHl_AK8JMSDown" });
  }
  if ( !(ignore_ak8_sys & kFatJetJMR) )
  {
    merge_systematic_shifts(supported_systematics, { "CMS_ttHl_AK8JMRUp", "CMS_ttHl_AK8JMRDown" });
  }
  return supported_systematics;
}
