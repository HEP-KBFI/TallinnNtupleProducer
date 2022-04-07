#include "tthAnalysis/HiggsToTauTau/interface/GenLeptonReader.h" // GenLeptonReader

#include "tthAnalysis/HiggsToTauTau/interface/cmsException.h" // cmsException()
#include "tthAnalysis/HiggsToTauTau/interface/BranchAddressInitializer.h" // BranchAddressInitializer, TTree, Form()

std::map<std::string, int> GenLeptonReader::numInstances_;
std::map<std::string, GenLeptonReader *> GenLeptonReader::instances_;

GenLeptonReader::GenLeptonReader(const edm::ParameterSet & cfg)
  : max_nPromptLeptons_(36)
  , branchName_nPromptLeptons_("")
  , branchName_promptLeptons_("")
  , readGenPartFlav_(false)
  , promptLepton_pt_(nullptr)
  , promptLepton_eta_(nullptr)
  , promptLepton_phi_(nullptr)
  , promptLepton_mass_(nullptr)
  , promptLepton_pdgId_(nullptr)
  , promptLepton_status_(nullptr)
  , promptLepton_statusFlags_(nullptr)
  , promptLepton_genPartFlav_(nullptr)
{
  branchName_obj_ = cfg.getParameter<std::string>("branchName"); // default = "GenLep"
  branchName_num_ = Form("n%s", branchName_obj_.data());
  setBranchNames();
}

GenLeptonReader::~GenLeptonReader()
{
  --numInstances_[branchName_promptLeptons_];
  assert(numInstances_[branchName_promptLeptons_] >= 0);
  if(numInstances_[branchName_promptLeptons_] == 0)
  {
    GenLeptonReader * const gInstance = instances_[branchName_promptLeptons_];
    assert(gInstance);
    delete[] gInstance->promptLepton_pt_;
    delete[] gInstance->promptLepton_eta_;
    delete[] gInstance->promptLepton_phi_;
    delete[] gInstance->promptLepton_mass_;
    delete[] gInstance->promptLepton_pdgId_;
    delete[] gInstance->promptLepton_status_;
    delete[] gInstance->promptLepton_statusFlags_;
    delete[] gInstance->promptLepton_genPartFlav_;
    instances_[branchName_promptLeptons_] = nullptr;
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
  if(numInstances_[branchName_promptLeptons_] == 0)
  {
    branchName_promptLepton_pt_ = Form("%s_%s", branchName_promptLeptons_.data(), "pt");
    branchName_promptLepton_eta_ = Form("%s_%s", branchName_promptLeptons_.data(), "eta");
    branchName_promptLepton_phi_ = Form("%s_%s", branchName_promptLeptons_.data(), "phi");
    branchName_promptLepton_mass_ = Form("%s_%s", branchName_promptLeptons_.data(), "mass");
    branchName_promptLepton_pdgId_ = Form("%s_%s", branchName_promptLeptons_.data(), "pdgId");
    branchName_promptLepton_status_ = Form("%s_%s", branchName_promptLeptons_.data(), "status");
    branchName_promptLepton_statusFlags_ = Form("%s_%s", branchName_promptLeptons_.data(), "statusFlags");
    branchName_promptLepton_genPartFlav_ = Form("%s_%s", branchName_promptLeptons_.data(), "genPartFlav");
    instances_[branchName_promptLeptons_] = this;
  }
  else
  {
    const GenLeptonReader * const gInstance = instances_[branchName_promptLeptons_];
    assert(gInstance);
    if(branchName_nPromptLeptons_  != gInstance->branchName_nPromptLeptons_)
    {
      throw cmsException(this)
        << "Association between configuration parameters 'branchName_nPromptLeptons', 'branchName_nLeptonsFromTau', "
           "'branchName_leptonsFromTau' and 'branchName_promptLeptons' must be unique:"
           " present association 'branchName_nPromptLeptons' = " << branchName_nPromptLeptons_ << ","
           " with 'branchName_promptLeptons' = " << branchName_promptLeptons_
        << " does not match previous association 'branchName_nPromptLeptons' = " << gInstance->branchName_nPromptLeptons_ << ","
           " with 'branchName_promptLeptons' = " << gInstance->branchName_promptLeptons_ << " !!\n";
    }
  }
  ++numInstances_[branchName_promptLeptons_];
}

std::vector<std::string>
GenLeptonReader::setBranchAddresses(TTree * tree)
{
  if(instances_[branchName_promptLeptons_] == this)
  {
    std::cout << "setting branch addresses for PromptLeptons: " << branchName_promptLeptons_ << '\n';
    BranchAddressInitializer bai(tree, max_nPromptLeptons_);
    bai.setBranchAddress(nPromptLeptons_, branchName_nPromptLeptons_);
    bai.setBranchAddress(promptLepton_pt_, branchName_promptLepton_pt_);
    bai.setBranchAddress(promptLepton_eta_, branchName_promptLepton_eta_);
    bai.setBranchAddress(promptLepton_phi_, branchName_promptLepton_phi_);
    bai.setBranchAddress(promptLepton_mass_, branchName_promptLepton_mass_);
    bai.setBranchAddress(promptLepton_pdgId_, branchName_promptLepton_pdgId_);
    bai.setBranchAddress(promptLepton_status_, branchName_promptLepton_status_);
    bai.setBranchAddress(promptLepton_statusFlags_, branchName_promptLepton_statusFlags_);
    bai.setBranchAddress(promptLepton_genPartFlav_, readGenPartFlav_ ? branchName_promptLepton_genPartFlav_ : "");
    return bai.getBoundBranchNames();
  }
  return {};
}

std::vector<GenLepton>
GenLeptonReader::read() const
{
  const GenLeptonReader * const gInstance = instances_[branchName_promptLeptons_];
  assert(gInstance);

  const UInt_t nPromptLeptons = gInstance->nPromptLeptons_;
  if(nPromptLeptons > max_nPromptLeptons_)
  {
    throw cmsException(this)
      << "Number of prompt leptons stored in Ntuple = " << nPromptLeptons << ","
         " exceeds max_nPromptLeptons = " << max_nPromptLeptons_ << " !!\n";
  }

  std::vector<GenLepton> leptons;
  if(nPromptLeptons > 0)
  {
    leptons.reserve(nPromptLeptons);
    for(UInt_t idxLepton = 0; idxLepton < nPromptLeptons; ++idxLepton)
    {
      leptons.push_back({
        gInstance->promptLepton_pt_[idxLepton],
        gInstance->promptLepton_eta_[idxLepton],
        gInstance->promptLepton_phi_[idxLepton],
        gInstance->promptLepton_mass_[idxLepton],
        gInstance->promptLepton_pdgId_[idxLepton],
        gInstance->promptLepton_status_[idxLepton],
        gInstance->promptLepton_statusFlags_[idxLepton],
        gInstance->promptLepton_genPartFlav_[idxLepton],
      });
    }
  }
  return leptons;
}
