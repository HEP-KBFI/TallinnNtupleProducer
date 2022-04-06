#ifndef TallinnNtupleProducer_Selectors_RecoHadTauCollectionSelector_h
#define TallinnNtupleProducer_Selectors_RecoHadTauCollectionSelector_h

#include "TallinnNtupleProducer/Selectors/interface/ParticleCollectionSelector.h" // ParticleCollectionSelector

enum class TauID;

template<typename T>
class RecoHadTauCollectionSelector : public ParticleCollectionSelector<RecoHadTau, T>
{
 public:
  explicit
  RecoHadTauCollectionSelector(Era era,
                               int index = -1,
                               bool debug = false)
    : ParticleCollectionSelector<RecoHadTau, T>(era, index, debug)
  {}
  ~RecoHadTauCollectionSelector() {}

  void
  set_min_pt(double min_pt)
  {
    this->selector_.set_min_pt(min_pt);
  }

  void
  set_max_absEta(double max_absEta)
  {
    this->selector_.set_max_absEta(max_absEta);
  }

  void
  set_min_id_mva(TauID tauId,
                 int min_id_mva)
  {
    this->selector_.set_min_id_mva(tauId, min_id_mva);
  }

  void
  set_min_raw_mva(TauID tauId,
                  double min_raw_mva)
  {
    this->selector_.set_min_raw_mva(tauId, min_raw_mva);
  }

  void
  set(const std::string & cut)
  {
    this->selector_.set(cut);
  }

  bool
  set_if_looser(const std::string & cut)
  {
    return this->selector_.set_if_looser(cut);
  }

  void
  set_min_antiElectron(int min_antiElectron)
  {
    this->selector_.set_min_antiElectron(min_antiElectron);
  }

  void
  set_min_antiMuon(int min_antiMuon)
  {
    this->selector_.set_min_antiMuon(min_antiMuon);
  }

  void
  disable_deeptau_lepton()
  {
    this->selector_.disable_deeptau_lepton();
  }
};

#endif // TallinnNtupleProducer_Selectors_RecoHadTauCollectionSelector_h
