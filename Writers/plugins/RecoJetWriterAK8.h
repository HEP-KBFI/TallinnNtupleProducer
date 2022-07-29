#ifndef TallinnNtupleProducer_Writers_RecoJetWriterAK8_h
#define TallinnNtupleProducer_Writers_RecoJetWriterAK8_h

#include "FWCore/ParameterSet/interface/ParameterSet.h"                       // edm::ParameterSet

#include "TallinnNtupleProducer/EvtWeightTools/interface/EvtWeightRecorder.h" // EvtWeightRecorder
#include "TallinnNtupleProducer/Objects/interface/Event.h"                    // Event
#include "TallinnNtupleProducer/Writers/interface/WriterBase.h"               // WriterBase

#include <string>                                                             // std::string
#include <vector>                                                             // std::vector

// forward declarations
class TTree;

class RecoJetWriterAK8 : public WriterBase
{
 public:
  RecoJetWriterAK8(const edm::ParameterSet & cfg);
  ~RecoJetWriterAK8();

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

  enum JetCollection { kUndefined, kSelJetsAK8_Wjj, kSelJetsAK8_Hbb };
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
    Float_t * msoftdrop_; // [*]
    Float_t * tau21_; // tau2/tau1
    Int_t * jetId_;
    Float_t * subjet1_pt_;
    Float_t * subjet2_pt_;
  };
  // [*] should be equal to the mass of the subjet pair, see:
  // https://github.com/cms-sw/cmssw/blob/a1ee449d0c95cbe0ce77e39a3ba396879d0437a4/PhysicsTools/NanoAOD/python/jetsAK8_cff.py#L105
  // https://github.com/cms-sw/cmssw/blob/a1ee449d0c95cbe0ce77e39a3ba396879d0437a4/DataFormats/PatCandidates/interface/Jet.h#L580-L590
  std::map<std::string, central_or_shiftEntry> central_or_shiftEntries_; // key = central_or_shift
  mutable central_or_shiftEntry * current_central_or_shiftEntry_;
};

#endif // TallinnNtupleProducer_Writers_RecoJetWriterAK4_h
