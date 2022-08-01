#include "TallinnNtupleProducer/EvtWeightTools/interface/Data_to_MC_CorrectionInterface_trigger_Base.h"

#include "TallinnNtupleProducer/CommonTools/interface/LocalFileInPath.h"                        // LocalFileInPath
#include "TallinnNtupleProducer/CommonTools/interface/leptonTypes.h"                            // getLeptonType()
#include "TallinnNtupleProducer/CommonTools/interface/Era.h"                                    // Era, get_era()
#include "TallinnNtupleProducer/Objects/interface/RecoHadTau.h"                                 // RecoHadTau
#include "TallinnNtupleProducer/Objects/interface/RecoLepton.h"                                 // RecoLepton
#include "TallinnNtupleProducer/CommonTools/interface/hadTauDefinitions.h"                      // get_tau_id_enum(), get_tau_id_wp_str()

#include "TString.h"                                                                            // Form()

#include <assert.h>                                                                             // assert()

// TODO ask Tau POG to allow underflow
const std::map<std::string, double> Data_to_MC_CorrectionInterface_trigger_Base::tau_trigger_ptThresholds_ = {
  { "ditau",    40. },
  { "etau",     25. },
  { "mutau",    25. },
  { "ditauvbf", 20. },
};

Data_to_MC_CorrectionInterface_trigger_Base::Data_to_MC_CorrectionInterface_trigger_Base(const edm::ParameterSet & cfg)
  : era_str_(cfg.getParameter<std::string>("era"))
  , era_(get_era(era_str_))
  , hadTauSelection_(cfg.getParameter<std::string>("hadTauSelection_againstJets"))
  , isDEBUG_(cfg.exists("isDEBUG") ? cfg.getParameter<bool>("isDEBUG") : false)
  , allowedDecayModes_({ 0, 1, 2, 10, 11 })
  , lepton_type_(-1)
  , lepton_pt_(0.)
  , lepton_eta_(0.)
  , hadTau1_genPdgId_(0)
  , hadTau1_pt_(0.)
  , hadTau1_eta_(0.)
  , hadTau1_phi_(0.)
  , hadTau1_decayMode_(0)
  , hadTau2_genPdgId_(0)
  , hadTau2_pt_(0.)
  , hadTau2_eta_(0.)
  , hadTau2_phi_(0.)
  , hadTau2_decayMode_(0)
  , hadTau3_genPdgId_(0)
  , hadTau3_pt_(0.)
  , hadTau3_eta_(0.)
  , hadTau3_phi_(0.)
  , hadTau3_decayMode_(0)
  , hadTau4_genPdgId_(0)
  , hadTau4_pt_(0.)
  , hadTau4_eta_(0.)
  , hadTau4_phi_(0.)
  , hadTau4_decayMode_(0)
  , tauId_(get_tau_id_enum(hadTauSelection_))
  , wp_str_(get_tau_id_wp_str(tauId_, get_tau_id_wp_int(hadTauSelection_)))
  , tau_tigger_cset_(nullptr)
  , sf_trigger_(nullptr)
{
  const std::string tauTriggerCorrectionSetFile = LocalFileInPath(Form("TallinnNtupleProducer/EvtWeightTools/data/correctionlib/tau/%s/tau.json.gz", era_str_.data())).fullPath();
  tau_tigger_cset_ = correction::CorrectionSet::from_file(tauTriggerCorrectionSetFile);
  sf_trigger_ = tau_tigger_cset_->at("tau_trigger");
}

void
Data_to_MC_CorrectionInterface_trigger_Base::setLeptons(const std::vector<const RecoLepton *> & leptons)
{
  if(! leptons.empty())
  {
    lepton_type_ = getLeptonType(leptons[0]->pdgId());
    lepton_pt_   = leptons[0]->pt();
    lepton_eta_  = leptons[0]->eta();
  }
}

void
Data_to_MC_CorrectionInterface_trigger_Base::setHadTaus(const std::vector<const RecoHadTau *>& hadTaus)
{
  if(! hadTaus.empty())
  {
    hadTau1_pt_ = hadTaus[0]->pt();
    hadTau1_eta_ = hadTaus[0]->eta();
    hadTau1_phi_ = hadTaus[0]->phi();
    hadTau1_decayMode_ = hadTaus[0]->decayMode();
  }

  if ( hadTaus.size() >1 )
  {
    hadTau2_pt_ = hadTaus[1]->pt();
    hadTau2_eta_ = hadTaus[1]->eta();
    hadTau2_phi_ = hadTaus[1]->phi();
    hadTau2_decayMode_ = hadTaus[1]->decayMode();
  }

  if ( hadTaus.size() >2 )
  {
    hadTau3_pt_ = hadTaus[2]->pt();
    hadTau3_eta_ = hadTaus[2]->eta();
    hadTau3_phi_ = hadTaus[2]->phi();
    hadTau3_decayMode_ = hadTaus[2]->decayMode();
  }
  
  if ( hadTaus.size() >3 )
  {
    hadTau4_pt_ = hadTaus[3]->pt();
    hadTau4_eta_ = hadTaus[3]->eta();
    hadTau4_phi_ = hadTaus[3]->phi();
    hadTau4_decayMode_ = hadTaus[3]->decayMode();
  }
}

double
Data_to_MC_CorrectionInterface_trigger_Base::tau_leg_efficiency(double pt,
                                                               int dm,
                                                               const std::string & trigger_type,
                                                               const std::string & wp,
                                                               const std::string & data_type,
                                                               const std::string & sys) const
{
  const double pt_threshold = tau_trigger_ptThresholds_.at(trigger_type);
  return pt >= pt_threshold ? sf_trigger_->evaluate({ pt, dm, trigger_type, wp, data_type, sys }) : 0.;
}

TauTriggerSFValues
Data_to_MC_CorrectionInterface_trigger_Base::tau_leg_efficiency(double pt,
                                                                int dm,
                                                                const std::string & trigger_type,
                                                                const std::string & wp,
                                                                const std::string & data_type) const
{
  const double nom = tau_leg_efficiency(pt, dm, trigger_type, wp, data_type, "nom");
  const double up = tau_leg_efficiency(pt, dm, trigger_type, wp, data_type, "up");
  const double down = tau_leg_efficiency(pt, dm, trigger_type, wp, data_type, "down");
  return { down, nom, up };
}
