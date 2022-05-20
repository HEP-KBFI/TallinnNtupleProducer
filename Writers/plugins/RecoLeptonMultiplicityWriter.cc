#include "TallinnNtupleProducer/Writers/plugins/RecoLeptonMultiplicityWriter.h"

#include "TallinnNtupleProducer/CommonTools/interface/BranchAddressInitializer.h" // BranchAddressInitializer
#include "TallinnNtupleProducer/CommonTools/interface/cmsException.h"             // cmsException()
#include "TallinnNtupleProducer/CommonTools/interface/merge_systematic_shifts.h"  // merge_systematic_shifts()
#include "TallinnNtupleProducer/Readers/interface/RecoElectronReader.h"           // RecoElectronReader::get_supported_systematics()
#include "TallinnNtupleProducer/Readers/interface/RecoMuonReader.h"               // RecoMuonReader::get_supported_systematics()

#include "TString.h"                                                              // Form()
#include "TTree.h"                                                                // TTree

#include <assert.h>                                                               // assert()
#include <cmath>                                                                  // std::abs()

RecoLeptonMultiplicityWriter::RecoLeptonMultiplicityWriter(const edm::ParameterSet & cfg)
  : WriterBase(cfg)
  , branchName_numLooseLeptonsFull_("nlooselep")
  , branchName_numFakeableLeptonsFull_("nfakeablelep")
  , branchName_numTightLeptonsFull_("ntightlep")
{
  nLooseLeptonsFull_ = 0;
  nFakeableLeptonsFull_ = 0;
  nTightLeptonsFull_ = 0;
}

RecoLeptonMultiplicityWriter::~RecoLeptonMultiplicityWriter()
{}

void
RecoLeptonMultiplicityWriter::setBranches(TTree * outputTree)
{
  BranchAddressInitializer bai(outputTree);
  bai.setBranch(nLooseLeptonsFull_, branchName_numLooseLeptonsFull_);
  bai.setBranch(nFakeableLeptonsFull_, branchName_numFakeableLeptonsFull_);
  bai.setBranch(nTightLeptonsFull_, branchName_numTightLeptonsFull_);
}

void
RecoLeptonMultiplicityWriter::writeImp(const Event & event, const EvtWeightRecorder & evtWeightRecorder)
{
  nLooseLeptonsFull_ = event.looseLeptonsFull_.size();
  nFakeableLeptonsFull_ = event.fakeableLeptonsFull_.size();
  nTightLeptonsFull_ = event.tightLeptonsFull_.size();
}

std::vector<std::string>
RecoLeptonMultiplicityWriter::get_supported_systematics(const edm::ParameterSet & cfg)
{
  std::vector<std::string> supported_systematics;
  merge_systematic_shifts(supported_systematics, RecoElectronReader::get_supported_systematics(cfg));
  merge_systematic_shifts(supported_systematics, RecoMuonReader::get_supported_systematics(cfg));
  return supported_systematics;
}

DEFINE_EDM_PLUGIN(WriterPluginFactory, RecoLeptonMultiplicityWriter, "RecoLeptonMultiplicityWriter");
