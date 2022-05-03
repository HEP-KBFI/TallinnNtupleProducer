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
{
  bool isMC = cfg.getParameter<bool>("isMC");
  metFilterSelector_ = new MEtFilterSelector(cfg, isMC);
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

DEFINE_EDM_PLUGIN(WriterPluginFactory, MEtFilterWriter, "MEtFilterWriter");
