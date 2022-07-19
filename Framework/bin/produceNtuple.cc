
/** produceNtuple
 *
 * Read nanoAOD Ntuple and produce "plain" Ntuple.
 *
 * \authors Christian Veelken, Tallinn
 *
 */

#include "DataFormats/FWLite/interface/InputSource.h"                                           // fwlite::InputSource
#include "DataFormats/FWLite/interface/OutputFiles.h"                                           // fwlite::OutputFiles
#include "FWCore/ParameterSet/interface/ParameterSet.h"                                         // edm::ParameterSet
#include "FWCore/ParameterSetReader/interface/ParameterSetReader.h"                             // edm::readPSetsFrom()
#include "FWCore/PluginManager/interface/PluginManager.h"                                       // edmplugin::PluginManager::configure()
#include "FWCore/PluginManager/interface/standard.h"                                            // edmplugin::standard::config()
#include "PhysicsTools/FWLite/interface/TFileService.h"                                         // fwlite::TFileService

#include "TallinnNtupleProducer/CommonTools/interface/BranchAddressInitializer.h"               // BranchAddressInitializer::print()
#include "TallinnNtupleProducer/CommonTools/interface/cmsException.h"                           // cmsException
#include "TallinnNtupleProducer/CommonTools/interface/Era.h"                                    // Era, get_era()
#include "TallinnNtupleProducer/CommonTools/interface/format_vT.h"                              // format_vstring()
#include "TallinnNtupleProducer/CommonTools/interface/hadTauDefinitions.h"                      // get_tau_id_wp_int()
#include "TallinnNtupleProducer/CommonTools/interface/merge_systematic_shifts.h"                // merge_systematic_shifts()
#include "TallinnNtupleProducer/CommonTools/interface/tH_auxFunctions.h"                        // get_tH_SM_str()
#include "TallinnNtupleProducer/CommonTools/interface/TTreeWrapper.h"                           // TTreeWrapper
#include "TallinnNtupleProducer/EvtWeightTools/interface/BtagSFRatioInterface.h"                // BtagSFRatioInterface
#include "TallinnNtupleProducer/EvtWeightTools/interface/ChargeMisIdRateInterface.h"            // ChargeMisIdRateInterface
#include "TallinnNtupleProducer/EvtWeightTools/interface/Data_to_MC_CorrectionInterface_2016.h" // Data_to_MC_CorrectionInterface_2016
#include "TallinnNtupleProducer/EvtWeightTools/interface/Data_to_MC_CorrectionInterface_2017.h" // Data_to_MC_CorrectionInterface_2017
#include "TallinnNtupleProducer/EvtWeightTools/interface/Data_to_MC_CorrectionInterface_2018.h" // Data_to_MC_CorrectionInterface_2018
#include "TallinnNtupleProducer/EvtWeightTools/interface/EvtWeightManager.h"                    // EvtWeightManager
#include "TallinnNtupleProducer/EvtWeightTools/interface/EvtWeightRecorder.h"                   // EvtWeightRecorder
#include "TallinnNtupleProducer/EvtWeightTools/interface/HadTauFakeRateInterface.h"             // HadTauFakeRateInterface
#include "TallinnNtupleProducer/EvtWeightTools/interface/HHWeightInterfaceCouplings.h"          // HHWeightInterfaceCouplings
#include "TallinnNtupleProducer/EvtWeightTools/interface/HHWeightInterfaceLO.h"                 // HHWeightInterfaceLO
#include "TallinnNtupleProducer/EvtWeightTools/interface/HHWeightInterfaceNLO.h"                // HHWeightInterfaceNLO
#include "TallinnNtupleProducer/EvtWeightTools/interface/LeptonFakeRateInterface.h"             // LeptonFakeRateInterface
#include "TallinnNtupleProducer/Objects/interface/Event.h"                                      // Event
#include "TallinnNtupleProducer/Objects/interface/EventInfo.h"                                  // EventInfo
#include "TallinnNtupleProducer/Objects/interface/GenHadTau.h"                                  // GenHadTau
#include "TallinnNtupleProducer/Objects/interface/GenJet.h"                                     // GenJet
#include "TallinnNtupleProducer/Objects/interface/RunLumiEvent.h"                               // RunLumiEvent
#include "TallinnNtupleProducer/Objects/interface/TriggerInfo.h"                                // TriggerInfo
#include "TallinnNtupleProducer/Readers/interface/EventReader.h"                                // EventReader
#include "TallinnNtupleProducer/Readers/interface/GenHadTauReader.h"                            // GenHadTauReader
#include "TallinnNtupleProducer/Readers/interface/GenJetReader.h"                               // GenJetReader
#include "TallinnNtupleProducer/Readers/interface/GenParticleReader.h"                          // GenParticleReader
#include "TallinnNtupleProducer/Readers/interface/L1PreFiringWeightReader.h"                    // L1PreFiringWeightReader
#include "TallinnNtupleProducer/Readers/interface/LHEInfoReader.h"                              // LHEInfoReader
#include "TallinnNtupleProducer/Readers/interface/LHEParticleReader.h"
#include "TallinnNtupleProducer/Readers/interface/PSWeightReader.h"                             // PSWeightReader
#include "TallinnNtupleProducer/Selectors/interface/RunLumiEventSelector.h"                     // RunLumiEventSelector
#include "TallinnNtupleProducer/Writers/interface/WriterBase.h"                                 // WriterBase, WriterPluginFactory
#include <TBenchmark.h>                                                                         // TBenchmark
#include <TError.h>                                                                             // gErrorAbortLevel, kError
#include <TString.h>                                                                            // TString, Form()
#include <TTree.h>                                                                              // TTree

#include "correction.h"
#include <assert.h>                                                                             // assert()
#include <cstdlib>                                                                              // EXIT_SUCCESS, EXIT_FAILURE
#include <iostream>                                                                             // std::cout
#include <string>                                                                               // std::string
#include <vector>                                                                               // std::vector

typedef std::vector<std::string> vstring;

template <typename T>
void
copyParameter(const edm::ParameterSet& cfg_source, edm::ParameterSet& cfg_target, const std::string& attr)
{
  cfg_target.addParameter<T>(attr, cfg_source.getParameter<T>(attr));
} 

int main(int argc, char* argv[])
{
//--- throw an exception in case ROOT encounters an error
  gErrorAbortLevel = kError;

//--- stop ROOT from keeping track of all histograms
  TH1::AddDirectory(false);

//--- parse command-line arguments
  if ( argc < 2 ) {
    std::cout << "Usage: " << argv[0] << " [parameters.py]" << std::endl;
    return EXIT_FAILURE;
  }

  std::cout << "<produceNtuple>:" << std::endl;

//--- keep track of time it takes the macro to execute
  TBenchmark clock;
  clock.Start("produceNtuple");

//--- read python configuration parameters
  std::cout << "Reading config file " << argv[1] << std::endl;
  if ( !edm::readPSetsFrom(argv[1])->existsAs<edm::ParameterSet>("process") )
    throw cmsException("produceNtuple", __LINE__) << "No ParameterSet 'process' found in config file !!";

  edm::ParameterSet cfg = edm::readPSetsFrom(argv[1])->getParameterSet("process");

  edm::ParameterSet cfg_produceNtuple = cfg.getParameterSet("produceNtuple");
  AnalysisConfig analysisConfig("produceNtuple", cfg_produceNtuple);
  
  std::string treeName = cfg_produceNtuple.getParameter<std::string>("treeName");

  std::string process = cfg_produceNtuple.getParameter<std::string>("process");
  std::cout << "Processing process = '" << process << "'" << std::endl;

  std::string era_string = cfg_produceNtuple.getParameter<std::string>("era");
  const Era era = get_era(era_string);
  std::cout << "Setting era to: " << get_era(era) << std::endl;

  bool isMC = cfg_produceNtuple.getParameter<bool>("isMC");
  edm::VParameterSet lumiScale = cfg_produceNtuple.getParameterSetVector("lumiScale");
  bool apply_genWeight = cfg_produceNtuple.getParameter<bool>("apply_genWeight");
  std::string apply_topPtReweighting_str = cfg_produceNtuple.getParameter<std::string>("apply_topPtReweighting");
  bool apply_topPtReweighting = ! apply_topPtReweighting_str.empty();
  bool apply_l1PreFireWeight = cfg_produceNtuple.getParameter<bool>("apply_l1PreFireWeight");
  bool apply_btagSFRatio = cfg_produceNtuple.getParameter<bool>("apply_btagSFRatio");

  unsigned int numNominalLeptons = cfg_produceNtuple.getParameter<unsigned int>("numNominalLeptons");
  bool applyNumNominalLeptonsCut = cfg_produceNtuple.getParameter<bool>("applyNumNominalLeptonsCut");
  unsigned int numNominalHadTaus = cfg_produceNtuple.getParameter<unsigned int>("numNominalHadTaus");
  bool applyNumNominalHadTausCut = cfg_produceNtuple.getParameter<bool>("applyNumNominalHadTausCut");
  std::cout << "Setting nominal multiplicity of leptons and taus to: #leptons = " << numNominalLeptons << ", #taus = " << numNominalHadTaus << std::endl;
  if ( applyNumNominalLeptonsCut )
  {
    std::cout << "Skipping events that contain fewer than the nominal number of leptons." << std::endl;
  }
  if ( applyNumNominalHadTausCut )
  {
    std::cout << "Skipping events that contain fewer than the nominal number of taus." << std::endl;
  }

  std::string hadTauWP_againstJets = cfg_produceNtuple.getParameter<std::string>("hadTauWP_againstJets_tight");
  std::string hadTauWP_againstElectrons = cfg_produceNtuple.getParameter<std::string>("hadTauWP_againstElectrons");
  std::string hadTauWP_againstMuons = cfg_produceNtuple.getParameter<std::string>("hadTauWP_againstMuons");
  std::string lep_mva_wp = cfg_produceNtuple.getParameter<std::string>("lep_mva_wp");

  bool apply_chargeMisIdRate = cfg_produceNtuple.getParameter<bool>("apply_chargeMisIdRate");

  std::string selection = cfg_produceNtuple.getParameter<std::string>("selection");
  std::cout << "Selection set in config file = '" << selection << "'" << std::endl;
  if ( applyNumNominalLeptonsCut )
  {
    const std::string numNominalLeptonsCut = Form("nlep == %u", numNominalLeptons);
    std::cout << "Adding the condition '" << numNominalLeptonsCut << "' to selection, because the flag applyNumNominalLeptonsCut is enabled." << std::endl;
    if ( selection != "" ) selection.append(" & ");
    selection.append(numNominalLeptonsCut);
  }
  if ( applyNumNominalHadTausCut )
  {
    const std::string numNominalHadTausCut = Form("ntau == %u", numNominalHadTaus);
    std::cout << "Adding the condition '" << numNominalHadTausCut << "' to selection, because the flag applyNumNominalHadTausCut is enabled." << std::endl;
    if ( selection != "" ) selection.append(" & ");
    selection.append(numNominalHadTausCut);
  }
  std::cout << "Applying selection = '" << selection << "'" << std::endl;
  
  bool isDEBUG = cfg_produceNtuple.getParameter<bool>("isDEBUG");

  edm::ParameterSet cfg_dataToMCcorrectionInterface;
  cfg_dataToMCcorrectionInterface.addParameter<std::string>("era", era_string);
  cfg_dataToMCcorrectionInterface.addParameter<std::string>("hadTauSelection_againstJets", hadTauWP_againstJets);
  cfg_dataToMCcorrectionInterface.addParameter<int>("hadTauSelection_againstElectrons", get_tau_id_wp_int(hadTauWP_againstElectrons));
  cfg_dataToMCcorrectionInterface.addParameter<int>("hadTauSelection_againstMuons", get_tau_id_wp_int(hadTauWP_againstMuons));
  cfg_dataToMCcorrectionInterface.addParameter<std::string>("lep_mva_wp", lep_mva_wp);
  cfg_dataToMCcorrectionInterface.addParameter<bool>("isDEBUG", isDEBUG);
  Data_to_MC_CorrectionInterface_Base * dataToMCcorrectionInterface = nullptr;
  switch ( era )
  {
    case Era::k2016: dataToMCcorrectionInterface = new Data_to_MC_CorrectionInterface_2016(cfg_dataToMCcorrectionInterface); break;
    case Era::k2017: dataToMCcorrectionInterface = new Data_to_MC_CorrectionInterface_2017(cfg_dataToMCcorrectionInterface); break;
    case Era::k2018: dataToMCcorrectionInterface = new Data_to_MC_CorrectionInterface_2018(cfg_dataToMCcorrectionInterface); break;
    case Era::kUndefined: __attribute__((fallthrough));
    default: throw cmsException("produceNtuple", __LINE__) << "Invalid era = " << static_cast<int>(era);
  }
  const ChargeMisIdRateInterface chargeMisIdRateInterface(era);

  edm::ParameterSet cfg_leptonFakeRateWeight = cfg_produceNtuple.getParameterSet("leptonFakeRateWeight");
  cfg_leptonFakeRateWeight.addParameter<std::string>("era", era_string);
  LeptonFakeRateInterface* jetToLeptonFakeRateInterface = new LeptonFakeRateInterface(cfg_leptonFakeRateWeight);

  edm::ParameterSet cfg_hadTauFakeRateWeight = cfg_produceNtuple.getParameterSet("hadTauFakeRateWeight");
  cfg_hadTauFakeRateWeight.addParameter<std::string>("hadTauSelection", hadTauWP_againstJets);
  HadTauFakeRateInterface* jetToHadTauFakeRateInterface = new HadTauFakeRateInterface(cfg_hadTauFakeRateWeight);

  std::string selEventsFileName = cfg_produceNtuple.getParameter<std::string>("selEventsFileName");
  std::cout << "selEventsFileName = " << selEventsFileName << std::endl;
  RunLumiEventSelector* run_lumi_eventSelector = 0;
  if ( selEventsFileName != "" ) {
    edm::ParameterSet cfg_run_lumi_eventSelector;
    cfg_run_lumi_eventSelector.addParameter<std::string>("inputFileName", selEventsFileName);
    cfg_run_lumi_eventSelector.addParameter<std::string>("separator", ":");
    run_lumi_eventSelector = new RunLumiEventSelector(cfg_run_lumi_eventSelector);
  }

  fwlite::InputSource inputFiles(cfg);
  int maxEvents = inputFiles.maxEvents();
  std::cout << " maxEvents = " << maxEvents << std::endl;
  unsigned reportEvery = inputFiles.reportAfter();

  fwlite::OutputFiles outputFile(cfg);
  fwlite::TFileService fs = fwlite::TFileService(outputFile.file().data());

  TTreeWrapper* inputTree = new TTreeWrapper(treeName.data(), inputFiles.files(), maxEvents);
  std::cout << "Loaded " << inputTree->getFileCount() << " file(s).\n";

  EventReader* eventReader = new EventReader(cfg_produceNtuple);
  inputTree->registerReader(eventReader);

  eventReader->set_tauEScset(dataToMCcorrectionInterface->get_tau_energy_scale_cset());

  TTree* outputTree = fs.make<TTree>("Events", "Events");

  const edm::ParameterSet additionalEvtWeight = cfg_produceNtuple.getParameterSet("evtWeight");
  const bool applyAdditionalEvtWeight = additionalEvtWeight.getParameter<bool>("apply");
  EvtWeightManager* eventWeightManager = nullptr;
  if ( applyAdditionalEvtWeight )
  {
    eventWeightManager = new EvtWeightManager(additionalEvtWeight);
    eventWeightManager->set_central_or_shift("central");
    inputTree->registerReader(eventWeightManager);
  }

  L1PreFiringWeightReader* l1PreFiringWeightReader = nullptr;
  if ( apply_l1PreFireWeight )
  {
    l1PreFiringWeightReader = new L1PreFiringWeightReader(cfg_produceNtuple);
    inputTree->registerReader(l1PreFiringWeightReader);
  }

  LHEInfoReader* lheInfoReader = nullptr;
  PSWeightReader* psWeightReader = nullptr;
  LHEParticleReader* lheParticleReader = nullptr;
  if ( isMC )
  {
    lheInfoReader = new LHEInfoReader(cfg_produceNtuple);
    inputTree->registerReader(lheInfoReader);
    psWeightReader = new PSWeightReader(cfg_produceNtuple);
    inputTree->registerReader(psWeightReader);
    lheParticleReader = new LHEParticleReader(cfg_produceNtuple);
    inputTree->registerReader(lheParticleReader);
  }

  BtagSFRatioInterface* btagSFRatioInterface = nullptr;
  if ( apply_btagSFRatio )
  {
    const edm::ParameterSet btagSFRatio = cfg_produceNtuple.getParameterSet("btagSFRatio");
    btagSFRatioInterface = new BtagSFRatioInterface(btagSFRatio);
  }

  // CV: create plugins that write branches to "plain" Ntuple
  if ( !edmplugin::PluginManager::isAvailable() )
  {  
    edmplugin::PluginManager::configure(edmplugin::standard::config());
  }
  edm::VParameterSet cfg_writers = cfg_produceNtuple.getParameterSetVector("writerPlugins");
  std::vector<std::unique_ptr<WriterBase>> writers;
  for ( auto cfg_writer : cfg_writers )
  {
    std::string pluginType = cfg_writer.getParameter<std::string>("pluginType");
    cfg_writer.addParameter<unsigned int>("numNominalLeptons", numNominalLeptons);
    cfg_writer.addParameter<unsigned int>("numNominalHadTaus", numNominalHadTaus);
    copyParameter<vstring>(cfg_produceNtuple, cfg_writer, "disable_ak8_corr");
    cfg_writer.addParameter<std::string>("era", get_era(era));
    cfg_writer.addParameter<bool>("isMC", isMC);
    cfg_writer.addParameter<std::string>("process", process);
    std::unique_ptr<WriterBase> writer = WriterPluginFactory::get()->create(pluginType, cfg_writer);
    writer->registerReaders(inputTree);
    writer->setBranches(outputTree);
    writers.push_back(std::move(writer));
  }

  std::vector<std::string> systematic_shifts;
  // CV: process all systematic uncertainties supported by any plugin that writes branches to "plain" Ntuple (only for MC)
  if ( isMC )
  {
    for ( auto & writer : writers )
    {
      merge_systematic_shifts(systematic_shifts, writer->get_supported_systematics(cfg_produceNtuple));
    }
  }
  // CV: add central value (for data and MC)
  merge_systematic_shifts(systematic_shifts, { "central" });
  std::cout << "Processing systematic uncertainties = " << format_vstring(systematic_shifts) << std::endl;

  int analyzedEntries = 0;
  TH1* histogram_analyzedEntries = fs.make<TH1D>("analyzedEntries", "analyzedEntries", 1, -0.5, +0.5);
  while ( inputTree->hasNextEvent() && (!run_lumi_eventSelector || (run_lumi_eventSelector && !run_lumi_eventSelector->areWeDone())) )
  {
    bool skipEvent = false;
    for ( const auto & central_or_shift : systematic_shifts )
    {
      const RunLumiEvent & runLumiEvent = eventReader->read_runLumiEvent();
      if ( central_or_shift == "central" )
      {
        if ( inputTree->canReport(reportEvery) )
        {
          std::cout << "processing Entry " << inputTree->getCurrentMaxEventIdx()
                    << " or " << inputTree->getCurrentEventIdx() << " entry in #"
                    << (inputTree->getProcessedFileCount() - 1)
                    << " (" << runLumiEvent
                    << ") file\n";
        }
        ++analyzedEntries;
        histogram_analyzedEntries->Fill(0.);
      }
      if ( run_lumi_eventSelector && !(*run_lumi_eventSelector)(runLumiEvent) )
      {
        skipEvent = true;
        break; // skip to the next event
      }
      if ( isDEBUG || run_lumi_eventSelector )
      {
        std::cout << "Processing central_or_shift = '" << central_or_shift << "'\n";
      }
      if ( central_or_shift == "central" && (isDEBUG || run_lumi_eventSelector) )
      {
        std::cout << "processing Entry " << inputTree->getCurrentMaxEventIdx() << ": " << runLumiEvent << '\n';
        if ( inputTree->isOpen() )
        {
          std::cout << "input File = " << inputTree->getCurrentFileName() << '\n';
        }
      }

      eventReader->set_central_or_shift(central_or_shift);
      const Event& event = eventReader->read();
      // CV: skip processing events that don't contain the nominal number of leptons and hadronic taus,
      //     if the flags applyNumNominalLeptonsCut and applyNumNominalHadTausCut are enabled in the config file
      if ( event.isInvalid() )
      {
        skipEvent = true;
        continue;
      }

      EvtWeightRecorder evtWeightRecorder({ central_or_shift }, central_or_shift, isMC);
      if ( isMC )
      {
        if ( apply_genWeight         ) evtWeightRecorder.record_genWeight(event.eventInfo());
        if ( eventWeightManager      )
        { 
          eventWeightManager->set_central_or_shift(central_or_shift);
          evtWeightRecorder.record_auxWeight(eventWeightManager);
        }
        if ( l1PreFiringWeightReader ) evtWeightRecorder.record_l1PrefireWeight(l1PreFiringWeightReader);
        if ( apply_topPtReweighting  ) evtWeightRecorder.record_toppt_rwgt(event.topPtRwgtSF());
        lheInfoReader->read();
        psWeightReader->read();
        evtWeightRecorder.record_lheScaleWeight(lheInfoReader);
        evtWeightRecorder.record_psWeight(psWeightReader);
        if ( analysisConfig.isHH_rwgt_allowed() )
        {
          LHEParticleCollection lheParticles = lheParticleReader->read();
          evtWeightRecorder.record_gen_mHH_cosThetaStar(lheParticles);
        }
        evtWeightRecorder.record_puWeight(&event.eventInfo());
        evtWeightRecorder.record_nom_tH_weight(&event.eventInfo());
        evtWeightRecorder.record_lumiScale(lumiScale);

//--- compute event-level weight for data/MC correction of b-tagging efficiency and mistag rate
//   (using the method "Event reweighting using scale factors calculated with a tag and probe method",
//    described on the BTV POG twiki https://twiki.cern.ch/twiki/bin/view/CMS/BTagShapeCalibration )
        evtWeightRecorder.record_btagWeight(event.selJetsAK4());
        if ( btagSFRatioInterface )
        {
          evtWeightRecorder.record_btagSFRatio(btagSFRatioInterface, event.selJetsAK4().size());
        }

        if ( analysisConfig.isMC_EWK() )
        {
          evtWeightRecorder.record_ewk_jet(event.selJetsAK4());
          evtWeightRecorder.record_ewk_bjet(event.selJetsAK4_btagMedium());
        }

        dataToMCcorrectionInterface->setLeptons(event.fakeableLeptons(), true);

//--- apply data/MC corrections for trigger efficiency
        evtWeightRecorder.record_leptonTriggerEff(dataToMCcorrectionInterface);

//--- apply data/MC corrections for efficiencies for lepton to pass loose identification and isolation criteria
        evtWeightRecorder.record_leptonIDSF_recoToLoose(dataToMCcorrectionInterface);

//--- apply data/MC corrections for efficiencies of leptons passing the loose identification and isolation criteria
//    to also pass the fakeable and/or tight identification and isolation criteria
        evtWeightRecorder.record_leptonIDSF_looseToTight(dataToMCcorrectionInterface, false);

//--- apply data/MC corrections for hadronic tau identification efficiency
//    and for e->tau and mu->tau misidentification rates
        dataToMCcorrectionInterface->setHadTaus(event.fakeableHadTaus());
        evtWeightRecorder.record_hadTauID_and_Iso(dataToMCcorrectionInterface);
        evtWeightRecorder.record_eToTauFakeRate(dataToMCcorrectionInterface);
        evtWeightRecorder.record_muToTauFakeRate(dataToMCcorrectionInterface);

        evtWeightRecorder.record_jetToLeptonFakeRate(jetToLeptonFakeRateInterface, event.fakeableLeptons());
        evtWeightRecorder.record_jetToTauFakeRate(jetToHadTauFakeRateInterface, event.fakeableHadTaus());
        evtWeightRecorder.compute_FR();

        if ( apply_chargeMisIdRate )
        {
          if ( numNominalLeptons == 2 && numNominalHadTaus == 0 )
          {
            double prob_chargeMisId_sum = 1.;
            if ( event.fakeableLeptons().size() == 2 )
            {
              const RecoLepton* fakeableLepton_lead = event.fakeableLeptons().at(0);
              const RecoLepton* fakeableLepton_sublead = event.fakeableLeptons().at(1);
              if ( fakeableLepton_lead->charge()*fakeableLepton_sublead->charge() > 0 )
              {
                prob_chargeMisId_sum = chargeMisIdRateInterface.get(fakeableLepton_lead, fakeableLepton_sublead);
              }
            }
            evtWeightRecorder.record_chargeMisIdProb(prob_chargeMisId_sum);
          }
          else if ( numNominalLeptons == 2 && numNominalHadTaus == 1 )
          {
            double prob_chargeMisId = 1.;
            if ( event.fakeableLeptons().size() == 2 && event.fakeableHadTaus().size() == 1 )
            {
              const RecoLepton* fakeableLepton_lead = event.fakeableLeptons().at(0);
              const RecoLepton* fakeableLepton_sublead = event.fakeableLeptons().at(1);
              const RecoHadTau* fakeableHadTau = event.fakeableHadTaus().at(0);
              if ( fakeableLepton_lead->charge()*fakeableLepton_sublead->charge() > 0 )
              {
                // CV: apply charge misidentification probability to lepton of same charge as hadronic tau
                //    (if the lepton of charge opposite to the charge of the hadronic tau "flips",
                //     the event has sum of charges equal to three and fails "lepton+tau charge" cut)
                if ( fakeableLepton_lead->charge()*fakeableHadTau->charge()    > 0 ) prob_chargeMisId *= chargeMisIdRateInterface.get(fakeableLepton_lead);
                if ( fakeableLepton_sublead->charge()*fakeableHadTau->charge() > 0 ) prob_chargeMisId *= chargeMisIdRateInterface.get(fakeableLepton_sublead);
              }
              else if ( fakeableLepton_lead->charge()*fakeableLepton_sublead->charge() < 0 )
              {
                // CV: apply charge misidentification probability to lepton of opposite charge as hadronic tau
                //    (if the lepton of same charge as the hadronic tau "flips",
                //     the event has sum of charges equal to one and fails "lepton+tau charge" cut)
                if ( fakeableLepton_lead->charge()*fakeableHadTau->charge()    < 0 ) prob_chargeMisId *= chargeMisIdRateInterface.get(fakeableLepton_lead);
                if ( fakeableLepton_sublead->charge()*fakeableHadTau->charge() < 0 ) prob_chargeMisId *= chargeMisIdRateInterface.get(fakeableLepton_sublead);
              } else assert(0);
            }
            evtWeightRecorder.record_chargeMisIdProb(prob_chargeMisId);
          }
          else 
          {
            throw cmsException("produceNtuple", __LINE__) 
              << "Configuration parameter 'apply_chargeMisIdRate' = " << apply_chargeMisIdRate 
              << " not supported for categories with " << numNominalLeptons << " lepton(s) and " << numNominalHadTaus << " hadronic tau(s) !!";
          }
        }
      }
      if(isDEBUG)
      {
        std::cout << evtWeightRecorder << '\n';
      }

      for ( auto & writer : writers )
      {
        writer->set_central_or_shift(central_or_shift);
        writer->write(event, evtWeightRecorder);
      }
    }
    if ( !skipEvent )
    {
      outputTree->Fill();
    }
  }

  TDirectory* dir = fs.getBareDirectory();
  dir->cd();

  TTree* outputTree_selected = nullptr;
  if ( selection != "" )
  {
    outputTree_selected = outputTree->CopyTree(selection.data());
  }
  else
  {
    outputTree_selected = outputTree;
  }
  Float_t evtWeight = 1.;
  if ( outputTree_selected->GetBranch("evtWeight") )
  {
    outputTree_selected->SetBranchAddress("evtWeight", &evtWeight);
  }
  int selectedEntries = 0;
  double selectedEntries_weighted = 0.;
  TH1* histogram_selectedEntries = fs.make<TH1D>("selectedEntries", "selectedEntries", 1, -0.5, +0.5);
  int numEntries = outputTree_selected->GetEntries();
  for ( int idxEntry = 0; idxEntry < numEntries; ++idxEntry )
  {
    outputTree_selected->GetEntry(idxEntry);
    ++selectedEntries;
    selectedEntries_weighted += evtWeight;
    histogram_selectedEntries->Fill(0.);
  }
  outputTree_selected->Write();

  std::cout << "max num. Entries = " << inputTree->getCumulativeMaxEventCount()
            << " (limited by " << maxEvents << ") processed in "
            << inputTree->getProcessedFileCount() << " file(s) (out of "
            << inputTree->getFileCount() << ")\n"
            << " analyzed = " << analyzedEntries << '\n'
            << " selected = " << selectedEntries << " (weighted = " << selectedEntries_weighted << ")" << std::endl;
  //std::cout << std::endl;

  //BranchAddressInitializer::print(std::cout);

//--- memory clean-up
  delete run_lumi_eventSelector;

  delete eventReader;
  delete l1PreFiringWeightReader;
  delete lheInfoReader;
  delete psWeightReader;
  delete lheParticleReader;

  delete dataToMCcorrectionInterface;
  delete jetToLeptonFakeRateInterface;
  delete jetToHadTauFakeRateInterface;
  delete btagSFRatioInterface;

  for ( auto & writer : writers )
  {
    writer.reset(nullptr);
  }

  delete inputTree;

  delete outputTree;
  if ( outputTree_selected != outputTree )
  {
    delete outputTree_selected;
  }

  clock.Show("produceNtuple");

  return EXIT_SUCCESS;
}
