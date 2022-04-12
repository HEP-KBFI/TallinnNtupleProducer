#include "TallinnNtupleProducer/Readers/interface/GenLeptonReader.h"

#include "TallinnNtupleProducer/CommonTools/interface/cmsException.h"         // cmsException()
#include "TallinnNtupleProducer/Readers/interface/BranchAddressInitializer.h" // BranchAddressInitializer

#include "TString.h"                                                          // Form()
#include "TTree.h"                                                            // TTree

std::map<std::string, int> GenLeptonReader::numInstances_;
std::map<std::string, GenLeptonReader *> GenLeptonReader::instances_;

GenLeptonReader::GenLeptonReader(const edm::ParameterSet & cfg)
  : ReaderBase(cfg)
  , max_nLeptons_(36)
  , branchName_num_("")
  , branchName_obj_("")
  , readGenPartFlav_(false)
  , lepton_pt_(nullptr)
  , lepton_eta_(nullptr)
  , lepton_phi_(nullptr)
  , lepton_mass_(nullptr)
  , lepton_pdgId_(nullptr)
  , lepton_status_(nullptr)
  , lepton_statusFlags_(nullptr)
  , lepton_genPartFlav_(nullptr)
{
  if ( cfg.exists("max_nLeptons") )
  {
    max_nLeptons_ = cfg.getParameter<unsigned int>("max_nLeptons");
  }
  branchName_obj_ = cfg.getParameter<std::string>("branchName"); // default = "GenLep"
  branchName_num_ = Form("n%s", branchName_obj_.data());
  setBranchNames();
}

GenLeptonReader::~GenLeptonReader()
{
  --numInstances_[branchName_obj_];
  assert(numInstances_[branchName_obj_] >= 0);
  if(numInstances_[branchName_obj_] == 0)
  {
    GenLeptonReader * const gInstance = instances_[branchName_obj_];
    assert(gInstance);
    delete[] gInstance->lepton_pt_;
    delete[] gInstance->lepton_eta_;
    delete[] gInstance->lepton_phi_;
    delete[] gInstance->lepton_mass_;
    delete[] gInstance->lepton_pdgId_;
    delete[] gInstance->lepton_status_;
    delete[] gInstance->lepton_statusFlags_;
    delete[] gInstance->lepton_genPartFlav_;
    instances_[branchName_obj_] = nullptr;
  }
}

void
GenLeptonReader::readGenPartFlav(bool flag)
{
  readGenPartFlav_ = flag;
}

void
GenLeptonReader::setBranchNames()
{
  if(numInstances_[branchName_obj_] == 0)
  {
    branchName_pt_ = Form("%s_%s", branchName_obj_.data(), "pt");
    branchName_eta_ = Form("%s_%s", branchName_obj_.data(), "eta");
    branchName_phi_ = Form("%s_%s", branchName_obj_.data(), "phi");
    branchName_mass_ = Form("%s_%s", branchName_obj_.data(), "mass");
    branchName_pdgId_ = Form("%s_%s", branchName_obj_.data(), "pdgId");
    branchName_status_ = Form("%s_%s", branchName_obj_.data(), "status");
    branchName_statusFlags_ = Form("%s_%s", branchName_obj_.data(), "statusFlags");
    branchName_genPartFlav_ = Form("%s_%s", branchName_obj_.data(), "genPartFlav");
    instances_[branchName_obj_] = this;
  }
  else
  {
    const GenLeptonReader * const gInstance = instances_[branchName_obj_];
    assert(gInstance);
    if(branchName_num_  != gInstance->branchName_num_)
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
GenLeptonReader::setBranchAddresses(TTree * tree)
{
  if(instances_[branchName_obj_] == this)
  {
    BranchAddressInitializer bai(tree, max_nLeptons_);
    bai.setBranchAddress(nLeptons_, branchName_num_);
    bai.setBranchAddress(lepton_pt_, branchName_pt_);
    bai.setBranchAddress(lepton_eta_, branchName_eta_);
    bai.setBranchAddress(lepton_phi_, branchName_phi_);
    bai.setBranchAddress(lepton_mass_, branchName_mass_);
    bai.setBranchAddress(lepton_pdgId_, branchName_pdgId_);
    bai.setBranchAddress(lepton_status_, branchName_status_);
    bai.setBranchAddress(lepton_statusFlags_, branchName_statusFlags_);
    bai.setBranchAddress(lepton_genPartFlav_, readGenPartFlav_ ? branchName_genPartFlav_ : "");
    return bai.getBoundBranchNames();
  }
  return {};
}

std::vector<GenLepton>
GenLeptonReader::read() const
{
  const GenLeptonReader * const gInstance = instances_[branchName_obj_];
  assert(gInstance);

  const UInt_t nLeptons = gInstance->nLeptons_;
  if(nLeptons > max_nLeptons_)
  {
    throw cmsException(this)
      << "Number of leptons stored in Ntuple = " << nLeptons << ","
         " exceeds max_nLeptons = " << max_nLeptons_ << " !!\n";
  }

  std::vector<GenLepton> leptons;
  if(nLeptons > 0)
  {
    leptons.reserve(nLeptons);
    for(UInt_t idxLepton = 0; idxLepton < nLeptons; ++idxLepton)
    {
      leptons.push_back({
        gInstance->lepton_pt_[idxLepton],
        gInstance->lepton_eta_[idxLepton],
        gInstance->lepton_phi_[idxLepton],
        gInstance->lepton_mass_[idxLepton],
        gInstance->lepton_pdgId_[idxLepton],
        gInstance->lepton_status_[idxLepton],
        gInstance->lepton_statusFlags_[idxLepton],
        gInstance->lepton_genPartFlav_[idxLepton],
      });
    }
  }
  return leptons;
}
