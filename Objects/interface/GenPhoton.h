#ifndef TallinnNtupleProducer_Objects_GenPhoton_h
#define TallinnNtupleProducer_Objects_GenPhoton_h

#include "TallinnNtupleProducer/Objects/interface/GenParticle.h" // GenParticle

class GenPhoton : public GenParticle
{
 public:
  GenPhoton() = default;
  GenPhoton(Double_t pt,
            Double_t eta,
            Double_t phi,
            Double_t mass,
            Int_t pdgId,
            Int_t status,
            Int_t statusFlags,
            UChar_t genPartFlav);
  GenPhoton(const GenParticle & genParticle);

  virtual ~GenPhoton() {}
};

typedef std::vector<GenPhoton> GenPhotonCollection;
typedef std::vector<const GenPhoton*> GenPhotonPtrCollection;

std::ostream &
operator<<(std::ostream & stream,
           const GenPhoton & genPhoton);

#endif // TallinnNtupleProducer_Objects_GenPhoton_h
