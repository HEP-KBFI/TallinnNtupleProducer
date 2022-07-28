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

enum class BtagWP { kUndefined, kLoose, kMedium, kTight };

extern const std::map<Era, std::map<Btag, std::map<BtagWP, double>>> BtagWP_map;

double
get_BtagWP(Era era,
           Btag btag,
           BtagWP wp);

int
get_fatJet_corrections(const std::vector<std::string> & corrections);

#endif // TallinnNtupleProducer_CommonTools_jetDefinitions_h
