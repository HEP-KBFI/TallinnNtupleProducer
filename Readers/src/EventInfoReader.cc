#include "TallinnNtupleProducer/Readers/interface/EventInfoReader.h"

#include "TallinnNtupleProducer/CommonTools/interface/AnalysisConfig.h"       // AnalysisConfig
#include "TallinnNtupleProducer/CommonTools/interface/sysUncertOptions.h"     // getBranchName_pileup()
#include "TallinnNtupleProducer/Objects/interface/EventInfo.h"                // EventInfo
#include "TallinnNtupleProducer/Readers/interface/BranchAddressInitializer.h" // BranchAddressInitializer

#include "TString.h"                                                          // Form()
#include "TTree.h"                                                            // TTree

EventInfoReader::EventInfoReader(const edm::ParameterSet & cfg)
  : ReaderBase(cfg)
  , read_genHiggsDecayMode_(true)
  , read_puWeight_(true)
  , info_(nullptr)
  , branchName_run("run")
  , branchName_lumi("luminosityBlock")
  , branchName_event("event")
  , branchName_genHiggsDecayMode("genHiggsDecayMode")
  , branchName_genWeight("genWeight")
  , branchName_LHEReweightingWeight("LHEReweightingWeight")
  , branchName_nLHEReweightingWeight(Form("n%s", branchName_LHEReweightingWeight.data()))
  , branchName_gen_mHH("mHH_lhe")
  , branchName_gen_cosThetaStar("cosThetaStar_lhe")
  , branchName_htxs_pt("HTXS_Higgs_pt")
  , branchName_htxs_y("HTXS_Higgs_y")
{
  std::string analysis = "produceNtuple";
  AnalysisConfig analysisConfig(analysis, cfg);
  info_ = new EventInfo(analysisConfig);
}

EventInfoReader::~EventInfoReader()
{
  if(info_ && info_->isMC_)
  {
    delete[] info_->LHEReweightingWeight;
  }
  delete info_;
}

std::vector<std::string>
EventInfoReader::setBranchAddresses(TTree * tree)
{
  std::vector<std::string> bound_branches;
  BranchAddressInitializer bai(tree);
  bai.setBranchAddress(info_->run, branchName_run);
  bai.setBranchAddress(info_->lumi, branchName_lumi);
  bai.setBranchAddress(info_->event, branchName_event);
  if(info_->isMC_H_)
  {
    if(info_->read_htxs())
    {
      bai.setBranchAddress(info_->htxs_.pt_, branchName_htxs_pt);
      bai.setBranchAddress(info_->htxs_.y_, branchName_htxs_y);
    }
  }
  if(info_->isMC_H_ || info_->isMC_HH_)
  {
    if(read_genHiggsDecayMode_)
    {
      bai.setBranchAddress(info_->genHiggsDecayMode, branchName_genHiggsDecayMode);
    }
  }
  if(info_->isMC_)
  {
    bai.setBranchAddress(info_->genWeight, branchName_genWeight);
    if(read_puWeight_)
    {
      bai.setBranchAddress(info_->pileupWeight, getBranchName_pileup(PUsys::central));
      bai.setBranchAddress(info_->pileupWeightUp, getBranchName_pileup(PUsys::up));
      bai.setBranchAddress(info_->pileupWeightDown, getBranchName_pileup(PUsys::down));
    }
    if(info_->apply_topPtRwgt_)
    {
      bai.setBranchAddress(info_->topPtRwgtSF, branchName_topPtRwgt);
    }
  }
  if(info_->isMC_ && ! info_->tH_sf.empty())
  {
    BranchAddressInitializer bai_LHEReweight(tree, info_->LHEReweightingWeight_max);
    bai_LHEReweight.setBranchAddress(info_->nLHEReweightingWeight, branchName_nLHEReweightingWeight);
    bai_LHEReweight.setBranchAddress(info_->LHEReweightingWeight, branchName_LHEReweightingWeight);

    const std::vector<std::string> lhe_branches = bai_LHEReweight.getBoundBranchNames();
    bound_branches.insert(bound_branches.end(), lhe_branches.begin(), lhe_branches.end());
  }
  //if(info_->isMC_HH_  && info_->isHH_rwgt_allowed_)
  if(info_->isMC_HH_  && (info_->isHH_rwgt_allowed_ ||
			    info_->process_string_.find("signal_ggf_nonresonant") != std::string::npos) ) // Siddhesh: Read gen_mHH for HH LO and NLO samples to make LOvsNLO HHreweighting comparison plots without much editing
  {
    bai.setBranchAddress(info_->gen_mHH, branchName_gen_mHH);
    bai.setBranchAddress(info_->gen_cosThetaStar, branchName_gen_cosThetaStar);
  }
  const std::vector<std::string> evt_branches = bai.getBoundBranchNames();
  bound_branches.insert(bound_branches.end(), evt_branches.begin(), evt_branches.end());
  return bound_branches;
}

void
EventInfoReader::setTopPtRwgtBranchName(const std::string & branchName)
{
  branchName_topPtRwgt = Form("topPtRwgt_%s", branchName.data());
}

std::string
EventInfoReader::getTopPtRwgtBranchName() const
{
  return branchName_topPtRwgt;
}

const EventInfo &
EventInfoReader::read() const
{
  return *info_;
}
