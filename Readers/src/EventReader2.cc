#include "TallinnNtupleProducer/Readers/interface/EventReader2.h"

#include "TallinnNtupleProducer/CommonTools/interface/cmsException.h"             // cmsException
#include "TallinnNtupleProducer/CommonTools/interface/contains.h"                 // contains()
#include "TallinnNtupleProducer/CommonTools/interface/Era.h"                      // Era, get_era()
#include "TallinnNtupleProducer/CommonTools/interface/merge_systematic_shifts.h"  // merge_systematic_shifts()
#include "TallinnNtupleProducer/CommonTools/interface/sysUncertOptions.h"         // getHadTauPt_option(), getFatJet_option(), getJet_option(), getBTagWeight_option(), getMET_option()
#include "TallinnNtupleProducer/Readers/interface/convert_to_ptrs.h"              // convert_to_ptrs()

namespace
{
  edm::ParameterSet
  make_cfg(const edm::ParameterSet& cfg, const std::string & attr_branchName)
  {
    edm::ParameterSet cfg_modified(cfg);
    std::string branchName = cfg.getParameter<std::string>(attr_branchName);
    cfg_modified.addParameter<std::string>("branchName", branchName);
    return cfg_modified;
  }
}

EventReader2::EventReader2(const edm::ParameterSet& cfg)
  : ReaderBase(cfg)
  //, cfg_(cfg)
  , numNominalLeptons_(0)
  , numNominalHadTaus_(0)
  , era_(Era::kUndefined)
  , isMC_(false)
  , readGenMatching_(false)
  , metReader_(nullptr)
  , isDEBUG_(cfg.getParameter<bool>("isDEBUG"))
{
std::cout << "<EventReader2::EventReader2>:" << std::endl;
std::cout << "break-point A.1 reached" << std::endl;
  numNominalLeptons_ = cfg.getParameter<unsigned>("numNominalLeptons");
  numNominalHadTaus_ = cfg.getParameter<unsigned>("numNominalHadTaus");
std::cout << "break-point A.2 reached" << std::endl;
  era_ = get_era(cfg.getParameter<std::string>("era"));
  isMC_ = cfg.getParameter<bool>("isMC");
  readGenMatching_ = isMC_ && !cfg.getParameter<bool>("redoGenMatching");
std::cout << "break-point A.3 reached" << std::endl;
  metReader_ = new RecoMEtReader(make_cfg(cfg, "branchName_met"));
std::cout << "break-point A.4 reached" << std::endl;
}

EventReader2::~EventReader2()
{
std::cout << "<EventReader2::~EventReader2>:" << std::endl;
  delete metReader_;
}

void
EventReader2::set_central_or_shift(const std::string& central_or_shift)
{
std::cout << "<EventReader2::set_central_or_shift>:" << std::endl;
std::cout << "central_or_shift = '" << central_or_shift << "'" << std::endl;
  //if ( central_or_shift == "central" || contains(metReader_->get_supported_systematics(cfg_), central_or_shift) )
  //{
  //  const int met_option = getMET_option(central_or_shift, isMC_);
  //  metReader_->setMEt_central_or_shift(met_option);
  //}
  metReader_->setMEt_central_or_shift(kJetMET_central);
}

std::vector<std::string>
EventReader2::setBranchAddresses(TTree * inputTree)
{
std::cout << "<EventReader2::setBranchAddresses>:" << std::endl;
  metReader_->setBranchAddresses(inputTree);
  return {};
}

void
EventReader2::read() const
{
std::cout << "<EventReader2::read>:" << std::endl;
  const RecoMEt met = metReader_->read();
std::cout << "met_pt(4) = " << met.pt() << std::endl;
}

std::vector<std::string>
EventReader2::get_supported_systematics(const edm::ParameterSet & cfg)
{
  std::vector<std::string> supported_systematics;
  merge_systematic_shifts(supported_systematics, RecoMEtReader::get_supported_systematics(cfg));
  return supported_systematics;
}
