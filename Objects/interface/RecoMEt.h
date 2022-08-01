#ifndef TallinnNtupleProducer_Objects_RecoMEt_h
#define TallinnNtupleProducer_Objects_RecoMEt_h

#include "TallinnNtupleProducer/Objects/interface/GenMEt.h" // GenMEt

#include <TMatrixD.h>                                       // TMatrixD

class RecoMEt
  : public GenMEt
{
 public:
  RecoMEt();
  RecoMEt(const GenMEt & genMEt,
          Float_t sumEt,
          Float_t unclEnDeltaX,
          Float_t unclEnDeltaY,
          Float_t covXX,
          Float_t covXY,
          Float_t covYY);

  RecoMEt &
  operator=(const RecoMEt & other);

  /**
   * @brief Funtions to access data-members
   * @return Values of data-members
   */  
  Double_t sumEt() const;
  Double_t unclEnDeltaX() const;
  Double_t unclEnDeltaY() const;
  Double_t covXX() const;
  Double_t covXY() const;
  Double_t covYY() const;

  const TMatrixD & cov() const;

  friend class JMECorrector;

 protected:
  Float_t sumEt_;        ///< sum of transverse energies of all particles in the event
  Float_t unclEnDeltaX_; ///< Delta (METx_mod-METx) Unclustered Energy Up
  Float_t unclEnDeltaY_; ///< Delta (METy_mod-METy) Unclustered Energy Up
  Float_t covXX_;        ///< XX element of MET resolution matrix
  Float_t covXY_;        ///< XY element of MET resolution matrix
  Float_t covYY_;        ///< YY element of MET resolution matrix

  TMatrixD cov_;         ///< MET resolution matrix
};

std::ostream &
operator<<(std::ostream& stream,
           const RecoMEt & met);

#endif // TallinnNtupleProducer_Objects_RecoMEt_h
