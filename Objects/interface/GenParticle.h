#ifndef TallinnNtupleProducer_Objects_GenParticle_h
#define TallinnNtupleProducer_Objects_GenParticle_h

#include "TallinnNtupleProducer/Objects/interface/ChargedParticle.h" // ChargedParticle

enum class StatusFlag
{
  isPrompt                           = 0,
  isDecayedLeptonHadron              = 1,
  isTauDecayProduct                  = 2,
  isPromptTauDecayProduct            = 3,
  isDirectTauDecayProduct            = 4,
  isDirectPromptTauDecayProduct      = 5,
  isDirectHadronDecayProduct         = 6,
  isHardProcess                      = 7,
  fromHardProcess                    = 8,
  isHardProcessTauDecayProduct       = 9,
  isDirectHardProcessTauDecayProduct = 10,
  fromHardProcessBeforeFSR           = 11,
  isFirstCopy                        = 12,
  isLastCopy                         = 13,
  isLastCopyBeforeFSR                = 14,
};

class GenParticle : public ChargedParticle
{
 public:
  GenParticle();
  GenParticle(const GenParticle & genParticle);
  GenParticle(Double_t pt,
              Double_t eta,
              Double_t phi,
              Double_t mass,
              Int_t pdgId,
              Int_t charge,
              Int_t status = -1,
              Int_t statusFlags = -1,
              UChar_t genPartFlav = 0,
              UChar_t genHadronFlav = 0);
  GenParticle(const math::PtEtaPhiMLorentzVector & p4,
              Int_t pdgId,
              Int_t charge,
              Int_t status = -1,
              Int_t statusFlags = -1,
              UChar_t genPartFlav = 0,
              UChar_t genHadronFlav = 0);
  virtual ~GenParticle() {}

  /**
   * @brief Get the status of the gen particle
   * @return Status of the gen particle
   *
   * 0 - null
   * 1 - particle not decayed or fragmented, represents the final state as given by the generator
   * 2 - decayed or fragmented entry (i.e. decayed particle or parton produced in shower)
   * 3 - identifies the "hard part" of the interaction, i.e. the partons that are used
   *     in the matrix element calculation, including immediate decays of resonances
   */
  Int_t
  status() const;

  /**
   * @brief Get the status flags of the gen particle
   * @return Status flags of the gen particle
   *
   * The status flags are bitwise sum of statusflags, where each power of 2 has a specific meaning
   * @see StatusFlag
   */
  Int_t
  statusFlags() const;

  /**
   * @brief Get parton flavor of the gen particle
   * @return Code for the parton flavor
   *
   * The nomencalture is different for each type of gen particle:
   * - gen muons:
   *     1  - prompt muon (including gamma* -> mu mu)
   *     3  - muon from light or unknown
   *     4  - muon from c
   *     5  - muon from b
   *     15 - muon from prompt tau
   * - gen electrons:
   *     1  - prompt electron (including gamma* -> e e)
   *     3  - electron from light or unknown
   *     4  - electron from c
   *     5  - electron from b
   *     15 - electron from prompt tau
   *     22 - prompt photon (likely conversion)
   * - gen taus:
   *     1 - prompt electron
   *     2 - prompt muon
   *     3 - tau -> e decay
   *     4 - tau -> mu decay
   *     5 - hadronic tau decay
   * 0 universally means that the gen particle does has no parton flavor
   */
  UChar_t
  genPartFlav() const;

  UChar_t
  genHadronFlav() const;

  /**
   * @brief Sets the variable isMatchedToReco_ to true, indicating that
   *        the gen lvl object is now matched to a reco object
   */
  void
  setMatchedToReco();

  /**
   * @brief Check if the gen lvl object is already matched to a reco object
   * @return true, if the gen lvl object is matched to a reco object;
   *         false otherwise
   */
  bool
  isMatchedToReco() const;

  /**
   * @brief Check if the gen particle has a status flag
   * @param statusFlag Status flag to check
   * @return false, if the particle has not the status flag
   */
  bool
  checkStatusFlag(StatusFlag statusFlag) const;

 protected:
  Int_t status_;        ///< particle status (1 = stable)
  Int_t statusFlags_;   ///< gen status flags stored bitwise
  UChar_t genPartFlav_; ///< parton flavor of the gen particle
  UChar_t genHadronFlav_; ///< hadron flavor of the gen particle

  bool isMatchedToReco_; ///< true, if the gen object is already matched to a reco object
};

typedef std::vector<GenParticle> GenParticleCollection;
typedef std::vector<const GenParticle*> GenParticlePtrCollection;

std::ostream &
operator<<(std::ostream & stream,
           const GenParticle & particle);

#endif // TallinnNtupleProducer_Objects_GenParticle_h
