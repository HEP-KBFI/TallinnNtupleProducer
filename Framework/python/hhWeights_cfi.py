import FWCore.ParameterSet.Config as cms

hhWeights = cms.PSet(
    denominator_file_lo = cms.string(''),
    denominator_file_nlo = cms.string(''),
    JHEP04Scan_file = cms.string(''),
    JHEP03Scan_file = cms.string(''),
    klScan_file = cms.string(''),
    ktScan_file = cms.string(''),
    c2Scan_file = cms.string(''),
    cgScan_file = cms.string(''),
    c2gScan_file = cms.string(''),
    extraScan_file = cms.string(''),
    coefFile = cms.string('HHStatAnalysis/AnalyticalModels/data/coefficientsByBin_extended_3M_costHHSim_19-4.txt'),
    histtitle = cms.string(''),
    isDEBUG = cms.bool(False),
    apply_rwgt_lo = cms.bool(True),
    scanMode = cms.vstring(),
    apply_rwgt_nlo = cms.bool(True),
    rwgt_nlo_mode = cms.string(''),
)
