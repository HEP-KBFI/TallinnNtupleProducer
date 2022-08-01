#ifndef TallinnNtupleProducer_Readers_ParticleReader_h
#define TallinnNtupleProducer_Readers_ParticleReader_h

#include "TallinnNtupleProducer/Objects/interface/Particle.h"   // Particle
#include "TallinnNtupleProducer/Readers/interface/ReaderBase.h" // ReaderBase

#include <map>                                                  // std::map

// forward declarations
class TTree;

class ParticleReader
  : public ReaderBase
{
 public:
  explicit ParticleReader(const edm::ParameterSet & cfg);
  ~ParticleReader();

  /**
   * @brief Call tree->SetBranchAddress for all Particle branches
   */
  std::vector<std::string>
  setBranchAddresses(TTree * tree) override;

  /**
   * @brief Read branches from tree and use information to fill collection of Particle objects
   * @return Collection of Particle objects
   */
  std::vector<Particle>
  read() const;

 protected:
 /**
   * @brief Initialize names of branches to be read from tree
   */
  void
  setBranchNames();

  unsigned int max_nParticles_;
  std::string branchName_obj_;
  std::string branchName_num_;

  std::string branchName_pt_;
  std::string branchName_eta_;
  std::string branchName_phi_;
  std::string branchName_mass_;

  UInt_t nParticles_;
  Float_t * particle_pt_;
  Float_t * particle_eta_;
  Float_t * particle_phi_;
  Float_t * particle_mass_;

  // CV: make sure that only one RecoJetReader instance exists for a given branchName,
  //     as ROOT cannot handle multiple TTree::SetBranchAddress calls for the same branch.
  static std::map<std::string, int> numInstances_;
  static std::map<std::string, ParticleReader *> instances_;
};

#endif // TallinnNtupleProducer_Readers_ParticleReader_h

