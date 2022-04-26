#ifndef TallinnNtupleProducer_Framework_leptonGenMatchingAuxFunctions_h
#define TallinnNtupleProducer_Framework_leptonGenMatchingAuxFunctions_h

#include <string> // std::string
#include <vector> // std::vector
#include <iosfwd> // std::ostream

// forward declarations
class RecoLepton;

enum { 
  kGen_LeptonUndefined1, 
  kGen_LeptonAll1, kGen_1l0g0t0j, kGen_0l1g0t0j, kGen_0l0g1t0j, kGen_0l0g0t1j 
};

enum {
  kGen_LeptonChargeFlipUndefined1, 
  kGen_LeptonChargeFlipAll1, kGen_1l1f0g0t0j, kGen_1l0f0g0t0j, kGen_0l0f1g0t0j, kGen_0l0f0g1t0j, kGen_0l0f0g0t1j,
};

enum { 
  kGen_LeptonUndefined2, kGen_LeptonAll2, 
  kGen_2l0g0t0j, 
  kGen_1l1g0t0j, kGen_1l0g1t0j, kGen_1l0g0t1j, 
  kGen_0l2g0t0j, kGen_0l1g1t0j, kGen_0l1g0t1j, kGen_0l0g2t0j, kGen_0l0g1t1j, kGen_0l0g0t2j 
};

enum {
  kGen_LeptonChargeFlipUndefined2, kGen_LeptonChargeFlipAll2,
  kGen_2l2f0g0t0j, kGen_2l1f0g0t0j, kGen_2l0f0g0t0j,
  kGen_1l1f1g0t0j, kGen_1l0f1g0t0j, kGen_1l0f0g1t0j, kGen_1l1f0g1t0j, kGen_1l0f0g0t1j, kGen_1l1f0g0t1j,
  kGen_0l0f2g0t0j, kGen_0l0f1g1t0j, kGen_0l0f1g0t1j, kGen_0l0f0g2t0j, kGen_0l0f0g1t1j, kGen_0l0f0g0t2j
};

enum { 
  kGen_LeptonUndefined3, kGen_LeptonAll3, 
  kGen_3l0g0t0j, 
  kGen_2l1g0t0j, kGen_2l0g1t0j, kGen_2l0g0t1j, 
  kGen_1l2g0t0j, kGen_1l1g1t0j, kGen_1l1g0t1j, kGen_1l0g2t0j, kGen_1l0g1t1j, kGen_1l0g0t2j, 
  kGen_0l3g0t0j, kGen_0l2g1t0j, kGen_0l2g0t1j, kGen_0l1g2t0j, kGen_0l1g1t1j, kGen_0l1g0t2j, kGen_0l0g3t0j, kGen_0l0g2t1j, kGen_0l0g1t2j, kGen_0l0g0t3j 
};

enum { 
  kGen_LeptonUndefined4, kGen_LeptonAll4, 
  kGen_4l0g0t0j, 
  kGen_3l1g0t0j, kGen_3l0g1t0j, kGen_3l0g0t1j, 
  kGen_2l2g0t0j, kGen_2l1g1t0j, kGen_2l1g0t1j, kGen_2l0g2t0j, kGen_2l0g1t1j, kGen_2l0g0t2j, 
  kGen_1l3g0t0j, kGen_1l2g1t0j, kGen_1l2g0t1j, kGen_1l1g2t0j, kGen_1l1g1t1j, kGen_1l1g0t2j, kGen_1l0g3t0j, kGen_1l0g2t1j, kGen_1l0g1t2j, kGen_1l0g0t3j, 
  kGen_0l4g0t0j, kGen_0l3g1t0j, kGen_0l3g0t1j, kGen_0l2g2t0j, kGen_0l2g1t1j, kGen_0l2g0t2j, kGen_0l1g3t0j, kGen_0l1g2t1j, kGen_0l1g1t2j, kGen_0l1g0t3j, kGen_0l0g4t0j, kGen_0l0g3t1j, kGen_0l0g2t2j, kGen_0l0g1t3j, kGen_0l0g0t4j
 };

struct leptonGenMatchEntry
{
  leptonGenMatchEntry(const std::string & name,
                      int idx,
                      int numGenMatchedLeptons,
                      int numGenMatchedPhotons,
                      int numGenMatchedHadTaus, 
                      int numGenMatchedJets);
  ~leptonGenMatchEntry() {}
  std::string name_;
  int idx_;
  int numGenMatchedLeptons_;
  int numGenMatchedPhotons_;
  int numGenMatchedHadTaus_;
  int numGenMatchedJets_;
};

struct leptonChargeFlipGenMatchEntry
{
  leptonChargeFlipGenMatchEntry(const std::string & name,
                                int idx,
                                int numGenMatchedLeptons,
                                int numChargeFlippedGenMatchedLeptons,
                                int numGenMatchedPhotons,
                                int numGenMatchedHadTaus,
                                int numGenMatchedJets);
  ~leptonChargeFlipGenMatchEntry() {}
  std::string name_;
  int idx_;
  int numGenMatchedLeptons_;
  int numChargeFlippedGenMatchedLeptons_;
  int numGenMatchedPhotons_;
  int numGenMatchedHadTaus_;
  int numGenMatchedJets_;
};

std::vector<leptonGenMatchEntry> getLeptonGenMatch_definitions_1lepton(bool apply_leptonGenMatching);
std::vector<leptonGenMatchEntry> getLeptonGenMatch_definitions_2lepton(bool apply_leptonGenMatching);
std::vector<leptonGenMatchEntry> getLeptonGenMatch_definitions_3lepton(bool apply_leptonGenMatching);
std::vector<leptonGenMatchEntry> getLeptonGenMatch_definitions_4lepton(bool apply_leptonGenMatching);
std::vector<leptonChargeFlipGenMatchEntry> getLeptonChargeFlipGenMatch_definitions_1lepton(bool apply_leptonGenMatching);
std::vector<leptonChargeFlipGenMatchEntry> getLeptonChargeFlipGenMatch_definitions_2lepton(bool apply_leptonGenMatching);

std::string
getLeptonGenMatch_string(const std::vector<leptonGenMatchEntry> & leptonGenMatch_definitions,
                         int leptonGenMatch_int);

std::string
getLeptonChargeFlipGenMatch_string(const std::vector<leptonChargeFlipGenMatchEntry> & leptonChargeFlipGenMatch_definitions,
                                   int leptonChargeFlipGenMatch_int);

int
getLeptonGenMatch_int(const std::vector<leptonGenMatchEntry> & leptonGenMatch_definitions,
                      const std::string & leptonGenMatch_string);

int
getLeptonChargeFlipGenMatch_int(const std::vector<leptonChargeFlipGenMatchEntry> & leptonChargeFlipGenMatch_definitions,
                                const std::string & leptonChargeFlipGenMatch_string);


void
countLeptonGenMatches(const RecoLepton * lepton,
                      int & numGenMatchedLeptons,
                      int & numGenMatchedPhotons,
                      int & numGenMatchedHadTaus,
                      int & numGenMatchedJets);

void
countLeptonChargeFlipGenMatches(const RecoLepton * lepton,
                                int & numGenMatchedLeptons,
                                int & numChargeFlippedGenMatchedLeptons,
                                int & numGenMatchedPhotons,
                                int & numGenMatchedHadTaus,
                                int & numGenMatchedJets);

const leptonGenMatchEntry &
getLeptonGenMatch(const std::vector<leptonGenMatchEntry> & leptonGenMatch_definitions,
                  const RecoLepton * lepton_lead,
                  const RecoLepton * lepton_sublead = nullptr,
                  const RecoLepton * lepton_third = nullptr,
                  const RecoLepton * lepton_fourth = nullptr);

const leptonChargeFlipGenMatchEntry &
getLeptonChargeFlipGenMatch(const std::vector<leptonChargeFlipGenMatchEntry> & leptonChargeFlipGenMatch_definitions,
                            const RecoLepton * lepton_lead,
                            const RecoLepton * lepton_sublead = nullptr,
                            const RecoLepton * lepton_third = nullptr,
                            const RecoLepton * lepton_fourth = nullptr);


std::ostream &
operator<<(std::ostream & stream,
           const leptonGenMatchEntry & leptonGenMatch_definition);

std::ostream &
operator<<(std::ostream & stream,
           const leptonChargeFlipGenMatchEntry & leptonChargeFlipGenMatch_definition);

std::ostream &
operator<<(std::ostream & stream,
           const std::vector<leptonGenMatchEntry> & leptonGenMatch_definitions);

std::ostream &
operator<<(std::ostream & stream,
           const std::vector<leptonChargeFlipGenMatchEntry> & leptonChargeFlipGenMatch_definitions);

#endif // TallinnNtupleProducer_Framework_leptonGenMatchingAuxFunctions_h
