#ifndef TallinnNtupleProducer_Selectors_RecoHadTauCollectionSelectorLoose_h
#define TallinnNtupleProducer_Selectors_RecoHadTauCollectionSelectorLoose_h

#include "TallinnNtupleProducer/Objects/interface/RecoHadTau.h"                     // RecoHadTau
#include "TallinnNtupleProducer/Selectors/interface/RecoHadTauCollectionSelector.h" // RecoHadTauCollectionSelector
#include "TallinnNtupleProducer/Selectors/interface/RecoHadTauSelectorBase.h"       // RecoHadTauSelectorBase

enum class Era;

class RecoHadTauSelectorLoose : public RecoHadTauSelectorBase
{
 public:
  explicit
  RecoHadTauSelectorLoose(Era era,
                          int index = -1,
                          bool debug = false,
                          bool set_selection_flags = true);
  ~RecoHadTauSelectorLoose() {}

  bool
  set_if_looser(const std::string & cut);

  void
  set(const std::string & cut);

  bool operator()(const RecoHadTau & hadTau) const;

  friend class RecoHadTauCollectionSelector<RecoHadTauSelectorLoose>;

 protected:
  void set_selection_flags(const RecoHadTau & hadTau) const;
  void reset_cuts();
};

typedef RecoHadTauCollectionSelector<RecoHadTauSelectorLoose> RecoHadTauCollectionSelectorLoose;

#endif // TallinnNtupleProducer_Selectors_RecoHadTauCollectionSelectorLoose_h

