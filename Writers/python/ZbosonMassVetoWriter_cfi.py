import FWCore.ParameterSet.Config as cms

ZbosonMassVeto = cms.PSet(
    z_mass = cms.double(91.1876),
    z_window = cms.double(10.),
    requireOS = cms.bool(False)
)
