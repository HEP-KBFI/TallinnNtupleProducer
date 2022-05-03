#include "TallinnNtupleProducer/Writers/plugins/ProcessWriter.h"

#include "TallinnNtupleProducer/CommonTools/interface/BranchAddressInitializer.h" // BranchAddressInitializer

#include "TTree.h"                                                                // TTree

ProcessWriter::ProcessWriter(const edm::ParameterSet & cfg)
  : WriterBase(cfg)
  , process_(cfg.getParameter<std::string>("process"))
{}

ProcessWriter::~ProcessWriter()
{}

void
ProcessWriter::setBranches(TTree * outputTree)
{
  BranchAddressInitializer bai(outputTree);
  bai.setBranch(process_, "process");
  //outputTree->Branch("process", &process_);
}

void
ProcessWriter::writeImp(const Event & event, const EvtWeightRecorder & evtWeightRecorder)
{}

DEFINE_EDM_PLUGIN(WriterPluginFactory, ProcessWriter, "ProcessWriter");
