#ifndef TallinnNtupleProducer_EvtWeightTools_Data_to_MC_CorrectionInterface_2016_h
#define TallinnNtupleProducer_EvtWeightTools_Data_to_MC_CorrectionInterface_2016_h

#include "TallinnNtupleProducer/EvtWeightTools/interface/Data_to_MC_CorrectionInterface_Base.h" // Data_to_MC_CorrectionInterface_Base

class Data_to_MC_CorrectionInterface_2016
  : public Data_to_MC_CorrectionInterface_Base
{
public:
  Data_to_MC_CorrectionInterface_2016(const edm::ParameterSet & cfg);
  ~Data_to_MC_CorrectionInterface_2016() override;

  //-----------------------------------------------------------------------------
  // data/MC correction for electron and muon trigger efficiency
  double
  getSF_leptonTriggerEff(TriggerSFsys central_or_shift) const override;
  //-----------------------------------------------------------------------------
};

#endif // TallinnNtupleProducer_EvtWeightTools_Data_to_MC_CorrectionInterface_2016_h
