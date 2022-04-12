#ifndef TallinnNtupleProducer_Readers_TauESTool_h
#define TallinnNtupleProducer_Readers_TauESTool_h

#include "TallinnNtupleProducer/CommonTools/interface/Era.h"               // Era
#include "TallinnNtupleProducer/CommonTools/interface/sysUncertOptions.h"  // kHadTauPt_central

#include <map>                                                             // std::map
#include <string>                                                          // std::string

enum class TauID;

class TauESTool
{
 public:
  TauESTool(Era era,
            TauID tauID,
            int central_or_shift = kHadTauPt_central,
            bool debug = false);

  void
  set_central_or_shift(int central_or_shift);

  void
  set_tauID(TauID tauID);

  double
  getTES(double pt,
         int dm,
         int genmatch) const;

 private:
  std::string
  get_era_str() const;

  std::string
  get_tauId_str() const;

  void
  load_sf();

  void
  load_sf(const std::string & input_name,
          std::map<int, double> & sf,
          std::map<int, double> & sfErr);

  Era era_;
  TauID tauID_;
  int central_or_shift_;
  bool debug_;

  double lowPt_;
  double highPt_;

  std::map<int, double> sf_lowPt_;
  std::map<int, double> sfErr_lowPt_;
  std::map<int, double> sf_highPt_;
  std::map<int, double> sfErr_highPt_;
};

#endif // tthAnalysis_HiggsToTauTau_TauESTool_h
