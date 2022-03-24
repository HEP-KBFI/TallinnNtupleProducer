#ifndef TallinnNtupleProducer_Writers_RecoLeptonWriter_h
#define TallinnNtupleProducer_Writers_RecoLeptonWriter_h

#include "FWCore/ParameterSet/interface/ParameterSet.h"         // edm::ParameterSet

#include "TallinnNtupleProducer/Writers/interface/WriterBase.h" // WriterBase
#include "TallinnNtupleProducer/Objects/interface/Event.h"      // Event

#include <string>                                               // std::string
#include <vector>                                               // std::vector

class TTree; // forward declaration

class RecoLeptonWriter : public WriterBase
{
 public:
  RecoLeptonWriter(const edm::ParameterSet & cfg);
  ~RecoLeptonWriter();

  /**
   * @brief Call tree->Branch for all branches
   */
  void
  setBranches(TTree * tree);

  /**
   * @brief Write relevant information to tree
   */
  void
  write(const Event & event);
 
 private:
  std::string branchName_num_;
  std::string branchName_obj_;

  UInt_t nLeptons_;
  Float_t * pt_;
  Float_t * eta_;
  Float_t * phi_;
  Float_t * mass_;
  Int_t * pdgId_;
  Int_t * charge_;
  Float_t * mvaRawTTH_;
  Bool_t * isFakeable_;
  Bool_t * isTight_;
  Int_t * tightCharge_;
  // CV: The value of the genMatch variable is bit-coded.
  //     The bit-coding is based on
  //       https://github.com/HEP-KBFI/tth-htt/blob/master/src/leptonGenMatchingAuxFunctions.cc#L323-L351
  //     The values are:
  //       1 = isGenLepton
  //       2 = isGenLeptonChargeFlip
  //       4 = isGenPhoton (only set if lepton is an electron)
  //       8 = isGenHadTau
  //      16 = isGenJet
  Int_t * genMatch_;
  Bool_t * isFake_; // true if genMatch = 16, false otherwise
  Bool_t * isFlip_; // true if genMatch = 2, false otherwise
};

#endif // TallinnNtupleProducer_Writers_RecoLeptonWriter_h
