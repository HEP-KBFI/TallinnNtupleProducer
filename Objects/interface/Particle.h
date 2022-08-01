#ifndef TallinnNtupleProducer_Objects_Particle_h
#define TallinnNtupleProducer_Objects_Particle_h

#include "DataFormats/Math/interface/LorentzVector.h" // math::PtEtaPhiMLorentzVector
#include "DataFormats/Math/interface/Vector3D.h"      // math::XYZVector

#include <Rtypes.h>                                   // Int_t, Long64_t, Double_t

class Particle
{
 public:
  typedef math::PtEtaPhiMLorentzVector LorentzVector;
  typedef math::XYZVector Point;

  Particle();
  Particle(Double_t pt,
           Double_t eta,
           Double_t phi,
           Double_t mass);
  Particle(const Particle::LorentzVector & p4);

  virtual ~Particle() {}

  /**
   * @brief Funtions to access data-members
   * @return Values of data-members
   * 
   * NOTE: get_pt and get_p4 functions needs to be virtual, so that they can be overwritten 
   *       to implement cone_pT logic for fakeable && !tight leptons in RecoLepton class
   */
  Double_t pt() const;
  Double_t eta() const;
  Double_t phi() const;
  Double_t mass() const;
  Double_t absEta() const;

  bool isValid() const;

  const Particle::LorentzVector & p4() const;

  double deltaR(const Particle & particle) const;
  double deltaR(const Particle * const particle) const;

  virtual void
  set_p4(const Particle::LorentzVector & p4);

  virtual void
  set_ptEtaPhiMass(Double_t pt,
                   Double_t eta,
                   Double_t phi,
                   Double_t mass);

 protected:
  Double_t pt_;   ///< pT of the particle
  Double_t eta_;  ///< eta of the particle
  Double_t phi_;  ///< phi of the particle
  Double_t mass_; ///< mass of the particle

  Double_t absEta_; ///< |eta| of the particle

  Particle::LorentzVector p4_; ///< 4-momentum constructed from the pT, eta, phi and mass

  bool isValid_; ///< true if the particle is physical (meaning that its pT > 0)
};

typedef std::vector<Particle> ParticleCollection;
typedef std::vector<const Particle *> ParticlePtrCollection;

std::ostream &
operator<<(std::ostream & stream,
           const Particle & particle);

#endif // TallinnNtupleProducer_Objects_Particle_h
