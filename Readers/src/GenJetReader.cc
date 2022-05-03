#include "TallinnNtupleProducer/Readers/interface/GenJetReader.h"

#include "TallinnNtupleProducer/CommonTools/interface/BranchAddressInitializer.h" // BranchAddressInitializer
#include "TallinnNtupleProducer/CommonTools/interface/cmsException.h"             // cmsException()

#include "TString.h"                                                              // Form()
#include "TTree.h"                                                                // TTree

std::map<std::string, int> GenJetReader::numInstances_;
std::map<std::string, GenJetReader *> GenJetReader::instances_;

GenJetReader::GenJetReader(const edm::ParameterSet & cfg)
  : ReaderBase(cfg)
  , read_partonFlavour_(false)
  , max_nJets_(36)
  , branchName_num_("")
  , branchName_obj_("")
  , jet_pt_(nullptr)
  , jet_eta_(nullptr)
  , jet_phi_(nullptr)
  , jet_mass_(nullptr)
  , jet_pdgId_(nullptr)
{
  if ( cfg.exists("max_nJets") )
  {
    max_nJets_ = cfg.getParameter<unsigned int>("max_nJets");
  }
  branchName_obj_ = cfg.getParameter<std::string>("branchName");
  branchName_num_ = Form("n%s", branchName_obj_.data());
  setBranchNames();
}

GenJetReader::~GenJetReader()
{
  --numInstances_[branchName_obj_];
  assert(numInstances_[branchName_obj_] >= 0);
  if(numInstances_[branchName_obj_] == 0)
  {
    GenJetReader * const gInstance = instances_[branchName_obj_];
    assert(gInstance);
    delete[] gInstance->jet_pt_;
    delete[] gInstance->jet_eta_;
    delete[] gInstance->jet_phi_;
    delete[] gInstance->jet_mass_;
    delete[] gInstance->jet_pdgId_;
    instances_[branchName_obj_] = nullptr;
  }
}

void
GenJetReader::setBranchNames()
{
  if(numInstances_[branchName_obj_] == 0)
  {
    branchName_pt_ = Form("%s_%s", branchName_obj_.data(), "pt");
    branchName_eta_ = Form("%s_%s", branchName_obj_.data(), "eta");
    branchName_phi_ = Form("%s_%s", branchName_obj_.data(), "phi");
    branchName_mass_ = Form("%s_%s", branchName_obj_.data(), "mass");
    branchName_pdgId_ = Form("%s_%s", branchName_obj_.data(), "pdgId");
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

void
GenJetReader::read_partonFlavour()
{
  branchName_pdgId_ = Form("%s_%s", branchName_obj_.data(), "partonFlavour");
}

std::vector<std::string>
GenJetReader::setBranchAddresses(TTree * tree)
{
  if(instances_[branchName_obj_] == this)
  {
    BranchAddressInitializer bai(tree, max_nJets_);
    bai.setBranchAddress(nJets_, branchName_num_);
    bai.setBranchAddress(jet_pt_, branchName_pt_);
    bai.setBranchAddress(jet_eta_, branchName_eta_);
    bai.setBranchAddress(jet_phi_, branchName_phi_);
    bai.setBranchAddress(jet_mass_, branchName_mass_);
    bai.setBranchAddress(jet_pdgId_, branchName_pdgId_);
    return bai.getBoundBranchNames_read();
  }
  return {};
}

std::vector<GenJet> GenJetReader::read() const
{
  const GenJetReader * const gInstance = instances_[branchName_obj_];
  assert(gInstance);

  const UInt_t nJets = gInstance->nJets_;
  if(nJets > max_nJets_)
  {
    throw cmsException(this)
      << "Number of jets stored in Ntuple = " << nJets << ", exceeds max_nJets = " << max_nJets_ << " !!\n";
  }

  std::vector<GenJet> jets;
  if(nJets > 0)
  {
    jets.reserve(nJets);
    for(UInt_t idxJet = 0; idxJet < nJets; ++idxJet)
    {
      jets.push_back({
        gInstance->jet_pt_[idxJet],
        gInstance->jet_eta_[idxJet],
        gInstance->jet_phi_[idxJet],
        gInstance->jet_mass_[idxJet],
        gInstance->jet_pdgId_[idxJet],
      });
    }
  }
  return jets;
}
