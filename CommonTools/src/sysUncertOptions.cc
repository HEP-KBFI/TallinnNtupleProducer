#include "TallinnNtupleProducer/CommonTools/interface/sysUncertOptions.h"

#include "TallinnNtupleProducer/CommonTools/interface/cmsException.h"            // cmsException
#include "TallinnNtupleProducer/CommonTools/interface/Era.h"                     // Era
#include "TallinnNtupleProducer/CommonTools/interface/jetDefinitions.h"          // Btag
#include "TallinnNtupleProducer/CommonTools/interface/merge_systematic_shifts.h" // merge_systematic_shifts()
#include "TallinnNtupleProducer/CommonTools/interface/contains.h"                // contains()
#include "TallinnNtupleProducer/CommonTools/interface/map_keys.h"                // map_keys()

#include <TString.h>                                                    // Form()

#include <boost/algorithm/string/predicate.hpp>                         // boost::algorithm::starts_with(), boost::algorithm::ends_with()

#include <assert.h>                                                     // assert()
#include <regex>                                                        // std::regex, std::smatch, std::regex_match()

const std::string pdfSysStr = "CMS_ttHl_PDF_shape_Member";

const std::map<std::string, int> btagWeightSysMap = {
  { "CMS_ttHl_btag_HFUp",                 kBtag_hfUp                      },
  { "CMS_ttHl_btag_HFDown",               kBtag_hfDown                    },
  { "CMS_ttHl_btag_HFStats1Up",           kBtag_hfStats1Up                },
  { "CMS_ttHl_btag_HFStats1Down",         kBtag_hfStats1Down              },
  { "CMS_ttHl_btag_HFStats2Up",           kBtag_hfStats2Up                },
  { "CMS_ttHl_btag_HFStats2Down",         kBtag_hfStats2Down              },
  { "CMS_ttHl_btag_LFUp",                 kBtag_lfUp                      },
  { "CMS_ttHl_btag_LFDown",               kBtag_lfDown                    },
  { "CMS_ttHl_btag_LFStats1Up",           kBtag_lfStats1Up                },
  { "CMS_ttHl_btag_LFStats1Down",         kBtag_lfStats1Down              },
  { "CMS_ttHl_btag_LFStats2Up",           kBtag_lfStats2Up                },
  { "CMS_ttHl_btag_LFStats2Down",         kBtag_lfStats2Down              },
  { "CMS_ttHl_btag_cErr1Up",              kBtag_cErr1Up                   },
  { "CMS_ttHl_btag_cErr1Down",            kBtag_cErr1Down                 },
  { "CMS_ttHl_btag_cErr2Up",              kBtag_cErr2Up                   },
  { "CMS_ttHl_btag_cErr2Down",            kBtag_cErr2Down                 },
  { "CMS_ttHl_JESUp",                     kBtag_jesTotalUp                },
  { "CMS_ttHl_JESDown",                   kBtag_jesTotalDown              },
  { "CMS_ttHl_JESAbsoluteUp",             kBtag_jesAbsoluteUp             },
  { "CMS_ttHl_JESAbsoluteDown",           kBtag_jesAbsoluteDown           },
  { "CMS_ttHl_JESAbsolute_EraUp",         kBtag_jesAbsolute_EraUp         },
  { "CMS_ttHl_JESAbsolute_EraDown",       kBtag_jesAbsolute_EraDown       },
  { "CMS_ttHl_JESBBEC1Up",                kBtag_jesBBEC1Up                },
  { "CMS_ttHl_JESBBEC1Down",              kBtag_jesBBEC1Down              },
  { "CMS_ttHl_JESBBEC1_EraUp",            kBtag_jesBBEC1_EraUp            },
  { "CMS_ttHl_JESBBEC1_EraDown",          kBtag_jesBBEC1_EraDown          },
  { "CMS_ttHl_JESEC2Up",                  kBtag_jesEC2Up                  },
  { "CMS_ttHl_JESEC2Down",                kBtag_jesEC2Down                },
  { "CMS_ttHl_JESEC2_EraUp",              kBtag_jesEC2_EraUp              },
  { "CMS_ttHl_JESEC2_EraDown",            kBtag_jesEC2_EraDown            },
  { "CMS_ttHl_JESFlavorQCDUp",            kBtag_jesFlavorQCDUp            },
  { "CMS_ttHl_JESFlavorQCDDown",          kBtag_jesFlavorQCDDown          },
  { "CMS_ttHl_JESHFUp",                   kBtag_jesHFUp                   },
  { "CMS_ttHl_JESHFDown",                 kBtag_jesHFDown                 },
  { "CMS_ttHl_JESHF_EraUp",               kBtag_jesHF_EraUp               },
  { "CMS_ttHl_JESHF_EraDown",             kBtag_jesHF_EraDown             },
  { "CMS_ttHl_JESRelativeBalUp",          kBtag_jesRelativeBalUp          },
  { "CMS_ttHl_JESRelativeBalDown",        kBtag_jesRelativeBalDown        },
  { "CMS_ttHl_JESRelativeSample_EraUp",   kBtag_jesRelativeSample_EraUp   },
  { "CMS_ttHl_JESRelativeSample_EraDown", kBtag_jesRelativeSample_EraDown },
};

const std::map<std::string, pileupJetIDSFsys> pileupJetIDSysMap = {
  { "CMS_ttHl_puJetIDEffUp",      pileupJetIDSFsys::effUp      },
  { "CMS_ttHl_puJetIDEffDown",    pileupJetIDSFsys::effDown    },
  { "CMS_ttHl_puJetIDMistagUp",   pileupJetIDSFsys::mistagUp   },
  { "CMS_ttHl_puJetIDMistagDown", pileupJetIDSFsys::mistagDown },
};

const std::map<std::string, int> jesAK4SysMap = {
  { "CMS_ttHl_JESUp",                     kJetMET_jesUp                     },
  { "CMS_ttHl_JESDown",                   kJetMET_jesDown                   },
};

const std::map<std::string, int> jesSplitAK4SysMap = {
  { "CMS_ttHl_JESAbsoluteUp",             kJetMET_jesAbsoluteUp             },
  { "CMS_ttHl_JESAbsoluteDown",           kJetMET_jesAbsoluteDown           },
  { "CMS_ttHl_JESAbsolute_EraUp",         kJetMET_jesAbsolute_EraUp         },
  { "CMS_ttHl_JESAbsolute_EraDown",       kJetMET_jesAbsolute_EraDown       },
  { "CMS_ttHl_JESBBEC1Up",                kJetMET_jesBBEC1Up                },
  { "CMS_ttHl_JESBBEC1Down",              kJetMET_jesBBEC1Down              },
  { "CMS_ttHl_JESBBEC1_EraUp",            kJetMET_jesBBEC1_EraUp            },
  { "CMS_ttHl_JESBBEC1_EraDown",          kJetMET_jesBBEC1_EraDown          },
  { "CMS_ttHl_JESEC2Up",                  kJetMET_jesEC2Up                  },
  { "CMS_ttHl_JESEC2Down",                kJetMET_jesEC2Down                },
  { "CMS_ttHl_JESEC2_EraUp",              kJetMET_jesEC2_EraUp              },
  { "CMS_ttHl_JESEC2_EraDown",            kJetMET_jesEC2_EraDown            },
  { "CMS_ttHl_JESFlavorQCDUp",            kJetMET_jesFlavorQCDUp            },
  { "CMS_ttHl_JESFlavorQCDDown",          kJetMET_jesFlavorQCDDown          },
  { "CMS_ttHl_JESHFUp",                   kJetMET_jesHFUp                   },
  { "CMS_ttHl_JESHFDown",                 kJetMET_jesHFDown                 },
  { "CMS_ttHl_JESHF_EraUp",               kJetMET_jesHF_EraUp               },
  { "CMS_ttHl_JESHF_EraDown",             kJetMET_jesHF_EraDown             },
  { "CMS_ttHl_JESRelativeBalUp",          kJetMET_jesRelativeBalUp          },
  { "CMS_ttHl_JESRelativeBalDown",        kJetMET_jesRelativeBalDown        },
  { "CMS_ttHl_JESRelativeSample_EraUp",   kJetMET_jesRelativeSample_EraUp   },
  { "CMS_ttHl_JESRelativeSample_EraDown", kJetMET_jesRelativeSample_EraDown },
  { "CMS_ttHl_JESHEMUp",                  kJetMET_jesHEMUp                  },
  { "CMS_ttHl_JESHEMDown",                kJetMET_jesHEMDown                },
};

const std::map<std::string, int> jerAK4SysMap = {
  { "CMS_ttHl_JERUp",                     kJetMET_jerUp                     },
  { "CMS_ttHl_JERDown",                   kJetMET_jerDown                   },
};

const std::map<std::string, int> jerSplitAK4SysMap = {  
  { "CMS_ttHl_JERBarrelUp",               kJetMET_jerBarrelUp               },
  { "CMS_ttHl_JERBarrelDown",             kJetMET_jerBarrelDown             },
  { "CMS_ttHl_JEREndcap1Up",              kJetMET_jerEndcap1Up              },
  { "CMS_ttHl_JEREndcap1Down",            kJetMET_jerEndcap1Down            },
  { "CMS_ttHl_JEREndcap2LowPtUp",         kJetMET_jerEndcap2LowPtUp         },
  { "CMS_ttHl_JEREndcap2LowPtDown",       kJetMET_jerEndcap2LowPtDown       },
  { "CMS_ttHl_JEREndcap2HighPtUp",        kJetMET_jerEndcap2HighPtUp        },
  { "CMS_ttHl_JEREndcap2HighPtDown",      kJetMET_jerEndcap2HighPtDown      },
  { "CMS_ttHl_JERForwardLowPtUp",         kJetMET_jerForwardLowPtUp         },
  { "CMS_ttHl_JERForwardLowPtDown",       kJetMET_jerForwardLowPtDown       },
  { "CMS_ttHl_JERForwardHighPtUp",        kJetMET_jerForwardHighPtUp        },
  { "CMS_ttHl_JERForwardHighPtDown",      kJetMET_jerForwardHighPtDown      },
};

const std::map<std::string, int> metSysMap = {
  { "CMS_ttHl_UnclusteredEnUp",   kJetMET_UnclusteredEnUp   },
  { "CMS_ttHl_UnclusteredEnDown", kJetMET_UnclusteredEnDown },
};

const std::map<std::string, int> jesAK8SysMap = {
  { "CMS_ttHl_JESUp",                     kFatJet_jesUp                     },
  { "CMS_ttHl_JESDown",                   kFatJet_jesDown                   },
};

const std::map<std::string, int> jesSplitAK8SysMap = {
  { "CMS_ttHl_JESAbsoluteUp",             kFatJet_jesAbsoluteUp             },
  { "CMS_ttHl_JESAbsoluteDown",           kFatJet_jesAbsoluteDown           },
  { "CMS_ttHl_JESAbsolute_EraUp",         kFatJet_jesAbsolute_EraUp         },
  { "CMS_ttHl_JESAbsolute_EraDown",       kFatJet_jesAbsolute_EraDown       },
  { "CMS_ttHl_JESBBEC1Up",                kFatJet_jesBBEC1Up                },
  { "CMS_ttHl_JESBBEC1Down",              kFatJet_jesBBEC1Down              },
  { "CMS_ttHl_JESBBEC1_EraUp",            kFatJet_jesBBEC1_EraUp            },
  { "CMS_ttHl_JESBBEC1_EraDown",          kFatJet_jesBBEC1_EraDown          },
  { "CMS_ttHl_JESEC2Up",                  kFatJet_jesEC2Up                  },
  { "CMS_ttHl_JESEC2Down",                kFatJet_jesEC2Down                },
  { "CMS_ttHl_JESEC2_EraUp",              kFatJet_jesEC2_EraUp              },
  { "CMS_ttHl_JESEC2_EraDown",            kFatJet_jesEC2_EraDown            },
  { "CMS_ttHl_JESFlavorQCDUp",            kFatJet_jesFlavorQCDUp            },
  { "CMS_ttHl_JESFlavorQCDDown",          kFatJet_jesFlavorQCDDown          },
  { "CMS_ttHl_JESHFUp",                   kFatJet_jesHFUp                   },
  { "CMS_ttHl_JESHFDown",                 kFatJet_jesHFDown                 },
  { "CMS_ttHl_JESHF_EraUp",               kFatJet_jesHF_EraUp               },
  { "CMS_ttHl_JESHF_EraDown",             kFatJet_jesHF_EraDown             },
  { "CMS_ttHl_JESRelativeBalUp",          kFatJet_jesRelativeBalUp          },
  { "CMS_ttHl_JESRelativeBalDown",        kFatJet_jesRelativeBalDown        },
  { "CMS_ttHl_JESRelativeSample_EraUp",   kFatJet_jesRelativeSample_EraUp   },
  { "CMS_ttHl_JESRelativeSample_EraDown", kFatJet_jesRelativeSample_EraDown },
  { "CMS_ttHl_JESHEMUp",                  kFatJet_jesHEMUp                  },
  { "CMS_ttHl_JESHEMDown",                kFatJet_jesHEMDown                },
};

const std::map<std::string, int> jerAK8SysMap = {
  { "CMS_ttHl_AK8JERUp",                  kFatJet_jerUp                     },
  { "CMS_ttHl_AK8JERDown",                kFatJet_jerDown                   },
};

const std::map<std::string, int> jerSplitAK8SysMap = {
//  { "CMS_ttHl_AK8JERBarrelUp",            kFatJet_jerBarrelUp               },
//  { "CMS_ttHl_AK8JERBarrelDown",          kFatJet_jerBarrelDown             },
//  { "CMS_ttHl_AK8JEREndcap1Up",           kFatJet_jerEndcap1Up              },
//  { "CMS_ttHl_AK8JEREndcap1Down",         kFatJet_jerEndcap1Down            },
//  { "CMS_ttHl_AK8JEREndcap2LowPtUp",      kFatJet_jerEndcap2LowPtUp         },
//  { "CMS_ttHl_AK8JEREndcap2LowPtDown",    kFatJet_jerEndcap2LowPtDown       },
//  { "CMS_ttHl_AK8JEREndcap2HighPtUp",     kFatJet_jerEndcap2HighPtUp        },
//  { "CMS_ttHl_AK8JEREndcap2HighPtDown",   kFatJet_jerEndcap2HighPtDown      },
//  { "CMS_ttHl_AK8JERForwardLowPtUp",      kFatJet_jerForwardLowPtUp         },
//  { "CMS_ttHl_AK8JERForwardLowPtDown",    kFatJet_jerForwardLowPtDown       },
//  { "CMS_ttHl_AK8JERForwardHighPtUp",     kFatJet_jerForwardHighPtUp        },
//  { "CMS_ttHl_AK8JERForwardHighPtDown",   kFatJet_jerForwardHighPtDown      },
};

const std::map<std::string, int> jmsAK8SysMap = {
  { "CMS_ttHl_AK8JMSUp",                  kFatJet_jmsUp                     },
  { "CMS_ttHl_AK8JMSDown",                kFatJet_jmsDown                   },
};

const std::map<std::string, int> jmrAK8SysMap = {
  { "CMS_ttHl_AK8JMRUp",                  kFatJet_jmrUp                     },
  { "CMS_ttHl_AK8JMRDown",                kFatJet_jmrDown                   },
};

const std::map<std::string, int> hadTauESSysMap = {
  { "CMS_ttHl_tauESUp",   kHadTauPt_shiftUp   },
  { "CMS_ttHl_tauESDown", kHadTauPt_shiftDown },
};

const std::map<std::string, int> jetToTauFRSysMap = {
  { "CMS_ttHl_FRjt_normUp",    kFRjt_normUp    },
  { "CMS_ttHl_FRjt_normDown",  kFRjt_normDown  },
  { "CMS_ttHl_FRjt_shapeUp",   kFRjt_shapeUp   },
  { "CMS_ttHl_FRjt_shapeDown", kFRjt_shapeDown },
};

const std::map<std::string, FRet> eToTauFRSysMap = {
  { "CMS_ttHl_FRet_shiftUp",   FRet::shiftUp   },
  { "CMS_ttHl_FRet_shiftDown", FRet::shiftDown },
};

const std::map<std::string, FRmt> mToTauFRSysMap = {
  { "CMS_ttHl_FRmt_shiftUp",   FRmt::shiftUp   },
  { "CMS_ttHl_FRmt_shiftDown", FRmt::shiftDown },
};

const std::map<std::string, LeptonIDSFsys> leptonIDSFSysMap = {
  { "CMS_ttHl_lepEff_eltightUp",         LeptonIDSFsys::elTightUp         },
  { "CMS_ttHl_lepEff_eltightDown",       LeptonIDSFsys::elTightDown       },
  { "CMS_ttHl_lepEff_mutightUp",         LeptonIDSFsys::muTightUp         },
  { "CMS_ttHl_lepEff_mutightDown",       LeptonIDSFsys::muTightDown       },
  { "CMS_ttHl_lepEff_ellooseUp",         LeptonIDSFsys::elLooseUp         },
  { "CMS_ttHl_lepEff_ellooseDown",       LeptonIDSFsys::elLooseDown       },
  { "CMS_ttHl_lepEff_mulooseUp",         LeptonIDSFsys::muLooseUp         },
  { "CMS_ttHl_lepEff_mulooseDown",       LeptonIDSFsys::muLooseDown       },
  { "CMS_ttHl_lepEff_eltightRecompUp",   LeptonIDSFsys::elTightRecompUp   },
  { "CMS_ttHl_lepEff_eltightRecompDown", LeptonIDSFsys::elTightRecompDown },
  { "CMS_ttHl_lepEff_mutightRecompUp",   LeptonIDSFsys::muTightRecompUp   },
  { "CMS_ttHl_lepEff_mutightRecompDown", LeptonIDSFsys::muTightRecompDown },
};

const std::map<std::string, TauIDSFsys> tauIDSFSysMap = {
  { "CMS_ttHl_tauIDSFUp",   TauIDSFsys::shiftUp   },
  { "CMS_ttHl_tauIDSFDown", TauIDSFsys::shiftDown },
};

const std::map<std::string, TriggerSFsys> triggerSFSysMap = {
  { "CMS_ttHl_triggerUp",            TriggerSFsys::shiftUp            },
  { "CMS_ttHl_triggerDown",          TriggerSFsys::shiftDown          },
  { "CMS_ttHl_trigger_2lssUp",       TriggerSFsys::shift_2lssUp       },
  { "CMS_ttHl_trigger_2lssDown",     TriggerSFsys::shift_2lssDown     },
  { "CMS_ttHl_trigger_2lssEEUp",     TriggerSFsys::shift_2lssEEUp     },
  { "CMS_ttHl_trigger_2lssEEDown",   TriggerSFsys::shift_2lssEEDown   },
  { "CMS_ttHl_trigger_2lssEMuUp",    TriggerSFsys::shift_2lssEMuUp    },
  { "CMS_ttHl_trigger_2lssEMuDown",  TriggerSFsys::shift_2lssEMuDown  },
  { "CMS_ttHl_trigger_2lssMuMuUp",   TriggerSFsys::shift_2lssMuMuUp   },
  { "CMS_ttHl_trigger_2lssMuMuDown", TriggerSFsys::shift_2lssMuMuDown },
  { "CMS_ttHl_trigger_1l1tauUp",     TriggerSFsys::shift_1l1tauUp     },
  { "CMS_ttHl_trigger_1l1tauDown",   TriggerSFsys::shift_1l1tauDown   },
  { "CMS_ttHl_trigger_0l2tauUp",     TriggerSFsys::shift_0l2tauUp     },
  { "CMS_ttHl_trigger_0l2tauDown",   TriggerSFsys::shift_0l2tauDown   },
};

const std::map<std::string, int> lheScaleSysMap = {
  { "CMS_ttHl_thu_shape_x1y1Up",   kLHE_scale_xyUp   },
  { "CMS_ttHl_thu_shape_x1y1Down", kLHE_scale_xyDown },
  { "CMS_ttHl_thu_shape_x1Up",     kLHE_scale_xUp    },
  { "CMS_ttHl_thu_shape_x1Down",   kLHE_scale_xDown  },
  { "CMS_ttHl_thu_shape_y1Up",     kLHE_scale_yUp    },
  { "CMS_ttHl_thu_shape_y1Down",   kLHE_scale_yDown  },
};

const std::map<std::string, int> psSysMap = {
  { "CMS_ttHl_PS_ISRDown", kPartonShower_ISRDown },
  { "CMS_ttHl_PS_ISRUp",   kPartonShower_ISRUp   },
  { "CMS_ttHl_PS_FSRDown", kPartonShower_FSRDown },
  { "CMS_ttHl_PS_FSRUp",   kPartonShower_FSRUp   },
};

const std::map<std::string, ElectronPtSys> ePtSysMap = {
  { "CMS_ttHl_electronESBarrelUp",   ElectronPtSys::scaleUp_barrel   },
  { "CMS_ttHl_electronESBarrelDown", ElectronPtSys::scaleDown_barrel },
  { "CMS_ttHl_electronESEndcapUp",   ElectronPtSys::scaleUp_endcap   },
  { "CMS_ttHl_electronESEndcapDown", ElectronPtSys::scaleDown_endcap },
  { "CMS_ttHl_electronERUp",         ElectronPtSys::resUp            },
  { "CMS_ttHl_electronERDown",       ElectronPtSys::resDown          },
};

const std::map<std::string, MuonPtSys> mPtSysMap = {
  { "CMS_ttHl_muonERUp",          MuonPtSys::ERUp          },
  { "CMS_ttHl_muonERDown",        MuonPtSys::ERDown        },
  { "CMS_ttHl_muonESBarrel1Up",   MuonPtSys::ESBarrel1Up   },
  { "CMS_ttHl_muonESBarrel1Down", MuonPtSys::ESBarrel1Down },
  { "CMS_ttHl_muonESBarrel2Up",   MuonPtSys::ESBarrel2Up   },
  { "CMS_ttHl_muonESBarrel2Down", MuonPtSys::ESBarrel2Down },
  { "CMS_ttHl_muonESEndcap1Up",   MuonPtSys::ESEndcap1Up   },
  { "CMS_ttHl_muonESEndcap1Down", MuonPtSys::ESEndcap1Down },
  { "CMS_ttHl_muonESEndcap2Up",   MuonPtSys::ESEndcap2Up   },
  { "CMS_ttHl_muonESEndcap2Down", MuonPtSys::ESEndcap2Down },
};

const std::map<std::string, int> jetToLeptonFRSysMap = {
  { "CMS_ttHl_FRe_shape_ptUp",           kFRe_shape_ptUp           },
  { "CMS_ttHl_FRe_shape_ptDown",         kFRe_shape_ptDown         },
  { "CMS_ttHl_FRe_shape_normUp",         kFRe_shape_normUp         },
  { "CMS_ttHl_FRe_shape_normDown",       kFRe_shape_normDown       },
  { "CMS_ttHl_FRe_shape_eta_barrelUp",   kFRe_shape_eta_barrelUp   },
  { "CMS_ttHl_FRe_shape_eta_barrelDown", kFRe_shape_eta_barrelDown },
  { "CMS_ttHl_FRe_shape_corrUp",         kFRe_shape_corrUp         },
  { "CMS_ttHl_FRe_shape_corrDown",       kFRe_shape_corrDown       },
  { "CMS_ttHl_FRm_shape_ptUp",           kFRm_shape_ptUp           },
  { "CMS_ttHl_FRm_shape_ptDown",         kFRm_shape_ptDown         },
  { "CMS_ttHl_FRm_shape_normUp",         kFRm_shape_normUp         },
  { "CMS_ttHl_FRm_shape_normDown",       kFRm_shape_normDown       },
  { "CMS_ttHl_FRm_shape_eta_barrelUp",   kFRm_shape_eta_barrelUp   },
  { "CMS_ttHl_FRm_shape_eta_barrelDown", kFRm_shape_eta_barrelDown },
  { "CMS_ttHl_FRm_shape_corrUp",         kFRm_shape_corrUp         },
  { "CMS_ttHl_FRm_shape_corrDown",       kFRm_shape_corrDown       },
};

const std::map<std::string, PUsys> puSysMap = {
  { "CMS_ttHl_pileupUp",   PUsys::up   },
  { "CMS_ttHl_pileupDown", PUsys::down },
};

const std::map<std::string, L1PreFiringWeightSys> l1prefireSysMap = {
  { "CMS_ttHl_l1PreFireUp",   L1PreFiringWeightSys::up   },
  { "CMS_ttHl_l1PreFireDown", L1PreFiringWeightSys::down },  
};

const std::map<std::string, int> dyMCRwgtSysMap = {
  { "CMS_ttHl_DYMCReweightingUp",   kDYMCReweighting_shiftUp   },
  { "CMS_ttHl_DYMCReweightingDown", kDYMCReweighting_shiftDown },
};

const std::map<std::string, int> dyMCNormSysMap = {
  { "CMS_ttHl_DYMCNormScaleFactorsUp",   kDYMCNormScaleFactors_shiftUp   },
  { "CMS_ttHl_DYMCNormScaleFactorsDown", kDYMCNormScaleFactors_shiftDown },
};

const std::map<std::string, int> topPtRwgtSysMap = {
  { "CMS_ttHl_topPtReweightingUp",   kTopPtReweighting_shiftUp   },
  { "CMS_ttHl_topPtReweightingDown", kTopPtReweighting_shiftDown },
};

const std::map<std::string, EWKJetSys> ewkJetSysMap = {
  { "CMS_ttHl_EWK_jetUp",   EWKJetSys::up   },
  { "CMS_ttHl_EWK_jetDown", EWKJetSys::down },
};

const std::map<std::string, EWKBJetSys> ewkBJetSysMap = {
  { "CMS_ttHl_EWK_btagUp",   EWKBJetSys::up   },
  { "CMS_ttHl_EWK_btagDown", EWKBJetSys::down },
};

const std::map<std::string, PDFSys> pdfSysMap = {
  { "CMS_ttHl_PDF_shape_Up",   PDFSys::up   },
  { "CMS_ttHl_PDF_shape_Down", PDFSys::down },  
};

const std::map<std::string, LHEVptSys> lheVptSysMap = {
  { "Vpt_nloUp",   LHEVptSys::up   },
  { "Vpt_nloDown", LHEVptSys::down },
};

const std::map<std::string, SubjetBtagSys> subjetBtagSysMap = {
  { "CMS_btag_subjetUp",   SubjetBtagSys::up   },
  { "CMS_btag_subjetDown", SubjetBtagSys::down },
};

extern const std::map<int, std::string> btagWeightSysMap_correctionLib = {
  { kBtag_central,                   "central"                    },
  { kBtag_hfUp,                      "up_hf"                      },
  { kBtag_hfDown,                    "down_hf"                    },
  { kBtag_hfStats1Up,                "up_hfstats1"                },
  { kBtag_hfStats1Down,              "down_hfstats1"              },
  { kBtag_hfStats2Up,                "up_hfstats2"                },
  { kBtag_hfStats2Down,              "down_hfstats2"              },
  { kBtag_lfUp,                      "up_lf"                      },
  { kBtag_lfDown,                    "down_lf"                    },
  { kBtag_lfStats1Up,                "up_lfstats1"                },
  { kBtag_lfStats1Down,              "down_lfstats1"              },
  { kBtag_lfStats2Up,                "up_lfstats2"                },
  { kBtag_lfStats2Down,              "down_lfstats2"              },
  { kBtag_cErr1Up,                   "up_cferr1"                  },
  { kBtag_cErr1Down,                 "down_cferr1"                },
  { kBtag_cErr2Up,                   "up_cferr2"                  },
  { kBtag_cErr2Down,                 "down_cferr2"                },
  { kBtag_jesTotalUp,                "up_jes"                     },
  { kBtag_jesTotalDown,              "down_jes"                   },
  { kBtag_jesAbsoluteUp,             "up_jesAbsolute"             },
  { kBtag_jesAbsoluteDown,           "down_jesAbsolute"           },
  { kBtag_jesAbsolute_EraUp,         "up_jesAbsolute_ERA"         },
  { kBtag_jesAbsolute_EraDown,       "down_jesAbsolute_ERA"       },
  { kBtag_jesBBEC1Up,                "up_jesBBEC1"                },
  { kBtag_jesBBEC1Down,              "down_jesBBEC1"              },
  { kBtag_jesBBEC1_EraUp,            "up_jesBBEC1_ERA"            },
  { kBtag_jesBBEC1_EraDown,          "down_jesBBEC1_ERA"          },
  { kBtag_jesEC2Up,                  "up_jesEC2"                  },
  { kBtag_jesEC2Down,                "down_jesEC2"                },
  { kBtag_jesEC2_EraUp,              "up_jesEC2_ERA"              },
  { kBtag_jesEC2_EraDown,            "down_jesEC2_ERA"            },
  { kBtag_jesFlavorQCDUp,            "up_jesFlavorQCD"            },
  { kBtag_jesFlavorQCDDown,          "down_jesFlavorQCD"          },
  { kBtag_jesHFUp,                   "up_jesHF"                   },
  { kBtag_jesHFDown,                 "down_jesHF"                 },
  { kBtag_jesHF_EraUp,               "up_jesHF_ERA"               },
  { kBtag_jesHF_EraDown,             "down_jesHF_ERA"             },
  { kBtag_jesRelativeBalUp,          "up_jesRelativeBal"          },
  { kBtag_jesRelativeBalDown,        "down_jesRelativeBal"        },
  { kBtag_jesRelativeSample_EraUp,   "up_jesRelativeSample_ERA"   },
  { kBtag_jesRelativeSample_EraDown, "down_jesRelativeSample_ERA" },
};

std::vector<std::string>
get_inclusive_systeatics(const std::vector<std::string> & systematics)
{
  static std::vector<std::string> exclusive_systematics;
  if(exclusive_systematics.empty())
  {
    merge_systematic_shifts(exclusive_systematics, map_keys(jesAK4SysMap));
    merge_systematic_shifts(exclusive_systematics, map_keys(jesSplitAK4SysMap));
    merge_systematic_shifts(exclusive_systematics, map_keys(jerAK4SysMap));
    merge_systematic_shifts(exclusive_systematics, map_keys(jerSplitAK4SysMap));
    merge_systematic_shifts(exclusive_systematics, map_keys(metSysMap));
    merge_systematic_shifts(exclusive_systematics, map_keys(jesAK8SysMap));
    merge_systematic_shifts(exclusive_systematics, map_keys(jesSplitAK8SysMap));
    merge_systematic_shifts(exclusive_systematics, map_keys(jerAK8SysMap));
    merge_systematic_shifts(exclusive_systematics, map_keys(jerSplitAK8SysMap));
    merge_systematic_shifts(exclusive_systematics, map_keys(jmsAK8SysMap));
    merge_systematic_shifts(exclusive_systematics, map_keys(jmrAK8SysMap));
    merge_systematic_shifts(exclusive_systematics, map_keys(hadTauESSysMap));
    merge_systematic_shifts(exclusive_systematics, map_keys(ePtSysMap));
    merge_systematic_shifts(exclusive_systematics, map_keys(mPtSysMap));
  }
  std::vector<std::string> inclusive_systematics;
  for(const std::string & sys_str: systematics)
  {
    if(! contains(exclusive_systematics, sys_str))
    {
      inclusive_systematics.push_back(sys_str);
    }
  }
  return inclusive_systematics;
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
  const auto kv = btagWeightSysMap.find(central_or_shift);
  if(kv != btagWeightSysMap.end())
  {
    return kv->second;
  }
  return kBtag_central;
}

pileupJetIDSFsys
getPileupJetIDSFsys_option(const std::string & central_or_shift)
{
  const auto kv = pileupJetIDSysMap.find(central_or_shift);
  if(kv != pileupJetIDSysMap.end())
  {
    return kv->second;
  }
  return pileupJetIDSFsys::central;
}

int
getJet_option(const std::string & central_or_shift,
              bool isMC)
{
  if(isMC)
  {
    for(const auto & map: { jesAK4SysMap, jesSplitAK4SysMap, jerAK4SysMap, jerSplitAK4SysMap })
    {
      const auto kv = map.find(central_or_shift);
      if(kv != map.end())
      {
        return kv->second;
      }
    }
    return kJetMET_central;
  }
  return kJetMET_central_nonNominal;
}

int
getMET_option(const std::string & central_or_shift,
              bool isMC)
{
  if(isMC)
  {
    const auto kv = metSysMap.find(central_or_shift);
    if(kv != metSysMap.end())
    {
      return kv->second;
    }
  }
  return getJet_option(central_or_shift, isMC);
}

int
getFatJet_option(const std::string & central_or_shift,
                 bool isMC)
{
  if(isMC)
  {
    for(const auto & map: { jesAK8SysMap, jesSplitAK8SysMap, jerAK8SysMap, jerSplitAK8SysMap, jmsAK8SysMap, jmrAK8SysMap })
    {
      const auto kv = map.find(central_or_shift);
      if(kv != map.end())
      {
        return kv->second;
      }
    }
    return kFatJet_central;
  }
  return kFatJet_central_nonNominal;
}

int
getHadTauPt_option(const std::string & central_or_shift)
{
  const auto kv = hadTauESSysMap.find(central_or_shift);
  if(kv != hadTauESSysMap.end())
  {
    return kv->second;
  }
  return kHadTauPt_central;
}

int
getJetToTauFR_option(const std::string & central_or_shift)
{
  const auto kv = jetToTauFRSysMap.find(central_or_shift);
  if(kv != jetToTauFRSysMap.end())
  {
    return kv->second;
  }
  return kFRjt_central;
}

FRet
getEToTauFR_option(const std::string & central_or_shift)
{
  const auto kv = eToTauFRSysMap.find(central_or_shift);
  if(kv != eToTauFRSysMap.end())
  {
    return kv->second;
  }
  return FRet::central;
}

FRmt
getMuToTauFR_option(const std::string & central_or_shift)
{
  const auto kv = mToTauFRSysMap.find(central_or_shift);
  if(kv != mToTauFRSysMap.end())
  {
    return kv->second;
  }
  return FRmt::central;
}

LeptonIDSFsys
getLeptonIDSFsys_option(const std::string & central_or_shift)
{
  const auto kv = leptonIDSFSysMap.find(central_or_shift);
  if(kv != leptonIDSFSysMap.end())
  {
    return kv->second;
  }
  return LeptonIDSFsys::central;
}

TauIDSFsys
getTauIDSFsys_option(const std::string & central_or_shift)
{
  const auto kv = tauIDSFSysMap.find(central_or_shift);
  if(kv != tauIDSFSysMap.end())
  {
    return kv->second;
  }
  return TauIDSFsys::central;
}

TriggerSFsys
getTriggerSF_option(const std::string & central_or_shift,
                    TriggerSFsysChoice choice)
{
  const bool isLeptonCompatible = choice == TriggerSFsysChoice::any || choice == TriggerSFsysChoice::leptonOnly;
  const bool isHadTauCompatible = choice == TriggerSFsysChoice::any || choice == TriggerSFsysChoice::hadTauOnly;
  const bool isAnyCompatible = isLeptonCompatible || isHadTauCompatible;

  const auto kv = triggerSFSysMap.find(central_or_shift);
  if(kv != triggerSFSysMap.end())
  {
    const TriggerSFsys central_or_shift_int = kv->second;
    if(((central_or_shift_int == TriggerSFsys::shiftUp || central_or_shift_int == TriggerSFsys::shiftDown) && isAnyCompatible) ||
       (central_or_shift.find("tau") == std::string::npos && isLeptonCompatible) ||
       (central_or_shift.find("tau") != std::string::npos && isHadTauCompatible))
    {
      return central_or_shift_int;
    }
  }
  return TriggerSFsys::central;
}

int
getLHEscale_option(const std::string & central_or_shift)
{
  const auto kv = lheScaleSysMap.find(central_or_shift);
  if(kv != lheScaleSysMap.end())
  {
    return kv->second;
  }
  return kLHE_scale_central;
}

int
getPartonShower_option(const std::string & central_or_shift)
{
  const auto kv = psSysMap.find(central_or_shift);
  if(kv != psSysMap.end())
  {
    return kv->second;
  }
  return kPartonShower_central;
}

ElectronPtSys
getElectronPt_option(const std::string & central_or_shift,
                     bool isMC)
{
  if(isMC)
  {
    const auto kv = ePtSysMap.find(central_or_shift);
    if(kv != ePtSysMap.end())
    {
      return kv->second;
    }
  }
  return ElectronPtSys::central;
}

MuonPtSys
getMuon_option(const std::string & central_or_shift,
               bool isMC)
{
  if(isMC)
  {
    const auto kv = mPtSysMap.find(central_or_shift);
    if(kv != mPtSysMap.end())
    {
      return kv->second;
    }
  }
  return MuonPtSys::central;
}

int
getJetToLeptonFR_option(const std::string & central_or_shift)
{
  const auto kv = jetToLeptonFRSysMap.find(central_or_shift);
  if(kv != jetToLeptonFRSysMap.end())
  {
    return kv->second;
  }
  return kFRl_central;
}

PUsys
getPUsys_option(const std::string & central_or_shift)
{
  const auto kv = puSysMap.find(central_or_shift);
  if(kv != puSysMap.end())
  {
    return kv->second;
  }
  return PUsys::central;
}

L1PreFiringWeightSys
getL1PreFiringWeightSys_option(const std::string & central_or_shift)
{
  const auto kv = l1prefireSysMap.find(central_or_shift);
  if(kv != l1prefireSysMap.end())
  {
    return kv->second;
  }
  return L1PreFiringWeightSys::nominal;
}

int
getDYMCReweighting_option(const std::string & central_or_shift)
{
  const auto kv = dyMCRwgtSysMap.find(central_or_shift);
  if(kv != dyMCRwgtSysMap.end())
  {
    return kv->second;
  }
  return kDYMCReweighting_central;
}

int
getDYMCNormScaleFactors_option(const std::string & central_or_shift)
{
  const auto kv = dyMCNormSysMap.find(central_or_shift);
  if(kv != dyMCNormSysMap.end())
  {
    return kv->second;
  }
  return kDYMCNormScaleFactors_central;
}

int
getTopPtReweighting_option(const std::string & central_or_shift)
{
  const auto kv = topPtRwgtSysMap.find(central_or_shift);
  if(kv != topPtRwgtSysMap.end())
  {
    return kv->second;
  }
  return kTopPtReweighting_central;
}

EWKJetSys
getEWKJetSys_option(const std::string & central_or_shift)
{
  const auto kv = ewkJetSysMap.find(central_or_shift);
  if(kv != ewkJetSysMap.end())
  {
    return kv->second;
  }
  return EWKJetSys::central;
}

EWKBJetSys
getEWKBJetSys_option(const std::string & central_or_shift)
{
  const auto kv = ewkBJetSysMap.find(central_or_shift);
  if(kv != ewkBJetSysMap.end())
  {
    return kv->second;
  }
  return EWKBJetSys::central;
}

PDFSys
getPDFSys_option(const std::string & central_or_shift)
{
  const auto kv = pdfSysMap.find(central_or_shift);
  if(kv != pdfSysMap.end())
  {
    return kv->second;
  }
  return PDFSys::central;
}

LHEVptSys
getLHEVptSys_option(const std::string & central_or_shift)
{
  const auto kv = lheVptSysMap.find(central_or_shift);
  if(kv != lheVptSysMap.end())
  {
    return kv->second;
  }
  return LHEVptSys::central;
}

SubjetBtagSys
getSubjetBtagSys_option(const std::string & central_or_shift)
{
  const auto kv = subjetBtagSysMap.find(central_or_shift);
  if(kv != subjetBtagSysMap.end())
  {
    return kv->second;
  }
  return SubjetBtagSys::central;
}

bool
isPDFsys_member(const std::string & central_or_shift)
{
  const std::regex pdfSysRegex(pdfSysStr + "[[:digit:]]+");
  std::smatch pdfSysMatch;
  return std::regex_match(central_or_shift, pdfSysMatch, pdfSysRegex);
}

std::string
getPDFsys_str(int memberIdx)
{
  assert(memberIdx >= 0);
  return pdfSysStr + std::to_string(memberIdx);
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
       ! isPDFsys_member(central_or_shift))
    {
      throw cmsException(__func__, __LINE__) << "Non-central MC systematics option not ending with Up or Down: " << central_or_shift;
    }
  }
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
  const std::string era_str = get_era(era);
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
  const std::string era_str = get_era(era);
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
