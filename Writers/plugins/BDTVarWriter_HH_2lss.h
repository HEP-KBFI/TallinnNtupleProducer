#ifndef TallinnNtupleProducer_Writers_BDTVarWriter_HH_2lss_h
#define TallinnNtupleProducer_Writers_BDTVarWriter_HH_2lss_h

#include "FWCore/ParameterSet/interface/ParameterSet.h"                       // edm::ParameterSet

#include "TallinnNtupleProducer/EvtWeightTools/interface/EvtWeightRecorder.h" // EvtWeightRecorder
#include "TallinnNtupleProducer/Objects/interface/Event.h"                    // Event
#include "TallinnNtupleProducer/Writers/interface/WriterBase.h"               // WriterBase

#include <string>                                                             // std::string
#include <vector>                                                             // std::vector

// forward declarations
class TTree;

class BDTVarWriter_HH_2lss : public WriterBase
{
 public:
  BDTVarWriter_HH_2lss(const edm::ParameterSet & cfg);
  ~BDTVarWriter_HH_2lss();

  /**
   * @brief Call tree->Branch for all branches
   */
  void
  setBranches(TTree * outputTree);
 
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

  void
  resetBranches();

  Float_t m_ll_;
  Float_t dPhi_ll_;
  Float_t dEta_ll_;
  Float_t dR_ll_;
  Float_t mT_lep1_;
  Float_t mT_lep2_;
  Float_t min_dR_lep1_jet_;
  Float_t min_dR_lep2_jet_;
  Float_t min_dR_lep_Wjets_;
  Float_t max_dR_lep_Wjets_;
  Float_t min_dR_lep_leadWjet_;
  Float_t max_dR_lep_leadWjet_;
  Float_t dR_Wjj1_;
  Float_t m_Wjj1_;
  Float_t dR_Wjj2_;
  Float_t m_Wjj2_;
  Float_t mHH_vis_;
};

#endif // TallinnNtupleProducer_Writers_BDTVarWriter_HH_2lss_h
