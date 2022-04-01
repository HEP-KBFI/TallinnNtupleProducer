#include "TallinnNtupleProducer/Objects/interface/MEtFilterFlag.h"

#include <cassert> // assert()

const char *
getMEtFilterFlagString(MEtFilterFlag flag)
{
  switch(flag)
  {
    case MEtFilterFlag::BadChargedCandidateFilter:          return "Flag_BadChargedCandidateFilter";
    case MEtFilterFlag::BadChargedCandidateSummer16Filter:  return "Flag_BadChargedCandidateSummer16Filter";
    case MEtFilterFlag::BadPFMuonFilter:                    return "Flag_BadPFMuonFilter";
    case MEtFilterFlag::BadPFMuonSummer16Filter:            return "Flag_BadPFMuonSummer16Filter";
    case MEtFilterFlag::chargedHadronTrackResolutionFilter: return "Flag_chargedHadronTrackResolutionFilter";
    case MEtFilterFlag::CSCTightHalo2015Filter:             return "Flag_CSCTightHalo2015Filter";
    case MEtFilterFlag::CSCTightHaloFilter:                 return "Flag_CSCTightHaloFilter";
    case MEtFilterFlag::CSCTightHaloTrkMuUnvetoFilter:      return "Flag_CSCTightHaloTrkMuUnvetoFilter";
    case MEtFilterFlag::ecalBadCalibFilter:                 return "Flag_ecalBadCalibFilter";
    case MEtFilterFlag::ecalBadCalibFilterV2:               return "Flag_ecalBadCalibFilterV2";
    case MEtFilterFlag::EcalDeadCellBoundaryEnergyFilter:   return "Flag_EcalDeadCellBoundaryEnergyFilter";
    case MEtFilterFlag::EcalDeadCellTriggerPrimitiveFilter: return "Flag_EcalDeadCellTriggerPrimitiveFilter";
    case MEtFilterFlag::ecalLaserCorrFilter:                return "Flag_ecalLaserCorrFilter";
    case MEtFilterFlag::eeBadScFilter:                      return "Flag_eeBadScFilter";
    case MEtFilterFlag::globalSuperTightHalo2016Filter:     return "Flag_globalSuperTightHalo2016Filter";
    case MEtFilterFlag::globalTightHalo2016Filter:          return "Flag_globalTightHalo2016Filter";
    case MEtFilterFlag::goodVertices:                       return "Flag_goodVertices";
    case MEtFilterFlag::HBHENoiseFilter:                    return "Flag_HBHENoiseFilter";
    case MEtFilterFlag::HBHENoiseIsoFilter:                 return "Flag_HBHENoiseIsoFilter";
    case MEtFilterFlag::hcalLaserEventFilter:               return "Flag_hcalLaserEventFilter";
    case MEtFilterFlag::HcalStripHaloFilter:                return "Flag_HcalStripHaloFilter";
    case MEtFilterFlag::METFilters:                         return "Flag_METFilters";
    case MEtFilterFlag::muonBadTrackFilter:                 return "Flag_muonBadTrackFilter";
    case MEtFilterFlag::trkPOGFilters:                      return "Flag_trkPOGFilters";
    case MEtFilterFlag::trkPOG_logErrorTooManyClusters:     return "Flag_trkPOG_logErrorTooManyClusters";
    case MEtFilterFlag::trkPOG_manystripclus53X:            return "Flag_trkPOG_manystripclus53X";
    case MEtFilterFlag::trkPOG_toomanystripclus53X:         return "Flag_trkPOG_toomanystripclus53X";
    case MEtFilterFlag::LAST: break;
  }
  assert(0);
}
