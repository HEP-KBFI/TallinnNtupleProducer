#ifndef SYSUNCEROPTIONS_H
#define SYSUNCEROPTIONS_H

#include "tthAnalysis/HiggsToTauTau/interface/analysisAuxFunctions.h" // Era

#include <string> // std::string

enum class Btag;

//--- declare systematic uncertainties on data/MC corrections for
//    b-tagging efficiency and mistag rates
enum
{
  kBtag_central,

  kBtag_hfUp,       kBtag_hfDown,
  kBtag_hfStats1Up, kBtag_hfStats1Down,
  kBtag_hfStats2Up, kBtag_hfStats2Down,

  kBtag_lfUp,       kBtag_lfDown,
  kBtag_lfStats1Up, kBtag_lfStats1Down,
  kBtag_lfStats2Up, kBtag_lfStats2Down,

  kBtag_cErr1Up, kBtag_cErr1Down,
  kBtag_cErr2Up, kBtag_cErr2Down,

  kBtag_jesTotalUp,              kBtag_jesTotalDown,
  kBtag_jesAbsoluteUp,           kBtag_jesAbsoluteDown,
  kBtag_jesAbsolute_EraUp,       kBtag_jesAbsolute_EraDown,
  kBtag_jesBBEC1Up,              kBtag_jesBBEC1Down,
  kBtag_jesBBEC1_EraUp,          kBtag_jesBBEC1_EraDown,
  kBtag_jesEC2Up,                kBtag_jesEC2Down,
  kBtag_jesEC2_EraUp,            kBtag_jesEC2_EraDown,
  kBtag_jesFlavorQCDUp,          kBtag_jesFlavorQCDDown,
  kBtag_jesHFUp,                 kBtag_jesHFDown,
  kBtag_jesHF_EraUp,             kBtag_jesHF_EraDown,
  kBtag_jesRelativeBalUp,        kBtag_jesRelativeBalDown,
  kBtag_jesRelativeSample_EraUp, kBtag_jesRelativeSample_EraDown,
};

//--- declare systematic uncertainties on data/MC corrections for
//    efficiency and mistag rates for jets to pass the pileup jet ID
enum class pileupJetIDSFsys
{
  central,
  effUp,    effDown,
  mistagUp, mistagDown,
};

//--- declare systematic uncertainties on data/MC corrections for jet pt & mass and MET pt & phi
enum
{
  kJetMET_central_nonNominal,  // for data and sync Ntuples
  kJetMET_central,             // for MC
  kJetMET_jesUp,                   kJetMET_jesDown, // total JES
  // regrouped, sources with era in their names are treated as uncorrelated, the remaining are correlated
  kJetMET_jesAbsoluteUp,           kJetMET_jesAbsoluteDown,
  kJetMET_jesAbsolute_EraUp,       kJetMET_jesAbsolute_EraDown,
  kJetMET_jesBBEC1Up,              kJetMET_jesBBEC1Down,
  kJetMET_jesBBEC1_EraUp,          kJetMET_jesBBEC1_EraDown,
  kJetMET_jesEC2Up,                kJetMET_jesEC2Down,
  kJetMET_jesEC2_EraUp,            kJetMET_jesEC2_EraDown,
  kJetMET_jesFlavorQCDUp,          kJetMET_jesFlavorQCDDown,
  kJetMET_jesHFUp,                 kJetMET_jesHFDown,
  kJetMET_jesHF_EraUp,             kJetMET_jesHF_EraDown,
  kJetMET_jesRelativeBalUp,        kJetMET_jesRelativeBalDown,
  kJetMET_jesRelativeSample_EraUp, kJetMET_jesRelativeSample_EraDown,
  kJetMET_jesHEMUp,                kJetMET_jesHEMDown,
  // JER
  kJetMET_jerUp,                   kJetMET_jerDown, // total JER
  kJetMET_jerBarrelUp,             kJetMET_jerBarrelDown,
  kJetMET_jerEndcap1Up,            kJetMET_jerEndcap1Down,
  kJetMET_jerEndcap2LowPtUp,       kJetMET_jerEndcap2LowPtDown,
  kJetMET_jerEndcap2HighPtUp,      kJetMET_jerEndcap2HighPtDown,
  kJetMET_jerForwardLowPtUp,       kJetMET_jerForwardLowPtDown,
  kJetMET_jerForwardHighPtUp,      kJetMET_jerForwardHighPtDown,
  // unclustered energy (only for MET)
  kJetMET_UnclusteredEnUp,         kJetMET_UnclusteredEnDown,
};

enum
{
  kFatJet_central_nonNominal,     // for data and sync Ntuples
  kFatJet_central,                // for MC
  kFatJet_jesUp,                   kFatJet_jesDown, // total JES
  // regrouped JES
  kFatJet_jesAbsoluteUp,           kFatJet_jesAbsoluteDown,
  kFatJet_jesAbsolute_EraUp,       kFatJet_jesAbsolute_EraDown,
  kFatJet_jesBBEC1Up,              kFatJet_jesBBEC1Down,
  kFatJet_jesBBEC1_EraUp,          kFatJet_jesBBEC1_EraDown,
  kFatJet_jesEC2Up,                kFatJet_jesEC2Down,
  kFatJet_jesEC2_EraUp,            kFatJet_jesEC2_EraDown,
  kFatJet_jesFlavorQCDUp,          kFatJet_jesFlavorQCDDown,
  kFatJet_jesHFUp,                 kFatJet_jesHFDown,
  kFatJet_jesHF_EraUp,             kFatJet_jesHF_EraDown,
  kFatJet_jesRelativeBalUp,        kFatJet_jesRelativeBalDown,
  kFatJet_jesRelativeSample_EraUp, kFatJet_jesRelativeSample_EraDown,
  kFatJet_jesHEMUp,                kFatJet_jesHEMDown,
  kFatJet_jerUp,                   kFatJet_jerDown, // total JER
  // split JER
//  kFatJet_jerBarrelUp,             kFatJet_jerBarrelDown,
//  kFatJet_jerEndcap1Up,            kFatJet_jerEndcap1Down,
//  kFatJet_jerEndcap2LowPtUp,       kFatJet_jerEndcap2LowPtDown,
//  kFatJet_jerEndcap2HighPtUp,      kFatJet_jerEndcap2HighPtDown,
//  kFatJet_jerForwardLowPtUp,       kFatJet_jerForwardLowPtDown,
//  kFatJet_jerForwardHighPtUp,      kFatJet_jerForwardHighPtDown,
  kFatJet_jmsUp,                   kFatJet_jmsDown, // mass scale
  kFatJet_jmrUp,                   kFatJet_jmrDown, // mass resolution
};

enum class METSyst
{
  central,
  respUp, respDown,
  resolUp, resolDown,
};

enum
{
  kHadTauPt_uncorrected,
  kHadTauPt_central,
  kHadTauPt_shiftUp, kHadTauPt_shiftDown,
};

enum
{
  kFRjt_central,
  kFRjt_normUp,  kFRjt_normDown,
  kFRjt_shapeUp, kFRjt_shapeDown,
};

enum class FRet
{
  central,
  shiftUp, shiftDown,
};

enum class FRmt
{
  central,
  shiftUp, shiftDown,
};

enum class LeptonIDSFsys
{
  central,
  elTightUp,       elTightDown,
  muTightUp,       muTightDown,
  elLooseUp,       elLooseDown,
  muLooseUp,       muLooseDown,
  elTightRecompUp, elTightRecompDown,
  muTightRecompUp, muTightRecompDown,
};

enum class TauIDSFsys
{
  central,
  shiftUp, shiftDown,
};

enum class TriggerSFsys
{
  central,
  shiftUp,          shiftDown,
  shift_2lssUp,     shift_2lssDown,
  shift_2lssEEUp,   shift_2lssEEDown,
  shift_2lssEMuUp,  shift_2lssEMuDown,
  shift_2lssMuMuUp, shift_2lssMuMuDown,
  shift_3lUp,       shift_3lDown,
  shift_1l2tauUp,   shift_1l2tauDown,
  shift_1l1tauUp,   shift_1l1tauDown,
  shift_0l2tauUp,   shift_0l2tauDown,
  shift_1lEUp,      shift_1lEDown,
  shift_1lMuUp,     shift_1lMuDown,
};

enum class TriggerSFsysChoice
{
  any,
  leptonOnly,
  hadTauOnly,
};

enum
{
  kLHE_scale_central,
  kLHE_scale_xUp,  kLHE_scale_xDown,  // factorization scale
  kLHE_scale_yUp,  kLHE_scale_yDown,  // renormalization scale
  kLHE_scale_xyUp, kLHE_scale_xyDown, // simultaneous factorization and renormalization scales
  kLHE_scale_Up,   kLHE_scale_Down,   // envelope (Swiss cross + both up + both down)
};

enum
{
  kPartonShower_central,
  kPartonShower_ISRUp, kPartonShower_ISRDown, // ISR -- initial state radiation
  kPartonShower_FSRUp, kPartonShower_FSRDown, // FSR -- final state radiation
  kPartonShower_Up,    kPartonShower_Down, // envelope from the variations of both ISR and FSR
};

enum
{
  kTopPtReweighting_central,
  kTopPtReweighting_shiftUp, kTopPtReweighting_shiftDown,
};

enum class ElectronPtSys
{
  central,
  scaleUp_barrel, scaleDown_barrel,
  scaleUp_endcap, scaleDown_endcap,
  resUp,          resDown,
  uncorrected
};

enum class MuonPtSys
{
  central,
  ERUp,        ERDown,
  ESBarrel1Up, ESBarrel1Down,
  ESBarrel2Up, ESBarrel2Down,
  ESEndcap1Up, ESEndcap1Down,
  ESEndcap2Up, ESEndcap2Down,
};

enum
{
  kFRl_central,
  kFRe_shape_ptUp,         kFRe_shape_ptDown,
  kFRe_shape_normUp,       kFRe_shape_normDown,
  kFRe_shape_eta_barrelUp, kFRe_shape_eta_barrelDown,
  kFRe_shape_corrUp,       kFRe_shape_corrDown,
  kFRm_shape_ptUp,         kFRm_shape_ptDown,
  kFRm_shape_normUp,       kFRm_shape_normDown,
  kFRm_shape_eta_barrelUp, kFRm_shape_eta_barrelDown,
  kFRm_shape_corrUp,       kFRm_shape_corrDown,
};

enum class PUsys
{
  central,
  up, down,
};

enum class L1PreFiringWeightSys
{
  nominal, up, down
};

enum
{
  kDYMCReweighting_central,
  kDYMCReweighting_shiftUp, kDYMCReweighting_shiftDown
};

enum
{
  kDYMCNormScaleFactors_central,
  kDYMCNormScaleFactors_shiftUp, kDYMCNormScaleFactors_shiftDown
};

enum class MEMsys
{
  nominal, up, down
};

enum class EWKJetSys
{
  central,
  up, down,
};

enum class EWKBJetSys
{
  central,
  up, down,
};

enum class PDFSys
{
  central,
  up, down,
};

enum class LHEVptSys
{
  central,
  up, down,
};

enum class SubjetBtagSys
{
  central,
  up, down,
};

bool
isTTbarSys(const std::string & central_or_shift);

bool
isValidJESsource(Era era,
                 int central_or_shift,
                 bool isFatJet = false);

bool
isValidFatJetAttribute(int central_or_shift,
                       const std::string & attribute_name);

/**
 * @brief Return branchName to read weights that need to be applied, per jet, to MC events
 *       in order to correct for data/MC differences in b-tagging efficiency and mistag rates
 */
int
getBTagWeight_option(const std::string & central_or_shift);

pileupJetIDSFsys
getPileupJetIDSFsys_option(const std::string & central_or_shift);

int
getJet_option(const std::string & central_or_shift,
              bool isMC);

int
getMET_option(const std::string & central_or_shift,
              bool isMC);

METSyst
getMETsyst_option(const std::string & central_or_shift);

int
getFatJet_option(const std::string & central_or_shift,
                 bool isMC);

int
getHadTauPt_option(const std::string & central_or_shift);

int
getJetToTauFR_option(const std::string & central_or_shift);

FRet
getEToTauFR_option(const std::string & central_or_shift);

FRmt
getMuToTauFR_option(const std::string & central_or_shift);

LeptonIDSFsys
getLeptonIDSFsys_option(const std::string & central_or_shift);

TauIDSFsys
getTauIDSFsys_option(const std::string & central_or_shift);

TriggerSFsys
getTriggerSF_option(const std::string & central_or_shift,
                    TriggerSFsysChoice choice);

int
getLHEscale_option(const std::string & central_or_shift);

int
getPartonShower_option(const std::string & central_or_shift);

ElectronPtSys
getElectronPt_option(const std::string & central_or_shift,
                     bool isMC);

MuonPtSys
getMuon_option(const std::string & central_or_shift,
               bool isMC);

int
getJetToLeptonFR_option(const std::string & central_or_shift);

PUsys
getPUsys_option(const std::string & central_or_shift);

L1PreFiringWeightSys
getL1PreFiringWeightSys_option(const std::string & central_or_shift);

int
getDYMCReweighting_option(const std::string & central_or_shift);

int
getDYMCNormScaleFactors_option(const std::string & central_or_shift);

int
getTopPtReweighting_option(const std::string & central_or_shift);

MEMsys
getMEMsys_option(const std::string & central_or_shift);

EWKJetSys
getEWKJetSys_option(const std::string & central_or_shift);

EWKBJetSys
getEWKBJetSys_option(const std::string & central_or_shift);

PDFSys
getPDFSys_option(const std::string & central_or_shift);

bool
isPDFsys_member(const std::string & central_or_shift);

LHEVptSys
getLHEVptSys_option(const std::string & central_or_shift);

SubjetBtagSys
getSubjetBtagSys_option(const std::string & central_or_shift);

void
checkOptionValidity(const std::string & central_or_shift,
                    bool isMC);

std::string
getBranchName_bTagWeight(Btag btag,
                         Era era,
                         const std::string & default_collectionName,
                         int central_or_shift);

/**
 * @brief Return branch name to read RecoJet pt and mass
 *
 * isPt -- if true, return pT branch name; if false, return mass branch name
 */
std::string
getBranchName_jetMET(const std::string & default_branchName,
                     Era era,
                     int central_or_shift,
                     bool isPt);

std::string
getBranchName_fatJet(const std::string & default_branchName,
                     Era era,
                     const std::string & attribute_name,
                     int central_or_shift);

/**
 * @brief Return branch name to read PU weights
 */
std::string
getBranchName_pileup(PUsys puSys_option);

#endif // SYSUNCEROPTIONS_H
