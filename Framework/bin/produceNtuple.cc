
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

#include "TallinnNtupleProducer/CommonTools/interface/memory_logger.h"                          // log_memory(), display_memory()
#include "TallinnNtupleProducer/CommonTools/interface/cmsException.h"                           // cmsException
#include "TallinnNtupleProducer/CommonTools/interface/Era.h"                                    // Era, get_era()
#include "TallinnNtupleProducer/CommonTools/interface/contains.h"                               // contains()
#include "TallinnNtupleProducer/CommonTools/interface/format_vT.h"                              // format_vstring()
#include "TallinnNtupleProducer/CommonTools/interface/hadTauDefinitions.h"                      // get_tau_id_wp_int()
#include "TallinnNtupleProducer/CommonTools/interface/merge_systematic_shifts.h"                // merge_systematic_shifts()
#include "TallinnNtupleProducer/CommonTools/interface/TTreeWrapper.h"                           // TTreeWrapper
#include "TallinnNtupleProducer/EvtWeightTools/interface/ChargeMisIdRateInterface.h"            // ChargeMisIdRateInterface
#include "TallinnNtupleProducer/EvtWeightTools/interface/Data_to_MC_CorrectionInterface_2016.h" // Data_to_MC_CorrectionInterface_2016
#include "TallinnNtupleProducer/EvtWeightTools/interface/Data_to_MC_CorrectionInterface_2017.h" // Data_to_MC_CorrectionInterface_2017
#include "TallinnNtupleProducer/EvtWeightTools/interface/Data_to_MC_CorrectionInterface_2018.h" // Data_to_MC_CorrectionInterface_2018
#include "TallinnNtupleProducer/EvtWeightTools/interface/DYMCReweighting.h"                     // DYMCReweighting
#include "TallinnNtupleProducer/EvtWeightTools/interface/DYMCNormScaleFactors.h"                // DYMCNormScaleFactors
#include "TallinnNtupleProducer/EvtWeightTools/interface/EvtWeightManager.h"                    // EvtWeightManager
#include "TallinnNtupleProducer/EvtWeightTools/interface/EvtWeightRecorder.h"                   // EvtWeightRecorder
#include "TallinnNtupleProducer/EvtWeightTools/interface/HadTauFakeRateInterface.h"             // HadTauFakeRateInterface
#include "TallinnNtupleProducer/EvtWeightTools/interface/LeptonFakeRateInterface.h"             // LeptonFakeRateInterface
#include "TallinnNtupleProducer/EvtWeightTools/interface/LHEVpt_LOtoNLO.h"                      // LHEVpt_LOtoNLO
#include "TallinnNtupleProducer/Objects/interface/Event.h"                                      // Event
#include "TallinnNtupleProducer/Objects/interface/EventInfo.h"                                  // EventInfo
#include "TallinnNtupleProducer/Objects/interface/RunLumiEvent.h"                               // RunLumiEvent
#include "TallinnNtupleProducer/Readers/interface/EventReader.h"                                // EventReader
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

  const edm::ParameterSet cfg = edm::readPSetsFrom(argv[1])->getParameterSet("process");

  const edm::ParameterSet cfg_produceNtuple = cfg.getParameterSet("produceNtuple");
  const AnalysisConfig analysisConfig("produceNtuple", cfg_produceNtuple);
  
  const std::string treeName = cfg_produceNtuple.getParameter<std::string>("treeName");

  const std::string process = cfg_produceNtuple.getParameter<std::string>("process");
  std::cout << "Processing process = '" << process << "'" << std::endl;

  std::string era_string = cfg_produceNtuple.getParameter<std::string>("era");
  const Era era = get_era(era_string);
  std::cout << "Setting era to: " << get_era(era) << std::endl;

  const bool isMC = cfg_produceNtuple.getParameter<bool>("isMC");
  const edm::VParameterSet lumiScale = cfg_produceNtuple.getParameterSetVector("lumiScale");
  const bool apply_genWeight = cfg_produceNtuple.getParameter<bool>("apply_genWeight");
  const std::string apply_topPtReweighting_str = cfg_produceNtuple.getParameter<std::string>("apply_topPtReweighting");
  const std::string apply_pileupJetID_str = cfg_produceNtuple.getParameter<std::string>("apply_pileupJetID");
  const pileupJetID apply_pileupJetID = get_pileupJetID(apply_pileupJetID_str);
  const bool apply_topPtReweighting = ! apply_topPtReweighting_str.empty();
  const bool apply_DYMCReweighting = cfg_produceNtuple.getParameter<bool>("apply_DYMCReweighting");
  const bool apply_DYMCNormScaleFactors = cfg_produceNtuple.getParameter<bool>("apply_DYMCNormScaleFactors");
  const bool apply_LHEVpt_rwgt = cfg_produceNtuple.getParameter<bool>("apply_LHEVpt_rwgt");
  const bool apply_l1PreFireWeight = cfg_produceNtuple.getParameter<bool>("apply_l1PreFireWeight");
  const bool isCP5 = cfg_produceNtuple.getParameter<bool>("isCP5");

  const unsigned int numNominalLeptons = cfg_produceNtuple.getParameter<unsigned int>("numNominalLeptons");
  const bool applyNumNominalLeptonsCut = cfg_produceNtuple.getParameter<bool>("applyNumNominalLeptonsCut");
  const unsigned int numNominalHadTaus = cfg_produceNtuple.getParameter<unsigned int>("numNominalHadTaus");
  const bool applyNumNominalHadTausCut = cfg_produceNtuple.getParameter<bool>("applyNumNominalHadTausCut");
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
  
  const bool isDEBUG = cfg_produceNtuple.getParameter<bool>("isDEBUG");

  edm::ParameterSet cfg_dataToMCcorrectionInterface;
  cfg_dataToMCcorrectionInterface.addParameter<std::string>("era", era_string);
  cfg_dataToMCcorrectionInterface.addParameter<std::string>("hadTauSelection_againstJets", hadTauWP_againstJets);
  cfg_dataToMCcorrectionInterface.addParameter<int>("hadTauSelection_againstElectrons", get_tau_id_wp_int(hadTauWP_againstElectrons));
  cfg_dataToMCcorrectionInterface.addParameter<int>("hadTauSelection_againstMuons", get_tau_id_wp_int(hadTauWP_againstMuons));
  cfg_dataToMCcorrectionInterface.addParameter<std::string>("lep_mva_wp", lep_mva_wp);
  cfg_dataToMCcorrectionInterface.addParameter<std::string>("pileupJetID", apply_pileupJetID_str);
  cfg_dataToMCcorrectionInterface.addParameter<bool>("isCP5", isCP5);
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
  int skipEvents = cfg.getParameterSet("fwliteInput").getParameter<int>("skipEvents");
  std::cout << " maxEvents = " << maxEvents << ", skipEvents = " << skipEvents << '\n';
  unsigned reportEvery = inputFiles.reportAfter();

  fwlite::OutputFiles outputFile(cfg);
  fwlite::TFileService fs = fwlite::TFileService(outputFile.file().data());

  TTreeWrapper* inputTree = new TTreeWrapper(treeName.data(), inputFiles.files(), maxEvents);
  std::cout << "Loaded " << inputTree->getFileCount() << " file(s).\n";

  EventReader* eventReader = new EventReader(cfg_produceNtuple);
  inputTree->registerReader(eventReader);

  eventReader->set_tauEScset(dataToMCcorrectionInterface->get_tau_energy_scale_cset());
  if(apply_pileupJetID != pileupJetID::kPileupJetID_disabled)
  {
    dataToMCcorrectionInterface->load_pileupJetID(eventReader->get_JMARcset());
  }

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

  DYMCReweighting * dyReweighting = nullptr;
  if(apply_DYMCReweighting)
  {
    dyReweighting = new DYMCReweighting(era);
  }
  DYMCNormScaleFactors * dyNormScaleFactors = nullptr;
  if(apply_DYMCNormScaleFactors)
  {
    dyNormScaleFactors = new DYMCNormScaleFactors(era);
  }
  LHEVpt_LOtoNLO * lhe_vpt = nullptr;
  if(apply_LHEVpt_rwgt)
  {
    lhe_vpt = new LHEVpt_LOtoNLO(analysisConfig, isDEBUG);
    inputTree->registerReader(lhe_vpt);
  }

  LHEInfoReader* lheInfoReader = nullptr;
  PSWeightReader* psWeightReader = nullptr;
  LHEParticleReader* lheParticleReader = nullptr;
  bool has_PDF_weights = false;
  std::map<std::string, int> pdf_map;
  if ( isMC )
  {
    lheInfoReader = new LHEInfoReader(cfg_produceNtuple);
    has_PDF_weights = lheInfoReader->has_PDF_weights();
    if(has_PDF_weights)
    {
      lheInfoReader->set_pdf_settings(cfg_produceNtuple.getParameter<edm::ParameterSet>("pdfSettings"));
      for(int pdf_member_idx = 0; pdf_member_idx < lheInfoReader->getPdfSize(); ++pdf_member_idx)
      {
        const std::string pdf_member_str = getPDFsys_str(pdf_member_idx);
        pdf_map[pdf_member_str] = pdf_member_idx;
      }
    }
    inputTree->registerReader(lheInfoReader);
    psWeightReader = new PSWeightReader(cfg_produceNtuple);
    inputTree->registerReader(psWeightReader);
    lheParticleReader = new LHEParticleReader(cfg_produceNtuple);
    inputTree->registerReader(lheParticleReader);
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
    // TODO bring out constant portion from this loop and merge with cfg_writer inside the loop
    const std::string pluginType = cfg_writer.getParameter<std::string>("pluginType");
    cfg_writer.addParameter<unsigned int>("numNominalLeptons", numNominalLeptons);
    cfg_writer.addParameter<unsigned int>("numNominalHadTaus", numNominalHadTaus);
    copyParameter<vstring>(cfg_produceNtuple, cfg_writer, "fatJet_corrections");
    cfg_writer.addParameter<std::string>("era", get_era(era));
    cfg_writer.addParameter<bool>("isMC", isMC);
    cfg_writer.addParameter<std::string>("process", process);
    cfg_writer.addParameter<bool>("split_jes", cfg_produceNtuple.getParameter<bool>("split_jes"));
    cfg_writer.addParameter<bool>("l1PreFiringWeightReader", l1PreFiringWeightReader);
    cfg_writer.addParameter<bool>("apply_topPtReweighting", apply_topPtReweighting);
    cfg_writer.addParameter<bool>("has_LHE_weights", lheInfoReader && lheInfoReader->has_LHE_weights());
    cfg_writer.addParameter<bool>("has_PS_weights", psWeightReader && psWeightReader->has_PS_weights());
    cfg_writer.addParameter<bool>("has_PDF_weights", has_PDF_weights);
    cfg_writer.addParameter<bool>("apply_pileupJetID", apply_pileupJetID != pileupJetID::kPileupJetID_disabled);
    cfg_writer.addParameter<bool>("apply_DYMCReweighting", apply_DYMCReweighting);
    cfg_writer.addParameter<bool>("apply_DYMCNormScaleFactors", apply_DYMCNormScaleFactors);
    cfg_writer.addParameter<bool>("apply_LHEVpt_rwgt", apply_LHEVpt_rwgt);
    cfg_writer.addParameter<bool>("apply_EWK_corrections", analysisConfig.isMC_EWK());
    if(has_PDF_weights)
    {
      cfg_writer.addParameter<int>("nof_PDF_members", lheInfoReader->saveAllPdfMembers() ? lheInfoReader->getPdfSize() : 0);
    }
    std::unique_ptr<WriterBase> writer = WriterPluginFactory::get()->create(pluginType, cfg_writer);
    writer->registerReaders(inputTree);
    writer->setBranches(outputTree);
    writers.push_back(std::move(writer));
  }

  std::vector<std::string> systematic_shifts;
  // CV: process all systematic uncertainties supported by any plugin that writes branches to "plain" Ntuple (only for MC)
  // TODO fake rates (and potentiall charge flip rates) do have systematic uncertainties
  if ( isMC )
  {
    for ( auto & writer : writers )
    {
      merge_systematic_shifts(systematic_shifts, writer->get_supported_systematics(cfg_produceNtuple)); // TODO passing cfg here should have no effect b/c branches already created
    }
  }
  // CV: add central value (for data and MC)
  merge_systematic_shifts(systematic_shifts, { "central" });

  const std::vector<std::string> inclusive_systematic_shifts = get_inclusive_systeatics(systematic_shifts);
  assert(contains(inclusive_systematic_shifts, "central"));
  std::cout << "Processing systematic uncertainties = " << format_vstring(systematic_shifts) << '\n';

  // keep inclusive systematics together, split the rest
  std::vector<std::vector<std::string>> sytematics_split { inclusive_systematic_shifts };
  for(const std::string & central_or_shift : systematic_shifts)
  {
    if(! contains(inclusive_systematic_shifts, central_or_shift))
    {
      sytematics_split.push_back({ central_or_shift });
    }
  }

  int analyzedEntries = 0;
  TH1* histogram_analyzedEntries = fs.make<TH1D>("analyzedEntries", "analyzedEntries", 1, -0.5, +0.5);
  while ( inputTree->hasNextEvent() && (!run_lumi_eventSelector || (run_lumi_eventSelector && !run_lumi_eventSelector->areWeDone())) )
  {
    bool skipEvent = false;
    const RunLumiEvent & runLumiEvent = eventReader->read_runLumiEvent();
    if (( run_lumi_eventSelector && !(*run_lumi_eventSelector)(runLumiEvent) ) ||
        ( skipEvents > 0 && analyzedEntries <= skipEvents ))
    {
      skipEvent = true;
    }
    for ( const std::vector<std::string> & central_or_shift : sytematics_split )
    {
      if(skipEvent)
      {
        // break out of the systematics loop
        break;
      }
      const bool has_central = contains(central_or_shift, "central");
      std::string default_systematics;
      if ( has_central )
      {
        if ( inputTree->canReport(reportEvery) )
        {
          const auto memory_consumption = log_memory();
          std::cout << "processing Entry " << inputTree->getCurrentMaxEventIdx()
                    << " or " << inputTree->getCurrentEventIdx() << " entry in #"
                    << (inputTree->getProcessedFileCount() - 1)
                    << " (" << runLumiEvent
                    << ") file; "
                       "VSIZE = " << display_memory(memory_consumption.first) << ", "
                       "RSS = " << display_memory(memory_consumption.second) << '\n';
          ;
        }
        ++analyzedEntries;
        histogram_analyzedEntries->Fill(0.);
        default_systematics = "central";
      }
      else
      {
        assert(central_or_shift.size() == 1); // shifting or smearing energy scales
        default_systematics = central_or_shift.at(0);
      }
      if ( isDEBUG || run_lumi_eventSelector )
      {
        std::cout << "Processing central_or_shift(s) = " << format_vstring(central_or_shift) << '\n';
      }
      if ( has_central && (isDEBUG || run_lumi_eventSelector) )
      {
        std::cout << "processing Entry " << inputTree->getCurrentMaxEventIdx() << ": " << runLumiEvent << '\n';
        if ( inputTree->isOpen() )
        {
          std::cout << "input File = " << inputTree->getCurrentFileName() << '\n';
        }
      }

      eventReader->set_central_or_shift(default_systematics);
      const Event& event = eventReader->read();
      // CV: skip processing events that don't contain the nominal number of leptons and hadronic taus,
      //     if the flags applyNumNominalLeptonsCut and applyNumNominalHadTausCut are enabled in the config file
      if ( event.isInvalid() )
      {
        skipEvent = true;
        continue;
      }

      EvtWeightRecorder evtWeightRecorder(central_or_shift, default_systematics, isMC);
      if ( isMC )
      {
        if ( apply_genWeight         ) evtWeightRecorder.record_genWeight(event.eventInfo());
        if ( eventWeightManager      ) evtWeightRecorder.record_auxWeight(eventWeightManager);
        if ( l1PreFiringWeightReader ) evtWeightRecorder.record_l1PrefireWeight(l1PreFiringWeightReader);
        if ( apply_topPtReweighting  ) evtWeightRecorder.record_toppt_rwgt(event.topPtRwgtSF());
        if ( apply_LHEVpt_rwgt       ) evtWeightRecorder.record_LHEVpt(lhe_vpt);
        if ( dyReweighting           ) evtWeightRecorder.record_dy_rwgt(dyReweighting, event.genParticles());
        if ( dyNormScaleFactors )
        {
          evtWeightRecorder.record_dy_norm(
            dyNormScaleFactors,
            event.genParticles(),
            event.selJetsAK4().size(),
            event.selJetsAK4_btagLoose().size(),
            event.selJetsAK4_btagMedium().size()
          );
        }
        lheInfoReader->read();
        psWeightReader->read();
        evtWeightRecorder.record_lheScaleWeight(lheInfoReader);
        evtWeightRecorder.record_pdfWeight(lheInfoReader);
        evtWeightRecorder.record_pdfMembers(lheInfoReader, pdf_map);
        evtWeightRecorder.record_psWeight(psWeightReader);
        if ( analysisConfig.isHH_rwgt_allowed() )
        {
          const LHEParticleCollection lheParticles = lheParticleReader->read();
          evtWeightRecorder.record_gen_mHH_cosThetaStar(lheParticles);
        }
        evtWeightRecorder.record_puWeight(dataToMCcorrectionInterface, &event.eventInfo());
        evtWeightRecorder.record_nom_tH_weight(&event.eventInfo());
        evtWeightRecorder.record_lumiScale(lumiScale);

        evtWeightRecorder.record_btagWeight(dataToMCcorrectionInterface, event.selJetsAK4());
        if(apply_pileupJetID != pileupJetID::kPileupJetID_disabled)
        {
          evtWeightRecorder.record_pileupJetIDSF(dataToMCcorrectionInterface, event.selJetsAK4());
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
        for(const std::string & sys_shift: central_or_shift)
        {
          writer->set_central_or_shift(sys_shift);
          writer->write(event, evtWeightRecorder);
        }
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
  delete dyReweighting;
  delete psWeightReader;
  delete lheParticleReader;

  delete dataToMCcorrectionInterface;
  delete jetToLeptonFakeRateInterface;
  delete jetToHadTauFakeRateInterface;

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
