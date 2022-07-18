import FWCore.ParameterSet.Config as cms

#----------------------------------------------------------------------------------------------------
#CV: Define trigger information, incl. matching of offline-reconstructed e, mu, taus to HLT filters.
#
#    The trigger information needs to be defined separately (as one PSet) for each data-taking period.
#    Each entry in this PSet consists of:
#      - type 
#      - list of HLT paths
#      - relevant HLT filter bits for e, mu, tau
#      - primary dataset (PD) that belonfs to this type of trigger
#
#    The trigger type is defined in:
#      https://github.com/HEP-KBFI/TallinnNtupleProducer/blob/db7eaec73a328698a7e39ccc9d068e045a7b63da/Objects/src/TriggerInfo.cc#L72-L85
#    The names of PDs are defined in:
#      https://github.com/HEP-KBFI/TallinnNtupleProducer/blob/db7eaec73a328698a7e39ccc9d068e045a7b63da/Writers/plugins/TriggerInfoWriter.cc#L21-L27
#    For NanoAODv7 HLT filter bits are defined in:
#      https://github.com/cms-sw/cmssw/blob/CMSSW_10_2_22/PhysicsTools/NanoAOD/python/triggerObjects_cff.py
#    (For other NanoAOD versions, please check the corresponding CMSSW release that was used in the production of said NanoAOD Ntuple)
#
#    Note that the present trigger information is incomplete:
#      - the trigger information for 2017 is missing the types 3mu, 1e2mu, 2e1mu, 3e, 2tau, 1mu1tau, 1e1tau, 1tau
#      - the trigger information for 2016 and 2018 is missing completely
#   (The present trigger information has been entered for the purpose of producing a plain Ntuple for the 2lss channel as example)
#----------------------------------------------------------------------------------------------------

triggers_2016 = cms.PSet()

triggers_2017 = cms.PSet(
    branchName = cms.string("TrigObj"),
    type_2mu = cms.PSet(
        type = cms.string("2mu"),
        hltPaths = cms.vstring(['HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8', 'HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8']),
        hltFilterBits_mu = cms.vuint32(16),
        in_PD = cms.string("DoubleMuon"),
        use_it = cms.bool(True)
    ),
    type_1e1mu = cms.PSet(
        type = cms.string("1e1mu"),
        hltPaths = cms.vstring(['HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ', 'HLT_Mu12_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ', 'HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL', 'HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_DZ']),
        hltFilterBits_e = cms.vuint32(32),
        hltFilterBits_mu = cms.vuint32(32),
        in_PD = cms.string("MuonEG"),
        use_it = cms.bool(True)
    ),
    type_2e = cms.PSet(
        type = cms.string("2e"),
        hltPaths = cms.vstring(['HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL']),
        hltFilterBits_e = cms.vuint32(16),
        in_PD = cms.string("DoubleEG"),
        use_it = cms.bool(True)
    ),    
    type_1mu = cms.PSet(
        type = cms.string("1mu"),
        hltPaths = cms.vstring(['HLT_IsoMu24', 'HLT_IsoMu27']),
        hltFilterBits_mu = cms.vuint32(8),
        in_PD = cms.string("SingleMuon"),
        use_it = cms.bool(True)
    ),
    type_1e = cms.PSet(
        type = cms.string("1e"),
        hltPaths = cms.vstring(['HLT_Ele32_WPTight_Gsf', 'HLT_Ele35_WPTight_Gsf']),
        hltFilterBits_e = cms.vuint32(2),
        in_PD = cms.string("SingleElectron"),
        use_it = cms.bool(True)
    )
)

triggers_2018 = cms.PSet()
