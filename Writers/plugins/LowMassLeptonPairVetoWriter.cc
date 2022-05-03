#include "TallinnNtupleProducer/Writers/plugins/LowMassLeptonPairVetoWriter.h"

#include "TallinnNtupleProducer/CommonTools/interface/BranchAddressInitializer.h" // BranchAddressInitializer
#include "TallinnNtupleProducer/CommonTools/interface/cmsException.h"             // cmsException()
#include "TallinnNtupleProducer/Objects/interface/RecoLepton.h"                   // RecoLeptonPtrCollection

#include "TString.h"                                                              // Form()
#include "TTree.h"                                                                // TTree

#include <assert.h>                                                               // assert()
#include <cstdlib>                                                                // std::abs()

LowMassLeptonPairVetoWriter::LowMassLeptonPairVetoWriter(const edm::ParameterSet & cfg)
  : WriterBase(cfg)
  , branchName_("passesLowMassLeptonPairVeto")
{
  requireSF_ = cfg.getParameter<bool>("requireSF");
  requireOS_ = cfg.getParameter<bool>("requireOS");
  passesLowMassLeptonPairVeto_ = true;
}

LowMassLeptonPairVetoWriter::~LowMassLeptonPairVetoWriter()
{}

void
LowMassLeptonPairVetoWriter::setBranches(TTree * outputTree)
{
  BranchAddressInitializer bai(outputTree);
  bai.setBranch(passesLowMassLeptonPairVeto_, branchName_);
}

void
LowMassLeptonPairVetoWriter::writeImp(const Event & event, const EvtWeightRecorder & evtWeightRecorder)
{
  passesLowMassLeptonPairVeto_ = true;
  const RecoLeptonPtrCollection& looseLeptons = event.looseLeptons();
  for ( auto lepton1 = looseLeptons.begin(); lepton1 != looseLeptons.end(); ++lepton1 )
  {
    for ( auto lepton2 = lepton1 + 1; lepton2 != looseLeptons.end(); ++lepton2 )
    {
      if ( requireSF_ && std::abs((*lepton1)->pdgId()) != std::abs((*lepton2)->pdgId()) ) continue;
      if ( requireOS_ && (*lepton1)->charge()*(*lepton2)->charge() > 0 ) continue;
      double mass = ((*lepton1)->p4() + (*lepton2)->p4()).mass();
      if ( mass < 12. )
      {
        passesLowMassLeptonPairVeto_ = false;
        break;
      }
    }
  }
}

std::vector<std::string>
LowMassLeptonPairVetoWriter::get_supported_systematics()
{
  return std::vector<std::string>();
}

DEFINE_EDM_PLUGIN(WriterPluginFactory, LowMassLeptonPairVetoWriter, "LowMassLeptonPairVetoWriter");
