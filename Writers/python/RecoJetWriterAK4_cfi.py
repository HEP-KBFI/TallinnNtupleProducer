import FWCore.ParameterSet.Config as cms

selJetsAK4 = cms.PSet(
    pluginType = cms.string("RecoJetWriterAK4"),
    jetCollection = cms.string("selJetsAK4"),
    branchName = cms.string("jetAK4"),
    max_numJets = cms.uint32(4)
)

selJetsAK4_btagLoose = cms.PSet(
    pluginType = cms.string("RecoJetWriterAK4"),
    jetCollection = cms.string("selJetsAK4_btagLoose"),
    branchName = cms.string("jetAK4bL"),
    max_numJets = cms.uint32(2)
)

selJetsAK4_btagMedium = cms.PSet(
    pluginType = cms.string("RecoJetWriterAK4"),
    jetCollection = cms.string("selJetsAK4_btagMedium"),
    branchName = cms.string("jetAK4bM"),
    max_numJets = cms.uint32(2)
)
