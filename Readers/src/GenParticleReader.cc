#include "TallinnNtupleProducer/Readers/interface/GenParticleReader.h" // GenParticleReader

#include "TallinnNtupleProducer/CommonTools/interface/BranchAddressInitializer.h" // BranchAddressInitializer
#include "TallinnNtupleProducer/CommonTools/interface/cmsException.h"             // cmsException()

#include "TTree.h"                                                                // TTree
#include "TString.h"                                                              // Form()

std::map<std::string, int> GenParticleReader::numInstances_;
std::map<std::string, GenParticleReader *> GenParticleReader::instances_;

GenParticleReader::GenParticleReader(const edm::ParameterSet & cfg)
  : ReaderBase(cfg)
  , max_nParticles_(512)
  , particle_pt_(nullptr)
  , particle_eta_(nullptr)
  , particle_phi_(nullptr)
  , particle_mass_(nullptr)
  , particle_pdgId_(nullptr)
  , particle_status_(nullptr)
  , particle_statusFlags_(nullptr)
  , particle_genPartIdxMother_(nullptr)
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
    delete[] gInstance->particle_status_;
    delete[] gInstance->particle_statusFlags_;
    delete[] gInstance->particle_genPartIdxMother_;
    instances_[branchName_obj_] = nullptr;
  }
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
    branchName_status_ = Form("%s_%s", branchName_obj_.data(), "status");
    branchName_statusFlags_ = Form("%s_%s", branchName_obj_.data(), "statusFlags");
    branchName_genPartIdxMother_ = Form("%s_%s", branchName_obj_.data(), "genPartIdxMother");
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
    bai.setBranchAddress(particle_status_, branchName_status_);
    bai.setBranchAddress(particle_statusFlags_, branchName_statusFlags_);
    bai.setBranchAddress(particle_genPartIdxMother_, branchName_genPartIdxMother_);
    return bai.getBoundBranchNames_read();
  }
  return {};
}

GenParticleCollection
GenParticleReader::read() const
{
  const GenParticleReader * const gInstance = instances_[branchName_obj_];
  assert(gInstance);

  const UInt_t nParticles = gInstance->nParticles_;
  if ( nParticles > max_nParticles_ ) {
    throw cmsException(this, __func__, __LINE__)
      << "Number of particles stored in Ntuple = " << nParticles << "," 
         " exceeds max_nParticles = " << max_nParticles_;
  }

  GenParticleCollection particles;
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
        gInstance->particle_status_[idxParticle],
        gInstance->particle_statusFlags_[idxParticle],
        gInstance->particle_genPartIdxMother_[idxParticle],
      });
    }
  }

  // loop over the gen particles in order to establish complete mapping between the mother-daughter particles
  const int nof_particles = static_cast<int>(particles.size());
  for(std::size_t genPartIdx = 0; genPartIdx < particles.size(); ++genPartIdx)
  {
    const int genPartIdxMother = particles.at(genPartIdx).momIdx();
    assert(genPartIdxMother < nof_particles);
    if(genPartIdxMother >= 0)
    {
      particles.at(genPartIdxMother).addDaughter(genPartIdx);
    }
  }

  return particles;
}
