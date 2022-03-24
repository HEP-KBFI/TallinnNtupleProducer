#ifndef TallinnNtupleProducer_Writers_RecoHadTauWriter_h
#define TallinnNtupleProducer_Writers_RecoHadTauWriter_h

#include "FWCore/ParameterSet/interface/ParameterSet.h"         // edm::ParameterSet

#include "TallinnNtupleProducer/Writers/interface/WriterBase.h" // WriterBase
#include "TallinnNtupleProducer/Objects/interface/Event.h"      // Event

#include <string>                                               // std::string
#include <vector>                                               // std::vector

class TTree; // forward declaration

class RecoHadTauWriter : public WriterBase
{
 public:
  RecoHadTauWriter(const edm::ParameterSet & cfg);
  ~RecoHadTauWriter();

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

  UInt_t nHadTaus_;
  Float_t * pt_;
  Float_t * eta_;
  Float_t * phi_;
  Float_t * mass_;
  Int_t * decayMode_;
  Int_t * charge_;
  Bool_t * isFakeable_;
  Bool_t * isTight_;
  // CV: The value of the genMatch variable is bit-coded.
  //     The bit-coding is based on
  //       https://github.com/HEP-KBFI/tth-htt/blob/master/src/hadTauGenMatchingAuxFunctions.cc#L323-L359
  //     The values are:
  //       1 = isGenHadTau
  //       2 = isGenHadTauChargeFlip
  //       4 = isGenElectron
  //       8 = isGenElectronChargeFlip
  //      16 = isGenMuon
  //      32 = isGenMuonChargeFlip
  //      64 = isGenJet
  Int_t * genMatch_;
  Bool_t * isFake_; // true if genMatch = 64, false otherwise
  Bool_t * isFlip_; // true if genMatch = 2 or 8 or 32, false otherwise
};

#endif // TallinnNtupleProducer_Writers_RecoHadTauWriter_h
