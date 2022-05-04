import FWCore.ParameterSet.Config as cms

from TallinnNtupleProducer.Framework.hhWeights_cfi import hhWeights as config_hhWeights
from TallinnNtupleProducer.Framework.recommendedMEtFilters_cfi import recommendedMEtFilters_2016 as config_recommendedMEtFilters_2016, recommendedMEtFilters_2017 as config_recommendedMEtFilters_2017, recommendedMEtFilters_2018 as config_recommendedMEtFilters_2018
from TallinnNtupleProducer.Framework.triggers_cfi import triggers_2016 as config_triggers_2016, triggers_2017 as config_triggers_2017, triggers_2018 as config_triggers_2018

from TallinnNtupleProducer.Writers.EvtWeightWriter_cfi import evtWeight as writers_evtWeight
from TallinnNtupleProducer.Writers.GenPhotonFilterWriter_cfi import genPhotonFilter as writers_genPhotonFilter
from TallinnNtupleProducer.Writers.HtoZZto4lVetoWriter_cfi import HtoZZto4lVeto as writers_HtoZZto4lVeto
from TallinnNtupleProducer.Writers.LowMassLeptonPairVetoWriter_cfi import lowMassLeptonPairVeto as writers_lowMassLeptonPairVeto
from TallinnNtupleProducer.Writers.MEtFilterWriter_cfi import metFilters as writers_metFilters
from TallinnNtupleProducer.Writers.ProcessWriter_cfi import process as writers_process
from TallinnNtupleProducer.Writers.RecoHadTauWriter_cfi import fakeableHadTaus as writers_fakeableHadTaus
from TallinnNtupleProducer.Writers.RecoJetWriterAK4_cfi import selJetsAK4 as writers_selJetsAK4, selJetsAK4_btagLoose as writers_selJetsAK4_btagLoose, selJetsAK4_btagMedium as writers_selJetsAK4_btagMedium
from TallinnNtupleProducer.Writers.RecoLeptonWriter_cfi import fakeableLeptons as writers_fakeableLeptons
from TallinnNtupleProducer.Writers.RecoMEtWriter_cfi import met as writers_met
from TallinnNtupleProducer.Writers.RunLumiEventWriter_cfi import run_lumi_event as writers_run_lumi_event
from TallinnNtupleProducer.Writers.TriggerInfoWriter_cfi import triggerInfo as writers_triggerInfo
from TallinnNtupleProducer.Writers.ZbosonMassVetoWriter_cfi import ZbosonMassVeto as writers_ZbosonMassVeto

process = cms.PSet()

process.fwliteInput = cms.PSet(
    fileNames = cms.vstring(),
    maxEvents = cms.int32(10000),
    outputEvery = cms.uint32(1000)
)

process.fwliteOutput = cms.PSet(
    fileName = cms.string('')
)

process.produceNtuple = cms.PSet(
    treeName = cms.string('Events'),

    process = cms.string(''),
    era = cms.string(''),

    # number of "nominal" leptons (electrons and muons) and hadronic taus
    numNominalLeptons = cms.uint32(2),
    numNominalHadTaus = cms.uint32(1),

    # selection of electrons, muons, and hadronic taus
    lep_mva_cut_mu = cms.double(0.5),
    lep_mva_cut_e = cms.double(0.3),
    lep_mva_wp = cms.string('hh_multilepton'),
    hadTauWP_againstJets_tight = cms.string("deepVSjMedium"),
    hadTauWP_againstJets_fakeable = cms.string("deepVSjVVLoose"),
    hadTauWP_againstElectrons = cms.string(""),
    hadTauWP_againstMuons = cms.string(""),

    # fake rates for electrons, muons, and hadronic taus
    applyFakeRateWeights = cms.string(""),
    leptonFakeRateWeight = cms.PSet(
        inputFileName = cms.string(""),
        histogramName_e = cms.string(""),
        histogramName_mu = cms.string(""),
        era = cms.string(""),
        applyNonClosureCorrection = cms.bool(False),
    ),
    hadTauFakeRateWeight = cms.PSet(
        inputFileName = cms.string(""),
        lead = cms.PSet(
            absEtaBins = cms.vdouble(-1., 1.479, 9.9),
            graphName = cms.string("jetToTauFakeRate/$hadTauSelection/$etaBin/jetToTauFakeRate_mc_hadTaus_pt"),
            applyGraph = cms.bool(True),
            fitFunctionName = cms.string("jetToTauFakeRate/$hadTauSelection/$etaBin/fitFunction_data_div_mc_hadTaus_pt"),
            applyFitFunction = cms.bool(True)
        ),
        sublead = cms.PSet(
            absEtaBins = cms.vdouble(-1., 1.479, 9.9),
            graphName = cms.string("jetToTauFakeRate/$hadTauSelection/$etaBin/jetToTauFakeRate_mc_hadTaus_pt"),
            applyGraph = cms.bool(True),
            fitFunctionName = cms.string("jetToTauFakeRate/$hadTauSelection/$etaBin/fitFunction_data_div_mc_hadTaus_pt"),
            applyFitFunction = cms.bool(True)
        )
    ),
 
    # trigger information
    triggers = cms.PSet(),
    
    # different event weights
    isMC = cms.bool(True),
    lumiScale = cms.VPSet(),
    ref_genWeight = cms.double(0.),
    apply_genWeight = cms.bool(True),
    apply_DYMCReweighting = cms.bool(False),
    apply_DYMCNormScaleFactors = cms.bool(False),
    apply_topPtReweighting = cms.string(''),
    apply_l1PreFireWeight = cms.bool(True),
    btagSFRatio = cms.PSet(),
    apply_btagSFRatio = cms.bool(True),
    metFilters = cms.PSet(),
    #apply_hadTauFakeRateSF = cms.bool(False),
    #apply_genPhotonFilter = cms.string("disabled"),
    disable_ak8_corr = cms.vstring(['JMS', 'JMR', 'PUPPI']),
    apply_chargeMisIdRate = cms.bool(True), # CV: set to True for 2lss and 2lss+1tau channels, and to False for all other channels 

    evtWeight = cms.PSet(
        apply = cms.bool(False),
        histogramFile = cms.string(''),
        histogramName = cms.string(''),
        branchNameXaxis = cms.string(''),
        branchNameYaxis = cms.string(''),
        branchTypeXaxis = cms.string(''),
        branchTypeYaxis = cms.string(''),
    ),
    thWeights = cms.VPSet(),
    hhWeights = config_hhWeights,

    # reconstructed objects
    branchName_electrons = cms.string('Electron'),
    branchName_muons = cms.string('Muon'),
    branchName_hadTaus = cms.string('Tau'),
    branchName_jets_ak4 = cms.string('Jet'),
    branchName_jets_ak8_Hbb = cms.string('FatJet'),
    branchName_subjets_ak8_Hbb = cms.string('SubJet'),
    branchName_jets_ak8_Wjj = cms.string('FatJetAK8LSLoose'),
    branchName_subjets_ak8_Wjj = cms.string('SubJetAK8LSLoose'), 
    branchName_met = cms.string('MET'),
    branchName_vertex = cms.string('PV'),

    # generator level information
    branchName_muonGenMatch = cms.string('MuonGenMatch'),
    branchName_electronGenMatch = cms.string('ElectronGenMatch'),
    branchName_hadTauGenMatch = cms.string('TauGenMatch'),
    branchName_jetGenMatch = cms.string('JetGenMatch'),

    branchName_genLeptons = cms.string('GenLep'),
    branchName_genHadTaus = cms.string('GenVisTau'),
    branchName_genPhotons = cms.string('GenPhoton'),
    branchName_genJets = cms.string('GenJet'),
    branchName_genHiggses = cms.string('GenHiggs'),

    # PS weights
    branchName_PSweights = cms.string('PSWeight'),
    branchName_LHE_nom = cms.string('LHEWeight_originalXWGTUP'),
    has_PS_weights = cms.bool(False),
    apply_LHE_nom = cms.bool(False),

    # LHE/PDF weights
    branchName_scale_weights = cms.string('LHEScaleWeight'),
    branchName_pdf_weights = cms.string('LHEPdfWeight'),
    branchName_envelope_weight_up = cms.string('LHEEnvelopeWeightUp'),
    branchName_envelope_weight_down = cms.string('LHEEnvelopeWeightDown'),
    has_LHE_weights = cms.bool(False),
    has_pdf_weights = cms.bool(False),

    redoGenMatching = cms.bool(False),
    genMatchingByIndex = cms.bool(True),
    jetCleaningByIndex = cms.bool(True),

    writerPlugins = cms.VPSet(
        writers_evtWeight,
        writers_fakeableHadTaus,
        writers_fakeableLeptons,
        # CV: GenPhotonFilterWriter plugin can be run for some MC samples only,
        #     because the collection "GenPhotonCandidate" does not exist in all MC samples !!
        #writers_genPhotonFilter,
        writers_lowMassLeptonPairVeto,
        writers_met,
        writers_metFilters,
        writers_process,
        writers_run_lumi_event,
        writers_selJetsAK4,
        writers_selJetsAK4_btagLoose,
        writers_selJetsAK4_btagMedium,
        writers_ZbosonMassVeto,
        writers_HtoZZto4lVeto
    ),

    selEventsFileName = cms.string(''),

    blacklist = cms.PSet(
        sampleName = cms.string(""),
        inputFileNames = cms.vstring()
    ),
    enable_blacklist = cms.bool(False),

    selection = cms.string(""),

    isDEBUG = cms.bool(False)
)

process.fwliteInput.fileNames = cms.vstring(['/hdfs/local/karl/ttHNtupleProduction/2017/2021Mar05_wPresel_nom_all_hh_multilepton/ntuples/signal_ggf_nonresonant_node_sm_hh_4v/0000/tree_1.root'])
process.fwliteOutput.fileName = cms.string('produceNtuple.root')
process.produceNtuple.era                                            = cms.string('2017')
process.produceNtuple.redoGenMatching                                = cms.bool(False)
process.produceNtuple.isDEBUG                                        = cms.bool(False)
writers_metFilters                                                   = config_recommendedMEtFilters_2017
writers_metFilters.pluginType                                        = cms.string("MEtFilterWriter")
process.produceNtuple.process                                        = cms.string('signal_ggf_nonresonant_hh')
process.produceNtuple.process_hh                                     = cms.string('signal_ggf_nonresonant_hh_wwww')
process.produceNtuple.isMC                                           = cms.bool(True)
process.produceNtuple.has_LHE_weights                                = cms.bool(True)
process.produceNtuple.has_PS_weights                                 = cms.bool(False)
process.produceNtuple.lep_mva_cut_mu                                 = cms.double(0.5)
process.produceNtuple.lep_mva_cut_e                                  = cms.double(0.3)
process.produceNtuple.lep_mva_wp                                     = cms.string('hh_multilepton')
process.produceNtuple.lumiScale                                      = cms.VPSet([cms.PSet(
    central_or_shift = cms.string('central'),
    lumi = cms.double(0.00018092350036)
), cms.PSet(
    central_or_shift = cms.string('CMS_ttHl_thu_shape_HHUp'),
    lumi = cms.double(0.000141651207997)
), cms.PSet(
    central_or_shift = cms.string('CMS_ttHl_thu_shape_HHDown'),
    lumi = cms.double(0.000227213083089)
), cms.PSet(
    central_or_shift = cms.string('CMS_ttHl_pileupUp'),
    lumi = cms.double(0.000180930084191)
), cms.PSet(
    central_or_shift = cms.string('CMS_ttHl_pileupDown'),
    lumi = cms.double(0.000180917254171)
), cms.PSet(
    central_or_shift = cms.string('CMS_ttHl_l1PreFireUp'),
    lumi = cms.double(0.000182202670616)
), cms.PSet(
    central_or_shift = cms.string('CMS_ttHl_l1PreFireDown'),
    lumi = cms.double(0.000179697637174)
)])
process.produceNtuple.ref_genWeight                                  = cms.double(1.0)
#process.produceNtuple.applyFakeRateWeights                           = cms.string('disabled')
process.produceNtuple.apply_genWeight                                = cms.bool(True)
process.produceNtuple.apply_DYMCReweighting                          = cms.bool(False)
process.produceNtuple.apply_DYMCNormScaleFactors                     = cms.bool(False)
process.produceNtuple.apply_l1PreFireWeight                          = cms.bool(True)
process.produceNtuple.leptonFakeRateWeight.inputFileName             = cms.string('TallinnNtupleProducer/EvtWeightTools/data/FakeRate/FR_lep_mva_hh_multilepton_wFullSyst_2017_KBFI_2020Dec21_wCERNUncs2_FRErrTheshold_0p01.root')
process.produceNtuple.leptonFakeRateWeight.histogramName_e           = cms.string('FR_mva030_el_data_comb')
process.produceNtuple.leptonFakeRateWeight.histogramName_mu          = cms.string('FR_mva050_mu_data_comb')
process.produceNtuple.leptonFakeRateWeight.applyNonClosureCorrection = cms.bool(True)
process.produceNtuple.hadTauFakeRateWeight.inputFileName             = cms.string('TallinnNtupleProducer/EvtWeightTools/data/FakeRate/FR_deeptau_DYmumu_2017_v6.root')
process.produceNtuple.hadTauFakeRateWeight.lead.fitFunctionName      = cms.string('jetToTauFakeRate/deepVSjMedium/$etaBin/fitFunction_data_div_mc_hadTaus_pt')
process.produceNtuple.hadTauFakeRateWeight.sublead.fitFunctionName   = cms.string('jetToTauFakeRate/deepVSjMedium/$etaBin/fitFunction_data_div_mc_hadTaus_pt')
process.produceNtuple.hadTauFakeRateWeight.lead.graphName            = cms.string('jetToTauFakeRate/deepVSjMedium/$etaBin/jetToTauFakeRate_mc_hadTaus_pt')
process.produceNtuple.hadTauFakeRateWeight.sublead.graphName         = cms.string('jetToTauFakeRate/deepVSjMedium/$etaBin/jetToTauFakeRate_mc_hadTaus_pt')
process.produceNtuple.triggers                                       = config_triggers_2017
process.produceNtuple.triggers.type_2mu.use_it                       = cms.bool(True)
process.produceNtuple.triggers.type_1e1mu.use_it                     = cms.bool(True)
process.produceNtuple.triggers.type_2e.use_it                        = cms.bool(True)
process.produceNtuple.triggers.type_1mu.use_it                       = cms.bool(True)
process.produceNtuple.triggers.type_1e.use_it                        = cms.bool(True)
process.produceNtuple.jetCleaningByIndex                             = cms.bool(True)
process.produceNtuple.genMatchingByIndex                             = cms.bool(True)
process.produceNtuple.hhWeights.denominator_file_lo                  = cms.string('TallinnNtupleProducer/EvtWeightTools/data/HHReweighting/denom_2017.root')
process.produceNtuple.hhWeights.denominator_file_nlo                 = cms.string('TallinnNtupleProducer/EvtWeightTools/data/HHReweighting/denom_2017_nlo.root')
process.produceNtuple.hhWeights.histtitle                            = cms.string('signal_ggf_nonresonant_hh_wwww')
process.produceNtuple.hhWeights.JHEP04Scan_file                      = cms.string('TallinnNtupleProducer/EvtWeightTools/data/HHReweighting/jhep04_scan.dat')
process.produceNtuple.hhWeights.JHEP03Scan_file                      = cms.string('TallinnNtupleProducer/EvtWeightTools/data/HHReweighting/jhep03_scan.dat')
process.produceNtuple.hhWeights.klScan_file                          = cms.string('TallinnNtupleProducer/EvtWeightTools/data/HHReweighting/kl_scan.dat')
process.produceNtuple.hhWeights.c2Scan_file                          = cms.string('TallinnNtupleProducer/EvtWeightTools/data/HHReweighting/c2_scan.dat')
process.produceNtuple.hhWeights.extraScan_file                       = cms.string('TallinnNtupleProducer/EvtWeightTools/data/HHReweighting/extra_scan.dat')
process.produceNtuple.hhWeights.scanMode                             = cms.vstring(['JHEP04', 'JHEP03', 'kl', 'c2', 'extra'])
process.produceNtuple.hhWeights.apply_rwgt_lo                        = cms.bool(False)
process.produceNtuple.hhWeights.rwgt_nlo_mode                        = cms.string('v3')
process.produceNtuple.apply_topPtReweighting                         = cms.string('')
#process.produceNtuple.useAssocJetBtag                                = cms.bool(False)
process.produceNtuple.apply_btagSFRatio                              = cms.bool(True)
#process.produceNtuple.gen_mHH                                        = cms.vdouble([250.0, 260.0, 270.0, 280.0, 300.0, 320.0, 350.0, 400.0, 450.0, 500.0, 550.0, 600.0, 650.0, 700.0, 750.0, 800.0, 850.0, 900.0, 1000.0])
writers_triggerInfo.PD                                               = cms.string("MC")
writers_genPhotonFilter.apply_genPhotonFilter                        = cms.string('disabled')
#process.produceNtuple.nonRes_BMs                                     = cms.vstring(['SM', 'JHEP04BM1', 'JHEP04BM2', 'JHEP04BM3', 'JHEP04BM4', 'JHEP04BM5', 'JHEP04BM6', 'JHEP04BM7', 'JHEP04BM8', 'JHEP04BM9', 'JHEP04BM10', 'JHEP04BM11', 'JHEP04BM12', 'JHEP04BM8a', 'JHEP03BM1', 'JHEP03BM2', 'JHEP03BM3', 'JHEP03BM4', 'JHEP03BM5', 'JHEP03BM6', 'JHEP03BM7', 'kl_2p45_kt_1p00_c2_0p00_BM9', 'kl_1p00_kt_1p00_c2_0p00_BM9', 'kl_5p00_kt_1p00_c2_0p00_BM9', 'kl_2p45_kt_1p00_c2_0p00_BM7', 'kl_1p00_kt_1p00_c2_0p00_BM7', 'kl_5p00_kt_1p00_c2_0p00_BM7', 'kl_1p00_kt_1p00_c2_0p35_BM9', 'kl_1p00_kt_1p00_c2_3p00_BM9', 'kl_1p00_kt_1p00_c2_0p10_BM9', 'kl_1p00_kt_1p00_c2_m2p00_BM9', 'kl_1p00_kt_1p00_c2_0p35_BM7', 'kl_1p00_kt_1p00_c2_3p00_BM7', 'kl_1p00_kt_1p00_c2_0p10_BM7', 'kl_1p00_kt_1p00_c2_m2p00_BM7', 'kl_0p00_kt_1p00_c2_0p00_BM9', 'kl_0p00_kt_1p00_c2_1p00_BM9', 'kl_0p00_kt_1p00_c2_0p00_BM7', 'kl_0p00_kt_1p00_c2_1p00_BM7'])
process.produceNtuple.enable_blacklist                               = cms.bool(True)
process.produceNtuple.blacklist.inputFileNames                       = cms.vstring(['TallinnNtupleProducer/Framework/data/blacklist/blacklist_postproc_2017.txt', 'TallinnNtupleProducer/Framework/data/blacklist/blacklist_skimmed_multilepton_2017.txt'])
process.produceNtuple.blacklist.sampleName                           = cms.string('signal_ggf_nonresonant_node_sm_hh_4v')
process.produceNtuple.disable_ak8_corr                               = cms.vstring(['JMS', 'JMR', 'PUPPI'])
process.produceNtuple.has_pdf_weights                                = cms.bool(False)
process.produceNtuple.btagSFRatio                                    = cms.PSet(
  central = cms.vdouble(1.0, 0.983850754831, 0.970806608203, 0.95589515666, 0.941090355157, 0.919510668991, 0.896747198034, 0.869121413881, 0.843409507134, 0.788891130366),
)
process.produceNtuple.selection                                      = cms.string("nlep == 2 & ntau == 1")
#process.produceNtuple.selection                                      = cms.string("")

