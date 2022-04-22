#include "TallinnNtupleProducer/EvtWeightTools/interface/HadTauFakeRateInterface.h"

#include "TallinnNtupleProducer/CommonTools/interface/cmsException.h"       // cmsException()
#include "TallinnNtupleProducer/EvtWeightTools/interface/lutAuxFunctions.h" // openFile()

#include "TFile.h"                                                          // TFile

#include <assert.h>                                                         // assert()

typedef std::vector<double> vdouble;

namespace
{
  void
  initializeHadTauFakeRateWeights(TFile * inputFile,
                                  const std::string & hadTauSelection,
                                  const edm::ParameterSet & cfg,
                                  const std::string& trigMatching,
                                  int central_or_shift,
                                  std::map<int, std::vector<HadTauFakeRateWeightEntry *>> & hadTauFakeRateWeights,
                                  bool & isInitialized)
  {
    const vdouble absEtaBins = cfg.getParameter<vdouble>("absEtaBins");
    const int numAbsEtaBins = absEtaBins.size() - 1;
    if(numAbsEtaBins < 1)
    {
      throw cmsException(__func__)
        << "Invalid Configuration parameter 'absEtaBins' !!\n";
    }

    for(int idxBin = 0; idxBin < numAbsEtaBins; ++idxBin)
    {
      const double absEtaMin = absEtaBins[idxBin];
      const double absEtaMax = absEtaBins[idxBin + 1];
      
      HadTauFakeRateWeightEntry * hadTauFakeRateWeight = new HadTauFakeRateWeightEntry(
        absEtaMin, absEtaMax, hadTauSelection, inputFile, cfg, trigMatching, central_or_shift
      );
      if(! hadTauFakeRateWeights.count(central_or_shift))
      {
        hadTauFakeRateWeights[central_or_shift] = {};
      }
      hadTauFakeRateWeights[central_or_shift].push_back(hadTauFakeRateWeight);
    }

    isInitialized = true;
  }
}

HadTauFakeRateInterface::HadTauFakeRateInterface(const edm::ParameterSet & cfg, const std::string& trigMatching)
  : inputFile_(nullptr)
  , isInitialized_lead_(false)
  , isInitialized_sublead_(false)
  , isInitialized_third_(false)
  , isInitialized_fourth_(false)
{
  const std::string inputFileName = cfg.getParameter<std::string>("inputFileName");
  inputFile_ = openFile(LocalFileInPath(inputFileName));

  const std::string hadTauSelection = cfg.getParameter<std::string>("hadTauSelection");

  const auto initializeHadTauFRWeights = [&, this](
    const edm::ParameterSet & cfg_prio,
    const std::string& trigMatching_prio,
    std::map<int, std::vector<HadTauFakeRateWeightEntry *>> & hadTauFakeRateWeights_prio,
    bool & isInitialized,
    int FRjt_option) -> void
  {
    initializeHadTauFakeRateWeights(
      inputFile_, hadTauSelection, cfg_prio, trigMatching_prio, FRjt_option, hadTauFakeRateWeights_prio, isInitialized
    );
  };

  for(int FRjt_option = kFRjt_central; FRjt_option <= kFRjt_shapeDown; ++FRjt_option)
  {
    if(cfg.exists("lead"))
    {
      const edm::ParameterSet cfg_lead = cfg.getParameter<edm::ParameterSet>("lead");
      initializeHadTauFRWeights(cfg_lead, trigMatching, hadTauFakeRateWeights_lead_, isInitialized_lead_, FRjt_option);
    }
    if(cfg.exists("sublead"))
    {
      const edm::ParameterSet cfg_sublead = cfg.getParameter<edm::ParameterSet>("sublead");
      initializeHadTauFRWeights(cfg_sublead, trigMatching, hadTauFakeRateWeights_sublead_, isInitialized_sublead_, FRjt_option);
    }
    if(cfg.exists("third"))
    {
      const edm::ParameterSet cfg_third = cfg.getParameter<edm::ParameterSet>("third");
      initializeHadTauFRWeights(cfg_third, trigMatching, hadTauFakeRateWeights_third_, isInitialized_third_, FRjt_option);
    }
    if(cfg.exists("fourth"))
    {
      const edm::ParameterSet cfg_fourth = cfg.getParameter<edm::ParameterSet>("fourth");
      initializeHadTauFRWeights(cfg_fourth, trigMatching, hadTauFakeRateWeights_fourth_, isInitialized_fourth_, FRjt_option);
    }
  }
}

HadTauFakeRateInterface::~HadTauFakeRateInterface()
{
  for(auto & kv: hadTauFakeRateWeights_lead_)
  {
    for(HadTauFakeRateWeightEntry * & it: kv.second)
    {
      delete it;
    }
  }
  for(auto & kv: hadTauFakeRateWeights_sublead_)
  {
    for(HadTauFakeRateWeightEntry * & it: kv.second)
    {
      delete it;
    }
  }
  for(auto & kv: hadTauFakeRateWeights_third_)
  {
    for(HadTauFakeRateWeightEntry * & it: kv.second)
    {
      delete it;
    }
  }
  for(auto & kv: hadTauFakeRateWeights_fourth_)
  {
    for(HadTauFakeRateWeightEntry * & it: kv.second)
    {
      delete it;
    }
  }
  delete inputFile_;
}

double
HadTauFakeRateInterface::getWeight(int idxHadTau,
                                   double hadTauPt,
                                   double hadTauAbsEta,
                                   int central_or_shift) const
{
  return getWeight_or_SF(hadTauPt, hadTauAbsEta, kWeight, idxHadTau, central_or_shift);
}

double
HadTauFakeRateInterface::getSF(int idxHadTau,
                               double hadTauPt,
                               double hadTauAbsEta,
                               int central_or_shift) const
{
  return getWeight_or_SF(hadTauPt, hadTauAbsEta, kSF, idxHadTau, central_or_shift);
}

double
HadTauFakeRateInterface::getWeight_or_SF(double hadTauPt,
                                         double hadTauAbsEta,
                                         int mode,
                                         int order,
                                         int central_or_shift) const
{
  std::string name;
  bool isInitialized = false;
  std::vector<HadTauFakeRateWeightEntry *> j2tFRweights;
  switch(order)
  {
    case 0: name = "leading";    isInitialized = isInitialized_lead_;    j2tFRweights = hadTauFakeRateWeights_lead_.at(central_or_shift);    break;
    case 1: name = "subleading"; isInitialized = isInitialized_sublead_; j2tFRweights = hadTauFakeRateWeights_sublead_.at(central_or_shift); break;
    case 2: name = "third";      isInitialized = isInitialized_third_;   j2tFRweights = hadTauFakeRateWeights_third_.at(central_or_shift);   break;
    case 3: name = "fourth";     isInitialized = isInitialized_fourth_;  j2tFRweights = hadTauFakeRateWeights_fourth_.at(central_or_shift);  break;
    default: assert(0);
  }

  if(! isInitialized )
    throw cmsException(this, __func__)
      << "Jet->tau fake-rate weights for '" << name << "' tau requested, but not initialized";

  double weight = 1.;
  for(const HadTauFakeRateWeightEntry * const hadTauFakeRateWeightEntry: j2tFRweights)
  {
    if(hadTauAbsEta >= hadTauFakeRateWeightEntry->absEtaMin() &&
       hadTauAbsEta < hadTauFakeRateWeightEntry->absEtaMax())
    {
      switch(mode)
      {
        case kWeight: weight *= hadTauFakeRateWeightEntry->getWeight(hadTauPt); break;
        case kSF:     weight *= hadTauFakeRateWeightEntry->getSF(hadTauPt);     break;
        default: assert(0);
      }
      break;
    }
  }
  return weight;
}
