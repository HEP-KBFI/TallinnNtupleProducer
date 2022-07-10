#include "TallinnNtupleProducer/Objects/interface/EventInfo.h"

#include "TallinnNtupleProducer/CommonTools/interface/cmsException.h"    // cmsException()
#include "TallinnNtupleProducer/CommonTools/interface/tH_auxFunctions.h" // get_tH_weight_str()

#include <boost/range/adaptor/map.hpp>                                   // boost::adaptors::map_keys
#include <boost/range/algorithm/copy.hpp>                                // boost::copy()

#include <assert.h>                                                      // assert()
#include <cmath>                                                         // std::fabs()
#include <cstring>                                                       // std::memcpy()
#include <sstream>                                                       // std::stringstream

#define EPS 1E-2

const std::map<Int_t, std::string> EventInfo::gProductionMode_idString_singleHiggs = {
  { 23000025, "ZH" },
  { 24000025, "WH" },
};

EventInfo::EventInfo()
  : analysisConfig_(nullptr)
  , central_or_shift_("central")
  , process_string_("")
  , run_(0)
  , lumi_(0)
  , event_(0)
  , genWeight_(1.)
  , pileupWeight_(1.)
  , nLHEReweightingWeight_(0)
  , LHEReweightingWeight_(nullptr)
  , LHEReweightingWeight_max_(69)
  , is_owner_(false)
  , read_htxs_(false)
  , refGenWeight_(0.)
  , productionMode_(-1)
{}

EventInfo::EventInfo(const AnalysisConfig & analysisConfig)
  : EventInfo()
{
  analysisConfig_ = &analysisConfig;
  process_string_ = analysisConfig.process();
  int checksum = 0;
  assert(analysisConfig_);
  if ( analysisConfig_->isMC_H()  ) ++checksum;
  if ( analysisConfig_->isMC_HH() ) ++checksum;
  if ( !(checksum == 0 || (checksum == 1 && analysisConfig_->isMC())) )
    throw cmsException(this, __func__, __LINE__)
      << "Invalid combination of Configuration parameters:\n" 
      << " isMC = " << analysisConfig_->isMC() << "\n"
      << " isMC_H = " << analysisConfig_->isMC_H() << "\n"
      << " isMC_HH = " << analysisConfig_->isMC_HH() << "\n";
}

EventInfo::EventInfo(const EventInfo & eventInfo)
  : EventInfo()
{
  copy(eventInfo);
}

EventInfo &
EventInfo::operator=(const EventInfo & eventInfo)
{
  copy(eventInfo);
  return *this;
}

void
EventInfo::copy(const EventInfo & eventInfo)
{
  analysisConfig_ = eventInfo.analysisConfig_;
  run_ = eventInfo.run_;
  lumi_ = eventInfo.lumi_;
  event_ = eventInfo.event_;
  genWeight_ = eventInfo.genWeight_;
  nLHEReweightingWeight_ = eventInfo.nLHEReweightingWeight_;
  if(eventInfo.LHEReweightingWeight_)
  {
    if(LHEReweightingWeight_)
    {
      if(is_owner_)
      {
        delete[] LHEReweightingWeight_;
        LHEReweightingWeight_ = nullptr;
      }
      else
      {
        throw cmsException(this, __func__, __LINE__)
          << "Cannot overwrite with instance" << eventInfo << " because this instance doesn't own LHEReweightingWeight"
        ;
      }
    }
    LHEReweightingWeight_ = new Float_t[LHEReweightingWeight_max_];
    is_owner_ = true;
    std::memcpy(LHEReweightingWeight_, eventInfo.LHEReweightingWeight_, nLHEReweightingWeight_);
  }
  htxs_ = eventInfo.htxs_;
}

EventInfo::~EventInfo()
{}

UInt_t
EventInfo::run() const
{
  return run_;
}

UInt_t
EventInfo::lumi() const
{
  return lumi_;
}

ULong64_t
EventInfo::event() const
{
  return event_;
}

Float_t
EventInfo::genWeight() const
{
  return genWeight_;
}

Float_t
EventInfo::pileupWeight() const
{
  return pileupWeight_;
}

Float_t
EventInfo::pileupWeightUp() const
{
  return pileupWeightUp_;
}

Float_t
EventInfo::pileupWeightDown() const
{
  return pileupWeightDown_;
}

const AnalysisConfig &
EventInfo::analysisConfig() const
{
  assert(analysisConfig_);
  return *analysisConfig_;
}

double
EventInfo::genWeight_tH() const
{
  return genWeight_tH(central_or_shift_);
}

double
EventInfo::genWeight_tH(double kv,
                        double kt,
                        double cosa) const
{
  return genWeight_tH(central_or_shift_, get_tH_weight_str(kv, kt, cosa));
}

double
EventInfo::genWeight_tH(const std::string & central_or_shift) const
{
  return genWeight_tH(central_or_shift, get_tH_SM_str());
}

double
EventInfo::genWeight_tH(const std::string & central_or_shfit,
                        const std::string & name) const
{
  if(tH_sf_.empty())
  {
    return 1.;
  }
  if(! tH_sf_.count(central_or_shfit))
  {
    throw cmsException(this, __func__, __LINE__) << "No such central or shift option: " << central_or_shfit;
  }
  if(! tH_sf_.at(central_or_shfit).count(name))
  {
    throw cmsException(this, __func__, __LINE__)
      << "Invalid coupling requested for central or shift option " << central_or_shfit << ": " << name
    ;
  }
  const std::pair<int, double> & settings = tH_sf_.at(central_or_shfit).at(name);
  assert(settings.first < static_cast<int>(nLHEReweightingWeight_));

  return (settings.first >= 0 ? LHEReweightingWeight_[settings.first] : 1.) * settings.second;
}

void
EventInfo::loadWeight_tH(const std::vector<edm::ParameterSet> & cfg)
{
  std::map<std::string, int> main_map;
  for(const edm::ParameterSet & cfg_entry: cfg)
  {
    const double weight = cfg_entry.getParameter<double>("weight");
    const double kt = cfg_entry.getParameter<double>("kt");
    const double kv = cfg_entry.getParameter<double>("kv");
    const double cosa = cfg_entry.exists("cosa") ?
      cfg_entry.getParameter<double>("cosa")     :
      std::numeric_limits<double>::quiet_NaN()
    ;
    const std::string central_or_shift = cfg_entry.getParameter<std::string>("central_or_shift");
    const int idx = cfg_entry.getParameter<int>("idx");
    const std::string name = get_tH_weight_str(kt, kv, cosa);

    // make sure that we can recover the paramters encoded by the string
    const std::tuple<double, double, double> kt_kv_cosa = get_tH_params(name);
    assert(std::get<0>(kt_kv_cosa) == kt);
    assert(std::get<1>(kt_kv_cosa) == kv);
    if(! std::isnan(cosa))
    {
      assert(std::get<2>(kt_kv_cosa) == cosa);
    }
    if(! has_central_or_shift(central_or_shift))
    {
      tH_sf_[central_or_shift] = {};
    }

    assert(! tH_sf_.at(central_or_shift).count(name));
    tH_sf_[central_or_shift][name] = std::pair<int, double>(idx, weight);
    std::cout
      << "Loaded weight '" << name << "': kt = " << kt << " & kv = " << kv << " & cosa = " << cosa
      << " for systematic option " << central_or_shift
      << " -> weight = " << weight <<" @ idx = " << idx << '\n'
    ;
  }
}

std::vector<std::string>
EventInfo::getWeight_tH_str(bool include_sm) const
{
  return getWeight_tH_str(central_or_shift_, include_sm);
}

std::vector<std::string>
EventInfo::getWeight_tH_str(const std::string & central_or_shift,
                            bool include_sm) const
{
  std::vector<std::string> names;
  const std::string sm_str = get_tH_SM_str();
  if(! has_central_or_shift(central_or_shift))
  {
    throw cmsException(this, __func__, __LINE__) << "No such central or shift option: " << central_or_shift;
  }
  for(const auto & kv: tH_sf_.at(central_or_shift))
  {
    if(! include_sm && kv.first == sm_str)
    {
      continue;
    }
    names.push_back(kv.first);
  }
  return names;
}

void
EventInfo::set_refGetWeight(double refGenWeight)
{
  refGenWeight_ = std::fabs(refGenWeight);
}

double
EventInfo::get_refGenWeight() const
{
  return refGenWeight_;
}

void
EventInfo::set_central_or_shift(const std::string & central_or_shift) const
{
  central_or_shift_ = central_or_shift;
}

bool
EventInfo::has_central_or_shift(const std::string & central_or_shift) const
{
  return tH_sf_.count(central_or_shift);
}

bool
EventInfo::read_htxs() const
{
  return read_htxs_;
}

void
EventInfo::read_htxs(bool flag)
{
  read_htxs_ = flag;
}

int
EventInfo::get_htxs_category() const
{
  return read_htxs_ ? htxs_.get_category() : HTXSCategory::kUnknown;
}

bool
EventInfo::is_initialized() const
{
  return run_ != 0 && lumi_ != 0 && event_ != 0;
}

std::string
EventInfo::getProductionModeString() const
{
  assert(analysisConfig_);
  if(! analysisConfig_->isMC_H())
  {
    throw cmsException(this, __func__, __LINE__)
      << "The event " << *this << " is not a H signal event => request "
         "for a production mode as a string is not applicable\n"
    ;
  }
  return
    gProductionMode_idString_singleHiggs.count(productionMode_) ?
    gProductionMode_idString_singleHiggs.at(productionMode_)    :
    ""
  ;
}

void
EventInfo::reset_productionMode()
{
  set_productionMode(-1);
}

void
EventInfo::set_productionMode(int productionMode)
{
  productionMode_ = productionMode;
}

std::string
EventInfo::str() const
{
  std::stringstream ss;
  ss << run_ << ':' << lumi_ << ':' << event_;
  return ss.str();
}

std::ostream &
operator<<(std::ostream & os,
           const EventInfo & info)
{
  os << "run = " << info.run() << ", ls = " << info.lumi() << ", event = " << info.event();
  return os;
}
