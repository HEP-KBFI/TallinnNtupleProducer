import FWCore.ParameterSet.Config as cms

genPhotonFilter = cms.PSet(
    pluginType = cms.string("GenPhotonFilterWriter"),
    apply_genPhotonFilter = cms.string("enabled")
)
