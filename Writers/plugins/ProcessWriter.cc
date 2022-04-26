#include "TallinnNtupleProducer/Writers/plugins/ProcessWriter.h"

#include "TallinnNtupleProducer/Readers/interface/BranchAddressInitializer.h" // BranchAddressInitializer

#include "TTree.h"                                                            // TTree

ProcessWriter::ProcessWriter(const edm::ParameterSet & cfg)
  : WriterBase(cfg)
  , process_(cfg.getParameter<std::string>("process"))
{}

ProcessWriter::~ProcessWriter()
{}

void
ProcessWriter::setBranches(TTree * tree)
{
  BranchAddressInitializer bai(tree);
  bai.setBranch(process_, "process");
}

void
ProcessWriter::writeImp(const Event & event, const EvtWeightRecorder & evtWeightRecorder)
{}

DEFINE_EDM_PLUGIN(WriterPluginFactory, ProcessWriter, "ProcessWriter");
