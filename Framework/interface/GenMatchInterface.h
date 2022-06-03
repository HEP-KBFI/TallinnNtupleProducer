#ifndef TallinnNtupleProducer_Framework_GenMatchInterface_h
#define TallinnNtupleProducer_Framework_GenMatchInterface_h

#include <string> // std::string
#include <vector> // std::vector

// forward declarations
class RecoLepton;
class RecoHadTau;

class GenMatchEntry
{
 public:
  GenMatchEntry(const std::string& name, int idx)
    : name_(name)
    , idx_(idx)
  {}
  ~GenMatchEntry() {}

  const std::string& getName() const { return name_; }
  int getIdx() const { return idx_; }

 protected:
  std::string name_;
  int idx_;
};

class GenMatchInterface
{
 public:
  GenMatchInterface(unsigned numLeptons, bool apply_leptonGenMatching, bool useFlips, unsigned numHadTaus, bool apply_hadTauGenMatching, bool useFlipsHadTau = false);
  GenMatchInterface(unsigned numLeptons, bool apply_leptonGenMatching, bool useFlips);
  GenMatchInterface(unsigned numHadTaus, bool apply_hadTauGenMatching);
  ~GenMatchInterface();

  std::vector<const GenMatchEntry*> getGenMatchDefinitions() const;
  
  std::vector<const GenMatchEntry*> getGenMatch(const std::vector<const RecoLepton*>& selLeptons, const std::vector<const RecoHadTau*>& selHadTaus);
  std::vector<const GenMatchEntry*> getGenMatch(const std::vector<const RecoLepton*>& selLeptons);
  std::vector<const GenMatchEntry*> getGenMatch(const std::vector<const RecoHadTau*>& selHadTaus);

 protected:
  unsigned numLeptons_;
  bool apply_leptonGenMatching_;
  bool useFlips_;
  bool useFlipsHadTau_;
  unsigned numHadTaus_;
  bool apply_hadTauGenMatching_;
  bool useGenTau_and_FakeTau_;

  void initialize();

  void addGenMatchDefinition(const std::string& name);
  int idx_;

  std::vector<const GenMatchEntry*> genMatchDefinitions_;
  const GenMatchEntry* genMatchDefinition_fakes_;
  const GenMatchEntry* genMatchDefinition_flips_;
  const GenMatchEntry* genMatchDefinition_conversions_;
  const GenMatchEntry* genMatchDefinition_nonfakes_;
  const GenMatchEntry* genMatchDefinition_gentau_;
  const GenMatchEntry* genMatchDefinition_faketau_;
};

#endif // TallinnNtupleProducer_Framework_GenMatchInterface_h

