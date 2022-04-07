#include "tthAnalysis/HiggsToTauTau/interface/sysUncertOptions.h"

#include "tthAnalysis/HiggsToTauTau/interface/analysisAuxFunctions.h" // Era::k*

#include <TString.h> // Form()

#include <boost/algorithm/string/predicate.hpp> // boost::algorithm::starts_with(), boost::algorithm::ends_with()

namespace {
  std::string
  era_str(Era era)
  {
    switch(era)
    {
      case Era::kUndefined: throw cmsException(__func__, __LINE__) << "Undefined era!";
      case Era::k2016 : return "2016";
      case Era::k2017 : return "2017";
      case Era::k2018 : return "2018";
      default: throw cmsException(__func__, __LINE__) << "Invalid era: " << static_cast<int>(era);
    }
  }
}

bool
isTTbarSys(const std::string & central_or_shift)
{
  return
    central_or_shift == "QCDbased"               ||
    central_or_shift == "GluonMove"              ||
    central_or_shift == "erdON"                  ||
    boost::starts_with(central_or_shift, "mtop") ||
    boost::starts_with(central_or_shift, "widthx")
  ;
}

bool
isValidJESsource(Era era,
                 int central_or_shift,
                 bool isFatJet)
{
  if((((central_or_shift == kJetMET_jesHEMUp || central_or_shift == kJetMET_jesHEMDown) && ! isFatJet) ||
      ((central_or_shift == kFatJet_jesHEMUp || central_or_shift == kFatJet_jesHEMDown) &&   isFatJet)  ) &&
     era != Era::k2018)
  {
    return false;
  }
  return true;
}

bool
isValidFatJetAttribute(int central_or_shift,
                       const std::string & attribute_name)
{
  std::vector<std::string> attribute_whitelist = { "mass", "msoftdrop" };
  if(central_or_shift >= kFatJet_central_nonNominal && central_or_shift < kFatJet_jmsUp)
  {
    attribute_whitelist.push_back("pt");
  }
  if(central_or_shift >= kFatJet_jerUp || central_or_shift == kFatJet_central)
  {
    attribute_whitelist.push_back("msoftdrop_tau21DDT");
  }
  return std::find(attribute_whitelist.cbegin(), attribute_whitelist.cend(), attribute_name) != attribute_whitelist.cend();
}

int
getBTagWeight_option(const std::string & central_or_shift)
{
  int central_or_shift_int = kBtag_central;
  if     (central_or_shift == "CMS_ttHl_btag_HFUp"                ) central_or_shift_int = kBtag_hfUp;
  else if(central_or_shift == "CMS_ttHl_btag_HFDown"              ) central_or_shift_int = kBtag_hfDown;
  else if(central_or_shift == "CMS_ttHl_btag_HFStats1Up"          ) central_or_shift_int = kBtag_hfStats1Up;
  else if(central_or_shift == "CMS_ttHl_btag_HFStats1Down"        ) central_or_shift_int = kBtag_hfStats1Down;
  else if(central_or_shift == "CMS_ttHl_btag_HFStats2Up"          ) central_or_shift_int = kBtag_hfStats2Up;
  else if(central_or_shift == "CMS_ttHl_btag_HFStats2Down"        ) central_or_shift_int = kBtag_hfStats2Down;
  else if(central_or_shift == "CMS_ttHl_btag_LFUp"                ) central_or_shift_int = kBtag_lfUp;
  else if(central_or_shift == "CMS_ttHl_btag_LFDown"              ) central_or_shift_int = kBtag_lfDown;
  else if(central_or_shift == "CMS_ttHl_btag_LFStats1Up"          ) central_or_shift_int = kBtag_lfStats1Up;
  else if(central_or_shift == "CMS_ttHl_btag_LFStats1Down"        ) central_or_shift_int = kBtag_lfStats1Down;
  else if(central_or_shift == "CMS_ttHl_btag_LFStats2Up"          ) central_or_shift_int = kBtag_lfStats2Up;
  else if(central_or_shift == "CMS_ttHl_btag_LFStats2Down"        ) central_or_shift_int = kBtag_lfStats2Down;
  else if(central_or_shift == "CMS_ttHl_btag_cErr1Up"             ) central_or_shift_int = kBtag_cErr1Up;
  else if(central_or_shift == "CMS_ttHl_btag_cErr1Down"           ) central_or_shift_int = kBtag_cErr1Down;
  else if(central_or_shift == "CMS_ttHl_btag_cErr2Up"             ) central_or_shift_int = kBtag_cErr2Up;
  else if(central_or_shift == "CMS_ttHl_btag_cErr2Down"           ) central_or_shift_int = kBtag_cErr2Down;
  else if(central_or_shift == "CMS_ttHl_JESUp"                    ) central_or_shift_int = kBtag_jesTotalUp;
  else if(central_or_shift == "CMS_ttHl_JESDown"                  ) central_or_shift_int = kBtag_jesTotalDown;
  else if(central_or_shift == "CMS_ttHl_JESAbsoluteUp"            ) central_or_shift_int = kBtag_jesAbsoluteUp;
  else if(central_or_shift == "CMS_ttHl_JESAbsoluteDown"          ) central_or_shift_int = kBtag_jesAbsoluteDown;
  else if(central_or_shift == "CMS_ttHl_JESAbsolute_EraUp"        ) central_or_shift_int = kBtag_jesAbsolute_EraUp;
  else if(central_or_shift == "CMS_ttHl_JESAbsolute_EraDown"      ) central_or_shift_int = kBtag_jesAbsolute_EraDown;
  else if(central_or_shift == "CMS_ttHl_JESBBEC1Up"               ) central_or_shift_int = kBtag_jesBBEC1Up;
  else if(central_or_shift == "CMS_ttHl_JESBBEC1Down"             ) central_or_shift_int = kBtag_jesBBEC1Down;
  else if(central_or_shift == "CMS_ttHl_JESBBEC1_EraUp"           ) central_or_shift_int = kBtag_jesBBEC1_EraUp;
  else if(central_or_shift == "CMS_ttHl_JESBBEC1_EraDown"         ) central_or_shift_int = kBtag_jesBBEC1_EraDown;
  else if(central_or_shift == "CMS_ttHl_JESEC2Up"                 ) central_or_shift_int = kBtag_jesEC2Up;
  else if(central_or_shift == "CMS_ttHl_JESEC2Down"               ) central_or_shift_int = kBtag_jesEC2Down;
  else if(central_or_shift == "CMS_ttHl_JESEC2_EraUp"             ) central_or_shift_int = kBtag_jesEC2_EraUp;
  else if(central_or_shift == "CMS_ttHl_JESEC2_EraDown"           ) central_or_shift_int = kBtag_jesEC2_EraDown;
  else if(central_or_shift == "CMS_ttHl_JESFlavorQCDUp"           ) central_or_shift_int = kBtag_jesFlavorQCDUp;
  else if(central_or_shift == "CMS_ttHl_JESFlavorQCDDown"         ) central_or_shift_int = kBtag_jesFlavorQCDDown;
  else if(central_or_shift == "CMS_ttHl_JESHFUp"                  ) central_or_shift_int = kBtag_jesHFUp;
  else if(central_or_shift == "CMS_ttHl_JESHFDown"                ) central_or_shift_int = kBtag_jesHFDown;
  else if(central_or_shift == "CMS_ttHl_JESHF_EraUp"              ) central_or_shift_int = kBtag_jesHF_EraUp;
  else if(central_or_shift == "CMS_ttHl_JESHF_EraDown"            ) central_or_shift_int = kBtag_jesHF_EraDown;
  else if(central_or_shift == "CMS_ttHl_JESRelativeBalUp"         ) central_or_shift_int = kBtag_jesRelativeBalUp;
  else if(central_or_shift == "CMS_ttHl_JESRelativeBalDown"       ) central_or_shift_int = kBtag_jesRelativeBalDown;
  else if(central_or_shift == "CMS_ttHl_JESRelativeSample_EraUp"  ) central_or_shift_int = kBtag_jesRelativeSample_EraUp;
  else if(central_or_shift == "CMS_ttHl_JESRelativeSample_EraDown") central_or_shift_int = kBtag_jesRelativeSample_EraDown;
  return central_or_shift_int;
}

pileupJetIDSFsys
getPileupJetIDSFsys_option(const std::string & central_or_shift)
{
  pileupJetIDSFsys central_or_shift_int = pileupJetIDSFsys::central;
  if     (central_or_shift == "CMS_ttHl_puJetIDEffUp"     ) central_or_shift_int = pileupJetIDSFsys::effUp;
  else if(central_or_shift == "CMS_ttHl_puJetIDEffDown"   ) central_or_shift_int = pileupJetIDSFsys::effDown;
  else if(central_or_shift == "CMS_ttHl_puJetIDMistagUp"  ) central_or_shift_int = pileupJetIDSFsys::mistagUp;
  else if(central_or_shift == "CMS_ttHl_puJetIDMistagDown") central_or_shift_int = pileupJetIDSFsys::mistagDown;
  return central_or_shift_int;
}

int
getJet_option(const std::string & central_or_shift,
              bool isMC)
{
  int central_or_shift_int = isMC ? kJetMET_central : kJetMET_central_nonNominal;
  if     (central_or_shift == "CMS_ttHl_JESUp"                    ) central_or_shift_int = kJetMET_jesUp;
  else if(central_or_shift == "CMS_ttHl_JESDown"                  ) central_or_shift_int = kJetMET_jesDown;
  else if(central_or_shift == "CMS_ttHl_JERUp"                    ) central_or_shift_int = kJetMET_jerUp;
  else if(central_or_shift == "CMS_ttHl_JERDown"                  ) central_or_shift_int = kJetMET_jerDown;
  else if(central_or_shift == "CMS_ttHl_JESAbsoluteUp"            ) central_or_shift_int = kJetMET_jesAbsoluteUp;
  else if(central_or_shift == "CMS_ttHl_JESAbsoluteDown"          ) central_or_shift_int = kJetMET_jesAbsoluteDown;
  else if(central_or_shift == "CMS_ttHl_JESAbsolute_EraUp"        ) central_or_shift_int = kJetMET_jesAbsolute_EraUp;
  else if(central_or_shift == "CMS_ttHl_JESAbsolute_EraDown"      ) central_or_shift_int = kJetMET_jesAbsolute_EraDown;
  else if(central_or_shift == "CMS_ttHl_JESBBEC1Up"               ) central_or_shift_int = kJetMET_jesBBEC1Up;
  else if(central_or_shift == "CMS_ttHl_JESBBEC1Down"             ) central_or_shift_int = kJetMET_jesBBEC1Down;
  else if(central_or_shift == "CMS_ttHl_JESBBEC1_EraUp"           ) central_or_shift_int = kJetMET_jesBBEC1_EraUp;
  else if(central_or_shift == "CMS_ttHl_JESBBEC1_EraDown"         ) central_or_shift_int = kJetMET_jesBBEC1_EraDown;
  else if(central_or_shift == "CMS_ttHl_JESEC2Up"                 ) central_or_shift_int = kJetMET_jesEC2Up;
  else if(central_or_shift == "CMS_ttHl_JESEC2Down"               ) central_or_shift_int = kJetMET_jesEC2Down;
  else if(central_or_shift == "CMS_ttHl_JESEC2_EraUp"             ) central_or_shift_int = kJetMET_jesEC2_EraUp;
  else if(central_or_shift == "CMS_ttHl_JESEC2_EraDown"           ) central_or_shift_int = kJetMET_jesEC2_EraDown;
  else if(central_or_shift == "CMS_ttHl_JESFlavorQCDUp"           ) central_or_shift_int = kJetMET_jesFlavorQCDUp;
  else if(central_or_shift == "CMS_ttHl_JESFlavorQCDDown"         ) central_or_shift_int = kJetMET_jesFlavorQCDDown;
  else if(central_or_shift == "CMS_ttHl_JESHFUp"                  ) central_or_shift_int = kJetMET_jesHFUp;
  else if(central_or_shift == "CMS_ttHl_JESHFDown"                ) central_or_shift_int = kJetMET_jesHFDown;
  else if(central_or_shift == "CMS_ttHl_JESHF_EraUp"              ) central_or_shift_int = kJetMET_jesHF_EraUp;
  else if(central_or_shift == "CMS_ttHl_JESHF_EraDown"            ) central_or_shift_int = kJetMET_jesHF_EraDown;
  else if(central_or_shift == "CMS_ttHl_JESRelativeBalUp"         ) central_or_shift_int = kJetMET_jesRelativeBalUp;
  else if(central_or_shift == "CMS_ttHl_JESRelativeBalDown"       ) central_or_shift_int = kJetMET_jesRelativeBalDown;
  else if(central_or_shift == "CMS_ttHl_JESRelativeSample_EraUp"  ) central_or_shift_int = kJetMET_jesRelativeSample_EraUp;
  else if(central_or_shift == "CMS_ttHl_JESRelativeSample_EraDown") central_or_shift_int = kJetMET_jesRelativeSample_EraDown;
  else if(central_or_shift == "CMS_ttHl_JERBarrelUp"              ) central_or_shift_int = kJetMET_jerBarrelUp;
  else if(central_or_shift == "CMS_ttHl_JERBarrelDown"            ) central_or_shift_int = kJetMET_jerBarrelDown;
  else if(central_or_shift == "CMS_ttHl_JEREndcap1Up"             ) central_or_shift_int = kJetMET_jerEndcap1Up;
  else if(central_or_shift == "CMS_ttHl_JEREndcap1Down"           ) central_or_shift_int = kJetMET_jerEndcap1Down;
  else if(central_or_shift == "CMS_ttHl_JEREndcap2LowPtUp"        ) central_or_shift_int = kJetMET_jerEndcap2LowPtUp;
  else if(central_or_shift == "CMS_ttHl_JEREndcap2LowPtDown"      ) central_or_shift_int = kJetMET_jerEndcap2LowPtDown;
  else if(central_or_shift == "CMS_ttHl_JEREndcap2HighPtUp"       ) central_or_shift_int = kJetMET_jerEndcap2HighPtUp;
  else if(central_or_shift == "CMS_ttHl_JEREndcap2HighPtDown"     ) central_or_shift_int = kJetMET_jerEndcap2HighPtDown;
  else if(central_or_shift == "CMS_ttHl_JERForwardLowPtUp"        ) central_or_shift_int = kJetMET_jerForwardLowPtUp;
  else if(central_or_shift == "CMS_ttHl_JERForwardLowPtDown"      ) central_or_shift_int = kJetMET_jerForwardLowPtDown;
  else if(central_or_shift == "CMS_ttHl_JERForwardHighPtUp"       ) central_or_shift_int = kJetMET_jerForwardHighPtUp;
  else if(central_or_shift == "CMS_ttHl_JERForwardHighPtDown"     ) central_or_shift_int = kJetMET_jerForwardHighPtDown;
  else if(central_or_shift == "CMS_ttHl_JESHEMUp"                 ) central_or_shift_int = kJetMET_jesHEMUp;
  else if(central_or_shift == "CMS_ttHl_JESHEMDown"               ) central_or_shift_int = kJetMET_jesHEMDown;
  return central_or_shift_int;
}

int
getMET_option(const std::string & central_or_shift,
              bool isMC)
{
  int central_or_shift_int = isMC ? getJet_option(central_or_shift, isMC) : kJetMET_central_nonNominal;
  if     (central_or_shift == "CMS_ttHl_UnclusteredEnUp"  ) central_or_shift_int = kJetMET_UnclusteredEnUp;
  else if(central_or_shift == "CMS_ttHl_UnclusteredEnDown") central_or_shift_int = kJetMET_UnclusteredEnDown;
  return central_or_shift_int;
}

METSyst
getMETsyst_option(const std::string & central_or_shift)
{
  METSyst central_or_shift_int = METSyst::central;
  if     (central_or_shift == "MET_RespUp")    central_or_shift_int = METSyst::respUp;
  else if(central_or_shift == "MET_RespDown")  central_or_shift_int = METSyst::respDown;
  else if(central_or_shift == "MET_ResolUp")   central_or_shift_int = METSyst::resolUp;
  else if(central_or_shift == "MET_ResolDown") central_or_shift_int = METSyst::resolDown;
  return central_or_shift_int;
}

int
getFatJet_option(const std::string & central_or_shift,
                 bool isMC)
{
  int central_or_shift_int = isMC ? kFatJet_central : kFatJet_central_nonNominal;
  if     (central_or_shift == "CMS_ttHl_JESUp"                    ) central_or_shift_int = kFatJet_jesUp;
  else if(central_or_shift == "CMS_ttHl_JESDown"                  ) central_or_shift_int = kFatJet_jesDown;
  else if(central_or_shift == "CMS_ttHl_JESAbsoluteUp"            ) central_or_shift_int = kFatJet_jesAbsoluteUp;
  else if(central_or_shift == "CMS_ttHl_JESAbsoluteDown"          ) central_or_shift_int = kFatJet_jesAbsoluteDown;
  else if(central_or_shift == "CMS_ttHl_JESAbsolute_EraUp"        ) central_or_shift_int = kFatJet_jesAbsolute_EraUp;
  else if(central_or_shift == "CMS_ttHl_JESAbsolute_EraDown"      ) central_or_shift_int = kFatJet_jesAbsolute_EraDown;
  else if(central_or_shift == "CMS_ttHl_JESBBEC1Up"               ) central_or_shift_int = kFatJet_jesBBEC1Up;
  else if(central_or_shift == "CMS_ttHl_JESBBEC1Down"             ) central_or_shift_int = kFatJet_jesBBEC1Down;
  else if(central_or_shift == "CMS_ttHl_JESBBEC1_EraUp"           ) central_or_shift_int = kFatJet_jesBBEC1_EraUp;
  else if(central_or_shift == "CMS_ttHl_JESBBEC1_EraDown"         ) central_or_shift_int = kFatJet_jesBBEC1_EraDown;
  else if(central_or_shift == "CMS_ttHl_JESEC2Up"                 ) central_or_shift_int = kFatJet_jesEC2Up;
  else if(central_or_shift == "CMS_ttHl_JESEC2Down"               ) central_or_shift_int = kFatJet_jesEC2Down;
  else if(central_or_shift == "CMS_ttHl_JESEC2_EraUp"             ) central_or_shift_int = kFatJet_jesEC2_EraUp;
  else if(central_or_shift == "CMS_ttHl_JESEC2_EraDown"           ) central_or_shift_int = kFatJet_jesEC2_EraDown;
  else if(central_or_shift == "CMS_ttHl_JESFlavorQCDUp"           ) central_or_shift_int = kFatJet_jesFlavorQCDUp;
  else if(central_or_shift == "CMS_ttHl_JESFlavorQCDDown"         ) central_or_shift_int = kFatJet_jesFlavorQCDDown;
  else if(central_or_shift == "CMS_ttHl_JESHFUp"                  ) central_or_shift_int = kFatJet_jesHFUp;
  else if(central_or_shift == "CMS_ttHl_JESHFDown"                ) central_or_shift_int = kFatJet_jesHFDown;
  else if(central_or_shift == "CMS_ttHl_JESHF_EraUp"              ) central_or_shift_int = kFatJet_jesHF_EraUp;
  else if(central_or_shift == "CMS_ttHl_JESHF_EraDown"            ) central_or_shift_int = kFatJet_jesHF_EraDown;
  else if(central_or_shift == "CMS_ttHl_JESRelativeBalUp"         ) central_or_shift_int = kFatJet_jesRelativeBalUp;
  else if(central_or_shift == "CMS_ttHl_JESRelativeBalDown"       ) central_or_shift_int = kFatJet_jesRelativeBalDown;
  else if(central_or_shift == "CMS_ttHl_JESRelativeSample_EraUp"  ) central_or_shift_int = kFatJet_jesRelativeSample_EraUp;
  else if(central_or_shift == "CMS_ttHl_JESRelativeSample_EraDown") central_or_shift_int = kFatJet_jesRelativeSample_EraDown;
  else if(central_or_shift == "CMS_ttHl_JESHEMUp"                 ) central_or_shift_int = kFatJet_jesHEMUp;
  else if(central_or_shift == "CMS_ttHl_JESHEMDown"               ) central_or_shift_int = kFatJet_jesHEMDown;
  else if(central_or_shift == "CMS_ttHl_AK8JERUp"                 ) central_or_shift_int = kFatJet_jerUp;
  else if(central_or_shift == "CMS_ttHl_AK8JERDown"               ) central_or_shift_int = kFatJet_jerDown;
//  else if(central_or_shift == "CMS_ttHl_AK8JERBarrelUp"           ) central_or_shift_int = kFatJet_jerBarrelUp;
//  else if(central_or_shift == "CMS_ttHl_AK8JERBarrelDown"         ) central_or_shift_int = kFatJet_jerBarrelDown;
//  else if(central_or_shift == "CMS_ttHl_AK8JEREndcap1Up"          ) central_or_shift_int = kFatJet_jerEndcap1Up;
//  else if(central_or_shift == "CMS_ttHl_AK8JEREndcap1Down"        ) central_or_shift_int = kFatJet_jerEndcap1Down;
//  else if(central_or_shift == "CMS_ttHl_AK8JEREndcap2LowPtUp"     ) central_or_shift_int = kFatJet_jerEndcap2LowPtUp;
//  else if(central_or_shift == "CMS_ttHl_AK8JEREndcap2LowPtDown"   ) central_or_shift_int = kFatJet_jerEndcap2LowPtDown;
//  else if(central_or_shift == "CMS_ttHl_AK8JEREndcap2HighPtUp"    ) central_or_shift_int = kFatJet_jerEndcap2HighPtUp;
//  else if(central_or_shift == "CMS_ttHl_AK8JEREndcap2HighPtDown"  ) central_or_shift_int = kFatJet_jerEndcap2HighPtDown;
//  else if(central_or_shift == "CMS_ttHl_AK8JERForwardLowPtUp"     ) central_or_shift_int = kFatJet_jerForwardLowPtUp;
//  else if(central_or_shift == "CMS_ttHl_AK8JERForwardLowPtDown"   ) central_or_shift_int = kFatJet_jerForwardLowPtDown;
//  else if(central_or_shift == "CMS_ttHl_AK8JERForwardHighPtUp"    ) central_or_shift_int = kFatJet_jerForwardHighPtUp;
//  else if(central_or_shift == "CMS_ttHl_AK8JERForwardHighPtDown"  ) central_or_shift_int = kFatJet_jerForwardHighPtDown;
  else if(central_or_shift == "CMS_ttHl_AK8JMSUp"                 ) central_or_shift_int = kFatJet_jmsUp;
  else if(central_or_shift == "CMS_ttHl_AK8JMSDown"               ) central_or_shift_int = kFatJet_jmsDown;
  else if(central_or_shift == "CMS_ttHl_AK8JMRUp"                 ) central_or_shift_int = kFatJet_jmrUp;
  else if(central_or_shift == "CMS_ttHl_AK8JMRDown"               ) central_or_shift_int = kFatJet_jmrDown;
  return central_or_shift_int;
}

int
getHadTauPt_option(const std::string & central_or_shift)
{
  int central_or_shift_int = kHadTauPt_central;
  if     (central_or_shift == "CMS_ttHl_tauESUp"  ) central_or_shift_int = kHadTauPt_shiftUp;
  else if(central_or_shift == "CMS_ttHl_tauESDown") central_or_shift_int = kHadTauPt_shiftDown;
  return central_or_shift_int;
}

int
getJetToTauFR_option(const std::string & central_or_shift)
{
  int central_or_shift_int = kFRjt_central;
  if     (central_or_shift == "CMS_ttHl_FRjt_normUp"   ) central_or_shift_int = kFRjt_normUp;
  else if(central_or_shift == "CMS_ttHl_FRjt_normDown" ) central_or_shift_int = kFRjt_normDown;
  else if(central_or_shift == "CMS_ttHl_FRjt_shapeUp"  ) central_or_shift_int = kFRjt_shapeUp;
  else if(central_or_shift == "CMS_ttHl_FRjt_shapeDown") central_or_shift_int = kFRjt_shapeDown;
  return central_or_shift_int;
}

FRet
getEToTauFR_option(const std::string & central_or_shift)
{
  FRet central_or_shift_int = FRet::central;
  if     (central_or_shift == "CMS_ttHl_FRet_shiftUp"  ) central_or_shift_int = FRet::shiftUp;
  else if(central_or_shift == "CMS_ttHl_FRet_shiftDown") central_or_shift_int = FRet::shiftDown;
  return central_or_shift_int;
}

FRmt
getMuToTauFR_option(const std::string & central_or_shift)
{
  FRmt central_or_shift_int = FRmt::central;
  if     (central_or_shift == "CMS_ttHl_FRmt_shiftUp"  ) central_or_shift_int = FRmt::shiftUp;
  else if(central_or_shift == "CMS_ttHl_FRmt_shiftDown") central_or_shift_int = FRmt::shiftDown;
  return central_or_shift_int;
}

LeptonIDSFsys
getLeptonIDSFsys_option(const std::string & central_or_shift)
{
  LeptonIDSFsys central_or_shift_int = LeptonIDSFsys::central;
  if     (central_or_shift == "CMS_ttHl_lepEff_eltightUp"        ) central_or_shift_int = LeptonIDSFsys::elTightUp;
  else if(central_or_shift == "CMS_ttHl_lepEff_eltightDown"      ) central_or_shift_int = LeptonIDSFsys::elTightDown;
  else if(central_or_shift == "CMS_ttHl_lepEff_mutightUp"        ) central_or_shift_int = LeptonIDSFsys::muTightUp;
  else if(central_or_shift == "CMS_ttHl_lepEff_mutightDown"      ) central_or_shift_int = LeptonIDSFsys::muTightDown;
  else if(central_or_shift == "CMS_ttHl_lepEff_ellooseUp"        ) central_or_shift_int = LeptonIDSFsys::elLooseUp;
  else if(central_or_shift == "CMS_ttHl_lepEff_ellooseDown"      ) central_or_shift_int = LeptonIDSFsys::elLooseDown;
  else if(central_or_shift == "CMS_ttHl_lepEff_mulooseUp"        ) central_or_shift_int = LeptonIDSFsys::muLooseUp;
  else if(central_or_shift == "CMS_ttHl_lepEff_mulooseDown"      ) central_or_shift_int = LeptonIDSFsys::muLooseDown;
  else if(central_or_shift == "CMS_ttHl_lepEff_eltightRecompUp"  ) central_or_shift_int = LeptonIDSFsys::elTightRecompUp;
  else if(central_or_shift == "CMS_ttHl_lepEff_eltightRecompDown") central_or_shift_int = LeptonIDSFsys::elTightRecompDown;
  else if(central_or_shift == "CMS_ttHl_lepEff_mutightRecompUp"  ) central_or_shift_int = LeptonIDSFsys::muTightRecompUp;
  else if(central_or_shift == "CMS_ttHl_lepEff_mutightRecompDown") central_or_shift_int = LeptonIDSFsys::muTightRecompDown;
  return central_or_shift_int;
}

TauIDSFsys
getTauIDSFsys_option(const std::string & central_or_shift)
{
  TauIDSFsys central_or_shift_int = TauIDSFsys::central;
  if     (central_or_shift == "CMS_ttHl_tauIDSFUp"  ) central_or_shift_int = TauIDSFsys::shiftUp;
  else if(central_or_shift == "CMS_ttHl_tauIDSFDown") central_or_shift_int = TauIDSFsys::shiftDown;
  return central_or_shift_int;
}

TriggerSFsys
getTriggerSF_option(const std::string & central_or_shift,
                    TriggerSFsysChoice choice)
{
  const bool isLeptonCompatible = choice == TriggerSFsysChoice::any || choice == TriggerSFsysChoice::leptonOnly;
  const bool isHadTauCompatible = choice == TriggerSFsysChoice::any || choice == TriggerSFsysChoice::hadTauOnly;
  const bool isAnyCompatible = isLeptonCompatible || isHadTauCompatible;
  TriggerSFsys central_or_shift_int = TriggerSFsys::central;
  if     (central_or_shift == "CMS_ttHl_triggerUp"            && isAnyCompatible   ) central_or_shift_int = TriggerSFsys::shiftUp;
  else if(central_or_shift == "CMS_ttHl_triggerDown"          && isAnyCompatible   ) central_or_shift_int = TriggerSFsys::shiftDown;
  else if(central_or_shift == "CMS_ttHl_trigger_2lssUp"       && isLeptonCompatible) central_or_shift_int = TriggerSFsys::shift_2lssUp;
  else if(central_or_shift == "CMS_ttHl_trigger_2lssDown"     && isLeptonCompatible) central_or_shift_int = TriggerSFsys::shift_2lssDown;
  else if(central_or_shift == "CMS_ttHl_trigger_2lssEEUp"     && isLeptonCompatible) central_or_shift_int = TriggerSFsys::shift_2lssEEUp;
  else if(central_or_shift == "CMS_ttHl_trigger_2lssEEDown"   && isLeptonCompatible) central_or_shift_int = TriggerSFsys::shift_2lssEEDown;
  else if(central_or_shift == "CMS_ttHl_trigger_2lssEMuUp"    && isLeptonCompatible) central_or_shift_int = TriggerSFsys::shift_2lssEMuUp;
  else if(central_or_shift == "CMS_ttHl_trigger_2lssEMuDown"  && isLeptonCompatible) central_or_shift_int = TriggerSFsys::shift_2lssEMuDown;
  else if(central_or_shift == "CMS_ttHl_trigger_2lssMuMuUp"   && isLeptonCompatible) central_or_shift_int = TriggerSFsys::shift_2lssMuMuUp;
  else if(central_or_shift == "CMS_ttHl_trigger_2lssMuMuDown" && isLeptonCompatible) central_or_shift_int = TriggerSFsys::shift_2lssMuMuDown;
  else if(central_or_shift == "CMS_ttHl_trigger_3lUp"         && isLeptonCompatible) central_or_shift_int = TriggerSFsys::shift_3lUp;
  else if(central_or_shift == "CMS_ttHl_trigger_3lDown"       && isLeptonCompatible) central_or_shift_int = TriggerSFsys::shift_3lDown;
  else if(central_or_shift == "CMS_ttHl_trigger_1l2tauUp"     && isHadTauCompatible) central_or_shift_int = TriggerSFsys::shift_1l2tauUp;
  else if(central_or_shift == "CMS_ttHl_trigger_1l2tauDown"   && isHadTauCompatible) central_or_shift_int = TriggerSFsys::shift_1l2tauDown;
  else if(central_or_shift == "CMS_ttHl_trigger_1l1tauUp"     && isHadTauCompatible) central_or_shift_int = TriggerSFsys::shift_1l1tauUp;
  else if(central_or_shift == "CMS_ttHl_trigger_1l1tauDown"   && isHadTauCompatible) central_or_shift_int = TriggerSFsys::shift_1l1tauDown;
  else if(central_or_shift == "CMS_ttHl_trigger_0l2tauUp"     && isHadTauCompatible) central_or_shift_int = TriggerSFsys::shift_0l2tauUp;
  else if(central_or_shift == "CMS_ttHl_trigger_0l2tauDown"   && isHadTauCompatible) central_or_shift_int = TriggerSFsys::shift_0l2tauDown;
  else if(central_or_shift == "CMS_ttHl_trigger_1lEUp"        && isLeptonCompatible) central_or_shift_int = TriggerSFsys::shift_1lEUp;
  else if(central_or_shift == "CMS_ttHl_trigger_1lEDown"      && isLeptonCompatible) central_or_shift_int = TriggerSFsys::shift_1lEDown;
  else if(central_or_shift == "CMS_ttHl_trigger_1lMuUp"       && isLeptonCompatible) central_or_shift_int = TriggerSFsys::shift_1lMuUp;
  else if(central_or_shift == "CMS_ttHl_trigger_1lMuDown"     && isLeptonCompatible) central_or_shift_int = TriggerSFsys::shift_1lMuDown;
  return central_or_shift_int;
}

int
getLHEscale_option(const std::string & central_or_shift)
{
  int central_or_shift_int = kLHE_scale_central;
  if(boost::starts_with(central_or_shift, "CMS_ttHl_thu_shape"))
  {
    if     (boost::ends_with(central_or_shift, "x1y1Up")  ) central_or_shift_int = kLHE_scale_xyUp;
    else if(boost::ends_with(central_or_shift, "x1y1Down")) central_or_shift_int = kLHE_scale_xyDown;
    else if(boost::ends_with(central_or_shift, "x1Up")    ) central_or_shift_int = kLHE_scale_xUp;
    else if(boost::ends_with(central_or_shift, "x1Down")  ) central_or_shift_int = kLHE_scale_xDown;
    else if(boost::ends_with(central_or_shift, "y1Up")    ) central_or_shift_int = kLHE_scale_yUp;
    else if(boost::ends_with(central_or_shift, "y1Down")  ) central_or_shift_int = kLHE_scale_yDown;
    else if(boost::ends_with(central_or_shift, "Up")      ) central_or_shift_int = kLHE_scale_Up;
    else if(boost::ends_with(central_or_shift, "Down")    ) central_or_shift_int = kLHE_scale_Down;
    else throw cmsException(__func__, __LINE__)
           << "Invalid option to LHE systematics: " << central_or_shift;
  }
  return central_or_shift_int;
}

int
getPartonShower_option(const std::string & central_or_shift)
{
  int central_or_shift_int = kPartonShower_central;
  if(boost::starts_with(central_or_shift, "CMS_ttHl_PS_"))
  {
    if     (boost::ends_with(central_or_shift, "ISRDown")) central_or_shift_int = kPartonShower_ISRDown;
    else if(boost::ends_with(central_or_shift, "ISRUp")  ) central_or_shift_int = kPartonShower_ISRUp;
    else if(boost::ends_with(central_or_shift, "FSRDown")) central_or_shift_int = kPartonShower_FSRDown;
    else if(boost::ends_with(central_or_shift, "FSRUp")  ) central_or_shift_int = kPartonShower_FSRUp;
    else if(boost::ends_with(central_or_shift, "Down")   ) central_or_shift_int = kPartonShower_Down;
    else if(boost::ends_with(central_or_shift, "Up")     ) central_or_shift_int = kPartonShower_Up;
    else throw cmsException(__func__, __LINE__)
           << "Invalid option to LHE systematics: " << central_or_shift;
  }
  return central_or_shift_int;
}

ElectronPtSys
getElectronPt_option(const std::string & central_or_shift,
                     bool isMC)
{
  ElectronPtSys central_or_shift_int = ElectronPtSys::central;
  if     (central_or_shift == "CMS_ttHl_electronESBarrelUp"  ) central_or_shift_int = ElectronPtSys::scaleUp_barrel;
  else if(central_or_shift == "CMS_ttHl_electronESBarrelDown") central_or_shift_int = ElectronPtSys::scaleDown_barrel;
  else if(central_or_shift == "CMS_ttHl_electronESEndcapUp"  ) central_or_shift_int = ElectronPtSys::scaleUp_endcap;
  else if(central_or_shift == "CMS_ttHl_electronESEndcapDown") central_or_shift_int = ElectronPtSys::scaleDown_endcap;
  else if(central_or_shift == "CMS_ttHl_electronERUp"        ) central_or_shift_int = ElectronPtSys::resUp;
  else if(central_or_shift == "CMS_ttHl_electronERDown"      ) central_or_shift_int = ElectronPtSys::resDown;
  assert((central_or_shift_int == ElectronPtSys::central && ! isMC) || isMC);
  return central_or_shift_int;
}

MuonPtSys
getMuon_option(const std::string & central_or_shift,
               bool isMC)
{
  MuonPtSys central_or_shift_int = MuonPtSys::central;
  if     (central_or_shift == "CMS_ttHl_muonERUp"         ) central_or_shift_int = MuonPtSys::ERUp;
  else if(central_or_shift == "CMS_ttHl_muonERDown"       ) central_or_shift_int = MuonPtSys::ERDown;
  else if(central_or_shift == "CMS_ttHl_muonESBarrel1Up"  ) central_or_shift_int = MuonPtSys::ESBarrel1Up;
  else if(central_or_shift == "CMS_ttHl_muonESBarrel1Down") central_or_shift_int = MuonPtSys::ESBarrel1Down;
  else if(central_or_shift == "CMS_ttHl_muonESBarrel2Up"  ) central_or_shift_int = MuonPtSys::ESBarrel2Up;
  else if(central_or_shift == "CMS_ttHl_muonESBarrel2Down") central_or_shift_int = MuonPtSys::ESBarrel2Down;
  else if(central_or_shift == "CMS_ttHl_muonESEndcap1Up"  ) central_or_shift_int = MuonPtSys::ESEndcap1Up;
  else if(central_or_shift == "CMS_ttHl_muonESEndcap1Down") central_or_shift_int = MuonPtSys::ESEndcap1Down;
  else if(central_or_shift == "CMS_ttHl_muonESEndcap2Up"  ) central_or_shift_int = MuonPtSys::ESEndcap2Up;
  else if(central_or_shift == "CMS_ttHl_muonESEndcap2Down") central_or_shift_int = MuonPtSys::ESEndcap2Down;
  assert((central_or_shift_int == MuonPtSys::central && ! isMC) || isMC);
  return central_or_shift_int;
}

int
getJetToLeptonFR_option(const std::string & central_or_shift)
{
  int central_or_shift_int = kFRl_central;
  if     (central_or_shift == "CMS_ttHl_FRe_shape_ptUp"          ) central_or_shift_int = kFRe_shape_ptUp;
  else if(central_or_shift == "CMS_ttHl_FRe_shape_ptDown"        ) central_or_shift_int = kFRe_shape_ptDown;
  else if(central_or_shift == "CMS_ttHl_FRe_shape_normUp"        ) central_or_shift_int = kFRe_shape_normUp;
  else if(central_or_shift == "CMS_ttHl_FRe_shape_normDown"      ) central_or_shift_int = kFRe_shape_normDown;
  else if(central_or_shift == "CMS_ttHl_FRe_shape_eta_barrelUp"  ) central_or_shift_int = kFRe_shape_eta_barrelUp;
  else if(central_or_shift == "CMS_ttHl_FRe_shape_eta_barrelDown") central_or_shift_int = kFRe_shape_eta_barrelDown;
  else if(central_or_shift == "CMS_ttHl_FRe_shape_corrUp"        ) central_or_shift_int = kFRe_shape_corrUp;
  else if(central_or_shift == "CMS_ttHl_FRe_shape_corrDown"      ) central_or_shift_int = kFRe_shape_corrDown;
  else if(central_or_shift == "CMS_ttHl_FRm_shape_ptUp"          ) central_or_shift_int = kFRm_shape_ptUp;
  else if(central_or_shift == "CMS_ttHl_FRm_shape_ptDown"        ) central_or_shift_int = kFRm_shape_ptDown;
  else if(central_or_shift == "CMS_ttHl_FRm_shape_normUp"        ) central_or_shift_int = kFRm_shape_normUp;
  else if(central_or_shift == "CMS_ttHl_FRm_shape_normDown"      ) central_or_shift_int = kFRm_shape_normDown;
  else if(central_or_shift == "CMS_ttHl_FRm_shape_eta_barrelUp"  ) central_or_shift_int = kFRm_shape_eta_barrelUp;
  else if(central_or_shift == "CMS_ttHl_FRm_shape_eta_barrelDown") central_or_shift_int = kFRm_shape_eta_barrelDown;
  else if(central_or_shift == "CMS_ttHl_FRm_shape_corrUp"        ) central_or_shift_int = kFRm_shape_corrUp;
  else if(central_or_shift == "CMS_ttHl_FRm_shape_corrDown"      ) central_or_shift_int = kFRm_shape_corrDown;
  return central_or_shift_int;
}

PUsys
getPUsys_option(const std::string & central_or_shift)
{
  PUsys central_or_shift_int = PUsys::central;
  if     (central_or_shift == "CMS_ttHl_pileupUp"  ) central_or_shift_int = PUsys::up;
  else if(central_or_shift == "CMS_ttHl_pileupDown") central_or_shift_int = PUsys::down;
  return central_or_shift_int;
}


L1PreFiringWeightSys
getL1PreFiringWeightSys_option(const std::string & central_or_shift)
{
  L1PreFiringWeightSys central_or_shift_int = L1PreFiringWeightSys::nominal;
  if     (central_or_shift == "CMS_ttHl_l1PreFireUp")   central_or_shift_int = L1PreFiringWeightSys::up;
  else if(central_or_shift == "CMS_ttHl_l1PreFireDown") central_or_shift_int = L1PreFiringWeightSys::down;
  return central_or_shift_int;
}

int
getDYMCReweighting_option(const std::string & central_or_shift)
{
  int central_or_shift_int = kDYMCReweighting_central;
  if     (central_or_shift == "CMS_ttHl_DYMCReweightingUp"  ) central_or_shift_int = kDYMCReweighting_shiftUp;
  else if(central_or_shift == "CMS_ttHl_DYMCReweightingDown") central_or_shift_int = kDYMCReweighting_shiftDown;
  return central_or_shift_int;
}

int
getDYMCNormScaleFactors_option(const std::string & central_or_shift)
{
  int central_or_shift_int = kDYMCNormScaleFactors_central;
  if     (central_or_shift == "CMS_ttHl_DYMCNormScaleFactorsUp"  ) central_or_shift_int = kDYMCNormScaleFactors_shiftUp;
  else if(central_or_shift == "CMS_ttHl_DYMCNormScaleFactorsDown") central_or_shift_int = kDYMCNormScaleFactors_shiftDown;
  return central_or_shift_int;
}

int
getTopPtReweighting_option(const std::string & central_or_shift)
{
  int central_or_shift_int = kTopPtReweighting_central;
  if     (central_or_shift == "CMS_ttHl_topPtReweightingUp"  ) central_or_shift_int = kTopPtReweighting_shiftUp;
  else if(central_or_shift == "CMS_ttHl_topPtReweightingDown") central_or_shift_int = kTopPtReweighting_shiftDown;
  return central_or_shift_int;
}

MEMsys
getMEMsys_option(const std::string & central_or_shift)
{
  MEMsys central_or_shift_int = MEMsys::nominal;
  if(boost::starts_with(central_or_shift, "CMS_ttHl_MEM"))
  {
    if     (boost::ends_with(central_or_shift, "Up")  ) central_or_shift_int = MEMsys::up;
    else if(boost::ends_with(central_or_shift, "Down")) central_or_shift_int = MEMsys::down;
  }
  return central_or_shift_int;
}

EWKJetSys
getEWKJetSys_option(const std::string & central_or_shift)
{
  EWKJetSys central_or_shift_int = EWKJetSys::central;
  if     (central_or_shift == "CMS_ttHl_EWK_jetUp"  ) central_or_shift_int = EWKJetSys::up;
  else if(central_or_shift == "CMS_ttHl_EWK_jetDown") central_or_shift_int = EWKJetSys::down;
  return central_or_shift_int;
}

EWKBJetSys
getEWKBJetSys_option(const std::string & central_or_shift)
{
  EWKBJetSys central_or_shift_int = EWKBJetSys::central;
  if     (central_or_shift == "CMS_ttHl_EWK_btagUp"  ) central_or_shift_int = EWKBJetSys::up;
  else if(central_or_shift == "CMS_ttHl_EWK_btagDown") central_or_shift_int = EWKBJetSys::down;
  return central_or_shift_int;
}

PDFSys
getPDFSys_option(const std::string & central_or_shift)
{
  PDFSys central_or_shift_int = PDFSys::central;
  if(boost::starts_with(central_or_shift, "CMS_ttHl_PDF_shape") &&
     ! isPDFsys_member(central_or_shift))
  {
    if     (boost::ends_with(central_or_shift, "Up")  ) central_or_shift_int = PDFSys::up;
    else if(boost::ends_with(central_or_shift, "Down")) central_or_shift_int = PDFSys::down;
    else throw cmsException(__func__, __LINE__)
           << "Invalid option to PDF systematics: " << central_or_shift;
  }
  return central_or_shift_int;
}

bool
isPDFsys_member(const std::string & central_or_shift)
{
  return
    boost::starts_with(central_or_shift, "CMS_ttHl_PDF_shape") &&
    central_or_shift.find("Member") != std::string::npos
  ;
}

LHEVptSys
getLHEVptSys_option(const std::string & central_or_shift)
{
  LHEVptSys central_or_shift_int = LHEVptSys::central;
  if     (central_or_shift == "Vpt_nloUp"  ) central_or_shift_int = LHEVptSys::up;
  else if(central_or_shift == "Vpt_nloDown") central_or_shift_int = LHEVptSys::down;
  return central_or_shift_int;
}

SubjetBtagSys
getSubjetBtagSys_option(const std::string & central_or_shift)
{
  SubjetBtagSys central_or_shift_int = SubjetBtagSys::central;
  if     (central_or_shift == "CMS_btag_subjetUp"  ) central_or_shift_int = SubjetBtagSys::up;
  else if(central_or_shift == "CMS_btag_subjetDown") central_or_shift_int = SubjetBtagSys::down;
  return central_or_shift_int;
}

void
checkOptionValidity(const std::string & central_or_shift,
                    bool isMC)
{
  if(! (central_or_shift.empty() || central_or_shift == "central"))
  {
    if(! isMC && getJetToLeptonFR_option(central_or_shift) != kFRl_central && getJetToTauFR_option(central_or_shift) != kFRjt_central)
    {
      throw cmsException(__func__, __LINE__) << "Non-empty or non-central systematics option passed to data";
    }
    if(isMC && ! (boost::ends_with(central_or_shift, "Up") || boost::ends_with(central_or_shift, "Down")) &&
       ! isTTbarSys(central_or_shift) &&
       ! isPDFsys_member(central_or_shift))
    {
      throw cmsException(__func__, __LINE__) << "Non-central MC systematics option not ending with Up or Down: " << central_or_shift;
    }
  }
}

std::string
getBranchName_bTagWeight(Btag btag,
                         Era era,
                         const std::string & default_collectionName,
                         int central_or_shift)
{
  std::map<int, std::string> branchNames_bTagWeight;
  std::string discrName;
  switch(btag)
  {
    case Btag::kDeepJet: discrName = "deepjet"; break;
    case Btag::kDeepCSV: discrName = "deepcsv"; break;
    case Btag::kCSVv2:   discrName = "csvv2";   break;
  }
  const std::string era_str = ::era_str(era);
  branchNames_bTagWeight[kBtag_central] = Form("%s_btagSF_%s_shape", default_collectionName.data(), discrName.data());
  branchNames_bTagWeight[kBtag_hfUp]                      = branchNames_bTagWeight[kBtag_central] + "_up_hf";
  branchNames_bTagWeight[kBtag_hfDown]                    = branchNames_bTagWeight[kBtag_central] + "_down_hf";
  branchNames_bTagWeight[kBtag_hfStats1Up]                = branchNames_bTagWeight[kBtag_central] + "_up_hfstats1";
  branchNames_bTagWeight[kBtag_hfStats1Down]              = branchNames_bTagWeight[kBtag_central] + "_down_hfstats1";
  branchNames_bTagWeight[kBtag_hfStats2Up]                = branchNames_bTagWeight[kBtag_central] + "_up_hfstats2";
  branchNames_bTagWeight[kBtag_hfStats2Down]              = branchNames_bTagWeight[kBtag_central] + "_down_hfstats2";
  branchNames_bTagWeight[kBtag_lfUp]                      = branchNames_bTagWeight[kBtag_central] + "_up_lf";
  branchNames_bTagWeight[kBtag_lfDown]                    = branchNames_bTagWeight[kBtag_central] + "_down_lf";
  branchNames_bTagWeight[kBtag_lfStats1Up]                = branchNames_bTagWeight[kBtag_central] + "_up_lfstats1";
  branchNames_bTagWeight[kBtag_lfStats1Down]              = branchNames_bTagWeight[kBtag_central] + "_down_lfstats1";
  branchNames_bTagWeight[kBtag_lfStats2Up]                = branchNames_bTagWeight[kBtag_central] + "_up_lfstats2";
  branchNames_bTagWeight[kBtag_lfStats2Down]              = branchNames_bTagWeight[kBtag_central] + "_down_lfstats2";
  branchNames_bTagWeight[kBtag_cErr1Up]                   = branchNames_bTagWeight[kBtag_central] + "_up_cferr1";
  branchNames_bTagWeight[kBtag_cErr1Down]                 = branchNames_bTagWeight[kBtag_central] + "_down_cferr1";
  branchNames_bTagWeight[kBtag_cErr2Up]                   = branchNames_bTagWeight[kBtag_central] + "_up_cferr2";
  branchNames_bTagWeight[kBtag_cErr2Down]                 = branchNames_bTagWeight[kBtag_central] + "_down_cferr2";
  branchNames_bTagWeight[kBtag_jesTotalUp]                = branchNames_bTagWeight[kBtag_central] + "_up_jes";
  branchNames_bTagWeight[kBtag_jesTotalDown]              = branchNames_bTagWeight[kBtag_central] + "_down_jes";
  branchNames_bTagWeight[kBtag_jesAbsoluteUp]             = branchNames_bTagWeight[kBtag_central] + "_up_jesAbsolute";
  branchNames_bTagWeight[kBtag_jesAbsoluteDown]           = branchNames_bTagWeight[kBtag_central] + "_down_jesAbsolute";
  branchNames_bTagWeight[kBtag_jesAbsolute_EraUp]         = branchNames_bTagWeight[kBtag_central] + Form("_up_jesAbsolute_%s", era_str.data());
  branchNames_bTagWeight[kBtag_jesAbsolute_EraDown]       = branchNames_bTagWeight[kBtag_central] + Form("_down_jesAbsolute_%s", era_str.data());
  branchNames_bTagWeight[kBtag_jesBBEC1Up]                = branchNames_bTagWeight[kBtag_central] + "_up_jesBBEC1";
  branchNames_bTagWeight[kBtag_jesBBEC1Down]              = branchNames_bTagWeight[kBtag_central] + "_down_jesBBEC1";
  branchNames_bTagWeight[kBtag_jesBBEC1_EraUp]            = branchNames_bTagWeight[kBtag_central] + Form("_up_jesBBEC1_%s", era_str.data());
  branchNames_bTagWeight[kBtag_jesBBEC1_EraDown]          = branchNames_bTagWeight[kBtag_central] + Form("_down_jesBBEC1_%s", era_str.data());
  branchNames_bTagWeight[kBtag_jesEC2Up]                  = branchNames_bTagWeight[kBtag_central] + "_up_jesEC2";
  branchNames_bTagWeight[kBtag_jesEC2Down]                = branchNames_bTagWeight[kBtag_central] + "_down_jesEC2";
  branchNames_bTagWeight[kBtag_jesEC2_EraUp]              = branchNames_bTagWeight[kBtag_central] + Form("_up_jesEC2_%s", era_str.data());
  branchNames_bTagWeight[kBtag_jesEC2_EraDown]            = branchNames_bTagWeight[kBtag_central] + Form("_down_jesEC2_%s", era_str.data());
  branchNames_bTagWeight[kBtag_jesFlavorQCDUp]            = branchNames_bTagWeight[kBtag_central] + "_up_jesFlavorQCD";
  branchNames_bTagWeight[kBtag_jesFlavorQCDDown]          = branchNames_bTagWeight[kBtag_central] + "_down_jesFlavorQCD";
  branchNames_bTagWeight[kBtag_jesHFUp]                   = branchNames_bTagWeight[kBtag_central] + "_up_jesHF";
  branchNames_bTagWeight[kBtag_jesHFDown]                 = branchNames_bTagWeight[kBtag_central] + "_down_jesHF";
  branchNames_bTagWeight[kBtag_jesHF_EraUp]               = branchNames_bTagWeight[kBtag_central] + Form("_up_jesHF_%s", era_str.data());
  branchNames_bTagWeight[kBtag_jesHF_EraDown]             = branchNames_bTagWeight[kBtag_central] + Form("_down_jesHF_%s", era_str.data());
  branchNames_bTagWeight[kBtag_jesRelativeBalUp]          = branchNames_bTagWeight[kBtag_central] + "_up_jesRelativeBal";
  branchNames_bTagWeight[kBtag_jesRelativeBalDown]        = branchNames_bTagWeight[kBtag_central] + "_down_jesRelativeBal";
  branchNames_bTagWeight[kBtag_jesRelativeSample_EraUp]   = branchNames_bTagWeight[kBtag_central] + Form("_up_jesRelativeSample_%s", era_str.data());
  branchNames_bTagWeight[kBtag_jesRelativeSample_EraDown] = branchNames_bTagWeight[kBtag_central] + Form("_down_jesRelativeSample_%s", era_str.data());
  assert(branchNames_bTagWeight.count(central_or_shift));
  return branchNames_bTagWeight.at(central_or_shift);
}

std::string
getBranchName_jetMET(const std::string & default_branchName,
                     Era era,
                     int central_or_shift,
                     bool isPt)
{
  static std::map<int, std::string> branchNames_sys;
  const bool isJet = boost::starts_with(default_branchName, "Jet");
  const bool isMET = default_branchName == "MET";
  const std::string era_str = ::era_str(era);
  if(! isJet && ! isMET)
  {
    throw cmsException(__func__, __LINE__) << "Invalid branch name provided: " << default_branchName;
  }
  branchNames_sys[kJetMET_central_nonNominal] = Form(
    "%s_%s", default_branchName.data(), isPt ? "pt" : (isJet ? "mass" : "phi")
  );
  const std::string branchName_corrected = Form(
    "%s%s_%s", default_branchName.data(), isMET ? "_T1Smear" : "", isPt ? "pt" : (isJet ? "mass" : "phi")
  );
  branchNames_sys[kJetMET_central]                   = branchName_corrected + (isMET ? "" : "_nom");
  branchNames_sys[kJetMET_jesUp]                     = branchName_corrected + "_jesTotalUp";
  branchNames_sys[kJetMET_jesDown]                   = branchName_corrected + "_jesTotalDown";
  //
  branchNames_sys[kJetMET_jesAbsoluteUp]             = branchName_corrected + "_jesAbsoluteUp";
  branchNames_sys[kJetMET_jesAbsoluteDown]           = branchName_corrected + "_jesAbsoluteDown";
  branchNames_sys[kJetMET_jesAbsolute_EraUp]         = branchName_corrected + Form("_jesAbsolute_%sUp",    era_str.data());
  branchNames_sys[kJetMET_jesAbsolute_EraDown]       = branchName_corrected + Form("_jesAbsolute_%sDown", era_str.data());
  branchNames_sys[kJetMET_jesBBEC1Up]                = branchName_corrected + "_jesBBEC1Up";
  branchNames_sys[kJetMET_jesBBEC1Down]              = branchName_corrected + "_jesBBEC1Down";
  branchNames_sys[kJetMET_jesBBEC1_EraUp]            = branchName_corrected + Form("_jesBBEC1_%sUp",   era_str.data());
  branchNames_sys[kJetMET_jesBBEC1_EraDown]          = branchName_corrected + Form("_jesBBEC1_%sDown", era_str.data());
  branchNames_sys[kJetMET_jesEC2Up]                  = branchName_corrected + "_jesEC2Up";
  branchNames_sys[kJetMET_jesEC2Down]                = branchName_corrected + "_jesEC2Down";
  branchNames_sys[kJetMET_jesEC2_EraUp]              = branchName_corrected + Form("_jesEC2_%sUp",   era_str.data());
  branchNames_sys[kJetMET_jesEC2_EraDown]            = branchName_corrected + Form("_jesEC2_%sDown", era_str.data());
  branchNames_sys[kJetMET_jesFlavorQCDUp]            = branchName_corrected + "_jesFlavorQCDUp";
  branchNames_sys[kJetMET_jesFlavorQCDDown]          = branchName_corrected + "_jesFlavorQCDDown";
  branchNames_sys[kJetMET_jesHFUp]                   = branchName_corrected + "_jesHFUp";
  branchNames_sys[kJetMET_jesHFDown]                 = branchName_corrected + "_jesHFDown";
  branchNames_sys[kJetMET_jesHF_EraUp]               = branchName_corrected + Form("_jesHF_%sUp",   era_str.data());
  branchNames_sys[kJetMET_jesHF_EraDown]             = branchName_corrected + Form("_jesHF_%sDown", era_str.data());
  branchNames_sys[kJetMET_jesRelativeBalUp]          = branchName_corrected + "_jesRelativeBalUp";
  branchNames_sys[kJetMET_jesRelativeBalDown]        = branchName_corrected + "_jesRelativeBalDown";
  branchNames_sys[kJetMET_jesRelativeSample_EraUp]   = branchName_corrected + Form("_jesRelativeSample_%sUp",   era_str.data());
  branchNames_sys[kJetMET_jesRelativeSample_EraDown] = branchName_corrected + Form("_jesRelativeSample_%sDown", era_str.data());
  branchNames_sys[kJetMET_jesHEMUp]                  = branchName_corrected + "_jesHEMIssueUp";
  branchNames_sys[kJetMET_jesHEMDown]                = branchName_corrected + "_jesHEMIssueDown";
  //
  branchNames_sys[kJetMET_jerUp]                     = branchName_corrected + "_jerUp";
  branchNames_sys[kJetMET_jerDown]                   = branchName_corrected + "_jerDown";
  branchNames_sys[kJetMET_jerBarrelUp]               = branchName_corrected + "_jer0Up";
  branchNames_sys[kJetMET_jerBarrelDown]             = branchName_corrected + "_jer0Down";
  branchNames_sys[kJetMET_jerEndcap1Up]              = branchName_corrected + "_jer1Up";
  branchNames_sys[kJetMET_jerEndcap1Down]            = branchName_corrected + "_jer1Down";
  branchNames_sys[kJetMET_jerEndcap2LowPtUp]         = branchName_corrected + "_jer2Up";
  branchNames_sys[kJetMET_jerEndcap2LowPtDown]       = branchName_corrected + "_jer2Down";
  branchNames_sys[kJetMET_jerEndcap2HighPtUp]        = branchName_corrected + "_jer3Up";
  branchNames_sys[kJetMET_jerEndcap2HighPtDown]      = branchName_corrected + "_jer3Down";
  branchNames_sys[kJetMET_jerForwardLowPtUp]         = branchName_corrected + "_jer4Up";
  branchNames_sys[kJetMET_jerForwardLowPtDown]       = branchName_corrected + "_jer4Down";
  branchNames_sys[kJetMET_jerForwardHighPtUp]        = branchName_corrected + "_jer5Up";
  branchNames_sys[kJetMET_jerForwardHighPtDown]      = branchName_corrected + "_jer5Down";
  //
  branchNames_sys[kJetMET_UnclusteredEnUp]           = branchName_corrected + "_unclustEnUp";
  branchNames_sys[kJetMET_UnclusteredEnDown]         = branchName_corrected + "_unclustEnDown";
  //
  assert(branchNames_sys.count(central_or_shift));
  assert(isValidJESsource(era, central_or_shift));
  assert((isJet && central_or_shift != kJetMET_UnclusteredEnUp && central_or_shift != kJetMET_UnclusteredEnDown) || ! isJet);
  return branchNames_sys.at(central_or_shift);
}

std::string
getBranchName_fatJet(const std::string & default_branchName,
                     Era era,
                     const std::string & attribute_name,
                     int central_or_shift)
{
  assert(boost::starts_with(default_branchName, "FatJet"));
  const std::string era_str = ::era_str(era);
  static std::map<int, std::string> branchNames_sys;
  branchNames_sys[kFatJet_central_nonNominal] = Form(
    "%s_%s", default_branchName.data(), attribute_name.data()
  );
  branchNames_sys[kFatJet_central]                   = branchNames_sys[kFatJet_central_nonNominal] + "_nom";
  branchNames_sys[kFatJet_jesUp]                     = branchNames_sys[kFatJet_central_nonNominal] + "_jesTotalUp";
  branchNames_sys[kFatJet_jesDown]                   = branchNames_sys[kFatJet_central_nonNominal] + "_jesTotalDown";
  branchNames_sys[kFatJet_jesAbsoluteUp]             = branchNames_sys[kFatJet_central_nonNominal] + "_jesAbsoluteUp";
  branchNames_sys[kFatJet_jesAbsoluteDown]           = branchNames_sys[kFatJet_central_nonNominal] + "_jesAbsoluteDown";
  branchNames_sys[kFatJet_jesAbsolute_EraUp]         = branchNames_sys[kFatJet_central_nonNominal] + Form("_jesAbsolute_%sUp",    era_str.data());
  branchNames_sys[kFatJet_jesAbsolute_EraDown]       = branchNames_sys[kFatJet_central_nonNominal] + Form("_jesAbsolute_%sDown", era_str.data());
  branchNames_sys[kFatJet_jesBBEC1Up]                = branchNames_sys[kFatJet_central_nonNominal] + "_jesBBEC1Up";
  branchNames_sys[kFatJet_jesBBEC1Down]              = branchNames_sys[kFatJet_central_nonNominal] + "_jesBBEC1Down";
  branchNames_sys[kFatJet_jesBBEC1_EraUp]            = branchNames_sys[kFatJet_central_nonNominal] + Form("_jesBBEC1_%sUp",   era_str.data());
  branchNames_sys[kFatJet_jesBBEC1_EraDown]          = branchNames_sys[kFatJet_central_nonNominal] + Form("_jesBBEC1_%sDown", era_str.data());
  branchNames_sys[kFatJet_jesEC2Up]                  = branchNames_sys[kFatJet_central_nonNominal] + "_jesEC2Up";
  branchNames_sys[kFatJet_jesEC2Down]                = branchNames_sys[kFatJet_central_nonNominal] + "_jesEC2Down";
  branchNames_sys[kFatJet_jesEC2_EraUp]              = branchNames_sys[kFatJet_central_nonNominal] + Form("_jesEC2_%sUp",   era_str.data());
  branchNames_sys[kFatJet_jesEC2_EraDown]            = branchNames_sys[kFatJet_central_nonNominal] + Form("_jesEC2_%sDown", era_str.data());
  branchNames_sys[kFatJet_jesFlavorQCDUp]            = branchNames_sys[kFatJet_central_nonNominal] + "_jesFlavorQCDUp";
  branchNames_sys[kFatJet_jesFlavorQCDDown]          = branchNames_sys[kFatJet_central_nonNominal] + "_jesFlavorQCDDown";
  branchNames_sys[kFatJet_jesHFUp]                   = branchNames_sys[kFatJet_central_nonNominal] + "_jesHFUp";
  branchNames_sys[kFatJet_jesHFDown]                 = branchNames_sys[kFatJet_central_nonNominal] + "_jesHFDown";
  branchNames_sys[kFatJet_jesHF_EraUp]               = branchNames_sys[kFatJet_central_nonNominal] + Form("_jesHF_%sUp",   era_str.data());
  branchNames_sys[kFatJet_jesHF_EraDown]             = branchNames_sys[kFatJet_central_nonNominal] + Form("_jesHF_%sDown", era_str.data());
  branchNames_sys[kFatJet_jesRelativeBalUp]          = branchNames_sys[kFatJet_central_nonNominal] + "_jesRelativeBalUp";
  branchNames_sys[kFatJet_jesRelativeBalDown]        = branchNames_sys[kFatJet_central_nonNominal] + "_jesRelativeBalDown";
  branchNames_sys[kFatJet_jesRelativeSample_EraUp]   = branchNames_sys[kFatJet_central_nonNominal] + Form("_jesRelativeSample_%sUp",   era_str.data());
  branchNames_sys[kFatJet_jesRelativeSample_EraDown] = branchNames_sys[kFatJet_central_nonNominal] + Form("_jesRelativeSample_%sDown", era_str.data());
  branchNames_sys[kFatJet_jesHEMUp]                  = branchNames_sys[kFatJet_central_nonNominal] + "_jesHEMIssueUp";
  branchNames_sys[kFatJet_jesHEMDown]                = branchNames_sys[kFatJet_central_nonNominal] + "_jesHEMIssueDown";
  //
  branchNames_sys[kFatJet_jerUp]                     = branchNames_sys[kFatJet_central_nonNominal] + "_jerUp";
  branchNames_sys[kFatJet_jerDown]                   = branchNames_sys[kFatJet_central_nonNominal] + "_jerDown";
//  branchNames_sys[kFatJet_jerBarrelUp]               = branchNames_sys[kFatJet_central_nonNominal] + "_jer0Up";
//  branchNames_sys[kFatJet_jerBarrelDown]             = branchNames_sys[kFatJet_central_nonNominal] + "_jer0Down";
//  branchNames_sys[kFatJet_jerEndcap1Up]              = branchNames_sys[kFatJet_central_nonNominal] + "_jer1Up";
//  branchNames_sys[kFatJet_jerEndcap1Down]            = branchNames_sys[kFatJet_central_nonNominal] + "_jer1Down";
//  branchNames_sys[kFatJet_jerEndcap2LowPtUp]         = branchNames_sys[kFatJet_central_nonNominal] + "_jer2Up";
//  branchNames_sys[kFatJet_jerEndcap2LowPtDown]       = branchNames_sys[kFatJet_central_nonNominal] + "_jer2Down";
//  branchNames_sys[kFatJet_jerEndcap2HighPtUp]        = branchNames_sys[kFatJet_central_nonNominal] + "_jer3Up";
//  branchNames_sys[kFatJet_jerEndcap2HighPtDown]      = branchNames_sys[kFatJet_central_nonNominal] + "_jer3Down";
//  branchNames_sys[kFatJet_jerForwardLowPtUp]         = branchNames_sys[kFatJet_central_nonNominal] + "_jer4Up";
//  branchNames_sys[kFatJet_jerForwardLowPtDown]       = branchNames_sys[kFatJet_central_nonNominal] + "_jer4Down";
//  branchNames_sys[kFatJet_jerForwardHighPtUp]        = branchNames_sys[kFatJet_central_nonNominal] + "_jer5Up";
//  branchNames_sys[kFatJet_jerForwardHighPtDown]      = branchNames_sys[kFatJet_central_nonNominal] + "_jer5Down";
  //
  branchNames_sys[kFatJet_jmsUp]                     = branchNames_sys[kFatJet_central_nonNominal] + "_jmsUp";
  branchNames_sys[kFatJet_jmsDown]                   = branchNames_sys[kFatJet_central_nonNominal] + "_jmsDown";
  branchNames_sys[kFatJet_jmrUp]                     = branchNames_sys[kFatJet_central_nonNominal] + "_jmrUp";
  branchNames_sys[kFatJet_jmrDown]                   = branchNames_sys[kFatJet_central_nonNominal] + "_jmrDown";
  assert(branchNames_sys.count(central_or_shift));
  assert(isValidJESsource(era, central_or_shift, false));
  assert(isValidFatJetAttribute(central_or_shift, attribute_name));
  return branchNames_sys.at(central_or_shift);
}

std::string
getBranchName_pileup(PUsys puSys_option)
{
  switch(puSys_option)
  {
    case PUsys::central: return "puWeight";
    case PUsys::up:      return "puWeightUp";
    case PUsys::down:    return "puWeightDown";
    default: throw cmsException(__func__, __LINE__) << "Invalid option = " << static_cast<int>(puSys_option);
  }
}
