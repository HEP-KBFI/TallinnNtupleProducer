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
             Double_t BtagCSV,
             Double_t QGDiscr,
             Double_t bRegCorr,
             Double_t bRegRes,
             Int_t jetId,
             Int_t puId,
             UInt_t idx,
             Int_t genJetIdx,
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
  Double_t BtagCSV() const;
  Double_t BtagCSV(Btag btag) const;
  Double_t QGDiscr() const;
  Double_t bRegCorr() const;
  Particle::LorentzVector p4_bRegCorr() const;
  Double_t bRegRes() const;
  Int_t jetId() const;
  Int_t puId() const;
  Int_t genJetIdx() const;

  bool passesPUID(pileupJetID puIdWP) const;
  bool is_PUID_taggable() const;
  bool is_PU() const;

  bool isBJet_loose() const;
  bool isBJet_medium() const;

  friend class RecoJetReaderAK4;

 protected:
  Double_t BtagCSV_;    ///< CSV b-tagging discriminator value
  Double_t QGDiscr_;    ///< quark/gluon discriminator
  Double_t bRegCorr_;   ///< pT correction for b-jet regression
  Double_t bRegRes_;    ///< resolution on pT corrected with b-jet regression
  Int_t jetId_;         ///< jet ID, as explained in https://twiki.cern.ch/twiki/bin/view/CMS/JetID
  Int_t puId_;          ///< pileup jet ID, as explained in https://twiki.cern.ch/twiki/bin/viewauth/CMS/PileupJetID
  Int_t genJetIdx_;     ///< index to gen jet
  Btag btag_;           ///< default b-tagging discriminant

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

