#include "TallinnNtupleProducer/Writers/plugins/GenPhotonFilterWriter.h"

#include "TallinnNtupleProducer/CommonTools/interface/BranchAddressInitializer.h" // BranchAddressInitializer
#include "TallinnNtupleProducer/CommonTools/interface/cmsException.h"             // cmsException()
#include "TallinnNtupleProducer/CommonTools/interface/TTreeWrapper.h"             // TTreeWrapper
#include "TallinnNtupleProducer/Objects/interface/GenParticle.h"                  // GenParticle
#include "TallinnNtupleProducer/Readers/interface/GenParticleReader.h"            // GenParticleReader
#include "TallinnNtupleProducer/Writers/interface/GenPhotonFilter.h"              // GenPhotonFilter

#include "TString.h"                                                              // Form()
#include "TTree.h"                                                                // TTree

#include <assert.h>                                                               // assert()

GenPhotonFilterWriter::GenPhotonFilterWriter(const edm::ParameterSet & cfg)
  : WriterBase(cfg)
  , branchName_("passesGenPhotonFilter")
  , passesGenPhotonFilter_(false)
{
  std::string apply_genPhotonFilter_string = cfg.getParameter<std::string>("apply_genPhotonFilter");
  genPhotonFilter_ = new GenPhotonFilter(apply_genPhotonFilter_string);
  apply_genPhotonFilter_ = ( apply_genPhotonFilter_string != "disabled" );
}

GenPhotonFilterWriter::~GenPhotonFilterWriter()
{
  delete genPhotonFilter_;
}

void
GenPhotonFilterWriter::setBranches(TTree * outputTree)
{
  BranchAddressInitializer bai(outputTree);
  bai.setBranch(passesGenPhotonFilter_, branchName_);
}

void
GenPhotonFilterWriter::writeImp(const Event & event,
                                const EvtWeightRecorder & evtWeightRecorder)
{
  assert(genPhotonFilter_);
  passesGenPhotonFilter_ = (*genPhotonFilter_)(event.genParticles());
}

std::vector<std::string>
GenPhotonFilterWriter::get_supported_systematics(const edm::ParameterSet & cfg)
{
  return std::vector<std::string>();
}

DEFINE_EDM_PLUGIN(WriterPluginFactory, GenPhotonFilterWriter, "GenPhotonFilterWriter"); // clazy:skip
