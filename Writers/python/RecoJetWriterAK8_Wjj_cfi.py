import FWCore.ParameterSet.Config as cms

selJetsAK8_Wjj = cms.PSet(
    pluginType = cms.string("RecoJetWriterAK8"),
    jetCollection = cms.string("selJetsAK8_Wjj"),
    branchName = cms.string("jetAK8Wjj"),
    max_numJets = cms.uint32(2)
)
