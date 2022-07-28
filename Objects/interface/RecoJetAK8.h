#ifndef TallinnNtupleProducer_Objects_RecoJetAK8_h
#define TallinnNtupleProducer_Objects_RecoJetAK8_h

/** \class RecoJetAK8
 *
 * Class to access information for jets reconstructed by the anti-kT algorithm with R=1.2 (AK8),
 * used for reconstruction of boosted W bosons (produced in hadronic top decays with resolved b-jets)
 *
 * \author Christian Veelken, Tallin
 *
 */

#include "TallinnNtupleProducer/Objects/interface/RecoJetBase.h"   // RecoJetBase
#include "TallinnNtupleProducer/Objects/interface/RecoSubjetAK8.h" // RecoSubjetAK8

#include <memory>                                                  // std::shared_ptr
#include <map>                                                     // std::map

class RecoJetAK8
  : public RecoJetBase
{
 public:
  RecoJetAK8() = default;
  RecoJetAK8(const GenJet & particle,
             Float_t msoftdrop,
             const RecoSubjetAK8* subJet1,
             const RecoSubjetAK8* subJet2,
             Float_t tau1,
             Float_t tau2,
             Float_t tau3,
             Float_t tau4,
             Int_t jetId,
             Float_t rawFactor,
             Float_t area,
             Int_t genJetAK8Idx,
             UInt_t idx);

  virtual ~RecoJetAK8();

  /**
   * @brief Funtions to access data-members
   * @return Values of data-members
   */
  Float_t msoftdrop() const;
  const RecoSubjetAK8* subJet1() const;
  const RecoSubjetAK8* subJet2() const;
  Float_t tau1() const;
  Float_t tau2() const;
  Float_t tau3() const;
  Float_t tau4() const;
  Int_t jetId() const;
  Float_t area() const;
  Int_t genJetAK8Idx() const;
  Int_t get_default_systematics() const;
  Int_t get_ignored_corrections() const;

  friend class RecoJetReaderAK8;
  friend class RecoJetWriterAK8;

 protected: 
  Float_t msoftdrop_;
  std::shared_ptr<const RecoSubjetAK8> subJet1_;
  std::shared_ptr<const RecoSubjetAK8> subJet2_;
  Float_t tau1_;
  Float_t tau2_;
  Float_t tau3_;
  Float_t tau4_;
  Int_t jetId_;     ///< jet ID, as explained in https://twiki.cern.ch/twiki/bin/view/CMS/JetID13TeVRun2017
  Float_t area_;    ///< jet catchment area
  Int_t genJetAK8Idx_;
};

typedef std::vector<RecoJetAK8> RecoJetCollectionAK8;
typedef std::vector<const RecoJetAK8*> RecoJetPtrCollectionAK8;

std::ostream &
operator<<(std::ostream & stream,
           const RecoJetAK8 & jet);

#endif // TallinnNtupleProducer_Objects_RecoJetAK8_h
