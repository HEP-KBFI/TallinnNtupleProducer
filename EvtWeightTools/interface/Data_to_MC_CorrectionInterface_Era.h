#ifndef TallinnNtupleProducer_EvtWeightTools_Data_to_MC_CorrectionInterface_Era_h
#define TallinnNtupleProducer_EvtWeightTools_Data_to_MC_CorrectionInterface_Era_h

#include "TallinnNtupleProducer/EvtWeightTools/interface/Data_to_MC_CorrectionInterface_Base.h" // Data_to_MC_CorrectionInterface_Base
#include "TallinnNtupleProducer/EvtWeightTools/interface/lutAuxFunctions.h"                     // vLutWrapperBase

class Data_to_MC_CorrectionInterface_Era : public Data_to_MC_CorrectionInterface_Base
{
public:
  Data_to_MC_CorrectionInterface_Era(const edm::ParameterSet & cfg);
  ~Data_to_MC_CorrectionInterface_Era() override;

  //-----------------------------------------------------------------------------
  // data/MC correction for electron and muon trigger efficiency
  double
  getSF_leptonTriggerEff(TriggerSFsys central_or_shift) const override;
  //-----------------------------------------------------------------------------

protected:
  // data/MC corrections for efficiencies of single lepton triggers in Era data
  std::string era_string_;
  vLutWrapperBase effTrigger_1e_data_;
  vLutWrapperBase effTrigger_1e_mc_;
  vLutWrapperBase effTrigger_1m_data_;
  vLutWrapperBase effTrigger_1m_mc_;
};

#endif // tthAnalysis_HiggsToTauTau_data_to_MC_corrections_Era_h
