import FWCore.ParameterSet.Config as cms

selJetsAK8_Hbb = cms.PSet(
    pluginType = cms.string("RecoJetWriterAK8"),
    jetCollection = cms.string("selJetsAK8_Hbb"),
    branchName = cms.string("jetAK8Hbb"),
    max_numJets = cms.uint32(2)
)
