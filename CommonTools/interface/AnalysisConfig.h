#ifndef TallinnNtupleProducer_CommonTools_AnalysisConfig_h
#define TallinnNtupleProducer_CommonTools_AnalysisConfig_h

/** \class AnalysisConfig
 *
 * Parse process name and determine type of sample that is being processed.
 *
 * \author Christian Veelken, Tallinn
 *
 */

#include "FWCore/ParameterSet/interface/ParameterSet.h" // edm::ParameterSet

#include <vector>
#include <string>

// forward declarations
enum class TopPtRwgtOption;

class AnalysisConfig
{
 public:
  AnalysisConfig(const std::string & analysis, const edm::ParameterSet & cfg);
  virtual ~AnalysisConfig();

  const std::string & analysis() const;
  const std::string & process() const;

  virtual bool isData() const;

  virtual bool isMC() const;
  virtual bool isMC_DY() const;
  virtual bool isMC_WJets() const;
  virtual bool isMC_WZ() const;
  virtual bool isMC_ZZ() const;
  virtual bool isMC_EWK() const;
  virtual bool isMC_QCD() const;
  virtual bool isMC_tH() const;
  virtual bool isMC_ttH() const;
  virtual bool isMC_ggH() const;
  virtual bool isMC_qqH() const;
  virtual bool isMC_VH() const;
  virtual bool isMC_WH() const;
  virtual bool isMC_ZH() const;
  virtual bool isMC_H() const;                 // single Higgs boson production (either tH, ttH, ggH, qqH, VH, ttZH, or ttWH)
  virtual bool isMC_HH_resonant() const;       // resonant HH production (either spin 0 or spin 2)
  virtual bool isMC_HH_resonant_spin0() const; // resonant HH production (spin 0 only)
  virtual bool isMC_HH_resonant_spin2() const; // resonant HH production (spin 2 only)
  virtual bool isMC_HH_nonresonant() const;    // non-resonant HH production
  virtual bool isMC_HH() const;                // resonant or non-resonant HH production

  virtual std::vector<std::string> get_decayModes_H() const;

  virtual double get_HH_resonant_mass() const;

  virtual bool isHH_rwgt_allowed() const;

  virtual TopPtRwgtOption apply_topPtReweighting() const;

 protected:
  std::string analysis_string_;
  std::string process_string_;

  bool isData_;

  bool isMC_;
  bool isMC_DY_;
  bool isMC_WJets_;
  bool isMC_WZ_;
  bool isMC_ZZ_;
  bool isMC_EWK_;
  bool isMC_QCD_;
  bool isMC_tH_;
  bool isMC_ttH_;
  bool isMC_ggH_;
  bool isMC_qqH_;
  bool isMC_VH_;
  bool isMC_WH_;
  bool isMC_ZH_;
  bool isMC_H_;
  bool isMC_HH_resonant_spin0_;
  bool isMC_HH_resonant_spin2_;
  bool isMC_HH_resonant_;
  bool isMC_HH_nonresonant_;
  bool isMC_HH_;

  std::vector<std::string> decayModes_H_;

  double mass_HH_resonant_;

  bool isHH_rwgt_allowed_;

  TopPtRwgtOption apply_topPtReweighting_;
};

#endif // TallinnNtupleProducer_Framework_AnalysisConfig_h
