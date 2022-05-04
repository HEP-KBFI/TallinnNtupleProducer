#include "TallinnNtupleProducer/Objects/interface/RunLumiEvent.h"

#include <sstream> // std::stringstream

RunLumiEvent::RunLumiEvent()
  : run_(0)
  , lumi_(0)
  , event_(0)
{}

RunLumiEvent::RunLumiEvent(UInt_t run,
                           UInt_t lumi,
                           ULong64_t event)
  : run_(run)
  , lumi_(lumi)
  , event_(event)
{}

RunLumiEvent::~RunLumiEvent()
{}

UInt_t
RunLumiEvent::run() const
{
  return run_;
}

UInt_t
RunLumiEvent::lumi() const
{
  return lumi_;
}

ULong64_t
RunLumiEvent::event() const
{
  return event_;
}

std::string
RunLumiEvent::str() const
{
  std::stringstream ss;
  ss << run_ << ':' << lumi_ << ':' << event_;
  return ss.str();
}

std::ostream &
operator<<(std::ostream & os,
           const RunLumiEvent & info)
{
  os << "run = " << info.run() << ", ls = " << info.lumi() << ", event = " << info.event();
  return os;
}
