#include "TallinnNtupleProducer/Writers/plugins/HtoZZto4lVetoWriter.h"

#include "TallinnNtupleProducer/CommonTools/interface/BranchAddressInitializer.h" // BranchAddressInitializer
#include "TallinnNtupleProducer/CommonTools/interface/cmsException.h"             // cmsException()
#include "TallinnNtupleProducer/CommonTools/interface/merge_systematic_shifts.h"  // merge_systematic_shifts()
#include "TallinnNtupleProducer/Objects/interface/RecoLepton.h"                   // RecoLeptonPtrCollection
#include "TallinnNtupleProducer/Readers/interface/RecoElectronReader.h"           // RecoElectronReader::get_supported_systematics()
#include "TallinnNtupleProducer/Readers/interface/RecoMuonReader.h"               // RecoMuonReader::get_supported_systematics()

#include "TString.h"                                                              // Form()
#include "TTree.h"                                                                // TTree

#include <assert.h>                                                               // assert()
#include <cstdlib>                                                                // std::abs()

HtoZZto4lVetoWriter::HtoZZto4lVetoWriter(const edm::ParameterSet & cfg)
  : WriterBase(cfg)
  , branchName_("passesHtoZZto4lVeto")
{
  passesHtoZZto4lVeto_ = true;
}

HtoZZto4lVetoWriter::~HtoZZto4lVetoWriter()
{}

void
HtoZZto4lVetoWriter::setBranches(TTree * outputTree)
{
  BranchAddressInitializer bai(outputTree);
  bai.setBranch(passesHtoZZto4lVeto_, branchName_);
}

void
HtoZZto4lVetoWriter::writeImp(const Event & event, const EvtWeightRecorder & evtWeightRecorder)
{
  passesHtoZZto4lVeto_ = true;
  const RecoLeptonPtrCollection& looseLeptons = event.looseLeptonsFull();
  for ( auto lepton1 = looseLeptons.begin(); lepton1 != looseLeptons.end(); ++lepton1 )
  {
    for ( auto lepton2 = lepton1 + 1; lepton2 != looseLeptons.end(); ++lepton2 )
    {
      // require that first and second lepton are of the same flavor and of opposite charge (first SFOS lepton pair)
      if ( !((*lepton1)->pdgId() == -(*lepton2)->pdgId()) ) continue;
      for ( auto lepton3 = looseLeptons.begin(); lepton3 != looseLeptons.end(); ++lepton3 )
      {
        if ( (*lepton3) == (*lepton1) || (*lepton3) == (*lepton2) )
        {
          continue;
        }
        for ( auto lepton4 = lepton3 + 1; lepton4 != looseLeptons.end(); ++lepton4 )
        {
          if ( (*lepton4) == (*lepton1) || (*lepton4) == (*lepton2) )
          {
            continue;
          }
          // require that third and fourth lepton are of the same flavor and of opposite charge (second SFOS lepton pair)
          if ( !((*lepton3)->pdgId() == -(*lepton4)->pdgId()) )
          {
            continue;
          }
          const double mass = ((*lepton1)->p4() + (*lepton2)->p4() + (*lepton3)->p4() + (*lepton4)->p4()).mass();
          if ( mass < 140. )
          {
            passesHtoZZto4lVeto_ = false;
            break;
          }
        }
      }
    }
  }
}

std::vector<std::string>
HtoZZto4lVetoWriter::get_supported_systematics(const edm::ParameterSet & cfg)
{
  std::vector<std::string> supported_systematics;
  merge_systematic_shifts(supported_systematics, RecoElectronReader::get_supported_systematics(cfg));
  merge_systematic_shifts(supported_systematics, RecoMuonReader::get_supported_systematics(cfg));
  return supported_systematics;
}

DEFINE_EDM_PLUGIN(WriterPluginFactory, HtoZZto4lVetoWriter, "HtoZZto4lVetoWriter");
