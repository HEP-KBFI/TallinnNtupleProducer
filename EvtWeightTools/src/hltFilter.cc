#include "TallinnNtupleProducer/EvtWeightTools/interface/hltFilter.h" 

#include "TallinnNtupleProducer/CommonTools/interface/cmsException.h" // cmsException()
#include "TallinnNtupleProducer/CommonTools/interface/Era.h"          // kEra_2016, kEra_2017, kEra_2018
#include "TallinnNtupleProducer/Objects/interface/RecoHadTau.h"       // RecoHadTau
#include "TallinnNtupleProducer/Objects/interface/RecoLepton.h"       // RecoLepton

#include <array>                                                      // std::array<,>

bool
hltFilter(const std::map<hltPathsE, bool> & trigger_bits,
          const std::vector<const RecoLepton *> & leptons,
          const std::vector<const RecoHadTau *> & taus)
{
  static const unsigned trigger_eleIdx = 0;
  static const unsigned trigger_muIdx  = 1;
  static const unsigned trigger_tauIdx = 2;

  static const std::map<hltPathsE, std::array<int, 3>> trigger_multiplicities = {
    { hltPathsE::trigger_1e,      {{ 1, 0, 0 }} },
    { hltPathsE::trigger_1mu,     {{ 0, 1, 0 }} },
    { hltPathsE::trigger_2e,      {{ 2, 0, 0 }} },
    { hltPathsE::trigger_2mu,     {{ 0, 2, 0 }} },
    { hltPathsE::trigger_1e1mu,   {{ 1, 1, 0 }} },
    { hltPathsE::trigger_1e2mu,   {{ 1, 1, 0 }} }, // [*] ideally: { 1, 2, 0 }
    { hltPathsE::trigger_2e1mu,   {{ 2, 1, 0 }} },
    { hltPathsE::trigger_3e,      {{ 3, 0, 0 }} },
    { hltPathsE::trigger_3mu,     {{ 0, 2, 0 }} }, // [*] ideally: { 0, 3, 0 }
    { hltPathsE::trigger_1e1tau,  {{ 1, 0, 1 }} },
    { hltPathsE::trigger_1mu1tau, {{ 0, 1, 1 }} },
    { hltPathsE::trigger_2tau,    {{ 0, 0, 2 }} },
  };
  // [*] There's a bug in HLTMuonL3PreFilter module, which loses one muon trigger object, see
  //     https://hypernews.cern.ch/HyperNews/CMS/get/muon-hlt/593.html for more information

  static const std::map<hltPathsE, std::array<int, 3>> trigger_filterBits = {
    { hltPathsE::trigger_1e,      {{   2,   0,   0 }} },
    { hltPathsE::trigger_1mu,     {{   0,   8,   0 }} },
    { hltPathsE::trigger_2e,      {{  16,   0,   0 }} },
    { hltPathsE::trigger_2mu,     {{   0,  16,   0 }} },
    { hltPathsE::trigger_1e1mu,   {{  32,  32,   0 }} },
    { hltPathsE::trigger_1e2mu,   {{ 512, 256,   0 }} },
    { hltPathsE::trigger_2e1mu,   {{ 256, 512,   0 }} },
    { hltPathsE::trigger_3e,      {{ 128,   0,   0 }} },
    { hltPathsE::trigger_3mu,     {{   0, 128,   0 }} },
    { hltPathsE::trigger_1e1tau,  {{  64,   0, 128 }} },
    { hltPathsE::trigger_1mu1tau, {{   0,  64, 256 }} },
    { hltPathsE::trigger_2tau,    {{   0,   0,  64 }} }
  };

  bool is_matched = false;
  for(const auto & kv: trigger_bits)
  {
    const bool is_triggered = kv.second;
    if(! is_triggered)
    {
      // If the trigger hasn't fired, there is no point trying to check if any of the reco objects
      // match to trigger objects that come from this (non-fired) HLT trigger path.
      continue;
    }

    const hltPathsE path = kv.first;
    int nof_electrons = 0;
    int nof_muons = 0;
    int nof_taus = 0;

    for(const RecoLepton * lepton: leptons)
    {
      const UInt_t filterBits = lepton->filterBits();
      if(lepton->is_electron())
      {
        if(filterBits & trigger_filterBits.at(path).at(trigger_eleIdx))
        {
          ++nof_electrons;
        }
      }
      else if(lepton->is_muon())
      {
        if(filterBits & trigger_filterBits.at(path).at(trigger_muIdx))
        {
          ++nof_muons;
        }
      }
      else
      {
        throw cmsException(__func__, __LINE__)
          << "Invalid lepton flavor";
      }
    }
    for(const RecoHadTau * tau: taus)
    {
      const UInt_t filterBits = tau->filterBits();
      if(filterBits & trigger_filterBits.at(path).at(trigger_tauIdx))
      {
        ++nof_taus;
      }
    }

    // There are at least two ways of how the HLT filter could be applied:
    // 1) require that all trigger objects that come from any fired HLT paths are matched to the reco objects
    // 2) require that all reco objects are matched to trigger objects that come from fired HLT paths
    // In the current implementation, we perform 1) here: if the reco objects are matched to at least one set
    // of trigger objects that originate from a fired trigger, we can say that the event passes HLT filter cuts.
    if(nof_electrons == trigger_multiplicities.at(path).at(trigger_eleIdx) &&
       nof_muons     == trigger_multiplicities.at(path).at(trigger_muIdx) &&
       nof_taus      == trigger_multiplicities.at(path).at(trigger_tauIdx))
    {
      is_matched = true;
      break;
    }
  }
  return is_matched;
}

bool
hltFilter(const RecoHadTau& tau, const hltPathsE& hltPath)
{ 
  static const std::map<hltPathsE, int> trigger_filterBits = {
    { hltPathsE::trigger_1e1tau,  128 },
    { hltPathsE::trigger_1mu1tau, 256 },
    { hltPathsE::trigger_2tau,     64 }
  };
  if ( trigger_filterBits.find(hltPath) == trigger_filterBits.end() )
    throw cms::Exception("hltFilter") 
      << "Invalid parameter 'hltPath' = " << (int)hltPath << " !!\n";
  if ( tau.filterBits() & trigger_filterBits.at(hltPath) ) return true;
  else return false;
}

bool
hltFilter(const RecoHadTau& tau, int filterBit, Era era)
{
  UInt_t filterBit_mask = 0;
  // CV: bit masks defined in https://github.com/HEP-KBFI/cmssw/blob/master/PhysicsTools/NanoAOD/python/triggerObjects_cff.py#L94
  int filterBit_status = -1; // passes = 1, fails = 0
  // HLT filter names specific to 2016 data-taking period
  if ( era == Era::k2016 ) 
  {
    if      ( filterBit == kTauFilterBit2016_notApplied                    ) return true;
    else if ( filterBit == kTauFilterBit2016_passesLooseIso                ) { filterBit_mask = 1; filterBit_status = 1; }
    else if ( filterBit == kTauFilterBit2016_failsLooseIso                 ) { filterBit_mask = 1; filterBit_status = 0; }
    else if ( filterBit == kTauFilterBit2016_passesMediumIso               ) { filterBit_mask = 2; filterBit_status = 1; }
    else if ( filterBit == kTauFilterBit2016_failsMediumIso                ) { filterBit_mask = 2; filterBit_status = 0; }
    else throw cms::Exception("hltFilter") 
      << "Invalid parameter 'filterBit' = " << filterBit << " !!\n";
  }
  // HLT filter names specific to 2017 and 2018 data-taking periods
  else if ( era == Era::k2017 || era == Era::k2018 )
  {
    if      ( filterBit == kTauFilterBit2017and2018_notApplied             ) return true;
    else if ( filterBit == kTauFilterBit2017and2018_passesLooseChargedIso  ) { filterBit_mask = 1; filterBit_status = 1; }
    else if ( filterBit == kTauFilterBit2017and2018_failsLooseChargedIso   ) { filterBit_mask = 1; filterBit_status = 0; }
    else if ( filterBit == kTauFilterBit2017and2018_passesMediumChargedIso ) { filterBit_mask = 2; filterBit_status = 1; }
    else if ( filterBit == kTauFilterBit2017and2018_failsMediumChargedIso  ) { filterBit_mask = 2; filterBit_status = 0; }
    else if ( filterBit == kTauFilterBit2017and2018_passesTightChargedIso  ) { filterBit_mask = 4; filterBit_status = 1; }
    else if ( filterBit == kTauFilterBit2017and2018_failsTightChargedIso   ) { filterBit_mask = 4; filterBit_status = 0; }
    else throw cms::Exception("hltFilter") 
      << "Invalid parameter 'filterBit' = " << filterBit << " !!\n";
  }
  else throw cms::Exception("hltFilter") 
    << "Invalid era = " << static_cast<int>(era) << " !!\n";
  if      ( filterBit_status == 1 ) 
  {
    if ( tau.filterBits() & filterBit_mask ) return true;
    else return false;
  } 
  else if ( filterBit_status == 0 )
  {
    if ( tau.filterBits() & filterBit_mask ) return false;
    else return true;
  }
  else assert(0);
}
