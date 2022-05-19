#include "TallinnNtupleProducer/Writers/plugins/ZbosonMassVetoWriter.h"

#include "TallinnNtupleProducer/CommonTools/interface/BranchAddressInitializer.h" // BranchAddressInitializer
#include "TallinnNtupleProducer/CommonTools/interface/cmsException.h"             // cmsException()
#include "TallinnNtupleProducer/Objects/interface/RecoLepton.h"                   // RecoLeptonPtrCollection

#include "TString.h"                                                              // Form()
#include "TTree.h"                                                                // TTree

#include <assert.h>                                                               // assert()
#include <cstdlib>                                                                // std::abs()

ZbosonMassVetoWriter::ZbosonMassVetoWriter(const edm::ParameterSet & cfg)
  : WriterBase(cfg)
  , branchName_("passesZbosonVeto")
{
  z_mass_ = cfg.getParameter<double>("z_mass");
  z_window_ = cfg.getParameter<double>("z_window");
  requireOS_ = cfg.getParameter<bool>("requireOS");
  passesZbosonMassVeto_ = true;
}

ZbosonMassVetoWriter::~ZbosonMassVetoWriter()
{}

void
ZbosonMassVetoWriter::setBranches(TTree * outputTree)
{
  BranchAddressInitializer bai(outputTree);
  bai.setBranch(passesZbosonMassVeto_, branchName_);
}

void
ZbosonMassVetoWriter::writeImp(const Event & event, const EvtWeightRecorder & evtWeightRecorder)
{
  passesZbosonMassVeto_ = true;
  const RecoLeptonPtrCollection& looseLeptons = event.looseLeptons();
  for ( auto lepton1 = looseLeptons.begin(); lepton1 != looseLeptons.end(); ++lepton1 )
  {
    for ( auto lepton2 = lepton1 + 1; lepton2 != looseLeptons.end(); ++lepton2 )
    {
      if ( std::abs((*lepton1)->pdgId()) != std::abs((*lepton2)->pdgId()) ) continue;
      if ( requireOS_ && (*lepton1)->charge()*(*lepton2)->charge() > 0 ) continue;
      double mass = ((*lepton1)->p4() + (*lepton2)->p4()).mass();
      if ( std::fabs(mass - z_mass_) < z_window_ )
      {
        passesZbosonMassVeto_ = false;
        break;
      }
    }
  }
}

std::vector<std::string>
ZbosonMassVetoWriter::get_supported_systematics(const edm::ParameterSet & cfg)
{
  return std::vector<std::string>();
}

DEFINE_EDM_PLUGIN(WriterPluginFactory, ZbosonMassVetoWriter, "ZbosonMassVetoWriter");
