#ifndef TallinnNtupleProducer_Objects_RecoMEt_h
#define TallinnNtupleProducer_Objects_RecoMEt_h

#include "TallinnNtupleProducer/Objects/interface/GenParticle.h" // Particle::LorentzVector

#include <TMatrixD.h>                                            // TMatrixD

class RecoMEt
{
 public:
  RecoMEt();
  RecoMEt(Float_t pt,
          Float_t phi,
          Float_t sumEt,
          Float_t unclEnDeltaX,
          Float_t unclEnDeltaY,
          Float_t covXX,
          Float_t covXY,
          Float_t covYY);
  RecoMEt(const math::PtEtaPhiMLorentzVector & p4,
          Float_t sumEt,
          Float_t unclEnDeltaX,
          Float_t unclEnDeltaY,
          const TMatrixD& cov);

  RecoMEt &
  operator=(const RecoMEt & other);

  /**
   * @brief Funtions to access data-members
   * @return Values of data-members
   */  
  Double_t pt() const;
  Double_t phi() const;
  Double_t sumEt() const;
  Double_t unclEnDeltaX() const;
  Double_t unclEnDeltaY() const;
  Double_t covXX() const;
  Double_t covXY() const;
  Double_t covYY() const;

  const Particle::LorentzVector & p4() const;
  const TMatrixD & cov() const;

  /**
   * @brief Set generator level missing transverse momentum
   */
  void set_genPt(double genPt);
  void set_genPhi(double genPhi);

  Double_t genPt() const;
  Double_t genPhi() const;

  friend class RecoMEtReader;
  friend class JMECorrector;

 protected:
  void
  set(Double_t pt,
      Double_t phi);
  
  Float_t pt_;           ///< pT of missing transverse momentum vector
  Float_t phi_;          ///< phi of missing transverse momentum vector
  Float_t sumEt_;        ///< sum of transverse energies of all particles in the event
  Float_t unclEnDeltaX_; ///< Delta (METx_mod-METx) Unclustered Energy Up
  Float_t unclEnDeltaY_; ///< Delta (METy_mod-METy) Unclustered Energy Up
  Float_t covXX_;        ///< XX element of MET resolution matrix
  Float_t covXY_;        ///< XY element of MET resolution matrix
  Float_t covYY_;        ///< YY element of MET resolution matrix

  ///< four-momentum constructed from pT and phi, assuming eta and mass to be equal to zero
  math::PtEtaPhiMLorentzVector p4_;

  TMatrixD cov_;         ///< MET resolution matrix

  Float_t genPt_;        ///< pT of generator level missing transverse momentum vector
  Float_t genPhi_;       ///< phi of generator level missing transverse momentum vector
};

std::ostream &
operator<<(std::ostream& stream,
           const RecoMEt & met);

#endif // TallinnNtupleProducer_Objects_RecoMEt_h
