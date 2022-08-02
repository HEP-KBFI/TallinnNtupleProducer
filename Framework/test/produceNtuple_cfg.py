import FWCore.ParameterSet.Config as cms

from TallinnNtupleProducer.Framework.recommendedMEtFilters_cfi import \
    recommendedMEtFilters_2016 as config_recommendedMEtFilters_2016, \
    recommendedMEtFilters_2017 as config_recommendedMEtFilters_2017, \
    recommendedMEtFilters_2018 as config_recommendedMEtFilters_2018

from TallinnNtupleProducer.Framework.triggers_cfi import \
    triggers_2016 as config_triggers_2016, \
    triggers_2018 as config_triggers_2018, \
    triggers_2018 as config_triggers_2018

from TallinnNtupleProducer.Writers.BDTVarWriter_HH_2lss_cfi import bdtVariables_HH_2lss as writers_bdtVariables_HH_2lss
from TallinnNtupleProducer.Writers.EvtReweightWriter_HH_cfi import evtReweight_HH as writers_evtReweight_HH
from TallinnNtupleProducer.Writers.EvtReweightWriter_tH_cfi import evtReweight_tH as writers_evtReweight_tH
from TallinnNtupleProducer.Writers.EvtWeightWriter_cfi import evtWeight as writers_evtWeight
from TallinnNtupleProducer.Writers.GenHHKinematicsWriter_cfi import genHHKinematics as writers_genHHKinematics
from TallinnNtupleProducer.Writers.GenPhotonFilterWriter_cfi import genPhotonFilter as writers_genPhotonFilter
from TallinnNtupleProducer.Writers.EvtInfoWriter_cfi import evtInfo as writers_evtInfo
from TallinnNtupleProducer.Writers.HtoZZto4lVetoWriter_cfi import HtoZZto4lVeto as writers_HtoZZto4lVeto
from TallinnNtupleProducer.Writers.LowMassLeptonPairVetoWriter_cfi import lowMassLeptonPairVeto as writers_lowMassLeptonPairVeto
from TallinnNtupleProducer.Writers.MEtFilterWriter_cfi import metFilters as writers_metFilters
from TallinnNtupleProducer.Writers.ProcessWriter_cfi import process as writers_process
from TallinnNtupleProducer.Writers.RecoHadTauMultiplicityWriter_cfi import hadTauMultiplicity as writers_hadTauMultiplicity
from TallinnNtupleProducer.Writers.RecoHadTauWriter_cfi import fakeableHadTaus as writers_fakeableHadTaus
from TallinnNtupleProducer.Writers.RecoJetWriterAK4_cfi import \
    selJetsAK4 as writers_selJetsAK4, \
    selJetsAK4_btagLoose as writers_selJetsAK4_btagLoose, \
    selJetsAK4_btagMedium as writers_selJetsAK4_btagMedium
from TallinnNtupleProducer.Writers.RecoJetWriterAK8_Wjj_cfi import selJetsAK8_Wjj as writers_selJetsAK8_Wjj
from TallinnNtupleProducer.Writers.RecoLeptonMultiplicityWriter_cfi import leptonMultiplicity as writers_leptonMultiplicity
from TallinnNtupleProducer.Writers.RecoLeptonWriter_cfi import fakeableLeptons as writers_fakeableLeptons
from TallinnNtupleProducer.Writers.RecoMEtWriter_cfi import met as writers_met
from TallinnNtupleProducer.Writers.RunLumiEventWriter_cfi import run_lumi_event as writers_run_lumi_event
from TallinnNtupleProducer.Writers.TriggerInfoWriter_cfi import triggerInfo as writers_triggerInfo
from TallinnNtupleProducer.Writers.ZbosonMassVetoWriter_cfi import ZbosonMassVeto as writers_ZbosonMassVeto

process = cms.PSet()

process.fwliteInput = cms.PSet(
    fileNames = cms.vstring(),
    skipEvents = cms.int32(0),
    maxEvents = cms.int32(-1),
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
    applyNumNominalLeptonsCut = cms.bool(False),
    numNominalHadTaus = cms.uint32(1),
    applyNumNominalHadTausCut = cms.bool(False),

    # selection of electrons, muons, and hadronic taus
    lep_mva_cut_mu = cms.double(0.5),
    lep_mva_cut_e = cms.double(0.3),
    lep_mva_wp = cms.string('hh_multilepton'),
    hadTauWP_againstJets_tight = cms.string("deepVSjMedium"),
    hadTauWP_againstJets_fakeable = cms.string("deepVSjVVLoose"),
    hadTauWP_againstElectrons = cms.string(""),
    hadTauWP_againstMuons = cms.string(""),

    # fake rates for electrons, muons, and hadronic taus
    leptonFakeRateWeight = cms.PSet(
        inputFileName = cms.string(""),
        histogramName_e = cms.string(""),
        histogramName_mu = cms.string(""),
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
    apply_LHEVpt_rwgt = cms.bool(False),
    apply_topPtReweighting = cms.string(''),
    apply_l1PreFireWeight = cms.bool(True),
    apply_pileupJetID = cms.string(''),
    #metFilters = cms.PSet(),
    #apply_hadTauFakeRateSF = cms.bool(False),
    #apply_genPhotonFilter = cms.string("disabled"),
    fatJet_corrections = cms.vstring(), # possible values: 'JMS', 'JMR', 'PUPPI'
    apply_chargeMisIdRate = cms.bool(True), # CV: set to True for 2lss and 2lss+1tau channels, and to False for all other channels
    split_jes = cms.bool(True),
    split_jer = cms.bool(True),
    isCP5 = cms.bool(False),
    reapply_JEC = cms.bool(False),
    apply_smearing = cms.bool(True),
    enable_phiModulationCorr = cms.bool(True),

    evtWeight = cms.PSet(
        apply = cms.bool(False),
        histogramFile = cms.string(''),
        histogramName = cms.string(''),
        branchNameXaxis = cms.string(''),
        branchNameYaxis = cms.string(''),
        branchTypeXaxis = cms.string(''),
        branchTypeYaxis = cms.string(''),
    ),

    # reconstructed objects
    branchName_electrons = cms.string('Electron'),
    branchName_muons = cms.string('Muon'),
    branchName_hadTaus = cms.string('Tau'),
    branchName_jets_ak4 = cms.string('Jet'),
    branchName_jets_ak8 = cms.string('FatJet'),
    branchName_subjets_ak8 = cms.string('SubJet'),
    branchName_met = cms.string('MET'),
    branchName_genJetAK8 = cms.string('GenJetAK8'),
    branchName_genSubJetAK8 = cms.string('SubGenJetAK8'),
    branchName_rawMet = cms.string('RawMET'),
    branchName_vertex = cms.string('PV'),

    # generator level information
    branchName_genPart = cms.string('GenPart'),
    branchName_genHadTaus = cms.string('GenVisTau'),
    branchName_genJets = cms.string('GenJet'),
    branchName_corrT1METJet = cms.string('CorrT1METJet'),

    # PS weights
    branchName_PSweights = cms.string('PSWeight'),
    branchName_LHE_nom = cms.string('LHEWeight_originalXWGTUP'),
    has_PS_weights = cms.bool(False),
    apply_LHE_nom = cms.bool(False),

    # LHE/PDF weights
    branchName_scale_weights = cms.string('LHEScaleWeight'),
    branchName_pdf_weights = cms.string('LHEPdfWeight'),
    has_LHE_weights = cms.bool(False),
    has_pdf_weights = cms.bool(False), # when set to True, reads the PDF weights
    pdfSettings = cms.PSet(
        lhaid = cms.uint32(0),
        norm = cms.vdouble(), # ratio of event sum with PDF weights over nominal event sum; when populated, derives PDF envelope
        saveAllMembers = cms.bool(False), # when set to True, saves the PDF weights of all its members
    ),

    branchName_LHE_particle = cms.string('LHEPart'),

    jetCleaningByIndex = cms.bool(True),
    genMatchRecoJets = cms.bool(False),

    writerPlugins = cms.VPSet(
        writers_bdtVariables_HH_2lss,
        # CV: use EvtReweightWriter_HH plugin only when processing non-resonant HH MC samples
        writers_evtReweight_HH,
        # CV: use EvtReweightWriter_tH plugin only when processing tH MC samples
        #writers_evtReweight_tH,
        writers_evtWeight,
        writers_fakeableHadTaus,
        writers_fakeableLeptons,
        # CV: use GenHHKinematicsWriter plugin only when processing HH MC samples (resonant and non-resonant)
        writers_genHHKinematics,
        # CV: GenPhotonFilterWriter plugin can be run for some MC samples only,
        #     because the collection "GenPhotonCandidate" does not exist in all MC samples !!
        #writers_genPhotonFilter,
        writers_evtInfo,
        writers_hadTauMultiplicity,
        writers_leptonMultiplicity,
        writers_lowMassLeptonPairVeto,
        writers_met,
        writers_metFilters,
        writers_process,
        writers_run_lumi_event,
        writers_selJetsAK4,
        writers_selJetsAK4_btagLoose,
        writers_selJetsAK4_btagMedium,
        writers_selJetsAK8_Wjj,
        writers_triggerInfo,
        writers_ZbosonMassVeto,
        writers_HtoZZto4lVeto
    ),

    selEventsFileName = cms.string(''),
    selection = cms.string(""),
    isDEBUG = cms.bool(False)
)

process.fwliteInput.fileNames = cms.vstring([
    "/hdfs/cms/store/mc/RunIIAutumn18NanoAODv7/GluGluToHHTo4V_node_SM_TuneCP5_PSWeights_13TeV-madgraph-pythia8/NANOAODSIM/Nano02Apr2020_102X_upgrade2018_realistic_v21-v1/120000/30CAD39B-17B0-8B4B-95F6-7FBAB3AE4222.root",
    "/hdfs/cms/store/mc/RunIIAutumn18NanoAODv7/GluGluToHHTo4V_node_SM_TuneCP5_PSWeights_13TeV-madgraph-pythia8/NANOAODSIM/Nano02Apr2020_102X_upgrade2018_realistic_v21-v1/120000/474A6444-3EE0-E646-AA4E-DF56958B896C.root",
    "/hdfs/cms/store/mc/RunIIAutumn18NanoAODv7/GluGluToHHTo4V_node_SM_TuneCP5_PSWeights_13TeV-madgraph-pythia8/NANOAODSIM/Nano02Apr2020_102X_upgrade2018_realistic_v21-v1/120000/4D632E2D-79BE-4D46-BDDB-AD0A3BEDB1F2.root",
    "/hdfs/cms/store/mc/RunIIAutumn18NanoAODv7/GluGluToHHTo4V_node_SM_TuneCP5_PSWeights_13TeV-madgraph-pythia8/NANOAODSIM/Nano02Apr2020_102X_upgrade2018_realistic_v21-v1/120000/C4271AA8-1557-C846-8CAF-3F74F5AD64E3.root",
    "/hdfs/cms/store/mc/RunIIAutumn18NanoAODv7/GluGluToHHTo4V_node_SM_TuneCP5_PSWeights_13TeV-madgraph-pythia8/NANOAODSIM/Nano02Apr2020_102X_upgrade2018_realistic_v21-v1/120000/E34A3E65-30F6-B44C-A4E1-87858B9FBF09.root",
    "/hdfs/cms/store/mc/RunIIAutumn18NanoAODv7/GluGluToHHTo4V_node_SM_TuneCP5_PSWeights_13TeV-madgraph-pythia8/NANOAODSIM/Nano02Apr2020_102X_upgrade2018_realistic_v21-v1/120000/FEAE72BC-40B4-2045-A65F-B92515AA6210.root",
])
process.fwliteOutput.fileName = cms.string('produceNtuple.root')
process.produceNtuple.era                                            = cms.string('2018')
process.produceNtuple.isDEBUG                                        = cms.bool(False)
writers_metFilters.flags                                             = config_recommendedMEtFilters_2018
process.produceNtuple.process                                        = cms.string('signal_ggf_nonresonant_hh')
process.produceNtuple.process_hh                                     = cms.string('signal_ggf_nonresonant_hh_wwww')
process.produceNtuple.isMC                                           = cms.bool(True)
process.produceNtuple.has_LHE_weights                                = cms.bool(True)
process.produceNtuple.has_PS_weights                                 = cms.bool(True)
process.produceNtuple.lep_mva_cut_mu                                 = cms.double(0.5)
process.produceNtuple.lep_mva_cut_e                                  = cms.double(0.3)
process.produceNtuple.lep_mva_wp                                     = cms.string('hh_multilepton')
process.produceNtuple.lumiScale                                      = cms.VPSet([cms.PSet(
    central_or_shift = cms.string('central'),
    lumi = cms.double(0.00018092350036)
), cms.PSet(
    central_or_shift = cms.string('CMS_ttHl_thu_shape_x1y1Up'),
    lumi = cms.double(0.00022758482348244583)
), cms.PSet(
    central_or_shift = cms.string('CMS_ttHl_thu_shape_x1y1Down'),
    lumi = cms.double(0.00014137135174667993)
), cms.PSet(
    central_or_shift = cms.string('CMS_ttHl_thu_shape_x1Up'),
    lumi = cms.double(0.0001915793492110972)
), cms.PSet(
    central_or_shift = cms.string('CMS_ttHl_thu_shape_x1Down'),
    lumi = cms.double(0.00017078853584880804)
), cms.PSet(
    central_or_shift = cms.string('CMS_ttHl_thu_shape_y1Up'),
    lumi = cms.double(0.00021488143889971005)
), cms.PSet(
    central_or_shift = cms.string('CMS_ttHl_thu_shape_y1Down'),
    lumi = cms.double(0.00014971928985888286)
)])
process.produceNtuple.ref_genWeight                                  = cms.double(1.0)
process.produceNtuple.apply_genWeight                                = cms.bool(True)
process.produceNtuple.apply_l1PreFireWeight                          = cms.bool(True)
process.produceNtuple.apply_pileupJetID                              = cms.string('')
process.produceNtuple.leptonFakeRateWeight.inputFileName             = cms.string('TallinnNtupleProducer/EvtWeightTools/data/FakeRate/FR_lep_mva_hh_multilepton_wFullSyst_2018_KBFI_2020Dec21_wCERNUncs2_FRErrTheshold_0p01.root')
process.produceNtuple.leptonFakeRateWeight.histogramName_e           = cms.string('FR_mva030_el_data_comb')
process.produceNtuple.leptonFakeRateWeight.histogramName_mu          = cms.string('FR_mva050_mu_data_comb')
process.produceNtuple.leptonFakeRateWeight.applyNonClosureCorrection = cms.bool(True)
process.produceNtuple.hadTauFakeRateWeight.inputFileName             = cms.string('TallinnNtupleProducer/EvtWeightTools/data/FakeRate/FR_deeptau_DYmumu_2018_v6.root')
process.produceNtuple.hadTauFakeRateWeight.lead.fitFunctionName      = cms.string('jetToTauFakeRate/deepVSjMedium/$etaBin/fitFunction_data_div_mc_hadTaus_pt')
process.produceNtuple.hadTauFakeRateWeight.sublead.fitFunctionName   = cms.string('jetToTauFakeRate/deepVSjMedium/$etaBin/fitFunction_data_div_mc_hadTaus_pt')
process.produceNtuple.hadTauFakeRateWeight.lead.graphName            = cms.string('jetToTauFakeRate/deepVSjMedium/$etaBin/jetToTauFakeRate_mc_hadTaus_pt')
process.produceNtuple.hadTauFakeRateWeight.sublead.graphName         = cms.string('jetToTauFakeRate/deepVSjMedium/$etaBin/jetToTauFakeRate_mc_hadTaus_pt')
process.produceNtuple.triggers                                       = config_triggers_2018
process.produceNtuple.triggers.type_2mu.use_it                       = cms.bool(True)
process.produceNtuple.triggers.type_1e1mu.use_it                     = cms.bool(True)
process.produceNtuple.triggers.type_2e.use_it                        = cms.bool(True)
process.produceNtuple.triggers.type_1mu.use_it                       = cms.bool(True)
process.produceNtuple.triggers.type_1e.use_it                        = cms.bool(True)
writers_triggerInfo.PD                                               = cms.string("MC")
writers_genPhotonFilter.apply_genPhotonFilter                        = cms.string('disabled')
process.produceNtuple.selection                                      = cms.string("")
