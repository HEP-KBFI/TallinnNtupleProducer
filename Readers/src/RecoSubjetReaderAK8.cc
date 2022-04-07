#include "TallinnNtupleProducer/Readers/interface/RecoSubjetReaderAK8.h"

#include "TallinnNtupleProducer/CommonTools/interface/cmsException.h"         // cmsException()
#include "TallinnNtupleProducer/CommonTools/interface/jetDefinitions.h"       // Btag, kBtag_*
#include "TallinnNtupleProducer/CommonTools/interface/Era.h"                  // Era
#include "TallinnNtupleProducer/Readers/interface/BranchAddressInitializer.h" // BranchAddressInitializer

#include "TTree.h"                                                            // TTree
#include "TString.h"                                                          // Form()

std::map<std::string, int> RecoSubjetReaderAK8::numInstances_;
std::map<std::string, RecoSubjetReaderAK8 *> RecoSubjetReaderAK8::instances_;

RecoSubjetReaderAK8::RecoSubjetReaderAK8(const edm::ParameterSet & cfg)
  : era_(Era::kUndefined)
  , btag_(Btag::kDeepCSV) // maybe read DeepJet ?
  , max_nJets_(88)
  , branchName_num_("")
  , branchName_obj_("")
  , jet_pt_(nullptr)
  , jet_eta_(nullptr)
  , jet_phi_(nullptr)
  , jet_mass_(nullptr)
  , jet_BtagCSV_(nullptr)
{
  era_ = get_era(cfg.getParameter<std::string>("era"));
  branchName_obj_ = cfg.getParameter<std::string>("branchName_subjet"); // default = "SubJet"
  branchName_num_ = Form("n%s", branchName_obj_.data());
  switch(btag_)
  {
    case Btag::kDeepCSV: branchName_btag_ = "DeepB";     break;
    case Btag::kDeepJet: branchName_btag_ = "DeepFlavB"; break;
    case Btag::kCSVv2:   branchName_btag_ = "CSVV2";     break;
  }
  assert(! branchName_btag_.empty());
  setBranchNames();
}

RecoSubjetReaderAK8::~RecoSubjetReaderAK8()
{
  --numInstances_[branchName_obj_];
  assert(numInstances_[branchName_obj_] >= 0);
  if(numInstances_[branchName_obj_] == 0)
  {
    RecoSubjetReaderAK8 * const gInstance = instances_[branchName_obj_];
    assert(gInstance);
    delete[] gInstance->jet_pt_;
    delete[] gInstance->jet_eta_;
    delete[] gInstance->jet_phi_;
    delete[] gInstance->jet_mass_;
    delete[] gInstance->jet_BtagCSV_;
    instances_[branchName_obj_] = nullptr;
  }
}

void
RecoSubjetReaderAK8::setBranchNames()
{
  if(numInstances_[branchName_obj_] == 0)
  {
    branchName_pt_ = Form("%s_%s", branchName_obj_.data(), "pt");
    branchName_eta_ = Form("%s_%s", branchName_obj_.data(), "eta");
    branchName_phi_ = Form("%s_%s", branchName_obj_.data(), "phi");
    branchName_mass_ = Form("%s_%s", branchName_obj_.data(), "mass");
    branchName_BtagCSV_ = Form("%s_%s", branchName_obj_.data(), Form("btag%s", branchName_btag_.data()));
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
RecoSubjetReaderAK8::setBranchAddresses(TTree * tree)
{
  if(instances_[branchName_obj_] == this)
  {
    BranchAddressInitializer bai(tree, max_nJets_);
    bai.setBranchAddress(nJets_, branchName_num_);
    bai.setBranchAddress(jet_pt_, branchName_pt_);
    bai.setBranchAddress(jet_eta_, branchName_eta_);
    bai.setBranchAddress(jet_phi_, branchName_phi_);
    bai.setBranchAddress(jet_mass_, branchName_mass_);
    bai.setBranchAddress(jet_BtagCSV_, branchName_BtagCSV_);
    return bai.getBoundBranchNames();
  }
  return {};
}

std::vector<RecoSubjetAK8>
RecoSubjetReaderAK8::read() const
{
  const RecoSubjetReaderAK8 * const gInstance = instances_[branchName_obj_];
  assert(gInstance);

  std::vector<RecoSubjetAK8> jets;
  const UInt_t nJets = gInstance->nJets_;
  if(nJets > max_nJets_)
  {
    throw cmsException(this, __func__, __LINE__)
      << "Number of jets stored in Ntuple = " << nJets << ", exceeds max_nJets = " << max_nJets_;
  }

  if(nJets > 0)
  {
    jets.reserve(nJets);
    for(UInt_t idxJet = 0; idxJet < nJets; ++idxJet)
    {
      jets.push_back({
        {
          gInstance->jet_pt_[idxJet],
          gInstance->jet_eta_[idxJet],
          gInstance->jet_phi_[idxJet],
          gInstance->jet_mass_[idxJet]
        },
        gInstance->jet_BtagCSV_[idxJet],
        static_cast<Int_t>(idxJet)
      });
    } // idxJet
  } // nJets > 0
  return jets;
}
