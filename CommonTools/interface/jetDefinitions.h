#ifndef TallinnNtupleProducer_CommonTools_jetDefinitions_h
#define TallinnNtupleProducer_CommonTools_jetDefinitions_h

#include "TallinnNtupleProducer/CommonTools/interface/Era.h"

#include <map>    // std::map
#include <string> // std::string
#include <vector> // std::vector

//--- declare b-tagging algorithms
enum class Btag
{
  kDeepJet, kDeepCSV, kCSVv2
};

//--- declare pileup jet ID working points
enum class pileupJetID 
{
  // The encoding of the pileup jet ID working points is:
  //   puId==0 means 000: fail all PU ID;
  //   puId==4 means 100: pass loose ID, fail medium, fail tight;
  //   puId==6 means 110: pass loose and medium ID, fail tight;
  //   puId==7 means 111: pass loose, medium, tight ID. 
  // https://twiki.cern.ch/twiki/bin/viewauth/CMS/PileupJetID#miniAOD_and_nanoAOD
  kPileupJetID_disabled = 0, kPileupJetID_loose = 4, kPileupJetID_medium = 2, kPileupJetID_tight = 1
};

pileupJetID
get_pileupJetID(const std::string & pileupJetID_str);

//--- declare AK8 jet corrections
enum 
{
  kFatJetNone  = 0,
  kFatJetJMS   = 1 << 0,
  kFatJetJMR   = 1 << 1,
  kFatJetPUPPI = 1 << 2,
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

double
get_BtagWP(Era era,
           Btag btag,
           BtagWP wp);

#endif // TallinnNtupleProducer_CommonTools_jetDefinitions_h
