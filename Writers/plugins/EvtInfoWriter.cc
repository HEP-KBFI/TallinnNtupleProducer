#include "TallinnNtupleProducer/Writers/plugins/EvtInfoWriter.h"

#include "TallinnNtupleProducer/CommonTools/interface/BranchAddressInitializer.h" // BranchAddressInitializer
#include "TallinnNtupleProducer/Objects/interface/genTools.h"                     // getHiggsDecayMode()

#include "TTree.h"                                                                // TTree

EvtInfoWriter::EvtInfoWriter(const edm::ParameterSet & cfg)
  : WriterBase(cfg)
  , branchName_genHiggsDecayMode_("genHiggsDecayMode")
  , genHiggsDecayMode_(-1)
{}

EvtInfoWriter::~EvtInfoWriter()
{}

void
EvtInfoWriter::setBranches(TTree * outputTree)
{
  BranchAddressInitializer bai(outputTree);
  bai.setBranch(genHiggsDecayMode_, branchName_genHiggsDecayMode_);
}

void
EvtInfoWriter::writeImp(const Event & event,
                        const EvtWeightRecorder & evtWeightRecorder)
{
  genHiggsDecayMode_ = getHiggsDecayMode(event.genParticles());
}

std::vector<std::string>
EvtInfoWriter::get_supported_systematics(const edm::ParameterSet & cfg)
{
  return std::vector<std::string>();
}

DEFINE_EDM_PLUGIN(WriterPluginFactory, EvtInfoWriter, "EvtInfoWriter"); // clazy:skip
