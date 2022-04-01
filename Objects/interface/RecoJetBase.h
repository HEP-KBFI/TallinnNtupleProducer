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

#include <memory>                                           // std::shared_ptr

// forward declarations
class GenLepton;
class GenHadTau;

class RecoJetBase : public GenJet
{
 public:
  RecoJetBase() = default;
  RecoJetBase(const GenJet & particle,
              Int_t idx);

  virtual ~RecoJetBase();

  /**
   * @brief Set links to generator level particles (matched by dR)
   */
  void set_genLepton(const GenLepton * genLepton);
  void set_genHadTau(const GenHadTau * genHadTau);
  void set_genJet(const GenJet * genJet);

  /**
   * @brief Funtions to access data-members
   * @return Values of data-members
   */
  Int_t idx() const;

  const GenLepton * genLepton() const;
  const GenHadTau * genHadTau() const;
  const GenJet * genJet() const;

  bool isGenMatched() const;
  bool hasAnyGenMatch() const;

 protected:
  Int_t idx_; ///< index of jet in the ntuple

//--- matching to generator level particles
  std::shared_ptr<const GenLepton> genLepton_;
  std::shared_ptr<const GenHadTau> genHadTau_;
  std::shared_ptr<const GenJet> genJet_;
};

std::ostream &
operator<<(std::ostream & stream,
           const RecoJetBase & jet);

#endif // TallinnNtupleProducer_Objects_RecoJetBase_h

