#ifndef TallinnNtupleProducer_Objects_LHEParticle_h
#define TallinnNtupleProducer_Objects_LHEParticle_h

#include "TallinnNtupleProducer/Objects/interface/Particle.h" // Particle

class LHEParticle : public Particle
{
 public:
  LHEParticle();
  LHEParticle(const LHEParticle & lheParticle);
  LHEParticle(Double_t pt,
              Double_t eta,
              Double_t phi,
              Double_t mass,
              Int_t pdgId,
              Int_t status);
  LHEParticle(const math::PtEtaPhiMLorentzVector & p4,
              Int_t pdgId,
              Int_t status);
  virtual ~LHEParticle() {}

  /**
   * @brief Funtions to access data-members
   * @return Values of data-members
   */
  Int_t 
  pdgId() const;

  Int_t
  status() const;

 protected:
  Int_t pdgId_;  ///< PDG id of the particle (signed)
  Int_t status_; ///< particle status (1 = stable)
};

typedef std::vector<LHEParticle> LHEParticleCollection;

std::ostream &
operator<<(std::ostream & stream,
           const LHEParticle & particle);

#endif // TallinnNtupleProducer_Objects_LHEParticle_h

