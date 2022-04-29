#include "TallinnNtupleProducer/Readers/interface/EventInfoReader.h"

#include "TallinnNtupleProducer/CommonTools/interface/get_htxs_binning.h"     // get_htxs_binning()
#include "TallinnNtupleProducer/CommonTools/interface/sysUncertOptions.h"     // getBranchName_pileup()
#include "TallinnNtupleProducer/Objects/interface/EventInfo.h"                // EventInfo
#include "TallinnNtupleProducer/Readers/interface/BranchAddressInitializer.h" // BranchAddressInitializer

#include "TString.h"                                                          // Form()
#include "TTree.h"                                                            // TTree

#include <assert.h>                                                           // assert()

EventInfoReader::EventInfoReader(const edm::ParameterSet & cfg)
  : ReaderBase(cfg)
  , read_genHiggsDecayMode_(true)
  , read_puWeight_(true)
  , analysisConfig_(new AnalysisConfig("produceNtuple", cfg))
  , info_(new EventInfo(*analysisConfig_))
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
  const bool isMC = cfg.getParameter<bool>("isMC");
  if ( isMC )
  {
    const double ref_genWeight = cfg.getParameter<double>("ref_genWeight");
    info_->set_refGetWeight(ref_genWeight);
  }
  const bool isMC_ttH = analysisConfig_->isMC_ttH();
  const std::vector<std::pair<std::string, int>> evt_htxs_binning = get_htxs_binning(isMC_ttH);
  info_->read_htxs(!evt_htxs_binning.empty());
}

EventInfoReader::~EventInfoReader()
{
  if(info_ && info_->analysisConfig().isMC())
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
  if(info_->analysisConfig().isMC_H())
  {
    if(info_->read_htxs())
    {
      bai.setBranchAddress(info_->htxs_.pt_, branchName_htxs_pt);
      bai.setBranchAddress(info_->htxs_.y_, branchName_htxs_y);
    }
  }
  if(info_->analysisConfig().isMC_H() || info_->analysisConfig().isMC_HH())
  {
    if(read_genHiggsDecayMode_)
    {
      bai.setBranchAddress(info_->genHiggsDecayMode, branchName_genHiggsDecayMode);
    }
  }
  if(info_->analysisConfig().isMC())
  {
    bai.setBranchAddress(info_->genWeight, branchName_genWeight);
    if(read_puWeight_)
    {
      bai.setBranchAddress(info_->pileupWeight, getBranchName_pileup(PUsys::central));
      bai.setBranchAddress(info_->pileupWeightUp, getBranchName_pileup(PUsys::up));
      bai.setBranchAddress(info_->pileupWeightDown, getBranchName_pileup(PUsys::down));
    }
    if(info_->analysisConfig().apply_topPtReweighting())
    {
      bai.setBranchAddress(info_->topPtRwgtSF, branchName_topPtRwgt);
    }
  }
  if(info_->analysisConfig().isMC() && ! info_->tH_sf.empty())
  {
    BranchAddressInitializer bai_LHEReweight(tree, info_->LHEReweightingWeight_max);
    bai_LHEReweight.setBranchAddress(info_->nLHEReweightingWeight, branchName_nLHEReweightingWeight);
    bai_LHEReweight.setBranchAddress(info_->LHEReweightingWeight, branchName_LHEReweightingWeight);

    const std::vector<std::string> lhe_branches = bai_LHEReweight.getBoundBranchNames();
    bound_branches.insert(bound_branches.end(), lhe_branches.begin(), lhe_branches.end());
  }
  // Siddhesh: Read gen_mHH for HH LO and NLO samples to make LOvsNLO HHreweighting comparison plots without much code editing
  if(info_->analysisConfig().isMC_HH() && (info_->analysisConfig().isHH_rwgt_allowed() || info_->analysisConfig().isMC_HH_nonresonant()))
  {
    bai.setBranchAddress(info_->gen_mHH, branchName_gen_mHH);
    bai.setBranchAddress(info_->gen_cosThetaStar, branchName_gen_cosThetaStar);
  }
  const std::vector<std::string> evt_branches = bai.getBoundBranchNames();
  bound_branches.insert(bound_branches.end(), evt_branches.begin(), evt_branches.end());
  return bound_branches;
}

void
EventInfoReader::set_central_or_shift(const std::string& central_or_shift)
{
  info_->set_central_or_shift(central_or_shift);
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
