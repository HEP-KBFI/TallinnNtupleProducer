#ifndef TallinnNtupleProducer_Objects_GenMEt_h
#define TallinnNtupleProducer_Objects_GenMEt_h

#include "TallinnNtupleProducer/Objects/interface/GenParticle.h" // Particle::LorentzVector

#include <TMatrixD.h>                                            // TMatrixD

#include <map>                                                   // std::map

class GenMEt
{
public:
  GenMEt();
  GenMEt(Float_t pt,
         Float_t phi);

  GenMEt &
  operator=(const GenMEt & other);

  /**
   * @brief Funtions to access data-members
   * @return Values of data-members
   */
  Double_t pt() const;
  Double_t phi() const;
  
  const Particle::LorentzVector & p4() const;

protected:
  void
  set(Double_t pt,
      Double_t phi);

  Float_t pt_;  ///< pT of missing transverse momentum vector
  Float_t phi_; ///< phi of missing transverse momentum vector

  ///< (default) 4-momentum constructed from pT and phi, assuming eta and mass to be equal to zero
  math::PtEtaPhiMLorentzVector p4_;
};

std::ostream& 
operator<<(std::ostream & stream,
           const GenMEt & met);

#endif // TallinnNtupleProducer_Objects_GenMEt_h
