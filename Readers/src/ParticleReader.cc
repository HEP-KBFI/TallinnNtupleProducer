#include "TallinnNtupleProducer/Readers/interface/ParticleReader.h"

#include "TallinnNtupleProducer/CommonTools/interface/BranchAddressInitializer.h" // BranchAddressInitializer
#include "TallinnNtupleProducer/CommonTools/interface/cmsException.h"             // cmsException()

#include "TString.h"                                                              // Form()
#include "TTree.h"                                                                // TTree

std::map<std::string, int> ParticleReader::numInstances_;
std::map<std::string, ParticleReader *> ParticleReader::instances_;

ParticleReader::ParticleReader(const edm::ParameterSet & cfg)
  : ReaderBase(cfg)
  , max_nParticles_(cfg.exists("max_nParticles") ? cfg.getParameter<unsigned>("max_nParticles"): 36)
  , branchName_obj_(cfg.getParameter<std::string>("branchName"))
  , branchName_num_(Form("n%s", branchName_obj_.data()))
  , particle_pt_(nullptr)
  , particle_eta_(nullptr)
  , particle_phi_(nullptr)
  , particle_mass_(nullptr)
{
  setBranchNames();
}

ParticleReader::~ParticleReader()
{
  --numInstances_[branchName_obj_];
  assert(numInstances_[branchName_obj_] >= 0);
  if(numInstances_[branchName_obj_] == 0)
  {
    ParticleReader * const gInstance = instances_[branchName_obj_];
    assert(gInstance);
    delete[] gInstance->particle_pt_;
    delete[] gInstance->particle_eta_;
    delete[] gInstance->particle_phi_;
    delete[] gInstance->particle_mass_;
    instances_[branchName_obj_] = nullptr;
  }
}

void
ParticleReader::setBranchNames()
{
  if(numInstances_[branchName_obj_] == 0)
  {
    branchName_pt_ = Form("%s_%s", branchName_obj_.data(), "pt");
    branchName_eta_ = Form("%s_%s", branchName_obj_.data(), "eta");
    branchName_phi_ = Form("%s_%s", branchName_obj_.data(), "phi");
    branchName_mass_ = Form("%s_%s", branchName_obj_.data(), "mass");
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
ParticleReader::setBranchAddresses(TTree * tree)
{
  if(instances_[branchName_obj_] == this)
  {
    BranchAddressInitializer bai(tree, max_nParticles_);
    bai.setBranchAddress(nParticles_, branchName_num_);
    bai.setBranchAddress(particle_pt_, branchName_pt_);
    bai.setBranchAddress(particle_eta_, branchName_eta_);
    bai.setBranchAddress(particle_phi_, branchName_phi_);
    bai.setBranchAddress(particle_mass_, branchName_mass_);
    return bai.getBoundBranchNames_read();
  }
  return {};
}

std::vector<Particle>
ParticleReader::read() const
{
  const ParticleReader * const gInstance = instances_[branchName_obj_];
  assert(gInstance);

  const UInt_t nParticles = gInstance->nParticles_;
  if(nParticles > max_nParticles_)
  {
    throw cmsException(this)
      << "Number of Particles stored in Ntuple = " << nParticles << ", exceeds max_nParticles = " << max_nParticles_;
  }

  std::vector<Particle> particles;
  if(nParticles > 0)
  {
    particles.reserve(nParticles);
    for(UInt_t idxJet = 0; idxJet < nParticles; ++idxJet)
    {
      particles.push_back({
        gInstance->particle_pt_[idxJet],
        gInstance->particle_eta_[idxJet],
        gInstance->particle_phi_[idxJet],
        gInstance->particle_mass_[idxJet],
      });
    }
  }
  return particles;
}
