import FWCore.ParameterSet.Config as cms

evtReweight_HH = cms.PSet(
    pluginType = cms.string("EvtReweightWriter_HH"),
    apply_rwgt_lo = cms.bool(False),
    apply_rwgt_nlo = cms.bool(True),
    rwgt_nlo_mode = cms.string('v3'),
    denominator_file_lo = cms.string('TallinnNtupleProducer/EvtWeightTools/data/HHReweighting/denom_2017.root'),
    denominator_file_nlo = cms.string('TallinnNtupleProducer/EvtWeightTools/data/HHReweighting/denom_2017_nlo.root'),
    histtitle = cms.string('signal_ggf_nonresonant_hh_wwww'),
    JHEP04Scan_file = cms.string('TallinnNtupleProducer/EvtWeightTools/data/HHReweighting/jhep04_scan.dat'),
    JHEP03Scan_file = cms.string('TallinnNtupleProducer/EvtWeightTools/data/HHReweighting/jhep03_scan.dat'),
    klScan_file = cms.string('TallinnNtupleProducer/EvtWeightTools/data/HHReweighting/kl_scan.dat'),
    ktScan_file = cms.string(''),
    c2Scan_file = cms.string('TallinnNtupleProducer/EvtWeightTools/data/HHReweighting/c2_scan.dat'),
    cgScan_file = cms.string(''),
    c2gScan_file = cms.string(''),
    extraScan_file = cms.string('TallinnNtupleProducer/EvtWeightTools/data/HHReweighting/extra_scan.dat'),
    coefFile = cms.string('TallinnNtupleProducer/EvtWeightTools/data/HHReweighting/coefficientsByBin_extended_3M_costHHSim_19-4.txt'),
    scanMode = cms.vstring([ 'JHEP04', 'JHEP03', 'kl', 'c2', 'extra' ]),
    isDEBUG = cms.bool(False)
)
