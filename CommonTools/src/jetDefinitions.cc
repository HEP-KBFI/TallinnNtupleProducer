#include "TallinnNtupleProducer/CommonTools/interface/jetDefinitions.h"

#include "TallinnNtupleProducer/CommonTools/interface/cmsException.h" // cmsException

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

pileupJetID
get_pileupJetID(const std::string & pileupJetID_str)
{
  if     (pileupJetID_str == "disabled" || pileupJetID_str.empty()) return pileupJetID::kPileupJetID_disabled;
  else if(pileupJetID_str == "loose"   )                            return pileupJetID::kPileupJetID_loose;
  else if(pileupJetID_str == "medium"  )                            return pileupJetID::kPileupJetID_medium;
  else if(pileupJetID_str == "tight"   )                            return pileupJetID::kPileupJetID_tight;
  else throw cmsException(__func__, __LINE__) << "Invalid argument: " << pileupJetID_str;
}

double
get_BtagWP(Era era,
           Btag btag,
           BtagWP wp)
{
  return BtagWP_map.at(era).at(btag).at(wp);
}

int
get_fatJet_corrections(const std::vector<std::string> & corrections)
{
  int code = 0;
  for(const std::string & correction: corrections)
  {
    if(correction == "JMS")
    {
      code |= kFatJetJMS;
    }
    else if(correction == "JMR")
    {
      code |= kFatJetJMR;
    }
    else if(correction == "PUPPI")
    {
      code |= kFatJetPUPPI;
    }
    else
    {
      throw cmsException(__func__, __LINE__) << "Invalid correction name: " << correction;
    }
  }
  return code;
}
