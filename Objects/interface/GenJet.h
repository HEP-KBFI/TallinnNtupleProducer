#ifndef TallinnNtupleProducer_Objects_GenJet_h
#define TallinnNtupleProducer_Objects_GenJet_h

#include "TallinnNtupleProducer/Objects/interface/GenParticle.h" // GenParticle

class GenJet : public GenParticle
{
 public:
  GenJet() = default;
  // ctor needed by RecoJetReaderAK4
  GenJet(Double_t pt,
         Double_t eta,
         Double_t phi,
         Double_t mass);
  GenJet(Double_t pt,
         Double_t eta,
         Double_t phi,
         Double_t mass,
         Int_t partonFlavour,
         Int_t hadronFlavour);
  GenJet(const GenParticle & genParticle);

  virtual ~GenJet() {};

  Int_t
  partonFlav() const;

  UChar_t
  hadronFlav() const;

 protected:
  Int_t partonFlav_;
  UChar_t hadronFlav_;
};

typedef std::vector<GenJet> GenJetCollection;
typedef std::vector<const GenJet*> GenJetPtrCollection;

std::ostream &
operator<<(std::ostream & stream,
           const GenJet & genJet);

#endif // TallinnNtupleProducer_Objects_GenJet_h
