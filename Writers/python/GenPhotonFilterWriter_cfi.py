import FWCore.ParameterSet.Config as cms

genPhotonFilter = cms.PSet(
    branchName_genPhotons = cms.string('GenPhoton'),
    branchName_genProxyPhotons = cms.string('GenPhotonCandidate'),
    branchName_genParticlesFromHardProcess = cms.string('GenIsHardProcess'),
    apply_genPhotonFilter = cms.string("enabled")
)
