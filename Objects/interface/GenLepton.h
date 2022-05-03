#ifndef TallinnNtupleProducer_Objects_GenLepton_h
#define TallinnNtupleProducer_Objects_GenLepton_h

#include "TallinnNtupleProducer/Objects/interface/GenParticle.h" // GenParticle

class GenLepton : public GenParticle
{
 public:
  GenLepton() = default;
  GenLepton(Double_t pt,
            Double_t eta,
            Double_t phi,
            Double_t mass,
            Int_t pdgId,
            Int_t status,
            Int_t statusFlags,
            UChar_t genPartFlav);
  GenLepton(Double_t pt,
            Double_t eta,
            Double_t phi,
            Double_t mass,
            Int_t pdgId,
            Int_t charge,
            Int_t status,
            Int_t statusFlags,
            UChar_t genPartFlav);
  GenLepton(const math::PtEtaPhiMLorentzVector & p4,
            Int_t pdgId,
            Int_t status,
            Int_t statusFlags,
            UChar_t genPartFlav);
  GenLepton(const GenParticle & genParticle);

  virtual ~GenLepton() {}

  /**
   * @brief Checks whether a given lepton is an electron by its PDG id
   * @return True if it is an electron; false otherwise
   */
  virtual bool
  is_electron() const;

  /**
   * @brief Checks whether a given lepton is a muon by its PDG id
   * @return True if it is a muon; false otherwise
   */
  virtual bool
  is_muon() const;
};

typedef std::vector<GenLepton> GenLeptonCollection;
typedef std::vector<const GenLepton*> GenLeptonPtrCollection;

std::ostream &
operator<<(std::ostream & stream,
           const GenLepton & lepton);

#endif // TallinnNtupleProducer_Objects_GenLepton_h
