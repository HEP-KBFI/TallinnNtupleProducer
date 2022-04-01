#ifndef TallinnNtupleProducer_Objects_RecoVertex_h
#define TallinnNtupleProducer_Objects_RecoVertex_h

/** \class RecoVertex
 *
 * Class to access information for reconstructed primary event vertex
 *
 * \author Christian Veelken, Tallinn
 *
 */

#include "TallinnNtupleProducer/Objects/interface/Particle.h" // Particle::Point

#include <Rtypes.h>                                           // Int_t, Double_t

#include <iostream>                                           // std::ostream
#include <string>                                             // std::string
#include <map>                                                // std::map

class RecoVertex
{
 public:
  RecoVertex();
  RecoVertex(Double_t x, 
             Double_t y, 
             Double_t z, 
             Double_t ndof,
             Double_t chi2,
             Double_t score,
             Int_t npvs,
             Int_t npvsGood);
  ~RecoVertex();

  const Particle::Point position() const;
  Double_t x() const;
  Double_t y() const;
  Double_t z() const; 
  Double_t ndof() const;
  Double_t chi2() const;
  Double_t score() const;
  Int_t npvs() const;
  Int_t npvsGood() const;

  friend class RecoVertexReader;

 protected:
  Float_t position_x_;      ///< x-component of the vertex position (in units of cm)
  Float_t position_y_;      ///< y-component of the vertex position (in units of cm)
  Float_t position_z_;      ///< z-component of the vertex position (in units of cm)
  Float_t ndof_;            ///< number of degrees of freedom in vertex fit
  Float_t chi2_;            ///< chi^2 per degree of freedom
  Float_t score_;           ///< vertex score, i.e. sum pT^2 of clustered objects
  Int_t   npvs_;            ///< total number of reconstructed primary vertices
  Int_t   npvsGood_;        ///< number of good reconstructed primary vertices
                            ///< (selection = !isFake && ndof > 4 && abs(z) <= 24 && position.Rho <= 2)
};

std::ostream &
operator<<(std::ostream & stream,
           const RecoVertex & vertex);

#endif // TallinnNtupleProducer_Objects_RecoVertex_h


