#include "TallinnNtupleProducer/Framework/interface/hadTauGenMatchingAuxFunctions.h"

#include "TallinnNtupleProducer/CommonTools/interface/cmsException.h" // cmsException()
#include "TallinnNtupleProducer/Objects/interface/RecoHadTau.h"       // RecoHadTau

#include <iostream> // std::ostream

hadTauGenMatchEntry::hadTauGenMatchEntry(const std::string & name,
                                         int idx,
                                         int numGenMatchedHadTaus,
                                         int numGenMatchedElectrons,
                                         int numGenMatchedMuons,
                                         int numGenMatchedJets)
  : name_(name)
  , idx_(idx)
  , numGenMatchedHadTaus_(numGenMatchedHadTaus)
  , numGenMatchedElectrons_(numGenMatchedElectrons)
  , numGenMatchedMuons_(numGenMatchedMuons)
  , numGenMatchedJets_(numGenMatchedJets)
{}

hadTauChargeFlipGenMatchEntry::hadTauChargeFlipGenMatchEntry(const std::string & name,
                                                             int idx,
                                                             int numGenMatchedHadTaus,
                                                             int numGenMatchedElectrons,
                                                             int numGenMatchedMuons,
                                                             int numChargeFlippedGenMatchedHadTaus_or_Leptons,
                                                             int numGenMatchedJets)
  : name_(name)
  , idx_(idx)
  , numGenMatchedHadTaus_(numGenMatchedHadTaus)
  , numGenMatchedElectrons_(numGenMatchedElectrons)
  , numGenMatchedMuons_(numGenMatchedMuons)
  , numChargeFlippedGenMatchedHadTaus_or_Leptons_(numChargeFlippedGenMatchedHadTaus_or_Leptons)
  , numGenMatchedJets_(numGenMatchedJets)
{}

std::vector<hadTauGenMatchEntry>
getHadTauGenMatch_definitions_1tau(bool apply_hadTauGenMatching)
{
  std::vector<hadTauGenMatchEntry> hadTauGenMatch_definitions;
  if(apply_hadTauGenMatching)
  {
    hadTauGenMatch_definitions = {
      { "1t0e0m0j", kGen_1t0e0m0j, 1, 0, 0, 0 },
      { "0t1e0m0j", kGen_0t1e0m0j, 0, 1, 0, 0 },
      { "0t0e1m0j", kGen_0t0e1m0j, 0, 0, 1, 0 },
      { "0t0e0m1j", kGen_0t0e0m1j, 0, 0, 0, 1 }
    };
  }
  else
  {
    hadTauGenMatch_definitions = { { "all", kGen_HadTauAll1, -1, -1, -1, -1 } };
  }
  return hadTauGenMatch_definitions;
}

std::vector<hadTauChargeFlipGenMatchEntry>
getHadTauChargeFlipGenMatch_definitions_1tau(bool apply_hadTauGenMatching)
{
  std::vector<hadTauChargeFlipGenMatchEntry> hadTauChargeFlipGenMatch_definitions;
  if(apply_hadTauGenMatching)
  {
    hadTauChargeFlipGenMatch_definitions = {
      { "1t0e0m1f0j", kGen_1t0e0m1f0j, 1, 0, 0, 1, 0 },
      { "1t0e0m0f0j", kGen_1t0e0m0f0j, 1, 0, 0, 0, 0 },
      { "0t1e0m1f0j", kGen_0t1e0m1f0j, 0, 1, 0, 1, 0 },
      { "0t1e0m0f0j", kGen_0t1e0m0f0j, 0, 1, 0, 0, 0 },
      { "0t0e1m1f0j", kGen_0t0e1m1f0j, 0, 0, 1, 1, 0 },
      { "0t0e1m0f0j", kGen_0t0e1m0f0j, 0, 0, 1, 0, 0 },
      { "0t0e0m0f1j", kGen_0t0e0m0f1j, 0, 0, 0, 0, 1 }
    };
  }
  else
  {
    hadTauChargeFlipGenMatch_definitions = { { "all", kGen_HadTauChargeFlipAll1, -1, -1, -1, -1, -1 } };
  }
  return hadTauChargeFlipGenMatch_definitions;
}

std::vector<hadTauGenMatchEntry>
getHadTauGenMatch_definitions_2tau(bool apply_hadTauGenMatching)
{
  std::vector<hadTauGenMatchEntry> hadTauGenMatch_definitions;
  if(apply_hadTauGenMatching)
  {
    hadTauGenMatch_definitions = {
      { "2t0e0m0j", kGen_2t0e0m0j, 2, 0, 0, 0 },
      { "1t1e0m0j", kGen_1t1e0m0j, 1, 1, 0, 0 },
      { "1t0e1m0j", kGen_1t0e1m0j, 1, 0, 1, 0 },
      { "1t0e0m1j", kGen_1t0e0m1j, 1, 0, 0, 1 },
      { "0t2e0m0j", kGen_0t2e0m0j, 0, 2, 0, 0 },
      { "0t1e1m0j", kGen_0t1e1m0j, 0, 1, 1, 0 },
      { "0t1e0m1j", kGen_0t1e0m1j, 0, 1, 0, 1 },
      { "0t0e2m0j", kGen_0t0e2m0j, 0, 0, 2, 0 },
      { "0t0e1m1j", kGen_0t0e1m1j, 0, 0, 1, 1 },
      { "0t0e0m2j", kGen_0t0e0m2j, 0, 0, 0, 2 }
    };
  }
  else
  {
    hadTauGenMatch_definitions = { { "all", kGen_HadTauAll2, -1, -1, -1, -1 } };
  }
  return hadTauGenMatch_definitions;
}

std::vector<hadTauChargeFlipGenMatchEntry>
getHadTauChargeFlipGenMatch_definitions_2tau(bool apply_hadTauGenMatching)
{
  std::vector<hadTauChargeFlipGenMatchEntry> hadTauChargeFlipGenMatch_definitions;
  if(apply_hadTauGenMatching)
  {
    hadTauChargeFlipGenMatch_definitions = {
      { "2t0e0m2f0j", kGen_2t0e0m2f0j, 2, 0, 0, 2, 0 },
      { "2t0e0m1f0j", kGen_2t0e0m1f0j, 2, 0, 0, 1, 0 },
      { "2t0e0m0f0j", kGen_2t0e0m0f0j, 2, 0, 0, 0, 0 },
      { "1t1e0m2f0j", kGen_1t1e0m2f0j, 1, 1, 0, 2, 0 },
      { "1t1e0m1f0j", kGen_1t1e0m1f0j, 1, 1, 0, 1, 0 },
      { "1t1e0m0f0j", kGen_1t1e0m0f0j, 1, 1, 0, 0, 0 },
      { "1t0e1m2f0j", kGen_1t0e1m2f0j, 1, 0, 1, 2, 0 },
      { "1t0e1m1f0j", kGen_1t0e1m1f0j, 1, 0, 1, 1, 0 },
      { "1t0e1m0f0j", kGen_1t0e1m0f0j, 1, 0, 1, 0, 0 },
      { "1t0e0m1f1j", kGen_1t0e0m1f1j, 1, 0, 0, 1, 1 },
      { "1t0e0m0f1j", kGen_1t0e0m0f1j, 1, 0, 0, 0, 1 },
      { "0t2e0m2f0j", kGen_0t2e0m2f0j, 0, 2, 0, 2, 0 },
      { "0t2e0m1f0j", kGen_0t2e0m1f0j, 0, 2, 0, 1, 0 },
      { "0t2e0m0f0j", kGen_0t2e0m0f0j, 0, 2, 0, 0, 0 },
      { "0t1e1m2f0j", kGen_0t1e1m2f0j, 0, 1, 1, 2, 0 },
      { "0t1e1m1f0j", kGen_0t1e1m1f0j, 0, 1, 1, 1, 0 },
      { "0t1e1m0f0j", kGen_0t1e1m0f0j, 0, 1, 1, 0, 0 },
      { "0t1e0m1f1j", kGen_0t1e0m1f1j, 0, 1, 0, 1, 1 },
      { "0t1e0m0f1j", kGen_0t1e0m0f1j, 0, 1, 0, 0, 1 },
      { "0t0e2m2f0j", kGen_0t0e2m2f0j, 0, 0, 2, 2, 0 },
      { "0t0e2m1f0j", kGen_0t0e2m1f0j, 0, 0, 2, 1, 0 },
      { "0t0e2m0f0j", kGen_0t0e2m0f0j, 0, 0, 2, 0, 0 },
      { "0t0e1m1f1j", kGen_0t0e1m1f1j, 0, 0, 1, 1, 1 },
      { "0t0e1m0f1j", kGen_0t0e1m0f1j, 0, 0, 1, 0, 1 },
      { "0t0e0m0f2j", kGen_0t0e0m0f2j, 0, 0, 0, 0, 2 }
    };
  }
  else
  {
    hadTauChargeFlipGenMatch_definitions = { { "all", kGen_HadTauChargeFlipAll2, -1, -1, -1, -1, -1 } };
  }
  return hadTauChargeFlipGenMatch_definitions;
}

std::vector<hadTauGenMatchEntry>
getHadTauGenMatch_definitions_3tau(bool apply_hadTauGenMatching)
{
  std::vector<hadTauGenMatchEntry> hadTauGenMatch_definitions;
  if(apply_hadTauGenMatching)
  {
    hadTauGenMatch_definitions = {
      { "3t0e0m0j", kGen_3t0e0m0j, 3, 0, 0, 0 },
      { "2t1e0m0j", kGen_2t1e0m0j, 2, 1, 0, 0 },
      { "2t0e1m0j", kGen_2t0e1m0j, 2, 0, 1, 0 },
      { "2t0e0m1j", kGen_2t0e0m1j, 2, 0, 0, 1 },
      { "1t2e0m0j", kGen_1t2e0m0j, 1, 2, 0, 0 },
      { "1t1e1m0j", kGen_1t1e1m0j, 1, 1, 1, 0 },
      { "1t1e0m1j", kGen_1t1e0m1j, 1, 1, 0, 1 },
      { "1t0e2m0j", kGen_1t0e2m0j, 1, 0, 2, 0 },
      { "1t0e1m1j", kGen_1t0e1m1j, 1, 0, 1, 1 },
      { "1t0e0m2j", kGen_1t0e0m2j, 1, 0, 0, 2 },
      { "0t3e0m0j", kGen_0t3e0m0j, 0, 3, 0, 0 },
      { "0t2e1m0j", kGen_0t2e1m0j, 0, 2, 1, 0 },
      { "0t2e0m1j", kGen_0t2e0m1j, 0, 2, 0, 1 },
      { "0t1e2m0j", kGen_0t1e2m0j, 0, 1, 2, 0 },
      { "0t1e1m1j", kGen_0t1e1m1j, 0, 1, 1, 1 },
      { "0t1e0m2j", kGen_0t1e0m2j, 0, 1, 0, 2 },
      { "0t0e3m0j", kGen_0t0e3m0j, 0, 0, 3, 0 },
      { "0t0e2m1j", kGen_0t0e2m1j, 0, 0, 2, 1 },
      { "0t0e1m2j", kGen_0t0e1m2j, 0, 0, 1, 2 },
      { "0t0e0m3j", kGen_0t0e0m3j, 0, 0, 0, 3 }
    };
  }
  else
  {
    hadTauGenMatch_definitions = { { "all", kGen_HadTauAll3, -1, -1, -1, -1 } };
  }
  return hadTauGenMatch_definitions;
}

std::vector<hadTauGenMatchEntry>
getHadTauGenMatch_definitions_4tau(bool apply_hadTauGenMatching)
{
  std::vector<hadTauGenMatchEntry> hadTauGenMatch_definitions;
  if(apply_hadTauGenMatching)
  {
    hadTauGenMatch_definitions = {
      { "4t0e0m0j", kGen_4t0e0m0j, 4, 0, 0, 0 },
      { "3t1e0m0j", kGen_3t1e0m0j, 3, 1, 0, 0 },
      { "3t0e1m0j", kGen_3t0e1m0j, 3, 0, 1, 0 },
      { "3t0e0m1j", kGen_3t0e0m1j, 3, 0, 0, 1 },
      { "2t2e0m0j", kGen_2t2e0m0j, 2, 2, 0, 0 },
      { "2t1e1m0j", kGen_2t1e1m0j, 2, 1, 1, 0 },
      { "2t1e0m1j", kGen_2t1e0m1j, 2, 1, 0, 1 },
      { "2t0e2m0j", kGen_2t0e2m0j, 2, 0, 2, 0 },
      { "2t0e1m1j", kGen_2t0e1m1j, 2, 0, 1, 1 },
      { "2t0e0m2j", kGen_2t0e0m2j, 2, 0, 0, 2 },
      { "1t3e0m0j", kGen_1t3e0m0j, 1, 3, 0, 0 },
      { "1t2e1m0j", kGen_1t2e1m0j, 1, 2, 1, 0 },
      { "1t2e0m1j", kGen_1t2e0m1j, 1, 2, 0, 1 },
      { "1t1e2m0j", kGen_1t1e2m0j, 1, 1, 2, 0 },
      { "1t1e1m1j", kGen_1t1e1m1j, 1, 1, 1, 1 },
      { "1t1e0m2j", kGen_1t1e0m2j, 1, 1, 0, 2 },
      { "1t0e3m0j", kGen_1t0e3m0j, 1, 0, 3, 0 },
      { "1t0e2m1j", kGen_1t0e2m1j, 1, 0, 2, 1 },
      { "1t0e1m2j", kGen_1t0e1m2j, 1, 0, 1, 2 },
      { "1t0e0m3j", kGen_1t0e0m3j, 1, 0, 0, 3 },
      { "0t4e0m0j", kGen_0t4e0m0j, 0, 4, 0, 0 },
      { "0t3e1m0j", kGen_0t3e1m0j, 0, 3, 1, 0 }, 
      { "0t2e2m0j", kGen_0t2e2m0j, 0, 2, 2, 0 }, 
      { "0t1e3m0j", kGen_0t1e3m0j, 0, 1, 3, 0 }, 
      { "0t0e4m0j", kGen_0t0e4m0j, 0, 0, 4, 0 }, 
      { "0t3e0m1j", kGen_0t3e0m1j, 0, 3, 0, 1 },
      { "0t2e1m1j", kGen_0t2e1m1j, 0, 2, 1, 1 }, 
      { "0t1e2m1j", kGen_0t1e2m1j, 0, 1, 2, 1 }, 
      { "0t0e3m1j", kGen_0t0e3m1j, 0, 0, 3, 1 }, 
      { "0t2e0m2j", kGen_0t2e0m2j, 0, 2, 0, 2 }, 
      { "0t1e1m2j", kGen_0t1e1m2j, 0, 1, 1, 2 }, 
      { "0t0e2m2j", kGen_0t0e2m2j, 0, 0, 2, 2 }, 
      { "0t1e0m3j", kGen_0t1e0m3j, 0, 1, 0, 3 }, 
      { "0t0e1m3j", kGen_0t0e1m3j, 0, 0, 1, 3 }, 
      { "0t0e0m4j", kGen_0t0e0m4j, 0, 0, 0, 4 },
    };
  }
  else
  {
    hadTauGenMatch_definitions = { { "all", kGen_HadTauAll4, -1, -1, -1, -1 } };
  }
  return hadTauGenMatch_definitions;
}

std::string
getHadTauGenMatch_string(const std::vector<hadTauGenMatchEntry> & hadTauGenMatch_definitions,
                         int hadTauGenMatch_int)
{
  const hadTauGenMatchEntry * hadTauGenMatch = nullptr;
  for(const hadTauGenMatchEntry & hadTauGenMatch_definition: hadTauGenMatch_definitions)
  {
    if(hadTauGenMatch_definition.idx_ == hadTauGenMatch_int)
    {
      hadTauGenMatch = &hadTauGenMatch_definition;
    }
  }
  if(! hadTauGenMatch)
  {
    throw cmsException(__func__, __LINE__)
      << "Invalid parameter 'hadTauGenMatch_int' = " << hadTauGenMatch_int;
  }
  return hadTauGenMatch->name_;
}

std::string
getHadTauChargeFlipGenMatch_string(const std::vector<hadTauChargeFlipGenMatchEntry> & hadTauChargeFlipGenMatch_definitions,
                                   int hadTauChargeFlipGenMatch_int)
{
  const hadTauChargeFlipGenMatchEntry * hadTauChargeFlipGenMatch = nullptr;
  for(const hadTauChargeFlipGenMatchEntry & hadTauChargeFlipGenMatch_definition: hadTauChargeFlipGenMatch_definitions)
  {
    if(hadTauChargeFlipGenMatch_definition.idx_ == hadTauChargeFlipGenMatch_int)
    {
      hadTauChargeFlipGenMatch = &hadTauChargeFlipGenMatch_definition;
    }
  }
  if(! hadTauChargeFlipGenMatch)
  {
    throw cmsException(__func__, __LINE__)
      << "Invalid parameter 'hadTauChargeFlipGenMatch_int' = " << hadTauChargeFlipGenMatch_int;
  }
  return hadTauChargeFlipGenMatch->name_;
}

int
getHadTauGenMatch_int(const std::vector<hadTauGenMatchEntry> & hadTauGenMatch_definitions,
                      const std::string & hadTauGenMatch_string)
{
  const hadTauGenMatchEntry * hadTauGenMatch = nullptr;
  for(const hadTauGenMatchEntry & hadTauGenMatch_definition: hadTauGenMatch_definitions)
  {
    if(hadTauGenMatch_definition.name_ == hadTauGenMatch_string)
    {
      hadTauGenMatch = &hadTauGenMatch_definition;
    }
  }
  if(! hadTauGenMatch)
    throw cmsException(__func__, __LINE__)
      << "Invalid parameter 'hadTauGenMatch_string' = " << hadTauGenMatch_string;
  return hadTauGenMatch->idx_;
}

int
getHadTauChargeFlipGenMatch_int(const std::vector<hadTauChargeFlipGenMatchEntry> & hadTauChargeFlipGenMatch_definitions,
                                const std::string & hadTauChargeFlipGenMatch_string)
{
  const hadTauChargeFlipGenMatchEntry * hadTauChargeFlipGenMatch = nullptr;
  for(const hadTauChargeFlipGenMatchEntry & hadTauChargeFlipGenMatch_definition: hadTauChargeFlipGenMatch_definitions)
  {
    if(hadTauChargeFlipGenMatch_definition.name_ == hadTauChargeFlipGenMatch_string)
    {
      hadTauChargeFlipGenMatch = &hadTauChargeFlipGenMatch_definition;
    }
  }
  if(! hadTauChargeFlipGenMatch)
    throw cmsException(__func__, __LINE__)
      << "Invalid parameter 'hadTauChargeFlipGenMatch_string' = " << hadTauChargeFlipGenMatch_string;
  return hadTauChargeFlipGenMatch->idx_;
}

void
countHadTauGenMatches(const RecoHadTau * hadTau,
                      int & numGenMatchedHadTaus,
                      int & numGenMatchedElectrons,
                      int & numGenMatchedMuons,
                      int & numGenMatchedJets)
{
  if     (hadTau->genHadTau()                                                ) ++numGenMatchedHadTaus;
  else if(hadTau->genLepton() && std::abs(hadTau->genLepton()->pdgId()) == 11) ++numGenMatchedElectrons;
  else if(hadTau->genLepton() && std::abs(hadTau->genLepton()->pdgId()) == 13) ++numGenMatchedMuons;
  else                                                                         ++numGenMatchedJets;
}

void
countHadTauChargeFlipGenMatches(const RecoHadTau * hadTau,
                                int & numGenMatchedHadTaus,
                                int & numGenMatchedElectrons,
                                int & numGenMatchedMuons,
                                int & numChargeFlippedGenMatchedHadTaus_or_Leptons,
                                int & numGenMatchedJets)
{
  if (hadTau->genHadTau()) 
  {
    ++numGenMatchedHadTaus;
    if (hadTau->charge() != hadTau->genHadTau()->charge())
    {
      ++numChargeFlippedGenMatchedHadTaus_or_Leptons;
    }
  }
  else if(hadTau->genLepton() && std::abs(hadTau->genLepton()->pdgId()) == 11) 
  {
    ++numGenMatchedElectrons;
    if (hadTau->charge() != hadTau->genLepton()->charge())
    {
      ++numChargeFlippedGenMatchedHadTaus_or_Leptons;
    }
  }
  else if(hadTau->genLepton() && std::abs(hadTau->genLepton()->pdgId()) == 13) 
  {
    ++numGenMatchedMuons;
    if (hadTau->charge() != hadTau->genLepton()->charge())
    {
      ++numChargeFlippedGenMatchedHadTaus_or_Leptons;
    }
  }
  else 
  {
    ++numGenMatchedJets;
  }
}

namespace
{
  bool constexpr
  matches(int nSel,
          int nMatches)
  {
    return nSel == -1 || nSel == nMatches;
  }

  const hadTauGenMatchEntry &
  getHadTauGenMatch(const std::vector<hadTauGenMatchEntry> & hadTauGenMatch_definitions,
                    int numGenMatchedHadTaus,
                    int numGenMatchedElectrons,
                    int numGenMatchedMuons,
                    int numGenMatchedJets)
  {
    const hadTauGenMatchEntry * hadTauGenMatch = nullptr;
    for(const hadTauGenMatchEntry & hadTauGenMatch_definition: hadTauGenMatch_definitions)
    {
      if(matches(hadTauGenMatch_definition.numGenMatchedHadTaus_,   numGenMatchedHadTaus)   &&
         matches(hadTauGenMatch_definition.numGenMatchedElectrons_, numGenMatchedElectrons) &&
         matches(hadTauGenMatch_definition.numGenMatchedMuons_,     numGenMatchedMuons)     &&
         matches(hadTauGenMatch_definition.numGenMatchedJets_,      numGenMatchedJets)       )
      {
        hadTauGenMatch = &hadTauGenMatch_definition;
      }
    }
    if(! hadTauGenMatch)
    {
      throw cmsException(__func__, __LINE__)
        << "Failed to compute 'hadTauGenMatch' for numGenMatched:"
           " hadTaus = "   << numGenMatchedHadTaus << ","
           " electrons = " << numGenMatchedElectrons << ","
           " muons = "     << numGenMatchedMuons << ","
           " jets = "      << numGenMatchedJets
      ;
    }
    return *hadTauGenMatch;
  }

  const hadTauChargeFlipGenMatchEntry &
  getHadTauChargeFlipGenMatch(const std::vector<hadTauChargeFlipGenMatchEntry> & hadTauChargeFlipGenMatch_definitions,
                              int numGenMatchedHadTaus,
                              int numGenMatchedElectrons,
                              int numGenMatchedMuons,
                              int numChargeFlippedGenMatchedHadTaus_or_Leptons,
                              int numGenMatchedJets)
  {
    const hadTauChargeFlipGenMatchEntry * hadTauChargeFlipGenMatch = nullptr;
    for(const hadTauChargeFlipGenMatchEntry & hadTauChargeFlipGenMatch_definition: hadTauChargeFlipGenMatch_definitions)
    {
      if(matches(hadTauChargeFlipGenMatch_definition.numGenMatchedHadTaus_,                         numGenMatchedHadTaus)   &&
         matches(hadTauChargeFlipGenMatch_definition.numGenMatchedElectrons_,                       numGenMatchedElectrons) &&
         matches(hadTauChargeFlipGenMatch_definition.numGenMatchedMuons_,                           numGenMatchedMuons)     &&
         matches(hadTauChargeFlipGenMatch_definition.numChargeFlippedGenMatchedHadTaus_or_Leptons_, numChargeFlippedGenMatchedHadTaus_or_Leptons) &&
         matches(hadTauChargeFlipGenMatch_definition.numGenMatchedJets_,                            numGenMatchedJets)       )
      {
        hadTauChargeFlipGenMatch = &hadTauChargeFlipGenMatch_definition;
      }
    }
    if(! hadTauChargeFlipGenMatch)
    {
      throw cmsException(__func__, __LINE__)
        << "Failed to compute 'hadTauChargeFlipGenMatch' for numGenMatched:"
           " hadTaus = "                           << numGenMatchedHadTaus << ","
           " electrons = "                         << numGenMatchedElectrons << ","
           " muons = "                             << numGenMatchedMuons << ","
           " charge flipped hadTaus or leptons = " << numChargeFlippedGenMatchedHadTaus_or_Leptons << ","
           " jets = "                              << numGenMatchedJets
      ;
    }
    return *hadTauChargeFlipGenMatch;
  }
}

const hadTauGenMatchEntry &
getHadTauGenMatch(const std::vector<hadTauGenMatchEntry> & hadTauGenMatch_definitions,
                  const RecoHadTau * hadTau_lead,
                  const RecoHadTau * hadTau_sublead,
                  const RecoHadTau * hadTau_third,
                  const RecoHadTau * hadTau_fourth)
{
  int numGenMatchedHadTaus = 0;
  int numGenMatchedElectrons = 0;
  int numGenMatchedMuons = 0;
  int numGenMatchedJets = 0;

  const auto countHadTauGenMatches_local = [&](const RecoHadTau * hadTau) -> void
  {
    return countHadTauGenMatches(
      hadTau, numGenMatchedHadTaus, numGenMatchedElectrons, numGenMatchedMuons, numGenMatchedJets
    );
  };

  assert(hadTau_lead);
  countHadTauGenMatches_local(hadTau_lead);
  if(hadTau_sublead)
  {
    countHadTauGenMatches_local(hadTau_sublead);
  }
  if(hadTau_third)
  {
    countHadTauGenMatches_local(hadTau_third);
  }
  if(hadTau_fourth)
  {
    countHadTauGenMatches_local(hadTau_fourth);
  }

  return getHadTauGenMatch(
    hadTauGenMatch_definitions, numGenMatchedHadTaus, numGenMatchedElectrons, numGenMatchedMuons, numGenMatchedJets
  );
}

const hadTauChargeFlipGenMatchEntry &
getHadTauChargeFlipGenMatch(const std::vector<hadTauChargeFlipGenMatchEntry> & hadTauChargeFlipGenMatch_definitions,
                            const RecoHadTau * hadTau_lead,
                            const RecoHadTau * hadTau_sublead,
                            const RecoHadTau * hadTau_third,
                            const RecoHadTau * hadTau_fourth)
{
  int numGenMatchedHadTaus = 0;
  int numGenMatchedElectrons = 0;
  int numGenMatchedMuons = 0;
  int numChargeFlippedGenMatchedHadTaus_or_Leptons = 0;  
  int numGenMatchedJets = 0;

  const auto countHadTauChargeFlipGenMatches_local = [&](const RecoHadTau * hadTau) -> void
  {
    return countHadTauChargeFlipGenMatches(
      hadTau, numGenMatchedHadTaus, numGenMatchedElectrons, numGenMatchedMuons,
      numChargeFlippedGenMatchedHadTaus_or_Leptons, numGenMatchedJets
    );
  };

  assert(hadTau_lead);
  countHadTauChargeFlipGenMatches_local(hadTau_lead);
  if(hadTau_sublead)
  {
    countHadTauChargeFlipGenMatches_local(hadTau_sublead);
  }
  if(hadTau_third)
  {
    countHadTauChargeFlipGenMatches_local(hadTau_third);
  }
  if(hadTau_fourth)
  {
    countHadTauChargeFlipGenMatches_local(hadTau_fourth);
  }

  return getHadTauChargeFlipGenMatch(
    hadTauChargeFlipGenMatch_definitions, numGenMatchedHadTaus, numGenMatchedElectrons, numGenMatchedMuons,
    numChargeFlippedGenMatchedHadTaus_or_Leptons, numGenMatchedJets
  );
}

std::ostream &
operator<<(std::ostream & stream,
           const hadTauGenMatchEntry & hadTauGenMatch_definition)
{
  stream << " hadTauGenMatch #" << hadTauGenMatch_definition.idx_
         << ": "                << hadTauGenMatch_definition.name_
         << '\n';
  return stream;
}

std::ostream &
operator<<(std::ostream & stream,
           const hadTauChargeFlipGenMatchEntry & hadTauChargeFlipGenMatch_definition)
{
  stream << " hadTauChargeFlipGenMatch #" << hadTauChargeFlipGenMatch_definition.idx_
         << ": "                          << hadTauChargeFlipGenMatch_definition.name_
         << '\n';
  return stream;
}

std::ostream &
operator<<(std::ostream & stream,
           const std::vector<hadTauGenMatchEntry> & hadTauGenMatch_definitions)
{
  for(const hadTauGenMatchEntry & hadTauGenMatch_definition: hadTauGenMatch_definitions)
  {
    stream << hadTauGenMatch_definition;
  }
  return stream;
}

std::ostream &
operator<<(std::ostream & stream,
           const std::vector<hadTauChargeFlipGenMatchEntry> & hadTauChargeFlipGenMatch_definitions)
{
  for(const hadTauChargeFlipGenMatchEntry & hadTauChargeFlipGenMatch_definition: hadTauChargeFlipGenMatch_definitions)
  {
    stream << hadTauChargeFlipGenMatch_definition;
  }
  return stream;
}
