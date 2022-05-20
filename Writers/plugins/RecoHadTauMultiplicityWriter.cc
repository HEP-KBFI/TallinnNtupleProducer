#include "TallinnNtupleProducer/Writers/plugins/RecoHadTauMultiplicityWriter.h"

#include "TallinnNtupleProducer/CommonTools/interface/BranchAddressInitializer.h" // BranchAddressInitializer
#include "TallinnNtupleProducer/CommonTools/interface/cmsException.h"             // cmsException()
#include "TallinnNtupleProducer/CommonTools/interface/merge_systematic_shifts.h"  // merge_systematic_shifts()
#include "TallinnNtupleProducer/Readers/interface/RecoHadTauReader.h"             // RecoHadTauReader::get_supported_systematics()

#include "TString.h"                                                              // Form()
#include "TTree.h"                                                                // TTree

#include <assert.h>                                                               // assert()
#include <cmath>                                                                  // std::abs()

RecoHadTauMultiplicityWriter::RecoHadTauMultiplicityWriter(const edm::ParameterSet & cfg)
  : WriterBase(cfg)
  , branchName_numFakeableHadTausFull_("nfakeabletau")
  , branchName_numTightHadTausFull_("ntighttau")
{
  nFakeableHadTausFull_ = 0;
  nTightHadTausFull_ = 0;
}

RecoHadTauMultiplicityWriter::~RecoHadTauMultiplicityWriter()
{}

void
RecoHadTauMultiplicityWriter::setBranches(TTree * outputTree)
{
  BranchAddressInitializer bai(outputTree);
  bai.setBranch(nFakeableHadTausFull_, branchName_numFakeableHadTausFull_);
  bai.setBranch(nTightHadTausFull_, branchName_numTightHadTausFull_);
}

void
RecoHadTauMultiplicityWriter::writeImp(const Event & event, const EvtWeightRecorder & evtWeightRecorder)
{
  nFakeableHadTausFull_ = event.fakeableHadTausFull_.size();
  nTightHadTausFull_ = event.tightHadTausFull_.size();
}

std::vector<std::string>
RecoHadTauMultiplicityWriter::get_supported_systematics(const edm::ParameterSet & cfg)
{
  return RecoHadTauReader::get_supported_systematics(cfg);
}

DEFINE_EDM_PLUGIN(WriterPluginFactory, RecoHadTauMultiplicityWriter, "RecoHadTauMultiplicityWriter");
