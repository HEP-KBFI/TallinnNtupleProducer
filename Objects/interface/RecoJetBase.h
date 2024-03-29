#ifndef TallinnNtupleProducer_Objects_RecoJetBase_h
#define TallinnNtupleProducer_Objects_RecoJetBase_h

/** \class RecoJetBase
 *
 * Base-class for different type of jets
 *
 * Derrived classes:
 *   RecoJet      = "resolved" anti-kT jets with dR=0.4
 *   RecoJetHTTv2 = collection of jets reconstructed by the hep-top-tagger (HTTv2) algorithm, used for reconstruction of boosted hadronic tops
 *
 * \author Christian Veelken, Tallin
 *
 */

#include "TallinnNtupleProducer/Objects/interface/GenJet.h" // GenJet

// forward declarations
class GenHadTau;

class RecoJetBase : public GenJet
{
 public:
  RecoJetBase() = default;
  RecoJetBase(const GenJet & particle,
              Float_t rawFactor,
              UInt_t idx);

  virtual ~RecoJetBase();

  virtual void
  set_ptEtaPhiMass(Double_t pt,
                   Double_t eta,
                   Double_t phi,
                   Double_t mass) override;

  /**
   * @brief Set links to generator level particles (matched by dR)
   */
  void set_genLepton(const GenParticle * genLepton);
  void set_genHadTau(const GenHadTau * genHadTau);
  void set_genJet(const GenJet * genJet);

  /**
   * @brief Funtions to access data-members
   * @return Values of data-members
   */
  Float_t rawFactor() const;
  UInt_t idx() const;

  const GenParticle * genLepton() const;
  const GenHadTau * genHadTau() const;
  const GenJet * genJet() const;

  bool isGenMatched() const;
  bool hasAnyGenMatch() const;

 protected:
  Float_t rawFactor_; ///< 1 - Factor to get back to raw pT
  UInt_t idx_;        ///< index of jet in the ntuple

//--- matching to generator level particles
  const GenParticle * genLepton_;
  const GenHadTau * genHadTau_;
  const GenJet * genJet_;
};

std::ostream &
operator<<(std::ostream & stream,
           const RecoJetBase & jet);

#endif // TallinnNtupleProducer_Objects_RecoJetBase_h

