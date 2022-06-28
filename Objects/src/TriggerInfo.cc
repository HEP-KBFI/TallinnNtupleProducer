#include "TallinnNtupleProducer/Objects/interface/TriggerInfo.h"

#include "TallinnNtupleProducer/CommonTools/interface/cmsException.h" // cmsException()

#include <TTree.h>                                                    // TTree

typedef std::vector<std::string> vstring;
typedef std::vector<unsigned> vunsigned;

//-------------------------------------------------------------------------------
// Implementation of auxiliary class trigger::HLTPath
trigger::HLTPath::HLTPath(const std::string & branchName)
  : branchName_(branchName)
  , status_(false)
{}

trigger::HLTPath::~HLTPath()
{}

const std::string &
trigger::HLTPath::branchName() const
{
  return branchName_;
}

Bool_t
trigger::HLTPath::status() const
{
  return status_;
}

std::ostream &
trigger::operator<<(std::ostream & stream,
                    const trigger::HLTPath & hltPath)
{
  stream << "HLT path = " << hltPath.branchName() << ": status = "   << hltPath.status() << std::endl;
  return stream;
}
//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------
// Implementation of auxiliary class trigger::Set_of_HLTPaths

namespace trigger
{
  struct TypeDef
  {
    TypeDef(const std::string & type, unsigned min_numElectrons, unsigned min_numMuons, unsigned min_numHadTaus)
      : type_(type)
      , min_numElectrons_(min_numElectrons)
      , min_numMuons_(min_numMuons)
      , min_numHadTaus_(min_numHadTaus)
    {}
    ~TypeDef() {}
    std::string type_;
    unsigned min_numElectrons_;
    unsigned min_numMuons_;
    unsigned min_numHadTaus_;
  };
}

trigger::Set_of_HLTPaths::Set_of_HLTPaths(const edm::ParameterSet & cfg)
  : type_(cfg.getParameter<std::string>("type"))
  , in_PD_(cfg.getParameter<std::string>("in_PD"))
  , use_it_(cfg.getParameter<bool>("use_it"))
{
  vstring hltPathNames = cfg.getParameter<vstring>("hltPaths");
  for ( auto hltPathName : hltPathNames )
  {
    hltPaths_.push_back(HLTPath(hltPathName));
  }
  std::vector<trigger::TypeDef> typeDefs;
  typeDefs.push_back(TypeDef("1e",      1, 0, 0));
  typeDefs.push_back(TypeDef("1mu",     0, 1, 0));
  typeDefs.push_back(TypeDef("1tau",    0, 0, 1));
  typeDefs.push_back(TypeDef("2e",      2, 0, 0));
  typeDefs.push_back(TypeDef("1e1mu",   1, 1, 0));
  typeDefs.push_back(TypeDef("1e1tau",  1, 0, 1));
  typeDefs.push_back(TypeDef("2mu",     0, 2, 0));
  typeDefs.push_back(TypeDef("1mu1tau", 0, 1, 1));
  typeDefs.push_back(TypeDef("2tau",    0, 0, 2));
  typeDefs.push_back(TypeDef("3e",      3, 0, 0));
  typeDefs.push_back(TypeDef("2e1mu",   2, 1, 0));
  typeDefs.push_back(TypeDef("1e2mu",   1, 2, 0));
  typeDefs.push_back(TypeDef("3mu",     0, 3, 0));
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

trigger::Set_of_HLTPaths::~Set_of_HLTPaths()
{}

const std::string &
trigger::Set_of_HLTPaths::type() const
{
  return type_;
}

const std::vector<trigger::HLTPath>&
trigger::Set_of_HLTPaths::hltPaths() const
{
  return hltPaths_;
}

unsigned
trigger::Set_of_HLTPaths::min_numElectrons() const
{
  return min_numElectrons_;
}

unsigned 
trigger::Set_of_HLTPaths::min_numMuons() const
{
  return min_numMuons_;
}

unsigned
trigger::Set_of_HLTPaths::min_numHadTaus() const
{
  return min_numHadTaus_;
}

const std::vector<unsigned> &
trigger::Set_of_HLTPaths::hltFilterBits_e() const
{
  return hltFilterBits_e_;
}

const std::vector<unsigned> &
trigger::Set_of_HLTPaths::hltFilterBits_mu() const
{
  return hltFilterBits_mu_;
}
 
const std::vector<unsigned> &
trigger::Set_of_HLTPaths::hltFilterBits_tau() const
{
  return hltFilterBits_tau_;
}

const std::string &
trigger::Set_of_HLTPaths::in_PD() const
{
  return in_PD_;
}

bool
trigger::Set_of_HLTPaths::use_it() const
{
  return use_it_;
}

std::ostream &
trigger::operator<<(std::ostream & stream,
                    const trigger::Set_of_HLTPaths & entry)
{
  stream << entry.type() << " HLT paths:" << std::endl;
  for ( auto hltPath : entry.hltPaths() )
  {
    stream << hltPath;
  }
  stream << "in_PD = " << entry.in_PD() << std::endl;
  stream << "use_it = " << entry.use_it() << std::endl;
  return stream;
}
//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------
// Implementation of auxiliary class trigger::Object
trigger::Object::Object()
  : is_electron_(false)
  , is_muon_(false)
  , is_hadTau_(false)
  , eta_(0.)
  , phi_(0.)
  , filterBits_(0)
{}

trigger::Object::~Object()
{}

bool
trigger::Object::is_electron() const
{
  return is_electron_;
}

bool
trigger::Object::is_muon() const
{
  return is_muon_;
}

bool
trigger::Object::is_hadTau() const
{
  return is_hadTau_;
}
    
Float_t
trigger::Object::eta() const
{
  return eta_;
}

Float_t
trigger::Object::phi() const
{
  return phi_;
}

Int_t
trigger::Object::filterBits() const
{
  return filterBits_;
}
//-------------------------------------------------------------------------------

TriggerInfo::TriggerInfo(const edm::ParameterSet & cfg)
{
  vstring entryNames = cfg.getParameterNamesForType<edm::ParameterSet>();
  for ( auto entryName : entryNames )
  {
    edm::ParameterSet cfgEntry = cfg.getParameter<edm::ParameterSet>(entryName);
    entries_.push_back(trigger::Set_of_HLTPaths(cfgEntry));
  }
}

TriggerInfo::~TriggerInfo()
{}

const std::vector<trigger::Set_of_HLTPaths> &
TriggerInfo::entries() const
{
  return entries_;
}

const std::vector<trigger::Object> &
TriggerInfo::objects() const
{
  return objects_;
}

std::ostream &
operator<<(std::ostream & stream,
           const TriggerInfo & triggerInfo)
{
  for ( auto entry : triggerInfo.entries() )
  {
    stream << entry;
  }
  return stream;
}
