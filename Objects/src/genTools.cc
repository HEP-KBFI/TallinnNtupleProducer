#include "TallinnNtupleProducer/Objects/interface/genTools.h"

#include "TallinnNtupleProducer/CommonTools/interface/cmsException.h" // cmsException()
#include "TallinnNtupleProducer/CommonTools/interface/format_vT.h" // format_vint()
#include "TallinnNtupleProducer/CommonTools/interface/topPtRwgt.h" // TopPtRwgtOption::

#include <algorithm> // std::sort()

namespace
{
  constexpr double
  square(double x)
  {
    return x * x;
  }
}

Int_t
getHiggsDecayMode(const GenParticleCollection & genParticles)
{
  // First, we need to find all gen-level Higgs bosons. The catch is that there may be
  // multiple Higgs bosons in Pythia gen particle listing that appear as part of a decay chain,
  // even though it's actually some intermediate Pythia step that we should ignore, eg:
  //
  // (protons) -> H -> H -> (descendants)
  //
  // The solution is to find all Higgs bosons that have two daughters, and use the PDG ID of
  // the daughters to construct the code.

  std::vector<std::vector<int>> higgsDaughterPdgIds;
  for(std::size_t genPartIdx = 0; genPartIdx < genParticles.size(); ++genPartIdx)
  {
    const GenParticle & genPart = genParticles.at(genPartIdx);
    if(genPart.absPdgId() == 25 && genPart.dauIdxs().size() == 2)
    {
      const std::vector<int> & dauIdxs = genPart.dauIdxs();
      std::vector<int> higgsDaughterPdgId {
        genParticles.at(dauIdxs.at(0)).absPdgId(),
        genParticles.at(dauIdxs.at(1)).absPdgId(),
      };
      std::sort(higgsDaughterPdgId.begin(), higgsDaughterPdgId.end());
      higgsDaughterPdgIds.push_back(higgsDaughterPdgId);
    }
  }

  // Construct the code with the following algorithm:
  // i) For each pair of Higgs daughters, figure out its code:
  // i.a) if it decays into the same particles (modulo electric charge), assign a single number
  // i.b) if it decays into two different particles -- ie Zgamma -- assign: (smaller PDG ID) * 100 + larger PDG ID, ie 2223
  // ii) compute final code from individual Higgses:
  // ii.a) if there are no Higgses
  // ii.b) if there's just one Higgs, return the code itself
  // ii.c) if there are two Higgs that have the same DM, then return just one code
  // ii.d) if there are two Higgses but wit different codes, sort them in ascending order and return (smaller code) * 10000 + larger code
  // ii.e) if there are more than two Higgses, then this could be extended to eg (smallest code) * 100000000 + (medium code) * 10000 + largest code
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
    if(codes.at(0) != codes.at(1))
    {
      return codes.at(0) * 10000 + codes.at(1);
    }
    else
    {
      return codes.at(0);
    }
  }
  else if(codes.size() > 2)
  {
    throw cmsException(__func__, __LINE__) << "Too many Higgs bosons found: " << codes.size();
  }
  
  return 0;
}

double
topPtRwgtSF(const GenParticleCollection & genParticles,
            TopPtRwgtOption option)
{
  // Find a top and an anti-top with two daughters
  std::vector<int> genTopIdxs;
  for(std::size_t genPartIdx = 0; genPartIdx < genParticles.size(); ++genPartIdx)
  {
    if(genTopIdxs.size() == 2)
    {
      break; // can break early because the SF is supposed to be applied only to ttbar, so at two tops
    }
    const GenParticle & topCandidate = genParticles.at(genPartIdx);
    if(topCandidate.absPdgId() == 6)
    {
      const std::vector<int> & dauIdxs = topCandidate.dauIdxs();
      if(dauIdxs.size() == 2)
      {
        genTopIdxs.push_back(genPartIdx);
      }
    }
  }
  if(genTopIdxs.size() != 2)
  {
    throw cmsException(__func__, __LINE__) << "Invalid number of gen tops found: " << genTopIdxs.size();
  }
  const GenParticle & firstTop = genParticles.at(genTopIdxs.at(0));
  const GenParticle & secondTop = genParticles.at(genTopIdxs.at(1));
  if(firstTop.pdgId() * secondTop.pdgId() > 0)
  {
    throw cmsException(__func__, __LINE__) << "Found two tops with the same PDG ID: " << firstTop << " and " << secondTop;
  }
  std::vector<double> topPts = { firstTop.pt(), secondTop.pt() };
  
  double topPtRwgt = 1.;
  for(double topPt: topPts)
  {
    switch(option)
    {
      case TopPtRwgtOption::TOP16011:
      {
        const double topPtLim = std::min(topPt, 800.);
        const double a = 0.0615;
        const double b = -0.0005;
        topPtRwgt *= std::exp(a + b * topPtLim);
        break;
      }
      case TopPtRwgtOption::Linear:
      {
        const double topPtLim = std::min(topPt, 500.);
        const double a = 0.058;
        const double b = -0.000466;
        topPtRwgt *= std::exp(a + b * topPtLim);
        break;
      }
      case TopPtRwgtOption::Quadratic:
      {
        const double topPtLim = std::min(topPt, 472.);
        const double a = 0.088;
        const double b = -0.00087;
        const double c = 9.2e-07;
        topPtRwgt *= std::exp(a + b * topPtLim + c * ::square(topPtLim));
        break;
      }
      case TopPtRwgtOption::HighPt:
      {
        // see slide 12 of:
        // https://indico.cern.ch/event/904971/contributions/3857701/attachments/2036949/3410728/TopPt_20.05.12.pdf
        const double topPtLim = std::min(topPt, 3000.);
        const double a = -2.02274e-01;
        const double b =  1.09734e-04;
        const double c = -1.30088e-07;
        const double d =  5.83494e+01;
        const double e =  1.96252e+02;
        topPtRwgt *= std::exp(a + b * topPtLim + c * ::square(topPtLim) + d / (topPtLim + e));
        break;
      }
      // just do nothing
      case TopPtRwgtOption::Disable: __attribute__((fallthrough));
      default: ;
    }
  }
  assert(topPtRwgt > 0.);
  return std::sqrt(topPtRwgt);
}

std::pair<const GenParticle *, const GenParticle *>
findTauLeptonPair(const GenParticleCollection & genParticles)
{
  const GenParticle * genTauLeptonPlus = nullptr;
  const GenParticle * genTauLeptonMinus = nullptr;
  for(const GenParticle & genParticle: genParticles)
  {
    if(genParticle.absPdgId() != 15)
    {
      continue;
    }
    bool not_last_copy = false; // TOOD could StatusFlag::isLastCopy work?
    for(int dau_idx: genParticle.dauIdxs())
    {
      if(genParticles.at(dau_idx).absPdgId() == 15)
      {
        not_last_copy = true;
        break;
      }
    }
    if(! not_last_copy)
    {
      continue;
    }
    // TODO require StatusFlag::isPrompt? or does that not work when the tau originates from (Z/gamma*) decays?
    if(genParticle.charge() > 0 && (! genTauLeptonPlus || genParticle.pt() > genTauLeptonPlus->pt()))
    {
      genTauLeptonPlus = &genParticle;
    }
    if(genParticle.charge() < 0 && (! genTauLeptonMinus || genParticle.pt() > genTauLeptonMinus->pt()))
    {
      genTauLeptonMinus = &genParticle;
    }
  }
  return std::make_pair(genTauLeptonPlus, genTauLeptonMinus);
}
