
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
#include "PhysicsTools/FWLite/interface/TFileService.h"                                         // fwlite::TFileService

#include "TallinnNtupleProducer/CommonTools/interface/cmsException.h"                           // cmsException
#include "TallinnNtupleProducer/CommonTools/interface/Era.h"                                    // Era, get_era()
#include "TallinnNtupleProducer/CommonTools/interface/tH_auxFunctions.h"                        // get_tH_SM_str()
#include "TallinnNtupleProducer/EvtWeightTools/interface/BtagSFRatioInterface.h"                // BtagSFRatioInterface
#include "TallinnNtupleProducer/EvtWeightTools/interface/ChargeMisIdRateInterface.h"            // ChargeMisIdRateInterface
#include "TallinnNtupleProducer/EvtWeightTools/interface/Data_to_MC_CorrectionInterface_2016.h" // Data_to_MC_CorrectionInterface_2016
#include "TallinnNtupleProducer/EvtWeightTools/interface/Data_to_MC_CorrectionInterface_2017.h" // Data_to_MC_CorrectionInterface_2017
#include "TallinnNtupleProducer/EvtWeightTools/interface/Data_to_MC_CorrectionInterface_2018.h" // Data_to_MC_CorrectionInterface_2018
#include "TallinnNtupleProducer/EvtWeightTools/interface/EvtWeightRecorder.h"                   // EvtWeightRecorder
#include "TallinnNtupleProducer/EvtWeightTools/interface/HadTauFakeRateInterface.h"             // HadTauFakeRateInterface
#include "TallinnNtupleProducer/EvtWeightTools/interface/HHWeightInterfaceCouplings.h"          // HHWeightInterfaceCouplings
#include "TallinnNtupleProducer/EvtWeightTools/interface/HHWeightInterfaceLO.h"                 // HHWeightInterfaceLO
#include "TallinnNtupleProducer/EvtWeightTools/interface/HHWeightInterfaceNLO.h"                // HHWeightInterfaceNLO
#include "TallinnNtupleProducer/EvtWeightTools/interface/LeptonFakeRateInterface.h"             // LeptonFakeRateInterface
#include "TallinnNtupleProducer/Framework/interface/GenMatchInterface.h"                        // GenMatchInterface
#include "TallinnNtupleProducer/Framework/interface/TTreeWrapper.h"                             // TTreeWrapper
#include "TallinnNtupleProducer/Objects/interface/Event.h"                                      // Event
#include "TallinnNtupleProducer/Objects/interface/EventInfo.h"                                  // EventInfo
#include "TallinnNtupleProducer/Objects/interface/GenHadTau.h"                                  // GenHadTau
#include "TallinnNtupleProducer/Objects/interface/GenJet.h"                                     // GenJet
#include "TallinnNtupleProducer/Objects/interface/GenLepton.h"                                  // GenLepton
#include "TallinnNtupleProducer/Objects/interface/GenPhoton.h"                                  // GenPhoton
#include "TallinnNtupleProducer/Objects/interface/TriggerInfo.h"                                // TriggerInfo
#include "TallinnNtupleProducer/Readers/interface/EventReader.h"                                // EventReader
#include "TallinnNtupleProducer/Readers/interface/GenHadTauReader.h"                            // GenHadTauReader
#include "TallinnNtupleProducer/Readers/interface/GenJetReader.h"                               // GenJetReader
#include "TallinnNtupleProducer/Readers/interface/GenLeptonReader.h"                            // GenLeptonReader
#include "TallinnNtupleProducer/Readers/interface/GenParticleReader.h"                          // GenParticleReader
#include "TallinnNtupleProducer/Readers/interface/GenPhotonReader.h"                            // GenPhotonReader
#include "TallinnNtupleProducer/Readers/interface/L1PreFiringWeightReader.h"                    // L1PreFiringWeightReader
#include "TallinnNtupleProducer/Readers/interface/LHEInfoReader.h"                              // LHEInfoReader
#include "TallinnNtupleProducer/Readers/interface/PSWeightReader.h"                             // PSWeightReader
#include "TallinnNtupleProducer/Selectors/interface/RunLumiEventSelector.h"                     // RunLumiEventSelector
#include "TallinnNtupleProducer/Writers/interface/WriterBase.h"                                 // WriterBase, WriterPluginFactory

#include <TBenchmark.h>                                                                         // TBenchmark
#include <TError.h>                                                                             // gErrorAbortLevel, kError
#include <TString.h>                                                                            // TString, Form()
     
#include <boost/algorithm/string/replace.hpp>                                                   // boost::replace_all_copy()
#include <boost/algorithm/string/predicate.hpp>                                                 // boost::starts_with()

#include <assert.h>                                                                             // assert
#include <cstdlib>                                                                              // EXIT_SUCCESS, EXIT_FAILURE
#include <fstream>                                                                              // std::ofstream
#include <iostream>                                                                             // std::cerr, std::fixed
#include <iomanip>                                                                              // std::setprecision(), std::setw()
#include <string>                                                                               // std::string
#include <vector>                                                                               // std::vector

typedef std::vector<std::string> vstring;

/**
 * @brief Produce "plain" Ntuple, which is used for final event selection & histogram filling.
 */
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
  if ( !edm::readPSetsFrom(argv[1])->existsAs<edm::ParameterSet>("process") )
    throw cmsException("produceNtuple", __LINE__) << "No ParameterSet 'process' found in configuration file = " << argv[1] << " !!";

  edm::ParameterSet cfg = edm::readPSetsFrom(argv[1])->getParameter<edm::ParameterSet>("process");

  edm::ParameterSet cfg_produceNtuple = cfg.getParameter<edm::ParameterSet>("produceNtuple");
  AnalysisConfig analysisConfig("produceNtuple", produceNtuple);

  std::string treeName = cfg_produceNtuple.getParameter<std::string>("treeName");

  std::string process_string = cfg_produceNtuple.getParameter<std::string>("process");
  const bool isMC_tH     = analysisConfig.isMC_tH();
  const bool isMC_VH     = analysisConfig.isMC_VH();
  const bool isMC_H      = analysisConfig.isMC_H();
  const bool isMC_HH     = analysisConfig.isMC_HH();
  const bool isMC_EWK    = analysisConfig.isMC_EWK();
  const bool isMC_signal = analysisConfig.isMC_ttH();
  const bool isSignal    = isMC_signal || isMC_tH || isMC_VH || isMC_HH || isMC_H;

  std::string era_string = cfg_produceNtuple.getParameter<std::string>("era");
  const Era era = get_era(era_string);

  bool isMC = cfg_produceNtuple.getParameter<bool>("isMC");
  edm::VParameterSet lumiScale = cfg_produceNtuple.getParameter<edm::VParameterSet>("lumiScale");
  bool apply_genWeight = cfg_produceNtuple.getParameter<bool>("apply_genWeight");
  bool hasLHE = cfg_produceNtuple.getParameter<bool>("hasLHE");
  bool hasPS = cfg_produceNtuple.getParameter<bool>("hasPS");
  bool apply_LHE_nom = cfg_produceNtuple.getParameter<bool>("apply_LHE_nom");
  std::string apply_topPtReweighting_str = cfg_produceNtuple.getParameter<std::string>("apply_topPtReweighting");
  bool apply_topPtReweighting = ! apply_topPtReweighting_str.empty();
  bool apply_l1PreFireWeight = cfg_produceNtuple.getParameter<bool>("apply_l1PreFireWeight");
  bool apply_btagSFRatio = cfg_produceNtuple.getParameter<bool>("applyBtagSFRatio");

  bool isDEBUG = cfg_analyze.getParameter<bool>("isDEBUG");

  std::vector<std::string> systematic_shifts = EventReader::get_supported_systematics();
  // CV: add central value 
  systematic_shifts.insert("central");

  edm::ParameterSet cfg_dataToMCcorrectionInterface;
  cfg_dataToMCcorrectionInterface.addParameter<std::string>("era", era_string);
  cfg_dataToMCcorrectionInterface.addParameter<std::string>("hadTauSelection", hadTauSelection_part2);
  cfg_dataToMCcorrectionInterface.addParameter<int>("hadTauSelection_antiElectron", hadTauSelection_antiElectron);
  cfg_dataToMCcorrectionInterface.addParameter<int>("hadTauSelection_antiMuon", hadTauSelection_antiMuon);
  cfg_dataToMCcorrectionInterface.addParameter<std::string>("lep_mva_wp", lep_mva_wp);
  cfg_dataToMCcorrectionInterface.addParameter<bool>("isDEBUG", isDEBUG);
  Data_to_MC_CorrectionInterface_Base * dataToMCcorrectionInterface = nullptr;
  switch(era)
  {
    case Era::k2016: dataToMCcorrectionInterface = new Data_to_MC_CorrectionInterface_2016(cfg_dataToMCcorrectionInterface); break;
    case Era::k2017: dataToMCcorrectionInterface = new Data_to_MC_CorrectionInterface_2017(cfg_dataToMCcorrectionInterface); break;
    case Era::k2018: dataToMCcorrectionInterface = new Data_to_MC_CorrectionInterface_2018(cfg_dataToMCcorrectionInterface); break;
    default: throw cmsException("produceNtuple", __LINE__) << "Invalid era = " << static_cast<int>(era);
  }
  const ChargeMisIdRate chargeMisIdRate(era);

  edm::ParameterSet cfg_leptonFakeRateWeight = cfg_produceNtuple.getParameter<edm::ParameterSet>("leptonFakeRateWeight");
  cfg_leptonFakeRateWeight.addParameter<std::string>("era", era_string);
  LeptonFakeRateInterface* leptonFakeRateInterface = new LeptonFakeRateInterface(cfg_leptonFakeRateWeight);

  edm::ParameterSet cfg_hadTauFakeRateWeight = cfg_produceNtuple.getParameter<edm::ParameterSet>("hadTauFakeRateWeight");
  cfg_hadTauFakeRateWeight.addParameter<std::string>("hadTauSelection", hadTauSelection_part2);
  HadTauFakeRateInterface* hadTauFakeRateInterface = new HadTauFakeRateInterface(cfg_hadTauFakeRateWeight);

  bool redoGenMatching = cfg_analyze.getParameter<bool>("redoGenMatching");
  bool genMatchingByIndex = cfg_analyze.getParameter<bool>("genMatchingByIndex");

  std::string selEventsFileName_input = cfg_analyze.getParameter<std::string>("selEventsFileName_input");
  std::cout << "selEventsFileName_input = " << selEventsFileName_input << std::endl;
  RunLumiEventSelector* run_lumi_eventSelector = 0;
  if ( selEventsFileName_input != "" ) {
    edm::ParameterSet cfgRunLumiEventSelector;
    cfgRunLumiEventSelector.addParameter<std::string>("inputFileName", selEventsFileName_input);
    cfgRunLumiEventSelector.addParameter<std::string>("separator", ":");
    run_lumi_eventSelector = new RunLumiEventSelector(cfgRunLumiEventSelector);
  }

  std::string selEventsFileName_output = cfg_analyze.getParameter<std::string>("selEventsFileName_output");

  fwlite::InputSource inputFiles(cfg);
  int maxEvents = inputFiles.maxEvents();
  std::cout << " maxEvents = " << maxEvents << std::endl;
  unsigned reportEvery = inputFiles.reportAfter();

  fwlite::OutputFiles outputFile(cfg);
  fwlite::TFileService fs = fwlite::TFileService(outputFile.file().data());

  TTreeWrapper * inputTree = new TTreeWrapper(treeName.data(), inputFiles.files(), maxEvents);

  std::cout << "Loaded " << inputTree -> getFileCount() << " file(s).\n";

//--- declare event-level variables
  EventInfo eventInfo(analysisConfig);
  if ( isMC )
  {
    const double ref_genWeight = cfg_analyze.getParameter<double>("ref_genWeight");
    eventInfo.set_refGetWeight(ref_genWeight);
  }
  const std::string default_cat_str = "default";
  std::vector<std::string> evt_cat_strs = { default_cat_str };
  const std::vector<std::pair<std::string, int>> evt_htxs_binning = get_htxs_binning(isMC_signal);
  eventInfo.read_htxs(!evt_htxs_binning.empty());

//--- prepare HH reweighting/coupling scan
  const edm::ParameterSet hhWeight_cfg = cfg_analyze.getParameterSet("hhWeight_cfg");
  const bool apply_HH_rwgt_lo = analysisConfig.isHH_rwgt_allowed() && hhWeight_cfg.getParameter<bool>("apply_rwgt_lo");
  const bool apply_HH_rwgt_nlo = analysisConfig.isHH_rwgt_allowed() && hhWeight_cfg.getParameter<bool>("apply_rwgt_nlo");
  const HHWeightInterfaceCouplings * hhWeight_couplings = nullptr;
  const HHWeightInterfaceLO * HHWeightLO_calc = nullptr;
  const HHWeightInterfaceNLO * HHWeightNLO_calc = nullptr;
  if ( apply_HH_rwgt_lo || apply_HH_rwgt_nlo )
  {
    hhWeight_couplings = new HHWeightInterfaceCouplings(hhWeight_cfg);

    if ( apply_HH_rwgt_lo )
    {
      HHWeightLO_calc = new HHWeightInterfaceLO(hhWeight_couplings, hhWeight_cfg);
      evt_cat_strs = hhWeight_couplings->get_bm_names();
    }

    if ( apply_HH_rwgt_nlo )
    {
      HHWeightNLO_calc = new HHWeightInterfaceNLO(hhWeight_couplings, era);
    }
  }

  const std::vector<edm::ParameterSet> tHweights = cfg_analyze.getParameterSetVector("tHweights");
  if ( (isMC_tH || isMC_signal) && ! tHweights.empty() )
  {
    eventInfo.set_central_or_shift(central_or_shift_main);
    eventInfo.loadWeight_tH(tHweights);
    const std::vector<std::string> evt_cat_tH_strs = eventInfo.getWeight_tH_str(central_or_shift_main);
    evt_cat_strs.insert(evt_cat_strs.end(), evt_cat_tH_strs.begin(), evt_cat_tH_strs.end());
  }
  EventInfoReader eventInfoReader(&eventInfo);
  if ( apply_topPtReweighting )
  {
    eventInfoReader.setTopPtRwgtBranchName(apply_topPtReweighting_str);
  }
  inputTree->registerReader(&eventInfoReader);

  L1PreFiringWeightReader* l1PreFiringWeightReader = nullptr;
  if ( apply_l1PreFireWeight )
  {
    l1PreFiringWeightReader = new L1PreFiringWeightReader(era);
    inputTree->registerReader(l1PreFiringWeightReader);
  }

  BtagSFRatioInterface * btagSFRatioInterface = nullptr;
  if(apply_btagSFRatio)
  {
    const edm::ParameterSet btagSFRatio = cfg_analyze.getParameterSet("btagSFRatio");
    btagSFRatioInterface = new BtagSFRatioInterface(btagSFRatio);
  }

//--- open output file containing run:lumi:event numbers of events passing final event selection criteria
  std::ostream* selEventsFile = ( selEventsFileName_output != "" ) ? new std::ofstream(selEventsFileName_output.data(), std::ios::out) : 0;
  std::cout << "selEventsFileName_output = " << selEventsFileName_output << std::endl;

  int analyzedEntries = 0;
  int selectedEntries = 0;
  double selectedEntries_weighted = 0.;
  TH1* histogram_analyzedEntries = fs.make<TH1D>("analyzedEntries", "analyzedEntries", 1, -0.5, +0.5);
  TH1* histogram_selectedEntries = fs.make<TH1D>("selectedEntries", "selectedEntries", 1, -0.5, +0.5);
  while ( inputTree->hasNextEvent() && (!run_lumi_eventSelector || (run_lumi_eventSelector && !run_lumi_eventSelector->areWeDone())) )
  {
    if ( inputTree->canReport(reportEvery) )
    {
      std::cout << "processing Entry " << inputTree->getCurrentMaxEventIdx()
                << " or " << inputTree->getCurrentEventIdx() << " entry in #"
                << (inputTree->getProcessedFileCount() - 1)
                << " (" << eventInfo
                << ") file (" << selectedEntries << " Entries selected)\n";
    }
    ++analyzedEntries;
    histogram_analyzedEntries->Fill(0.);

    if ( run_lumi_eventSelector && !(*run_lumi_eventSelector)(eventInfo) )
    {
      continue;
    }

    EvtWeightRecorder evtWeightRecorder(central_or_shifts_local, central_or_shift_main, isMC);

    if ( isDEBUG ) {
      std::cout << "event #" << inputTree -> getCurrentMaxEventIdx() << ' ' << eventInfo << '\n';
    }

    if ( run_lumi_eventSelector )
    {
      std::cout << "processing Entry " << inputTree->getCurrentMaxEventIdx() << ": " << eventInfo << '\n';
      if ( inputTree->isOpen() )
      {
        std::cout << "input File = " << inputTree->getCurrentFileName() << '\n';
      }
    }

    if ( isMC )
    {
      if ( apply_genWeight         ) evtWeightRecorder.record_genWeight(eventInfo);
      if ( eventWeightManager      ) evtWeightRecorder.record_auxWeight(eventWeightManager);
      if ( l1PreFiringWeightReader ) evtWeightRecorder.record_l1PrefireWeight(l1PreFiringWeightReader);
      if ( apply_topPtReweighting  ) evtWeightRecorder.record_toppt_rwgt(eventInfo.topPtRwgtSF);
      lheInfoReader->read();
      psWeightReader->read();
      evtWeightRecorder.record_lheScaleWeight(lheInfoReader);
      evtWeightRecorder.record_psWeight(psWeightReader);
      evtWeightRecorder.record_puWeight(&eventInfo);
      evtWeightRecorder.record_nom_tH_weight(&eventInfo);
      evtWeightRecorder.record_lumiScale(lumiScale);

//--- compute event-level weight for data/MC correction of b-tagging efficiency and mistag rate
//   (using the method "Event reweighting using scale factors calculated with a tag and probe method",
//    described on the BTV POG twiki https://twiki.cern.ch/twiki/bin/view/CMS/BTagShapeCalibration )
      evtWeightRecorder.record_btagWeight(selJets);
      if(btagSFRatioFacility)
      {
        evtWeightRecorder.record_btagSFRatio(btagSFRatioFacility, selJets.size());
      }

      if(isMC_EWK)
      {
        evtWeightRecorder.record_ewk_jet(selJets);
        evtWeightRecorder.record_ewk_bjet(selBJets_medium);
      }

      dataToMCcorrectionInterface->setLeptons({ selLepton_lead, selLepton_sublead }, true);

//--- apply data/MC corrections for trigger efficiency
      evtWeightRecorder.record_leptonTriggerEff(dataToMCcorrectionInterface);

//--- apply data/MC corrections for efficiencies for lepton to pass loose identification and isolation criteria
      evtWeightRecorder.record_leptonIDSF_recoToLoose(dataToMCcorrectionInterface);

//--- apply data/MC corrections for efficiencies of leptons passing the loose identification and isolation criteria
//    to also pass the tight identification and isolation criteria
      if(electronSelection >= kFakeable && muonSelection >= kFakeable)
      {
        // apply looseToTight SF to leptons matched to generator-level prompt leptons and passing Tight selection conditions
        evtWeightRecorder.record_leptonIDSF_looseToTight(dataToMCcorrectionInterface, false);
      }

//--- apply data/MC corrections for hadronic tau identification efficiency
//    and for e->tau and mu->tau misidentification rates
      dataToMCcorrectionInterface->setHadTaus({ selHadTau });
      evtWeightRecorder.record_hadTauID_and_Iso(dataToMCcorrectionInterface);
      evtWeightRecorder.record_eToTauFakeRate(dataToMCcorrectionInterface);
      evtWeightRecorder.record_muToTauFakeRate(dataToMCcorrectionInterface);
    }

    bool passesTight_lepton_lead = isMatched(*selLepton_lead, tightElectrons) || isMatched(*selLepton_lead, tightMuons);
    bool passesTight_lepton_sublead = isMatched(*selLepton_sublead, tightElectrons) || isMatched(*selLepton_sublead, tightMuons);
    bool passesTight_hadTau = isMatched(*selHadTau, tightHadTausFull);

    if(leptonFakeRateInterface)
    {
      evtWeightRecorder.record_jetToLepton_FR_lead(leptonFakeRateInterface, selLepton_lead);
      evtWeightRecorder.record_jetToLepton_FR_sublead(leptonFakeRateInterface, selLepton_sublead);
    }
    if(jetToTauFakeRateInterface)
    {
      evtWeightRecorder.record_jetToTau_FR_lead(jetToTauFakeRateInterface, selHadTau);
    }

    if(! selectBDT)
    {
      if(applyFakeRateWeights == kFR_3L)
      {
        evtWeightRecorder.compute_FR_2l1tau(passesTight_lepton_lead, passesTight_lepton_sublead, passesTight_hadTau);
      }
      else if(applyFakeRateWeights == kFR_2lepton)
      {
        evtWeightRecorder.compute_FR_2l(passesTight_lepton_lead, passesTight_lepton_sublead);
      }
      else if (applyFakeRateWeights == kFR_1tau)
      {
        evtWeightRecorder.compute_FR_1tau(passesTight_hadTau);
      }
    }

    // CV: apply data/MC ratio for jet->tau fake-rates in case data-driven "fake" background estimation is applied to leptons only
    if(isMC && apply_hadTauFakeRateSF && hadTauSelection == kTight && !(selHadTau->genHadTau() || selHadTau->genLepton()))
    {
      evtWeightRecorder.record_jetToTau_SF_lead(jetToTauFakeRateInterface, selHadTau);
    }

    if ( leptonChargeSelection == kOS ) {
      double prob_chargeMisId_lead = chargeMisIdRate.get(selLepton_lead);
      double prob_chargeMisId_sublead = chargeMisIdRate.get(selLepton_sublead);

      double prob_chargeMisId_applied = 1.;
      if ( apply_lepton_and_hadTauCharge_cut ) {
        if ( chargeSumSelection == kOS ) {
          // CV: apply charge misidentification probability to lepton of same charge as hadronic tau
          //    (if the lepton of charge opposite to the charge of the hadronic tau "flips",
          //     the event has sum of charges equal to three and fails "lepton+tau charge" cut)
          if ( selLepton_lead->charge()*selHadTau->charge()    > 0 ) prob_chargeMisId_applied *= prob_chargeMisId_lead;
          if ( selLepton_sublead->charge()*selHadTau->charge() > 0 ) prob_chargeMisId_applied *= prob_chargeMisId_sublead;
        } else if ( chargeSumSelection == kSS ) {
          // CV: apply charge misidentification probability to lepton of opposite charge as hadronic tau
          //    (if the lepton of same charge as the hadronic tau "flips",
          //     the event has sum of charges equal to one and fails "lepton+tau charge" cut)
          if ( selLepton_lead->charge()*selHadTau->charge()    < 0 ) prob_chargeMisId_applied *= prob_chargeMisId_lead;
          if ( selLepton_sublead->charge()*selHadTau->charge() < 0 ) prob_chargeMisId_applied *= prob_chargeMisId_sublead;
        } else assert(0);
      } else {
        prob_chargeMisId_applied *= (prob_chargeMisId_lead + prob_chargeMisId_sublead);
      }
      // Karl: reject the event, if the applied probability of charge misidentification is 0. This can happen only if
      //       1) both selected leptons are muons (their misId prob is 0).
      //       2) one lepton is a muon and the other is an electron, and the muon has the same sign as the selected tau.
      if(prob_chargeMisId_applied == 0.)
      {
        if(run_lumi_eventSelector)
        {
          std::cout << "event " << eventInfo.str() << " FAILS charge flip selection\n"
                       "(leading lepton charge (pdgId) = " << selLepton_lead->charge() << " (" << selLepton_lead->pdgId()
                    << ") => misId prob = " << prob_chargeMisId_lead << "; "
                       "subleading lepton charge (pdgId) = " << selLepton_sublead->charge() << " (" << selLepton_sublead->pdgId()
                    << ") => misId prob = " << prob_chargeMisId_sublead << "); "
                       "tau charge = " << selHadTau->charge() << ")\n"
          ;
        }
        continue;
      }
      evtWeightRecorder.record_chargeMisIdProb(prob_chargeMisId_applied);
    }

    std::map<std::string, double> Weight_ktScan; // weights to do histograms for BMs
    if(apply_HH_rwgt_lo)
    {
      assert(HHWeightLO_calc);
      evtWeightRecorder.record_hhWeight_lo(HHWeightLO_calc, eventInfo, isDEBUG);
      for(const std::string & HHWeightName: evt_cat_strs)
      {
        Weight_ktScan[HHWeightName] = HHWeightLO_calc->getRelativeWeight(HHWeightName, eventInfo.gen_mHH, eventInfo.gen_cosThetaStar, isDEBUG);
      }
    }
    if(apply_HH_rwgt_nlo)
    {
      assert(HHWeightNLO_calc);
      evtWeightRecorder.record_hhWeight_nlo(HHWeightNLO_calc, eventInfo, isDEBUG);
      for(const std::string & HHWeightName: evt_cat_strs)
      {
        Weight_ktScan[HHWeightName] *= HHWeightNLO_calc->getRelativeWeight_LOtoNLO(HHWeightName, eventInfo.gen_mHH, eventInfo.gen_cosThetaStar, isDEBUG);
      }
    }

        const std::string central_or_shift_tH = eventInfo.has_central_or_shift(central_or_shift) ? central_or_shift : central_or_shift_main;
        const double evtWeight_tH_nom = evtWeightRecorder.get_nom_tH_weight(central_or_shift_tH);
        for(const std::string & evt_cat_str: evt_cat_strs)
        {
          if(skipFilling && evt_cat_str != default_cat_str)
          {
            continue;
          }
	  if(isMC_tH)
          {
            const std::string evt_cat_str_query = evt_cat_str == default_cat_str ? get_tH_SM_str() : evt_cat_str;
            tH_weight_map[evt_cat_str] = evtWeight / evtWeight_tH_nom * eventInfo.genWeight_tH(central_or_shift_tH, evt_cat_str_query);
          }
          else if(apply_HH_rwgt_lo)
          {
            tH_weight_map[evt_cat_str] = evtWeight * Weight_ktScan[evt_cat_str];
          }
          else
          {
            tH_weight_map[evt_cat_str] = evtWeight;
          }
        }
    }

TO-DO: LOOP OVER WRITER PLUGINS


  }

TO-DO: APPLY LOOSE EVENT SELECTION ON PLAIN NTUPLE

    ++selectedEntries;
    selectedEntries_weighted += evtWeightRecorder.get(central_or_shift_main);
    histogram_selectedEntries->Fill(0.);

  if ( plainNtuple )
  {
    plainNtuple->write();
  }

  std::cout << "max num. Entries = " << inputTree->getCumulativeMaxEventCount()
            << " (limited by " << maxEvents << ") processed in "
            << inputTree->getProcessedFileCount() << " file(s) (out of "
            << inputTree->getFileCount() << ")\n"
            << " analyzed = " << analyzedEntries << '\n'
            << " selected = " << selectedEntries << " (weighted = " << selectedEntries_weighted << ")" << std::endl;

//--- memory clean-up
  delete dataToMCcorrectionInterface;

  delete leptonFakeRateInterface;
  delete jetToTauFakeRateInterface;

  delete run_lumi_eventSelector;

  delete selEventsFile;

  delete muonReader;
  delete electronReader;
  delete hadTauReader;
  delete jetReader;
  delete metReader;
  delete metFilterReader;
  delete memReader;
  delete genLeptonReader;
  delete genHadTauReader;
  delete genPhotonReader;
  delete genJetReader;
  delete lheInfoReader;
  delete psWeightReader;

  delete l1PreFiringWeightReader;
  delete cutFlowHistManager;
  delete eventWeightManager;

  delete inputTree;
  delete plainNtuple;

  clock.Show("prodNtuple");

  return EXIT_SUCCESS;
}
