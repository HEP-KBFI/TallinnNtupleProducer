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

class RecoJetAK8 : public RecoJetBase
{
 public:
  RecoJetAK8() = default;
  RecoJetAK8(const GenJet & particle,
              Double_t msoftdrop,
              const RecoSubjetAK8* subJet1,
              const RecoSubjetAK8* subJet2,
              Double_t tau1,
              Double_t tau2,
              Double_t tau3,
              Double_t tau4,
              Int_t jetId,
              UInt_t idx,
              Int_t central_or_shift,
              Int_t ignored_corrections);

  virtual ~RecoJetAK8();

  /**
   * @brief Funtions to access data-members
   * @return Values of data-members
   */
  Double_t msoftdrop() const;
  const RecoSubjetAK8* subJet1() const;
  const RecoSubjetAK8* subJet2() const;
  Double_t tau1() const;
  Double_t tau2() const;
  Double_t tau3() const;
  Double_t tau4() const;
  Int_t jetId() const;
  Int_t get_default_systematics() const;
  Int_t get_ignored_corrections() const;

  friend class RecoJetReaderAK8;
  friend class RecoJetWriterAK8;

 protected: 
  Double_t msoftdrop_;
  std::shared_ptr<const RecoSubjetAK8> subJet1_;
  std::shared_ptr<const RecoSubjetAK8> subJet2_;
  Double_t tau1_;
  Double_t tau2_;
  Double_t tau3_;
  Double_t tau4_;
  Int_t jetId_;   ///< jet ID, as explained in https://twiki.cern.ch/twiki/bin/view/CMS/JetID13TeVRun2017

//---------------------------------------------------------
// needed by RecoJetWriter
  std::map<int, Double_t> pt_systematics_;
  std::map<int, Double_t> mass_systematics_;
  std::map<int, Double_t> msoftdrop_systematics_;
  int default_systematics_;

  std::map<int, Double_t> mass_corrections_;
  std::map<int, Double_t> sd_corrections_;
  int ignored_corrections_;
//---------------------------------------------------------
};

typedef std::vector<RecoJetAK8> RecoJetCollectionAK8;
typedef std::vector<const RecoJetAK8*> RecoJetPtrCollectionAK8;

std::ostream &
operator<<(std::ostream & stream,
           const RecoJetAK8 & jet);

#endif // TallinnNtupleProducer_Objects_RecoJetAK8_h
