#include "TallinnNtupleProducer/EvtWeightTools/interface/hadTauFakeRateAuxFunctions.h"

#include "TallinnNtupleProducer/CommonTools/interface/cmsException.h" // cmsException()
#include "TallinnNtupleProducer/EvtWeightTools/interface/hltFilter.h" // kTauFilterBit*

#include "TString.h"                                                  // Form()

#include <boost/algorithm/string/replace.hpp>                         // boost::replace_all_copy()

#include <assert.h>                                                   // assert()

std::string
getEtaBin(double minAbsEta,
          double maxAbsEta)
{
  std::string absEtaBin;
  if     (minAbsEta > 0. && maxAbsEta > 0.) absEtaBin = Form("absEta%1.1fto%1.1f", minAbsEta, maxAbsEta);
  else if(minAbsEta > 0.                  ) absEtaBin = Form("absEtaGt%1.1f", minAbsEta);
  else if(                  maxAbsEta > 0.) absEtaBin = Form("absEtaLt%1.1f", maxAbsEta);
  else if(minAbsEta < 0. && maxAbsEta < 0.){ 
    absEtaBin = "incl";
  }else{
    assert(0);
  }
  return boost::replace_all_copy(absEtaBin, ".", "_");
}

std::string
getPtBin(double minPt,
         double maxPt)
{
  std::string PtBin = "";
  if     (minPt > 0. && maxPt > 0.) PtBin = Form("Pt%1.1fto%1.1f", minPt, maxPt);
  else if(minPt > 0.              ) PtBin = Form("PtGt%1.1f", minPt);
  else if(              maxPt > 0.) PtBin = Form("PtLt%1.1f", maxPt);
  else if(minPt < 0. && maxPt < 0.) PtBin = "incl";                          
  else                              assert(0);
  return boost::replace_all_copy(PtBin, ".", "_");
}

int
getTrigMatchingOption_2016(const std::string& trigMatching)
{
  int filterBit;
  // HLT filter names specific to 2016 data-taking period
  if      ( trigMatching == "withoutTriggerMatching"                ) filterBit = kTauFilterBit2016_notApplied;
  else if ( trigMatching == "passesTriggerMatchingLooseIso"         ) filterBit = kTauFilterBit2016_passesLooseIso;
  else if ( trigMatching == "failsTriggerMatchingLooseIso"          ) filterBit = kTauFilterBit2016_failsLooseIso;
  else if ( trigMatching == "passesTriggerMatchingMediumIso"        ) filterBit = kTauFilterBit2016_passesMediumIso;
  else if ( trigMatching == "failsTriggerMatchingMediumIso"         ) filterBit = kTauFilterBit2016_failsMediumIso;
  else throw cmsException(__func__, __LINE__)
          << "Invalid parameter 'trigMatching' = " << trigMatching << " !!";
  return filterBit;
}

int
getTrigMatchingOption_2017and2018(const std::string& trigMatching)
{
  int filterBit;
  // HLT filter names specific to 2017 and 2018 data-taking periods
  if      ( trigMatching == "withoutTriggerMatching"                ) filterBit = kTauFilterBit2017and2018_notApplied;
  else if ( trigMatching == "passesTriggerMatchingLooseChargedIso"  ) filterBit = kTauFilterBit2017and2018_passesLooseChargedIso;
  else if ( trigMatching == "failsTriggerMatchingLooseChargedIso"   ) filterBit = kTauFilterBit2017and2018_failsLooseChargedIso;
  else if ( trigMatching == "passesTriggerMatchingMediumChargedIso" ) filterBit = kTauFilterBit2017and2018_passesMediumChargedIso;
  else if ( trigMatching == "failsTriggerMatchingMediumChargedIso"  ) filterBit = kTauFilterBit2017and2018_failsMediumChargedIso;
  else if ( trigMatching == "passesTriggerMatchingTightChargedIso"  ) filterBit = kTauFilterBit2017and2018_passesTightChargedIso;
  else if ( trigMatching == "failsTriggerMatchingTightChargedIso"   ) filterBit = kTauFilterBit2017and2018_failsTightChargedIso;
  else throw cmsException(__func__, __LINE__)
          << "Invalid parameter 'trigMatching' = " << trigMatching << " !!";
  return filterBit;
}
