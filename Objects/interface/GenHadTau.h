#ifndef TallinnNtupleProducer_Objects_GenHadTau_h
#define TallinnNtupleProducer_Objects_GenHadTau_h

#include "TallinnNtupleProducer/Objects/interface/GenParticle.h" // GenParticle

class GenHadTau : public GenParticle
{
 public:
  GenHadTau() = default;
  GenHadTau(Double_t pt,
            Double_t eta,
            Double_t phi,
            Double_t mass,
            Int_t charge,
            Int_t status = -1);

  virtual ~GenHadTau() {}
};

typedef std::vector<GenHadTau> GenHadTauCollection;
typedef std::vector<const GenHadTau*> GenHadTauPtrCollection;

std::ostream &
operator<<(std::ostream & stream,
           const GenHadTau & hadTau);

#endif // TallinnNtupleProducer_Objects_GenHadTau_h
