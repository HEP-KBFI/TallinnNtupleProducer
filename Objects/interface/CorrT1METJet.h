#ifndef TallinnNtupleProducer_Objects_CorrT1METJet_h
#define TallinnNtupleProducer_Objects_CorrT1METJet_h

#include "TallinnNtupleProducer/Objects/interface/Particle.h" // Particle

// forward declarations
class GenJet;

class CorrT1METJet
  : public Particle
{
public:
  CorrT1METJet() = default;
  CorrT1METJet(Double_t rawPt,
               Double_t eta,
               Double_t phi,
               Double_t area,
               Double_t muonSubtrFactor);
  virtual ~CorrT1METJet() {}

  Double_t
  area() const;

  Double_t
  muonSubtrFactor() const;

protected:
  Double_t area_;
  Double_t muonSubtrFactor_;
};

typedef std::vector<CorrT1METJet> CorrT1METJetCollection;
typedef std::vector<const CorrT1METJet *> CorrT1METJetPtrCollection;

std::ostream &
operator<<(std::ostream & stream,
           const CorrT1METJet & jet);

#endif // TallinnNtupleProducer_Objects_CorrT1METJet_h
