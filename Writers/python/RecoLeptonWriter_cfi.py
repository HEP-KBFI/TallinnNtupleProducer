import FWCore.ParameterSet.Config as cms

fakeableLeptons = cms.PSet(
    pluginType = cms.string("RecoLeptonWriter")
)
