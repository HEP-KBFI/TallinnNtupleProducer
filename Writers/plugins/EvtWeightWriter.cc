#include "TallinnNtupleProducer/Writers/plugins/EvtWeightWriter.h"

#include "TallinnNtupleProducer/CommonTools/interface/BranchAddressInitializer.h" // BranchAddressInitializer
#include "TallinnNtupleProducer/CommonTools/interface/cmsException.h"             // cmsException()
#include "TallinnNtupleProducer/CommonTools/interface/merge_systematic_shifts.h"  // merge_systematic_shifts()
#include "TallinnNtupleProducer/CommonTools/interface/map_keys.h"                 // map_keys()
#include "TallinnNtupleProducer/CommonTools/interface/sysUncertOptions.h"         // *SysMap

#include "TString.h"                                                              // Form()
#include "TTree.h"                                                                // TTree

#include <assert.h>                                                               // assert()
#include <cstdlib>                                                                // std::abs()

EvtWeightWriter::EvtWeightWriter(const edm::ParameterSet & cfg)
  : WriterBase(cfg)
{
  merge_systematic_shifts(supported_systematics_, EvtWeightWriter::get_supported_systematics(cfg));
  for ( const std::string & central_or_shift : supported_systematics_ )
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
  for ( const std::string & central_or_shift : supported_systematics_ )
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
  else
  {
    current_central_or_shiftEntry_ = nullptr;
  }
}

void
EvtWeightWriter::writeImp(const Event & event, const EvtWeightRecorder & evtWeightRecorder)
{
  if ( current_central_or_shiftEntry_ )
  {
    current_central_or_shiftEntry_->evtWeight_ = evtWeightRecorder.get(current_central_or_shift_);
  }
}

std::vector<std::string>
EvtWeightWriter::get_supported_systematics(const edm::ParameterSet & cfg)
{
  // TODO use AnalysisConfig instead of ParameterSet?
  static std::vector<std::string> supported_systematics;
  if(supported_systematics.empty())
  {
    merge_systematic_shifts(supported_systematics, { "central" }); // CV: add central value
    if(cfg.getParameter<bool>("apply_topPtReweighting"))
    {
      merge_systematic_shifts(supported_systematics, map_keys(topPtRwgtSysMap));
    }
    if(cfg.getParameter<bool>("has_LHE_weights"))
    {
      merge_systematic_shifts(supported_systematics, map_keys(lheScaleSysMap));
    }
  }
  return supported_systematics;
}

DEFINE_EDM_PLUGIN(WriterPluginFactory, EvtWeightWriter, "EvtWeightWriter");
