#ifndef TallinnNtupleProducer_Objects_ChargedParticle_h
#define TallinnNtupleProducer_Objects_ChargedParticle_h

#include "TallinnNtupleProducer/Objects/interface/Particle.h" // Particle

class ChargedParticle
  : public Particle
{
public:
  ChargedParticle();
  ChargedParticle(Double_t pt,
                  Double_t eta,
                  Double_t phi,
                  Double_t mass,
                  Int_t pdgId,
                  Int_t charge);
  ChargedParticle(const math::PtEtaPhiMLorentzVector & p4,
                  Int_t pdgId,
                  Int_t charge);
  virtual ~ChargedParticle() {}

  /**
   * @brief Funtions to access data-members
   * @return Values of data-members
   */
  Int_t pdgId() const;
  Int_t absPdgId() const;
  Int_t charge() const;

protected:
  Int_t pdgId_;  ///< PDG id of the particle (signed)
  Int_t charge_; ///< charge of particle
};

std::ostream &
operator<<(std::ostream & stream,
           const ChargedParticle & particle);

#endif // TallinnNtupleProducer_Objects_ChargedParticle_h
