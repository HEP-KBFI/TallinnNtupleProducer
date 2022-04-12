#include "TallinnNtupleProducer/Readers/interface/GenPhotonReader.h"

#include "TallinnNtupleProducer/CommonTools/interface/cmsException.h"         // cmsException()
#include "TallinnNtupleProducer/Readers/interface/BranchAddressInitializer.h" // BranchAddressInitializer

#include "TString.h"                                                          // Form()
#include "TTree.h"                                                            // TTree

std::map<std::string, int> GenPhotonReader::numInstances_;
std::map<std::string, GenPhotonReader *> GenPhotonReader::instances_;

GenPhotonReader::GenPhotonReader(const edm::ParameterSet & cfg)
  : ReaderBase(cfg)
  , max_nPhotons_(36)
  , branchName_num_("")
  , branchName_obj_("")
  , readGenPartFlav_(false)
  , photon_pt_(nullptr)
  , photon_eta_(nullptr)
  , photon_phi_(nullptr)
  , photon_mass_(nullptr)
  , photon_pdgId_(nullptr)
  , photon_status_(nullptr)
  , photon_statusFlags_(nullptr)
  , photon_genPartFlav_(nullptr)
{
  if ( cfg.exists("max_nPhotons") )
  {
    max_nPhotons_ = cfg.getParameter<unsigned int>("max_nPhotons");
  }
  branchName_obj_ = cfg.getParameter<std::string>("branchName"); // default = "GenPhoton"
  branchName_num_ = Form("n%s", branchName_obj_.data());
  setBranchNames();
}

GenPhotonReader::~GenPhotonReader()
{
  --numInstances_[branchName_obj_];
  assert(numInstances_[branchName_obj_] >= 0);
  if(numInstances_[branchName_obj_] == 0)
  {
    GenPhotonReader * const gInstance = instances_[branchName_obj_];
    assert(gInstance);
    delete[] gInstance->photon_pt_;
    delete[] gInstance->photon_eta_;
    delete[] gInstance->photon_phi_;
    delete[] gInstance->photon_mass_;
    delete[] gInstance->photon_pdgId_;
    delete[] gInstance->photon_status_;
    delete[] gInstance->photon_statusFlags_;
    delete[] gInstance->photon_genPartFlav_;
    instances_[branchName_obj_] = nullptr;
  }
}

void
GenPhotonReader::readGenPartFlav(bool flag)
{
  readGenPartFlav_ = flag;
}

void
GenPhotonReader::setBranchNames()
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
GenPhotonReader::setBranchAddresses(TTree * tree)
{
  if(instances_[branchName_obj_] == this)
  {
    BranchAddressInitializer bai(tree, max_nPhotons_);
    bai.setBranchAddress(nPhotons_, branchName_num_);
    bai.setBranchAddress(photon_pt_, branchName_pt_);
    bai.setBranchAddress(photon_eta_, branchName_eta_);
    bai.setBranchAddress(photon_phi_, branchName_phi_);
    bai.setBranchAddress(photon_mass_, branchName_mass_);
    bai.setBranchAddress(photon_pdgId_, branchName_pdgId_);
    bai.setBranchAddress(photon_status_, branchName_status_);
    bai.setBranchAddress(photon_statusFlags_, branchName_statusFlags_);
    bai.setBranchAddress(photon_genPartFlav_, readGenPartFlav_ ? branchName_genPartFlav_ : "");
    return bai.getBoundBranchNames();
  }
  return {};
}

namespace
{
  template <typename T>
  std::vector<T>
  filterByStatus(const std::vector<T> & genParticles, int status)
  {
    if(status > 0)
    {
      std::vector<T> filteredGenParticles;
      std::copy_if(
        genParticles.cbegin(), genParticles.cend(), std::back_inserter(filteredGenParticles),
          [status](const GenParticle & genParticle) -> bool
          {
            return genParticle.status() == status;
          }
      );
      return filteredGenParticles;
    }
    return genParticles;
  }
}

std::vector<GenPhoton>
GenPhotonReader::read(bool readAll) const
{
  const GenPhotonReader * const gInstance = instances_[branchName_obj_];
  assert(gInstance);

  const UInt_t nPhotons = gInstance->nPhotons_;
  if(nPhotons > max_nPhotons_)
  {
    throw cmsException(this)
      << "Number of photons stored in Ntuple = " << nPhotons << ", "
         "exceeds max_nPhotons = " << max_nPhotons_ << " !!\n"
    ;
  }

  std::vector<GenPhoton> photons;
  if(nPhotons > 0)
  {
    photons.reserve(nPhotons);
    for(UInt_t idxPhoton = 0; idxPhoton < nPhotons; ++idxPhoton)
    {
      photons.push_back({
        gInstance->photon_pt_[idxPhoton],
        gInstance->photon_eta_[idxPhoton],
        gInstance->photon_phi_[idxPhoton],
        gInstance->photon_mass_[idxPhoton],
        gInstance->photon_pdgId_[idxPhoton],
        gInstance->photon_status_[idxPhoton],
        gInstance->photon_statusFlags_[idxPhoton],
        gInstance->photon_genPartFlav_[idxPhoton],
      });
    }
  }
  return readAll ? photons : filterByStatus(photons, 1);
}
