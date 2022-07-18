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
}

void
ProcessWriter::writeImp(const Event & event, const EvtWeightRecorder & evtWeightRecorder)
{}

std::vector<std::string>
ProcessWriter::get_supported_systematics(const edm::ParameterSet & cfg)
{
  return std::vector<std::string>();
}

DEFINE_EDM_PLUGIN(WriterPluginFactory, ProcessWriter, "ProcessWriter"); // clazy:skip
