#ifndef TallinnNtupleProducer_Objects_RunLumiEvent_h
#define TallinnNtupleProducer_Objects_RunLumiEvent_h

#include "RtypesCore.h" // UInt_t, ULong64_t

#include <iostream>                                                     // std::ostream
#include <string>                                                       // std::string
#include <map>                                                          // std::map

class RunLumiEvent
{
 public:
  RunLumiEvent();
  RunLumiEvent(UInt_t run,
               UInt_t lumi,
               ULong64_t event);

  ~RunLumiEvent();

  UInt_t run() const;
  UInt_t lumi() const;
  ULong64_t event() const;

  std::string
  str() const;

  friend class RunLumiEventReader;

 protected:
  UInt_t    run_;   ///< run number
  UInt_t    lumi_;  ///< luminosity
  ULong64_t event_; ///< event number
};

std::ostream &
operator<<(std::ostream & os,
           const RunLumiEvent & info);

#endif // TallinnNtupleProducer_Objects_RunLumiEvent_h
