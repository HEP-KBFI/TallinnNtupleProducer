#ifndef TallinnNtupleProducer_EvtWeightTools_Data_to_MC_CorrectionInterface_2017_h
#define TallinnNtupleProducer_EvtWeightTools_Data_to_MC_CorrectionInterface_2017_h

#include "TallinnNtupleProducer/EvtWeightTools/interface/Data_to_MC_CorrectionInterface_Base.h" // Data_to_MC_CorrectionInterface_Base
#include "TallinnNtupleProducer/EvtWeightTools/interface/lutAuxFunctions.h"                     // vLutWrapperBase

class Data_to_MC_CorrectionInterface_2017 : public Data_to_MC_CorrectionInterface_Base
{
 public:
  Data_to_MC_CorrectionInterface_2017(const edm::ParameterSet & cfg);
  ~Data_to_MC_CorrectionInterface_2017() override;

  //-----------------------------------------------------------------------------
  // data/MC correction for electron and muon trigger efficiency
  double
  getSF_leptonTriggerEff(TriggerSFsys central_or_shift) const override;
  //-----------------------------------------------------------------------------

 protected:
  // data/MC corrections for efficiencies of single lepton triggers in 2016 data
  vLutWrapperBase effTrigger_1e_data_;
  vLutWrapperBase effTrigger_1e_mc_;
  vLutWrapperBase effTrigger_1m_data_;
  vLutWrapperBase effTrigger_1m_mc_;
};

#endif // TallinnNtupleProducer_EvtWeightTools_Data_to_MC_CorrectionInterface_2017_h
