#ifndef TallinnNtupleProducer_Selectors_RecoHadTauCollectionSelectorFakeable_h
#define TallinnNtupleProducer_Selectors_RecoHadTauCollectionSelectorFakeable_h

#include "TallinnNtupleProducer/Objects/interface/RecoHadTau.h"                     // RecoHadTau
#include "TallinnNtupleProducer/Selectors/interface/RecoHadTauCollectionSelector.h" // RecoHadTauCollectionSelector
#include "TallinnNtupleProducer/Selectors/interface/RecoHadTauSelectorBase.h"       // RecoHadTauSelectorBase

enum class Era;

class RecoHadTauSelectorFakeable : public RecoHadTauSelectorBase
{
 public:
  explicit
  RecoHadTauSelectorFakeable(Era era,
                             int index = -1,
                             bool debug = false,
                             bool set_selection_flags = true);
  ~RecoHadTauSelectorFakeable() {}

  bool operator()(const RecoHadTau & hadTau) const;

  friend class RecoHadTauCollectionSelector<RecoHadTauSelectorFakeable>;

 protected:
  void set_selection_flags(const RecoHadTau & hadTau) const;
};

typedef RecoHadTauCollectionSelector<RecoHadTauSelectorFakeable> RecoHadTauCollectionSelectorFakeable;

#endif // TallinnNtupleProducer_Selectors_RecoHadTauCollectionSelectorFakeable_h

