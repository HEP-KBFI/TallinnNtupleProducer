#include "TallinnNtupleProducer/Readers/interface/GenParticleReader.h" // GenParticleReader

#include "TallinnNtupleProducer/CommonTools/interface/BranchAddressInitializer.h" // BranchAddressInitializer
#include "TallinnNtupleProducer/CommonTools/interface/cmsException.h"             // cmsException()

#include "TTree.h"                                                                // TTree
#include "TString.h"                                                              // Form()

std::map<std::string, int> GenParticleReader::numInstances_;
std::map<std::string, GenParticleReader *> GenParticleReader::instances_;

GenParticleReader::GenParticleReader(const edm::ParameterSet & cfg)
  : ReaderBase(cfg)
  , max_nParticles_(32)
  , readGenPartFlav_(false)
  , particle_pt_(nullptr)
  , particle_eta_(nullptr)
  , particle_phi_(nullptr)
  , particle_mass_(nullptr)
  , particle_pdgId_(nullptr)
  , particle_charge_(nullptr)
  , particle_status_(nullptr)
  , particle_statusFlags_(nullptr)
  , particle_genPartFlav_(nullptr)
{
  branchName_obj_ = cfg.getParameter<std::string>("branchName");
  branchName_num_ = Form("n%s", branchName_obj_.data());
  setBranchNames();
}

GenParticleReader::~GenParticleReader()
{
  --numInstances_[branchName_obj_];
  assert(numInstances_[branchName_obj_] >= 0);

  if(numInstances_[branchName_obj_] == 0)
  {
    GenParticleReader * const gInstance = instances_[branchName_obj_];
    assert(gInstance);
    delete[] gInstance->particle_pt_;
    delete[] gInstance->particle_eta_;
    delete[] gInstance->particle_phi_;
    delete[] gInstance->particle_mass_;
    delete[] gInstance->particle_pdgId_;
    delete[] gInstance->particle_charge_;
    delete[] gInstance->particle_status_;
    delete[] gInstance->particle_statusFlags_;
    delete[] gInstance->particle_genPartFlav_;
    instances_[branchName_obj_] = nullptr;
  }
}

void
GenParticleReader::readGenPartFlav(bool flag)
{
  readGenPartFlav_ = flag;
}

void
GenParticleReader::setBranchNames()
{
  if(numInstances_[branchName_obj_] == 0)
  {
    branchName_pt_ = Form("%s_%s", branchName_obj_.data(), "pt");
    branchName_eta_ = Form("%s_%s", branchName_obj_.data(), "eta");
    branchName_phi_ = Form("%s_%s", branchName_obj_.data(), "phi");
    branchName_mass_ = Form("%s_%s", branchName_obj_.data(), "mass");
    branchName_pdgId_ = Form("%s_%s", branchName_obj_.data(), "pdgId");
    branchName_charge_ = Form("%s_%s", branchName_obj_.data(), "charge");
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
GenParticleReader::setBranchAddresses(TTree * tree)
{
  if(instances_[branchName_obj_] == this)
  {
    BranchAddressInitializer bai(tree, max_nParticles_);
    bai.setBranchAddress(nParticles_, branchName_num_);
    bai.setBranchAddress(particle_pt_, branchName_pt_);
    bai.setBranchAddress(particle_eta_, branchName_eta_);
    bai.setBranchAddress(particle_phi_, branchName_phi_);
    bai.setBranchAddress(particle_mass_, branchName_mass_);
    bai.setBranchAddress(particle_pdgId_, branchName_pdgId_);
    bai.setBranchAddress(particle_charge_, branchName_charge_);
    bai.setBranchAddress(particle_status_, branchName_status_);
    bai.setBranchAddress(particle_genPartFlav_, readGenPartFlav_ ? branchName_genPartFlav_ : "");
    bai.setBranchAddress(particle_statusFlags_, branchName_statusFlags_);
    return bai.getBoundBranchNames_read();
  }
  return {};
}

std::vector<GenParticle>
GenParticleReader::read() const
{
  const GenParticleReader * const gInstance = instances_[branchName_obj_];
  assert(gInstance);

  const UInt_t nParticles = gInstance->nParticles_;
  if ( nParticles > max_nParticles_ ) {
    throw cmsException(this)
      << "Number of particles stored in Ntuple = " << nParticles << "," 
         " exceeds max_nParticles = " << max_nParticles_ << " !!\n";
  }

  std::vector<GenParticle> particles;
  if(nParticles > 0)
  {
    particles.reserve(nParticles);
    for(UInt_t idxParticle = 0; idxParticle < nParticles; ++idxParticle)
    {
      particles.push_back({
        gInstance->particle_pt_[idxParticle],
        gInstance->particle_eta_[idxParticle],
        gInstance->particle_phi_[idxParticle],
        gInstance->particle_mass_[idxParticle],
        gInstance->particle_pdgId_[idxParticle],
        gInstance->particle_charge_[idxParticle],
        gInstance->particle_status_[idxParticle],
        gInstance->particle_statusFlags_[idxParticle],
        gInstance->particle_genPartFlav_[idxParticle],
      });
    }
  } 
  return particles;
}
