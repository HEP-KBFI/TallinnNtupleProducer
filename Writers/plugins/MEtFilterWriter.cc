#include "TallinnNtupleProducer/Writers/plugins/MEtFilterWriter.h"

#include "TallinnNtupleProducer/CommonTools/interface/BranchAddressInitializer.h" // BranchAddressInitializer
#include "TallinnNtupleProducer/CommonTools/interface/cmsException.h"             // cmsException()
#include "TallinnNtupleProducer/Writers/interface/MEtFilterSelector.h"            // MEtFilterSelector

#include "TString.h"                                                              // Form()
#include "TTree.h"                                                                // TTree

#include <assert.h>                                                               // assert()

MEtFilterWriter::MEtFilterWriter(const edm::ParameterSet & cfg)
  : WriterBase(cfg)
  , branchName_("passesMEtFilters")
  , passesMEtFilters_(false)
{
  edm::ParameterSet cfg_flags = cfg.getParameterSet("flags");
  bool isMC = cfg.getParameter<bool>("isMC");
  metFilterSelector_ = new MEtFilterSelector(cfg_flags, isMC);
}

MEtFilterWriter::~MEtFilterWriter()
{
  delete metFilterSelector_;
}

void
MEtFilterWriter::setBranches(TTree * outputTree)
{
  BranchAddressInitializer bai(outputTree);
  bai.setBranch(passesMEtFilters_, branchName_);
}

void
MEtFilterWriter::writeImp(const Event & event, const EvtWeightRecorder & evtWeightRecorder)
{
  assert(metFilterSelector_);
  passesMEtFilters_ = (*metFilterSelector_)(event.metFilters());
}

std::vector<std::string>
MEtFilterWriter::get_supported_systematics(const edm::ParameterSet & cfg)
{
  return std::vector<std::string>();
}

DEFINE_EDM_PLUGIN(WriterPluginFactory, MEtFilterWriter, "MEtFilterWriter");
