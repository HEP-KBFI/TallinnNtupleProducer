import FWCore.ParameterSet.Config as cms

lowMassLeptonPairVeto = cms.PSet(
    pluginType = cms.string("LowMassLeptonPairVetoWriter"),
    requireSF = cms.bool(False),
    requireOS = cms.bool(False),
    isDEBUG = cms.bool(True)
)
