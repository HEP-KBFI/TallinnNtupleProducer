#include "TallinnNtupleProducer/Writers/plugins/GenHHKinematicsWriter.h"

#include "TallinnNtupleProducer/CommonTools/interface/BranchAddressInitializer.h" // BranchAddressInitializer

#include "TTree.h"                                                                // TTree

GenHHKinematicsWriter::GenHHKinematicsWriter(const edm::ParameterSet & cfg)
  : WriterBase(cfg)
  , gen_mHH_(0.)
  , gen_cosThetaStar_(0.)
{}

GenHHKinematicsWriter::~GenHHKinematicsWriter()
{}

void
GenHHKinematicsWriter::setBranches(TTree * outputTree)
{
  BranchAddressInitializer bai(outputTree);
  bai.setBranch(gen_mHH_, "gen_mHH");
  bai.setBranch(gen_cosThetaStar_, "gen_cosThetaStar");
}

void
GenHHKinematicsWriter::writeImp(const Event & event, const EvtWeightRecorder & evtWeightRecorder)
{
  const EventInfo& eventInfo = event.eventInfo();
  const AnalysisConfig& analysisConfig = eventInfo.analysisConfig();
  if ( analysisConfig.isMC_HH() )
  {
    gen_mHH_ = eventInfo.gen_mHH();
    gen_cosThetaStar_ = eventInfo.gen_cosThetaStar();
  }
  else
  {
    gen_mHH_ = 0.;
    gen_cosThetaStar_ = 0.;
  }
}

std::vector<std::string>
GenHHKinematicsWriter::get_supported_systematics(const edm::ParameterSet & cfg)
{
  return std::vector<std::string>();
}

DEFINE_EDM_PLUGIN(WriterPluginFactory, GenHHKinematicsWriter, "GenHHKinematicsWriter");
