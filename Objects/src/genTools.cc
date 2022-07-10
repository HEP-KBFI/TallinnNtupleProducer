#include "TallinnNtupleProducer/Objects/interface/genTools.h"

#include "TallinnNtupleProducer/CommonTools/interface/cmsException.h" // cmsException()
#include "TallinnNtupleProducer/CommonTools/interface/format_vT.h" // format_vint()

#include <algorithm> // std::sort()

Int_t
getHiggsDecayMode(const GenParticleCollection & genParticles)
{
  // First, we need to find all gen-level Higgs bosons. The catch is that there may be
  // multiple Higgs bosons in Pythia gen particle listing that appear as part of a decay chain,
  // even though it's actually some intermediate Pythia step that we should ignore, eg:
  //
  // (protons) -> H -> H -> (descendants)
  //
  // The issue is more complicated in HH events, which have two genuine Higgs bosons at the gen level.
  // AFAICT, the off-shell Higgs in HH events is not recorded, only the initial state protons are.
  // The solution is thus divided into two parts:
  //
  // 1) find the unique list of Higgs bosons that do not have Higgs as a parent
  // 2) for each such unique Higgs bosons, check its decscendants recursively until you'll find non-Higgs particles
  //
  // An integer code is built from the descendants of all Higgs bosons

  // 1)
  std::vector<int> firstHiggsCopies;
  for(std::size_t genPartIdx = 0; genPartIdx < genParticles.size(); ++genPartIdx)
  {
    const GenParticle & genPart = genParticles.at(genPartIdx);
    if(genPart.absPdgId() == 25 && genPart.momIdx() >= 0 && genParticles.at(genPart.momIdx()).absPdgId() != 25)
    {
      // record the Higgs if its parent is not a Higgs (which is guaranteed to exist in Higgs samples)
      firstHiggsCopies.push_back(genPartIdx);
    }
  }
  
  // 2)
  std::vector<std::vector<int>> higgsDaughterPdgIds;
  for(int firstHiggsCopy: firstHiggsCopies)
  {
    int currentHiggsIdx = firstHiggsCopy;
    while(currentHiggsIdx >= 0)
    {
      const GenParticle & currentHiggs = genParticles.at(currentHiggsIdx);
      currentHiggsIdx = -1;
      
      const std::vector<int> & currentHiggsDaughterIdxs = currentHiggs.dauIdxs();
      std::vector<int> currentHiggsDaughterPdgIds;
      for(int currentHiggsDaughterIdx: currentHiggsDaughterIdxs)
      {
        assert(currentHiggsDaughterIdx != currentHiggsIdx); // to avoid accidental infinite loop
        currentHiggsDaughterPdgIds.push_back(genParticles.at(currentHiggsDaughterIdx).absPdgId());
      }
      
      if(currentHiggsDaughterPdgIds.size() == 1)
      {
        if(currentHiggsDaughterPdgIds.at(0) == 25)
        {
          currentHiggsIdx = currentHiggsDaughterIdxs.at(0);
        }
        else
        {
          throw cmsException(__func__, __LINE__) << "Found a Higgs with only one non-Higgs descendant: " << currentHiggs;
        }
      }
      else if(currentHiggsDaughterPdgIds.size() == 2)
      {
        if(currentHiggsDaughterPdgIds.at(0) == 25 || currentHiggsDaughterPdgIds.at(1) == 25)
        {
          throw cmsException(__func__, __LINE__) << "Found a Higgs with two daughters, one of which is Higgs: " << currentHiggs;
        }
        // sort the indices in ascending order
        std::sort(currentHiggsDaughterPdgIds.begin(), currentHiggsDaughterPdgIds.end());
      }
      else
      {
        throw cmsException(__func__, __LINE__) << "Found a Higgs with invalid number of daughters: " << currentHiggs;
      }
    }
  }
  
  // Construct the code with the following algorithm:
  // i) For each pair of Higgs daughters, figure out its code:
  // i.a) if it decays into the same particles (modulo electric charge), assign a single number
  // i.b) if it decays into two different particles -- ie Zgamma -- assign: (smaller PDG ID) * 100 + larger PDG ID, ie 2223
  // ii) compute final code from individual Higgses:
  // ii.a) if there are no Higgses
  // ii.b) if there's just one Higgs, return the code itself
  // ii.c) if there are two Higgses, sort them in ascending order and return (smaller code) * 10000 + larger code
  // ii.d) if there are more than two Higgses, then this could be extended to eg (smallest code) * 100000000 + (medium code) * 10000 + largest code
  //       but we're going to throw an error here
  //
  // This yields the following mapping to Higgs decay modes:
  // H: hww -> 24, hzz -> 23, htt -> 15, hzg -> 2223, hmm -> 13, hbb -> 5
  // HH: tttt -> 15, zzzz -> 23, wwww -> 24, bbtt -> 50015, bbzz -> 50023, bbww -> 50024, ttzz -> 150023, ttww -> 150024, zzww -> 230024
  //
  // Note that the codes can overlap between H and HH events, but this shouldn't be a problem.

  // i)
  std::vector<int> codes;
  for(const std::vector<int> & higgsDaughterPdgId: higgsDaughterPdgIds)
  {
    assert(higgsDaughterPdgId.size() == 2);
    if(higgsDaughterPdgId.at(0) == higgsDaughterPdgId.at(1))
    {
      codes.push_back(higgsDaughterPdgId.at(0));
    }
    else
    {
      // the PDG IDs are already sorted, cf 2)
      codes.push_back(higgsDaughterPdgId.at(0) * 100 + higgsDaughterPdgId.at(1));
    }
  }
  std::sort(codes.begin(), codes.end());

  // ii)
  if(codes.size() == 1)
  {
    return codes.at(0);
  }
  else if(codes.size() == 2)
  {
    return codes.at(0) * 10000 + codes.at(1);
  }
  else if(codes.size() > 2)
  {
    throw cmsException(__func__, __LINE__) << "Too many Higgs bosons found (indices): " << format_vint(firstHiggsCopies);
  }
  
  return 0;
}

double
topPtRwgtSF(const GenParticleCollection & genParticles)
{
  // TODO implement me!
  return 0.;
}
