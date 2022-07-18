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
    if(cfg.getParameter<bool>("isMC"))
    {
      merge_systematic_shifts(supported_systematics, map_keys(puSysMap));
      merge_systematic_shifts(supported_systematics, map_keys(btagWeightSysMap));
      if(cfg.getParameter<bool>("apply_pileupJetID"))
      {
        merge_systematic_shifts(supported_systematics, map_keys(pileupJetIDSysMap));
      }
      if(cfg.getParameter<bool>("l1PreFiringWeightReader"))
      {
        merge_systematic_shifts(supported_systematics, map_keys(l1prefireSysMap));
      }
      if(cfg.getParameter<bool>("apply_topPtReweighting"))
      {
        merge_systematic_shifts(supported_systematics, map_keys(topPtRwgtSysMap));
      }
      if(cfg.getParameter<bool>("has_LHE_weights"))
      {
        merge_systematic_shifts(supported_systematics, map_keys(lheScaleSysMap));
      }
      if(cfg.getParameter<bool>("has_PS_weights"))
      {
        merge_systematic_shifts(supported_systematics, map_keys(psSysMap));
      }
      if(cfg.getParameter<bool>("has_PDF_weights"))
      {
        merge_systematic_shifts(supported_systematics, map_keys(pdfSysMap));
        const int nof_pdf_members = cfg.getParameter<int>("nof_PDF_members");
        std::vector<std::string> pdf_member_sys;
        for(int pdf_member_idx = 0; pdf_member_idx < nof_pdf_members; ++pdf_member_idx)
        {
          pdf_member_sys.push_back(getPDFsys_str(pdf_member_idx));
        }
        merge_systematic_shifts(supported_systematics, pdf_member_sys);
      }
      if(cfg.getParameter<unsigned>("numNominalLeptons") > 0)
      {
        merge_systematic_shifts(supported_systematics, map_keys(leptonIDSFSysMap));
        merge_systematic_shifts(supported_systematics, map_keys(jetToLeptonFRSysMap));
      }
      if(cfg.getParameter<unsigned>("numNominalHadTaus") > 0)
      {
        merge_systematic_shifts(supported_systematics, map_keys(jetToTauFRSysMap));
        merge_systematic_shifts(supported_systematics, map_keys(eToTauFRSysMap));
        merge_systematic_shifts(supported_systematics, map_keys(mToTauFRSysMap));
        merge_systematic_shifts(supported_systematics, map_keys(tauIDSFSysMap));
      }
      //TODO: trigger SF, b-tagging weight, PU jet ID SF, subjet b-tagging SF, jet/e/mu->tau FR, lepton/tau ID SF, jet->lepton FR, DY weights, EWK eights, Vpt
    }
  }
  return supported_systematics;
}

DEFINE_EDM_PLUGIN(WriterPluginFactory, EvtWeightWriter, "EvtWeightWriter"); // clazy:skip
