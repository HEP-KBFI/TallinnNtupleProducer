#include "TallinnNtupleProducer/EvtWeightTools/interface/ChargeMisIdRateInterface.h"

#include "TallinnNtupleProducer/CommonTools/interface/cmsException.h"       // get_human_line()
#include "TallinnNtupleProducer/CommonTools/interface/Era.h"                // Era
#include "TallinnNtupleProducer/EvtWeightTools/interface/lutAuxFunctions.h" // lutWrapperTH2
#include "TallinnNtupleProducer/Objects/interface/RecoLepton.h"             // RecoLepton

#include "TFile.h"                                                          // TFile

ChargeMisIdRateInterface::ChargeMisIdRateInterface(Era era,
                                                   const std::string & lep_mva_wp,
                                                   bool wTightCharge,
                                                   bool isDEBUG)
  : chargeMisId_(nullptr)
  , isDEBUG_(isDEBUG)
{
  const std::string path = ( lep_mva_wp == "hh_multilepton" ) ?
    "TallinnNtupleProducer/EvtWeightTools/data/ChargeFlipRate/ChargeFlipR_e_mva_hh_multilepton_%s_KBFI_2020Oct27_w%sTightCharge.root" :
    "TallinnNtupleProducer/EvtWeightTools/data/ChargeFlipRate/ChargeFlipR_e_ttH_%s_KBFI_2020Jan29_w%sTightCharge.root"
  ;
  chargeMisId_ = new lutWrapperTH2(
    inputFiles_,
    Form(path.data(), get_era(era).data(), wTightCharge ? "" : "o"),
    "chargeMisId",
    lut::kXptYabsEta
  );
}

ChargeMisIdRateInterface::~ChargeMisIdRateInterface()
{
  for(auto & kv: inputFiles_)
  {
    delete kv.second;
  }
  delete chargeMisId_;
}

double
ChargeMisIdRateInterface::get(const RecoLepton * const lepton) const
{
  const double chargeMisIdRate = ( lepton->is_electron() ) ? chargeMisId_->getSF(lepton->pt(), lepton->absEta()) : 0.;
  if(isDEBUG_)
  {
    std::cout << get_human_line(this, __func__, __LINE__)
      << "The charge misidentification rate of lepton ("
         "pT = " << lepton->pt() << ", |eta| = " << lepton->absEta() << ", PDG ID = " << lepton->pdgId()
      << ") is " << chargeMisIdRate << '\n';
    ;
  }
  return chargeMisIdRate;
}

double
ChargeMisIdRateInterface::get(const RecoLepton * const lepton_lead, const RecoLepton * const lepton_sublead) const
{
  return get(lepton_lead) + get(lepton_sublead);
}
