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
#----------------------------------------------------------------------------------------------------
# template PSet:
"""
type_x = cms.PSet(
    type = cms.string(""),
    hltPaths = cms.vstring([]),
    hltFilterBits_e = cms.vuint32(0),
    hltFilterBits_mu = cms.vuint32(0),
    hltFilterBits_tau = cms.vuint32(0),
    in_PD = cms.string(""),
    use_it = cms.bool(True),
),
"""

#TODO is it sensible to default all use_it flags to True?

triggers_2016 = cms.PSet(
    branchName = cms.string("TrigObj"),
    type_2tau = cms.PSet(
        type = cms.string("2tau"),
        hltPaths = cms.vstring([
            'HLT_DoubleMediumIsoPFTau35_Trk1_eta2p1_Reg',
            'HLT_DoubleMediumCombinedIsoPFTau35_Trk1_eta2p1_Reg',
        ]),
        hltFilterBits_tau = cms.vuint32(64),
        in_PD = cms.string("Tau"),
        use_it = cms.bool(True),
    ),
    type_1mu1tau = cms.PSet(
        type = cms.string("1mu1tau"),
        hltPaths = cms.vstring(['HLT_IsoMu19_eta2p1_LooseIsoPFTau20_SingleL1']),
        hltFilterBits_mu = cms.vuint32(64),
        hltFilterBits_tau = cms.vuint32(256),
        in_PD = cms.string("SingleMuon"),
        use_it = cms.bool(True),
    ),
    type_1e1tau = cms.PSet(
        type = cms.string("1e1tau"),
        hltPaths = cms.vstring([
            'HLT_Ele24_eta2p1_WPLoose_Gsf_LooseIsoPFTau20',
            'HLT_Ele24_eta2p1_WPLoose_Gsf_LooseIsoPFTau20_SingleL1',
            'HLT_Ele24_eta2p1_WPLoose_Gsf_LooseIsoPFTau30',
        ]),
        hltFilterBits_e = cms.vuint32(64),
        hltFilterBits_tau = cms.vuint32(128),
        in_PD = cms.string("SingleElectron"),
        use_it = cms.bool(True),
    ),
    type_3mu = cms.PSet(
        type = cms.string("3mu"),
        hltPaths = cms.vstring(['HLT_TripleMu_12_10_5']),
        hltFilterBits_mu = cms.vuint32(128),
        in_PD = cms.string("DoubleMuon"),
        use_it = cms.bool(True),
    ),
    type_3e = cms.PSet(
        type = cms.string("3e"),
        hltPaths = cms.vstring(['HLT_Ele16_Ele12_Ele8_CaloIdL_TrackIdL']),
        hltFilterBits_e = cms.vuint32(128),
        in_PD = cms.string("DoubleEG"),
        use_it = cms.bool(True),
    ),
    type_2e1mu = cms.PSet(
        type = cms.string("2e1mu"),
        hltPaths = cms.vstring(['HLT_Mu8_DiEle12_CaloIdL_TrackIdL']),
        hltFilterBits_e = cms.vuint32(256),
        hltFilterBits_mu = cms.vuint32(512),
        in_PD = cms.string("MuonEG"),
        use_it = cms.bool(True),
    ),
    type_1e2mu = cms.PSet(
        type = cms.string("1e2mu"),
        hltPaths = cms.vstring(['HLT_DiMu9_Ele9_CaloIdL_TrackIdL']),
        hltFilterBits_e = cms.vuint32(512),
        hltFilterBits_mu = cms.vuint32(256),
        in_PD = cms.string("MuonEG"),
        use_it = cms.bool(True),
    ),
    type_2mu = cms.PSet(
        type = cms.string("2mu"),
        hltPaths = cms.vstring([
            'HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL',
            'HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ',
            'HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL',
            'HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ',
        ]),
        hltFilterBits_mu = cms.vuint32(16),
        in_PD = cms.string("DoubleMuon"),
        use_it = cms.bool(True),
    ),
    type_1e1mu = cms.PSet(
        type = cms.string("1e1mu"),
        hltPaths = cms.vstring([
            'HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL',
            'HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ',
            'HLT_Mu23_TrkIsoVVL_Ele8_CaloIdL_TrackIdL_IsoVL',
            'HLT_Mu23_TrkIsoVVL_Ele8_CaloIdL_TrackIdL_IsoVL_DZ',
        ]),
        hltFilterBits_e = cms.vuint32(32),
        hltFilterBits_mu = cms.vuint32(32),
        in_PD = cms.string("MuonEG"),
        use_it = cms.bool(True),
    ),
    type_2e = cms.PSet(
        type = cms.string("2e"),
        hltPaths = cms.vstring(['HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ']),
        hltFilterBits_e = cms.vuint32(16),
        in_PD = cms.string("DoubleEG"),
        use_it = cms.bool(True),
    ),
    type_1mu = cms.PSet(
        type = cms.string("1mu"),
        hltPaths = cms.vstring([
            'HLT_IsoMu22',
            'HLT_IsoTkMu22',
            'HLT_IsoMu22_eta2p1',
            'HLT_IsoTkMu22_eta2p1',
            'HLT_IsoMu24',
            'HLT_IsoTkMu24',
        ]),
        hltFilterBits_mu = cms.vuint32(8),
        in_PD = cms.string("SingleMuon"),
        use_it = cms.bool(True),
    ),
    type_1e = cms.PSet(
        type = cms.string("1e"),
        hltPaths = cms.vstring([
            'HLT_Ele27_WPTight_Gsf',
            'HLT_Ele25_eta2p1_WPTight_Gsf',
            'HLT_Ele27_eta2p1_WPLoose_Gsf',
         ]),
        hltFilterBits_e = cms.vuint32(2),
        in_PD = cms.string("SingleElectron"),
        use_it = cms.bool(True),
    ),
)

triggers_2017 = cms.PSet(
    branchName = cms.string("TrigObj"),
    type_2tau = cms.PSet(
        type = cms.string("2tau"),
        hltPaths = cms.vstring([
            'HLT_DoubleMediumChargedIsoPFTau35_Trk1_eta2p1_Reg',
            'HLT_DoubleTightChargedIsoPFTau35_Trk1_TightID_eta2p1_Reg',
            'HLT_DoubleMediumChargedIsoPFTau40_Trk1_TightID_eta2p1_Reg',
            'HLT_DoubleTightChargedIsoPFTau40_Trk1_eta2p1_Reg',
        ]),
        hltFilterBits_tau = cms.vuint32(64),
        in_PD = cms.string("Tau"),
        use_it = cms.bool(True),
    ),
    type_1mu1tau = cms.PSet(
        type = cms.string("1mu1tau"),
        hltPaths = cms.vstring(['HLT_IsoMu20_eta2p1_LooseChargedIsoPFTau27_eta2p1_CrossL1']),
        hltFilterBits_mu = cms.vuint32(64),
        hltFilterBits_tau = cms.vuint32(256),
        in_PD = cms.string("SingleMuon"),
        use_it = cms.bool(True),
    ),
    type_1e1tau = cms.PSet(
        type = cms.string("1e1tau"),
        hltPaths = cms.vstring(['HLT_Ele24_eta2p1_WPTight_Gsf_LooseChargedIsoPFTau30_eta2p1_CrossL1']),
        hltFilterBits_e = cms.vuint32(64),
        hltFilterBits_tau = cms.vuint32(128),
        in_PD = cms.string("SingleElectron"),
        use_it = cms.bool(True),
    ),
    type_3mu = cms.PSet(
        type = cms.string("3mu"),
        hltPaths = cms.vstring(['HLT_TripleMu_12_10_5']),
        hltFilterBits_mu = cms.vuint32(128),
        in_PD = cms.string("DoubleMuon"),
        use_it = cms.bool(True),
    ),
    type_3e = cms.PSet(
        type = cms.string("3e"),
        hltPaths = cms.vstring(['HLT_Ele16_Ele12_Ele8_CaloIdL_TrackIdL']),
        hltFilterBits_e = cms.vuint32(128),
        in_PD = cms.string("DoubleEG"),
        use_it = cms.bool(True),
    ),
    type_2e1mu = cms.PSet(
        type = cms.string("2e1mu"),
        hltPaths = cms.vstring(['HLT_Mu8_DiEle12_CaloIdL_TrackIdL']),
        hltFilterBits_e = cms.vuint32(256),
        hltFilterBits_mu = cms.vuint32(512),
        in_PD = cms.string("MuonEG"),
        use_it = cms.bool(True),
    ),
    type_1e2mu = cms.PSet(
        type = cms.string("1e2mu"),
        hltPaths = cms.vstring(['HLT_DiMu9_Ele9_CaloIdL_TrackIdL_DZ']),
        hltFilterBits_e = cms.vuint32(512),
        hltFilterBits_mu = cms.vuint32(256),
        in_PD = cms.string("MuonEG"),
        use_it = cms.bool(True),
    ),
    type_2mu = cms.PSet(
        type = cms.string("2mu"),
        hltPaths = cms.vstring([
            'HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8',
            'HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8',
        ]),
        hltFilterBits_mu = cms.vuint32(16),
        in_PD = cms.string("DoubleMuon"),
        use_it = cms.bool(True),
    ),
    type_1e1mu = cms.PSet(
        type = cms.string("1e1mu"),
        hltPaths = cms.vstring([
            'HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ',
            'HLT_Mu12_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ',
            'HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL',
            'HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_DZ',
        ]),
        hltFilterBits_e = cms.vuint32(32),
        hltFilterBits_mu = cms.vuint32(32),
        in_PD = cms.string("MuonEG"),
        use_it = cms.bool(True),
    ),
    type_2e = cms.PSet(
        type = cms.string("2e"),
        hltPaths = cms.vstring(['HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL']),
        hltFilterBits_e = cms.vuint32(16),
        in_PD = cms.string("DoubleEG"),
        use_it = cms.bool(True),
    ),
    type_1mu = cms.PSet(
        type = cms.string("1mu"),
        hltPaths = cms.vstring([
            'HLT_IsoMu24',
            'HLT_IsoMu27',
        ]),
        hltFilterBits_mu = cms.vuint32(8),
        in_PD = cms.string("SingleMuon"),
        use_it = cms.bool(True),
    ),
    type_1e = cms.PSet(
        type = cms.string("1e"),
        hltPaths = cms.vstring([
            'HLT_Ele32_WPTight_Gsf',
            'HLT_Ele35_WPTight_Gsf',
        ]),
        hltFilterBits_e = cms.vuint32(2),
        in_PD = cms.string("SingleElectron"),
        use_it = cms.bool(True),
    ),
)

triggers_2018 = cms.PSet(
    branchName = cms.string("TrigObj"),
    type_2tau = cms.PSet(
        type = cms.string("2tau"),
        hltPaths = cms.vstring([
            'HLT_DoubleTightChargedIsoPFTau35_Trk1_TightID_eta2p1_Reg',
            'HLT_DoubleMediumChargedIsoPFTau40_Trk1_TightID_eta2p1_Reg',
            'HLT_DoubleTightChargedIsoPFTau40_Trk1_eta2p1_Reg',
            'HLT_DoubleMediumChargedIsoPFTauHPS35_Trk1_eta2p1_Reg',
        ]),
        hltFilterBits_tau = cms.vuint32(64),
        in_PD = cms.string("Tau"),
        use_it = cms.bool(True),
    ),
    type_1mu1tau = cms.PSet(
        type = cms.string("1mu1tau"),
        hltPaths = cms.vstring([
            'HLT_IsoMu20_eta2p1_LooseChargedIsoPFTau27_eta2p1_CrossL1',
            'HLT_IsoMu20_eta2p1_LooseChargedIsoPFTauHPS27_eta2p1_CrossL1',
        ]),
        hltFilterBits_mu = cms.vuint32(64),
        hltFilterBits_tau = cms.vuint32(256),
        in_PD = cms.string("SingleMuon"),
        use_it = cms.bool(True),
    ),
    type_1e1tau = cms.PSet(
        type = cms.string("1e1tau"),
        hltPaths = cms.vstring([
            'HLT_Ele24_eta2p1_WPTight_Gsf_LooseChargedIsoPFTau30_eta2p1_CrossL1',
            'HLT_Ele24_eta2p1_WPTight_Gsf_LooseChargedIsoPFTauHPS30_eta2p1_CrossL1',
        ]),
        hltFilterBits_e = cms.vuint32(64),
        hltFilterBits_tau = cms.vuint32(128),
        in_PD = cms.string("SingleElectron"),
        use_it = cms.bool(True),
    ),
    type_3mu = cms.PSet(
        type = cms.string("3mu"),
        hltPaths = cms.vstring(['HLT_TripleMu_12_10_5']),
        hltFilterBits_mu = cms.vuint32(128),
        in_PD = cms.string("DoubleMuon"),
        use_it = cms.bool(True),
    ),
    type_3e = cms.PSet(
        type = cms.string("3e"),
        hltPaths = cms.vstring(['HLT_Ele16_Ele12_Ele8_CaloIdL_TrackIdL']),
        hltFilterBits_e = cms.vuint32(128),
        in_PD = cms.string("DoubleEG"),
        use_it = cms.bool(True),
    ),
    type_2e1mu = cms.PSet(
        type = cms.string("2e1mu"),
        hltPaths = cms.vstring(['HLT_Mu8_DiEle12_CaloIdL_TrackIdL']),
        hltFilterBits_e = cms.vuint32(256),
        hltFilterBits_mu = cms.vuint32(512),
        in_PD = cms.string("MuonEG"),
        use_it = cms.bool(True),
    ),
    type_1e2mu = cms.PSet(
        type = cms.string("1e2mu"),
        hltPaths = cms.vstring(['HLT_DiMu9_Ele9_CaloIdL_TrackIdL_DZ']),
        hltFilterBits_e = cms.vuint32(512),
        hltFilterBits_mu = cms.vuint32(256),
        in_PD = cms.string("MuonEG"),
        use_it = cms.bool(True),
    ),
    type_2mu = cms.PSet(
        type = cms.string("2mu"),
        hltPaths = cms.vstring(['HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8']),
        hltFilterBits_mu = cms.vuint32(16),
        in_PD = cms.string("DoubleMuon"),
        use_it = cms.bool(True),
    ),
    type_1e1mu = cms.PSet(
        type = cms.string("1e1mu"),
        hltPaths = cms.vstring([
            'HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ',
            'HLT_Mu12_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ',
            'HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL',
        ]),
        hltFilterBits_e = cms.vuint32(32),
        hltFilterBits_mu = cms.vuint32(32),
        in_PD = cms.string("MuonEG"),
        use_it = cms.bool(True),
    ),
    type_2e = cms.PSet(
        type = cms.string("2e"),
        hltPaths = cms.vstring(['HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL']),
        hltFilterBits_e = cms.vuint32(16),
        in_PD = cms.string("DoubleEG"),
        use_it = cms.bool(True),
    ),
    type_1mu = cms.PSet(
        type = cms.string("1mu"),
        hltPaths = cms.vstring([
            'HLT_IsoMu24',
            'HLT_IsoMu27',
        ]),
        hltFilterBits_mu = cms.vuint32(8),
        in_PD = cms.string("SingleMuon"),
        use_it = cms.bool(True),
    ),
    type_1e = cms.PSet(
        type = cms.string("1e"),
        hltPaths = cms.vstring(['HLT_Ele32_WPTight_Gsf']),
        hltFilterBits_e = cms.vuint32(2),
        in_PD = cms.string("SingleElectron"),
        use_it = cms.bool(True),
    ),
)
