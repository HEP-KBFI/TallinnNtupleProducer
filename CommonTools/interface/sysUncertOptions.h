#ifndef TallinnNtupleProducer_CommonTools_sysUncertOptions_h
#define TallinnNtupleProducer_CommonTools_sysUncertOptions_h

#include <string> // std::string
#include <map> // std::map<,>
#include <vector> // std::vector<>

enum class Btag;
enum class Era;

//--- declare systematic uncertainties on data/MC corrections for
//    b-tagging efficiency and mistag rates
enum
{
  kBtag_central,
  kBtag_noBtagSF,

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
  kJetMET_jesHEMDown,
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
  kFatJet_jesHEMDown,
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
  shift_2lssEEUp,   shift_2lssEEDown,
  shift_2lssEMuUp,  shift_2lssEMuDown,
  shift_2lssMuMuUp, shift_2lssMuMuDown,
  shift_1l1tauUp,   shift_1l1tauDown,
  shift_0l2tauUp,   shift_0l2tauDown,
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
};

enum
{
  kPartonShower_central,
  kPartonShower_ISRUp, kPartonShower_ISRDown, // ISR -- initial state radiation
  kPartonShower_FSRUp, kPartonShower_FSRDown, // FSR -- final state radiation
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

extern const std::string pdfSysStr;
extern const std::string noBtagWeightStr;

// theory uncertainties
extern const std::map<std::string, int> lheScaleSysMap;
extern const std::map<std::string, int> psSysMap;
extern const std::map<std::string, PDFSys> pdfSysMap;

// experimental uncertainties
extern const std::map<std::string, L1PreFiringWeightSys> l1prefireSysMap;
extern const std::map<std::string, PUsys> puSysMap;
extern const std::map<std::string, TriggerSFsys> triggerSFSysMap;
extern const std::map<std::string, TriggerSFsys> triggerSFSysMap_2lss;
extern const std::map<std::string, TriggerSFsys> triggerSFSysMap_1l_1tau;
extern const std::map<std::string, TriggerSFsys> triggerSFSysMap_0l_2tau;

// AK4 jet uncertainties
extern const std::map<std::string, int> btagWeightSysMap;
extern const std::map<std::string, int> btagWeightJESsplitSysMap;
extern const std::map<std::string, pileupJetIDSFsys> pileupJetIDSysMap;

extern const std::map<std::string, int> jesAK4SysMap;
extern const std::map<std::string, int> jesSplitAK4SysMap;
extern const std::map<std::string, int> jerAK4SysMap;
extern const std::map<std::string, int> jerSplitAK4SysMap;

// MET uncertainties
extern const std::map<std::string, int> metSysMap;

// AK8 jet uncertainties
extern const std::map<std::string, int> jesAK8SysMap;
extern const std::map<std::string, int> jesSplitAK8SysMap;
extern const std::map<std::string, int> jerAK8SysMap;
extern const std::map<std::string, int> jerSplitAK8SysMap;
extern const std::map<std::string, int> jmsAK8SysMap;
extern const std::map<std::string, int> jmrAK8SysMap;
extern const std::map<std::string, SubjetBtagSys> subjetBtagSysMap; // NOT WRITTEN YET

// tau uncertainties
extern const std::map<std::string, int> hadTauESSysMap;
extern const std::map<std::string, int> jetToTauFRSysMap;
extern const std::map<std::string, FRet> eToTauFRSysMap;
extern const std::map<std::string, FRmt> mToTauFRSysMap;
extern const std::map<std::string, TauIDSFsys> tauIDSFSysMap;

// lepton uncertainties
extern const std::map<std::string, LeptonIDSFsys> leptonIDSFSysMap;
extern const std::map<std::string, int> jetToLeptonFRSysMap;
extern const std::map<std::string, ElectronPtSys> ePtSysMap;
extern const std::map<std::string, MuonPtSys> mPtSysMap;

// DY uncertainties
extern const std::map<std::string, int> dyMCRwgtSysMap;
extern const std::map<std::string, int> dyMCNormSysMap;

// top uncertainties
extern const std::map<std::string, int> topPtRwgtSysMap;

// EWK uncertainties
extern const std::map<std::string, EWKJetSys> ewkJetSysMap;
extern const std::map<std::string, EWKBJetSys> ewkBJetSysMap;
extern const std::map<std::string, LHEVptSys> lheVptSysMap;

// ------------------------
// mapping of b-tagging systematics enums to correctionlib strings
extern const std::map<int, std::string> btagWeightSysMap_correctionLib;

std::vector<std::string>
get_inclusive_systeatics(const std::vector<std::string> & systematics);

bool
isValidJESsource(Era era,
                 int central_or_shift,
                 bool isFatJet = false);

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

EWKJetSys
getEWKJetSys_option(const std::string & central_or_shift);

EWKBJetSys
getEWKBJetSys_option(const std::string & central_or_shift);

PDFSys
getPDFSys_option(const std::string & central_or_shift);

LHEVptSys
getLHEVptSys_option(const std::string & central_or_shift);

SubjetBtagSys
getSubjetBtagSys_option(const std::string & central_or_shift);

bool
isPDFsys_member(const std::string & central_or_shift);

std::string
getPDFsys_str(int memberIdx);

void
checkOptionValidity(const std::string & central_or_shift,
                    bool isMC);

#endif // TallinnNtupleProducer_CommonTools_sysUncertOptions_h
