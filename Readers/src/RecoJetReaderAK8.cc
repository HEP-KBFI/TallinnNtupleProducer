#include "TallinnNtupleProducer/Readers/interface/RecoJetReaderAK8.h"

#include "TallinnNtupleProducer/CommonTools/interface/BranchAddressInitializer.h" // BranchAddressInitializer
#include "TallinnNtupleProducer/CommonTools/interface/cmsException.h"             // cmsException()
#include "TallinnNtupleProducer/CommonTools/interface/Era.h"                      // Era, get_era()
#include "TallinnNtupleProducer/CommonTools/interface/get_ignore_ak8_sys.h"       // get_ignore_ak8_sys()
#include "TallinnNtupleProducer/CommonTools/interface/jetDefinitions.h"           // Btag, kBtag_*
#include "TallinnNtupleProducer/CommonTools/interface/map_keys.h"                 // map_keys()
#include "TallinnNtupleProducer/CommonTools/interface/merge_systematic_shifts.h"  // merge_systematic_shifts()
#include "TallinnNtupleProducer/CommonTools/interface/sysUncertOptions.h"         // jesAK8SysMap, jerAK8SysMap, jesSplitAK8SysMap, jerSplitAK8SysMap, jmsAK8SysMap, jmrAK8SysMap
#include "TallinnNtupleProducer/Objects/interface/RecoSubjetAK8.h"                // RecoSubjetAK8
#include "TallinnNtupleProducer/Readers/interface/RecoSubjetReaderAK8.h"          // RecoSubjetReaderAK8

#include "TTree.h"                                                                // TTree
#include "TString.h"                                                              // Form()

std::map<std::string, int> RecoJetReaderAK8::numInstances_;
std::map<std::string, RecoJetReaderAK8 *> RecoJetReaderAK8::instances_;

RecoJetReaderAK8::RecoJetReaderAK8(const edm::ParameterSet & cfg)
  : ReaderBase(cfg)
  , era_(get_era(cfg.getParameter<std::string>("era")))
  , isMC_(cfg.getParameter<bool>("isMC"))
  , max_nJets_(32)
  , branchName_obj_(cfg.getParameter<std::string>("branchName_jet"))
  , branchName_num_(Form("n%s", branchName_obj_.data()))
  , subjetReader_(new RecoSubjetReaderAK8(cfg))
  , jet_pt_(nullptr)
  , jet_eta_(nullptr)
  , jet_phi_(nullptr)
  , jet_mass_(nullptr)
  , jet_msoftdrop_(nullptr)
  , subjet_idx1_(nullptr)
  , subjet_idx2_(nullptr)
  , jet_tau1_(nullptr)
  , jet_tau2_(nullptr)
  , jet_tau3_(nullptr)
  , jet_tau4_(nullptr)
  , jet_jetId_(nullptr)
  , jet_rawFactor_(nullptr)
  , jet_area_(nullptr)
{
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
    delete[] gInstance->jet_pt_;
    delete[] gInstance->jet_eta_;
    delete[] gInstance->jet_phi_;
    delete[] gInstance->jet_mass_;
    delete[] gInstance->jet_msoftdrop_;
    delete[] gInstance->subjet_idx1_;
    delete[] gInstance->subjet_idx2_;
    delete[] gInstance->jet_tau1_;
    delete[] gInstance->jet_tau2_;
    delete[] gInstance->jet_tau3_;
    delete[] gInstance->jet_tau4_;
    delete[] gInstance->jet_jetId_;
    delete[] gInstance->jet_rawFactor_;
    delete[] gInstance->jet_area_;
    instances_[branchName_obj_] = nullptr;
  }
}

void
RecoJetReaderAK8::setBranchNames()
{
  if(numInstances_[branchName_obj_] == 0)
  {
    branchName_pt_ = Form("%s_%s", branchName_obj_.data(), "pt");
    branchName_eta_ = Form("%s_%s", branchName_obj_.data(), "eta");
    branchName_phi_ = Form("%s_%s", branchName_obj_.data(), "phi");
    branchName_mass_ = Form("%s_%s", branchName_obj_.data(), "mass");
    branchName_msoftdrop_ = Form("%s_%s", branchName_obj_.data(), "msoftdrop");
    branchName_subJetIdx1_ = Form("%s_%s", branchName_obj_.data(), "subJetIdx1");
    branchName_subJetIdx2_ = Form("%s_%s", branchName_obj_.data(), "subJetIdx2");
    branchName_tau1_ = Form("%s_%s", branchName_obj_.data(), "tau1");
    branchName_tau2_ = Form("%s_%s", branchName_obj_.data(), "tau2");
    branchName_tau3_ = Form("%s_%s", branchName_obj_.data(), "tau3");
    branchName_tau4_ = Form("%s_%s", branchName_obj_.data(), "tau4");
    branchName_jetId_ = Form("%s_%s", branchName_obj_.data(), "jetId");
    branchName_rawFactor_ = Form("%s_%s", branchName_obj_.data(), "rawFactor");
    branchName_area_ = Form("%s_%s", branchName_obj_.data(), "area");
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
    bai.setBranchAddress(jet_pt_, branchName_pt_);
    bai.setBranchAddress(jet_eta_, branchName_eta_);
    bai.setBranchAddress(jet_phi_, branchName_phi_);
    bai.setBranchAddress(jet_mass_, branchName_mass_);
    bai.setBranchAddress(jet_msoftdrop_, branchName_msoftdrop_);
    bai.setBranchAddress(subjet_idx1_, branchName_subJetIdx1_);
    bai.setBranchAddress(subjet_idx2_, branchName_subJetIdx2_);
    bai.setBranchAddress(jet_tau1_, branchName_tau1_);
    bai.setBranchAddress(jet_tau2_, branchName_tau2_);
    bai.setBranchAddress(jet_tau3_, branchName_tau3_);
    bai.setBranchAddress(jet_tau4_, branchName_tau4_);
    bai.setBranchAddress(jet_jetId_, branchName_jetId_);
    bai.setBranchAddress(jet_rawFactor_, branchName_rawFactor_);
    bai.setBranchAddress(jet_area_, branchName_area_);

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

      jets.push_back({
        {
          gInstance->jet_pt_[idxJet],
          gInstance->jet_eta_[idxJet],
          gInstance->jet_phi_[idxJet],
          gInstance->jet_mass_[idxJet],
        },
        gInstance->jet_msoftdrop_[idxJet],
        subJet1 ? new RecoSubjetAK8(*subJet1) : nullptr,
        subJet2 ? new RecoSubjetAK8(*subJet2) : nullptr,
        gInstance->jet_tau1_[idxJet],
        gInstance->jet_tau2_[idxJet],
        gInstance->jet_tau3_[idxJet],
        gInstance->jet_tau4_[idxJet],
        gInstance->jet_jetId_[idxJet],
        gInstance->jet_rawFactor_[idxJet],
        gInstance->jet_area_[idxJet],
        static_cast<UInt_t>(idxJet),
      });
    } // idxJet
  } // nJets > 0

  return jets;
}

std::vector<std::string>
RecoJetReaderAK8::get_supported_systematics(const edm::ParameterSet & cfg)
{
  static std::vector<std::string> supported_systematics;
  if(supported_systematics.empty())
  {
    merge_systematic_shifts(supported_systematics, map_keys(jesAK8SysMap));
    merge_systematic_shifts(supported_systematics, map_keys(jerAK8SysMap));
    const bool split_jes = cfg.getParameter<bool>("split_jes");
    if(split_jes)
    {
      std::vector<std::string> jesSplitAK4SysStrs = map_keys(jesSplitAK8SysMap);
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
    const bool split_jer = cfg.getParameter<bool>("split_jer");
    if(split_jer)
    {
      merge_systematic_shifts(supported_systematics, map_keys(jerSplitAK8SysMap));
    }
    const std::vector<std::string> disable_ak8_corr = cfg.getParameter<std::vector<std::string>>("disable_ak8_corr");
    const int ignore_ak8_sys = get_ignore_ak8_sys(disable_ak8_corr);
    if ( !(ignore_ak8_sys & kFatJetJMS) )
    {
      merge_systematic_shifts(supported_systematics, map_keys(jmsAK8SysMap));
    }
    if ( !(ignore_ak8_sys & kFatJetJMR) )
    {
      merge_systematic_shifts(supported_systematics, map_keys(jmrAK8SysMap));
    }
  }
  return supported_systematics;
}
