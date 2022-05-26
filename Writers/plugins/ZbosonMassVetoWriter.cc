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
  requireOS_e_ = cfg.getParameter<bool>("requireOS_e");
  requireOS_mu_ = cfg.getParameter<bool>("requireOS_mu");
  passesZbosonMassVeto_ = true;
  isDEBUG_ = cfg.getParameter<bool>("isDEBUG");
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
  const RecoLeptonPtrCollection& looseLeptons = event.looseLeptonsFull();
  for ( auto lepton1 = looseLeptons.begin(); lepton1 != looseLeptons.end(); ++lepton1 )
  {
    for ( auto lepton2 = lepton1 + 1; lepton2 != looseLeptons.end(); ++lepton2 )
    {
      if ( std::abs((*lepton1)->pdgId()) != std::abs((*lepton2)->pdgId()) ) continue;
      bool is_ee = std::abs((*lepton1)->pdgId()) == 11 && std::abs((*lepton2)->pdgId()) == 11;
      if ( requireOS_e_ && is_ee && (*lepton1)->charge()*(*lepton2)->charge() > 0 ) continue;
      bool is_mumu = std::abs((*lepton1)->pdgId()) == 13 && std::abs((*lepton2)->pdgId()) == 13;
      if ( requireOS_mu_ && is_mumu && (*lepton1)->charge()*(*lepton2)->charge() > 0 ) continue;
      //double mass = ((*lepton1)->cone_p4() + (*lepton2)->cone_p4()).mass();
      double mass = ((*lepton1)->p4() + (*lepton2)->p4()).mass(); // CV: only for testing (backwards compatibility with old HH->multilepton 2lss code) !!
      if ( std::fabs(mass - z_mass_) < z_window_ )
      {
        if ( isDEBUG_ )
        {
          std::cout << "failsZbosonMassVeto:" << std::endl;
          std::cout << "lepton1: pT = " << (*lepton1)->pt() << ", eta = " << (*lepton1)->eta() << ", phi = " << (*lepton1)->phi() << "," 
                    << " pdgId = " << (*lepton1)->pdgId() << std::endl;
          std::cout << "lepton2: pT = " << (*lepton2)->pt() << ", eta = " << (*lepton2)->eta() << ", phi = " << (*lepton2)->phi() << "," 
                    << " pdgId = " << (*lepton2)->pdgId() << std::endl;
          std::cout << "mass = " << mass << " (mass computed using cone_p4() = " << ((*lepton1)->cone_p4() + (*lepton2)->cone_p4()).mass() << ")" << std::endl;
        }
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
