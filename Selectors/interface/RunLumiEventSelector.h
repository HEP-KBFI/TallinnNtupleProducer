#ifndef TallinnNtupleProducer_Selectors_RunLumiEventSelector_h
#define TallinnNtupleProducer_Selectors_RunLumiEventSelector_h

/** \class RunLumiEventSelector
 *
 * Select events based on run + luminosity section + event number pairs
 * written (a three columns separated by white-space character) into an ASCII file
 * 
 * \author Christian Veelken, Tallinn
 *
 */

#include "FWCore/ParameterSet/interface/ParameterSet.h"           // edm::ParameterSet

#include "TallinnNtupleProducer/Objects/interface/RunLumiEvent.h" // RunLumiEvent

#include <set> // std::set

class RunLumiEventSelector 
{
 public:
  explicit RunLumiEventSelector(const edm::ParameterSet & cfg);
  explicit RunLumiEventSelector(const std::string & inputFileName,
                                const std::string & separator = ":");
  virtual ~RunLumiEventSelector();

  bool
  operator()(UInt_t run,
             UInt_t lumi,
             ULong_t event) const;

  bool
  operator()(const RunLumiEvent & info) const;

  bool
  areWeDone() const;

 private:
  /// read ASCII file containing run and event numbers
  void readInputFile();
  
  std::string inputFileName_;
  std::string separator_;

  typedef UInt_t RunType;
  typedef UInt_t LumiSectionType;
  typedef ULong64_t EventType;

  typedef std::set<EventType> eventNumberSet;
  typedef std::map<LumiSectionType, eventNumberSet> lumiSectionEventNumberMap;
  std::map<RunType, lumiSectionEventNumberMap> runLumiSectionEventNumbers_;
  
  typedef std::map<EventType, int> matchedEventNumbersMap;
  typedef std::map<LumiSectionType, matchedEventNumbersMap> matchedLumiSectionEventNumberMap;
  mutable std::map<RunType, matchedLumiSectionEventNumberMap> matchedRunLumiSectionEventNumbers_;

  mutable RunType lastRun_;
  mutable LumiSectionType lastLumi_;
  mutable EventType lastEvent_;

  mutable long numEventsProcessed_;
  mutable long numEventsToBeSelected_;
  mutable long numEventsSelected_;
};

#endif // TallinnNtupleProducer_Selectors_RunLumiEventSelector_h
