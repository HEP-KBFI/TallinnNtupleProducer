#include "TallinnNtupleProducer/EvtWeightTools/interface/Data_to_MC_CorrectionInterface_trigger_Base.h"

#include "TallinnNtupleProducer/CommonTools/interface/leptonTypes.h"                            // getLeptonType()
#include "TallinnNtupleProducer/CommonTools/interface/cmsException.h"                           // get_human_line()
#include "TallinnNtupleProducer/CommonTools/interface/Era.h"                                    // Era, get_era()
#include "TallinnNtupleProducer/Objects/interface/RecoHadTau.h"                                 // RecoHadTau
#include "TallinnNtupleProducer/Objects/interface/RecoLepton.h"                               // RecoLepton
#include "TallinnNtupleProducer/CommonTools/interface/hadTauDefinitions.h"

#include <assert.h>                                                                             // assert()

Data_to_MC_CorrectionInterface_trigger_Base::Data_to_MC_CorrectionInterface_trigger_Base(const edm::ParameterSet & cfg)
  : era_str_(cfg.getParameter<std::string>("era"))
  , era_(get_era(era_str_))
  , hadTauSelection_(cfg.getParameter<std::string>("hadTauSelection"))
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
{}

Data_to_MC_CorrectionInterface_trigger_Base::~Data_to_MC_CorrectionInterface_trigger_Base()
{}

void
Data_to_MC_CorrectionInterface_trigger_Base::setLepton(const RecoLepton * const lepton)
{
  lepton_type_ = getLeptonType(lepton->pdgId());
  lepton_pt_   = lepton->pt();
  lepton_eta_  = lepton->eta();
}

void
Data_to_MC_CorrectionInterface_trigger_Base::setHadTaus(const std::vector<const RecoHadTau *>& hadTaus)
{
  assert( !hadTaus.empty() );
  hadTau1_pt_ = hadTaus[0]->pt();
  hadTau1_eta_ = hadTaus[0]->eta();
  hadTau1_phi_ = hadTaus[0]->phi();
  hadTau1_decayMode_ = hadTaus[0]->decayMode();

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
