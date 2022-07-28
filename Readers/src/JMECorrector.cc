#include "TallinnNtupleProducer/Readers/interface/JMECorrector.h"

#include "TallinnNtupleProducer/CommonTools/interface/Era.h"              // get_era()
#include "TallinnNtupleProducer/CommonTools/interface/sysUncertOptions.h" // kJetMET_*
#include "TallinnNtupleProducer/CommonTools/interface/jetDefinitions.h"   // get_fatJet_corrections()
#include "TallinnNtupleProducer/CommonTools/interface/LocalFileInPath.h"  // LocalFileInPath
#include "TallinnNtupleProducer/CommonTools/interface/cmsException.h"     // cmsException()
#include "TallinnNtupleProducer/Objects/interface/EventInfo.h"            // EventInfo
#include "TallinnNtupleProducer/Readers/interface/metPhiModulation.h"     // METXYCorr_Met_MetPhi()

#include "DataFormats/Math/interface/deltaR.h"                            // deltaR()

#include <TString.h>                                                      // Form()

#include <boost/algorithm/string/join.hpp>                                // boost::algorithm::join()
#include <boost/algorithm/string/predicate.hpp>                           // boost::ends_with()
#include <boost/algorithm/string/replace.hpp>                             // boost::replace_all_copy()

#define _USE_MATH_DEFINES // M_PI

namespace
{
  double
  square(double x)
  {
    return x * x;
  }

  /**
   * @brief https://en.wikipedia.org/wiki/Kahan_summation_algorithm
   */
  double
  kahan_sum(const std::vector<double> & input)
  {
    double compensation = 0.;
    return std::accumulate(
      input.begin(), input.end(), 0.,
      [compensation](double sum, double element) mutable -> double
      {
        const double y = element - compensation;
        const double t = sum + y;
        compensation = (t - sum) - y;
        return t;
      }
    );
  }
}

JMECorrector::JetParams::JetParams(const RecoJetAK4 & jet)
  : pt(jet.pt())
  , eta(jet.eta())
  , area(jet.area())
  , rawFactor(jet.rawFactor())
  , algo(JetAlgo::AK4)
{}

JMECorrector::JetParams::JetParams(const RecoJetAK8 & jet)
  : pt(jet.pt())
  , eta(jet.eta())
  , area(jet.area())
  , rawFactor(jet.rawFactor())
  , algo(JetAlgo::AK8)
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
  , fatJet_corr_(0)
  , enable_phiModulationCorr_(cfg.getParameter<bool>("enable_phiModulationCorr"))
  , rho_(0.)
  , rle_(0)
  , info_(nullptr)
  , generator_(0)
  , use_deterministic_seed_(true)
  , jmar_cset_(nullptr)
  , jet_jerc_cset_(nullptr)
  , fatJet_jerc_cset_(nullptr)
{
  const std::vector<std::string> fatJet_corrections_vstring = cfg.getParameter<std::vector<std::string>>("fatJet_corrections");
  fatJet_corr_ = get_fatJet_corrections(fatJet_corrections_vstring);

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
  jet_jerc_cset_ = correction::CorrectionSet::from_file(jetCorrectionSetFile);
  const std::string fatJetCorrectionSetFile = LocalFileInPath(Form(
    "TallinnNtupleProducer/EvtWeightTools/data/correctionlib/jme/%s/fatJet_jerc.json.gz", era_str_.data()
  )).fullPath();
  fatJet_jerc_cset_ = correction::CorrectionSet::from_file(fatJetCorrectionSetFile);

  // we could benefit from compound corrections here, but for now let's stack them manually
  const std::string source = isMC_ ? "MC" : "DATA";
  for(const std::string & correction_level: { "L1FastJet", "L2Relative", "L3Absolute", "L2L3Residual" })
  {
    jec_compound_[JetAlgo::AK4].push_back(
      jet_jerc_cset_->at(boost::algorithm::join(std::vector<std::string>{ globalJECTag_, source, correction_level, ak4_jetType_ }, "_"))
    );
    jec_compound_[JetAlgo::AK8].push_back(
      fatJet_jerc_cset_->at(boost::algorithm::join(std::vector<std::string>{ globalJECTag_, source, correction_level, ak8_jetType_ }, "_"))
    );
  }
  if(isMC_)
  {
    jec_uncs_[JetAlgo::AK4][kJetMET_jesUp] = jet_jerc_cset_->at(Form("%s_MC_Total_%s", globalJECTag_.data(), ak4_jetType_.data()));
    for(const auto & kv: jesSplitAK4SysMap)
    {
      if(kv.second % 2 == 0 || kv.second >= kJetMET_jesHEMDown)
      {
        continue;
      }
      assert(boost::ends_with(kv.first, "Up"));
      // 12 = length of string "CMS_ttHl_JES"; +2 = length of string "Up"
      const std::string sys_opt = boost::replace_all_copy(kv.first.substr(12, kv.first.size() - 14), "Era", era_str_);
      jec_uncs_[JetAlgo::AK4][kv.second] = jet_jerc_cset_->at(Form("%s_MC_%s_%s", globalJECTag_.data(), sys_opt.data(), ak4_jetType_.data()));
    }
    jet_reso_[JetAlgo::AK4] = jet_jerc_cset_->at(Form("%s_MC_PtResolution_%s", globalJERTag_.data(), ak4_jetType_.data()));
    jet_jer_sf_[JetAlgo::AK4] = jet_jerc_cset_->at(Form("%s_MC_ScaleFactor_%s", globalJERTag_.data(), ak4_jetType_.data()));

    jec_uncs_[JetAlgo::AK8][kFatJet_jesUp] = fatJet_jerc_cset_->at(Form("%s_MC_Total_%s", globalJECTag_.data(), ak8_jetType_.data()));
    for(const auto & kv: jesSplitAK8SysMap)
    {
      if(kv.second % 2 == 0 || kv.second >= kFatJet_jesHEMDown)
      {
        continue;
      }
      assert(boost::ends_with(kv.first, "Up"));
      // 12 = length of string "CMS_ttHl_JES"; +2 = length of string "Up"
      const std::string sys_opt = boost::replace_all_copy(kv.first.substr(12, kv.first.size() - 14), "Era", era_str_);
      jec_uncs_[JetAlgo::AK8][kv.second] = fatJet_jerc_cset_->at(Form("%s_MC_%s_%s", globalJECTag_.data(), sys_opt.data(), ak8_jetType_.data()));
    }
    jet_reso_[JetAlgo::AK8] = fatJet_jerc_cset_->at(Form("%s_MC_PtResolution_%s", globalJERTag_.data(), ak8_jetType_.data()));
    jet_jer_sf_[JetAlgo::AK8] = fatJet_jerc_cset_->at(Form("%s_MC_ScaleFactor_%s", globalJERTag_.data(), ak8_jetType_.data()));
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
}

void
JMECorrector::reset()
{
  met_T1Smear_px_.clear();
  met_T1Smear_py_.clear();
}

void
JMECorrector::set_opt(int jet_opt,
                      int met_opt,
                      int fatJet_opt)
{
  jet_sys_ = jet_opt;
  met_sys_ = met_opt;
  fatJet_sys_ = fatJet_opt;
}

void
JMECorrector::correct(RecoJetAK4 & jet,
                      const std::vector<const GenJet *> & genJets,
                      bool force_recalibration)
{
  // Compute raw jet pT
  const double raw = 1 - jet.rawFactor();
  const double jet_rawpt = jet.pt() * raw;

  // Recompute jet pT and mass by re-applying JEC
  const double jec = (reapply_JEC_ || force_recalibration) ? calibrate(jet, true, 4) : 1.;
  const double jet_pt = jet.pt() * jec;
  const double jet_mass = jet.mass() * jec;

  // Smear the jets
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

    jer = smear(jet_p4, genJet_p4, JetAlgo::AK4);
  }
  const double jet_pt_nom = jet_pt * jer;
  const double jet_mass_nom = jet_mass * jer;

  // Propagate JEC uncertainties to the smeared jets
  const double delta = jec_unc(jet_pt_nom, jet.eta(), jet.phi(), jet.jetId(), JetAlgo::AK4);
  const double delta_shift = 1 + delta;
  const double jet_pt_shifted = jet_pt_nom * delta_shift;
  const double jet_mass_shifted = jet_mass_nom * delta_shift;

  // Re-apply JEC on the muon-subtracted jet pT, so that the difference between
  // fully-corrected and L1-corrected jet pT can be propagated to MET.
  // A brief overiview is given in:
  // https://indico.cern.ch/event/854654/contributions/3594580/attachments/1924445/3184422/MET_type1corrections_nanoAODtools.pdf
  // The choice of pT threshold in the following is explained in: https://github.com/cms-nanoAOD/nanoAOD-tools/pull/240
  const double jecL1 = calibrate(jet, false, 1);
  const double muon_pt = jet_rawpt * jet.muonSubtrFactor();
  const double jet_rawpt_noMu = jet_rawpt * (1 - jet.muonSubtrFactor());
  const double jet_pt_noMuL1L2L3 = jet_rawpt_noMu * jec;
  if(jet_pt_noMuL1L2L3 > 15. && (jet.chEmEF() + jet.neEmEF()) < 0.9)
  {
    const double jet_pt_L1L2L3 = jet_pt_noMuL1L2L3 + muon_pt;
    const double jet_pt_noMuL1 = jet_rawpt_noMu * jecL1;
    const double jet_pt_L1 = jet_pt_noMuL1 + muon_pt;

    const double jet_phi = jet.phi();
    const double jet_pt_diff = jet_pt_L1L2L3 * (jer + delta) - jet_pt_L1;
    met_T1Smear_px_.push_back(jet_pt_diff * std::cos(jet_phi));
    met_T1Smear_py_.push_back(jet_pt_diff * std::sin(jet_phi));
  }

  jet.set_ptEtaPhiMass(jet_pt_shifted, jet.eta(), jet.phi(), jet_mass_shifted);
}

void
JMECorrector::correct(const CorrT1METJet & jet,
                      const std::vector<const GenJet *> & genJets,
                      const std::vector<const RecoJetAK4 *> & recoJets)
{
  // Collect the indices of gen jets that are matched to reco jets
  std::vector<int> genJetIdxs;
  std::transform(
    recoJets.begin(), recoJets.begin(), std::back_inserter(genJetIdxs),
    [](const RecoJetAK4 * recoJet) -> int
    {
      return recoJet->genJetIdx();
    }
  );
  std::sort(genJetIdxs.begin(), genJetIdxs.end());

  int genJetIdx_best = -1;
  double dR_best = 1.e+3;
  for(std::size_t genJetIdx = 0; genJetIdx < genJets.size(); ++genJetIdx)
  {
    if(std::find(genJetIdxs.begin(), genJetIdxs.end(), genJetIdx) != genJetIdxs.end())
    {
      // Gen jet has already been matched to a RecoJetAK4
      continue;
    }
    const GenJet * genJet = genJets.at(genJetIdx);
    const double dR = genJet->deltaR(jet);
    if(dR < dR_best)
    {
      dR_best = dR;
      genJetIdx_best = genJetIdx;
    }
  }
  RecoJetAK4 jet_copy(jet, genJetIdx_best);
  return correct(jet_copy, genJets, true); // force recalibration because CorrT1METJets have only raw pT
}

void
JMECorrector::correct(RecoJetAK8 & jet,
                      const std::vector<const Particle *> & genJetsAK8,
                      const std::vector<const Particle *> & genSubJetsAK8) const
{
  // Recompute jet pT and mass by re-applying JEC
  const double jec = reapply_JEC_ ? calibrate(jet, true, 4) : 1.;
  const double jet_pt = jet.pt() * jec;
  const double jet_mass = jet.mass() * jec;

  // Smear the jets
  double jer = 1.;
  if(apply_smearing_)
  {
    const Particle::LorentzVector jet_p4 { jet_pt, jet.eta(), jet.phi(), jet_mass };

    const int genJetIdx = jet.genJetAK8Idx();
    Particle::LorentzVector genJet_p4 {0., 0., 0., 0.};
    if(genJetIdx >= 0 && genJetIdx < static_cast<int>(genJetsAK8.size()))
    {
      genJet_p4 = genJetsAK8.at(genJetIdx)->p4();
    }

    jer = smear(jet_p4, genJet_p4, JetAlgo::AK8);
  }
  const double jet_pt_nom = jet_pt * jer;
  const double jet_mass_nom = jet_mass * jer;

  // Propagate JEC uncertainties to the smeared jets
  const double delta = jec_unc(jet_pt_nom, jet.eta(), jet.phi(), jet.jetId(), JetAlgo::AK8);
  const double delta_shift = 1 + delta;
  const double jet_pt_shifted = jet_pt_nom * delta_shift;
  const double jet_mass_shifted = jet_mass_nom * delta_shift;

  jet.set_ptEtaPhiMass(jet_pt_shifted, jet.eta(), jet.phi(), jet_mass_shifted);
}

void
JMECorrector::correct(RecoMEt & met,
                      const GenMEt & rawmet,
                      const RecoVertex * const recoVertex) const
{
  const double rawmet_px = rawmet.pt() * std::cos(rawmet.phi());
  const double rawmet_py = rawmet.pt() * std::sin(rawmet.phi());
  const double dpx = ::kahan_sum(met_T1Smear_px_);
  const double dpy = ::kahan_sum(met_T1Smear_py_);
  double newmet_px = rawmet_px - dpx;
  double newmet_py = rawmet_py - dpy;

  // Shift the energy of unclustered MET
  if     (met_sys_ == kJetMET_UnclusteredEnUp)
  {
    newmet_px += met.unclEnDeltaX();
    newmet_py += met.unclEnDeltaY();
  }
  else if(met_sys_ == kJetMET_UnclusteredEnDown)
  {
    newmet_px -= met.unclEnDeltaX();
    newmet_py -= met.unclEnDeltaY();
  }

  if(enable_phiModulationCorr_)
  {
    const std::pair<double, double> met_pxpyCorr = METXYCorr_Met_MetPhi(info_, recoVertex, era_);
    newmet_px += met_pxpyCorr.first;
    newmet_py += met_pxpyCorr.second;
  }

  const double newmet_pt = std::sqrt(::square(newmet_px) + ::square(newmet_py));
  double newmet_phi = 0.;
  if     (newmet_px > 0) { newmet_phi = std::atan(newmet_py / newmet_px); }
  else if(newmet_px < 0) { newmet_phi = std::atan(newmet_py / newmet_px) + ((newmet_py > 0. ? +1. : -1.) * M_PI);  }
  else                   { newmet_phi = (newmet_py > 0. ? +1. : -1.) * M_PI; }

  met.set(newmet_pt, newmet_phi);
}

double
JMECorrector::calibrate(const JetParams & jetParams,
                        bool include_residual,
                        int max_level) const
{
  const std::vector<correction::Correction::Ref> & compound_corrections = jec_compound_.at(jetParams.algo);
  const double raw = 1 - jetParams.rawFactor;
  double corr = 1.;
  for(int level = 0; level < max_level; ++level)
  {
    corr *= level ?
      compound_corrections.at(level)->evaluate({ jetParams.eta, jetParams.pt * raw }) :
      compound_corrections.at(level)->evaluate({ jetParams.area, jetParams.eta, jetParams.pt * raw, rho_ })
    ;
  }
  return corr <= 0. ? 1. : corr * raw;
}

double
JMECorrector::jec_unc(double jet_pt,
                      double jet_eta,
                      double jet_phi,
                      int jet_id,
                      JetAlgo jet_algo) const
{
  double delta = 0.;
  if((jet_sys_    != kJetMET_central && jet_algo == JetAlgo::AK4) ||
     (fatJet_sys_ != kFatJet_central && jet_algo == JetAlgo::AK8)  )
  {
    assert(isMC_);
    if((jet_sys_    < kJetMET_jesHEMDown && jet_algo == JetAlgo::AK4) ||
       (fatJet_sys_ < kFatJet_jesHEMDown && jet_algo == JetAlgo::AK8)  )
    {
      const int sys_choice = jet_algo == JetAlgo::AK4 ? jet_sys_ : fatJet_sys_;
      const bool is_up = sys_choice % 2 == 1;
      const int key = is_up ? sys_choice : (sys_choice - 1);
      delta = jec_uncs_.at(jet_algo).at(key)->evaluate({ jet_eta, jet_pt });
      if(! is_up)
      {
        delta *= -1;
      }
    }
    else if((jet_sys_    == kJetMET_jesHEMDown && jet_algo == JetAlgo::AK4) ||
            (fatJet_sys_ == kFatJet_jesHEMDown && jet_algo == JetAlgo::AK8)  )
    {
      if(era_ != Era::k2018)
      {
        throw cmsException(this, __func__, __LINE__)
          << "Invalid combination of JES uncertainty and era: " << jet_sys_ << " / " << fatJet_sys_ << " and " << era_str_
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
                    const Particle::LorentzVector & genJet,
                    JetAlgo jet_algo) const
{
  std::string sf_sys = "nom";
  if((jet_sys_    >= kJetMET_jerUp && jet_sys_    <= kJetMET_jerForwardHighPtDown && jet_algo == JetAlgo::AK4) ||
     (fatJet_sys_ >= kFatJet_jerUp && fatJet_sys_ <= kFatJet_jerDown              && jet_algo == JetAlgo::AK8)  )
  {
    const bool is_barrel = std::fabs(jet.Eta()) < 1.93;
    const bool is_endcap1 = ! is_barrel && std::fabs(jet.Eta()) < 2.5;
    const bool is_endcap2 = ! is_barrel && ! is_endcap1 && std::fabs(jet.Eta()) < 3;
    const bool is_forward = ! is_barrel && ! is_endcap1 && ! is_endcap2;
    const bool is_lowpt = jet.Pt() < 50.;

    if     (
             (
               jet_algo == JetAlgo::AK4 &&
               (
                  jet_sys_ == kJetMET_jerUp                                           ||
                 (jet_sys_ == kJetMET_jerBarrelUp        && is_barrel               ) ||
                 (jet_sys_ == kJetMET_jerEndcap1Up       && is_endcap1              ) ||
                 (jet_sys_ == kJetMET_jerEndcap2LowPtUp  && is_endcap2 &&   is_lowpt) ||
                 (jet_sys_ == kJetMET_jerEndcap2HighPtUp && is_endcap2 && ! is_lowpt) ||
                 (jet_sys_ == kJetMET_jerForwardLowPtUp  && is_forward &&   is_lowpt) ||
                 (jet_sys_ == kJetMET_jerForwardHighPtUp && is_forward && ! is_lowpt)
               )
             ) || (
               jet_algo == JetAlgo::AK8 &&
               (
                 fatJet_sys_ == kFatJet_jerUp
               )
             )
           )
    {
      sf_sys = "up";
    }
    else if(
             (
               jet_algo == JetAlgo::AK4 &&
               (
                 jet_sys_ == kJetMET_jerDown                                            ||
                 (jet_sys_ == kJetMET_jerBarrelDown        && is_barrel               ) ||
                 (jet_sys_ == kJetMET_jerEndcap1Down       && is_endcap1              ) ||
                 (jet_sys_ == kJetMET_jerEndcap2LowPtDown  && is_endcap2 &&   is_lowpt) ||
                 (jet_sys_ == kJetMET_jerEndcap2HighPtDown && is_endcap2 && ! is_lowpt) ||
                 (jet_sys_ == kJetMET_jerForwardLowPtDown  && is_forward &&   is_lowpt) ||
                 (jet_sys_ == kJetMET_jerForwardHighPtDown && is_forward && ! is_lowpt)
               )
             ) || (
               jet_algo == JetAlgo::AK8 &&
               (
                 fatJet_sys_ == kFatJet_jerDown
               )
             )
           )
    {
      sf_sys = "down";
    }
  }
  const correction::Correction::Ref & sf = jet_jer_sf_.at(jet_algo);
  const double jer_sf =
    era_ == Era::k2018 ?
    sf->evaluate({ jet.Eta(), jet.Pt(), sf_sys }) :
    sf->evaluate({ jet.Eta(), sf_sys })
  ;

  const double sigma = jet_reso_.at(jet_algo)->evaluate({ jet.Eta(), jet.Pt(), rho_ });
  const double dPt = (jet.Pt() - genJet.Pt()) / jet.Pt();
  const bool has_genJet = genJet.Pt() > 0.;

  double smearFactor = 1.;
  if(has_genJet && std::fabs(dPt) < 3 * sigma && ::deltaR(jet.Eta(), jet.Phi(), genJet.Eta(), genJet.Phi()) < 0.2)
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
    std::normal_distribution<> gaus(0, sigma);
    smearFactor += gaus(generator_) * std::sqrt(std::max(::square(jer_sf) - 1, 0.));
  }
  return smearFactor > 0 ? smearFactor : 1.;
}
