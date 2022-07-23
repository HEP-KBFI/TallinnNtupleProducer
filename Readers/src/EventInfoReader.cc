#include "TallinnNtupleProducer/Readers/interface/EventInfoReader.h"

#include "TallinnNtupleProducer/CommonTools/interface/BranchAddressInitializer.h" // BranchAddressInitializer
#include "TallinnNtupleProducer/CommonTools/interface/get_htxs_binning.h"         // get_htxs_binning()
#include "TallinnNtupleProducer/Objects/interface/EventInfo.h"                    // EventInfo
#include "TallinnNtupleProducer/Objects/interface/RunLumiEvent.h"                 // RunLumiEvent
#include "TallinnNtupleProducer/CommonTools/interface/LocalFileInPath.h"
#include "TString.h"                                                              // Form()
#include "TTree.h"                                                                // TTree
#include "correction.h"

#include <assert.h>                                                               // assert()

EventInfoReader::EventInfoReader(const edm::ParameterSet & cfg)
  : ReaderBase(cfg)
  , analysisConfig_(new AnalysisConfig("produceNtuple", cfg))
  , info_(new EventInfo(*analysisConfig_))
  , runLumiEventReader_(new RunLumiEventReader(cfg))
  , era_(cfg.getParameter<std::string>("era"))
  , branchName_genWeight_("genWeight")
  , branchName_LHEReweightingWeight_("LHEReweightingWeight")
  , branchName_nLHEReweightingWeight_(Form("n%s", branchName_LHEReweightingWeight_.data()))
  , branchName_htxs_pt_("HTXS_Higgs_pt")
  , branchName_htxs_y_("HTXS_Higgs_y")
  , branchName_nof_pileup_("Pileup_nTrueInt")
  , branchName_rho_("fixedGridRhoFastjetAll")
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
    delete[] info_->LHEReweightingWeight_;
  }
  delete info_;
}

std::vector<std::string>
EventInfoReader::setBranchAddresses(TTree * inputTree)
{
  std::vector<std::string> bound_branches;

  const std::vector<std::string> runLumiEventBranches = runLumiEventReader_->setBranchAddresses(inputTree);
  bound_branches.insert(bound_branches.end(), runLumiEventBranches.begin(), runLumiEventBranches.end());

  BranchAddressInitializer bai(inputTree);
  if(info_->analysisConfig().isMC_H())
  {
    if(info_->read_htxs())
    {
      bai.setBranchAddress(info_->htxs_.pt_, branchName_htxs_pt_);
      bai.setBranchAddress(info_->htxs_.y_, branchName_htxs_y_);
    }
  }
  if(info_->analysisConfig().isMC())
  {
    bai.setBranchAddress(info_->genWeight_, branchName_genWeight_);
    bai.setBranchAddress(info_->nof_pileup_, branchName_nof_pileup_);
    bai.setBranchAddress(info_->rho_, branchName_rho_);
  }
  if(info_->analysisConfig().isMC() && ! info_->tH_sf_.empty())
  {
    BranchAddressInitializer bai_LHEReweight(inputTree, info_->LHEReweightingWeight_max_);
    bai_LHEReweight.setBranchAddress(info_->nLHEReweightingWeight_, branchName_nLHEReweightingWeight_);
    bai_LHEReweight.setBranchAddress(info_->LHEReweightingWeight_, branchName_LHEReweightingWeight_);

    const std::vector<std::string> lhe_branches = bai_LHEReweight.getBoundBranchNames_read();
    bound_branches.insert(bound_branches.end(), lhe_branches.begin(), lhe_branches.end());
  }

  const std::vector<std::string> eventInfoBranches = bai.getBoundBranchNames_read();
  bound_branches.insert(bound_branches.end(), eventInfoBranches.begin(), eventInfoBranches.end());

  return bound_branches;
}

void
EventInfoReader::set_central_or_shift(const std::string& central_or_shift)
{
  info_->set_central_or_shift(central_or_shift);
}

const EventInfo &
EventInfoReader::read() const
{
  RunLumiEvent runLumiEvent = runLumiEventReader_->read();
  info_->run_ = runLumiEvent.run();
  info_->lumi_ = runLumiEvent.lumi();
  info_->event_ = runLumiEvent.event();
  return *info_;
}

std::vector<std::string>
EventInfoReader::get_supported_systematics(const edm::ParameterSet & cfg)
{
  return {};
}
