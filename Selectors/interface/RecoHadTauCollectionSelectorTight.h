#ifndef TallinnNtupleProducer_Selectors_RecoHadTauCollectionSelectorTight_h
#define TallinnNtupleProducer_Selectors_RecoHadTauCollectionSelectorTight_h

#include "TallinnNtupleProducer/Objects/interface/RecoHadTau.h"                     // RecoHadTau
#include "TallinnNtupleProducer/Selectors/interface/RecoHadTauCollectionSelector.h" // RecoHadTauCollectionSelector
#include "TallinnNtupleProducer/Selectors/interface/RecoHadTauSelectorBase.h"       // RecoHadTauSelectorBase

enum class Era;

class RecoHadTauSelectorTight : public RecoHadTauSelectorBase
{
 public:
  explicit
  RecoHadTauSelectorTight(Era era,
                          int index = -1,
                          bool debug = false,
                          bool set_selection_flags = true);
  ~RecoHadTauSelectorTight() {}

  bool
  set_if_looser(const std::string & cut);

  bool operator()(const RecoHadTau & hadTau) const;

  friend class RecoHadTauCollectionSelector<RecoHadTauSelectorTight>;

 protected:
  void set_selection_flags(const RecoHadTau & hadTau) const;
};

typedef RecoHadTauCollectionSelector<RecoHadTauSelectorTight> RecoHadTauCollectionSelectorTight;

#endif // TallinnNtupleProducer_Selectors_RecoHadTauCollectionSelectorTight_h

