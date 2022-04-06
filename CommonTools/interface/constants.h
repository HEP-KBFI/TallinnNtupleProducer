#ifndef TallinnNtupleProducer_CommonTools_constants_h
#define TallinnNtupleProducer_CommonTools_constants_h

//--- declare selection criteria for leptons and hadronic taus
enum { kLoose, kFakeable, kTight };

//--- declare constants
const double w_mass   = 80.385;
const double w_window = 10.;
const double z_mass   = 91.1876;
const double z_window = 10.;
const double met_coef =  0.6;
const double mht_coef =  0.4;

#endif // TallinnNtupleProducer_CommonTools_constants_h
