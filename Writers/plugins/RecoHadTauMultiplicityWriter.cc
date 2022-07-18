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
  merge_systematic_shifts(supported_systematics_, RecoHadTauMultiplicityWriter::get_supported_systematics(cfg));
  merge_systematic_shifts(supported_systematics_, { "central" }); // CV: add central value
  for ( const std::string & central_or_shift : supported_systematics_ )
  {    
    central_or_shiftEntry it;
    it.nFakeableHadTausFull_ = 0;
    it.nTightHadTausFull_ = 0;
    central_or_shiftEntries_[central_or_shift] = it;
  }
  current_central_or_shiftEntry_ = &central_or_shiftEntries_["central"];
}

RecoHadTauMultiplicityWriter::~RecoHadTauMultiplicityWriter()
{}

namespace
{
  std::string
  get_branchName_num(const std::string & branchName_num, const std::string & central_or_shift)
  {
    if ( central_or_shift == "central" ) return branchName_num;
    else                                 return Form("%s_%s", branchName_num.data(), central_or_shift.data());
  }
}

void
RecoHadTauMultiplicityWriter::setBranches(TTree * outputTree)
{
  BranchAddressInitializer bai(outputTree);
  for ( const std::string & central_or_shift : supported_systematics_ )
  {
    auto it = central_or_shiftEntries_.find(central_or_shift);
    assert(it != central_or_shiftEntries_.end());
    bai.setBranch(it->second.nFakeableHadTausFull_, get_branchName_num(branchName_numFakeableHadTausFull_, central_or_shift));
    bai.setBranch(it->second.nTightHadTausFull_, get_branchName_num(branchName_numTightHadTausFull_, central_or_shift));
  }
}

void
RecoHadTauMultiplicityWriter::set_central_or_shift(const std::string & central_or_shift) const
{
  WriterBase::set_central_or_shift(central_or_shift);
  auto it = central_or_shiftEntries_.find(central_or_shift);
  if ( it != central_or_shiftEntries_.end() )
  {
    current_central_or_shiftEntry_ = const_cast<central_or_shiftEntry *>(&it->second);
  }
  else
  {
    current_central_or_shiftEntry_ = nullptr;
  }
}

void
RecoHadTauMultiplicityWriter::writeImp(const Event & event, const EvtWeightRecorder & evtWeightRecorder)
{
  if ( current_central_or_shiftEntry_ )
  {
    auto it = current_central_or_shiftEntry_;
    it->nFakeableHadTausFull_ = event.fakeableHadTausFull().size();
    it->nTightHadTausFull_ = event.tightHadTausFull().size();
  }
}

std::vector<std::string>
RecoHadTauMultiplicityWriter::get_supported_systematics(const edm::ParameterSet & cfg)
{
  return RecoHadTauReader::get_supported_systematics(cfg);
}

DEFINE_EDM_PLUGIN(WriterPluginFactory, RecoHadTauMultiplicityWriter, "RecoHadTauMultiplicityWriter"); // clazy:skip
