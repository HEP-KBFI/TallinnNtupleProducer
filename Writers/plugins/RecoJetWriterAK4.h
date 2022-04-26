#ifndef TallinnNtupleProducer_Writers_RecoJetWriterAK4_h
#define TallinnNtupleProducer_Writers_RecoJetWriterAK4_h

#include "FWCore/ParameterSet/interface/ParameterSet.h"                       // edm::ParameterSet

#include "TallinnNtupleProducer/EvtWeightTools/interface/EvtWeightRecorder.h" // EvtWeightRecorder
#include "TallinnNtupleProducer/Objects/interface/Event.h"                    // Event
#include "TallinnNtupleProducer/Writers/interface/WriterBase.h"               // WriterBase

#include <string>                                                             // std::string
#include <vector>                                                             // std::vector

// forward declarations
class TTree;

class RecoJetWriterAK4 : public WriterBase
{
 public:
  RecoJetWriterAK4(const edm::ParameterSet & cfg);
  ~RecoJetWriterAK4();

  /**
   * @brief Call tree->Branch for all branches
   */
  void
  setBranches(TTree * tree);

  /**
   * @brief Switch branches to those for the central value or for systematic shifts.
   *        This method needs to be called before each call to the "write" method in the event loop.
   */
  void
  set_central_or_shift(const std::string & central_or_shift) const;
 
  /**
    * @brief Return list of systematic uncertainties supported by this plugin
    */
  static
  std::vector<std::string>
  get_supported_systematics();

 private:
  /**
   * @brief Write relevant information to tree
   */
  void
  writeImp(const Event & event, const EvtWeightRecorder & evtWeightRecorder);

  enum JetCollection { kUndefined, kSelJetsAK4, kSelJetsAK4_btagLoose, kSelJetsAK4_btagMedium };
  JetCollection jetCollection_;
  std::string branchName_num_;
  std::string branchName_obj_;

  UInt_t max_nJets_;

  struct central_or_shiftEntry
  {
    UInt_t nJets_;
    Float_t * pt_;
    Float_t * eta_;
    Float_t * phi_;
    Float_t * mass_;
    Int_t * charge_;
    Float_t * qgDiscr_;
    Float_t * bRegCorr_;
    Int_t * jetId_;
    Int_t * puId_;

    // CV: The value of the genMatch variable is bit-coded.
    //     The bit-coding is based on
    //       https://github.com/HEP-KBFI/tth-htt/blob/master/src/hadTauGenMatchingAuxFunctions.cc#L323-L359
    //     The values are:
    //       1 = isGenJet
    //       2 = isGenElectron
    //       4 = isGenMuon
    //       8 = isGenHadTau
    Int_t * genMatch_;
  };
  std::map<std::string, central_or_shiftEntry> central_or_shiftEntries_; // key = central_or_shift
  mutable central_or_shiftEntry * current_central_or_shiftEntry_;
};

#endif // TallinnNtupleProducer_Writers_RecoJetWriterAK4_h
