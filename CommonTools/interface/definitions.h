#ifndef TallinnNtupleProducer_CommonTools_definitions_h
#define TallinnNtupleProducer_CommonTools_definitions_h

//--- declare constants
const double wBosonMass = 80.379; // GeV
const double z_mass   = 91.1876;
const double z_window = 10.;
const double met_coef =  0.6;
const double mht_coef =  0.4;
const double w_mass   = 80.385;
const double w_window = 10.;

//--- declare data-taking periods
enum class Era
{
  kUndefined, k2016, k2017, k2018
};

//--- declare b-tagging algorithms
enum class Btag
{
  kDeepJet, kDeepCSV, kCSVv2
};

//--- declare pileup jet ID working points
enum pileupJetID {
  // The encoding of the pileup jet ID working points is:
  //   puId==0 means 000: fail all PU ID;
  //   puId==4 means 100: pass loose ID, fail medium, fail tight;
  //   puId==6 means 110: pass loose and medium ID, fail tight;
  //   puId==7 means 111: pass loose, medium, tight ID. 
  // https://twiki.cern.ch/twiki/bin/viewauth/CMS/PileupJetID#miniAOD_and_nanoAOD
  kPileupJetID_disabled = 0, kPileupJetID_loose = 4, kPileupJetID_medium = 2, kPileupJetID_tight = 1
};

//--- declare selection criteria for leptons and hadronic taus
enum { kLoose, kFakeable, kTight };

//--- AK8 jet corrections
enum {
  kFatJetNone  = 0,
  kFatJetJMS   = 1 << 0,
  kFatJetJMR   = 1 << 1,
  kFatJetPUPPI = 1 << 2,
};

//--- declare tau ID discriminators

enum class TauID {
  MVAnewDM2017v2, MVAoldDM, MVAoldDMdR032017v2, MVAoldDM2017v1, MVAoldDM2017v2,
  DeepTau2017v2VSe, DeepTau2017v2VSmu, DeepTau2017v2VSjet
};

const std::map<TauID, int> TauID_levels = {
  { TauID::MVAnewDM2017v2,     7 }, // VVLoose  - 1, VVTight - 7
  { TauID::MVAoldDM,           6 }, // VLoose   - 1, VVTight - 7
  { TauID::MVAoldDMdR032017v2, 7 }, // VVLoose  - 1, VVTight - 7
  { TauID::MVAoldDM2017v1,     7 }, // VVLoose  - 1, VVTight - 7
  { TauID::MVAoldDM2017v2,     7 }, // VVLoose  - 1, VVTight - 7
  { TauID::DeepTau2017v2VSe,   8 }, // VVVLoose - 1, VVTight - 8
  { TauID::DeepTau2017v2VSmu,  4 }, // VLoose   - 1, Tight   - 4
  { TauID::DeepTau2017v2VSjet, 8 }, // VVVLoose - 1, VVTight - 8
};

const std::map<int, std::vector<std::string>> TauID_level_strings = {
  { 1, {                                                     "Tight"                      } },
  { 2, {                                  "Loose",           "Tight"                      } },
  { 4, {                        "VLoose", "Loose", "Medium", "Tight"                      } },
  { 5, {                        "VLoose", "Loose", "Medium", "Tight", "VTight"            } },
  { 6, {                        "VLoose", "Loose", "Medium", "Tight", "VTight", "VVTight" } },
  { 7, {             "VVLoose", "VLoose", "Loose", "Medium", "Tight", "VTight", "VVTight" } },
  { 8, { "VVVLoose", "VVLoose", "VLoose", "Loose", "Medium", "Tight", "VTight", "VVTight" } },
};

const std::map<TauID, std::string> TauID_names = {
  { TauID::MVAnewDM2017v2,     "MVAnewDM2017v2"     },
  { TauID::MVAoldDM,           "MVAoldDM"           },
  { TauID::MVAoldDMdR032017v2, "MVAoldDMdR032017v2" },
  { TauID::MVAoldDM2017v1,     "MVAoldDM2017v1"     },
  { TauID::MVAoldDM2017v2,     "MVAoldDM2017v2"     },
  { TauID::DeepTau2017v2VSe,   "DeepTau2017v2VSe"   },
  { TauID::DeepTau2017v2VSmu,  "DeepTau2017v2VSmu"  },
  { TauID::DeepTau2017v2VSjet, "DeepTau2017v2VSjet" },
};

// NB! must be exactly 7 characters!
const std::map<std::string, TauID> TauID_PyMap = {
  { "dR03mva", TauID::MVAoldDMdR032017v2 },
  { "dR05mva", TauID::MVAoldDM2017v2     },
  { "deepVSj", TauID::DeepTau2017v2VSjet },
};

enum class EGammaID {
  Fall17V1noIso, Fall17V1Iso,
  Fall17V2noIso, Fall17V2Iso,
};

enum class EGammaWP {
  WP90, WP80, WPL
};

// read only Fall17V2noIso
const std::map<EGammaID, std::string> EGammaID_map = {
//  { EGammaID::Fall17V1noIso, "mvaFall17V1noIso" },
//  { EGammaID::Fall17V1Iso,   "mvaFall17V1Iso"   },
  { EGammaID::Fall17V2noIso, "mvaFall17V2noIso" },
  { EGammaID::Fall17V2Iso,   "mvaFall17V2Iso"   },
};

const std::map<EGammaWP, std::string> EGammaWP_map = {
  { EGammaWP::WP90, "WP90" },
  { EGammaWP::WP80, "WP80" },
  { EGammaWP::WPL,  "WPL"  },
};

//--- declare b-tagging working points

enum class BtagWP { kLoose, kMedium, kTight };

const std::map<Era, std::map<Btag, std::map<BtagWP, double>>> BtagWP_map = {
  {
    Era::k2016, {
//--- source: https://twiki.cern.ch/twiki/bin/viewauth/CMS/BtagRecommendation2016Legacy
      {
        Btag::kDeepJet, {
          { BtagWP::kLoose,  0.0614 },
          { BtagWP::kMedium, 0.3093 },
          { BtagWP::kTight,  0.7221 },
        },
      },
//--- source: https://twiki.cern.ch/twiki/bin/viewauth/CMS/BtagRecommendation2016Legacy
      {
        Btag::kDeepCSV, {
          { BtagWP::kLoose,  0.2217 },
          { BtagWP::kMedium, 0.6321 },
          { BtagWP::kTight,  0.8953 },
        }
      },
//--- source: https://twiki.cern.ch/twiki/bin/viewauth/CMS/BtagRecommendation80XReReco
      {
        Btag::kCSVv2, {
          { BtagWP::kLoose,  0.5426 },
          { BtagWP::kMedium, 0.8484 },
          { BtagWP::kTight,  0.9535 },
        },
      },
    },
  },
  {
    Era::k2017, {
//--- source: https://twiki.cern.ch/twiki/bin/viewauth/CMS/BtagRecommendation94X
      {
        Btag::kDeepJet, {
          { BtagWP::kLoose,  0.0521 },
          { BtagWP::kMedium, 0.3033 },
          { BtagWP::kTight,  0.7489 },
        },
      },
//--- source: https://twiki.cern.ch/twiki/bin/viewauth/CMS/BtagRecommendation94X
      {
        Btag::kDeepCSV, {
          { BtagWP::kLoose,  0.1522 },
          { BtagWP::kMedium, 0.4941 },
          { BtagWP::kTight,  0.8001 },
        },
      },
//--- source: https://twiki.cern.ch/twiki/bin/viewauth/CMS/BtagRecommendation94X
      {
        Btag::kCSVv2, {
          { BtagWP::kLoose,  0.5803 },
          { BtagWP::kMedium, 0.8838 },
          { BtagWP::kTight,  0.9693 },
        },
      },
    },
  },
  {
    Era::k2018, {
//--- source: https://twiki.cern.ch/twiki/bin/viewauth/CMS/BtagRecommendation102X
      {
        Btag::kDeepJet, {
          { BtagWP::kLoose,  0.0494 },
          { BtagWP::kMedium, 0.2770 },
          { BtagWP::kTight,  0.7264 },
        },
      },
//--- source: https://twiki.cern.ch/twiki/bin/viewauth/CMS/BtagRecommendation102X
      {
        Btag::kDeepCSV, {
          { BtagWP::kLoose,  0.1241 },
          { BtagWP::kMedium, 0.4184 },
          { BtagWP::kTight,  0.7527 },
        },
      },
    },
  },
};

#endif // TallinnNtupleProducer_CommonTools_definitions_h
