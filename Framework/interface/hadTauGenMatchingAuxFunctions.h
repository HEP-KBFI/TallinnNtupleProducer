#ifndef TallinnNtupleProducer_Framework_hadTauGenMatchingAuxFunctions_h
#define TallinnNtupleProducer_Framework_hadTauGenMatchingAuxFunctions_h

#include <vector> // std::vector
#include <string> // std::string
#include <iosfwd> // std::ostream

// forward declarations
class RecoHadTau;

enum {
  kGen_HadTauUndefined1, kGen_HadTauAll1,
  kGen_1t0e0m0j, 
  kGen_0t1e0m0j, kGen_0t0e1m0j, kGen_0t0e0m1j
};

enum {
  kGen_HadTauChargeFlipUndefined1, kGen_HadTauChargeFlipAll1,
  kGen_1t0e0m1f0j, kGen_1t0e0m0f0j, 
  kGen_0t1e0m1f0j, kGen_0t1e0m0f0j, kGen_0t0e1m1f0j, kGen_0t0e1m0f0j, kGen_0t0e0m0f1j
}; // this is the new enum, where generator-level matched hadronic tau decays (tau_h) are split into charge flip
   // and non-flip contributions (needed for ttH 1l+1tau channel; "1f" refers to reconstructed tau_h that are matched
   // to either an electron, muon, or tau_h of opposite charge on generator-level)

enum
{
  kGen_HadTauUndefined2, kGen_HadTauAll2,
  kGen_2t0e0m0j, 
  kGen_1t1e0m0j, kGen_1t0e1m0j, kGen_1t0e0m1j,
  kGen_0t2e0m0j, kGen_0t1e1m0j, kGen_0t0e2m0j, kGen_0t1e0m1j, kGen_0t0e1m1j, kGen_0t0e0m2j
};

enum {
  kGen_HadTauChargeFlipUndefined2, kGen_HadTauChargeFlipAll2,
  kGen_2t0e0m2f0j, kGen_2t0e0m1f0j, kGen_2t0e0m0f0j, 
  kGen_1t1e0m2f0j, kGen_1t1e0m1f0j, kGen_1t1e0m0f0j, 
  kGen_1t0e1m2f0j, kGen_1t0e1m1f0j, kGen_1t0e1m0f0j, 
  kGen_1t0e0m1f1j, kGen_1t0e0m0f1j,
  kGen_0t2e0m2f0j, kGen_0t2e0m1f0j, kGen_0t2e0m0f0j, 
  kGen_0t1e1m2f0j, kGen_0t1e1m1f0j, kGen_0t1e1m0f0j, 
  kGen_0t0e2m2f0j, kGen_0t0e2m1f0j, kGen_0t0e2m0f0j, 
  kGen_0t1e0m1f1j, kGen_0t1e0m0f1j, kGen_0t0e1m1f1j, kGen_0t0e1m0f1j, 
  kGen_0t0e0m0f2j
}; // this is the new enum, where generator-level matched hadronic tau decays (tau_h) are split into charge flip and
   // non-flip contributions

enum
{
  kGen_HadTauUndefined3, kGen_HadTauAll3,
  kGen_3t0e0m0j, 
  kGen_2t1e0m0j, kGen_2t0e1m0j, kGen_2t0e0m1j, 
  kGen_1t2e0m0j, kGen_1t1e1m0j, kGen_1t0e2m0j, kGen_1t1e0m1j, kGen_1t0e1m1j, kGen_1t0e0m2j, 
  kGen_0t3e0m0j, kGen_0t2e1m0j, kGen_0t1e2m0j, kGen_0t0e3m0j, 
  kGen_0t2e0m1j, kGen_0t1e1m1j, kGen_0t0e2m1j, kGen_0t1e0m2j, kGen_0t0e1m2j, kGen_0t0e0m3j
};

enum
{
  kGen_HadTauUndefined4, kGen_HadTauAll4,
  kGen_4t0e0m0j, 
  kGen_3t1e0m0j, kGen_3t0e1m0j, kGen_3t0e0m1j,
  kGen_2t2e0m0j, kGen_2t1e1m0j, kGen_2t0e2m0j, kGen_2t1e0m1j, kGen_2t0e1m1j, kGen_2t0e0m2j, 
  kGen_1t3e0m0j, kGen_1t2e1m0j, kGen_1t1e2m0j, kGen_1t0e3m0j, 
  kGen_1t2e0m1j, kGen_1t1e1m1j, kGen_1t0e2m1j, kGen_1t1e0m2j, kGen_1t0e1m2j, kGen_1t0e0m3j,
  kGen_0t4e0m0j, kGen_0t3e1m0j, kGen_0t2e2m0j, kGen_0t1e3m0j, kGen_0t0e4m0j, 
  kGen_0t3e0m1j, kGen_0t2e1m1j, kGen_0t1e2m1j, kGen_0t0e3m1j, 
  kGen_0t2e0m2j, kGen_0t1e1m2j, kGen_0t0e2m2j, kGen_0t1e0m3j, kGen_0t0e1m3j, kGen_0t0e0m4j
};

struct hadTauGenMatchEntry
{
  hadTauGenMatchEntry(const std::string & name,
                      int idx,
                      int numGenMatchedHadTaus,
                      int numGenMatchedElectrons,
                      int numGenMatchedMuons,
                      int numGenMatchedJets);
  ~hadTauGenMatchEntry() {}

  std::string name_;
  int idx_;
  int numGenMatchedHadTaus_;
  int numGenMatchedElectrons_;
  int numGenMatchedMuons_;
  int numGenMatchedJets_;
};

struct hadTauChargeFlipGenMatchEntry
{
  hadTauChargeFlipGenMatchEntry(const std::string & name,
                      int idx,
                      int numGenMatchedHadTaus,
                      int numGenMatchedElectrons,
                      int numGenMatchedMuons,
                      int numChargeFlippedGenMatchedHadTaus_or_Leptons,
                      int numGenMatchedJets);
  ~hadTauChargeFlipGenMatchEntry() {}

  std::string name_;
  int idx_;
  int numGenMatchedHadTaus_;
  int numGenMatchedElectrons_;
  int numGenMatchedMuons_;
  int numChargeFlippedGenMatchedHadTaus_or_Leptons_;
  int numGenMatchedJets_;
};

std::vector<hadTauGenMatchEntry>
getHadTauGenMatch_definitions_1tau(bool apply_hadTauGenMatching);
std::vector<hadTauGenMatchEntry>
getHadTauGenMatch_definitions_2tau(bool apply_hadTauGenMatching);
std::vector<hadTauGenMatchEntry>
getHadTauGenMatch_definitions_3tau(bool apply_hadTauGenMatching);
std::vector<hadTauGenMatchEntry>
getHadTauGenMatch_definitions_4tau(bool apply_hadTauGenMatching);

std::vector<hadTauChargeFlipGenMatchEntry>
getHadTauChargeFlipGenMatch_definitions_1tau(bool apply_hadTauGenMatching);
std::vector<hadTauChargeFlipGenMatchEntry>
getHadTauChargeFlipGenMatch_definitions_2tau(bool apply_hadTauGenMatching);

std::string
getHadTauGenMatch_string(const std::vector<hadTauGenMatchEntry> & hadTauGenMatch_definitions,
                         int hadTauGenMatch_int);

std::string
getHadTauChargeFlipGenMatch_string(const std::vector<hadTauChargeFlipGenMatchEntry> & hadTauChargeFlipGenMatch_definitions,
                                   int hadTauChargeFlipGenMatch_int);

int
getHadTauGenMatch_int(const std::vector<hadTauGenMatchEntry> & hadTauGenMatch_definitions,
                      const std::string& hadTauGenMatch_string);

int
getHadTauChargeFlipGenMatch_int(const std::vector<hadTauChargeFlipGenMatchEntry> & hadTauChargeFlipGenMatch_definitions,
                                const std::string& hadTauChargeFlipGenMatch_string);

void
countHadTauGenMatches(const RecoHadTau * hadTau,
                      int & numGenMatchedHadTaus,
                      int & numGenMatchedElectrons,
                      int & numGenMatchedMuons,
                      int & numGenMatchedJets);

void
countHadTauChargeFlipGenMatches(const RecoHadTau * hadTau,
                                int & numGenMatchedHadTaus,
                                int & numGenMatchedElectrons,
                                int & numGenMatchedMuons,
                                int & numChargeFlippedGenMatchedHadTaus_or_Leptons,
                                int & numGenMatchedJets);

const hadTauGenMatchEntry &
getHadTauGenMatch(const std::vector<hadTauGenMatchEntry> & hadTauGenMatch_definitions,
                  const RecoHadTau * hadTau_lead,
                  const RecoHadTau * hadTau_sublead = nullptr,
                  const RecoHadTau * hadTau_third = nullptr,
                  const RecoHadTau * hadTau_fourth = nullptr);

const hadTauChargeFlipGenMatchEntry &
getHadTauChargeFlipGenMatch(const std::vector<hadTauChargeFlipGenMatchEntry> & hadTauChargeFlipGenMatch_definitions,
                            const RecoHadTau * hadTau_lead,
                            const RecoHadTau * hadTau_sublead = nullptr,
                            const RecoHadTau * hadTau_third = nullptr,
                            const RecoHadTau * hadTau_fourth = nullptr);

std::ostream &
operator<<(std::ostream & stream,
           const hadTauGenMatchEntry & hadTauGenMatch_definition);

std::ostream &
operator<<(std::ostream & stream,
           const hadTauChargeFlipGenMatchEntry & hadTauChargeFlipGenMatch_definition);

std::ostream &
operator<<(std::ostream & stream,
           const std::vector<hadTauGenMatchEntry> & hadTauGenMatch_definitions);

std::ostream &
operator<<(std::ostream & stream,
           const std::vector<hadTauChargeFlipGenMatchEntry> & hadTauChargeFlipGenMatch_definitions);

#endif // TallinnNtupleProducer_Framework_hadTauGenMatchingAuxFunctions_h
