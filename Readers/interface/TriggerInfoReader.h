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

  /**
   * @brief Read branches from tree and use information to fill TriggerInfo object
   * @return TriggerInfo object
   */
  const TriggerInfo &
  read() const;

 private:
  std::vector<std::string>
  get_available_branches(TTree * tree) const;

  TriggerInfo triggerInfo_;
};

#endif // TallinnNtupleProducer_Readers_TriggerInfoReader_h
