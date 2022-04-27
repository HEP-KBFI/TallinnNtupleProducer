#include "TallinnNtupleProducer/Writers/plugins/EvtWeightWriter.h"

#include "TallinnNtupleProducer/CommonTools/interface/cmsException.h"            // cmsException()
#include "TallinnNtupleProducer/CommonTools/interface/merge_systematic_shifts.h" // merge_systematic_shifts.()
#include "TallinnNtupleProducer/Readers/interface/BranchAddressInitializer.h"    // BranchAddressInitializer

#include "TString.h"                                                             // Form()
#include "TTree.h"                                                               // TTree

#include <assert.h>                                                              // assert()
#include <cstdlib>                                                               // std::abs()

EvtWeightWriter::EvtWeightWriter(const edm::ParameterSet & cfg)
  : WriterBase(cfg)
{
  merge_systematic_shifts(supported_systematics_, EvtWeightWriter::get_supported_systematics());
  merge_systematic_shifts(supported_systematics_, { "central" }); // CV: add central value
  for ( auto central_or_shift : supported_systematics_ )
  {    
    central_or_shiftEntry it;
    it.evtWeight_ = 0.;
    central_or_shiftEntries_[central_or_shift] = it;
  }
  current_central_or_shiftEntry_ = &central_or_shiftEntries_["central"];
}

EvtWeightWriter::~EvtWeightWriter()
{}

namespace
{
  std::string
  get_branchName(const std::string & branchName, const std::string & central_or_shift)
  {
    if ( central_or_shift == "central" ) return Form("%s",    branchName.data());
    else                                 return Form("%s_%s", branchName.data(), central_or_shift.data());
  }
}

void
EvtWeightWriter::setBranches(TTree * outputTree)
{
  BranchAddressInitializer bai(outputTree);
  for ( auto central_or_shift : supported_systematics_ )
  {
    auto it = central_or_shiftEntries_.find(central_or_shift);
    assert(it != central_or_shiftEntries_.end());
    bai.setBranch(it->second.evtWeight_, get_branchName("evtWeight", central_or_shift));
  }
}

void
EvtWeightWriter::set_central_or_shift(const std::string & central_or_shift) const
{
  WriterBase::set_central_or_shift(central_or_shift);
  auto it = central_or_shiftEntries_.find(central_or_shift);
  if ( it != central_or_shiftEntries_.end() )
  {
    current_central_or_shiftEntry_ = const_cast<central_or_shiftEntry *>(&it->second);
  }
  else throw cmsException(__func__, __LINE__) 
    << "Invalid systematic shift = '" << central_or_shift << "' !!";
}

void
EvtWeightWriter::writeImp(const Event & event, const EvtWeightRecorder & evtWeightRecorder)
{
  assert(current_central_or_shiftEntry_);
  current_central_or_shiftEntry_->evtWeight_ = evtWeightRecorder.get(current_central_or_shift_);
}

std::vector<std::string>
EvtWeightWriter::get_supported_systematics()
{
  return std::vector<std::string>(); // CV: to be implemented !!
}

DEFINE_EDM_PLUGIN(WriterPluginFactory, EvtWeightWriter, "EvtWeightWriter");
