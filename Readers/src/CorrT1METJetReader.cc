#include "TallinnNtupleProducer/Readers/interface/CorrT1METJetReader.h"

#include "TallinnNtupleProducer/CommonTools/interface/BranchAddressInitializer.h" // BranchAddressInitializer
#include "TallinnNtupleProducer/CommonTools/interface/cmsException.h"             // cmsException()

#include "TString.h"                                                              // Form()
#include "TTree.h"                                                                // TTree

std::map<std::string, int> CorrT1METJetReader::numInstances_;
std::map<std::string, CorrT1METJetReader *> CorrT1METJetReader::instances_;

CorrT1METJetReader::CorrT1METJetReader(const edm::ParameterSet & cfg)
  : ReaderBase(cfg)
  , max_nJets_(36)
  , branchName_num_("")
  , branchName_obj_("")
  , jet_rawPt_(nullptr)
  , jet_eta_(nullptr)
  , jet_phi_(nullptr)
  , jet_area_(nullptr)
  , jet_muonSubtrFactor_(nullptr)
{
  if ( cfg.exists("max_nJets") )
  {
    max_nJets_ = cfg.getParameter<unsigned int>("max_nJets");
  }
  branchName_obj_ = cfg.getParameter<std::string>("branchName");
  branchName_num_ = Form("n%s", branchName_obj_.data());
  setBranchNames();
}

CorrT1METJetReader::~CorrT1METJetReader()
{
  --numInstances_[branchName_obj_];
  assert(numInstances_[branchName_obj_] >= 0);
  if(numInstances_[branchName_obj_] == 0)
  {
    CorrT1METJetReader * const gInstance = instances_[branchName_obj_];
    assert(gInstance);
    delete[] gInstance->jet_rawPt_;
    delete[] gInstance->jet_eta_;
    delete[] gInstance->jet_phi_;
    delete[] gInstance->jet_area_;
    delete[] gInstance->jet_muonSubtrFactor_;
    instances_[branchName_obj_] = nullptr;
  }
}

void
CorrT1METJetReader::setBranchNames()
{
  if(numInstances_[branchName_obj_] == 0)
  {
    branchName_rawPt_ = Form("%s_%s", branchName_obj_.data(), "rawPt");
    branchName_eta_ = Form("%s_%s", branchName_obj_.data(), "eta");
    branchName_phi_ = Form("%s_%s", branchName_obj_.data(), "phi");
    branchName_area_ = Form("%s_%s", branchName_obj_.data(), "area");
    branchName_muonSubtrFactor_ = Form("%s_%s", branchName_obj_.data(), "muonSubtrFactor");
    instances_[branchName_obj_] = this;
  }
  else
  {
    if(branchName_num_ != instances_[branchName_obj_]->branchName_num_)
    {
      throw cmsException(this)
        << "Association between configuration parameters 'branchName_num' and 'branchName_obj' must be unique:"
        << " present association 'branchName_num' = " << branchName_num_ << " with 'branchName_obj' = " << branchName_obj_
        << " does not match previous association 'branchName_num' = " << instances_[branchName_obj_]->branchName_num_
        << " with 'branchName_obj' = " << instances_[branchName_obj_]->branchName_obj_ << " !!\n";
    }
  }
  ++numInstances_[branchName_obj_];
}

std::vector<std::string>
CorrT1METJetReader::setBranchAddresses(TTree * tree)
{
  if(instances_[branchName_obj_] == this)
  {
    BranchAddressInitializer bai(tree, max_nJets_);
    bai.setBranchAddress(nJets_, branchName_num_);
    bai.setBranchAddress(jet_rawPt_, branchName_rawPt_);
    bai.setBranchAddress(jet_eta_, branchName_eta_);
    bai.setBranchAddress(jet_phi_, branchName_phi_);
    bai.setBranchAddress(jet_area_, branchName_area_);
    bai.setBranchAddress(jet_muonSubtrFactor_, branchName_muonSubtrFactor_);
    return bai.getBoundBranchNames_read();
  }
  return {};
}

std::vector<CorrT1METJet>
CorrT1METJetReader::read() const
{
  const CorrT1METJetReader * const gInstance = instances_[branchName_obj_];
  assert(gInstance);

  const UInt_t nJets = gInstance->nJets_;
  if(nJets > max_nJets_)
  {
    throw cmsException(this)
      << "Number of jets stored in Ntuple = " << nJets << ", exceeds max_nJets = " << max_nJets_ << " !!\n";
  }

  std::vector<CorrT1METJet> jets;
  if(nJets > 0)
  {
    jets.reserve(nJets);
    for(UInt_t idxJet = 0; idxJet < nJets; ++idxJet)
    {
      jets.push_back({
        gInstance->jet_rawPt_[idxJet],
        gInstance->jet_eta_[idxJet],
        gInstance->jet_phi_[idxJet],
        gInstance->jet_area_[idxJet],
        gInstance->jet_muonSubtrFactor_[idxJet],
      });
    }
  }
  return jets;
}
