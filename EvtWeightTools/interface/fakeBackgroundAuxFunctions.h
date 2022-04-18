#ifndef TallinnNtupleProducer_EvtWeightTools_fakeBackgroundAuxFunctions_h
#define TallinnNtupleProducer_EvtWeightTools_fakeBackgroundAuxFunctions_h

double
getWeight_1L(double prob_fake);

double
getWeight_2L(double prob_fake_lead,    bool passesTight_lead,
             double prob_fake_sublead, bool passesTight_sublead);

double
getWeight_3L(double prob_fake_lead,    bool passesTight_lead,
             double prob_fake_sublead, bool passesTight_sublead,
             double prob_fake_third,   bool passesTight_third);

double
getWeight_4L(double prob_fake_lead,    bool passesTight_lead,
             double prob_fake_sublead, bool passesTight_sublead,
             double prob_fake_third,   bool passesTight_third,
             double prob_fake_fourth,  bool passesTight_fourth);

#endif // TallinnNtupleProducer_EvtWeightTools_fakeBackgroundAuxFunctions_h
