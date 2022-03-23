import FWCore.ParameterSet.Config as cms

trigger_1e = cms.PSet(
    type = cms.string("1e"),
    hltPaths = cms.vstring([ "HLT_Ele25_eta2p1_WPTight_Gsf", "HLT_Ele27_WPTight_Gsf", "HLT_Ele27_eta2p1_WPLoose_Gsf" ]),
    in_PD = cms.string("SingleElectron"),
    use_it = cms.bool(False)
)

trigger_1mu = cms.PSet(
    type = cms.string("1mu"),
    hltPaths = cms.vstring([ "HLT_IsoMu22_eta2p1", "HLT_IsoMu22", "HLT_IsoMu24", "HLT_IsoTkMu22_eta2p1", "HLT_IsoTkMu24", "HLT_IsoTkMu22" ]),
    in_PD = cms.string("SingleMuon"),
    use_it = cms.bool(False)
)

triggers = cms.VPSet(
    trigger_1e,
    trigger_1mu
)

PD_priority = cms.vstring([ "SingleMuon", "SingleElectron" ])
