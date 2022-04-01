#ifndef TallinnNtupleProducer_Objects_RecoMuon_h
#define TallinnNtupleProducer_Objects_RecoMuon_h

#include "TallinnNtupleProducer/Objects/interface/RecoLepton.h" // RecoLepton

class RecoMuon : public RecoLepton
{
 public:
  RecoMuon() = default;
  RecoMuon(const RecoLepton & lepton,
           Bool_t passesLooseIdPOG,
           Bool_t passesMediumIdPOG,
           Float_t segmentCompatibility,
           Float_t ptErr);

  /**
   * @brief Funtions to access data-members
   * @return Values of data-members
   */
  Bool_t
  passesLooseIdPOG() const;

  Bool_t
  passesMediumIdPOG() const;

  Float_t
  segmentCompatibility() const;

  Float_t
  ptErr() const;

  Float_t
  dpt_div_pt() const;

  /**
   * @brief Checks whether a given lepton is an electron by its PDG id
   * @return True if it is an electron; false otherwise
   */
  bool
  is_electron() const override;

  /**
   * @brief Checks whether a given lepton is a muon by its PDG id
   * @return True if it is a muon; false otherwise
   */
  bool
  is_muon() const override;

  Double_t
  cone_pt() const override;

  const Particle::LorentzVector &
  cone_p4() const override;

//--- observables specific to muons
  Bool_t passesLooseIdPOG_;      ///< flag indicating if muon passes (true) or fails (false) loose PFMuon id
  Bool_t passesMediumIdPOG_;     ///< flag indicating if muon passes (true) or fails (false) medium PFMuon id
  Float_t segmentCompatibility_; ///< muon segment compatibility
  Float_t ptErr_;                ///< pT error of the muon track
};

typedef std::vector<RecoMuon> RecoMuonCollection;
typedef std::vector<const RecoMuon*> RecoMuonPtrCollection;

std::ostream &
operator<<(std::ostream & stream,
           const RecoMuon & muon);

#endif // TallinnNtupleProducer_Objects_RecoMuon_h
