import FWCore.ParameterSet.Config as cms

from TallinnNtupleProducer.EvtWeightTools.HHCoupling_cfi import hhCoupling as config_hhCoupling

evtReweight_HH = cms.PSet(
    pluginType = cms.string("EvtReweightWriter_HH"),
    apply_rwgt_lo = cms.bool(False),
    apply_rwgt_nlo = cms.bool(True),
    hhCoupling = config_hhCoupling,
    coefFile = cms.string('TallinnNtupleProducer/EvtWeightTools/data/HHReweighting/coefficientsByBin_extended_3M_costHHSim_19-4.txt')
)
