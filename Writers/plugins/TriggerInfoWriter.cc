#include "TallinnNtupleProducer/Writers/plugins/TriggerInfoWriter.h"

#include "TallinnNtupleProducer/CommonTools/interface/BranchAddressInitializer.h" // BranchAddressInitializer
#include "TallinnNtupleProducer/CommonTools/interface/cmsException.h"             // cmsException()

#include "TTree.h"                                                                // TTree

typedef std::vector<std::string> vstring;

enum PD_type { kUndefined, kMC, kSingleMuon, kSingleElectron, kDoubleMuon, kMuonEG, kDoubleEG, kTau };

namespace
{
  int convertPD_to_int(const std::string & PD)
  {
    if      ( PD == "MC"             ) return PD_type::kMC;
    else if ( PD == "SingleMuon"     ) return PD_type::kSingleMuon;
    else if ( PD == "SingleElectron" ) return PD_type::kSingleElectron;
    else if ( PD == "DoubleMuon"     ) return PD_type::kDoubleMuon;
    else if ( PD == "MuonEG"         ) return PD_type::kMuonEG;
    else if ( PD == "DoubleEG"       ) return PD_type::kDoubleEG;
    else if ( PD == "Tau"            ) return PD_type::kTau;
    else throw cmsException(__func__, __LINE__) 
      << "Invalid PD type = '" << PD << "' !!";
  }
}

TriggerInfoWriter::TriggerInfoWriter(const edm::ParameterSet & cfg)
  : WriterBase(cfg)
  , PD_(PD_type::kUndefined)
  , passesTrigger_(false)
{
  std::string PD_string = cfg.getParameter<std::string>("PD");
  PD_ = convertPD_to_int(PD_string);
  vstring PD_priority_strings = cfg.getParameter<vstring>("PD_priority");
  for ( auto PD_priority_string : PD_priority_strings )
  {
    PD_priority_.push_back(convertPD_to_int(PD_priority_string));
  }
}

TriggerInfoWriter::~TriggerInfoWriter()
{}

void
TriggerInfoWriter::setBranches(TTree * outputTree)
{
  BranchAddressInitializer bai(outputTree);
  bai.setBranch(passesTrigger_, "passesTrigger");
}

namespace
{
  template <typename T>
  unsigned
  countTriggerMatchedObjects(const std::vector<const T *> & objects, const std::vector<unsigned> & hltFilterBits)
  {
    unsigned numTriggerMatchedObjects = 0;
    if ( hltFilterBits.size() > 0 ) // at least one HLT filter bit defined
    {
      for ( auto object : objects )
      {
        bool passesHLTFilterBit = false;
        for ( auto hltFilterBit : hltFilterBits )
        {
          if ( object->filterBits() & hltFilterBit )
          {
            passesHLTFilterBit = true;
            break;
          }
        }
        if ( passesHLTFilterBit ) ++numTriggerMatchedObjects;
      }
    }
    else                            // no HLT filter bits defined
    {
      numTriggerMatchedObjects = objects.size();
    }
    return numTriggerMatchedObjects;
  }
}

void
TriggerInfoWriter::writeImp(const Event & event, const EvtWeightRecorder & evtWeightRecorder)
{
  const RecoElectronPtrCollection& electrons = event.fakeableElectrons();
  const RecoMuonPtrCollection& muons = event.fakeableMuons();
  const RecoHadTauPtrCollection& hadTaus = event.fakeableHadTaus();

  std::map<int, bool> passesTriggerMap; // key = PD

  const std::vector<trigger::Entry>& triggerEntries = event.triggerInfo().entries();
  for ( auto triggerEntry : triggerEntries )
  {
    if ( !triggerEntry.use_it() ) continue;

    bool passesHLTPath = false;
    const std::vector<trigger::HLTPath>& hltPaths = triggerEntry.hltPaths();
    for ( auto hltPath : hltPaths )
    {
      if ( hltPath.status() )
      {
        passesHLTPath = true;
        break;
      }
    }
    if ( !passesHLTPath ) continue;

    if ( triggerEntry.min_numElectrons() > 0 )
    {
      unsigned numElectrons = countTriggerMatchedObjects(electrons, triggerEntry.hltFilterBits_e());
      if ( numElectrons < triggerEntry.min_numElectrons() ) continue;
    }

    if ( triggerEntry.min_numMuons() > 0 )
    {
      unsigned numMuons = countTriggerMatchedObjects(muons, triggerEntry.hltFilterBits_mu());
      if ( numMuons < triggerEntry.min_numMuons() ) continue;
    }

    if ( triggerEntry.min_numHadTaus() > 0 )
    {
      unsigned numHadTaus = countTriggerMatchedObjects(hadTaus, triggerEntry.hltFilterBits_tau());
      if ( numHadTaus < triggerEntry.min_numHadTaus() ) continue;
    }

    int in_PD = convertPD_to_int(triggerEntry.in_PD());
    passesTriggerMap[in_PD] = true;
  }

  //CV: Rank triggers by priority and ignore triggers of lower priority if a trigger of higher priority has fired for given event;
  //    the triggers are ranked by primary dataset (PD).
  //    The ranking of the PDs is as follows: DoubleMuon, MuonEG, DoubleEG, SingleMuon, SingleElectron, Tau.
  //    See https://cmssdt.cern.ch/lxr/source/HLTrigger/Configuration/python/HLT_GRun_cff.py?v=CMSSW_8_0_24 for association of triggers paths to PD.
  //    This logic is necessary to avoid that the same event is selected multiple times when processing different primary datasets.
  //    The trigger ranking is applied to data only. Simulated events pass once they qualify for any PD.
  passesTrigger_ = false;
  for ( auto PD_priority_iter : PD_priority_ )
  {
    if ( passesTriggerMap[PD_priority_iter] )
    {
      if ( PD_ == kMC ) // MC
      {
        passesTrigger_ = true;
      }
      else              // data
      {
        if ( PD_priority_iter != PD_ ) passesTrigger_ = false; // event passes trigger for PD of higher priority
        else                           passesTrigger_ = true;  // event does not pass trigger for any PD of higher priority
      }
      break;
    }
  }
}

std::vector<std::string>
TriggerInfoWriter::get_supported_systematics(const edm::ParameterSet & cfg)
{
  return std::vector<std::string>();
}

DEFINE_EDM_PLUGIN(WriterPluginFactory, TriggerInfoWriter, "TriggerInfoWriter");
