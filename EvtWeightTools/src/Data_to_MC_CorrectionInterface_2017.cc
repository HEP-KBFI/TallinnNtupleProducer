#include "TallinnNtupleProducer/EvtWeightTools/interface/Data_to_MC_CorrectionInterface_2017.h"

#include "TallinnNtupleProducer/CommonTools/interface/as_integer.h"                             // as_integer()
#include "TallinnNtupleProducer/CommonTools/interface/cmsException.h"                           // cmsException
#include "TallinnNtupleProducer/CommonTools/interface/Era.h"                                    // Era
#include "TallinnNtupleProducer/EvtWeightTools/interface/data_to_MC_corrections_auxFunctions.h" // aux::compSF(), aux::loadTriggerEff_1e_2016(), aux::loadTriggerEff_1m_2016()
#include "TallinnNtupleProducer/EvtWeightTools/interface/lutAuxFunctions.h"                     // lutWrapperTH2

#include "TauPOG/TauIDSFs/interface/TauIDSFTool.h"                                              // TauIDSFTool

Data_to_MC_CorrectionInterface_2017::Data_to_MC_CorrectionInterface_2017(const edm::ParameterSet & cfg)
  : Data_to_MC_CorrectionInterface_Base(Era::k2017, cfg)
{
  // Reconstruction efficiencies of electrons with pT < 20 GeV, measured by EGamma POG
  // https://twiki.cern.ch/twiki/pub/CMS/Egamma2017DataRecommendations/egammaEffi.txt_EGM2D_runBCDEF_passingRECO_lowEt.root
  // Downloaded from: https://twiki.cern.ch/twiki/bin/view/CMS/EgammaIDRecipesRun2?rev=104#94X_series_Fall17V2_Scale_factor
  sfElectronID_and_Iso_loose_.push_back(new lutWrapperTH2(
    inputFiles_,
    "TallinnNtupleProducer/EvtWeightTools/data/leptonSF/2017/el_scaleFactors_gsf_ptLt20.root",
    "EGamma_SF2D",
    lut::kXetaYpt, -2.5, +2.5, lut::kLimit, -1., 20., lut::kLimit_and_Cut
  ));
  // Reconstruction efficiencies of electrons with pT > 20 GeV, measured by EGamma POG
  // https://twiki.cern.ch/twiki/pub/CMS/Egamma2017DataRecommendations/egammaEffi.txt_EGM2D_runBCDEF_passingRECO.root
  // Downloaded from: https://twiki.cern.ch/twiki/bin/view/CMS/EgammaIDRecipesRun2?rev=104#94X_series_Fall17V2_Scale_factor
  sfElectronID_and_Iso_loose_.push_back(new lutWrapperTH2(
    inputFiles_,
    "TallinnNtupleProducer/EvtWeightTools/data/leptonSF/2017/el_scaleFactors_gsf_ptGt20.root",
    "EGamma_SF2D",
    lut::kXetaYpt, -2.5, +2.5, lut::kLimit, 20., -1., lut::kLimit_and_Cut
  ));
  // Loose electron SFs, measured by the CERN group
  // /afs/cern.ch/user/b/balvarez/work/public/ttHAnalysis/TnP_loose_ele_2017.root
  sfElectronID_and_Iso_loose_.push_back(new lutWrapperTH2(
    inputFiles_,
    "TallinnNtupleProducer/EvtWeightTools/data/leptonSF/2017/TnP_loose_ele_2017.root",
    "EGamma_SF2D",
    lut::kXabsEtaYpt
  ));
  // /afs/cern.ch/user/b/balvarez/work/public/ttHAnalysis/TnP_loosettH_ele_2017.root
  sfElectronID_and_Iso_loose_.push_back(new lutWrapperTH2(
    inputFiles_,
    "TallinnNtupleProducer/EvtWeightTools/data/leptonSF/2017/TnP_loosettH_ele_2017.root",
    "EGamma_SF2D",
    lut::kXabsEtaYpt
  ));

  // Loose-to-tight electron ID SFs, measured by the CERN group
  // /afs/cern.ch/user/b/balvarez/work/public/ttHAnalysis/TnP_ttH_ele_2017_3l/passttH/egammaEffi.txt_EGM2D.root
  sfElectronID_and_Iso_tight_to_loose_woTightCharge_.push_back(new lutWrapperTH2(
    inputFiles_,
    "TallinnNtupleProducer/EvtWeightTools/data/leptonSF/2017/lepMVAEffSF_e_3l.root",
    "EGamma_SF2D",
    lut::kXabsEtaYpt
  ));
  // /afs/cern.ch/user/b/balvarez/work/public/ttHAnalysis/TnP_ttH_ele_2017_2lss/passttH/egammaEffi.txt_EGM2D.root
  sfElectronID_and_Iso_tight_to_loose_wTightCharge_.push_back(new lutWrapperTH2(
    inputFiles_,
    "TallinnNtupleProducer/EvtWeightTools/data/leptonSF/2017/lepMVAEffSF_e_2lss.root",
    "EGamma_SF2D",
    lut::kXabsEtaYpt
  ));
  // combined
  // /afs/cern.ch/user/b/balvarez/work/public/ttHAnalysis/error/SFttbar_2017_ele_pt.root
  // /afs/cern.ch/user/b/balvarez/work/public/ttHAnalysis/error/SFttbar_2017_ele_eta.root
  sfElectronID_and_Iso_tight_to_loose_errors_up_.push_back(new lutWrapperTH2(
    inputFiles_, "TallinnNtupleProducer/EvtWeightTools/data/leptonSF/2017/lepMVAEffSF_e_error.root", "histo_eff_data_max",
    lut::kXabsEtaYpt
  ));
  sfElectronID_and_Iso_tight_to_loose_errors_down_.push_back(new lutWrapperTH2(
    inputFiles_, "TallinnNtupleProducer/EvtWeightTools/data/leptonSF/2017/lepMVAEffSF_e_error.root", "histo_eff_data_min",
    lut::kXabsEtaYpt
  ));

  // Loose muon SFs, measured by the CERN group
  sfMuonID_and_Iso_loose_.push_back(new lutWrapperTH2(
    inputFiles_,
    "TallinnNtupleProducer/EvtWeightTools/data/leptonSF/2017/TnP_loose_muon_2017.root",
    "EGamma_SF2D",
    lut::kXabsEtaYpt
  ));

  // Loose-to-tight muon ID SFs, measured by the CERN group
  // /afs/cern.ch/user/b/balvarez/work/public/ttHAnalysis/TnP_ttH_muon_2017_3l/passttH/egammaEffi.txt_EGM2D.root
  sfMuonID_and_Iso_tight_to_loose_woTightCharge_.push_back(new lutWrapperTH2(
    inputFiles_,
    "TallinnNtupleProducer/EvtWeightTools/data/leptonSF/2017/lepMVAEffSF_m_3l.root",
    "EGamma_SF2D",
    lut::kXabsEtaYpt
  ));
  // /afs/cern.ch/user/b/balvarez/work/public/ttHAnalysis/TnP_ttH_muon_2017_2lss/passttH/egammaEffi.txt_EGM2D.root
  sfMuonID_and_Iso_tight_to_loose_wTightCharge_.push_back(new lutWrapperTH2(
    inputFiles_,
    "TallinnNtupleProducer/EvtWeightTools/data/leptonSF/2017/lepMVAEffSF_m_2lss.root",
    "EGamma_SF2D",
    lut::kXabsEtaYpt
  ));
  // combined
  // /afs/cern.ch/user/b/balvarez/work/public/ttHAnalysis/error/SFttbar_2017_muon_pt.root
  // /afs/cern.ch/user/b/balvarez/work/public/ttHAnalysis/error/SFttbar_2017_muon_eta.root
  sfMuonID_and_Iso_tight_to_loose_errors_up_.push_back(new lutWrapperTH2(
    inputFiles_, "TallinnNtupleProducer/EvtWeightTools/data/leptonSF/2017/lepMVAEffSF_m_error.root", "histo_eff_data_max",
    lut::kXabsEtaYpt
  ));
  sfMuonID_and_Iso_tight_to_loose_errors_down_.push_back(new lutWrapperTH2(
    inputFiles_, "TallinnNtupleProducer/EvtWeightTools/data/leptonSF/2017/lepMVAEffSF_m_error.root", "histo_eff_data_min",
    lut::kXabsEtaYpt
  ));

  aux::loadTriggerEff_1e_2017(effTrigger_1e_data_, effTrigger_1e_mc_, inputFiles_);
  aux::loadTriggerEff_1m_2017(effTrigger_1m_data_, effTrigger_1m_mc_, inputFiles_);

  if(applyHadTauSF_)
  {
    const std::string tauIDSFTool_era = "2017ReReco";
    tauIdSFs_ = new TauIDSFTool(tauIDSFTool_era, tauIDSF_str_, tauIDSF_level_str_, false);
    initAntiEle_tauIDSFs(tauIDSFTool_era);
    initAntiMu_tauIDSFs(tauIDSFTool_era);
  }
}

Data_to_MC_CorrectionInterface_2017::~Data_to_MC_CorrectionInterface_2017()
{}

double
Data_to_MC_CorrectionInterface_2017::getSF_leptonTriggerEff(TriggerSFsys central_or_shift) const
{
  if(! check_triggerSFsys_opt(central_or_shift))
  {
    throw cmsException(this, __func__, __LINE__)
      << "Invalid central_or_shift option, given the number of "
         "leptons (" << numLeptons_ << "), electrons (" << numElectrons_ << "), muons (" << numMuons_ << ") and "
         "hadronic taus (" << numHadTaus_ << "): " << as_integer(central_or_shift);
  }
  // see https://cernbox.cern.ch/index.php/s/lW2BiTli5tJR0MN
  double sf = 1.;
  double sfErr = 0.;

  if((numElectrons_ + numMuons_) == 1)
  {
    if(numElectrons_ == 1)
    {
      double eff_data = get_from_lut(effTrigger_1e_data_, lepton_pt_[0], lepton_eta_[0], isDEBUG_);
      double eff_mc   = get_from_lut(effTrigger_1e_mc_,   lepton_pt_[0], lepton_eta_[0], isDEBUG_);
      sf = aux::compSF(eff_data, eff_mc);
      sfErr = 2.;
    } 
    else if(numMuons_ == 1)
    {
      double eff_data = get_from_lut(effTrigger_1m_data_, lepton_pt_[0], lepton_eta_[0], isDEBUG_);
      double eff_mc   = get_from_lut(effTrigger_1m_mc_,   lepton_pt_[0], lepton_eta_[0], isDEBUG_);
      sf = aux::compSF(eff_data, eff_mc);
      sfErr = 2.;
    } else assert(0);
  }
  else if((numElectrons_ + numMuons_) == 2)
  {
    const double lepton_cone_pt_sublead = std::min(lepton_cone_pt_[0], lepton_cone_pt_[1]);
    if(numElectrons_ == 2 && numMuons_ == 0)
    {
      if  (lepton_cone_pt_sublead >= 40.) { sf = 1.000; }
      else                                { sf = 0.980; }
      sfErr = 1.;
    }
    else if(numElectrons_ == 1 && numMuons_ == 1)
    {
      if     (lepton_cone_pt_sublead >= 40.) { sf = 0.990; }
      else                                   { sf = 0.980; }
      sfErr = 1.;
    }
    else if(numElectrons_ == 0 && numMuons_ == 2)
    {
      if     (lepton_cone_pt_sublead >= 70.) { sf = 0.940; }
      else if(lepton_cone_pt_sublead >= 55.) { sf = 0.960; }
      else if(lepton_cone_pt_sublead >= 40.) { sf = 0.995; }
      else if(lepton_cone_pt_sublead >= 15.) { sf = 0.970; }
      sfErr = 2.;
    } else assert(0);
  }
  else if((numElectrons_ + numMuons_) >= 3)
  {
    sf = 1.;
    sfErr = 1.;
  }

  sfErr /= 100.;
  return comp_triggerSFsys_opt(sf, sfErr, central_or_shift);
}
