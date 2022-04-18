#include "TallinnNtupleProducer/EvtWeightTools/interface/EvtWeightRecorder.h"

#include "TallinnNtupleProducer/CommonTools/interface/cmsException.h"                                      // cmsException()
#include "TallinnNtupleProducer/CommonTools/interface/sysUncertOptions.h"                                  // EWKJetSys, EWKBJetSys, kFRjt_central, kFRl_central, TriggerSFsysChoice
#include "TallinnNtupleProducer/EvtWeightTools/interface/BtagSFRatioInterface.h"                           // BtagSFRatioInterface
#include "TallinnNtupleProducer/EvtWeightTools/interface/Data_to_MC_CorrectionInterface_Base.h"            // Data_to_MC_CorrectionInterface_Base
#include "TallinnNtupleProducer/EvtWeightTools/interface/Data_to_MC_CorrectionInterface_0l_2tau_trigger.h" // Data_to_MC_CorrectionInterface_0l_2tau_trigger
#include "TallinnNtupleProducer/EvtWeightTools/interface/Data_to_MC_CorrectionInterface_1l_1tau_trigger.h" // Data_to_MC_CorrectionInterface_1l_1tau_trigger
#include "TallinnNtupleProducer/EvtWeightTools/interface/Data_to_MC_CorrectionInterface_1l_2tau_trigger.h" // Data_to_MC_CorrectionInterface_1l_2tau_trigger
#include "TallinnNtupleProducer/EvtWeightTools/interface/DYMCNormScaleFactors.h"                           // DYMCNormScaleFactors
#include "TallinnNtupleProducer/EvtWeightTools/interface/DYMCReweighting.h"                                // DYMCReweighting
#include "TallinnNtupleProducer/EvtWeightTools/interface/EvtWeightManager.h"                               // EvtWeightManager
#include "TallinnNtupleProducer/EvtWeightTools/interface/fakeBackgroundAuxFunctions.h"                     // getWeight_1L(), getWeight_2L(), getWeight_3L(),getWeight_4L()
#include "TallinnNtupleProducer/EvtWeightTools/interface/HadTauFakeRateInterface.h"                        // HadTauFakeRateInterface
#include "TallinnNtupleProducer/EvtWeightTools/interface/HHWeightInterfaceLO.h"                            // HHWeightInterfaceLO
#include "TallinnNtupleProducer/EvtWeightTools/interface/HHWeightInterfaceNLO.h"                           // HHWeightInterfaceNLO
#include "TallinnNtupleProducer/EvtWeightTools/interface/LeptonFakeRateInterface.h"                        // LeptonFakeRateInterface
#include "TallinnNtupleProducer/EvtWeightTools/interface/LHEVpt_LOtoNLO.h"                                 // LHEVpt_LOtoNLO
#include "TallinnNtupleProducer/EvtWeightTools/interface/SubjetBtagSFInterface.h"                          // SubjetBtagSFInterface
#include "TallinnNtupleProducer/Objects/interface/EventInfo.h"                                             // EventInfo
#include "TallinnNtupleProducer/Objects/interface/RecoHadTau.h"                                            // RecoHadTau
#include "TallinnNtupleProducer/Objects/interface/RecoJetAK4.h"                                            // RecoJetAK4
#include "TallinnNtupleProducer/Objects/interface/RecoLepton.h"                                            // RecoLepton
#include "TallinnNtupleProducer/Readers/interface/L1PreFiringWeightReader.h"                               // L1PreFiringWeightReader
#include "TallinnNtupleProducer/Readers/interface/LHEInfoReader.h"                                         // LHEInfoReader
#include "TallinnNtupleProducer/Readers/interface/PSWeightReader.h"                                        // PSWeightReader

#include <boost/math/special_functions/sign.hpp>                                                           // boost::math::sign()

#include <assert.h>                                                                                        // assert()

EvtWeightRecorder::EvtWeightRecorder()
  : EvtWeightRecorder({ "central" }, "central", false)
{}

EvtWeightRecorder::EvtWeightRecorder(const std::vector<std::string> & central_or_shifts,
                                     const std::string & central_or_shift,
                                     bool isMC)
  : isMC_(isMC)
  , genWeight_(1.)
  , leptonSF_(1.)
  , chargeMisIdProb_(1.)
  , dyBgrWeight_(1.)
  , prescale_(1.)
  , hhWeight_lo_(1.)
  , hhWeight_nlo_(1.)
  , rescaling_(1.)
  , central_or_shift_(central_or_shift)
  , central_or_shifts_(central_or_shifts)
{
  for(const std::string & central_or_shift_option: central_or_shifts_)
  {
    checkOptionValidity(central_or_shift_option, isMC);
  }
  assert(std::find(central_or_shifts_.cbegin(), central_or_shifts_.cend(), central_or_shift_) != central_or_shifts_.cend());
}

double
EvtWeightRecorder::get(const std::string & central_or_shift,
                       const std::string & bin) const
{
  double retVal = (isMC_ ? get_inclusive(central_or_shift, bin) * get_data_to_MC_correction(central_or_shift) * get_prescaleWeight() : 1.) *
         get_FR(central_or_shift) * get_chargeMisIdProb() * get_dyBgrWeight()
  ;
  return retVal;
}

double
EvtWeightRecorder::get_inclusive(const std::string & central_or_shift,
                                 const std::string & bin) const
{
  double retVal = isMC_ ? get_genWeight() * get_auxWeight(central_or_shift) * get_lumiScale(central_or_shift, bin) *
                 get_nom_tH_weight(central_or_shift) * get_puWeight(central_or_shift) *
                 get_l1PreFiringWeight(central_or_shift) * get_lheScaleWeight(central_or_shift) * get_pdfWeight(central_or_shift) *
                 get_dy_rwgt(central_or_shift) * get_rescaling() * get_psWeight(central_or_shift) * get_hhWeight() *
                 get_pdfMemberWeight(central_or_shift) * get_LHEVpt(central_or_shift)
               : 1.
  ;
  return retVal;
}

double
EvtWeightRecorder::get_genWeight() const
{
  return genWeight_;
}

double
EvtWeightRecorder::get_hhWeight() const
{
  return get_hhWeight_lo() * get_hhWeight_nlo();
}

double
EvtWeightRecorder::get_hhWeight_lo() const
{
  return hhWeight_lo_;
}

double
EvtWeightRecorder::get_hhWeight_nlo() const
{
  return hhWeight_nlo_;
}

double
EvtWeightRecorder::get_rescaling() const
{
  return rescaling_;
}

double
EvtWeightRecorder::get_auxWeight(const std::string & central_or_shift) const
{
  if(isMC_ && auxWeight_.count(central_or_shift))
  {
    return auxWeight_.at(central_or_shift);
  }
  return 1.;
}

double
EvtWeightRecorder::get_lumiScale(const std::string & central_or_shift,
                                 const std::string & bin) const
{
  if(isMC_ && lumiScale_.count(central_or_shift))
  {
    if(! lumiScale_.at(central_or_shift).count(bin))
    {
      throw cmsException(this, __func__, __LINE__) << "No such bin found in lumiscale map: '" << bin << '\'';
    }
    return lumiScale_.at(central_or_shift).at(bin);
  }
  return 1.;
}

double
EvtWeightRecorder::get_prescaleWeight() const
{
  return prescale_;
}

double
EvtWeightRecorder::get_btagSFRatio(const std::string & central_or_shift) const
{
  if(isMC_ && btagSFRatio_.count(central_or_shift))
  {
    return btagSFRatio_.at(central_or_shift);
  }
  return 1.;
}

double
EvtWeightRecorder::get_nom_tH_weight(const std::string & central_or_shift) const
{
  if(isMC_ && nom_tH_weight_.count(central_or_shift))
  {
    return nom_tH_weight_.at(central_or_shift);
  }
  return 1.;
}

double
EvtWeightRecorder::get_puWeight(const std::string & central_or_shift) const
{
  if(isMC_)
  {
    const PUsys puSys_option = getPUsys_option(central_or_shift);
    if(weights_pu_.count(puSys_option))
    {
      return  weights_pu_.at(puSys_option);
    }
  }
  return 1.;
}

double
EvtWeightRecorder::get_pileupJetIDSF(const std::string & central_or_shift) const
{
  if(isMC_)
  {
    const pileupJetIDSFsys puJetIDSF_option = getPileupJetIDSFsys_option(central_or_shift);
    if(weights_puJetIDSF_.count(puJetIDSF_option))
    {
      return  weights_puJetIDSF_.at(puJetIDSF_option);
    }
  }
  return 1.;
}

double
EvtWeightRecorder::get_l1PreFiringWeight(const std::string & central_or_shift) const
{
  if(isMC_ && ! weights_l1PreFiring_.empty())
  {
    const L1PreFiringWeightSys l1PreFire_option = getL1PreFiringWeightSys_option(central_or_shift);
    if(weights_l1PreFiring_.count(l1PreFire_option))
    {
      return weights_l1PreFiring_.at(l1PreFire_option);
    }
  }
  return 1.;
}

double
EvtWeightRecorder::get_lheScaleWeight(const std::string & central_or_shift) const
{
  if(isMC_ && ! weights_lheScale_.empty())
  {
    const int lheScale_option = getLHEscale_option(central_or_shift);
    if(weights_lheScale_.count(lheScale_option))
    {
      return weights_lheScale_.at(lheScale_option);
    }
  }
  return 1.;
}

double
EvtWeightRecorder::get_pdfWeight(const std::string & central_or_shift) const
{
  if(isMC_ && ! weights_pdf_.empty())
  {
    const PDFSys pdf_option = getPDFSys_option(central_or_shift);
    if(weights_pdf_.count(pdf_option))
    {
      return weights_pdf_.at(pdf_option);
    }
  }
  return 1.;
}

double
EvtWeightRecorder::get_pdfMemberWeight(const std::string & central_or_shift) const
{
  if(isMC_ && ! weights_pdf_members_.empty() && isPDFsys_member(central_or_shift))
  {
    assert(weights_pdf_members_.count(central_or_shift));
    return weights_pdf_members_.at(central_or_shift);
  }
  return 1.;
}

double
EvtWeightRecorder::get_psWeight(const std::string & central_or_shift) const
{
  if(isMC_ && ! weights_partonShower_.empty())
  {
    const int psWeight_option = getPartonShower_option(central_or_shift);
    if(weights_partonShower_.count(psWeight_option))
    {
      return weights_partonShower_.at(psWeight_option);
    }
  }
  return 1.;
}

double
EvtWeightRecorder::get_btag(const std::string & central_or_shift) const
{
  if(isMC_ && ! weights_btag_.empty())
  {
    const int jetBtagSF_option = getBTagWeight_option(central_or_shift);
    if(weights_btag_.count(jetBtagSF_option))
    {
      return weights_btag_.at(jetBtagSF_option);
    }
  }
  return 1.;
}

double
EvtWeightRecorder::get_ewk_jet(const std::string & central_or_shift) const
{
  if(isMC_ && ! weights_ewk_jet_.empty())
  {
    const EWKJetSys ewk_jet_option = getEWKJetSys_option(central_or_shift);
    if(weights_ewk_jet_.count(ewk_jet_option))
    {
      return weights_ewk_jet_.at(ewk_jet_option);
    }
  }
  return 1.;
}

double
EvtWeightRecorder::get_ewk_bjet(const std::string & central_or_shift) const
{
  if(isMC_ && ! weights_ewk_bjet_.empty())
  {
    const EWKBJetSys ewk_bjet_option = getEWKBJetSys_option(central_or_shift);
    if(weights_ewk_bjet_.count(ewk_bjet_option))
    {
      return weights_ewk_bjet_.at(ewk_bjet_option);
    }
  }
  return 1.;
}

double
EvtWeightRecorder::get_dy_rwgt(const std::string & central_or_shift) const
{
  if(isMC_ && ! weights_dy_rwgt_.empty())
  {
    const int dyMCReweighting_option = getDYMCReweighting_option(central_or_shift);
    if(weights_dy_rwgt_.count(dyMCReweighting_option))
    {
      return weights_dy_rwgt_.at(dyMCReweighting_option);
    }
  }
  return 1.;
}

double
EvtWeightRecorder::get_dy_norm(const std::string & central_or_shift) const
{
  if(isMC_ && ! weights_dy_norm_.empty())
  {
    const int dyMCNormScaleFactors_option = getDYMCNormScaleFactors_option(central_or_shift);
    if(weights_dy_norm_.count(dyMCNormScaleFactors_option))
    {
      return weights_dy_norm_.at(dyMCNormScaleFactors_option);
    }
  }
  return 1.;
}

double
EvtWeightRecorder::get_toppt_rwgt(const std::string & central_or_shift) const
{
  if(isMC_ && ! weights_toppt_rwgt_.empty())
  {
    const int topPtReweighting_option = getTopPtReweighting_option(central_or_shift);
    if(weights_toppt_rwgt_.count(topPtReweighting_option))
    {
      return weights_toppt_rwgt_.at(topPtReweighting_option);
    }
  }
  return 1.;
}

double
EvtWeightRecorder::get_LHEVpt(const std::string & central_or_shift) const
{
  if(isMC_ && ! weights_lhe_vpt_.empty())
  {
    const LHEVptSys lhe_vpt_option = getLHEVptSys_option(central_or_shift);
    if(weights_lhe_vpt_.count(lhe_vpt_option))
    {
      return weights_lhe_vpt_.at(lhe_vpt_option);
    }
  }
  return 1.;
}

double
EvtWeightRecorder::get_subjetBtagSF(const std::string & central_or_shift) const
{
  if(isMC_ && ! weights_subjet_btag_.empty())
  {
    const SubjetBtagSys subjet_btag_option = getSubjetBtagSys_option(central_or_shift);
    if(weights_subjet_btag_.count(subjet_btag_option))
    {
      return weights_subjet_btag_.at(subjet_btag_option);
    }
  }
  return 1.;
}

double
EvtWeightRecorder::get_sf_triggerEff(const std::string & central_or_shift) const
{
  double sf_triggerEff = 1.;
  if(isMC_ && (! weights_leptonTriggerEff_.empty() || ! weights_tauTriggerEff_.empty()))
  {
    const TriggerSFsys triggerSF_lepton_option = getTriggerSF_option(central_or_shift, TriggerSFsysChoice::leptonOnly);
    if(weights_leptonTriggerEff_.count(triggerSF_lepton_option))
    {
      sf_triggerEff *= weights_leptonTriggerEff_.at(triggerSF_lepton_option);
    }
    const TriggerSFsys triggerSF_hadTau_option = getTriggerSF_option(central_or_shift, TriggerSFsysChoice::hadTauOnly);
    if(weights_tauTriggerEff_.count(triggerSF_hadTau_option))
    {
      sf_triggerEff *= weights_tauTriggerEff_.at(triggerSF_hadTau_option);
    }
  }
  return sf_triggerEff;
}

double
EvtWeightRecorder::get_leptonSF() const
{
  return isMC_ ? leptonSF_ : 1.;
}

double
EvtWeightRecorder::get_chargeMisIdProb() const
{
  return chargeMisIdProb_;
}

double
EvtWeightRecorder::get_dyBgrWeight() const
{
  return dyBgrWeight_;
}

double
EvtWeightRecorder::get_data_to_MC_correction(const std::string & central_or_shift) const
{
  return isMC_ ? get_sf_triggerEff(central_or_shift) * get_leptonSF() * get_leptonIDSF(central_or_shift) *
                 get_tauSF(central_or_shift) * get_btag(central_or_shift) * get_dy_norm(central_or_shift) *
                 get_toppt_rwgt(central_or_shift) * get_ewk_jet(central_or_shift) * get_ewk_bjet(central_or_shift) *
                 get_btagSFRatio(central_or_shift) * get_pileupJetIDSF(central_or_shift) * get_subjetBtagSF(central_or_shift)
               : 1.
  ;
}

double
EvtWeightRecorder::get_tauSF(const std::string & central_or_shift) const
{
  double tauSF_weight = 1.;
  if(isMC_)
  {
    const TauIDSFsys tauIDSF_option = getTauIDSFsys_option(central_or_shift);
    if(weights_hadTauID_and_Iso_.count(tauIDSF_option))
    {
      tauSF_weight *= weights_hadTauID_and_Iso_.at(tauIDSF_option);
    }
    const FRet eToTauFakeRate_option = getEToTauFR_option(central_or_shift);
    if(weights_eToTauFakeRate_.count(eToTauFakeRate_option))
    {
      tauSF_weight *= weights_eToTauFakeRate_.at(eToTauFakeRate_option);
    }
    const FRmt muToTauFakeRate_option = getMuToTauFR_option(central_or_shift);
    if(weights_muToTauFakeRate_.count(muToTauFakeRate_option))
    {
      tauSF_weight *= weights_muToTauFakeRate_.at(muToTauFakeRate_option);
    }
    const int jetToTauFakeRate_option = getJetToTauFR_option(central_or_shift);
    if(weights_SF_hadTau_lead_.count(jetToTauFakeRate_option))
    {
      tauSF_weight *= weights_SF_hadTau_lead_.at(jetToTauFakeRate_option);
    }
    if(weights_SF_hadTau_sublead_.count(jetToTauFakeRate_option))
    {
      tauSF_weight *= weights_SF_hadTau_sublead_.at(jetToTauFakeRate_option);
    }
  }
  return tauSF_weight;
}

double
EvtWeightRecorder::get_FR(const std::string & central_or_shift) const
{
  const int jetToLeptonFakeRate_option = getJetToLeptonFR_option(central_or_shift);
  const int jetToTauFakeRate_option = getJetToTauFR_option(central_or_shift);
  const std::string weightKey = jetToLeptonFakeRate_option == kFRl_central && jetToTauFakeRate_option == kFRjt_central ? "central" : central_or_shift;
  if(weights_FR_.count(weightKey))
  {
    return weights_FR_.at(weightKey);
  }
  return 1.;
}

void
EvtWeightRecorder::record_genWeight(const EventInfo & eventInfo,
                                    bool use_sign_only)
{
  assert(isMC_);
  genWeight_ = boost::math::sign(eventInfo.genWeight);
  if(! use_sign_only)
  {
    const double genWeight_abs = std::fabs(eventInfo.genWeight);
    const double ref_genWeight = eventInfo.get_refGenWeight();
    if(std::fpclassify(ref_genWeight) == FP_ZERO)
    {
      // TODO move this check to EventInfo::set_refGenWeight()
      throw cmsException(this, __func__, __LINE__) << "Reference weight cannot be zero";
    }
    genWeight_ *= std::min(genWeight_abs, 3 * ref_genWeight);
  }
}

void
EvtWeightRecorder::record_auxWeight(const EvtWeightManager * const evtWeightManager)
{
  assert(isMC_);
  auxWeight_.clear();
  for(const std::string & central_or_shift: central_or_shifts_)
  {
    auxWeight_[central_or_shift] = evtWeightManager->has_central_or_shift(central_or_shift) ?
      evtWeightManager->getWeight(central_or_shift) :
      evtWeightManager->getWeight()
    ;
  }
}

void
EvtWeightRecorder::record_dy_rwgt(const DYMCReweighting * const dyReweighting,
                                  const std::vector<GenParticle> & genTauLeptons)
{
  assert(isMC_);
  weights_dy_rwgt_.clear();
  for(const std::string & central_or_shift: central_or_shifts_)
  {
    const int dyMCReweighting_option = getDYMCReweighting_option(central_or_shift);
    if(weights_dy_rwgt_.count(dyMCReweighting_option))
    {
      continue;
    }
    weights_dy_rwgt_[dyMCReweighting_option] = dyReweighting->getWeight(genTauLeptons, dyMCReweighting_option);
  }
}

void
EvtWeightRecorder::record_dy_norm(const DYMCNormScaleFactors * const dyNormScaleFactors,
                                  const std::vector<GenParticle> & genTauLeptons,
                                  int nJets,
                                  int nBLoose,
                                  int nBMedium)
{
  assert(isMC_);
  weights_dy_norm_.clear();
  for(const std::string & central_or_shift: central_or_shifts_)
  {
    const int dyMCNormScaleFactors_option = getDYMCNormScaleFactors_option(central_or_shift);
    if(weights_dy_norm_.count(dyMCNormScaleFactors_option))
    {
      continue;
    }
    weights_dy_norm_[dyMCNormScaleFactors_option] = dyNormScaleFactors->getWeight(
      genTauLeptons, nJets, nBLoose, nBMedium, dyMCNormScaleFactors_option
    );
  }
}

void
EvtWeightRecorder::record_toppt_rwgt(double sf)
{
  assert(isMC_);
  weights_toppt_rwgt_.clear();
  for(const std::string & central_or_shift: central_or_shifts_)
  {
    const int topPtReweighting_option = getTopPtReweighting_option(central_or_shift);
    if(weights_toppt_rwgt_.count(topPtReweighting_option))
    {
      continue;
    }

    if      (topPtReweighting_option == kTopPtReweighting_central  ) weights_toppt_rwgt_[topPtReweighting_option] = sf;
    else if (topPtReweighting_option == kTopPtReweighting_shiftUp  ) weights_toppt_rwgt_[topPtReweighting_option] = sf * sf;
    else if (topPtReweighting_option == kTopPtReweighting_shiftDown) weights_toppt_rwgt_[topPtReweighting_option] = 1.;
    else assert(0);
  }
}
 
void
EvtWeightRecorder::record_lumiScale(const edm::VParameterSet & lumiScales)
{
  assert(isMC_);
  lumiScale_.clear();
  for(const edm::ParameterSet & lumiScale: lumiScales)
  {
    const std::string central_or_shift = lumiScale.getParameter<std::string>("central_or_shift");
    const std::string bin = lumiScale.exists("bin") ? lumiScale.getParameter<std::string>("bin") : "";
    const double nof_events = lumiScale.getParameter<double>("lumi");
    if(! lumiScale_.count(central_or_shift))
    {
      lumiScale_[central_or_shift] = {};
    }
    assert(! lumiScale_.at(central_or_shift).count(bin));
    lumiScale_[central_or_shift][bin] = nof_events;
  }
  assert(lumiScale_.count(central_or_shift_));
  for(const std::string & central_or_shift: central_or_shifts_)
  {
    if(! lumiScale_.count(central_or_shift))
    {
      lumiScale_[central_or_shift] = lumiScale_.at(central_or_shift_);
    }
  }
}

void
EvtWeightRecorder::record_btagSFRatio(const BtagSFRatioInterface * const btagSFRatioInterface,
                                      int nselJets)
{
  assert(isMC_);
  btagSFRatio_.clear();
  for(const std::string & central_or_shift: central_or_shifts_)
  {
    btagSFRatio_[central_or_shift] = btagSFRatioInterface->get_btagSFRatio(central_or_shift, nselJets);
  }
  assert(btagSFRatio_.count(central_or_shift_));
}

void
EvtWeightRecorder::record_rescaling(double rescaling)
{
  assert(isMC_);
  rescaling_ = rescaling;
}

void
EvtWeightRecorder::record_nom_tH_weight(const EventInfo * const eventInfo)
{
  assert(isMC_);
  nom_tH_weight_.clear();
  for(const std::string & central_or_shift: central_or_shifts_)
  {
    nom_tH_weight_[central_or_shift] = eventInfo->has_central_or_shift(central_or_shift) ?
      eventInfo->genWeight_tH(central_or_shift) :
      eventInfo->genWeight_tH()
    ;
  }
}

void
EvtWeightRecorder::record_leptonSF(double weight)
{
  assert(isMC_);
  leptonSF_ *= weight;
}

void
EvtWeightRecorder::record_leptonIDSF_recoToLoose(const Data_to_MC_CorrectionInterface_Base * const dataToMCcorrectionInterface)
{
  assert(isMC_);
  weights_leptonID_and_Iso_recoToLoose_.clear();
  for(const std::string & central_or_shift: central_or_shifts_)
  {
    const LeptonIDSFsys leptonIDSF_option = getLeptonIDSFsys_option(central_or_shift);
    if(weights_leptonID_and_Iso_recoToLoose_.count(leptonIDSF_option))
    {
      continue;
    }
    weights_leptonID_and_Iso_recoToLoose_[leptonIDSF_option] = dataToMCcorrectionInterface->getSF_leptonID_and_Iso_loose(leptonIDSF_option);
  }
}

void
EvtWeightRecorder::record_leptonIDSF_looseToTight(const Data_to_MC_CorrectionInterface_Base * const dataToMCcorrectionInterface,
                                                  bool woTightCharge)
{
  assert(isMC_);
  weights_leptonID_and_Iso_looseToTight_.clear();
  for(const std::string & central_or_shift: central_or_shifts_)
  {
    const LeptonIDSFsys leptonIDSF_option = getLeptonIDSFsys_option(central_or_shift);
    if(weights_leptonID_and_Iso_looseToTight_.count(leptonIDSF_option))
    {
      continue;
    }
    if(woTightCharge)
    {
      weights_leptonID_and_Iso_looseToTight_[leptonIDSF_option] = dataToMCcorrectionInterface->getSF_leptonID_and_Iso_tight_to_loose_woTightCharge(leptonIDSF_option);
    }
    else
    {
      weights_leptonID_and_Iso_looseToTight_[leptonIDSF_option] = dataToMCcorrectionInterface->getSF_leptonID_and_Iso_tight_to_loose_wTightCharge(leptonIDSF_option);
    }
  }
}

double
EvtWeightRecorder::get_leptonIDSF_recoToLoose(const std::string & central_or_shift) const
{
  if(isMC_ && ! weights_leptonID_and_Iso_recoToLoose_.empty())
  {
    const LeptonIDSFsys leptonIDSF_option = getLeptonIDSFsys_option(central_or_shift);
    if(weights_leptonID_and_Iso_recoToLoose_.count(leptonIDSF_option))
    {
      return weights_leptonID_and_Iso_recoToLoose_.at(leptonIDSF_option);
    }
  }
  return 1.;
}

double
EvtWeightRecorder::get_leptonIDSF_looseToTight(const std::string & central_or_shift) const
{
  if(isMC_ && ! weights_leptonID_and_Iso_looseToTight_.empty())
  {
    const LeptonIDSFsys leptonIDSF_option = getLeptonIDSFsys_option(central_or_shift);
    if(weights_leptonID_and_Iso_looseToTight_.count(leptonIDSF_option))
    {
      return weights_leptonID_and_Iso_looseToTight_.at(leptonIDSF_option);
    }
  }
  return 1.;
}

double
EvtWeightRecorder::get_leptonIDSF(const std::string & central_or_shift) const
{
  return get_leptonIDSF_recoToLoose(central_or_shift) * get_leptonIDSF_looseToTight(central_or_shift);
}

void
EvtWeightRecorder::record_chargeMisIdProb(double weight)
{
  chargeMisIdProb_ = weight;
}

void
EvtWeightRecorder::record_dyBgrWeight(double weight)
{
  dyBgrWeight_ = weight;
}

void
EvtWeightRecorder::record_prescale(double weight)
{
  assert(isMC_);
  prescale_ = weight;
}

void
EvtWeightRecorder::record_hhWeight_lo(double weight)
{
  assert(isMC_);
  hhWeight_lo_ = weight;
}

void
EvtWeightRecorder::record_hhWeight_lo(const HHWeightInterfaceLO * const HHWeightLO_calc,
                                      const EventInfo & eventInfo,
                                      bool isDEBUG)
{
  assert(HHWeightLO_calc);
  return record_hhWeight_lo(HHWeightLO_calc->getWeight("SM", eventInfo.gen_mHH, eventInfo.gen_cosThetaStar, isDEBUG));
}

void
EvtWeightRecorder::record_hhWeight_nlo(double weight)
{
  assert(isMC_);
  hhWeight_nlo_ = weight;
}

void
EvtWeightRecorder::record_hhWeight_nlo(const HHWeightInterfaceNLO * const HHWeightNLO_calc,
                                       const EventInfo & eventInfo,
                                       bool isDEBUG)
{
  assert(HHWeightNLO_calc);
  return record_hhWeight_nlo(HHWeightNLO_calc->getWeight_LOtoNLO("SM", eventInfo.gen_mHH, eventInfo.gen_cosThetaStar, isDEBUG));
}

void
EvtWeightRecorder::record_l1PrefireWeight(const L1PreFiringWeightReader * const l1PreFiringWeightReader)
{
  assert(isMC_);
  weights_l1PreFiring_.clear();
  for(const std::string & central_or_shift: central_or_shifts_)
  {
    const L1PreFiringWeightSys l1PreFire_option = getL1PreFiringWeightSys_option(central_or_shift);
    if(weights_l1PreFiring_.count(l1PreFire_option))
    {
      continue;
    }
    weights_l1PreFiring_[l1PreFire_option] = l1PreFiringWeightReader->getWeight(l1PreFire_option);
  }
}

void
EvtWeightRecorder::record_lheScaleWeight(const LHEInfoReader * const lheInfoReader)
{
  assert(isMC_);
  weights_lheScale_.clear();
  for(const std::string & central_or_shift: central_or_shifts_)
  {
    const int lheScale_option = getLHEscale_option(central_or_shift);
    if(weights_lheScale_.count(lheScale_option))
    {
      continue;
    }
    weights_lheScale_[lheScale_option] = lheInfoReader->getWeight_scale(lheScale_option);
  }
}

void
EvtWeightRecorder::record_pdfWeight(const LHEInfoReader * const lheInfoReader)
{
  assert(isMC_);
  weights_pdf_.clear();
  for(const std::string & central_or_shift: central_or_shifts_)
  {
    const PDFSys pdf_option = getPDFSys_option(central_or_shift);
    if(weights_pdf_.count(pdf_option))
    {
      continue;
    }
    weights_pdf_[pdf_option] = lheInfoReader->getWeight_pdf((unsigned int)pdf_option);
  }
}

void
EvtWeightRecorder::record_pdfMembers(const LHEInfoReader * const lheInfoReader,
                                     const std::map<std::string, int> & pdf_map)
{
  assert(isMC_);
  weights_pdf_members_.clear();
  for(const auto & kv: pdf_map)
  {
    weights_pdf_members_[kv.first] = lheInfoReader->getWeightNorm_pdf(kv.second);
  }
}

void
EvtWeightRecorder::record_psWeight(const PSWeightReader * const psWeightReader)
{
  assert(isMC_);
  weights_partonShower_.clear();
  for(const std::string & central_or_shift: central_or_shifts_)
  {
    const int psWeight_option = getPartonShower_option(central_or_shift);
    if(weights_partonShower_.count(psWeight_option))
    {
      continue;
    }
    weights_partonShower_[psWeight_option] = psWeightReader->getWeight_ps(psWeight_option);
  }
}

void
EvtWeightRecorder::record_puWeight(const EventInfo * const eventInfo)
{
  assert(isMC_);
  weights_pu_.clear();
  for(const std::string & central_or_shift: central_or_shifts_)
  {
    const PUsys puSys_option = getPUsys_option(central_or_shift);
    if(weights_pu_.count(puSys_option))
    {
      continue;
    }
    switch(puSys_option)
    {
      case PUsys::central: weights_pu_[puSys_option] = eventInfo->pileupWeight;     break;
      case PUsys::up:      weights_pu_[puSys_option] = eventInfo->pileupWeightUp;   break;
      case PUsys::down:    weights_pu_[puSys_option] = eventInfo->pileupWeightDown; break;
      default: assert(0);
    }
  }
}

void
EvtWeightRecorder::record_pileupJetIDSF(const Data_to_MC_CorrectionInterface_Base * const dataToMCcorrectionInterface)
{
  assert(isMC_);
  weights_puJetIDSF_.clear();
  for(const std::string & central_or_shift: central_or_shifts_)
  {
    const pileupJetIDSFsys puJetIDSF_option = getPileupJetIDSFsys_option(central_or_shift);
    if(weights_puJetIDSF_.count(puJetIDSF_option))
    {
      continue;
    }
    weights_puJetIDSF_[puJetIDSF_option] = dataToMCcorrectionInterface->getSF_pileupJetID(puJetIDSF_option);
  }
}

namespace
{
  double
  get_EWK_jet_weight(const std::vector<const RecoJetAK4 *> & jets,
                     EWKJetSys ewk_jet_option)
  {
    const double ewk_jet_weight = 1.;
    const double ewk_jet_unc = 0.3;
    if(jets.size() > 2)
    {
      switch(ewk_jet_option)
      {
        case EWKJetSys::central: return ewk_jet_weight;
        case EWKJetSys::up:      return ewk_jet_weight + ewk_jet_unc;
        case EWKJetSys::down:    return ewk_jet_weight - ewk_jet_unc;
        default: assert(0);
      }
    }
    return ewk_jet_weight;
  }

  double
  get_EWK_bjet_weight(const std::vector<const RecoJetAK4 *> & bjets,
                      EWKBJetSys ewk_bjet_option)
  {
    const double ewk_bjet_weight = 1.;
    const double ewk_bjet_unc = bjets.size() > 1 ? 0.4 : 0.1;
    switch(ewk_bjet_option)
    {
      case EWKBJetSys::central: return ewk_bjet_weight;
      case EWKBJetSys::up:      return ewk_bjet_weight + ewk_bjet_unc;
      case EWKBJetSys::down:    return ewk_bjet_weight - ewk_bjet_unc;
      default: assert(0);
    }
  } 
}

void
EvtWeightRecorder::record_ewk_jet(const std::vector<const RecoJetAK4 *> & jets)
{
  assert(isMC_);
  weights_ewk_jet_.clear();
  for(const std::string & central_or_shift: central_or_shifts_)
  {
    const EWKJetSys ewk_jet_option = getEWKJetSys_option(central_or_shift);
    if(weights_ewk_jet_.count(ewk_jet_option))
    {
      continue;
    }
    weights_ewk_jet_[ewk_jet_option] = get_EWK_jet_weight(jets, ewk_jet_option);
  }
}

void
EvtWeightRecorder::record_ewk_bjet(const std::vector<const RecoJetAK4 *> & bjets)
{
  assert(isMC_);
  weights_ewk_bjet_.clear();
  for(const std::string & central_or_shift: central_or_shifts_)
  {
    const EWKBJetSys ewk_bjet_option = getEWKBJetSys_option(central_or_shift);
    if(weights_ewk_bjet_.count(ewk_bjet_option))
    {
      continue;
    }
    weights_ewk_bjet_[ewk_bjet_option] = get_EWK_bjet_weight(bjets, ewk_bjet_option);
  }
}

void
EvtWeightRecorder::record_LHEVpt(const LHEVpt_LOtoNLO * const lhe_vpt)
{
  assert(isMC_);
  weights_lhe_vpt_.clear();
  for(const std::string & central_or_shift: central_or_shifts_)
  {
    const LHEVptSys lhe_vpt_option = getLHEVptSys_option(central_or_shift);
    if(weights_lhe_vpt_.count(lhe_vpt_option))
    {
      continue;
    }
    weights_lhe_vpt_[lhe_vpt_option] = lhe_vpt->getWeight(lhe_vpt_option);
  }
}

void
EvtWeightRecorder::record_subjetBtagSF(SubjetBtagSFInterface * const subjetBtagSFInterface)
{
  assert(isMC_);
  weights_subjet_btag_.clear();
  for(const std::string & central_or_shift: central_or_shifts_)
  {
    const SubjetBtagSys subjet_btag_option = getSubjetBtagSys_option(central_or_shift);
    if(weights_subjet_btag_.count(subjet_btag_option))
    {
      continue;
    }
    weights_subjet_btag_[subjet_btag_option] = subjetBtagSFInterface->get_sf(subjet_btag_option);
  }
}

namespace
{
  double
  get_BtagWeight(const std::vector<const RecoJetAK4 *> & jets,
                 int central_or_shift)
  {
    double btag_weight = 1.;
    for(const RecoJetAK4 * jet: jets)
    {
      btag_weight *= jet->BtagWeight(central_or_shift);
    }
    return btag_weight;
  }
}

void
EvtWeightRecorder::record_btagWeight(const std::vector<const RecoJetAK4 *> & jets)
{
  assert(isMC_);
  weights_btag_.clear();
  for(const std::string & central_or_shift: central_or_shifts_)
  {
    const int jetBtagSF_option = getBTagWeight_option(central_or_shift);
    if(weights_btag_.count(jetBtagSF_option))
    {
      continue;
    }
    weights_btag_[jetBtagSF_option] = get_BtagWeight(jets, jetBtagSF_option);
  }
}

void
EvtWeightRecorder::record_leptonTriggerEff(const Data_to_MC_CorrectionInterface_Base * const dataToMCcorrectionInterface)
{
  assert(isMC_);
  weights_leptonTriggerEff_.clear();
  for(const std::string & central_or_shift: central_or_shifts_)
  {
    const TriggerSFsys triggerSF_option = getTriggerSF_option(central_or_shift, TriggerSFsysChoice::leptonOnly);
    if(weights_leptonTriggerEff_.count(triggerSF_option))
    {
      continue;
    }
    weights_leptonTriggerEff_[triggerSF_option] = dataToMCcorrectionInterface->getSF_leptonTriggerEff(triggerSF_option);
  }
}

void
EvtWeightRecorder::record_tauTriggerEff(const Data_to_MC_CorrectionInterface_0l_2tau_trigger * const dataToMCcorrectionInterface_0l_2tau_trigger)
{
  assert(isMC_);
  weights_tauTriggerEff_.clear();
  for(const std::string & central_or_shift: central_or_shifts_)
  {
    const TriggerSFsys triggerSF_option = getTriggerSF_option(central_or_shift, TriggerSFsysChoice::hadTauOnly);
    if(weights_tauTriggerEff_.count(triggerSF_option))
    {
      continue;
    }
    weights_tauTriggerEff_[triggerSF_option] = dataToMCcorrectionInterface_0l_2tau_trigger->getSF_triggerEff(triggerSF_option);
  }
}

void
EvtWeightRecorder::record_tauTriggerEff(const Data_to_MC_CorrectionInterface_1l_1tau_trigger * const dataToMCcorrectionInterface_1l_1tau_trigger)
{
  assert(isMC_);
  weights_tauTriggerEff_.clear();
  for(const std::string & central_or_shift: central_or_shifts_)
  {
    const TriggerSFsys triggerSF_option = getTriggerSF_option(central_or_shift, TriggerSFsysChoice::hadTauOnly);
    if(weights_tauTriggerEff_.count(triggerSF_option))
    {
      continue;
    }
    weights_tauTriggerEff_[triggerSF_option] = dataToMCcorrectionInterface_1l_1tau_trigger->getSF_triggerEff(triggerSF_option);
  }
}

void
EvtWeightRecorder::record_tauTriggerEff(const Data_to_MC_CorrectionInterface_1l_2tau_trigger * const dataToMCcorrectionInterface_1l_2tau_trigger)
{
  assert(isMC_);
  weights_tauTriggerEff_.clear();
  for(const std::string & central_or_shift: central_or_shifts_)
  {
    const TriggerSFsys triggerSF_option = getTriggerSF_option(central_or_shift, TriggerSFsysChoice::hadTauOnly);
    if(weights_tauTriggerEff_.count(triggerSF_option))
    {
      continue;
    }
    weights_tauTriggerEff_[triggerSF_option] = dataToMCcorrectionInterface_1l_2tau_trigger->getSF_triggerEff(triggerSF_option);
  }
}

void
EvtWeightRecorder::record_hadTauID_and_Iso(const Data_to_MC_CorrectionInterface_Base * const dataToMCcorrectionInterface)
{
  assert(isMC_);
  weights_hadTauID_and_Iso_.clear();
  for(const std::string & central_or_shift: central_or_shifts_)
  {
    const TauIDSFsys tauIDSF_option = getTauIDSFsys_option(central_or_shift);
    if(weights_hadTauID_and_Iso_.count(tauIDSF_option))
    {
      continue;
    }
    weights_hadTauID_and_Iso_[tauIDSF_option] = dataToMCcorrectionInterface->getSF_hadTauID_and_Iso(tauIDSF_option);
  }
}

void
EvtWeightRecorder::record_eToTauFakeRate(const Data_to_MC_CorrectionInterface_Base * const dataToMCcorrectionInterface)
{
  assert(isMC_);
  weights_eToTauFakeRate_.clear();
  for(const std::string & central_or_shift: central_or_shifts_)
  {
    const FRet eToTauFakeRate_option = getEToTauFR_option(central_or_shift);
    if(weights_eToTauFakeRate_.count(eToTauFakeRate_option))
    {
      continue;
    }
    weights_eToTauFakeRate_[eToTauFakeRate_option] = dataToMCcorrectionInterface->getSF_eToTauFakeRate(eToTauFakeRate_option);
  }
}

void
EvtWeightRecorder::record_muToTauFakeRate(const Data_to_MC_CorrectionInterface_Base * const dataToMCcorrectionInterface)
{
  assert(isMC_);
  weights_muToTauFakeRate_.clear();
  for(const std::string & central_or_shift: central_or_shifts_)
  {
    const FRmt muToTauFakeRate_option = getMuToTauFR_option(central_or_shift);
    if(weights_muToTauFakeRate_.count(muToTauFakeRate_option))
    {
      continue;
    }
    weights_muToTauFakeRate_[muToTauFakeRate_option] = dataToMCcorrectionInterface->getSF_muToTauFakeRate(muToTauFakeRate_option);
  }
}

void
EvtWeightRecorder::record_jetToTau_FR_lead(const HadTauFakeRateInterface * const hadTauFakeRateInterface,
                                           const RecoHadTau * const hadTau_lead)
{
  const double hadTauPt_lead = hadTau_lead->pt();
  const double hadTauAbsEta_lead = hadTau_lead->absEta();
  weights_FR_hadTau_lead_.clear();
  for(const std::string & central_or_shift: central_or_shifts_)
  {
    const int jetToTauFakeRate_option = getJetToTauFR_option(central_or_shift);
    if(weights_FR_hadTau_lead_.count(jetToTauFakeRate_option))
    {
      continue;
    }
    weights_FR_hadTau_lead_[jetToTauFakeRate_option] = hadTauFakeRateInterface->getWeight_lead(
      hadTauPt_lead, hadTauAbsEta_lead, jetToTauFakeRate_option
    );
  }
}

void
EvtWeightRecorder::record_jetToTau_FR_sublead(const HadTauFakeRateInterface * const hadTauFakeRateInterface,
                                              const RecoHadTau * const hadTau_sublead)
{
  const double hadTauPt_sublead = hadTau_sublead->pt();
  const double hadTauAbsEta_sublead = hadTau_sublead->absEta();
  weights_FR_hadTau_sublead_.clear();
  for(const std::string & central_or_shift: central_or_shifts_)
  {
    const int jetToTauFakeRate_option = getJetToTauFR_option(central_or_shift);
    if(weights_FR_hadTau_sublead_.count(jetToTauFakeRate_option))
    {
      continue;
    }
    weights_FR_hadTau_sublead_[jetToTauFakeRate_option] = hadTauFakeRateInterface->getWeight_sublead(
      hadTauPt_sublead, hadTauAbsEta_sublead, jetToTauFakeRate_option
    );
  }
}

void
EvtWeightRecorder::record_jetToTau_SF_lead(const HadTauFakeRateInterface * const hadTauFakeRateInterface,
                                           const RecoHadTau * const hadTau_lead)
{
  assert(isMC_);
  const double hadTauPt_lead = hadTau_lead->pt();
  const double hadTauAbsEta_lead = hadTau_lead->absEta();
  weights_SF_hadTau_lead_.clear();
  for(const std::string & central_or_shift: central_or_shifts_)
  {
    const int jetToTauFakeRate_option = getJetToTauFR_option(central_or_shift);
    if(weights_SF_hadTau_lead_.count(jetToTauFakeRate_option))
    {
      continue;
    }
    weights_SF_hadTau_lead_[jetToTauFakeRate_option] = hadTauFakeRateInterface->getSF_lead(
      hadTauPt_lead, hadTauAbsEta_lead, jetToTauFakeRate_option
    );
  }
}

void
EvtWeightRecorder::record_jetToTau_SF_sublead(const HadTauFakeRateInterface * const hadTauFakeRateInterface,
                                              const RecoHadTau * const hadTau_sublead)
{
  assert(isMC_);
  const double hadTauPt_sublead = hadTau_sublead->pt();
  const double hadTauAbsEta_sublead = hadTau_sublead->absEta();
  weights_SF_hadTau_sublead_.clear();
  for(const std::string & central_or_shift: central_or_shifts_)
  {
    const int jetToTauFakeRate_option = getJetToTauFR_option(central_or_shift);
    if(weights_SF_hadTau_sublead_.count(jetToTauFakeRate_option))
    {
      continue;
    }
    weights_SF_hadTau_sublead_[jetToTauFakeRate_option] = hadTauFakeRateInterface->getSF_sublead(
      hadTauPt_sublead, hadTauAbsEta_sublead, jetToTauFakeRate_option
    );
  }
}

void
EvtWeightRecorder::record_jetToLepton_FR(const LeptonFakeRateInterface * const leptonFakeRateInterface,
                                         const RecoLepton * const lepton,
                                         std::map<int, double> & weights_FR_lepton)
{
  const int leptonPdgId = std::abs(lepton->pdgId());
  const double leptonPt = lepton->cone_pt();
  const double leptonAbsEta = lepton->absEta();
  assert(leptonPdgId == 11 || leptonPdgId == 13);
  weights_FR_lepton.clear();
  for(const std::string & central_or_shift: central_or_shifts_)
  {
    const int jetToLeptonFakeRate_option = getJetToLeptonFR_option(central_or_shift);
    if(weights_FR_lepton.count(jetToLeptonFakeRate_option))
    {
      continue;
    }
    if(leptonPdgId == 11)
    {
      int jetToLeptonFakeRate_option_e = jetToLeptonFakeRate_option;
      if(jetToLeptonFakeRate_option_e >= kFRm_shape_ptUp && jetToLeptonFakeRate_option_e <= kFRm_shape_corrDown)
      {
        jetToLeptonFakeRate_option_e = kFRl_central;
      }
      weights_FR_lepton[jetToLeptonFakeRate_option] = leptonFakeRateInterface->getWeight_e(
        leptonPt, leptonAbsEta, jetToLeptonFakeRate_option_e
      );
    }
    else if(leptonPdgId == 13)
    {
      int jetToLeptonFakeRate_option_m = jetToLeptonFakeRate_option;
      if(jetToLeptonFakeRate_option_m >= kFRe_shape_ptUp && jetToLeptonFakeRate_option_m <= kFRe_shape_corrDown)
      {
        jetToLeptonFakeRate_option_m = kFRl_central;
      }
      weights_FR_lepton[jetToLeptonFakeRate_option] = leptonFakeRateInterface->getWeight_mu(
        leptonPt, leptonAbsEta, jetToLeptonFakeRate_option_m
      );
    }
    else
    {
      throw cmsException(this, __func__, __LINE__) << "Invalid PDG ID: " << leptonPdgId;
    }
  }
}

void
EvtWeightRecorder::record_jetToLepton_FR_lead(const LeptonFakeRateInterface * const leptonFakeRateInterface,
                                              const RecoLepton * const lepton_lead)
{
  record_jetToLepton_FR(leptonFakeRateInterface, lepton_lead, weights_FR_lepton_lead_);
}

void
EvtWeightRecorder::record_jetToLepton_FR_sublead(const LeptonFakeRateInterface * const leptonFakeRateInterface,
                                                 const RecoLepton * const lepton_sublead)
{
  record_jetToLepton_FR(leptonFakeRateInterface, lepton_sublead, weights_FR_lepton_sublead_);
}

void
EvtWeightRecorder::record_jetToLepton_FR_third(const LeptonFakeRateInterface * const leptonFakeRateInterface,
                                               const RecoLepton * const lepton_third)
{
  record_jetToLepton_FR(leptonFakeRateInterface, lepton_third, weights_FR_lepton_third_);
}

void
EvtWeightRecorder::record_jetToLepton_FR_fourth(const LeptonFakeRateInterface * const leptonFakeRateInterface,
                                                const RecoLepton * const lepton_fourth)
{
  record_jetToLepton_FR(leptonFakeRateInterface, lepton_fourth, weights_FR_lepton_fourth_);
}

void
EvtWeightRecorder::compute_FR_2l2tau(bool passesTight_lepton_lead,
                                     bool passesTight_lepton_sublead,
                                     bool passesTight_hadTau_lead,
                                     bool passesTight_hadTau_sublead)
{
  assert(! weights_FR_lepton_lead_.empty());
  assert(! weights_FR_lepton_sublead_.empty());
  assert(! weights_FR_hadTau_lead_.empty());
  assert(! weights_FR_hadTau_sublead_.empty());
  weights_FR_.clear();
  for(const std::string & central_or_shift: central_or_shifts_)
  {
    const int jetToLeptonFakeRate_option = getJetToLeptonFR_option(central_or_shift);
    const int jetToTauFakeRate_option = getJetToTauFR_option(central_or_shift);
    assert(weights_FR_lepton_lead_.count(jetToLeptonFakeRate_option));
    assert(weights_FR_lepton_sublead_.count(jetToLeptonFakeRate_option));
    assert(weights_FR_hadTau_lead_.count(jetToTauFakeRate_option));
    assert(weights_FR_hadTau_sublead_.count(jetToTauFakeRate_option));
    const std::string weightKey = jetToLeptonFakeRate_option == kFRl_central && jetToTauFakeRate_option == kFRjt_central ? "central" : central_or_shift;
    if(weights_FR_.count(weightKey))
    {
      continue;
    }
    weights_FR_[weightKey] = getWeight_4L(
      weights_FR_lepton_lead_.at(jetToLeptonFakeRate_option),    passesTight_lepton_lead,
      weights_FR_lepton_sublead_.at(jetToLeptonFakeRate_option), passesTight_lepton_sublead,
      weights_FR_hadTau_lead_.at(jetToTauFakeRate_option),       passesTight_hadTau_lead,
      weights_FR_hadTau_sublead_.at(jetToTauFakeRate_option),    passesTight_hadTau_sublead
    );
  }
}

void
EvtWeightRecorder::compute_FR_2l1tau(bool passesTight_lepton_lead,
                                     bool passesTight_lepton_sublead,
                                     bool passesTight_hadTau)
{
  assert(! weights_FR_lepton_lead_.empty());
  assert(! weights_FR_lepton_sublead_.empty());
  assert(! weights_FR_hadTau_lead_.empty());
  weights_FR_.clear();
  for(const std::string & central_or_shift: central_or_shifts_)
  {
    const int jetToLeptonFakeRate_option = getJetToLeptonFR_option(central_or_shift);
    const int jetToTauFakeRate_option = getJetToTauFR_option(central_or_shift);
    assert(weights_FR_lepton_lead_.count(jetToLeptonFakeRate_option));
    assert(weights_FR_lepton_sublead_.count(jetToLeptonFakeRate_option));
    assert(weights_FR_hadTau_lead_.count(jetToTauFakeRate_option));
    const std::string weightKey = jetToLeptonFakeRate_option == kFRl_central && jetToTauFakeRate_option == kFRjt_central ? "central" : central_or_shift;
    if(weights_FR_.count(weightKey))
    {
      continue;
    }
    weights_FR_[weightKey] = getWeight_3L(
      weights_FR_lepton_lead_.at(jetToLeptonFakeRate_option),    passesTight_lepton_lead,
      weights_FR_lepton_sublead_.at(jetToLeptonFakeRate_option), passesTight_lepton_sublead,
      weights_FR_hadTau_lead_.at(jetToTauFakeRate_option),       passesTight_hadTau
    );
  }
}

void
EvtWeightRecorder::compute_FR_2l(bool passesTight_lepton_lead,
                                 bool passesTight_lepton_sublead)
{
  assert(! weights_FR_lepton_lead_.empty());
  assert(! weights_FR_lepton_sublead_.empty());
  weights_FR_.clear();
  for(const std::string & central_or_shift: central_or_shifts_)
  {
    const int jetToLeptonFakeRate_option = getJetToLeptonFR_option(central_or_shift);
    const int jetToTauFakeRate_option = getJetToTauFR_option(central_or_shift);
    assert(weights_FR_lepton_lead_.count(jetToLeptonFakeRate_option));
    assert(weights_FR_lepton_sublead_.count(jetToLeptonFakeRate_option));
    const std::string weightKey = jetToLeptonFakeRate_option == kFRl_central && jetToTauFakeRate_option == kFRjt_central ? "central" : central_or_shift;
    if(weights_FR_.count(weightKey))
    {
      continue;
    }
    weights_FR_[weightKey] = getWeight_2L(
      weights_FR_lepton_lead_.at(jetToLeptonFakeRate_option),    passesTight_lepton_lead,
      weights_FR_lepton_sublead_.at(jetToLeptonFakeRate_option), passesTight_lepton_sublead
    );
  }
}

void
EvtWeightRecorder::compute_FR_3l(bool passesTight_lepton_lead,
                                 bool passesTight_lepton_sublead,
                                 bool passesTight_lepton_third)
{
  assert(! weights_FR_lepton_lead_.empty());
  assert(! weights_FR_lepton_sublead_.empty());
  assert(! weights_FR_lepton_third_.empty());
  weights_FR_.clear();
  for(const std::string & central_or_shift: central_or_shifts_)
  {
    const int jetToLeptonFakeRate_option = getJetToLeptonFR_option(central_or_shift);
    const int jetToTauFakeRate_option = getJetToTauFR_option(central_or_shift);
    assert(weights_FR_lepton_lead_.count(jetToLeptonFakeRate_option));
    assert(weights_FR_lepton_sublead_.count(jetToLeptonFakeRate_option));
    assert(weights_FR_lepton_third_.count(jetToLeptonFakeRate_option));
    const std::string weightKey = jetToLeptonFakeRate_option == kFRl_central && jetToTauFakeRate_option == kFRjt_central ? "central" : central_or_shift;
    if(weights_FR_.count(weightKey))
    {
      continue;
    }
    weights_FR_[weightKey] = getWeight_3L(
      weights_FR_lepton_lead_.at(jetToLeptonFakeRate_option),    passesTight_lepton_lead,
      weights_FR_lepton_sublead_.at(jetToLeptonFakeRate_option), passesTight_lepton_sublead,
      weights_FR_lepton_third_.at(jetToLeptonFakeRate_option),   passesTight_lepton_third
    );
  }
}

void
EvtWeightRecorder::compute_FR_4l(bool passesTight_lepton_lead,
                                 bool passesTight_lepton_sublead,
                                 bool passesTight_lepton_third,
                                 bool passesTight_lepton_fourth)
{
  assert(! weights_FR_lepton_lead_.empty());
  assert(! weights_FR_lepton_sublead_.empty());
  assert(! weights_FR_lepton_third_.empty());
  assert(! weights_FR_lepton_fourth_.empty());
  weights_FR_.clear();
  for(const std::string & central_or_shift: central_or_shifts_)
  {
    const int jetToLeptonFakeRate_option = getJetToLeptonFR_option(central_or_shift);
    const int jetToTauFakeRate_option = getJetToTauFR_option(central_or_shift);
    assert(weights_FR_lepton_lead_.count(jetToLeptonFakeRate_option));
    assert(weights_FR_lepton_sublead_.count(jetToLeptonFakeRate_option));
    assert(weights_FR_lepton_third_.count(jetToLeptonFakeRate_option));
    assert(weights_FR_lepton_fourth_.count(jetToLeptonFakeRate_option));
    const std::string weightKey = jetToLeptonFakeRate_option == kFRl_central && jetToTauFakeRate_option == kFRjt_central ? "central" : central_or_shift;
    if(weights_FR_.count(weightKey))
    {
      continue;
    }
    weights_FR_[weightKey] = getWeight_4L(
      weights_FR_lepton_lead_.at(jetToLeptonFakeRate_option),    passesTight_lepton_lead,
      weights_FR_lepton_sublead_.at(jetToLeptonFakeRate_option), passesTight_lepton_sublead,
      weights_FR_lepton_third_.at(jetToLeptonFakeRate_option),   passesTight_lepton_third,
      weights_FR_lepton_fourth_.at(jetToLeptonFakeRate_option),  passesTight_lepton_fourth
    );
  }
}

void
EvtWeightRecorder::compute_FR_3l1tau(bool passesTight_lepton_lead,
                                     bool passesTight_lepton_sublead,
                                     bool passesTight_lepton_third,
                                     bool passesTight_hadTau)
{
  assert(! weights_FR_lepton_lead_.empty());
  assert(! weights_FR_lepton_sublead_.empty());
  assert(! weights_FR_lepton_third_.empty());
  assert(! weights_FR_hadTau_lead_.empty());
  weights_FR_.clear();
  for(const std::string & central_or_shift: central_or_shifts_)
  {
    const int jetToLeptonFakeRate_option = getJetToLeptonFR_option(central_or_shift);
    const int jetToTauFakeRate_option = getJetToTauFR_option(central_or_shift);
    assert(weights_FR_lepton_lead_.count(jetToLeptonFakeRate_option));
    assert(weights_FR_lepton_sublead_.count(jetToLeptonFakeRate_option));
    assert(weights_FR_lepton_third_.count(jetToLeptonFakeRate_option));
    assert(weights_FR_hadTau_lead_.count(jetToTauFakeRate_option));
    const std::string weightKey = jetToLeptonFakeRate_option == kFRl_central && jetToTauFakeRate_option == kFRjt_central ? "central" : central_or_shift;
    if(weights_FR_.count(weightKey))
    {
      continue;
    }
    weights_FR_[weightKey] = getWeight_4L(
      weights_FR_lepton_lead_.at(jetToLeptonFakeRate_option),    passesTight_lepton_lead,
      weights_FR_lepton_sublead_.at(jetToLeptonFakeRate_option), passesTight_lepton_sublead,
      weights_FR_lepton_third_.at(jetToLeptonFakeRate_option),   passesTight_lepton_third,
      weights_FR_hadTau_lead_.at(jetToTauFakeRate_option),       passesTight_hadTau
    );
  }
}

void
EvtWeightRecorder::compute_FR_1l2tau(bool passesTight_lepton,
                                     bool passesTight_hadTau_lead,
                                     bool passesTight_hadTau_sublead)
{
  assert(! weights_FR_lepton_lead_.empty());
  assert(! weights_FR_hadTau_lead_.empty());
  assert(! weights_FR_hadTau_sublead_.empty());
  weights_FR_.clear();
  for(const std::string & central_or_shift: central_or_shifts_)
  {
    const int jetToLeptonFakeRate_option = getJetToLeptonFR_option(central_or_shift);
    const int jetToTauFakeRate_option = getJetToTauFR_option(central_or_shift);
    assert(weights_FR_lepton_lead_.count(jetToLeptonFakeRate_option));
    assert(weights_FR_hadTau_lead_.count(jetToTauFakeRate_option));
    assert(weights_FR_hadTau_sublead_.count(jetToTauFakeRate_option));
    const std::string weightKey = jetToLeptonFakeRate_option == kFRl_central && jetToTauFakeRate_option == kFRjt_central ? "central" : central_or_shift;
    if(weights_FR_.count(weightKey))
    {
      continue;
    }
    weights_FR_[weightKey] = getWeight_3L(
      weights_FR_lepton_lead_.at(jetToLeptonFakeRate_option), passesTight_lepton,
      weights_FR_hadTau_lead_.at(jetToTauFakeRate_option),    passesTight_hadTau_lead,
      weights_FR_hadTau_sublead_.at(jetToTauFakeRate_option), passesTight_hadTau_sublead
    );
  }
}

void
EvtWeightRecorder::compute_FR_1l1tau(bool passesTight_lepton,
                                     bool passesTight_hadTau)
{
  assert(! weights_FR_lepton_lead_.empty());
  assert(! weights_FR_hadTau_lead_.empty());
  weights_FR_.clear();
  for(const std::string & central_or_shift: central_or_shifts_)
  {
    const int jetToLeptonFakeRate_option = getJetToLeptonFR_option(central_or_shift);
    const int jetToTauFakeRate_option = getJetToTauFR_option(central_or_shift);
    assert(weights_FR_lepton_lead_.count(jetToLeptonFakeRate_option));
    assert(weights_FR_hadTau_lead_.count(jetToTauFakeRate_option));
    const std::string weightKey = jetToLeptonFakeRate_option == kFRl_central && jetToTauFakeRate_option == kFRjt_central ? "central" : central_or_shift;
    if(weights_FR_.count(weightKey))
    {
      continue;
    }
    weights_FR_[weightKey] = getWeight_2L(
      weights_FR_lepton_lead_.at(jetToLeptonFakeRate_option), passesTight_lepton,
      weights_FR_hadTau_lead_.at(jetToTauFakeRate_option),    passesTight_hadTau
    );
  }
}

void
EvtWeightRecorder::compute_FR_1l(bool passesTight_lepton)
{
  assert(! weights_FR_lepton_lead_.empty());
  weights_FR_.clear();
  for(const std::string & central_or_shift: central_or_shifts_)
  {
    const int jetToLeptonFakeRate_option = getJetToLeptonFR_option(central_or_shift);
    const int jetToTauFakeRate_option = getJetToTauFR_option(central_or_shift);
    assert(weights_FR_lepton_lead_.count(jetToLeptonFakeRate_option));
    const std::string weightKey = jetToLeptonFakeRate_option == kFRl_central && jetToTauFakeRate_option == kFRjt_central ? "central" : central_or_shift;
    if(weights_FR_.count(weightKey))
    {
      continue;
    }
    weights_FR_[weightKey] = ! passesTight_lepton ? getWeight_1L(weights_FR_lepton_lead_.at(jetToLeptonFakeRate_option)) : 1.;
  }
}

void
EvtWeightRecorder::compute_FR_1tau(bool passesTight_hadTau)
{
  assert(! weights_FR_hadTau_lead_.empty());
  weights_FR_.clear();
  for(const std::string & central_or_shift: central_or_shifts_)
  {
    const int jetToLeptonFakeRate_option = getJetToLeptonFR_option(central_or_shift);
    const int jetToTauFakeRate_option = getJetToTauFR_option(central_or_shift);
    assert(weights_FR_hadTau_lead_.count(jetToTauFakeRate_option));
    const std::string weightKey = jetToLeptonFakeRate_option == kFRl_central && jetToTauFakeRate_option == kFRjt_central ? "central" : central_or_shift;
    if(weights_FR_.count(weightKey))
    {
      continue;
    }
    weights_FR_[weightKey] = ! passesTight_hadTau ? getWeight_1L(weights_FR_hadTau_lead_.at(jetToTauFakeRate_option)) : 1.;
  }
}

void
EvtWeightRecorder::compute_FR_2tau(bool passesTight_hadTau_lead,
                                   bool passesTight_hadTau_sublead)
{
  assert(! weights_FR_hadTau_lead_.empty());
  assert(! weights_FR_hadTau_sublead_.empty());
  weights_FR_.clear();
  for(const std::string & central_or_shift: central_or_shifts_)
  {
    const int jetToLeptonFakeRate_option = getJetToLeptonFR_option(central_or_shift);
    const int jetToTauFakeRate_option = getJetToTauFR_option(central_or_shift);
    assert(weights_FR_hadTau_lead_.count(jetToTauFakeRate_option));
    assert(weights_FR_hadTau_sublead_.count(jetToTauFakeRate_option));
    const std::string weightKey = jetToLeptonFakeRate_option == kFRl_central && jetToTauFakeRate_option == kFRjt_central ? "central" : central_or_shift;
    if(weights_FR_.count(weightKey))
    {
      continue;
    }
    weights_FR_[weightKey] = getWeight_2L(
      weights_FR_hadTau_lead_.at(jetToTauFakeRate_option),    passesTight_hadTau_lead,
      weights_FR_hadTau_sublead_.at(jetToTauFakeRate_option), passesTight_hadTau_sublead
    );
  }
}

double
EvtWeightRecorder::get_jetToLepton_FR_lead(const std::string & central_or_shift)
{
  assert(! weights_FR_lepton_lead_.empty());
  const int jetToLeptonFakeRate_option = getJetToLeptonFR_option(central_or_shift);
  assert(weights_FR_lepton_lead_.count(jetToLeptonFakeRate_option));
  return weights_FR_lepton_lead_.at(jetToLeptonFakeRate_option);
}

double
EvtWeightRecorder::get_jetToLepton_FR_sublead(const std::string & central_or_shift)
{
  assert(! weights_FR_lepton_sublead_.empty());
  const int jetToLeptonFakeRate_option = getJetToLeptonFR_option(central_or_shift);
  assert(weights_FR_lepton_sublead_.count(jetToLeptonFakeRate_option));
  return weights_FR_lepton_sublead_.at(jetToLeptonFakeRate_option);
}

double
EvtWeightRecorder::get_jetToLepton_FR_third(const std::string & central_or_shift)
{
  assert(! weights_FR_lepton_third_.empty());
  const int jetToLeptonFakeRate_option = getJetToLeptonFR_option(central_or_shift);
  assert(weights_FR_lepton_third_.count(jetToLeptonFakeRate_option));
  return weights_FR_lepton_third_.at(jetToLeptonFakeRate_option);
}

double
EvtWeightRecorder::get_jetToLepton_FR_fourth(const std::string & central_or_shift)
{
  assert(! weights_FR_lepton_fourth_.empty());
  const int jetToLeptonFakeRate_option = getJetToLeptonFR_option(central_or_shift);
  assert(weights_FR_lepton_fourth_.count(jetToLeptonFakeRate_option));
  return weights_FR_lepton_fourth_.at(jetToLeptonFakeRate_option);
}

double
EvtWeightRecorder::get_jetToTau_FR_lead(const std::string & central_or_shift)
{
  assert(! weights_FR_hadTau_lead_.empty());
  const int jetToTauFakeRate_option = getJetToTauFR_option(central_or_shift);
  assert(weights_FR_hadTau_lead_.count(jetToTauFakeRate_option));
  return weights_FR_hadTau_lead_.at(jetToTauFakeRate_option);
}

double
EvtWeightRecorder::get_jetToTau_FR_sublead(const std::string & central_or_shift)
{
  assert(! weights_FR_hadTau_sublead_.empty());
  const int jetToTauFakeRate_option = getJetToTauFR_option(central_or_shift);
  assert(weights_FR_hadTau_sublead_.count(jetToTauFakeRate_option));
  return weights_FR_hadTau_sublead_.at(jetToTauFakeRate_option);
}

std::ostream &
operator<<(std::ostream & os,
           const EvtWeightRecorder & evtWeightRecorder)
{
  for(const std::string & central_or_shift: evtWeightRecorder.central_or_shifts_)
  {
    os << "central_or_shift = " << central_or_shift                                                       << "\n"
          "  genWeight             = " << evtWeightRecorder.get_genWeight()                               << "\n"
          "  HH weight (LO)        = " << evtWeightRecorder.get_hhWeight_lo()                             << "\n"
          "  HH weight (LO to NLO) = " << evtWeightRecorder.get_hhWeight_nlo()                            << "\n"
          "  stitching weight      = " << evtWeightRecorder.get_auxWeight(central_or_shift)               << "\n"
          "  lumiScale             = " << evtWeightRecorder.get_lumiScale(central_or_shift)               << "\n"
          "  prescale weight       = " << evtWeightRecorder.get_prescaleWeight()                          << "\n"
          "  btag SF ratio         = " << evtWeightRecorder.get_btagSFRatio(central_or_shift)             << "\n"
          "  nominal tH weight     = " << evtWeightRecorder.get_nom_tH_weight(central_or_shift)           << "\n"
          "  PU weight             = " << evtWeightRecorder.get_puWeight(central_or_shift)                << "\n"
          "  L1 prefiring weight   = " << evtWeightRecorder.get_l1PreFiringWeight(central_or_shift)       << "\n"
          "  LHE scale weight      = " << evtWeightRecorder.get_lheScaleWeight(central_or_shift)          << "\n"
          "  parton shower weight  = " << evtWeightRecorder.get_psWeight(central_or_shift)                << "\n"
          "  DY reweighting weight = " << evtWeightRecorder.get_dy_rwgt(central_or_shift)                 << "\n"
          "  inclusive weight      = " << evtWeightRecorder.get_inclusive(central_or_shift)               << "\n"
          "  trigger eff SF        = " << evtWeightRecorder.get_sf_triggerEff(central_or_shift)           << "\n"
          "  lepton SF             = " << evtWeightRecorder.get_leptonSF()                                << "\n"
          "  lepton ID SF (loose)  = " << evtWeightRecorder.get_leptonIDSF_recoToLoose(central_or_shift)  << "\n"
          "  lepton ID SF (tight)  = " << evtWeightRecorder.get_leptonIDSF_looseToTight(central_or_shift) << "\n"
          "  lepton ID SF          = " << evtWeightRecorder.get_leptonIDSF(central_or_shift)              << "\n"
          "  tau SF                = " << evtWeightRecorder.get_tauSF(central_or_shift)                   << "\n"
          "  DY norm weight        = " << evtWeightRecorder.get_dy_norm(central_or_shift)                 << "\n"
          "  TT pT weight          = " << evtWeightRecorder.get_toppt_rwgt(central_or_shift)              << "\n"
          "  btag weight           = " << evtWeightRecorder.get_btag(central_or_shift)                    << "\n"
          "  PU jet ID SF          = " << evtWeightRecorder.get_pileupJetIDSF(central_or_shift)           << "\n"
          "  EWK jet weight        = " << evtWeightRecorder.get_ewk_jet(central_or_shift)                 << "\n"
          "  EWK bjet weight       = " << evtWeightRecorder.get_ewk_bjet(central_or_shift)                << "\n"
          "  data/MC correction    = " << evtWeightRecorder.get_data_to_MC_correction(central_or_shift)   << "\n"
          "  FR weight             = " << evtWeightRecorder.get_FR(central_or_shift)                      << "\n"
          "  rescaling             = " << evtWeightRecorder.get_rescaling()                               << "\n"
          "  charge mis-ID prob    = " << evtWeightRecorder.get_chargeMisIdProb()                         << "\n"
          "  DY bgr weight         = " << evtWeightRecorder.get_dyBgrWeight()                             << "\n"
          "  PDF evnelope weight   = " << evtWeightRecorder.get_pdfWeight(central_or_shift)               << "\n"
          "  PDF member weight     = " << evtWeightRecorder.get_pdfMemberWeight(central_or_shift)         << "\n"
          "  LHE Vpt weight        = " << evtWeightRecorder.get_LHEVpt(central_or_shift)                  << "\n"
          "  subjet b-tagging SF   = " << evtWeightRecorder.get_subjetBtagSF(central_or_shift)            << "\n"
          "  final weight          = " << evtWeightRecorder.get(central_or_shift)                         << '\n'
   ;
  }
  return os;
}
