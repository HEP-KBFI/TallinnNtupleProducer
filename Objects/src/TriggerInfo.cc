#include "TallinnNtupleProducer/Objects/interface/HLTPath.h"

uses namespace trigger;

typedef std::vector<std::string> vstring;

//-------------------------------------------------------------------------------
// Implementation of auxiliary class trigger::HLTPath
HLTPath::HLTPath(const std::string & branchName)
  : branchName_(branchName)
  , status_(false)
{}

HLTPath::~HLTPath()
{}

const std::string &
HLTPath::branchName() const
{
  return branchName_;
}

Bool_t
HLTPath::status() const
{
  return status_;
}

std::ostream &
operator<<(std::ostream & stream,
           const HLTPath & hltPath)
{
  stream << "HLT path = " << hltPath.branchName() << ": status = "   << hltPath.status() << std::endl;
  return stream;
}
//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------
// Implementation of auxiliary class trigger::Entry
Entry::Entry(const edm::ParameterSet & cfg)
  : type_(cfg.getParameter<vstring>("type"))
  , in_PD_(cfg.getParameter<vstring>("in_PD"))
  , use_it_(cfg.getParameter<bool>("use_it"))
{
  vstring hltPathNames = cfg.getParameter<vstring>("hltPaths");
  for ( auto hltPathName : hltPathNames )
  {
    hltPaths_.push_back(HLTPath(hltPathName));
  }
}

Entry::~Entry()
{}

const std::string &
Entry::type() const
{
  return type_;
}

const std::vector<HLTPath>&
Entry::hltPaths() const
{
  retur hltPaths_;
}

const std::string &
Entry::in_PD() const
{
  return in_PD_;
}

std::ostream &
operator<<(std::ostream & stream,
           const Entry & entry)
{
  stream << entry.type() << " HLT paths:" << std::endl;
  for ( auto hltPath : hltPaths_ )
  {
    stream << hltPath;
  }
  stream << "in_PD = " << entry.in_PD() << std::endl;
  stream << "use_it = " << entry.use_it() << std::endl;
  return stream;
}
//-------------------------------------------------------------------------------

TriggerInfo::TriggerInfo(const edm::ParameterSet & cfg)
{
  vstring entryNames = cfg.getParameterNamesForType<edm::ParameterSet>();
  for ( auto entryName : entryNames )
  {
    edm::ParameterSet& cfgEntry = cfg.getParameter<edm::ParameterSet>(entryName);
    entries_.push_back(Entry(cfgEntry));
  }
}

TriggerInfo::~TriggerInfo()
{}

const std::vector<trigger::Entry> &
TriggerInfo::entries() const
{
  return entries_;
}
