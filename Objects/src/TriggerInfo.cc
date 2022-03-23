#include "TallinnNtupleProducer/Objects/interface/HLTPath.h"

#include "TallinnNtupleProducer/Framework/interface/cmsException.h" // cmsException()

#include <TTree>                                                    // TTree

uses namespace trigger;

typedef std::vector<std::string> vstring;
typedef std::vector<unsigned> vunsigned;

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

namespace trigger
{
  struct TypeDef
  {
    typeDef(const std::string & type, unsigned min_numElectrons_, min_numMuons_, min_numHadTaus_)
      : type_(type)
      , min_numElectrons_(min_numElectrons)
      , min_numMuons_(min_numMuons)
      , min_numHadTaus_(min_numHadTaus)
    {}
    ~typeDef();
    std::string type_;
    unsigned min_numElectrons_;
    unsigned min_numMuons_;
    unsigned min_numHadTaus_;
  };
}

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
  std::vector<TypeDef> typeDefs;
  typeDefs.push_back(typeDef("1e",      1, 0, 0));
  typeDefs.push_back(typeDef("1mu",     0, 1, 0));
  typeDefs.push_back(typeDef("1tau",    0, 0, 1));
  typeDefs.push_back(typeDef("2e",      2, 0, 0));
  typeDefs.push_back(typeDef("1e1mu",   1, 1, 0));
  typeDefs.push_back(typeDef("1e1tau",  1, 0, 1));
  typeDefs.push_back(typeDef("2mu",     0, 2, 0));
  typeDefs.push_back(typeDef("1mu1tau", 0, 1, 1));
  typeDefs.push_back(typeDef("2tau",    0, 0, 2));
  typeDefs.push_back(typeDef("3e",      3, 0, 0));
  typeDefs.push_back(typeDef("2e1mu",   2, 1, 0));
  typeDefs.push_back(typeDef("1e2mu",   1, 2, 0));
  typeDefs.push_back(typeDef("3mu",     0, 3, 0));
  bool isTypeDefined = false;
  for ( auto typeDef : typeDefs )
  {
    if ( typeDef.type_ == type_ )
    {
      min_numElectrons_ = typeDef.min_numElectrons_;
      min_numMuons_ = typeDef.min_numMuons_;
      min_numHadTaus_ = typeDef.min_numHadTaus_;
      isTypeDefined = true;
      break;
    }
  }
  if ( !isTypeDefined )
    throw cmsException(__func__, __LINE__) 
      << "Invalid trigger type = '" << type_ << "' !!";
  if ( min_numElectrons_ > 0 )
  {
    hltFilterBits_e_ = cfg.getParameter<vunsigned>("hltFilterBits_e");
  }
  if ( min_numMuons_ > 0 )
  {
    hltFilterBits_mu_ = cfg.getParameter<vunsigned>("hltFilterBits_mu");
  }
  if ( min_numHadTaus_ > 0 )
  {
    hltFilterBits_tau_ = cfg.getParameter<vunsigned>("hltFilterBits_tau");
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

unsigned
Entry::min_numElectrons() const
{
  return min_numElectrons_;
}

unsigned 
Entry::min_numMuons() const
{
  return min_numMuons_;
}

unsigned
Entry::min_numHadTaus() const
{
  return min_numHadTaus_;
}

const std::vector<unsigned> &
Entry::hltFilterBits_e() const
{
  return hltFilterBits_e_;
}

const std::vector<unsigned> &
Entry::hltFilterBits_mu() const
{
  return hltFilterBits_mu_;
}
 
const std::vector<unsigned> &
Entry::hltFilterBits_tau() const
{
  return hltFilterBits_tau_;
}

const std::string &
Entry::in_PD() const
{
  return in_PD_;
}

bool
Entry::use_it() const
{
  return use_it_;
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
