#include "TallinnNtupleProducer/Readers/interface/TriggerInfoReader.h"

#include "TallinnNtupleProducer/CommonTools/interface/BranchAddressInitializer.h" // BranchAddressInitializer
#include "TallinnNtupleProducer/CommonTools/interface/cmsException.h"             // cmsException()

#include <set>                                                                    // std::set
#include <string>                                                                 // std::string

std::map<std::string, int> TriggerInfoReader::numInstances_;
std::map<std::string, TriggerInfoReader *> TriggerInfoReader::instances_;

TriggerInfoReader::TriggerInfoReader(const edm::ParameterSet & cfg)
  : ReaderBase(cfg)
  , max_nTriggerObjects_(64)
  , triggerInfo_(cfg)
  , branchName_num_("")
  , branchName_obj_("")
  , triggerObj_id_(nullptr)
  , triggerObj_filterBits_(nullptr)
  , triggerObj_eta_(nullptr)
  , triggerObj_phi_(nullptr)
{
  branchName_obj_ = "TrigObj";//cfg.getParameter<std::string>("branchName"); // default = "TriObj"
  branchName_num_ = Form("n%s", branchName_obj_.data());
  setBranchNames();
}

TriggerInfoReader::~TriggerInfoReader()
{
  --numInstances_[branchName_obj_];
  assert(numInstances_[branchName_obj_] >= 0);
  if(numInstances_[branchName_obj_] == 0)
  {
    TriggerInfoReader * const gInstance = instances_[branchName_obj_];
    assert(gInstance);
    delete [] gInstance->triggerObj_id_;
    delete [] gInstance->triggerObj_filterBits_;
    delete [] gInstance->triggerObj_eta_;
    delete [] gInstance->triggerObj_phi_;
    instances_[branchName_obj_] = nullptr;
  }
}

void
TriggerInfoReader::setBranchNames()
{
  if (numInstances_[branchName_obj_] == 0)
  {
      branchName_triggerObj_id_ = Form("%s_id", branchName_obj_.data());
      branchName_triggerObj_filterBits_ = Form("%s_filterBits", branchName_obj_.data());
      branchName_triggerObj_eta_ = Form("%s_eta", branchName_obj_.data());
      branchName_triggerObj_phi_ = Form("%s_phi", branchName_obj_.data());
    instances_[branchName_obj_] = this;
  }
  else
    {
      if(branchName_num_ != instances_[branchName_obj_]->branchName_num_)
        {
          throw cmsException(this)
            << "Association fails";
        }
    }
  ++numInstances_[branchName_obj_];
}

std::vector<std::string>
TriggerInfoReader::setBranchAddresses(TTree * tree)
{
  BranchAddressInitializer bai(tree);
  if(instances_[branchName_obj_] == this){
  const std::vector<std::string> available_branches = this->get_available_branches(tree);
  std::set<std::string> used_branches;
  for ( trigger::Entry & entry : triggerInfo_.entries_ )
  {
    for ( trigger::HLTPath & hltPath : entry.hltPaths_ )
    {
      if ( std::find(available_branches.cbegin(), available_branches.cend(), hltPath.branchName()) != available_branches.cend() )
      {
        if ( used_branches.find(hltPath.branchName_) != used_branches.end() )
          throw cmsException(__func__, __LINE__) 
            << "Branch '" << hltPath.branchName() << "' cannot be read more than once !!";
        bai.setBranchAddress(hltPath.status_, hltPath.branchName_);         
        used_branches.insert(hltPath.branchName());
      }
      else
      {
        std::cerr << "Warning in <TriggerInfoReader::setBranchAddresses>:" 
                  << " Branch '" << hltPath.branchName_ << "' not available, defaulting to false" << std::endl;
        hltPath.status_ = false;
      }
    }
  }
  bai.setBranchAddress(ntriggerObj_, branchName_num_);
  bai.setLenVar(max_nTriggerObjects_).setBranchAddress(triggerObj_id_, branchName_triggerObj_id_);
  bai.setLenVar(max_nTriggerObjects_).setBranchAddress(triggerObj_filterBits_, branchName_triggerObj_filterBits_);
  bai.setLenVar(max_nTriggerObjects_).setBranchAddress(triggerObj_eta_, branchName_triggerObj_eta_);
  bai.setLenVar(max_nTriggerObjects_).setBranchAddress(triggerObj_phi_, branchName_triggerObj_phi_);
  }
  return bai.getBoundBranchNames_read();
}

const TriggerInfo &
TriggerInfoReader::read() const
{
  triggerInfo_.ele_trigobj_.clear();
  triggerInfo_.muon_trigobj_.clear();
  triggerInfo_.tau_trigobj_.clear();
  triggerInfo_.triggerObj_filterBits_.clear();
  triggerInfo_.triggerObj_eta_.clear();
  triggerInfo_.triggerObj_phi_.clear();

  const TriggerInfoReader * const gTrigger = instances_[branchName_obj_];
  const UInt_t nTriggerObjects = gTrigger->ntriggerObj_;
  if(nTriggerObjects > max_nTriggerObjects_)
  {
    throw cmsException(this)
      << "Number of Trigger Objects stored in Ntuple = " << nTriggerObjects
      << ", exceeds m = "  << max_nTriggerObjects_
      ;
  }
  for (UInt_t itrig=0; itrig<nTriggerObjects; itrig++)
  {
    if ( gTrigger->triggerObj_id_[itrig] == 11 )
    {
      triggerInfo_.ele_trigobj_.push_back(itrig);
    }
    else if ( gTrigger->triggerObj_id_[itrig] == 13 )
    {
      triggerInfo_.muon_trigobj_.push_back(itrig);
    }
    else if ( gTrigger->triggerObj_id_[itrig] == 15 )
    {
      triggerInfo_.tau_trigobj_.push_back(itrig);
    }
    triggerInfo_.triggerObj_filterBits_.push_back(gTrigger->triggerObj_filterBits_[itrig]);
    triggerInfo_.triggerObj_eta_.push_back(gTrigger->triggerObj_eta_[itrig]);
    triggerInfo_.triggerObj_phi_.push_back(gTrigger->triggerObj_phi_[itrig]);
   }
  return triggerInfo_;
}

std::vector<std::string>
TriggerInfoReader::get_available_branches(TTree * tree) const
{
  TObjArray * arr = tree->GetListOfBranches();
  TIter it(arr);
  TObject * obj = nullptr;
  std::vector<std::string> available_branches;
  while( (obj = it.Next()) )
  {
    available_branches.push_back(obj->GetName());
  }
  delete obj;
  return available_branches;
}

std::vector<std::string>
TriggerInfoReader::get_supported_systematics(const edm::ParameterSet & cfg)
{
  return {};
}
