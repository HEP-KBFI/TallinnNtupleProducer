#ifndef TallinnNtupleProducer_Objects_RecoJetAK4_h
#define TallinnNtupleProducer_Objects_RecoJetAK4_h

/** \class RecoJetAK4
 *
 * Class to access information for "resolved" jets, 
 * reconstructed by anti-kT algorithm with dR=0.4
 *
 * \author Christian Veelken, Tallinn
 *
 */

#include "TallinnNtupleProducer/Objects/interface/RecoJetBase.h" // RecoJetBase

#include <map>                                                   // std::map

enum class Btag;
enum class pileupJetID;

class RecoJetAK4 : public RecoJetBase
{
 public:
  RecoJetAK4() = default;
  RecoJetAK4(const GenJet & particle,
             Double_t charge,
             Double_t BtagCSV,
             Double_t BtagWeight,
             Double_t QGDiscr,
             Double_t bRegCorr,
             Double_t bRegRes,
             Int_t jetId,
             Int_t puId,
             Int_t genMatchIdx,
             Int_t idx,
             Btag btag,
             Int_t central_or_shift);

  virtual ~RecoJetAK4();

  /**
   * @brief Set flags indicating whether or not jet passes loose and medium b-jet selection criteria
   */
  void set_isBJet_loose() const;
  void set_isBJet_medium() const;

  /**
   * @brief Funtions to access data-members
   * @return Values of data-members
   */
  Double_t charge() const;
  Double_t BtagCSV() const;
  Double_t BtagCSV(Btag btag) const;
  Double_t BtagWeight() const;
  Double_t BtagWeight(int central_or_shift) const;
  Double_t BtagWeight(Btag btag, int central_or_shift) const;
  Double_t QGDiscr() const;
  Double_t bRegCorr() const;
  Particle::LorentzVector p4_bRegCorr() const;
  Double_t bRegRes() const;
  Double_t pullEta() const;
  Double_t pullPhi() const;
  Double_t pullMag() const;
  Int_t jetId() const;
  Int_t puId() const;
  Int_t genMatchIdx() const;

  bool passesPUID(pileupJetID puIdWP) const;
  bool is_PUID_taggable() const;

  bool isBJet_loose() const;
  bool isBJet_medium() const;

  friend class RecoJetReaderAK4;

 protected:
  Double_t jetCharge_;  ///< jet charge, computed according to JME-13-006
  Double_t BtagCSV_;    ///< CSV b-tagging discriminator value
  Double_t BtagWeight_; ///< weight for data/MC correction of b-tagging efficiency and mistag rate
  Double_t QGDiscr_;    ///< quark/gluon discriminator
  Double_t bRegCorr_;   ///< pT correction for b-jet regression
  Double_t bRegRes_;    ///< resolution on pT corrected with b-jet regression
  Double_t pullEta_;    ///< eta component of pull vector, computed according to arXiv:1001.5027
  Double_t pullPhi_;    ///< phi component of pull vector, computed according to arXiv:1001.5027
  Double_t pullMag_;    ///< magnitude of pull vector, computed according to arXiv:1001.5027
  Int_t jetId_;         ///< jet ID, as explained in https://twiki.cern.ch/twiki/bin/view/CMS/JetID
  Int_t puId_;          ///< pileup jet ID, as explained in https://twiki.cern.ch/twiki/bin/viewauth/CMS/PileupJetID
  Int_t genMatchIdx_;   ///< index to gen jet
  Btag btag_;           ///< default b-tagging discriminant

  std::map<Btag, std::map<int, Double_t>> BtagWeight_systematics_;
  std::map<Btag, Double_t> BtagCSV_systematics_;

//--- flags indicating whether or not jet passes loose and medium b-jet selection criteria
  mutable bool isBJet_loose_;
  mutable bool isBJet_medium_;
};

typedef std::vector<RecoJetAK4> RecoJetCollectionAK4;
typedef std::vector<const RecoJetAK4*> RecoJetPtrCollectionAK4;

std::ostream &
operator<<(std::ostream & stream,
           const RecoJetAK4 & jet);

#endif // TallinnNtupleProducer_Objects_RecoJetAK4_h

