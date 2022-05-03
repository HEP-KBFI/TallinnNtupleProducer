#include "TallinnNtupleProducer/Writers/plugins/RunLumiEventWriter.h"

#include "TallinnNtupleProducer/CommonTools/interface/BranchAddressInitializer.h" // BranchAddressInitializer

#include "TTree.h"                                                                // TTree

RunLumiEventWriter::RunLumiEventWriter(const edm::ParameterSet & cfg)
  : WriterBase(cfg)
{}

RunLumiEventWriter::~RunLumiEventWriter()
{}

void
RunLumiEventWriter::setBranches(TTree * outputTree)
{
  BranchAddressInitializer bai(outputTree);
  bai.setBranch(run_,   "run");
  bai.setBranch(lumi_,  "lumi");
  bai.setBranch(event_, "event");  
}

void
RunLumiEventWriter::writeImp(const Event & event, const EvtWeightRecorder & evtWeightRecorder)
{
  run_   = event.eventInfo().run;
  lumi_  = event.eventInfo().lumi;
  event_ = event.eventInfo().event;
}

DEFINE_EDM_PLUGIN(WriterPluginFactory, RunLumiEventWriter, "RunLumiEventWriter");
