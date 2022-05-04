#ifndef TallinnNtupleProducer_Writers_RecoHadTauWriter_h
#define TallinnNtupleProducer_Writers_RecoHadTauWriter_h

#include "FWCore/ParameterSet/interface/ParameterSet.h"                       // edm::ParameterSet

#include "TallinnNtupleProducer/EvtWeightTools/interface/EvtWeightRecorder.h" // EvtWeightRecorder
#include "TallinnNtupleProducer/Objects/interface/Event.h"                    // Event
#include "TallinnNtupleProducer/Writers/interface/WriterBase.h"               // WriterBase

#include <string>                                                             // std::string
#include <vector>                                                             // std::vector

// forward declarations
class TTree;

class RecoHadTauWriter : public WriterBase
{
 public:
  RecoHadTauWriter(const edm::ParameterSet & cfg);
  ~RecoHadTauWriter();

  /**
   * @brief Call tree->Branch for all branches
   */
  void
  setBranches(TTree * outputTree);

  /**
   * @brief Switch branches to those for the central value or for systematic shifts.
   *        This method needs to be called before each call to the "write" method in the event loop.
   */
  void
  set_central_or_shift(const std::string & central_or_shift) const;
 
  /**
    * @brief Return list of systematic uncertainties supported by this plugin
    */
  std::vector<std::string>
  get_supported_systematics(const edm::ParameterSet & cfg);

 private:
  /**
   * @brief Write relevant information to tree
   */
  void
  writeImp(const Event & event, const EvtWeightRecorder & evtWeightRecorder);

  std::string branchName_num_;
  std::string branchName_obj_;

  UInt_t max_nHadTaus_;

  struct central_or_shiftEntry
  {
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
  std::map<std::string, central_or_shiftEntry> central_or_shiftEntries_; // key = central_or_shift
  mutable central_or_shiftEntry * current_central_or_shiftEntry_;
};

#endif // TallinnNtupleProducer_Writers_RecoHadTauWriter_h
