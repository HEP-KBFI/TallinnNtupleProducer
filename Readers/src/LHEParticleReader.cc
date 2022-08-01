#include "TallinnNtupleProducer/Readers/interface/LHEParticleReader.h"

#include "TallinnNtupleProducer/CommonTools/interface/BranchAddressInitializer.h" // BranchAddressInitializer
#include "TallinnNtupleProducer/CommonTools/interface/cmsException.h"             // cmsException

#include <TString.h>                                                              // Form
#include <TTree.h>                                                                // TTree

#include <assert.h>                                                               // assert()

std::map<std::string, int> LHEParticleReader::numInstances_;
std::map<std::string, LHEParticleReader *> LHEParticleReader::instances_;

LHEParticleReader::LHEParticleReader(const edm::ParameterSet & cfg)
  : ReaderBase(cfg)
  , max_nParticles_(32)
  , branchName_num_("")
  , branchName_obj_("")
  , particle_pt_(nullptr)
  , particle_eta_(nullptr)
  , particle_phi_(nullptr)
  , particle_mass_(nullptr)
  , particle_pdgId_(nullptr)
  , particle_status_(nullptr)
{
  branchName_obj_ = cfg.getParameter<std::string>("branchName_LHE_particle"); // default = "LHEPart"
  branchName_num_ = Form("n%s", branchName_obj_.data());
  setBranchNames();
}

LHEParticleReader::~LHEParticleReader()
{
  --numInstances_[branchName_obj_];
  assert(numInstances_[branchName_obj_] >= 0);

  if(numInstances_[branchName_obj_] == 0)
  {
    LHEParticleReader * const gInstance = instances_[branchName_obj_];
    assert(gInstance);
    delete[] gInstance->particle_pt_;
    delete[] gInstance->particle_eta_;
    delete[] gInstance->particle_phi_;
    delete[] gInstance->particle_mass_;
    delete[] gInstance->particle_pdgId_;
    delete[] gInstance->particle_status_;
    instances_[branchName_obj_] = nullptr;
  }
}

void
LHEParticleReader::setBranchNames()
{
  if(numInstances_[branchName_obj_] == 0)
  {
    branchName_particle_pt_ = Form("%s_%s", branchName_obj_.data(), "pt");
    branchName_particle_eta_ = Form("%s_%s", branchName_obj_.data(), "eta");
    branchName_particle_phi_ = Form("%s_%s", branchName_obj_.data(), "phi");
    branchName_particle_mass_ = Form("%s_%s", branchName_obj_.data(), "mass");
    branchName_particle_pdgId_ = Form("%s_%s", branchName_obj_.data(), "pdgId");
    branchName_particle_status_ = Form("%s_%s", branchName_obj_.data(), "status");
    instances_[branchName_obj_] = this;
  }
  else
  {
    const LHEParticleReader * const gInstance = instances_[branchName_obj_];
    assert(gInstance);
    if(branchName_num_ != gInstance->branchName_num_)
    {
      throw cmsException(this)
        << "Association between configuration parameters 'branchName_num' and 'branchName_obj' must be unique:"
        << " present association 'branchName_num' = " << branchName_num_ << ","
        << " with 'branchName_obj' = " << branchName_obj_
        << " does not match previous association 'branchName_num' = " << gInstance->branchName_num_ << ","
        << " with 'branchName_obj' = " << gInstance->branchName_obj_ << " !!\n";
    }
  }
  ++numInstances_[branchName_obj_];
}

std::vector<std::string>
LHEParticleReader::setBranchAddresses(TTree * tree)
{
  if(instances_[branchName_obj_] == this)
  {
    BranchAddressInitializer bai(tree, max_nParticles_);
    bai.setBranchAddress(nParticles_, branchName_num_);
    bai.setBranchAddress(particle_pt_, branchName_particle_pt_);
    bai.setBranchAddress(particle_eta_, branchName_particle_eta_);
    bai.setBranchAddress(particle_phi_, branchName_particle_phi_);
    bai.setBranchAddress(particle_mass_, branchName_particle_mass_);
    bai.setBranchAddress(particle_pdgId_, branchName_particle_pdgId_);
    bai.setBranchAddress(particle_status_, branchName_particle_status_);
    return bai.getBoundBranchNames_read();
  }
  return {};
}

std::vector<LHEParticle>
LHEParticleReader::read() const
{
  const LHEParticleReader * const gInstance = instances_[branchName_obj_];
  assert(gInstance);

  const UInt_t nParticles = gInstance->nParticles_;
  if ( nParticles > max_nParticles_ ) {
    throw cmsException(this)
      << "Number of particles stored in Ntuple = " << nParticles << "," 
         " exceeds max_nParticles = " << max_nParticles_ << " !!\n";
  }

  std::vector<LHEParticle> particles;
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
        gInstance->particle_status_[idxParticle]
      });
    }
  } 
  return particles;
}
