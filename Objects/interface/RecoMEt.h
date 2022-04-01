#ifndef TallinnNtupleProducer_Objects_RecoMEt_h
#define TallinnNtupleProducer_Objects_RecoMEt_h

#include "TallinnNtupleProducer/Objects/interface/GenParticle.h" // Particle::LorentzVector

#include <TMatrixD.h>                                            // TMatrixD

#include <map>                                                   // std::map

class RecoMEt
{
 public:
  RecoMEt();

  RecoMEt(Float_t pt,
          Float_t phi,
          Float_t covXX,
          Float_t covXY,
          Float_t covYY);

  RecoMEt(const math::PtEtaPhiMLorentzVector & p4,
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
  friend class RecoMEtWriter;

  struct MEt
  {
    MEt();

    MEt(Float_t pt,
        Float_t phi);

    double
    px() const;

    double
    py() const;

    void
    shift_PxPy(const std::pair<double, double> & PxPyCorr);

    MEt &
    operator=(const MEt & other);

    Float_t pt_;  ///< pT of missing transverse momentum vector
    Float_t phi_; ///< phi of missing transverse momentum vector
  };

  void
  set_default(int central_or_shift);

  bool
  has_systematics(int central_or_shift) const;

  void
  set_systematics(const MEt & met,
                  int central_or_shift,
                  bool replace = false);

  MEt
  get_systematics(int central_or_shift) const;

  int
  get_default_systematics() const;

  void
  shift_PxPy(const std::pair<double, double> & PxPyCorr);

 protected:
  MEt default_; ///< Default values
  std::map<int, MEt> systematics_; ///< Needed by RecoMEtReader/Writer
  int default_systematics_;

  Float_t covXX_; ///< XX element of MET resolution matrix
  Float_t covXY_; ///< XY element of MET resolution matrix
  Float_t covYY_; ///< YY element of MET resolution matrix

  ///< (default) 4-momentum constructed from pT and phi, assuming eta and mass to be equal to zero
  math::PtEtaPhiMLorentzVector p4_;

  Float_t sumEt_; ///< sum of transverse energies of all particles in the event

  TMatrixD cov_; ///< MET resolution matrix

  Float_t genPt_;  ///< pT of generator level missing transverse momentum vector
  Float_t genPhi_;

  ///< Update cov and p4 (needed by RecoMEtReader)
  void update();
  void update_p4();
  void update_cov();
};

std::ostream &
operator<<(std::ostream& stream,
           const RecoMEt & met);

std::ostream &
operator<<(std::ostream & stream,
           const RecoMEt::MEt & met);


#endif // TallinnNtupleProducer_Objects_RecoMEt_h
