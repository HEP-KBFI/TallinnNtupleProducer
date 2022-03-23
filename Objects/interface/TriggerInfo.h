#ifndef TallinnNtupleProducer_Objects_TriggerInfo_h
#define TallinnNtupleProducer_Objects_HLTPath_h

#include "FWCore/ParameterSet/interface/ParameterSet.h"     // edm::ParameterSet

#include <TTree>                                            // TTree

#include <ostream>                                          // std::ostream
#include <string>                                           // std::string
#include <vector>                                           // std::vector

namespace trigger
{
  //-------------------------------------------------------------------------------
  // Declaration of auxiliary class trigger::HLTPath
  class HLTPath
  {
   public:
    HLTPath(const std::string & branchName);
    ~HLTPath() {}

    const std::string &
    branchName() const;

    /**
     * @brief Return status of HLT path for given event
     * @return true if event passes HLT path, false if it fails
     *
     */
    Bool_t
    status() const;

    friend class TriggerInfoReader;

   private:
    std::string branchName_;
    Bool_t status_;
  };

  std::ostream &
  operator<<(std::ostream & stream,
             const HLTPath & hltPath);
  //-------------------------------------------------------------------------------

  //-------------------------------------------------------------------------------
  // Declaration of auxiliary class trigger::Entry
  class Entry
  {
   public:
    Entry(const edm::ParameterSet & cfg);
    ~Entry();

    const std::string &
    type() const;

    const std::vector<HLTPath>&
    hltPaths() const;

    const std::string &
    in_PD() const;

    friend class TriggerInfoReader;

   private:
    std::string type_;
    std::vector<HLTPath> hltPaths_;
    std::string in_PD_;
    bool use_it_;
  };

  std::ostream &
  operator<<(std::ostream & stream,
             const Entry & entry);
  //-------------------------------------------------------------------------------
}

/**
 * @brief Handling of trigger information
 */
class TriggerInfo
{
 public:
  TriggerInfo(const edm::ParameterSet & cfg);
  ~TriggerInfo() {}

  const std::vector<trigger::Entry> &
  entries() const;

  friend class TriggerInfoReader;

 private:
  std::vector entries_;
};

std::ostream &
operator<<(std::ostream & stream,
           const TriggerInfo & triggerInfo);

#endif // TallinnNtupleProducer_Objects_TriggerInfo_h
