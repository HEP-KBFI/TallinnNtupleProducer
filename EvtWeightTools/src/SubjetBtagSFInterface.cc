#include "TallinnNtupleProducer/EvtWeightTools/interface/SubjetBtagSFInterface.h"

#include "CondFormats/BTauObjects/interface/BTagCalibration.h"                                  // BTagCalibration
#include "CondTools/BTau/interface/BTagCalibrationReader.h"                                     // BTagCalibrationReader
#include "DataFormats/Math/interface/deltaR.h"                                                  // deltaR()

#include "TallinnNtupleProducer/CommonTools/interface/as_integer.h"                             // as_integer()
#include "TallinnNtupleProducer/CommonTools/interface/cmsException.h"                           // cmsException()
#include "TallinnNtupleProducer/CommonTools/interface/Era.h"                                    // Era, get_era()
#include "TallinnNtupleProducer/CommonTools/interface/isHigherPt.h"                             // isHigherPt()
#include "TallinnNtupleProducer/CommonTools/interface/jetDefinitions.h"                         // Btag, BtagWP, get_BtagWP()
#include "TallinnNtupleProducer/CommonTools/interface/LocalFileInPath.h"                        // LocalFileInPath
#include "TallinnNtupleProducer/CommonTools/interface/sysUncertOptions.h"                       // SubjetBtagSys::
#include "TallinnNtupleProducer/EvtWeightTools/interface/data_to_MC_corrections_auxFunctions.h" // aux::compSF()
#include "TallinnNtupleProducer/EvtWeightTools/interface/lutAuxFunctions.h"                     // lutWrapperTH2
#include "TallinnNtupleProducer/Objects/interface/RecoJetAK8.h"                                 // RecoJetAK8
#include "TallinnNtupleProducer/Objects/interface/RecoSubjetAK8.h"                              // RecoSubjetAK8

#include <TString.h>                                                                            // Form()

inline constexpr unsigned char
operator "" _size(unsigned long long arg) noexcept
{
  return static_cast<std::size_t>(arg);
}

std::map<int, BTagEntry::JetFlavor> SubjetBtagSFInterface::flavorMap_ = {
  { 0, BTagEntry::JetFlavor::FLAV_UDSG },
  { 4, BTagEntry::JetFlavor::FLAV_C    },
  { 5, BTagEntry::JetFlavor::FLAV_B    },
};

SubjetBtagSFInterface::SubjetBtagSFInterface(Era era,
                                             const std::string & procName,
                                             BTagEntry::OperatingPoint wp)
  : btag_wp_(-1.)
  , calibration_(nullptr)
  , reader_(new BTagCalibrationReader(wp, "central", { "up", "down" }))
{
  char wp_chr = '\0';
  switch(wp)
  {
    case BTagEntry::OperatingPoint::OP_LOOSE:     wp_chr = 'l'; btag_wp_ = get_BtagWP(era, Btag::kDeepCSV, BtagWP::kLoose);  break;
    case BTagEntry::OperatingPoint::OP_MEDIUM:    wp_chr = 'm'; btag_wp_ = get_BtagWP(era, Btag::kDeepCSV, BtagWP::kMedium); break;
    case BTagEntry::OperatingPoint::OP_TIGHT:     __attribute__((fallthrough));
    case BTagEntry::OperatingPoint::OP_RESHAPING: __attribute__((fallthrough));
    default: throw cmsException(this, __func__, __LINE__) << "Invalid operating point: " << wp;
  }
  assert(wp_chr);
  assert(btag_wp_ > 0.);

  const std::string csvBaseName = [&era, this]() -> std::string {
    switch(era)
    {
      case Era::k2016:      return "subjet_DeepCSV_2016LegacySF_V1.csv";
      case Era::k2017:      return "subjet_DeepCSV_94XSF_V4_B_F_v2.csv";
      case Era::k2018:      return "subjet_DeepCSV_102XSF_V1.csv";
      case Era::kUndefined: __attribute__((fallthrough));
      default: throw cmsException(this, __func__, __LINE__) << "Invalid era: " << as_integer(era);
    }
    assert(0);
  }();
  const std::string csvFullPath = LocalFileInPath(Form("PhysicsTools/NanoAODTools/data/btagSF/%s", csvBaseName.data())).fullPath();
  calibration_ = new BTagCalibration("deepcsv", csvFullPath, true);

  for(const auto & kv: flavorMap_)
  {
    effMaps_[kv.first] = new lutWrapperTH2(
      inputFiles_,
      Form("TallinnNtupleProducer/EvtWeightTools/data/subjetBtagEff/subjetCount_%s_ratio.root", get_era(era).data()),
      Form("coarse_%i_%c_ratio_%s", kv.first, wp_chr, procName.data()),
      lut::kXptYabsEta, 0., -1., lut::kLimit, 0., +2.5, lut::kLimit_and_Cut
    );
    reader_->load(*calibration_, kv.second, kv.first > 0 ? "lt" : "incl");
  }
}

SubjetBtagSFInterface::~SubjetBtagSFInterface()
{
  delete reader_;
  delete calibration_;
  for(auto & kv: effMaps_)
  {
    delete kv.second;
  }
}

void
SubjetBtagSFInterface::addGenParticles(const std::vector<GenParticle> & genParticles)
{
  for(const GenParticle & genParticle: genParticles)
  {
    if(! (0 < std::abs(genParticle.pdgId()) && (std::abs(genParticle.pdgId()) < 6)))
    {
      continue; // not a quark
    }
    if(std::any_of(
         genParticles_.cbegin(), genParticles_.cend(),
         [&genParticle](const GenParticle & genParticle_) -> bool
         {
           return deltaR(genParticle.p4(), genParticle_.p4()) < 1e-2;
         }
      ))
    {
      // already in the list of gen particles
      continue;
    }
    genParticles_.push_back(genParticle);
  }
}

void
SubjetBtagSFInterface::addSubjets(const RecoJetAK8 * const fatJet)
{
  if(fatJet)
  {
    if(fatJet->subJet1() && fatJet->subJet1()->is_btaggable())
    {
      subjets_.push_back(fatJet->subJet1());
    }
    if(fatJet->subJet2() && fatJet->subJet2()->is_btaggable())
    {
      subjets_.push_back(fatJet->subJet2());
    }
  }
}

void
SubjetBtagSFInterface::get_flavors()
{
  assert(flavors_.empty());
  std::sort(
    genParticles_.begin(), genParticles_.end(),
    [](const GenParticle & lhs, const GenParticle & rhs) -> bool
    {
      return lhs.pt() > rhs.pt();
    }
  );
  std::sort(subjets_.begin(), subjets_.end(), isHigherPt<RecoSubjetAK8>);
  if(subjets_.size() > 2_size)
  {
    // assume that we're trying to b-tag subjets of only one AK8 jet
    throw cmsException(this, __func__, __LINE__) << "Unexpected number of subjets found: " << subjets_.size();
  }

  std::vector<std::size_t> genPartMatches;
  for(const RecoSubjetAK8 * subjet: subjets_)
  {
    double min_dr = 1e3;
    int match_idx = -1;
    for(std::size_t genPartIdx = 0; genPartIdx < genParticles_.size(); ++genPartIdx)
    {
      if(std::find(genPartMatches.cbegin(), genPartMatches.cend(), genPartIdx) != genPartMatches.cend())
      {
        continue; // gen particle already been matched
      }
      const GenParticle & genPart = genParticles_.at(genPartIdx);
      const double dr = deltaR(subjet->p4(), genPart.p4());
      if(dr < 0.4 && dr < min_dr)
      {
        min_dr = dr;
        match_idx = genPartIdx;
      }
    }
    int flavor = 0;
    if(match_idx >= 0)
    {
      genPartMatches.push_back(match_idx);
      flavor = std::abs(genParticles_.at(match_idx).pdgId());
    }
    if(flavor < 4)
    {
      flavor = 0;
    }
    flavors_.push_back(flavor);
  }
}

double
SubjetBtagSFInterface::get_sf(SubjetBtagSys option)
{
  if(flavors_.size() != subjets_.size())
  {
    get_flavors();
  }
  assert(flavors_.size() == subjets_.size());

  double prob_data = 1.;
  double prob_mc = 1.;
  for(std::size_t subjet_idx = 0; subjet_idx < subjets_.size(); ++subjet_idx)
  {
    const int flavor = flavors_.at(subjet_idx);
    const RecoSubjetAK8 * const subjet = subjets_.at(subjet_idx);
    const double eff = effMaps_.at(flavor)->getSF(subjet->pt(), subjet->eta());
    const double sf = [this, option, flavor, &subjet]() -> double
    {
      switch(option)
      {
        case SubjetBtagSys::central: return reader_->eval_auto_bounds("central", flavorMap_.at(flavor), subjet->eta(), subjet->pt());
        case SubjetBtagSys::up:      return reader_->eval_auto_bounds("up",      flavorMap_.at(flavor), subjet->eta(), subjet->pt());
        case SubjetBtagSys::down:    return reader_->eval_auto_bounds("down",    flavorMap_.at(flavor), subjet->eta(), subjet->pt());
        default: assert(0);
      }
      assert(0);
    }();
    const bool is_btagged = subjet->BtagCSV() >= btag_wp_;
    if(is_btagged)
    {
      prob_data *= sf * eff;
      prob_mc *= eff;
    }
    else
    {
      prob_data *= (1. - sf * eff);
      prob_mc *= (1. - eff);
    }
  }
  const double sf = aux::compSF(prob_data, prob_mc);
  return sf;
}

void
SubjetBtagSFInterface::reset()
{
  genParticles_.clear();
  subjets_.clear();
  flavors_.clear();
}
