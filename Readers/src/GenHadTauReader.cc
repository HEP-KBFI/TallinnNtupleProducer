#include "TallinnNtupleProducer/Readers/interface/GenHadTauReader.h"

#include "TallinnNtupleProducer/CommonTools/interface/BranchAddressInitializer.h" // BranchAddressInitializer
#include "TallinnNtupleProducer/CommonTools/interface/cmsException.h"             // cmsException()

#include "TString.h"                                                              // Form()
#include "TTree.h"                                                                // TTree

std::map<std::string, int> GenHadTauReader::numInstances_;
std::map<std::string, GenHadTauReader *> GenHadTauReader::instances_;

GenHadTauReader::GenHadTauReader(const edm::ParameterSet & cfg)
  : ReaderBase(cfg)
  , max_nHadTaus_(36)
  , branchName_num_("")
  , branchName_obj_("")
  , hadTau_pt_(nullptr)
  , hadTau_eta_(nullptr)
  , hadTau_phi_(nullptr)
  , hadTau_mass_(nullptr)
  , hadTau_charge_(nullptr)
  , hadTau_status_(nullptr)
{
  if ( cfg.exists("max_nHadTaus") )
  {
    max_nHadTaus_ = cfg.getParameter<unsigned int>("max_nHadTaus");
  }
  branchName_obj_ = cfg.getParameter<std::string>("branchName"); // default = "GenVisTau"
  branchName_num_ = Form("n%s", branchName_obj_.data());
  setBranchNames();
}

GenHadTauReader::~GenHadTauReader()
{
  --numInstances_[branchName_obj_];
  assert(numInstances_[branchName_obj_] >= 0);
  if(numInstances_[branchName_obj_] == 0)
  {
    GenHadTauReader * const gInstance = instances_[branchName_obj_];
    assert(gInstance);
    delete[] gInstance->hadTau_pt_;
    delete[] gInstance->hadTau_eta_;
    delete[] gInstance->hadTau_phi_;
    delete[] gInstance->hadTau_mass_;
    delete[] gInstance->hadTau_charge_;
    delete[] gInstance->hadTau_status_;
    instances_[branchName_obj_] = nullptr;
  }
}

void
GenHadTauReader::setBranchNames()
{
  if(numInstances_[branchName_obj_] == 0)
  {
    branchName_pt_ = Form("%s_%s", branchName_obj_.data(), "pt");
    branchName_eta_ = Form("%s_%s", branchName_obj_.data(), "eta");
    branchName_phi_ = Form("%s_%s", branchName_obj_.data(), "phi");
    branchName_mass_ = Form("%s_%s", branchName_obj_.data(), "mass");
    branchName_charge_ = Form("%s_%s", branchName_obj_.data(), "charge");
    branchName_status_ = Form("%s_%s", branchName_obj_.data(), "status");
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
GenHadTauReader::setBranchAddresses(TTree * tree)
{
  if(instances_[branchName_obj_] == this)
  {
    BranchAddressInitializer bai(tree, max_nHadTaus_);
    bai.setBranchAddress(nHadTaus_, branchName_num_);
    bai.setBranchAddress(hadTau_pt_, branchName_pt_);
    bai.setBranchAddress(hadTau_eta_, branchName_eta_);
    bai.setBranchAddress(hadTau_phi_, branchName_phi_);
    bai.setBranchAddress(hadTau_mass_, branchName_mass_);
    bai.setBranchAddress(hadTau_charge_, branchName_charge_);
    bai.setBranchAddress(hadTau_status_, branchName_status_);
    return bai.getBoundBranchNames_read();
  }
  return {};
}

std::vector<GenHadTau>
GenHadTauReader::read() const
{
  const GenHadTauReader * const gInstance = instances_[branchName_obj_];
  assert(gInstance);

  const UInt_t nHadTaus = gInstance->nHadTaus_;
  if(nHadTaus > max_nHadTaus_)
  {
    throw cmsException(this)
      << "Number of hadronic taus stored in Ntuple = " << nHadTaus << ", "
         "exceeds max_nHadTaus = " << max_nHadTaus_ << " !!\n";
  }

  std::vector<GenHadTau> hadTaus;
  if(nHadTaus > 0)
  {
    hadTaus.reserve(nHadTaus);
    for(UInt_t idxHadTau = 0; idxHadTau < nHadTaus; ++idxHadTau)
    {
      hadTaus.push_back({
        gInstance->hadTau_pt_[idxHadTau],
        gInstance->hadTau_eta_[idxHadTau],
        gInstance->hadTau_phi_[idxHadTau],
        gInstance->hadTau_mass_[idxHadTau],
        gInstance->hadTau_charge_[idxHadTau],
        gInstance->hadTau_status_[idxHadTau],
      });
    }
  }
  return hadTaus;
}
