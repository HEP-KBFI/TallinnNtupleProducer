import FWCore.ParameterSet.Config as cms

# MET filter settings taken from:
# 2018: https://twiki.cern.ch/twiki/bin/view/CMS/MissingETOptionalFiltersRun2?rev=136#2018_data
# 2017: https://twiki.cern.ch/twiki/bin/view/CMS/MissingETOptionalFiltersRun2?rev=136#2017_data
# 2016: https://twiki.cern.ch/twiki/bin/view/CMS/MissingETOptionalFiltersRun2?rev=136#2016_data

recommendedMEtFilters_2016 = cms.PSet(
    apply_Flag_BadChargedCandidateFilter          = cms.PSet(forMC = cms.bool(False), forData = cms.bool(False)), # Under review
    apply_Flag_BadChargedCandidateSummer16Filter  = cms.PSet(forMC = cms.bool(False), forData = cms.bool(False)),
    apply_Flag_BadPFMuonFilter                    = cms.PSet(forMC = cms.bool(True),  forData = cms.bool(True)),
    apply_Flag_BadPFMuonSummer16Filter            = cms.PSet(forMC = cms.bool(False), forData = cms.bool(False)),
    apply_Flag_chargedHadronTrackResolutionFilter = cms.PSet(forMC = cms.bool(False), forData = cms.bool(False)),
    apply_Flag_CSCTightHalo2015Filter             = cms.PSet(forMC = cms.bool(False), forData = cms.bool(False)),
    apply_Flag_CSCTightHaloFilter                 = cms.PSet(forMC = cms.bool(False), forData = cms.bool(False)),
    apply_Flag_CSCTightHaloTrkMuUnvetoFilter      = cms.PSet(forMC = cms.bool(False), forData = cms.bool(False)),
    apply_Flag_ecalBadCalibFilter                 = cms.PSet(forMC = cms.bool(False), forData = cms.bool(False)),
    apply_Flag_EcalDeadCellBoundaryEnergyFilter   = cms.PSet(forMC = cms.bool(False), forData = cms.bool(False)),
    apply_Flag_EcalDeadCellTriggerPrimitiveFilter = cms.PSet(forMC = cms.bool(True),  forData = cms.bool(True)),
    apply_Flag_ecalLaserCorrFilter                = cms.PSet(forMC = cms.bool(False), forData = cms.bool(False)),
    apply_Flag_eeBadScFilter                      = cms.PSet(forMC = cms.bool(False), forData = cms.bool(True)),
    apply_Flag_globalSuperTightHalo2016Filter     = cms.PSet(forMC = cms.bool(True),  forData = cms.bool(True)),  # Not suggested for FastSim
    apply_Flag_globalTightHalo2016Filter          = cms.PSet(forMC = cms.bool(False), forData = cms.bool(False)),
    apply_Flag_goodVertices                       = cms.PSet(forMC = cms.bool(True),  forData = cms.bool(True)),
    apply_Flag_HBHENoiseFilter                    = cms.PSet(forMC = cms.bool(True),  forData = cms.bool(True)),
    apply_Flag_HBHENoiseIsoFilter                 = cms.PSet(forMC = cms.bool(True),  forData = cms.bool(True)),
    apply_Flag_hcalLaserEventFilter               = cms.PSet(forMC = cms.bool(False), forData = cms.bool(False)),
    apply_Flag_HcalStripHaloFilter                = cms.PSet(forMC = cms.bool(False), forData = cms.bool(False)),
    apply_Flag_METFilters                         = cms.PSet(forMC = cms.bool(False), forData = cms.bool(False)),
    apply_Flag_muonBadTrackFilter                 = cms.PSet(forMC = cms.bool(False), forData = cms.bool(False)),
    apply_Flag_trkPOGFilters                      = cms.PSet(forMC = cms.bool(False), forData = cms.bool(False)),
    apply_Flag_trkPOG_logErrorTooManyClusters     = cms.PSet(forMC = cms.bool(False), forData = cms.bool(False)),
    apply_Flag_trkPOG_manystripclus53X            = cms.PSet(forMC = cms.bool(False), forData = cms.bool(False)),
    apply_Flag_trkPOG_toomanystripclus53X         = cms.PSet(forMC = cms.bool(False), forData = cms.bool(False)),
)

recommendedMEtFilters_2017 = cms.PSet(
    apply_Flag_BadChargedCandidateFilter          = cms.PSet(forMC = cms.bool(False), forData = cms.bool(False)), # Under review
    apply_Flag_BadChargedCandidateSummer16Filter  = cms.PSet(forMC = cms.bool(False), forData = cms.bool(False)),
    apply_Flag_BadPFMuonFilter                    = cms.PSet(forMC = cms.bool(True),  forData = cms.bool(True)),
    apply_Flag_BadPFMuonSummer16Filter            = cms.PSet(forMC = cms.bool(False), forData = cms.bool(False)),
    apply_Flag_chargedHadronTrackResolutionFilter = cms.PSet(forMC = cms.bool(False), forData = cms.bool(False)),
    apply_Flag_CSCTightHalo2015Filter             = cms.PSet(forMC = cms.bool(False), forData = cms.bool(False)),
    apply_Flag_CSCTightHaloFilter                 = cms.PSet(forMC = cms.bool(False), forData = cms.bool(False)),
    apply_Flag_CSCTightHaloTrkMuUnvetoFilter      = cms.PSet(forMC = cms.bool(False), forData = cms.bool(False)),
    apply_Flag_ecalBadCalibFilter                 = cms.PSet(forMC = cms.bool(False), forData = cms.bool(False)),
    apply_Flag_ecalBadCalibFilterV2               = cms.PSet(forMC = cms.bool(True),  forData = cms.bool(True)),  # NEW!
    apply_Flag_EcalDeadCellBoundaryEnergyFilter   = cms.PSet(forMC = cms.bool(False), forData = cms.bool(False)),
    apply_Flag_EcalDeadCellTriggerPrimitiveFilter = cms.PSet(forMC = cms.bool(True),  forData = cms.bool(True)),
    apply_Flag_ecalLaserCorrFilter                = cms.PSet(forMC = cms.bool(False), forData = cms.bool(False)),
    apply_Flag_eeBadScFilter                      = cms.PSet(forMC = cms.bool(False), forData = cms.bool(True)),
    apply_Flag_globalSuperTightHalo2016Filter     = cms.PSet(forMC = cms.bool(True),  forData = cms.bool(True)),  # Not suggested for FastSim
    apply_Flag_globalTightHalo2016Filter          = cms.PSet(forMC = cms.bool(False), forData = cms.bool(False)),
    apply_Flag_goodVertices                       = cms.PSet(forMC = cms.bool(True),  forData = cms.bool(True)),
    apply_Flag_HBHENoiseFilter                    = cms.PSet(forMC = cms.bool(True),  forData = cms.bool(True)),
    apply_Flag_HBHENoiseIsoFilter                 = cms.PSet(forMC = cms.bool(True),  forData = cms.bool(True)),
    apply_Flag_hcalLaserEventFilter               = cms.PSet(forMC = cms.bool(False), forData = cms.bool(False)),
    apply_Flag_HcalStripHaloFilter                = cms.PSet(forMC = cms.bool(False), forData = cms.bool(False)),
    apply_Flag_METFilters                         = cms.PSet(forMC = cms.bool(False), forData = cms.bool(False)),
    apply_Flag_muonBadTrackFilter                 = cms.PSet(forMC = cms.bool(False), forData = cms.bool(False)),
    apply_Flag_trkPOGFilters                      = cms.PSet(forMC = cms.bool(False), forData = cms.bool(False)),
    apply_Flag_trkPOG_logErrorTooManyClusters     = cms.PSet(forMC = cms.bool(False), forData = cms.bool(False)),
    apply_Flag_trkPOG_manystripclus53X            = cms.PSet(forMC = cms.bool(False), forData = cms.bool(False)),
    apply_Flag_trkPOG_toomanystripclus53X         = cms.PSet(forMC = cms.bool(False), forData = cms.bool(False)),
)

recommendedMEtFilters_2018 = cms.PSet(
    apply_Flag_BadChargedCandidateFilter          = cms.PSet(forMC = cms.bool(False), forData = cms.bool(False)), # Under review
    apply_Flag_BadChargedCandidateSummer16Filter  = cms.PSet(forMC = cms.bool(False), forData = cms.bool(False)),
    apply_Flag_BadPFMuonFilter                    = cms.PSet(forMC = cms.bool(True),  forData = cms.bool(True)),
    apply_Flag_BadPFMuonSummer16Filter            = cms.PSet(forMC = cms.bool(False), forData = cms.bool(False)),
    apply_Flag_chargedHadronTrackResolutionFilter = cms.PSet(forMC = cms.bool(False), forData = cms.bool(False)),
    apply_Flag_CSCTightHalo2015Filter             = cms.PSet(forMC = cms.bool(False), forData = cms.bool(False)),
    apply_Flag_CSCTightHaloFilter                 = cms.PSet(forMC = cms.bool(False), forData = cms.bool(False)),
    apply_Flag_CSCTightHaloTrkMuUnvetoFilter      = cms.PSet(forMC = cms.bool(False), forData = cms.bool(False)),
    apply_Flag_ecalBadCalibFilter                 = cms.PSet(forMC = cms.bool(False), forData = cms.bool(False)),
    apply_Flag_ecalBadCalibFilterV2               = cms.PSet(forMC = cms.bool(True),  forData = cms.bool(True)),  # NEW!
    apply_Flag_EcalDeadCellBoundaryEnergyFilter   = cms.PSet(forMC = cms.bool(False), forData = cms.bool(False)),
    apply_Flag_EcalDeadCellTriggerPrimitiveFilter = cms.PSet(forMC = cms.bool(True),  forData = cms.bool(True)),
    apply_Flag_ecalLaserCorrFilter                = cms.PSet(forMC = cms.bool(False), forData = cms.bool(False)),
    apply_Flag_eeBadScFilter                      = cms.PSet(forMC = cms.bool(False), forData = cms.bool(True)),
    apply_Flag_globalSuperTightHalo2016Filter     = cms.PSet(forMC = cms.bool(True),  forData = cms.bool(True)),  # Not tested for FastSim
    apply_Flag_globalTightHalo2016Filter          = cms.PSet(forMC = cms.bool(False), forData = cms.bool(False)),
    apply_Flag_goodVertices                       = cms.PSet(forMC = cms.bool(True),  forData = cms.bool(True)),
    apply_Flag_HBHENoiseFilter                    = cms.PSet(forMC = cms.bool(True),  forData = cms.bool(True)),
    apply_Flag_HBHENoiseIsoFilter                 = cms.PSet(forMC = cms.bool(True),  forData = cms.bool(True)),
    apply_Flag_hcalLaserEventFilter               = cms.PSet(forMC = cms.bool(False), forData = cms.bool(False)),
    apply_Flag_HcalStripHaloFilter                = cms.PSet(forMC = cms.bool(False), forData = cms.bool(False)),
    apply_Flag_METFilters                         = cms.PSet(forMC = cms.bool(False), forData = cms.bool(False)),
    apply_Flag_muonBadTrackFilter                 = cms.PSet(forMC = cms.bool(False), forData = cms.bool(False)),
    apply_Flag_trkPOGFilters                      = cms.PSet(forMC = cms.bool(False), forData = cms.bool(False)),
    apply_Flag_trkPOG_logErrorTooManyClusters     = cms.PSet(forMC = cms.bool(False), forData = cms.bool(False)),
    apply_Flag_trkPOG_manystripclus53X            = cms.PSet(forMC = cms.bool(False), forData = cms.bool(False)),
    apply_Flag_trkPOG_toomanystripclus53X         = cms.PSet(forMC = cms.bool(False), forData = cms.bool(False)),
)
