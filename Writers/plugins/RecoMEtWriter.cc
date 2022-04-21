#include "TallinnNtupleProducer/Writers/plugins/RecoMEtWriter.h"

#include "TallinnNtupleProducer/CommonTools/interface/cmsException.h"         // cmsException()
#include "TallinnNtupleProducer/CommonTools/interface/constants.h"            // met_coef, mht_coef
#include "TallinnNtupleProducer/Objects/interface/Particle.h"                 // Particle::LorentzVector
#include "TallinnNtupleProducer/Readers/interface/BranchAddressInitializer.h" // BranchAddressInitializer
#include "TallinnNtupleProducer/Readers/interface/RecoElectronReader.h"       // RecoElectronReader::get_supported_systematics()
#include "TallinnNtupleProducer/Readers/interface/RecoHadTauReader.h"         // RecoHadTauReader::get_supported_systematics()
#include "TallinnNtupleProducer/Readers/interface/RecoJetReaderAK4.h"         // RecoJetReaderAK4::get_supported_systematics()
#include "TallinnNtupleProducer/Readers/interface/RecoMEtReader.h"            // RecoMEtReader::get_supported_systematics()
#include "TallinnNtupleProducer/Readers/interface/RecoMuonReader.h"           // RecoMuonReader::get_supported_systematics()

#include "TString.h"                                                          // Form()
#include "TTree.h"                                                            // TTree

#include <assert.h>                                                           // assert()

RecoMEtWriter::RecoMEtWriter(const edm::ParameterSet & cfg)
  : WriterBase(cfg)
  , branchName_met_("met")
  , branchName_htmiss_("htmiss")
  , branchName_ht_("ht")
  , branchName_stmet_("stmet")
  , current_central_or_shiftEntry_(nullptr)
{
  merge_systematic_shifts(supported_systematics_, RecoMEtWriter::get_supported_systematics());
  merge_systematic_shifts(supported_systematics_, { "central" }); // CV: add central value
  for ( auto central_or_shift : supported_systematics_, )
  {    
    central_or_shiftEntry it;
    it.metPt_ = 0.;
    it.metPhi_ = 0.;
    it.metLD_ = 0.;
    it.htmiss_ = 0.;
    it.ht_ = 0.;
    it.stmet_ = 0.;
    central_or_shiftEntries_[central_or_shift] = it;
  }
  current_central_or_shiftEntry_ = &central_or_shiftEntries_["central"];
}

RecoMEtWriter::~RecoMEtWriter()
{}

namespace
{
  std::string
  get_branchName(const std::string & branchName, const std::string & suffix, const std::string & central_or_shift)
  {
    if ( suffix == "" )
    {
      if ( central_or_shift == "central" ) return branchName;
      else                                 return Form("%s_%s", branchName.data(), central_or_shift.data());
    }
    else
    {
      if ( central_or_shift == "central" ) return Form("%s_%s",    branchName.data(), suffix.data());
      else                                 return Form("%s_%s_%s", branchName.data(), central_or_shift.data(), suffix.data());
    }
  }
}

void
RecoMEtWriter::setBranches(TTree * tree)
{
  BranchAddressInitializer bai(tree);
  for ( auto central_or_shift : supported_systematics_ )
  {
    auto it = central_or_shiftEntries_.find(central_or_shift);
    assert(it != central_or_shiftEntries_.end());
    bai.setBranch(it->second.metPt_, get_branchName(branchName_met_, "pt",  central_or_shift));
    bai.setBranch(it->second.metPhi_, get_branchName(branchName_met_, "phi", central_or_shift));
    bai.setBranch(it->second.metLD_, get_branchName(branchName_met_, "LD", central_or_shift));
    bai.setBranch(it->second.htmiss_, get_branchName(branchName_htmiss_, "", central_or_shift));
    bai.setBranch(it->second.ht_, get_branchName(branchName_ht_, "", central_or_shift));
    bai.setBranch(it->second.stmet_, get_branchName(branchName_stmet_, "", central_or_shift));
  }
}

void
RecoMEtWriter::set_central_or_shift(const std::string & central_or_shift) const
{
  WriterBase::set_central_or_shift(central_or_shift);
  auto it = central_or_shiftEntries_.find(central_or_shift);
  if ( it != central_or_shiftEntries_.end() )
  {
    current_central_or_shiftEntry_ = const_cast<central_or_shiftEntry *>(&it->second);
  }
  else throw cmsException(__func__, __LINE__) 
    << "Invalid systematic shift = '" << central_or_shift << "' !!";
}

namespace
{
  Particle::LorentzVector
  compMHT(const std::vector<const RecoLepton *> & leptons,
          const std::vector<const RecoHadTau *> & hadTaus,
          const std::vector<const RecoJetAK4 *> & jets)
  {
    Particle::LorentzVector mhtP4(0,0,0,0);
    for ( auto lepton : leptons )
    {
      mhtP4 += lepton->p4();
    }
    for ( auto hadTau : hadTaus )
    {
      mhtP4 += hadTau->p4();
    }
    for ( auto jet : jets )
    {
      mhtP4 += jet->p4();
    }
    return mhtP4;
  }

  /**
   * @brief Compute MEt linear discriminant (cf. Eq. (2) in AN-2019/111 v14)
   */
  double
  compMEt_LD(const Particle::LorentzVector & metP4,
             const Particle::LorentzVector & mhtP4)
  {
    return met_coef * metP4.pt() + mht_coef * mhtP4.pt();
  }

  /**
   * @brief Compute scalar HT observable
   */
  double
  compHT(const std::vector<const RecoLepton *> & leptons,
         const std::vector<const RecoHadTau *> & hadTaus,
         const std::vector<const RecoJetAK4 *> & jets)
  {
    double ht = 0.;
    for ( auto lepton : leptons )
    {
      ht += lepton->pt();
    }
    for ( auto hadTau : hadTaus)
    {
      ht += hadTau->pt();
    }
    for ( auto jet : jets)
    {
      ht += jet->pt();
    }
    return ht;
  }

  /**
   * @brief Compute STMET observable (used in e.g. EXO-17-016 paper)
   */
  double
  compSTMEt(const std::vector<const RecoLepton *> & leptons,
            const std::vector<const RecoHadTau *> & hadTaus,
            const std::vector<const RecoJetAK4 *> & jets,
            const Particle::LorentzVector & metP4)
  {
    double stmet = compHT(leptons, hadTaus, jets) + metP4.pt();
    return stmet;
  }
}

void
RecoMEtWriter::writeImp(const Event & event, const EvtWeightRecorder & evtWeightRecorder)
{
  assert(current_central_or_shiftEntry_);
  const RecoMEt& met = event.met();
  auto it = current_central_or_shiftEntry_;
  it->metPt_ = met.pt();
  it->metPhi_ = met.phi();
  const Particle::LorentzVector mhtP4 = compMHT(event.fakeableLeptons(), event.fakeableHadTaus(), event.selJetsAK4());
  it->metLD_ = compMEt_LD(met.p4(), mhtP4);
  it->htmiss_ = mhtP4.pt();
  it->ht_ = compHT(event.fakeableLeptons(), event.fakeableHadTaus(), event.selJetsAK4());
  it->stmet_ = compSTMEt(event.fakeableLeptons(), event.fakeableHadTaus(), event.selJetsAK4(), met.p4());
}

std::vector<std::string>
RecoMEtWriter::get_supported_systematics()
{
  std::vector<std::string> supported_systematics;
  merge_systematic_shifts(supported_systematics, RecoElectronReader::get_supported_systematics());
  merge_systematic_shifts(supported_systematics, RecoHadTauReader::get_supported_systematics());
  merge_systematic_shifts(supported_systematics, RecoJetReaderAK4::get_supported_systematics());
  merge_systematic_shifts(supported_systematics, RecoMEtReader::get_supported_systematics());
  merge_systematic_shifts(supported_systematics, RecoMuonReader::get_supported_systematics());
  return supported_systematics;
}

DEFINE_EDM_PLUGIN(WriterPluginFactory, RecoMEtWriter, "RecoMEtWriter");
