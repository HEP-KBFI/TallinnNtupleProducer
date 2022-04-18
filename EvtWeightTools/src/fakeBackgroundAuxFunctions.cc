#include "TallinnNtupleProducer/EvtWeightTools/interface/fakeBackgroundAuxFunctions.h"

double
getWeight_1L(double prob_fake)
{
  const double p = prob_fake / (1. - prob_fake);
  const double weight = p;
  return weight;
}

double
getWeight_2L(double prob_fake_lead,    bool passesTight_lead,
             double prob_fake_sublead, bool passesTight_sublead)
{
  const double p1 = prob_fake_lead    / (1. - prob_fake_lead);
  const double p2 = prob_fake_sublead / (1. - prob_fake_sublead);
  double weight = 1.;
  if     (! passesTight_lead &&   passesTight_sublead) weight =  p1;
  else if(  passesTight_lead && ! passesTight_sublead) weight =  p2;
  else if(! passesTight_lead && ! passesTight_sublead) weight = -p1 * p2;
  return weight;
}

double
getWeight_3L(double prob_fake_lead,    bool passesTight_lead,
             double prob_fake_sublead, bool passesTight_sublead,
             double prob_fake_third,   bool passesTight_third)
{
  const double p1 = prob_fake_lead    / (1. - prob_fake_lead);
  const double p2 = prob_fake_sublead / (1. - prob_fake_sublead);
  const double p3 = prob_fake_third   / (1. - prob_fake_third);
  double weight = 1.;
  if     (! passesTight_lead &&   passesTight_sublead &&   passesTight_third) weight =  p1;
  else if(  passesTight_lead && ! passesTight_sublead &&   passesTight_third) weight =  p2;
  else if(  passesTight_lead &&   passesTight_sublead && ! passesTight_third) weight =  p3;
  else if(! passesTight_lead && ! passesTight_sublead &&   passesTight_third) weight = -p1 * p2;
  else if(! passesTight_lead &&   passesTight_sublead && ! passesTight_third) weight = -p1 * p3;
  else if(  passesTight_lead && ! passesTight_sublead && ! passesTight_third) weight = -p2 * p3;
  else if(! passesTight_lead && ! passesTight_sublead && ! passesTight_third) weight =  p1 * p2 * p3;
  return weight;
}

double
getWeight_4L(double prob_fake_lead,    bool passesTight_lead,
             double prob_fake_sublead, bool passesTight_sublead,
             double prob_fake_third,   bool passesTight_third,
             double prob_fake_fourth,  bool passesTight_fourth)
{
  const double p1 = prob_fake_lead    / (1. - prob_fake_lead);
  const double p2 = prob_fake_sublead / (1. - prob_fake_sublead);
  const double p3 = prob_fake_third   / (1. - prob_fake_third);
  const double p4 = prob_fake_fourth  / (1. - prob_fake_fourth);
  double weight = 1.;
  if     (! passesTight_lead &&   passesTight_sublead &&   passesTight_third &&   passesTight_fourth) weight =  p1;
  else if(  passesTight_lead && ! passesTight_sublead &&   passesTight_third &&   passesTight_fourth) weight =  p2;
  else if(  passesTight_lead &&   passesTight_sublead && ! passesTight_third &&   passesTight_fourth) weight =  p3;
  else if(  passesTight_lead &&   passesTight_sublead &&   passesTight_third && ! passesTight_fourth) weight =  p4;
  else if(! passesTight_lead && ! passesTight_sublead &&   passesTight_third &&   passesTight_fourth) weight = -p1 * p2;
  else if(! passesTight_lead &&   passesTight_sublead && ! passesTight_third &&   passesTight_fourth) weight = -p1 * p3;
  else if(! passesTight_lead &&   passesTight_sublead &&   passesTight_third && ! passesTight_fourth) weight = -p1 * p4;
  else if(  passesTight_lead && ! passesTight_sublead && ! passesTight_third &&   passesTight_fourth) weight = -p2 * p3;
  else if(  passesTight_lead && ! passesTight_sublead &&   passesTight_third && ! passesTight_fourth) weight = -p2 * p4;
  else if(  passesTight_lead &&   passesTight_sublead && ! passesTight_third && ! passesTight_fourth) weight = -p3 * p4;
  else if(! passesTight_lead && ! passesTight_sublead && ! passesTight_third &&   passesTight_fourth) weight =  p1 * p2 * p3;
  else if(! passesTight_lead && ! passesTight_sublead &&   passesTight_third && ! passesTight_fourth) weight =  p1 * p2 * p4;
  else if(! passesTight_lead &&   passesTight_sublead && ! passesTight_third && ! passesTight_fourth) weight =  p1 * p3 * p4;
  else if(  passesTight_lead && ! passesTight_sublead && ! passesTight_third && ! passesTight_fourth) weight =  p2 * p3 * p4;
  else if(! passesTight_lead && ! passesTight_sublead && ! passesTight_third && ! passesTight_fourth) weight = -p1 * p2 * p3 * p4;
  return weight;
}
