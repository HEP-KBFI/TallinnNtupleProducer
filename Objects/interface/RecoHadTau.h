#ifndef TallinnNtupleProducer_Objects_RecoHadTau_h
#define TallinnNtupleProducer_Objects_RecoHadTau_h

#include "TallinnNtupleProducer/Objects/interface/GenHadTau.h" // GenHadTau

#include <map>                                                 // std::map

// forward declarations
class GenJet;

enum class TauID;

class RecoHadTau : public GenHadTau
{
 public:
  RecoHadTau() = default;
  RecoHadTau(const GenHadTau & particle,
             Double_t corrFactor,
             Double_t dxy,
             Double_t dz,
             Int_t decayMode,
             Bool_t idDecayMode,
             Int_t id_mva,
             Double_t raw_mva,
             Int_t antiElectron,
             Int_t antiMuon,
             Int_t jetIdx,
             UChar_t genPartFlav,
             Int_t genPartIdx);

  virtual ~RecoHadTau();

  /**
   * @brief Set flags indicating whether or not lepton passes loose, fakeable and/or tight selection criteria
   */
  void set_isLoose() const;
  void set_isFakeable() const;
  void set_isTight() const;

  /**
   * @brief Set links to generator level particles (matched by dR)
   */
  void set_genLepton(const GenParticle * genLepton);
  void set_genHadTau(const GenHadTau * genHadTau);
  void set_genJet(const GenJet * genJet);

  /**
   * @brief Set HLT filter bits
   */
  void set_filterBits(Int_t filterBits);

  /**
   * @brief Funtions to access data-members
   * @return Values of data-members
   */
  Double_t corrFactor() const;
  Double_t dxy() const;
  Double_t dz() const;
  Int_t decayMode() const;
  Bool_t idDecayMode() const;
  Int_t id_mva() const;
  Double_t raw_mva() const;
  Int_t id_mva(TauID tauID) const;
  Double_t raw_mva(TauID tauID) const;
  Int_t antiElectron() const;
  Int_t antiMuon() const;
  Int_t filterBits() const;
  Int_t jetIdx() const;
  UChar_t genPartFlav() const;
  Int_t genPartIdx() const;

  const GenParticle * genLepton() const;
  const GenHadTau * genHadTau() const;
  const GenJet * genJet() const;

  bool isGenMatched(bool requireChargeMatch) const;
  bool hasAnyGenMatch() const;

  bool isLoose() const;
  bool isFakeable() const;
  bool isTight() const;

  friend class RecoHadTauReader;
  friend class RecoHadTauWriter;

 protected:
  Double_t corrFactor_; ///< correction factor for the tau energy scale
  Double_t dxy_;        ///< d_{xy}, distance in the transverse plane w.r.t PV
  Double_t dz_;         ///< d_{z}, distance on the z axis w.r.t PV
  Int_t decayMode_;     ///< tau decay mode (5x(nof charged pions - 1) - (nof neutral pions))
  Bool_t idDecayMode_;  ///< old tau decay mode ID
  Int_t id_mva_;        ///< MVA-based tau id
  Double_t raw_mva_;    ///< raw output of MVA-based tau id
  Int_t antiElectron_;  ///< discriminator against electrons
  Int_t antiMuon_;      ///< discriminator against muons
  Int_t filterBits_;    ///< bitmask of matching with trigger objects
  Int_t jetIdx_;        ///< index of the matched jet from initial jet collection (-1 if no match)
  UChar_t genPartFlav_; ///< generator-level parton flavor (1 = prompt electron, 2 = prompt muon, 3 = tau->e decay,
                        ///<                                4 = tau->mu decay, 5 = hadronic tau decay, 0 = unknown/no match)
  Int_t genPartIdx_;   ///< index to matched gen particle (-1 if no match)

//--- matching to generator level particles
  const GenParticle * genLepton_;
  const GenHadTau * genHadTau_;
  const GenJet * genJet_;

//--- flags indicating whether or not lepton passes loose, fakeable and/or tight selection criteria
  mutable bool isLoose_;
  mutable bool isFakeable_;
  mutable bool isTight_;

//--- all tau IDs and their raw values
  std::map<TauID, int> tauID_ids_;
  std::map<TauID, Double_t> tauID_raws_;
};

typedef std::vector<RecoHadTau> RecoHadTauCollection;
typedef std::vector<const RecoHadTau*> RecoHadTauPtrCollection;

std::ostream &
operator<<(std::ostream & stream,
           const RecoHadTau & hadTau);

#endif // TallinnNtupleProducer_Objects_RecoHadTau_h

