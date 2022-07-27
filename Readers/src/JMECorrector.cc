#include "TallinnNtupleProducer/Readers/interface/JMECorrector.h"

#include "TallinnNtupleProducer/CommonTools/interface/Era.h"              // get_era()
#include "TallinnNtupleProducer/CommonTools/interface/sysUncertOptions.h" // kJetMET_*
#include "TallinnNtupleProducer/CommonTools/interface/LocalFileInPath.h"  // LocalFileInPath
#include "TallinnNtupleProducer/CommonTools/interface/cmsException.h"     // cmsException()
#include "TallinnNtupleProducer/Objects/interface/EventInfo.h"            // EventInfo
#include "TallinnNtupleProducer/Readers/interface/metPhiModulation.h"     // METXYCorr_Met_MetPhi

#include "DataFormats/Math/interface/deltaR.h"                            // deltaR()

#include <TString.h>                                                      // Form()

#include <boost/algorithm/string/predicate.hpp>                           // boost::ends_with()
#include <boost/algorithm/string/replace.hpp>                             // boost::replace_all_copy()

#define _USE_MATH_DEFINES // M_PI

// TODO verify that it is acceptable to use the gen jet associations that are already present in NanoAOD

namespace
{
  double square(double x)
  {
    return x*x;
  }
}

JMECorrector::JetParams::JetParams(const RecoJetAK4 & jet)
  : pt(jet.pt())
  , eta(jet.eta())
  , phi(jet.phi())
  , mass(jet.mass())
  , area(jet.area())
  , rawFactor(jet.rawFactor())
{}

JMECorrector::JMECorrector(const edm::ParameterSet & cfg)
  : isDEBUG_(cfg.getParameter<bool>("isDEBUG"))
  , isMC_(cfg.getParameter<bool>("isMC"))
  , reapply_JEC_(cfg.getParameter<bool>("reapply_JEC"))
  , apply_smearing_(cfg.getParameter<bool>("apply_smearing"))
  , era_(get_era(cfg.getParameter<std::string>("era")))
  , era_str_(get_era(era_))
  , ak4_jetType_("AK4PFchs")
  , ak8_jetType_("AK8PFPuppi")
  , jet_sys_(kJetMET_central)
  , met_sys_(kJetMET_central)
  , fatJet_sys_(kFatJet_central)
  , enable_phiModulationCorr_(cfg.getParameter<bool>("enable_phiModulationCorr"))
  , rho_(0.)
  , rle_(0)
  , info_(nullptr)
  , generator_(0)
  , use_deterministic_seed_(true)
  , jet_cset_(nullptr)
  , jet_reso_(nullptr)
  , jet_jer_sf_(nullptr)
  , fatJet_cset_(nullptr)
  , jmar_cset_(nullptr)
{
  // TODO read the following options from cfg: disable_ak8_corr
  switch(era_)
  {
    case Era::k2016:      globalJECTag_ = "Summer16_07Aug2017_V11"; globalJERTag_ = "Summer16_25nsV1"; break;
    case Era::k2017:      globalJECTag_ = "Fall17_17Nov2017_V32";   globalJERTag_ = "Fall17_V3b";      break;
    case Era::k2018:      globalJECTag_ = "Autumn18_V19";           globalJERTag_ = "Autumn18_V7";     break;
    case Era::kUndefined: __attribute__((fallthrough));
    default:              assert(0);
  }
  assert(! globalJECTag_.empty());
  assert(! globalJERTag_.empty());

  const std::string jetCorrectionSetFile = LocalFileInPath(Form(
    "TallinnNtupleProducer/EvtWeightTools/data/correctionlib/jme/%s/jet_jerc.json.gz", era_str_.data()
  )).fullPath();
  jet_cset_ = correction::CorrectionSet::from_file(jetCorrectionSetFile);

  // we could benefit from compound corrections here, but for now let's stack them manually
  const std::string source = isMC_ ? "MC" : "DATA";
  jet_compound_ = {
    jet_cset_->at(Form("%s_%s_L1FastJet_%s",    globalJECTag_.data(), source.data(), ak4_jetType_.data())),
    jet_cset_->at(Form("%s_%s_L2Relative_%s",   globalJECTag_.data(), source.data(), ak4_jetType_.data())),
    jet_cset_->at(Form("%s_%s_L3Absolute_%s",   globalJECTag_.data(), source.data(), ak4_jetType_.data())),
    jet_cset_->at(Form("%s_%s_L2L3Residual_%s", globalJECTag_.data(), source.data(), ak4_jetType_.data())),
  };
  if(isMC_)
  {
    jet_uncs_[kJetMET_jesUp] = jet_cset_->at(Form("%s_MC_Total_%s", globalJECTag_.data(), ak4_jetType_.data()));
    for(const auto & kv: jesSplitAK4SysMap)
    {
      if(kv.second % 2 == 0 || kv.second >= kJetMET_jesHEMDown)
      {
        continue;
      }
      assert(boost::ends_with(kv.first, "Up"));
      const std::string sys_opt = boost::replace_all_copy(kv.first.substr(12, kv.first.size() - 14), "Era", era_str_);
      jet_uncs_[kv.second] = jet_cset_->at(Form("%s_MC_%s_%s", globalJECTag_.data(), sys_opt.data(), ak4_jetType_.data()));
    }

    jet_reso_ = jet_cset_->at(Form("%s_MC_PtResolution_%s", globalJERTag_.data(), ak4_jetType_.data()));
    jet_jer_sf_ = jet_cset_->at(Form("%s_MC_ScaleFactor_%s", globalJERTag_.data(), ak4_jetType_.data()));
  }
  else
  {
    if(apply_smearing_)
    {
      throw cmsException(this, __func__, __LINE__) << "Cannot smear jets in data";
    }
  }
}

JMECorrector::~JMECorrector()
{}

void
JMECorrector::set_info(const EventInfo * info)
{
  info_ = info;
  rho_ = info_->rho();
  rle_ = (info_->run() << 20) + (info_->lumi() << 10) + info_->event();

  delta_x_T1Jet_.clear();
  delta_y_T1Jet_.clear();
  delta_x_rawJet_.clear();
  delta_y_rawJet_.clear();
}

void
JMECorrector::set_jet_opt(int central_or_shift)
{
  jet_sys_ = central_or_shift;
}

void
JMECorrector::set_met_opt(int central_or_shift)
{
  met_sys_ = central_or_shift;
}

void
JMECorrector::set_fatJet_opt(int central_or_shift)
{
  fatJet_sys_ = central_or_shift;
}

void
JMECorrector::correct(RecoJetAK4 & jet,
                      const std::vector<const GenJet *> & genJets)
{
  // raw jet pT and mass
  const double raw = 1 - jet.rawFactor();
  const double jet_rawpt = jet.pt() * raw;
  const double jet_rawmass = jet.mass() * raw;

  // nominal jet pT and mass without smearing
  double jet_pt = jet.pt();
  double jet_mass = jet.mass();
  if(reapply_JEC_)
  {
    const auto jet_calibrated = calibrate(jet, true, 4);
    jet_pt = jet_calibrated.first;
    jet_mass = jet_calibrated.second;
  }
  const double jec = jet_pt / jet_rawpt;

  // jet pT and mass with only L1 corrections applied
  const auto jet_calibrated_l1 = calibrate(jet, false, 1);
  const double & jet_pt_l1 = jet_calibrated_l1.first;
  const double & jet_mass_l1 = jet_calibrated_l1.second;
  const double jecL1 = jet_pt_l1 / jet_rawpt;

  // muon-subtracted jet pT
  const double muon_pt = jet_rawpt * jet.muonSubtrFactor();
  const double jet_rawpt_noMu = jet_rawpt * (1 - jet.muonSubtrFactor());
  const double jet_pt_noMuL1L2L3 = jet_rawpt_noMu * jec;
  const double jet_pt_noMuL1 = jet_rawpt_noMu * jecL1;

  // smear the jets
  double jer = 1.;
  if(apply_smearing_)
  {
    const Particle::LorentzVector jet_p4 { jet_pt, jet.eta(), jet.phi(), jet_mass };

    const int genJetIdx = jet.genJetIdx();
    Particle::LorentzVector genJet_p4 {0., 0., 0., 0.};
    if(genJetIdx >= 0 && genJetIdx < static_cast<int>(genJets.size()))
    {
      genJet_p4 = genJets.at(genJetIdx)->p4();
    }

    jer = smear(jet_p4, genJet_p4);
  }
  const double jet_pt_nom = jet_pt * jer;
  const double jet_mass_nom = jer * jet_mass;

  // JEC uncertainties
  const double delta = jec_unc(jet_pt_nom, jet.eta(), jet.phi(), jet.jetId());
  const double jet_pt_shifted = jet_pt_nom * (1 + delta);
  const double jet_mass_shifted = jet_mass_nom * (1 + delta);

  // record the delta for removing (L1L2L3 - L1) corrected jets from the EE region
  const double jet_pt_L1L2L3 = jet_pt_noMuL1L2L3 + muon_pt;
  const double jet_pt_L1 = jet_pt_noMuL1 + muon_pt;
  const double jet_absEta = std::fabs(jet.eta());
  if(era_ == Era::k2017 && jet_pt_L1L2L3 > 15. && jet_absEta > 2.65 && jet_absEta < 3.14 && jet_rawpt < 50.)
  {
    const double jet_phi = jet.phi();
    const double cos_jet_phi = std::cos(jet_phi);
    const double sin_jet_phi = std::sin(jet_phi);
    delta_x_T1Jet_.push_back((jet_pt_L1L2L3 - jet_pt_L1) * cos_jet_phi + jet_rawpt * cos_jet_phi);
    delta_y_T1Jet_.push_back((jet_pt_L1L2L3 - jet_pt_L1) * sin_jet_phi + jet_rawpt * sin_jet_phi);
    delta_x_rawJet_.push_back(jet_rawpt * cos_jet_phi);
    delta_y_rawJet_.push_back(jet_rawpt * sin_jet_phi);
  }

  jet.set_ptEtaPhiMass(jet_pt_shifted, jet.eta(), jet.phi(), jet_mass_shifted);
}

void
JMECorrector::correct(const CorrT1METJet & jet,
                      const std::vector<const GenJet *> & genJets)
{
  //
}

void
JMECorrector::correct(RecoJetAK8 & jet,
                      const std::vector<const Particle *> & genJetsAK8,
                      const std::vector<const Particle *> & genSubJetsAK8) const
{
  //
}

void
JMECorrector::correct(RecoMEt & met,
                      const GenMEt & rawmet,
                      const RecoVertex * const recoVertex) const
{
  if(enable_phiModulationCorr_)
  {
    double met_pt = met.pt();
    double met_phi = met.phi();

    const std::pair<double, double> met_pxpyCorr = METXYCorr_Met_MetPhi(info_, recoVertex, era_);
    const double met_px = met_pt * std::cos(met_phi) + met_pxpyCorr.first;
    const double met_py = met_pt * std::sin(met_phi) + met_pxpyCorr.second;

    met_pt = std::sqrt(square(met_px) + square(met_py));
    if     (met_px > 0) { met_phi = std::atan(met_py / met_px); }
    else if(met_px < 0) { met_phi = std::atan(met_py / met_px) + ((met_py > 0. ? +1. : -1.) * M_PI);  }
    else                { met_phi = (met_py > 0. ? +1. : -1.) * M_PI; }

    met.set(met_pt, met_phi);
  }
}

std::pair<double, double>
JMECorrector::calibrate(const JetParams & jetParams,
                        bool include_residual,
                        int max_level) const
{
  const double raw = 1 - jetParams.rawFactor;
  double corr = 1.;
  for(int level = 0; level < max_level; ++level)
  {
    corr *= level ?
      jet_compound_.at(level)->evaluate({ jetParams.eta, jetParams.pt * raw }) :
      jet_compound_.at(level)->evaluate({ jetParams.area, jetParams.eta, jetParams.pt * raw, rho_ })
    ;
  }
  const double raw_corr = corr <= 0. ? 1. : raw * corr;
  const double newpt = jetParams.pt * raw_corr;
  const double newmass = jetParams.mass * raw_corr;
  return std::make_pair(newpt, newmass);
}

double
JMECorrector::jec_unc(double jet_pt,
                      double jet_eta,
                      double jet_phi,
                      int jet_id) const
{
  double delta = 0.;
  if(jet_sys_ != kJetMET_central)
  {
    assert(isMC_);
    if(jet_sys_ < kJetMET_jesHEMDown)
    {
      const bool is_up = jet_sys_ % 2 == 1;
      const int key = is_up ? jet_sys_ : (jet_sys_ - 1);
      delta = jet_uncs_.at(key)->evaluate({ jet_eta, jet_pt });
      if(! is_up)
      {
        delta *= -1;
      }
    }
    else if(jet_sys_ == kJetMET_jesHEMDown)
    {
      if(era_ != Era::k2018)
      {
        throw cmsException(this, __func__, __LINE__)
          << "Invalid combination of JES uncertainty and era: " << jet_sys_ << " and " << era_str_
        ;
      }
      if(jet_pt > 15. && jet_id & 2 && jet_phi > -1.57 && jet_phi < -0.87)
      {
        // https://hypernews.cern.ch/HyperNews/CMS/get/JetMET/2000.html
        if(jet_eta > -2.5 && jet_eta < -1.3)
        {
          delta = -0.2;
        }
        else if(jet_eta <= 2.5 && jet_eta > -3)
        {
          delta = -0.35;
        }
      }
    }
  }
  return delta;
}

double
JMECorrector::smear(const Particle::LorentzVector & jet,
                    const Particle::LorentzVector & genJet)
{
  std::string sf_sys = "nom";
  if(jet_sys_ >= kJetMET_jerUp && jet_sys_ <= kJetMET_jerForwardHighPtDown)
  {
    const bool is_barrel = std::fabs(jet.Eta()) < 1.93;
    const bool is_endcap1 = ! is_barrel && std::fabs(jet.Eta()) < 2.5;
    const bool is_endcap2 = ! is_barrel && ! is_endcap1 && std::fabs(jet.Eta()) < 3;
    const bool is_forward = ! is_barrel && ! is_endcap1 && ! is_endcap2;
    const bool is_lowpt = jet.Pt() < 50.;

    if     (jet_sys_ == kJetMET_jerUp                                           ||
           (jet_sys_ == kJetMET_jerBarrelUp        && is_barrel               ) ||
           (jet_sys_ == kJetMET_jerEndcap1Up       && is_endcap1              ) ||
           (jet_sys_ == kJetMET_jerEndcap2LowPtUp  && is_endcap2 &&   is_lowpt) ||
           (jet_sys_ == kJetMET_jerEndcap2HighPtUp && is_endcap2 && ! is_lowpt) ||
           (jet_sys_ == kJetMET_jerForwardLowPtUp  && is_forward &&   is_lowpt) ||
           (jet_sys_ == kJetMET_jerForwardHighPtUp && is_forward && ! is_lowpt)  )
    {
      sf_sys = "up";
    }
    else if(jet_sys_ == kJetMET_jerDown                                           ||
           (jet_sys_ == kJetMET_jerBarrelDown        && is_barrel               ) ||
           (jet_sys_ == kJetMET_jerEndcap1Down       && is_endcap1              ) ||
           (jet_sys_ == kJetMET_jerEndcap2LowPtDown  && is_endcap2 &&   is_lowpt) ||
           (jet_sys_ == kJetMET_jerEndcap2HighPtDown && is_endcap2 && ! is_lowpt) ||
           (jet_sys_ == kJetMET_jerForwardLowPtDown  && is_forward &&   is_lowpt) ||
           (jet_sys_ == kJetMET_jerForwardHighPtDown && is_forward && ! is_lowpt)  )
    {
      sf_sys = "down";
    }
  }
  const double jer_sf =
    era_ == Era::k2018 ?
    jet_jer_sf_->evaluate({ jet.Eta(), jet.Pt(), sf_sys }) :
    jet_jer_sf_->evaluate({ jet.Eta(), sf_sys })
  ;

  const double reso = jet_reso_->evaluate({ jet.Eta(), jet.Pt(), rho_ });
  const double dPt = (jet.Pt() - genJet.Pt()) / jet.Pt();
  const bool has_genJet = genJet.Pt() > 0.;

  double smearFactor = 1.;
  if(has_genJet && std::fabs(dPt) < 3 * reso && ::deltaR(jet.Eta(), jet.Phi(), genJet.Eta(), genJet.Phi()) < 0.2)
  {
    // additional constraints on the selected gen jet are documented in:
    // https://twiki.cern.ch/twiki/bin/view/CMS/JetResolution
    smearFactor += (jer_sf - 1) * dPt;
  }
  else
  {
    // stochastic smearing
    if(use_deterministic_seed_)
    {
      // inspiration from CMSSW implementation:
      // https://github.com/cms-sw/cmssw/blob/1fef057ea0118be780f3ccd15eb7b1ce7c9a9dab/PhysicsTools/PatUtils/interface/SmearedJetProducerT.h#L208-L215
      const unsigned seed = static_cast<unsigned>(jet.Eta() / 0.01) + rle_;
      generator_.seed(seed);
    }
    std::normal_distribution<> gaus(0, reso);
    smearFactor += gaus(generator_) * std::sqrt(std::max(square(jer_sf) - 1, 0.));
  }
  return smearFactor > 0 ? smearFactor : 1.;
}
