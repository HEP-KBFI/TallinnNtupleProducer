import FWCore.ParameterSet.Config as cms

triggerInfo = cms.PSet(
    pluginType = cms.string("TriggerInfoWriter"),
    PD = cms.string(""),
    PD_priority = cms.vstring("DoubleMuon", "MuonEG", "DoubleEG", "SingleMuon", "SingleElectron", "Tau")
)
