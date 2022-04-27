from tthAnalysis.NanoAOD.triggers import Triggers

trigger_runs_2016 = Triggers('2016').runs
trigger_runs_2017 = Triggers('2017').runs
trigger_runs_2018 = Triggers('2018').runs

# Integrated luminosity -- pre-legacy lumi mask, the latest normtag

# Reproduced https://github.com/HEP-KBFI/tth-nanoAOD/blob/dde7a1e5640f7a66ad635cb6f618dd1119bc1244/test/datasets/txt/datasets_data_2016_17Jul18.txt#L50
# Official figures: https://hypernews.cern.ch/HyperNews/CMS/get/physics-announcements/6191.html & PAS LUM-17-003
lumi_2016 = 36.326e+3 # 1/pb (uncertainty: 1.2%)

# Reproduced https://github.com/HEP-KBFI/tth-nanoAOD/blob/dde7a1e5640f7a66ad635cb6f618dd1119bc1244/test/datasets/txt/datasets_data_2017_31Mar18.txt#L38
# Official figures: https://hypernews.cern.ch/HyperNews/CMS/get/luminosity/761/1.html & PAS LUM-17-004
lumi_2017 = 41.529e+3 # 1/pb (uncertainty: 2.3%)

# Reproduced https://github.com/HEP-KBFI/tth-nanoAOD/blob/dde7a1e5640f7a66ad635cb6f618dd1119bc1244/test/datasets/txt/datasets_data_2018_17Sep18.txt#L24
# Official figures: https://hypernews.cern.ch/HyperNews/CMS/get/luminosity/860.html & PAS LUM-18-002
lumi_2018 = 59.741e+3 # 1/pb (uncertainty: 2.5%)

HTXS_BINS = [ "fwd", "pt0to60", "pt60to120", "pt120to200", "pt200to300", "ptGt300", "pt300to450", "ptGt450" ]

def get_lumi(era):
  if era == "2016":
    return lumi_2016
  elif era == "2017":
    return lumi_2017
  elif era == "2018":
    return lumi_2018
  else:
    raise ValueError("Invalid era: %s" % era)

# https://twiki.cern.ch/twiki/bin/view/LHCPhysics/CERNYellowReportPageAt13TeV
WH_xsec = 1.373 # pp -> WH
ZH_xsec = 0.8839 # pp -> ZH
VH_xsec = WH_xsec + ZH_xsec
VH_keys = {
  'WH' : WH_xsec / VH_xsec,
  'ZH' : ZH_xsec / VH_xsec,
}

# Systematic uncertainties

class systematics(object):

  # Basic definitions

  central = [ "central" ]

  mcClosure_str = 'mcClosure'
  mcClosure = [ mcClosure_str ]

  JES                  = [ "CMS_ttHl_JESUp",                  "CMS_ttHl_JESDown"                  ]
  JER                  = [ "CMS_ttHl_JERUp",                  "CMS_ttHl_JERDown"                  ]
  UnclusteredEn        = [ "CMS_ttHl_UnclusteredEnUp",        "CMS_ttHl_UnclusteredEnDown"        ]
  tauES                = [ "CMS_ttHl_tauESUp",                "CMS_ttHl_tauESDown"                ]
  tauIDSF              = [ "CMS_ttHl_tauIDSFUp",              "CMS_ttHl_tauIDSFDown"              ]
  triggerSF            = [ "CMS_ttHl_triggerUp",              "CMS_ttHl_triggerDown"              ]
  DYMCReweighting      = [ "CMS_ttHl_DYMCReweightingUp",      "CMS_ttHl_DYMCReweightingDown"      ]
  DYMCNormScaleFactors = [ "CMS_ttHl_DYMCNormScaleFactorsUp", "CMS_ttHl_DYMCNormScaleFactorsDown" ]
  EWK_jet              = [ "CMS_ttHl_EWK_jetUp",              "CMS_ttHl_EWK_jetDown"              ]
  EWK_bjet             = [ "CMS_ttHl_EWK_btagUp",             "CMS_ttHl_EWK_btagDown"             ]
  MET_ResponseSyst     = [ "MET_RespUp",                      "MET_RespDown"                      ]
  MET_ResolutionSyst   = [ "MET_ResolUp",                     "MET_ResolDown"                     ]

  triggerSF_2lss   = [
#    "CMS_ttHl_trigger_2lssUp",     "CMS_ttHl_trigger_2lssDown",
    "CMS_ttHl_trigger_2lssEEUp",   "CMS_ttHl_trigger_2lssEEDown",
    "CMS_ttHl_trigger_2lssEMuUp",  "CMS_ttHl_trigger_2lssEMuDown",
    "CMS_ttHl_trigger_2lssMuMuUp", "CMS_ttHl_trigger_2lssMuMuDown",
  ]
  triggerSF_1l = [
    "CMS_ttHl_trigger_1lEUp",  "CMS_ttHl_trigger_1lEDown",
    "CMS_ttHl_trigger_1lMuUp", "CMS_ttHl_trigger_1lMuDown",
  ]
  triggerSF_3l     = [ "CMS_ttHl_trigger_3lUp",     "CMS_ttHl_trigger_3lDown"     ]
  triggerSF_1l2tau = [ "CMS_ttHl_trigger_1l2tauUp", "CMS_ttHl_trigger_1l2tauDown" ]
  triggerSF_1l1tau = [ "CMS_ttHl_trigger_1l1tauUp", "CMS_ttHl_trigger_1l1tauDown" ]
  triggerSF_0l2tau = [ "CMS_ttHl_trigger_0l2tauUp", "CMS_ttHl_trigger_0l2tauDown" ]
  triggerSF_split = triggerSF_2lss + triggerSF_1l + triggerSF_3l + triggerSF_1l2tau + triggerSF_1l1tau + triggerSF_0l2tau

  JES_HEM = "CMS_ttHl_JESHEMDown" # addresses HEM15/16, see https://hypernews.cern.ch/HyperNews/CMS/get/JetMET/2000.html

  JEC_regrouped = [
    "CMS_ttHl_JESAbsoluteUp",           "CMS_ttHl_JESAbsoluteDown",
    "CMS_ttHl_JESAbsolute_EraUp",       "CMS_ttHl_JESAbsolute_EraDown",
    "CMS_ttHl_JESBBEC1Up",              "CMS_ttHl_JESBBEC1Down",
    "CMS_ttHl_JESBBEC1_EraUp",          "CMS_ttHl_JESBBEC1_EraDown",
    "CMS_ttHl_JESEC2Up",                "CMS_ttHl_JESEC2Down",
    "CMS_ttHl_JESEC2_EraUp",            "CMS_ttHl_JESEC2_EraDown",
    "CMS_ttHl_JESFlavorQCDUp",          "CMS_ttHl_JESFlavorQCDDown",
    "CMS_ttHl_JESHFUp",                 "CMS_ttHl_JESHFDown",
    "CMS_ttHl_JESHF_EraUp",             "CMS_ttHl_JESHF_EraDown",
    "CMS_ttHl_JESRelativeBalUp",        "CMS_ttHl_JESRelativeBalDown",
    "CMS_ttHl_JESRelativeSample_EraUp", "CMS_ttHl_JESRelativeSample_EraDown",
    JES_HEM,
  ]

  JER_split = [
    "CMS_ttHl_JERBarrelUp",        "CMS_ttHl_JERBarrelDown",
    "CMS_ttHl_JEREndcap1Up",       "CMS_ttHl_JEREndcap1Down",
    "CMS_ttHl_JEREndcap2LowPtUp",  "CMS_ttHl_JEREndcap2LowPtDown",
    "CMS_ttHl_JEREndcap2HighPtUp", "CMS_ttHl_JEREndcap2HighPtDown",
    "CMS_ttHl_JERForwardLowPtUp",  "CMS_ttHl_JERForwardLowPtDown",
    "CMS_ttHl_JERForwardHighPtUp", "CMS_ttHl_JERForwardHighPtDown",
  ]

  #AK8_JES = [ "CMS_ttHl_AK8JESUp", "CMS_ttHl_AK8JESDown" ]
  AK8_JER = [ "CMS_ttHl_AK8JERUp", "CMS_ttHl_AK8JERDown" ]
  AK8_JMS = [ "CMS_ttHl_AK8JMSUp", "CMS_ttHl_AK8JMSDown" ]
  AK8_JMR = [ "CMS_ttHl_AK8JMRUp", "CMS_ttHl_AK8JMRDown" ]

  #AK8_JEC_regrouped = [ central_or_shift.replace('_JES', '_AK8JES') for central_or_shift in JEC_regrouped ]
  #AK8_JES_HEM = JES_HEM.replace('_JES', '_AK8JES')
  #assert(AK8_JES_HEM in AK8_JEC_regrouped)
  # [*]

  # not splitting JER for AK8 jets
  #AK8_JER_split     = [ central_or_shift.replace('_JER', '_AK8JER') for central_or_shift in JER_split     ]

  AK8 = AK8_JER + AK8_JMS + AK8_JMR # [*]

  JEC_regrouped_ALL = JEC_regrouped# + AK8_JEC_regrouped # [*]
  JER_split_ALL     = JER_split     #+ AK8_JER_split

  # [*] varying JES for AK4 and AK8 jets simultaneously

  pileupJetID = [
    "CMS_ttHl_puJetIDEffUp",    "CMS_ttHl_puJetIDEffDown",
    "CMS_ttHl_puJetIDMistagUp", "CMS_ttHl_puJetIDMistagDown",
  ]

  MEM_3l        = [ "CMS_ttHl_MEM_3l_LRUp",        "CMS_ttHl_MEM_3l_LRDown"        ]
  MEM_2lss_1tau = [ "CMS_ttHl_MEM_2lss_1tau_LRUp", "CMS_ttHl_MEM_2lss_1tau_LRDown" ]
  MEM_3l_1tau   = [ "CMS_ttHl_MEM_3l_1tau_LRUp",   "CMS_ttHl_MEM_3l_1tau_LRDown"   ]
  MEM_bb2l      = [ "CMS_ttHl_MEM_bb2l_LRUp",      "CMS_ttHl_MEM_bb2l_LRDown"      ]
  MEM = MEM_3l + MEM_2lss_1tau + MEM_3l_1tau + MEM_bb2l

  LHEVpt = [ "Vpt_nloUp", "Vpt_nloDown" ]

  subjetBtag = [ "CMS_btag_subjetUp", "CMS_btag_subjetDown" ]

  class TTbar(object):
    hdamp = [ "hdampUp", "hdampDown" ]
    ue    = [ "ueUp",    "ueDown"    ]
    cr    = [ "QCDbased", "GluonMove", "erdON" ]
    mtop  = [
      # Keep only those mass points that are closest to the nominal,
      # see: https://hypernews.cern.ch/HyperNews/CMS/get/TOP-17-023/97.html
      #"mtop166p5",
      "mtop169p5",
      "mtop171p5", "mtop173p5",
      "mtop175p5",
      #"mtop178p5",
    ]
    width = [
      "widthx0p7", "widthx0p85", "widthx1p15", "widthx1p3",
    ]
    full = hdamp + cr + mtop + ue #width # removed UE because its effect is very small

  class L1PreFiring_(object):
    up   = "CMS_ttHl_l1PreFireUp"
    down = "CMS_ttHl_l1PreFireDown"
    full = [ up, down ]

  L1PreFiring = L1PreFiring_().full

  class PU_(object):
    up   = "CMS_ttHl_pileupUp"
    down = "CMS_ttHl_pileupDown"
    full = [ up, down ]

  PU = PU_().full

  class topPtReweighting_(object):
    up   = "CMS_ttHl_topPtReweightingUp"
    down = "CMS_ttHl_topPtReweightingDown"
    full = [ up, down ]

  topPtReweighting = topPtReweighting_().full

  class PartonShower(object):

    class TT(object):

      class ISR_(object):
        up   = "CMS_ttHl_PS_TT_ISRUp"
        down = "CMS_ttHl_PS_TT_ISRDown"
        full = [ up, down ]

      class FSR_(object):
        up   = "CMS_ttHl_PS_TT_FSRUp"
        down = "CMS_ttHl_PS_TT_FSRDown"
        full = [ up, down ]

      class env_(object):
        up   = "CMS_ttHl_PS_TTUp"
        down = "CMS_ttHl_PS_TTDown"
        full = [ up, down ]

      full = ISR_().full + FSR_().full + env_().full
    
    class DY(object):

      class ISR_(object):
        up   = "CMS_ttHl_PS_DY_ISRUp"
        down = "CMS_ttHl_PS_DY_ISRDown"
        full = [ up, down ]

      class FSR_(object):
        up   = "CMS_ttHl_PS_DY_FSRUp"
        down = "CMS_ttHl_PS_DY_FSRDown"
        full = [ up, down ]

      class env_(object):
        up   = "CMS_ttHl_PS_DYUp"
        down = "CMS_ttHl_PS_DYDown"
        full = [ up, down ]

      full = ISR_().full + FSR_().full + env_().full
    
    class WJets(object):

      class ISR_(object):
        up   = "CMS_ttHl_PS_W_ISRUp"
        down = "CMS_ttHl_PS_W_ISRDown"
        full = [ up, down ]

      class FSR_(object):
        up   = "CMS_ttHl_PS_W_FSRUp"
        down = "CMS_ttHl_PS_W_FSRDown"
        full = [ up, down ]

      class env_(object):
        up   = "CMS_ttHl_PS_WUp"
        down = "CMS_ttHl_PS_WDown"
        full = [ up, down ]

      full = ISR_().full + FSR_().full + env_().full

    ttbar = TT().full
    dy    = DY().full
    #wjets = WJets().full

    procs = [ TT, DY, ] #WJets ]

    isr_up   = [ proc.ISR_().up   for proc in procs ]
    isr_down = [ proc.ISR_().down for proc in procs ]
    fsr_up   = [ proc.FSR_().up   for proc in procs ]
    fsr_down = [ proc.FSR_().down for proc in procs ]
    env_up   = [ proc.env_().up   for proc in procs ]
    env_down = [ proc.env_().down for proc in procs ]

    #full = ttbar + dy# + wjets
    full = isr_up + isr_down + fsr_up + fsr_down

  class LHE(object):

    class TTH(object):

      class x1_(object):
        up   = "CMS_ttHl_thu_shape_ttH_x1Up"
        down = "CMS_ttHl_thu_shape_ttH_x1Down"
        full = [ up, down ]

      class y1_(object):
        up   = "CMS_ttHl_thu_shape_ttH_y1Up"
        down = "CMS_ttHl_thu_shape_ttH_y1Down"
        full = [ up, down ]

      class x1y1_(object):
        up   = "CMS_ttHl_thu_shape_ttH_x1y1Up"
        down = "CMS_ttHl_thu_shape_ttH_x1y1Down"
        full = [ up, down ]

      class env_(object):
        up   = "CMS_ttHl_thu_shape_ttHUp"
        down = "CMS_ttHl_thu_shape_ttHDown"
        full = [ up, down ]

      full = x1_().full + y1_().full + x1y1_().full + env_().full

    class THQ(object):

      class x1_(object):
        up   = "CMS_ttHl_thu_shape_tHq_x1Up"
        down = "CMS_ttHl_thu_shape_tHq_x1Down"
        full = [ up, down ]

      class y1_(object):
        up   = "CMS_ttHl_thu_shape_tHq_y1Up"
        down = "CMS_ttHl_thu_shape_tHq_y1Down"
        full = [ up, down ]

      class x1y1_(object):
        up   = "CMS_ttHl_thu_shape_tHq_x1y1Up"
        down = "CMS_ttHl_thu_shape_tHq_x1y1Down"
        full = [ up, down ]

      class env_(object):
        up   = "CMS_ttHl_thu_shape_tHqUp"
        down = "CMS_ttHl_thu_shape_tHqDown"
        full = [ up, down ]

      full = x1_().full + y1_().full + x1y1_().full + env_().full

    class THW(object):

      class x1_(object):
        up   = "CMS_ttHl_thu_shape_tHW_x1Up"
        down = "CMS_ttHl_thu_shape_tHW_x1Down"
        full = [ up, down ]

      class y1_(object):
        up   = "CMS_ttHl_thu_shape_tHW_y1Up"
        down = "CMS_ttHl_thu_shape_tHW_y1Down"
        full = [ up, down ]

      class x1y1_(object):
        up   = "CMS_ttHl_thu_shape_tHW_x1y1Up"
        down = "CMS_ttHl_thu_shape_tHW_x1y1Down"
        full = [ up, down ]

      class env_(object):
        up   = "CMS_ttHl_thu_shape_tHWUp"
        down = "CMS_ttHl_thu_shape_tHWDown"
        full = [ up, down ]

      full = x1_().full + y1_().full + x1y1_().full + env_().full

    class TTW(object):

      class x1_(object):
        up   = "CMS_ttHl_thu_shape_ttW_x1Up"
        down = "CMS_ttHl_thu_shape_ttW_x1Down"
        full = [ up, down ]

      class y1_(object):
        up   = "CMS_ttHl_thu_shape_ttW_y1Up"
        down = "CMS_ttHl_thu_shape_ttW_y1Down"
        full = [ up, down ]

      class x1y1_(object):
        up   = "CMS_ttHl_thu_shape_ttW_x1y1Up"
        down = "CMS_ttHl_thu_shape_ttW_x1y1Down"
        full = [ up, down ]

      class env_(object):
        up   = "CMS_ttHl_thu_shape_ttWUp"
        down = "CMS_ttHl_thu_shape_ttWDown"
        full = [ up, down ]

      full = x1_().full + y1_().full + x1y1_().full + env_().full

    class TTZ(object):

      class x1_(object):
        up   = "CMS_ttHl_thu_shape_ttZ_x1Up"
        down = "CMS_ttHl_thu_shape_ttZ_x1Down"
        full = [ up, down ]

      class y1_(object):
        up   = "CMS_ttHl_thu_shape_ttZ_y1Up"
        down = "CMS_ttHl_thu_shape_ttZ_y1Down"
        full = [ up, down ]

      class x1y1_(object):
        up   = "CMS_ttHl_thu_shape_ttZ_x1y1Up"
        down = "CMS_ttHl_thu_shape_ttZ_x1y1Down"
        full = [ up, down ]

      class env_(object):
        up   = "CMS_ttHl_thu_shape_ttZUp"
        down = "CMS_ttHl_thu_shape_ttZDown"
        full = [ up, down ]

      full = x1_().full + y1_().full + x1y1_().full + env_().full

    class HH(object):
      class x1_(object):

        up   = "CMS_ttHl_thu_shape_HH_x1Up"
        down = "CMS_ttHl_thu_shape_HH_x1Down"
        full = [ up, down ]

      class y1_(object):
        up   = "CMS_ttHl_thu_shape_HH_y1Up"
        down = "CMS_ttHl_thu_shape_HH_y1Down"
        full = [ up, down ]

      class x1y1_(object):
        up   = "CMS_ttHl_thu_shape_HH_x1y1Up"
        down = "CMS_ttHl_thu_shape_HH_x1y1Down"
        full = [ up, down ]

      class env_(object):
        up   = "CMS_ttHl_thu_shape_HHUp"
        down = "CMS_ttHl_thu_shape_HHDown"
        full = [ up, down ]

      full = x1_().full + y1_().full + x1y1_().full + env_().full

    class DY(object):
      class x1_(object):

        up   = "CMS_ttHl_thu_shape_DY_x1Up"
        down = "CMS_ttHl_thu_shape_DY_x1Down"
        full = [ up, down ]

      class y1_(object):
        up   = "CMS_ttHl_thu_shape_DY_y1Up"
        down = "CMS_ttHl_thu_shape_DY_y1Down"
        full = [ up, down ]

      class x1y1_(object):
        up   = "CMS_ttHl_thu_shape_DY_x1y1Up"
        down = "CMS_ttHl_thu_shape_DY_x1y1Down"
        full = [ up, down ]

      class env_(object):
        up   = "CMS_ttHl_thu_shape_DYUp"
        down = "CMS_ttHl_thu_shape_DYDown"
        full = [ up, down ]

      full = x1_().full + y1_().full + x1y1_().full + env_().full

    class TT(object):
      class x1_(object):

        up   = "CMS_ttHl_thu_shape_TT_x1Up"
        down = "CMS_ttHl_thu_shape_TT_x1Down"
        full = [ up, down ]

      class y1_(object):
        up   = "CMS_ttHl_thu_shape_TT_y1Up"
        down = "CMS_ttHl_thu_shape_TT_y1Down"
        full = [ up, down ]

      class x1y1_(object):
        up   = "CMS_ttHl_thu_shape_TT_x1y1Up"
        down = "CMS_ttHl_thu_shape_TT_x1y1Down"
        full = [ up, down ]

      class env_(object):
        up   = "CMS_ttHl_thu_shape_TTUp"
        down = "CMS_ttHl_thu_shape_TTDown"
        full = [ up, down ]

      full = x1_().full + y1_().full + x1y1_().full + env_().full

    class WZ(object):
      class x1_(object):

        up   = "CMS_ttHl_thu_shape_WZ_x1Up"
        down = "CMS_ttHl_thu_shape_WZ_x1Down"
        full = [ up, down ]

      class y1_(object):
        up   = "CMS_ttHl_thu_shape_WZ_y1Up"
        down = "CMS_ttHl_thu_shape_WZ_y1Down"
        full = [ up, down ]

      class x1y1_(object):
        up   = "CMS_ttHl_thu_shape_WZ_x1y1Up"
        down = "CMS_ttHl_thu_shape_WZ_x1y1Down"
        full = [ up, down ]

      class env_(object):
        up   = "CMS_ttHl_thu_shape_WZUp"
        down = "CMS_ttHl_thu_shape_WZDown"
        full = [ up, down ]

      full = x1_().full + y1_().full + x1y1_().full + env_().full
    
    class ZZ(object):
      class x1_(object):

        up   = "CMS_ttHl_thu_shape_ZZ_x1Up"
        down = "CMS_ttHl_thu_shape_ZZ_x1Down"
        full = [ up, down ]

      class y1_(object):
        up   = "CMS_ttHl_thu_shape_ZZ_y1Up"
        down = "CMS_ttHl_thu_shape_ZZ_y1Down"
        full = [ up, down ]

      class x1y1_(object):
        up   = "CMS_ttHl_thu_shape_ZZ_x1y1Up"
        down = "CMS_ttHl_thu_shape_ZZ_x1y1Down"
        full = [ up, down ]

      class env_(object):
        up   = "CMS_ttHl_thu_shape_ZZUp"
        down = "CMS_ttHl_thu_shape_ZZDown"
        full = [ up, down ]

      full = x1_().full + y1_().full + x1y1_().full + env_().full

    class ggZZ(object):
      class x1_(object):

        up   = "CMS_ttHl_thu_shape_ggZZ_x1Up"
        down = "CMS_ttHl_thu_shape_ggZZ_x1Down"
        full = [ up, down ]

      class y1_(object):
        up   = "CMS_ttHl_thu_shape_ggZZ_y1Up"
        down = "CMS_ttHl_thu_shape_ggZZ_y1Down"
        full = [ up, down ]

      class x1y1_(object):
        up   = "CMS_ttHl_thu_shape_ggZZ_x1y1Up"
        down = "CMS_ttHl_thu_shape_ggZZ_x1y1Down"
        full = [ up, down ]

      class env_(object):
        up   = "CMS_ttHl_thu_shape_ggZZUp"
        down = "CMS_ttHl_thu_shape_ggZZDown"
        full = [ up, down ]

      full = x1_().full + y1_().full + x1y1_().full + env_().full

    class qqZZ(object):
      class x1_(object):

        up   = "CMS_ttHl_thu_shape_qqZZ_x1Up"
        down = "CMS_ttHl_thu_shape_qqZZ_x1Down"
        full = [ up, down ]

      class y1_(object):
        up   = "CMS_ttHl_thu_shape_qqZZ_y1Up"
        down = "CMS_ttHl_thu_shape_qqZZ_y1Down"
        full = [ up, down ]

      class x1y1_(object):
        up   = "CMS_ttHl_thu_shape_qqZZ_x1y1Up"
        down = "CMS_ttHl_thu_shape_qqZZ_x1y1Down"
        full = [ up, down ]

      class env_(object):
        up   = "CMS_ttHl_thu_shape_qqZZUp"
        down = "CMS_ttHl_thu_shape_qqZZDown"
        full = [ up, down ]

      full = x1_().full + y1_().full + x1y1_().full + env_().full

    class WW(object):
      class x1_(object):

        up   = "CMS_ttHl_thu_shape_WW_x1Up"
        down = "CMS_ttHl_thu_shape_WW_x1Down"
        full = [ up, down ]

      class y1_(object):
        up   = "CMS_ttHl_thu_shape_WW_y1Up"
        down = "CMS_ttHl_thu_shape_WW_y1Down"
        full = [ up, down ]

      class x1y1_(object):
        up   = "CMS_ttHl_thu_shape_WW_x1y1Up"
        down = "CMS_ttHl_thu_shape_WW_x1y1Down"
        full = [ up, down ]

      class env_(object):
        up   = "CMS_ttHl_thu_shape_WWUp"
        down = "CMS_ttHl_thu_shape_WWDown"
        full = [ up, down ]

      full = x1_().full + y1_().full + x1y1_().full + env_().full

    class VV(object):
      class x1_(object):

        up   = "CMS_ttHl_thu_shape_VV_x1Up"
        down = "CMS_ttHl_thu_shape_VV_x1Down"
        full = [ up, down ]

      class y1_(object):
        up   = "CMS_ttHl_thu_shape_VV_y1Up"
        down = "CMS_ttHl_thu_shape_VV_y1Down"
        full = [ up, down ]

      class x1y1_(object):
        up   = "CMS_ttHl_thu_shape_VV_x1y1Up"
        down = "CMS_ttHl_thu_shape_VV_x1y1Down"
        full = [ up, down ]

      class env_(object):
        up   = "CMS_ttHl_thu_shape_VVUp"
        down = "CMS_ttHl_thu_shape_VVDown"
        full = [ up, down ]

      full = x1_().full + y1_().full + x1y1_().full + env_().full

    class WJets(object):
      class x1_(object):

        up   = "CMS_ttHl_thu_shape_WJets_x1Up"
        down = "CMS_ttHl_thu_shape_WJets_x1Down"
        full = [ up, down ]

      class y1_(object):
        up   = "CMS_ttHl_thu_shape_WJets_y1Up"
        down = "CMS_ttHl_thu_shape_WJets_y1Down"
        full = [ up, down ]

      class x1y1_(object):
        up   = "CMS_ttHl_thu_shape_WJets_x1y1Up"
        down = "CMS_ttHl_thu_shape_WJets_x1y1Down"
        full = [ up, down ]

      class env_(object):
        up   = "CMS_ttHl_thu_shape_WJetsUp"
        down = "CMS_ttHl_thu_shape_WJetsDown"
        full = [ up, down ]

      full = x1_().full + y1_().full + x1y1_().full + env_().full

    class Other(object):
      class x1_(object):

        up   = "CMS_ttHl_thu_shape_Other_x1Up"
        down = "CMS_ttHl_thu_shape_Other_x1Down"
        full = [ up, down ]

      class y1_(object):
        up   = "CMS_ttHl_thu_shape_Other_y1Up"
        down = "CMS_ttHl_thu_shape_Other_y1Down"
        full = [ up, down ]

      class x1y1_(object):
        up   = "CMS_ttHl_thu_shape_Other_x1y1Up"
        down = "CMS_ttHl_thu_shape_Other_x1y1Down"
        full = [ up, down ]

      class env_(object):
        up   = "CMS_ttHl_thu_shape_OtherUp"
        down = "CMS_ttHl_thu_shape_OtherDown"
        full = [ up, down ]

      full = x1_().full + y1_().full + x1y1_().full + env_().full

    ttH   = TTH().full
    tHq   = THQ().full
    tHW   = THW().full
    ttW   = TTW().full
    ttZ   = TTZ().full
    hh    = HH().full
    dy    = DY().full
    ttbar = TT().full
    wz    = WZ().full
    zz    = ZZ().full
    gg_zz = ggZZ().full
    qq_zz = qqZZ().full
    ww    = WW().full
    vv    = VV().full
    wjets = WJets().full
    other = Other().full

    procs = [ TTH, THQ, THW, TTW, TTZ, HH, DY, TT, WZ, ZZ, ggZZ, qqZZ, WW, VV, WJets, Other ]

    x1_up     = [ proc.x1_().up     for proc in procs ]
    x1_down   = [ proc.x1_().down   for proc in procs ]
    y1_up     = [ proc.y1_().up     for proc in procs ]
    y1_down   = [ proc.y1_().down   for proc in procs ]
    x1y1_up   = [ proc.x1y1_().up   for proc in procs ]
    x1y1_down = [ proc.x1y1_().down for proc in procs ]
    env_up    = [ proc.env_().up    for proc in procs ]
    env_down  = [ proc.env_().down  for proc in procs ]

    #full = ttH + tHq + tHW + ttW + ttZ + dy + hh + ttbar + wz + zz + gg_zz + qq_zz + ww + vv
    full = env_up + env_down

  class PDF(object):

    class TT(object):
      class env_(object):
        up   = "CMS_ttHl_PDF_shape_TTUp"
        down = "CMS_ttHl_PDF_shape_TTDown"
        mem  = "CMS_ttHl_PDF_shape_TTMember"
        full = [ up, down ]
      full = env_().full
      mem = [ env_().mem ]

    ttbar = TT().full

    procs = [ TT ]

    env_up    = [ proc.env_().up   for proc in procs ]
    env_down  = [ proc.env_().down for proc in procs ]
    mem       = [ proc.env_().mem  for proc in procs ]

    full = env_up + env_down

  class LeptonIDSF(object):
    el_tight = [ "CMS_ttHl_lepEff_eltightUp", "CMS_ttHl_lepEff_eltightDown" ]
    mu_tight = [ "CMS_ttHl_lepEff_mutightUp", "CMS_ttHl_lepEff_mutightDown" ]
    el_loose = [ "CMS_ttHl_lepEff_ellooseUp", "CMS_ttHl_lepEff_ellooseDown" ]
    mu_loose = [ "CMS_ttHl_lepEff_mulooseUp", "CMS_ttHl_lepEff_mulooseDown" ]

    full = el_tight + mu_tight + el_loose + mu_loose

  leptonIDSF_hh_recomp = [
    "CMS_ttHl_lepEff_eltightRecompUp", "CMS_ttHl_lepEff_eltightRecompDown",
    "CMS_ttHl_lepEff_mutightRecompUp", "CMS_ttHl_lepEff_mutightRecompDown",
  ]

  class Btag(object):

    HF       = [ "CMS_ttHl_btag_HFUp",       "CMS_ttHl_btag_HFDown"       ]
    HFStats1 = [ "CMS_ttHl_btag_HFStats1Up", "CMS_ttHl_btag_HFStats1Down" ]
    HFStats2 = [ "CMS_ttHl_btag_HFStats2Up", "CMS_ttHl_btag_HFStats2Down" ]

    LF       = [ "CMS_ttHl_btag_LFUp",       "CMS_ttHl_btag_LFDown"       ]
    LFStats1 = [ "CMS_ttHl_btag_LFStats1Up", "CMS_ttHl_btag_LFStats1Down" ]
    LFStats2 = [ "CMS_ttHl_btag_LFStats2Up", "CMS_ttHl_btag_LFStats2Down" ]

    cErr1 = [ "CMS_ttHl_btag_cErr1Up", "CMS_ttHl_btag_cErr1Down" ]
    cErr2 = [ "CMS_ttHl_btag_cErr2Up", "CMS_ttHl_btag_cErr2Down" ]

    full = HF + HFStats1 + HFStats2 + LF + LFStats1 + LFStats2 + cErr1 + cErr2

  class FakeRate_e_shape(object):
    pt         = [ "CMS_ttHl_FRe_shape_ptUp",         "CMS_ttHl_FRe_shape_ptDown"         ]
    norm       = [ "CMS_ttHl_FRe_shape_normUp",       "CMS_ttHl_FRe_shape_normDown"       ]
    eta_barrel = [ "CMS_ttHl_FRe_shape_eta_barrelUp", "CMS_ttHl_FRe_shape_eta_barrelDown" ]
    corr       = [ "CMS_ttHl_FRe_shape_corrUp",       "CMS_ttHl_FRe_shape_corrDown"       ]

    full = pt + norm + eta_barrel + corr

  class FakeRate_m_shape(object):
    pt         = [ "CMS_ttHl_FRm_shape_ptUp",         "CMS_ttHl_FRm_shape_ptDown"         ]
    norm       = [ "CMS_ttHl_FRm_shape_normUp",       "CMS_ttHl_FRm_shape_normDown"       ]
    eta_barrel = [ "CMS_ttHl_FRm_shape_eta_barrelUp", "CMS_ttHl_FRm_shape_eta_barrelDown" ]
    corr       = [ "CMS_ttHl_FRm_shape_corrUp",       "CMS_ttHl_FRm_shape_corrDown"       ]

    full = pt + norm + eta_barrel + corr

  FakeRate_l_shape_corr = FakeRate_e_shape().corr + FakeRate_m_shape().corr

  class FakeRate_t(object):
    jt_norm  = [ "CMS_ttHl_FRjt_normUp",  "CMS_ttHl_FRjt_normDown"  ]
    jt_shape = [ "CMS_ttHl_FRjt_shapeUp", "CMS_ttHl_FRjt_shapeDown" ]
    et_shift = [ "CMS_ttHl_FRet_shiftUp", "CMS_ttHl_FRet_shiftDown" ]
    mt_shift = [ "CMS_ttHl_FRmt_shiftUp", "CMS_ttHl_FRmt_shiftDown" ]

    jt   = jt_norm + jt_shape
    full = jt + et_shift + mt_shift

  class Electron_energy(object):
    ER       = [ "CMS_ttHl_electronERUp",       "CMS_ttHl_electronERDown"       ]
    ESEndcap = [ "CMS_ttHl_electronESEndcapUp", "CMS_ttHl_electronESEndcapDown" ]
    ESBarrel = [ "CMS_ttHl_electronESBarrelUp", "CMS_ttHl_electronESBarrelDown" ]

    full = ER + ESEndcap + ESBarrel

  class Muon_energy(object):
    ER        = [ "CMS_ttHl_muonERUp",        "CMS_ttHl_muonERDown"        ]
    ESBarrel1 = [ "CMS_ttHl_muonESBarrel1Up", "CMS_ttHl_muonESBarrel1Down" ]
    ESBarrel2 = [ "CMS_ttHl_muonESBarrel2Up", "CMS_ttHl_muonESBarrel2Down" ]
    ESEndcap1 = [ "CMS_ttHl_muonESEndcap1Up", "CMS_ttHl_muonESEndcap1Down" ]
    ESEndcap2 = [ "CMS_ttHl_muonESEndcap2Up", "CMS_ttHl_muonESEndcap2Down" ]

    full = ER + ESBarrel1 + ESBarrel2 + ESEndcap1 + ESEndcap2

  lhe          = LHE().full
  pdf          = PDF().full
  pdf_mem      = PDF().mem
  btag         = Btag().full
  leptonIDSF   = LeptonIDSF().full
  FRe_shape    = FakeRate_e_shape().full
  FRm_shape    = FakeRate_m_shape().full
  FRl_shape    = FRe_shape + FRm_shape
  FR_t         = FakeRate_t().full
  electron_E   = Electron_energy().full
  muon_E       = Muon_energy().full
  FR_all       = FRl_shape + FR_t
  partonShower = PartonShower().full
  ttbar        = TTbar().full

  # Analysis-specific definitions
  an_leptonFR        =    central +  JES +  JER          + UnclusteredEn
  an_leptonFR_opts   = [ "central", "JES", "JER",         "UnclusteredEn"]
  an_jetToTauFR      =    central +  JES         + tauES                   + tauIDSF
  an_jetToTauFR_opts = [ "central", "JES", "JER",          "UnclusteredEn", "tauIDSF" ]
  an_addMEM          =    central +  JES +  JER +  tauES +  UnclusteredEn
  an_addMEM_opts     = [ "central", "JES", "JER", "tauES", "UnclusteredEn" ]

  an_chargeFlip_e       =    central +  electron_E
  an_chargeFlip_e_opts  = [ "central", "electron_E" ]
  an_chargeFlip_mu      =    central +  muon_E
  an_chargeFlip_mu_opts = [ "central", "muon_E" ]

  an_tth = central + JES + JER + tauES + leptonIDSF + tauIDSF + UnclusteredEn + btag + FR_t + FRe_shape + FRm_shape + \
           lhe + triggerSF + PU + DYMCReweighting + DYMCNormScaleFactors + L1PreFiring + EWK_jet + EWK_bjet + partonShower + \
           mcClosure
  an_tth_opts = [
    "central", "JES", "JER", "tauES", "leptonIDSF", "tauIDSF", "UnclusteredEn", "btag", "FR_t", "FRe_shape", "FRm_shape",
    "lhe", "triggerSF", "PU", "DYMCReweighting", "DYMCNormScaleFactors", "L1PreFiring", "EWK_jet", "EWK_bjet", "partonShower",
    mcClosure_str,
  ]

  an_inclusive      =    central +  JES +  JER +  tauES +  UnclusteredEn +  btag
  an_inclusive_opts = [ "central", "JES", "JER", "tauES", "UnclusteredEn", "btag" ]

  an_hh_multilepton      = an_tth      +    leptonIDSF_hh_recomp +  topPtReweighting
  an_opts_hh_multilepton = an_tth_opts + [ "leptonIDSF_hh_recomp", "topPtReweighting" ]

  an_hh_multilepton_wAK8      = an_hh_multilepton      +    AK8
  an_opts_hh_multilepton_wAK8 = an_opts_hh_multilepton + [ "AK8" ]

  an_internal_no_mem = central + leptonIDSF + tauIDSF + btag + FR_t + lhe + triggerSF + PU + L1PreFiring + \
                       FRe_shape + FRm_shape + DYMCReweighting + DYMCNormScaleFactors + topPtReweighting + \
                       EWK_jet + EWK_bjet + partonShower + leptonIDSF_hh_recomp + pileupJetID + pdf + pdf_mem + \
                       LHEVpt + subjetBtag
  an_internal = an_internal_no_mem + MEM + triggerSF_split
