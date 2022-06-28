#ifndef TallinnNtupleProducer_Readers_TriggerInfoReader_h
#define TallinnNtupleProducer_Readers_TriggerInfoReader_h

#include "FWCore/ParameterSet/interface/ParameterSet.h"          // edm::ParameterSet

#include "TallinnNtupleProducer/Readers/interface/ReaderBase.h"  // ReaderBase
#include "TallinnNtupleProducer/Objects/interface/TriggerInfo.h" // TriggerInfo

#include <vector>                                                // std::vector

class TriggerInfoReader : public ReaderBase
{
 public:
  TriggerInfoReader(const edm::ParameterSet & cfg);
  ~TriggerInfoReader();

  /**
   * @brief Call tree->SetBranchAddress for all HLT paths
   */
  std::vector<std::string>
  setBranchAddresses(TTree * tree) override;

  void
  setBranchNames();

  /**
   * @brief Read branches from tree and use information to fill TriggerInfo object
   * @return reference to TriggerInfo object
   */
  const TriggerInfo &
  read() const;

  /**
    * @brief Return list of systematic uncertainties supported by TriggerInfoReader class
    */
  static
  std::vector<std::string>
  get_supported_systematics(const edm::ParameterSet & cfg);

 private:
  std::vector<std::string>
  get_available_branches(TTree * tree) const;

  unsigned int max_nTriggerObjects_;
  mutable TriggerInfo triggerInfo_;
  std::string branchName_num_;
  std::string branchName_obj_;
  std::string branchName_triggerObj_id_;
  std::string branchName_triggerObj_filterBits_;
  std::string branchName_triggerObj_eta_;
  std::string branchName_triggerObj_phi_;

  UInt_t ntriggerObj_;
  Int_t * triggerObj_id_;
  Int_t * triggerObj_filterBits_;
  Float_t * triggerObj_eta_;
  Float_t * triggerObj_phi_;

  static std::map<std::string, int> numInstances_;
  static std::map<std::string, TriggerInfoReader *> instances_;
};

#endif // TallinnNtupleProducer_Readers_TriggerInfoReader_h
