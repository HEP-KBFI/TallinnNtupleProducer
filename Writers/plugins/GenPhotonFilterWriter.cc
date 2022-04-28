#include "TallinnNtupleProducer/Writers/plugins/GenPhotonFilterWriter.h"

#include "TallinnNtupleProducer/CommonTools/interface/cmsException.h"         // cmsException()
#include "TallinnNtupleProducer/CommonTools/interface/TTreeWrapper.h"         // TTreeWrapper
#include "TallinnNtupleProducer/Objects/interface/GenParticle.h"              // GenParticle
#include "TallinnNtupleProducer/Objects/interface/GenPhoton.h"                // GenPhoton
#include "TallinnNtupleProducer/Readers/interface/BranchAddressInitializer.h" // BranchAddressInitializer
#include "TallinnNtupleProducer/Readers/interface/GenParticleReader.h"        // GenParticleReader
#include "TallinnNtupleProducer/Readers/interface/GenPhotonReader.h"          // GenPhotonReader
#include "TallinnNtupleProducer/Writers/interface/GenPhotonFilter.h"          // GenPhotonFilter

#include "TString.h"                                                          // Form()
#include "TTree.h"                                                            // TTree

#include <assert.h>                                                           // assert()

namespace
{
  edm::ParameterSet
  make_cfg(const std::string & branchName)
  {
    edm::ParameterSet cfg;
    cfg.addParameter<std::string>("branchName", branchName);
    return cfg;
  }
}

GenPhotonFilterWriter::GenPhotonFilterWriter(const edm::ParameterSet & cfg)
  : WriterBase(cfg)
  , branchName_("passesGenPhotonFilter")
  , genPhotonReader_(nullptr)
  , genProxyPhotonReader_(nullptr)
  , genParticlesFromHardProcessReader_(nullptr)
  , genPhotonFilter_(nullptr)
  , passesGenPhotonFilter_(false)
{
  branchName_genPhotons_ = cfg.getParameter<std::string>("branchName_genPhotons");
  branchName_genProxyPhotons_ = cfg.getParameter<std::string>("branchName_genProxyPhotons");
  branchName_genParticlesFromHardProcess_ = cfg.getParameter<std::string>("branchName_genParticlesFromHardProcess");

  genPhotonReader_ = new GenPhotonReader(make_cfg(branchName_genPhotons_));
  genProxyPhotonReader_ = new GenPhotonReader(make_cfg(branchName_genProxyPhotons_));
  genParticlesFromHardProcessReader_ = new GenParticleReader(make_cfg(branchName_genParticlesFromHardProcess_));

  std::string apply_genPhotonFilter_string = cfg.getParameter<std::string>("apply_genPhotonFilter");
  genPhotonFilter_ = new GenPhotonFilter(apply_genPhotonFilter_string);
  apply_genPhotonFilter_ = ( apply_genPhotonFilter_string != "disabled" );
}

GenPhotonFilterWriter::~GenPhotonFilterWriter()
{
  delete genPhotonReader_;
  delete genProxyPhotonReader_;
  delete genParticlesFromHardProcessReader_;
  delete genPhotonFilter_;
}

void
GenPhotonFilterWriter::registerReaders(TTreeWrapper * inputTree)
{
std::cout << "<GenPhotonFilterWriter::registerReaders>:" << std::endl;
  assert(inputTree);
  inputTree->registerReader(genPhotonReader_);
  inputTree->registerReader(genProxyPhotonReader_);
  inputTree->registerReader(genParticlesFromHardProcessReader_);
}

void
GenPhotonFilterWriter::setBranches(TTree * outputTree)
{
std::cout << "<GenPhotonFilterWriter::setBranches>:" << std::endl;
  BranchAddressInitializer bai(outputTree);
  bai.setBranch(passesGenPhotonFilter_, branchName_);
}

void
GenPhotonFilterWriter::writeImp(const Event & event, const EvtWeightRecorder & evtWeightRecorder)
{
  std::vector<GenPhoton> genPhotons = genPhotonReader_->read(apply_genPhotonFilter_);
  std::vector<GenPhoton> genProxyPhotons = genProxyPhotonReader_->read(apply_genPhotonFilter_);
  std::vector<GenParticle> genParticlesFromHardProcess = genParticlesFromHardProcessReader_->read();

  assert(genPhotonFilter_);
  passesGenPhotonFilter_ = (*genPhotonFilter_)(genPhotons, genProxyPhotons, genParticlesFromHardProcess);
}

DEFINE_EDM_PLUGIN(WriterPluginFactory, GenPhotonFilterWriter, "GenPhotonFilterWriter");



 
