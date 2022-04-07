#ifndef TallinnNtupleProducer_Readers_GenParticleReader_h
#define TallinnNtupleProducer_Readers_GenParticleReader_h

#include "TallinnNtupleProducer/Objects/interface/GenParticle.h" // GenParticle
#include "TallinnNtupleProducer/Readers/interface/ReaderBase.h"  // ReaderBase

#include <map>                                                   // std::map

// forward declarations
class TTree;

class GenParticleReader : public ReaderBase
{
 public:
  GenParticleReader(const edm::ParameterSet & cfg);
  ~GenParticleReader();

  /**
   * @brief Call tree->SetBranchAddress for all GenParticle branches
   */
  std::vector<std::string>
  setBranchAddresses(TTree * tree) override;

  /**
   * @brief Read branches from tree and use information to fill collection of GenParticle objects
   * @return Collection of GenParticle objects
   */
  std::vector<GenParticle>
  read() const;

  /**
   * @brief enable/disable read genPartFlav branch
   * @param flag If true, reads genPartFlav branch; if false, does not read genPartFlav branch
   */
  void
  readGenPartFlav(bool flag);

 protected:
 /**
   * @brief Initialize names of branches to be read from tree
   */
  void
  setBranchNames();

  const unsigned int max_nParticles_;
  std::string branchName_nParticles_;
  std::string branchName_particles_;

  bool readGenPartFlav_;

  std::string branchName_particle_pt_;
  std::string branchName_particle_eta_;
  std::string branchName_particle_phi_;
  std::string branchName_particle_mass_;
  std::string branchName_particle_pdgId_;
  std::string branchName_particle_charge_;
  std::string branchName_particle_status_;
  std::string branchName_particle_statusFlags_;
  std::string branchName_particle_genPartFlav_;

  UInt_t nParticles_;
  Float_t * particle_pt_;
  Float_t * particle_eta_;
  Float_t * particle_phi_;
  Float_t * particle_mass_;
  Int_t * particle_pdgId_;
  Int_t * particle_charge_;
  Int_t * particle_status_;
  Int_t * particle_statusFlags_;
  UChar_t * particle_genPartFlav_;

  // CV: make sure that only one GenParticleReader instance exists for a given branchName,
  //     as ROOT cannot handle multiple TTree::SetBranchAddress calls for the same branch.
  static std::map<std::string, int> numInstances_;
  static std::map<std::string, GenParticleReader*> instances_;
};

#endif // TallinnNtupleProducer_Readers_GenParticleReader_h

