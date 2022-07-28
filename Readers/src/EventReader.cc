#include "TallinnNtupleProducer/Readers/interface/EventReader.h"

#include "TallinnNtupleProducer/CommonTools/interface/cmsException.h"             // cmsException
#include "TallinnNtupleProducer/CommonTools/interface/contains.h"                 // contains()
#include "TallinnNtupleProducer/CommonTools/interface/Era.h"                      // Era, get_era()
#include "TallinnNtupleProducer/CommonTools/interface/hadTauDefinitions.h"        // get_tau_id_wp_int()
#include "TallinnNtupleProducer/CommonTools/interface/isHigherPt.h"               // isHigherPt()
#include "TallinnNtupleProducer/CommonTools/interface/merge_systematic_shifts.h"  // merge_systematic_shifts()
#include "TallinnNtupleProducer/CommonTools/interface/pickFirstNobjects.h"        // pickFirstNobjects()
#include "TallinnNtupleProducer/CommonTools/interface/sysUncertOptions.h"         // getHadTauPt_option(), getFatJet_option(), getJet_option(), getMET_option()
#include "TallinnNtupleProducer/Objects/interface/printCollection.h"              // printCollection()
#include "TallinnNtupleProducer/Objects/interface/RunLumiEvent.h"                 // RunLumiEvent
#include "TallinnNtupleProducer/Readers/interface/convert_to_ptrs.h"              // convert_to_ptrs()
#include "TallinnNtupleProducer/Readers/interface/setFilterBits.h"                // setFilterBits()
#include "TallinnNtupleProducer/Readers/interface/setJetBtagScore.h"              // setJetBtagScore

#include "TallinnNtupleProducer/Readers/interface/RunLumiEventReader.h"                       // RunLumiEventReader
#include "TallinnNtupleProducer/Readers/interface/TriggerInfoReader.h"                        // TriggerInfoReader
#include "TallinnNtupleProducer/Readers/interface/EventInfoReader.h"                          // EventInfoReader
#include "TallinnNtupleProducer/Readers/interface/GenHadTauReader.h"                          // GenHadTauReader
#include "TallinnNtupleProducer/Readers/interface/GenJetReader.h"                             // GenJetReader
#include "TallinnNtupleProducer/Readers/interface/CorrT1METJetReader.h"                       // CorrT1METJetReader
#include "TallinnNtupleProducer/Readers/interface/GenParticleReader.h"                        // GenParticleReader
#include "TallinnNtupleProducer/Readers/interface/MEtFilterReader.h"                          // MEtFilterReader
#include "TallinnNtupleProducer/Selectors/interface/RecoElectronCollectionSelectorFakeable.h" // RecoElectronCollectionSelectorFakeable
#include "TallinnNtupleProducer/Selectors/interface/RecoElectronCollectionSelectorLoose.h"    // RecoElectronCollectionSelectorLoose
#include "TallinnNtupleProducer/Selectors/interface/RecoElectronCollectionSelectorTight.h"    // RecoElectronCollectionSelectorTight
#include "TallinnNtupleProducer/Selectors/interface/RecoMuonCollectionSelectorFakeable.h"     // RecoMuonCollectionSelectorFakeable
#include "TallinnNtupleProducer/Selectors/interface/RecoMuonCollectionSelectorTight.h"        // RecoMuonCollectionSelectorTight
#include "TallinnNtupleProducer/Readers/interface/RecoElectronReader.h"                       // RecoElectronReader
#include "TallinnNtupleProducer/Readers/interface/RecoHadTauReader.h"                         // RecoHadTauReader
#include "TallinnNtupleProducer/Readers/interface/GenMEtReader.h"                             // GenMEtReader
#include "TallinnNtupleProducer/Readers/interface/RecoMEtReader.h"                            // RecoMEtReader
#include "TallinnNtupleProducer/Readers/interface/RecoMuonReader.h"                           // RecoMuonReader
#include "TallinnNtupleProducer/Readers/interface/RecoVertexReader.h"                         // RecoVertexReader
#include "TallinnNtupleProducer/Readers/interface/RecoJetReaderAK4.h"                         // RecoJetReaderAK4
#include "TallinnNtupleProducer/Readers/interface/RecoJetReaderAK8.h"                         // RecoJetReaderAK8
#include "TallinnNtupleProducer/Readers/interface/ParticleReader.h"                           // ParticleReader
#include "TallinnNtupleProducer/Readers/interface/JMECorrector.h"                             // JMECorrector

namespace
{
  std::set<std::string>
  make_supported_systematics(const std::vector<std::string> & systematic_shifts)
  {
    std::vector<std::string> supported_systematics = systematic_shifts;
    // CV: add central value
    merge_systematic_shifts(supported_systematics, { "central" });
    std::set<std::string> retVal(supported_systematics.begin(), supported_systematics.end());
    return retVal;
  }

  edm::ParameterSet
  make_cfg(const edm::ParameterSet& cfg, const std::string & attr_branchName)
  {
    edm::ParameterSet retVal(cfg);
    std::string branchName = cfg.getParameter<std::string>(attr_branchName);
    retVal.addParameter<std::string>("branchName", branchName);
    return retVal;
  }

  edm::ParameterSet
  make_cfg_jetsAK8(const edm::ParameterSet& cfg, const std::string & attr_branchName_jet, const std::string & attr_branchName_subjet)
  {
    edm::ParameterSet retVal(cfg);
    std::string branchName_jet = cfg.getParameter<std::string>(attr_branchName_jet);
    retVal.addParameter<std::string>("branchName_jet", branchName_jet);
    std::string branchName_subjet = cfg.getParameter<std::string>(attr_branchName_subjet);
    retVal.addParameter<std::string>("branchName_subjet", branchName_subjet);
    return retVal;
  }
}

EventReader::EventReader(const edm::ParameterSet& cfg)
  : ReaderBase(cfg)
  , cfg_(cfg)
  , isDEBUG_(cfg.getParameter<bool>("isDEBUG"))
  , numNominalLeptons_(cfg_.getParameter<unsigned>("numNominalLeptons"))
  , applyNumNominalLeptonsCut_(cfg_.getParameter<bool>("applyNumNominalLeptonsCut"))
  , numNominalHadTaus_(cfg_.getParameter<unsigned>("numNominalHadTaus"))
  , applyNumNominalHadTausCut_(cfg_.getParameter<bool>("applyNumNominalHadTausCut"))
  , era_(get_era(cfg_.getParameter<std::string>("era")))
  , isMC_(cfg_.getParameter<bool>("isMC"))
  , jetCleaningByIndex_(cfg_.getParameter<bool>("jetCleaningByIndex"))
  , genMatchRecoJets_(cfg_.getParameter<bool>("genMatchRecoJets"))
  , lastRun_(0)
  , lastLumi_(0)
  , lastEvent_(0)
  , current_central_or_shift_("central")
  , runLumiEventReader_(new RunLumiEventReader(cfg_))
  , eventInfoReader_(new EventInfoReader(cfg_))
  , eventInfo_supported_systematics_(make_supported_systematics(EventInfoReader::get_supported_systematics(cfg_)))
  , triggerInfoReader_(new TriggerInfoReader(cfg_.getParameter<edm::ParameterSet>("triggers")))
  , triggerInfo_supported_systematics_(make_supported_systematics(TriggerInfoReader::get_supported_systematics(cfg_)))
  , muonReader_(new RecoMuonReader(make_cfg(cfg_, "branchName_muons")))
  , looseMuonSelector_(new RecoMuonCollectionSelectorLoose(era_, -1, isDEBUG_))
  , fakeableMuonSelector_(new RecoMuonCollectionSelectorFakeable(era_, -1, isDEBUG_))
  , tightMuonSelector_(new RecoMuonCollectionSelectorTight(era_, -1, isDEBUG_))
  , muon_supported_systematics_(make_supported_systematics(RecoMuonReader::get_supported_systematics(cfg_)))
  , electronReader_(new RecoElectronReader(make_cfg(cfg_, "branchName_electrons")))
  , electronCleaner_(0.3, isDEBUG_)
  , looseElectronSelector_(new RecoElectronCollectionSelectorLoose(era_, -1, isDEBUG_))
  , fakeableElectronSelector_(new RecoElectronCollectionSelectorFakeable(era_, -1, isDEBUG_))
  , tightElectronSelector_(new RecoElectronCollectionSelectorTight(era_, -1, isDEBUG_))
  , electron_supported_systematics_(make_supported_systematics(RecoElectronReader::get_supported_systematics(cfg_)))
  , hadTauReader_(new RecoHadTauReader(make_cfg(cfg_, "branchName_hadTaus")))
  , hadTauCleaner_(0.3, isDEBUG_)
  , looseHadTauSelector_(new RecoHadTauCollectionSelectorLoose(era_, -1, isDEBUG_))
  , fakeableHadTauSelector_(new RecoHadTauCollectionSelectorFakeable(era_, -1, isDEBUG_))
  , tightHadTauSelector_(new RecoHadTauCollectionSelectorTight(era_, -1, isDEBUG_))
  , hadTau_supported_systematics_(make_supported_systematics(RecoHadTauReader::get_supported_systematics(cfg)))
  , hadTau_isInvalid_(false)
  , jmeCorrector_(new JMECorrector(cfg_))
  , jetReaderAK4_(new RecoJetReaderAK4(make_cfg(cfg_, "branchName_jets_ak4")))
  , jetCleanerAK4_dR04_(0.4, isDEBUG_)
  , jetCleanerAK4ByIndex_dR04_(isDEBUG_)
  , jetCleanerAK4_dR12_(1.2, isDEBUG_)
  , jetSelectorAK4_(new RecoJetCollectionSelectorAK4(era_, -1, isDEBUG_))
  , jetSelectorAK4_btagLoose_(new RecoJetCollectionSelectorAK4_btagLoose(era_, -1, isDEBUG_))
  , jetSelectorAK4_btagMedium_(new RecoJetCollectionSelectorAK4_btagMedium(era_, -1, isDEBUG_))
  , jetsAK4_supported_systematics_(make_supported_systematics(RecoJetReaderAK4::get_supported_systematics(cfg)))
  , jetAK4_isInvalid_(false)
  , apply_pileupJetID_(get_pileupJetID(cfg_.getParameter<std::string>("apply_pileupJetID")))
  , genParticleReader_(nullptr)
  , genHadTauReader_(nullptr)
  , genJetReader_(nullptr)
  , corrT1METJetReader_(nullptr)
  , muonGenMatcher_(nullptr)
  , electronGenMatcher_(nullptr)
  , hadTauGenMatcher_(nullptr)
  , jetGenMatcherAK4_(nullptr)
  , corrT1METJetGenMatcher_(nullptr)
  , jetReaderAK8_(new RecoJetReaderAK8(make_cfg_jetsAK8(cfg_, "branchName_jets_ak8", "branchName_subjets_ak8")))
  , jetCleanerAK8_dR08_(0.8, isDEBUG_)
  , jetSelectorAK8_Hbb_(new RecoJetCollectionSelectorAK8_Hbb(era_, -1, isDEBUG_))
  , jetSelectorAK8_Wjj_(new RecoJetCollectionSelectorAK8_Wjj(era_, -1, isDEBUG_))
  , jetsAK8_Hbb_supported_systematics_(make_supported_systematics(RecoJetReaderAK8::get_supported_systematics(cfg_)))
  , jetAK8_Hbb_isInvalid_(false)
  , jetsAK8_Wjj_supported_systematics_(make_supported_systematics(RecoJetReaderAK8::get_supported_systematics(cfg_)))
  , jetAK8_Wjj_isInvalid_(false)
  , genJetAK8Reader_(nullptr)
  , genSubJetAK8Reader_(nullptr)
  , rawmetReader_(new GenMEtReader(make_cfg(cfg, "branchName_rawMet")))
  , metReader_(new RecoMEtReader(make_cfg(cfg, "branchName_met")))
  , met_supported_systematics_(make_supported_systematics(RecoMEtReader::get_supported_systematics(cfg_)))
  , met_isInvalid_(false)
  , metFilterReader_(new MEtFilterReader(cfg))
  , metFilter_supported_systematics_(make_supported_systematics(MEtFilterReader::get_supported_systematics(cfg_)))
  , metFilter_isInvalid_(false)
  , vertexReader_(new RecoVertexReader(make_cfg(cfg, "branchName_vertex")))
  , vertex_supported_systematics_(make_supported_systematics(RecoVertexReader::get_supported_systematics(cfg_)))
  , vertex_isInvalid_(false)
{
  const double lep_mva_cut_mu = cfg_.getParameter<double>("lep_mva_cut_mu");
  muonReader_->set_mvaTTH_wp(lep_mva_cut_mu);

  const double lep_mva_cut_e = cfg_.getParameter<double>("lep_mva_cut_e");
  electronReader_->set_mvaTTH_wp(lep_mva_cut_e);

  hadTauReader_->setHadTauPt_central_or_shift(kHadTauPt_central);

  const std::string hadTauWP_againstJets_fakeable = cfg.getParameter<std::string>("hadTauWP_againstJets_fakeable");
  const std::string hadTauWP_againstJets_tight = cfg.getParameter<std::string>("hadTauWP_againstJets_tight");
  if ( get_tau_id_wp_int(hadTauWP_againstJets_tight) <= get_tau_id_wp_int(hadTauWP_againstJets_fakeable) )
  {
    throw cmsException(this, __func__, __LINE__) << "Selection of 'tight' taus must be more stringent than selection of 'fakeable' taus !!";
  }
  fakeableHadTauSelector_->set(hadTauWP_againstJets_fakeable);
  tightHadTauSelector_->set(hadTauWP_againstJets_tight);
  const int hadTauWP_againstElectrons = get_tau_id_wp_int(cfg.getParameter<std::string>("hadTauWP_againstElectrons"));
  fakeableHadTauSelector_->set_min_antiElectron(hadTauWP_againstElectrons);
  tightHadTauSelector_->set_min_antiElectron(hadTauWP_againstElectrons);
  const int hadTauWP_againstMuons = get_tau_id_wp_int(cfg.getParameter<std::string>("hadTauWP_againstMuons"));
  fakeableHadTauSelector_->set_min_antiMuon(hadTauWP_againstMuons);
  tightHadTauSelector_->set_min_antiMuon(hadTauWP_againstMuons);
  if ( applyNumNominalHadTausCut_ )
  {
    // CV: relax tau pT cut from 20 to 19 GeV, to allow for tau energy scale shifts
    std::cout << "Relaxing tau pT cut to 19 GeV, as applyNumNominalHadTausCut is enabled\n";
    fakeableHadTauSelector_->set_min_pt(19.);
    tightHadTauSelector_->set_min_pt(19.);
  }

  jetSelectorAK4_->getSelector().set_pileupJetId(apply_pileupJetID_);
  jetSelectorAK4_btagLoose_->getSelector().set_pileupJetId(apply_pileupJetID_);
  jetSelectorAK4_btagMedium_->getSelector().set_pileupJetId(apply_pileupJetID_);

  if ( isMC_ )
  {
    genParticleReader_ = new GenParticleReader(make_cfg(cfg, "branchName_genPart"));
    genHadTauReader_ = new GenHadTauReader(make_cfg(cfg, "branchName_genHadTaus"));
    genJetReader_ = new GenJetReader(make_cfg(cfg, "branchName_genJets"));
    corrT1METJetReader_ = new CorrT1METJetReader(make_cfg(cfg, "branchName_corrT1METJet"));
    genJetAK8Reader_ = new ParticleReader(make_cfg(cfg, "branchName_genJetAK8"));
    genSubJetAK8Reader_ = new ParticleReader(make_cfg(cfg, "branchName_genSubJetAK8"));

    muonGenMatcher_ = new RecoMuonCollectionGenMatcher();
    electronGenMatcher_ = new RecoElectronCollectionGenMatcher();
    hadTauGenMatcher_ = new RecoHadTauCollectionGenMatcher();
    jetGenMatcherAK4_ = new RecoJetCollectionGenMatcherAK4();
    corrT1METJetGenMatcher_ = new CorrT1METJetCollectionGenMatcher();
  }
}

EventReader::~EventReader()
{
  delete triggerInfoReader_;
  delete muonReader_;
  delete looseMuonSelector_;
  delete fakeableMuonSelector_;
  delete tightMuonSelector_;
  delete electronReader_;
  delete looseElectronSelector_;
  delete fakeableElectronSelector_;
  delete tightElectronSelector_;
  delete hadTauReader_;
  delete looseHadTauSelector_;
  delete fakeableHadTauSelector_;
  delete tightHadTauSelector_;
  delete jmeCorrector_;
  delete jetReaderAK4_;
  delete jetSelectorAK4_;
  delete jetSelectorAK4_btagLoose_;
  delete jetSelectorAK4_btagMedium_;
  delete genParticleReader_;
  delete genHadTauReader_;
  delete genJetReader_;
  delete corrT1METJetReader_;
  delete genJetAK8Reader_;
  delete genSubJetAK8Reader_;
  delete muonGenMatcher_;
  delete electronGenMatcher_;
  delete hadTauGenMatcher_;
  delete jetGenMatcherAK4_;
  delete corrT1METJetGenMatcher_;
  delete jetReaderAK8_;
  delete jetSelectorAK8_Hbb_;
  delete jetSelectorAK8_Wjj_;
  delete rawmetReader_;
  delete metReader_;
  delete metFilterReader_;
  delete vertexReader_;
}

void
EventReader::set_central_or_shift(const std::string& central_or_shift)
{
  eventInfoReader_->set_central_or_shift(central_or_shift);
  hadTauReader_->setHadTauPt_central_or_shift(getHadTauPt_option(central_or_shift));
  jmeCorrector_->set_opt(
    getJet_option(central_or_shift, isMC_),
    getMET_option(central_or_shift, isMC_),
    getFatJet_option(central_or_shift, isMC_)
  );
  current_central_or_shift_ = central_or_shift;
}

std::vector<std::string>
EventReader::setBranchAddresses(TTree * inputTree)
{
  const std::vector<std::string> eventInfoBranches = eventInfoReader_->setBranchAddresses(inputTree);
  const std::vector<std::string> triggerInfoBranches = triggerInfoReader_->setBranchAddresses(inputTree);
  const std::vector<std::string> muonBranches = muonReader_->setBranchAddresses(inputTree);
  const std::vector<std::string> electronBranches = electronReader_->setBranchAddresses(inputTree);
  const std::vector<std::string> hadTauBranches = hadTauReader_->setBranchAddresses(inputTree);
  const std::vector<std::string> jetBranchesAK4 = jetReaderAK4_->setBranchAddresses(inputTree);

  // TODO by the looks of it, it won't work on data
  const std::vector<std::string> genPartBranches = genParticleReader_->setBranchAddresses(inputTree);
  const std::vector<std::string> genHadTauBranches = genHadTauReader_->setBranchAddresses(inputTree);
  const std::vector<std::string> genJetBranches = genJetReader_->setBranchAddresses(inputTree);
  const std::vector<std::string> corrT1METJetBranches = corrT1METJetReader_->setBranchAddresses(inputTree);
  const std::vector<std::string> genJetAKBranches = genJetAK8Reader_->setBranchAddresses(inputTree);
  const std::vector<std::string> genSubJetAKBranches = genSubJetAK8Reader_->setBranchAddresses(inputTree);

  const std::vector<std::string> jetBranchesAK8 = jetReaderAK8_->setBranchAddresses(inputTree);
  const std::vector<std::string> rawmetBranches = rawmetReader_->setBranchAddresses(inputTree);
  const std::vector<std::string> metBranches = metReader_->setBranchAddresses(inputTree);
  const std::vector<std::string> metFilterBranches = metFilterReader_->setBranchAddresses(inputTree);
  const std::vector<std::string> vertexBranches = vertexReader_->setBranchAddresses(inputTree);

  std::vector<std::string> bound_branches;
  bound_branches.insert(bound_branches.end(), eventInfoBranches.begin(), eventInfoBranches.end());
  bound_branches.insert(bound_branches.end(), triggerInfoBranches.begin(), triggerInfoBranches.end());
  bound_branches.insert(bound_branches.end(), muonBranches.begin(), muonBranches.end());
  bound_branches.insert(bound_branches.end(), electronBranches.begin(), electronBranches.end());
  bound_branches.insert(bound_branches.end(), hadTauBranches.begin(), hadTauBranches.end());
  bound_branches.insert(bound_branches.end(), jetBranchesAK4.begin(), jetBranchesAK4.end());

  bound_branches.insert(bound_branches.end(), genPartBranches.begin(), genPartBranches.end());
  bound_branches.insert(bound_branches.end(), genHadTauBranches.begin(), genHadTauBranches.end());
  bound_branches.insert(bound_branches.end(), genJetBranches.begin(), genJetBranches.end());
  bound_branches.insert(bound_branches.end(), corrT1METJetBranches.begin(), corrT1METJetBranches.end());
  bound_branches.insert(bound_branches.end(), genJetAKBranches.begin(), genJetAKBranches.end());
  bound_branches.insert(bound_branches.end(), genSubJetAKBranches.begin(), genSubJetAKBranches.end());

  bound_branches.insert(bound_branches.end(), jetBranchesAK8.begin(), jetBranchesAK8.end());
  bound_branches.insert(bound_branches.end(), rawmetBranches.begin(), rawmetBranches.end());
  bound_branches.insert(bound_branches.end(), metBranches.begin(), metBranches.end());
  bound_branches.insert(bound_branches.end(), metFilterBranches.begin(), metFilterBranches.end());
  bound_branches.insert(bound_branches.end(), vertexBranches.begin(), vertexBranches.end());
  return bound_branches;
}

namespace
{
  /**
   * @brief Auxiliary function used for sorting leptons by decreasing cone pT
   * @param Given pair of leptons
   * @return True, if first lepton has higher cone pT; false if second lepton has higher cone pT
   */
  template <typename T>
  bool
  isHigherConePt(const T * particle1, const T * particle2)
  {
    return particle1->cone_pt() > particle2->cone_pt();
  }

  /**
   * @brief Compute subset of particles contained in "lhs" as well as in "rhs" collection.
   *       (in the typical use-case "lhs" = fakeableLeptonsFull/fakeableHadTausFull and "rhs" = tightLeptons/tightHadTaus)
   */
  template <typename T,
            typename U,
            typename F>
  std::vector<T>
  getIntersection(const std::vector<T> & lhs_collection,
                  const std::vector<U> & rhs_collection,
                  bool (*sortFunction)(const F *, const F *))
  {
    std::vector<T> output_collection;
    for(const T & lhs_element: lhs_collection)
    {
      for(const U & rhs_element: rhs_collection)
      {
        if(lhs_element == rhs_element)
        {
          output_collection.push_back(lhs_element);
        }
      }
    }
    std::sort(output_collection.begin(), output_collection.end(), sortFunction);
    return output_collection;
  }

  /**
   * @brief Build collection of selected leptons by merging collections of selected electrons and selected muons
   */
  std::vector<const RecoLepton *>
  mergeLeptonCollectionsNoSort(const std::vector<const RecoElectron *> & electrons,
                               const std::vector<const RecoMuon *> & muons)
  {
    std::vector<const RecoLepton *> leptons;
    const std::size_t nLeptons = electrons.size() + muons.size();
    if(nLeptons > 0)
    {
      leptons.reserve(nLeptons);
      leptons.insert(leptons.end(), electrons.begin(), electrons.end());
      leptons.insert(leptons.end(), muons.begin(), muons.end());
    }
    return leptons;
  }

  template <typename T>
  std::vector<const RecoLepton *>
  mergeLeptonCollections(const std::vector<const RecoElectron *> & electrons,
                         const std::vector<const RecoMuon *> & muons,
                         bool (*sortFunction)(const T *, const T *))
  {
    std::vector<const RecoLepton *> leptons = mergeLeptonCollectionsNoSort(electrons, muons);
    std::sort(leptons.begin(), leptons.end(), sortFunction);
    return leptons;
  }
}

const RunLumiEvent &
EventReader::read_runLumiEvent() const
{
  return runLumiEventReader_->read();
}

const Event &
EventReader::read() const
{
  const RunLumiEvent & runLumiEvent = runLumiEventReader_->read();
  UInt_t run = runLumiEvent.run();
  UInt_t lumi = runLumiEvent.lumi();
  ULong64_t event = runLumiEvent.event();

  bool isNewEvent = run != lastRun_ || lumi != lastLumi_ || event != lastEvent_;
  lastRun_ = run;
  lastLumi_ = lumi;
  lastEvent_ = event;

  bool isEventInfoSystematic = contains(eventInfo_supported_systematics_, current_central_or_shift_);
  if ( isEventInfoSystematic || isNewEvent )
  {
    event_.eventInfo_ = &eventInfoReader_->read();
    jmeCorrector_->set_info(event_.eventInfo_);
  }
  if ( event_.isInvalid() && !isNewEvent )
  {
    return event_;
  }
  bool isTriggerInfoSystematic = contains(triggerInfo_supported_systematics_, current_central_or_shift_);
  if ( isTriggerInfoSystematic || isNewEvent )
  {
    event_.triggerInfo_ = &triggerInfoReader_->read();
  }

  bool isJetSystematicAK4 = contains(jetsAK4_supported_systematics_, current_central_or_shift_);
  bool isUpdatedJetsAK4 = false;
  bool jetAK4_needsUpdate = isJetSystematicAK4 || isNewEvent || jetAK4_isInvalid_ || (jetAK4_lastSystematic_ != "central" && !isJetSystematicAK4);
  if ( jetAK4_needsUpdate )
  {
    event_.jetsAK4_ = jetReaderAK4_->read();
    event_.jet_ptrsAK4_ = convert_to_ptrs(event_.jetsAK4_);

    if ( isMC_ )
    {
      if ( isNewEvent )
      {
        event_.genJets_ = genJetReader_->read();
        event_.genJetPtrs_ = convert_to_ptrs(event_.genJets_);
        event_.corrT1METJets_ = corrT1METJetReader_->read();
      }
      jmeCorrector_->reset();
      for(RecoJetAK4 & jet: event_.jetsAK4_)
      {
        jmeCorrector_->correct(jet, event_.genJetPtrs_);
      }
      for(const CorrT1METJet & jet: event_.corrT1METJets_)
      {
        // We're actually not correcting these jets, we just need to store deltas and propagate them to MET, hence the const qualifier.
        // This is also to avoid re-reading the collection every time the AK4 jet systematics changes.
        jmeCorrector_->correct(jet, event_.genJetPtrs_, event_.jet_ptrsAK4_);
      }
    }

    event_.selJetsUncleanedAK4_ = jetSelectorAK4_->operator()(event_.jet_ptrsAK4_, isHigherPt<RecoJetAK4>);
    event_.selJetsUncleanedAK4_btagLoose_ = jetSelectorAK4_btagLoose_->operator()(event_.jet_ptrsAK4_, isHigherPt<RecoJetAK4>);
    event_.selJetsUncleanedAK4_btagMedium_ = jetSelectorAK4_btagMedium_->operator()(event_.jet_ptrsAK4_, isHigherPt<RecoJetAK4>);
    isUpdatedJetsAK4 = true;
  }

  bool isMuonSystematic = contains(muon_supported_systematics_, current_central_or_shift_);
  bool isUpdatedMuons = false;
  bool muon_needsUpdate = isMuonSystematic || isNewEvent || (muon_lastSystematic_ != "central" && !isMuonSystematic);
  if ( muon_needsUpdate )
  {
    event_.muons_ = muonReader_->read();
    setJetBtagScore(event_.muons_, event_.jetsAK4_);
    setFilterBits(event_.muons_, event_.triggerInfo());
    event_.muon_ptrs_ = convert_to_ptrs(event_.muons_);
    event_.looseMuonsFull_ = looseMuonSelector_->operator()(event_.muon_ptrs_, isHigherConePt<RecoMuon>);
    event_.fakeableMuonsFull_ = fakeableMuonSelector_->operator()(event_.looseMuonsFull_, isHigherConePt<RecoMuon>);
    event_.tightMuonsFull_ = tightMuonSelector_->operator()(event_.fakeableMuonsFull_, isHigherConePt<RecoMuon>);
    event_.fakeableMuons_ = pickFirstNobjects(event_.fakeableMuonsFull_, numNominalLeptons_);
    event_.tightMuons_ = getIntersection(event_.fakeableMuons_, event_.tightMuonsFull_, isHigherConePt<RecoMuon>);
    isUpdatedMuons = true;
    muon_lastSystematic_ = ( isMuonSystematic ) ? current_central_or_shift_ : "central";
  }
  bool isElectronSystematic = contains(electron_supported_systematics_, current_central_or_shift_);
  bool isUpdatedElectrons = false;
  bool electron_needsUpdate = isElectronSystematic || isNewEvent || (electron_lastSystematic_ != "central" && !isElectronSystematic);
  if ( electron_needsUpdate )
  {
    event_.electrons_ = electronReader_->read();
    setJetBtagScore(event_.electrons_, event_.jetsAK4_);
    setFilterBits(event_.electrons_, event_.triggerInfo());
    event_.electron_ptrs_ = convert_to_ptrs(event_.electrons_);
    event_.looseElectronsUncleaned_ = looseElectronSelector_->operator()(event_.electron_ptrs_, isHigherConePt<RecoElectron>);
    event_.fakeableElectronsUncleaned_ = fakeableElectronSelector_->operator()(event_.looseElectronsUncleaned_, isHigherConePt<RecoElectron>);
    event_.tightElectronsUncleaned_ = tightElectronSelector_->operator()(event_.fakeableElectronsUncleaned_, isHigherConePt<RecoElectron>);
    isUpdatedElectrons = true;
  }
  if ( isUpdatedMuons || isUpdatedElectrons )
  {
    event_.looseElectronsFull_ = electronCleaner_(event_.looseElectronsUncleaned_, event_.looseMuonsFull_);
    event_.fakeableElectronsFull_ = electronCleaner_(event_.fakeableElectronsUncleaned_, event_.looseMuonsFull_);
    event_.tightElectronsFull_ = getIntersection(event_.fakeableElectronsFull_, event_.tightElectronsUncleaned_, isHigherConePt<RecoElectron>);
    event_.fakeableElectrons_ = pickFirstNobjects(event_.fakeableElectronsFull_, numNominalLeptons_);
    event_.tightElectrons_ = getIntersection(event_.fakeableElectrons_, event_.tightElectronsFull_, isHigherConePt<RecoElectron>);
    isUpdatedElectrons = true;
    electron_lastSystematic_ = ( isElectronSystematic ) ? current_central_or_shift_ : "central";
  }
  bool isUpdatedLeptons = false;
  if ( isUpdatedMuons || isUpdatedElectrons )
  {
    event_.looseLeptonsUncleaned_ = mergeLeptonCollections(event_.looseElectronsUncleaned_, event_.looseMuonsFull_, isHigherConePt<RecoLepton>);
    event_.looseLeptonsFull_ = mergeLeptonCollections(event_.looseElectronsFull_, event_.looseMuonsFull_, isHigherConePt<RecoLepton>);
    event_.fakeableLeptonsFull_ = mergeLeptonCollections(event_.fakeableElectronsFull_, event_.fakeableMuonsFull_, isHigherConePt<RecoLepton>);
    event_.tightLeptonsFull_ = mergeLeptonCollections(event_.tightElectronsFull_, event_.tightMuonsFull_, isHigherConePt<RecoLepton>);
    event_.fakeableLeptons_ = pickFirstNobjects(event_.fakeableLeptonsFull_, numNominalLeptons_);
    event_.tightLeptons_ = getIntersection(event_.fakeableLeptons_, event_.tightLeptonsFull_, isHigherConePt<RecoLepton>);
    isUpdatedLeptons = true;
  }
  if ( applyNumNominalLeptonsCut_ && event_.fakeableLeptons_.size() < numNominalLeptons_ )
  {
    clearEvent(Level::kLepton);
    return event_;
  }
  bool isHadTauSystematic = contains(hadTau_supported_systematics_, current_central_or_shift_);
  bool isUpdatedHadTaus = false;
  bool hadTau_needsUpdate = isHadTauSystematic || isNewEvent || hadTau_isInvalid_ || (hadTau_lastSystematic_ != "central" && !isHadTauSystematic);
  if ( hadTau_needsUpdate )
  {
    event_.hadTaus_ = hadTauReader_->read();
    setFilterBits(event_.hadTaus_, event_.triggerInfo());
    event_.hadTau_ptrs_ = convert_to_ptrs(event_.hadTaus_);
    event_.fakeableHadTausUncleaned_ = fakeableHadTauSelector_->operator()(event_.hadTau_ptrs_, isHigherPt<RecoHadTau>);
    event_.tightHadTausUncleaned_ = tightHadTauSelector_->operator()(event_.fakeableHadTausUncleaned_, isHigherPt<RecoHadTau>);
    isUpdatedHadTaus = true;
  }
  if ( isUpdatedMuons || isUpdatedElectrons || isUpdatedHadTaus )
  {
    event_.fakeableHadTausFull_ = hadTauCleaner_(event_.fakeableHadTausUncleaned_, event_.looseMuonsFull_, event_.looseElectronsFull_);
    event_.tightHadTausFull_ = getIntersection(event_.fakeableHadTausFull_, event_.tightHadTausUncleaned_, isHigherPt<RecoHadTau>);
    event_.fakeableHadTaus_ = pickFirstNobjects(event_.fakeableHadTausFull_, numNominalHadTaus_);
    event_.tightHadTaus_ = getIntersection(event_.fakeableHadTaus_, event_.tightHadTausFull_, isHigherPt<RecoHadTau>);
    isUpdatedHadTaus = true;
    hadTau_lastSystematic_ = ( isHadTauSystematic ) ? current_central_or_shift_ : "central";
  }
  hadTau_isInvalid_ = false;
  if ( applyNumNominalHadTausCut_ && event_.fakeableHadTaus_.size() < numNominalHadTaus_ )
  {
    clearEvent(Level::kHadTau);
    return event_;
  }

  if ( isUpdatedMuons || isUpdatedElectrons || isUpdatedHadTaus || isUpdatedJetsAK4 )
  {
    if ( jetCleaningByIndex_ )
    {
      event_.selJetsAK4_ = jetCleanerAK4_dR04_(event_.selJetsUncleanedAK4_, event_.fakeableLeptons_, event_.fakeableHadTaus_);
      event_.selJetsAK4_btagLoose_ = jetCleanerAK4_dR04_(event_.selJetsUncleanedAK4_btagLoose_, event_.fakeableLeptons_, event_.fakeableHadTaus_);
      event_.selJetsAK4_btagMedium_ = jetCleanerAK4_dR04_(event_.selJetsUncleanedAK4_btagMedium_, event_.fakeableLeptons_, event_.fakeableHadTaus_);
    }
    else
    {
      event_.selJetsAK4_ = jetCleanerAK4ByIndex_dR04_(event_.selJetsUncleanedAK4_, event_.fakeableLeptons_, event_.fakeableHadTaus_);
      event_.selJetsAK4_btagLoose_ = jetCleanerAK4ByIndex_dR04_(event_.selJetsUncleanedAK4_btagLoose_, event_.fakeableLeptons_, event_.fakeableHadTaus_);
      event_.selJetsAK4_btagMedium_ = jetCleanerAK4ByIndex_dR04_(event_.selJetsUncleanedAK4_btagMedium_, event_.fakeableLeptons_, event_.fakeableHadTaus_);
    }
    isUpdatedJetsAK4 = true;
    jetAK4_lastSystematic_ = ( isJetSystematicAK4 ) ? current_central_or_shift_ : "central";
  }
  jetAK4_isInvalid_ = false;

  if ( isMC_ )
  {
    if ( isNewEvent )
    {
      event_.genParticles_ = genParticleReader_->read();
      event_.genHadTaus_ = genHadTauReader_->read();
    }

    // CV: performing the gen-matching on the muon_ptrs, electron_ptrs, and hadTau_ptrs collections
    //     adds gen-matching information to all loose, fakeable, and tight collections of muons, electrons and hadronic taus
    if ( isUpdatedMuons )
    {
      muonGenMatcher_->addGenMatchByIdx(event_.muon_ptrs_, event_.genParticles_);
      muonGenMatcher_->addGenHadTauMatch(event_.muon_ptrs_, event_.genHadTaus_);
      muonGenMatcher_->addGenJetMatch(event_.muon_ptrs_, event_.genJets_);
    }

    if ( isUpdatedElectrons )
    {
      electronGenMatcher_->addGenMatchByIdx(event_.electron_ptrs_, event_.genParticles_);
      electronGenMatcher_->addGenHadTauMatch(event_.electron_ptrs_, event_.genHadTaus_);
      electronGenMatcher_->addGenJetMatch(event_.electron_ptrs_, event_.genJets_);
    }

    if ( isUpdatedHadTaus )
    {
      hadTauGenMatcher_->addGenMatchByIdx(event_.hadTau_ptrs_, event_.genParticles_);
      hadTauGenMatcher_->addGenHadTauMatch(event_.hadTau_ptrs_, event_.genHadTaus_);
      hadTauGenMatcher_->addGenJetMatch(event_.hadTau_ptrs_, event_.genJets_);
    }
    
    // CV: performing the gen-matching on the jet_ptrsAK4 collection
    //     adds gen-matching information to three collections of AK4 jets at once (selJetsAK4, selJetsAK4_btagLoose, selJetsAK4_btagMedium)
    if ( genMatchRecoJets_ && isUpdatedJetsAK4 )
    {
      jetGenMatcherAK4_->addGenLeptonMatch(event_.jet_ptrsAK4_, event_.genParticles_);
      jetGenMatcherAK4_->addGenHadTauMatch(event_.jet_ptrsAK4_, event_.genHadTaus_); 
      jetGenMatcherAK4_->addGenJetMatchByIdx(event_.jet_ptrsAK4_, event_.genJets_);
    }
  }
  if ( isDEBUG_ )
  {
    // print the collections *after* we've gen-matched everything
    printCollection("looseLeptonsUncleaned", event_.looseLeptonsUncleaned_);
    printCollection("fakeableLeptonsFull", event_.fakeableLeptonsFull_);
    printCollection("tightLeptonsFull", event_.tightLeptonsFull_);

    printCollection("fakeableHadTausUncleaned", event_.fakeableHadTausUncleaned_);
    printCollection("fakeableHadTausFull", event_.fakeableHadTausFull_);
    printCollection("tightHadTausFull", event_.tightHadTausFull_);

    printCollection("selJetsUncleanedAK4", event_.selJetsUncleanedAK4_);
    printCollection("selJetsAK4", event_.selJetsAK4_);
    printCollection("selJetsAK4_btagLoose", event_.selJetsAK4_btagLoose_);
    printCollection("selJetsAK4_btagMedium", event_.selJetsAK4_btagMedium_);
  }
  bool isJetSystematicAK8_Hbb = contains(jetsAK8_Hbb_supported_systematics_, current_central_or_shift_);
  bool isUpdatedJetsAK8_Hbb = false;
  bool jetAK8_Hbb_needsUpdate = isJetSystematicAK8_Hbb || isNewEvent || jetAK8_Hbb_isInvalid_ || (jetAK8_Hbb_lastSystematic_ != "central" && !isJetSystematicAK8_Hbb);
  if ( jetAK8_Hbb_needsUpdate )
  {
    event_.jetsAK8_ = jetReaderAK8_->read();
    if ( isMC_ && isNewEvent )
    {
      event_.genJetsAK8_ = genJetAK8Reader_->read();
      event_.genJetPtrsAK8_ = convert_to_ptrs(event_.genJetsAK8_);
      event_.genSubJetsAK8_ = genSubJetAK8Reader_->read();
      event_.genSubJetPtrsAK8_ = convert_to_ptrs(event_.genSubJetsAK8_);
    }
    for(RecoJetAK8 & jet: event_.jetsAK8_)
    {
      jmeCorrector_->correct(jet, event_.genJetPtrsAK8_, event_.genSubJetPtrsAK8_);
    }
    event_.jet_ptrsAK8_ = convert_to_ptrs(event_.jetsAK8_);
    event_.selJetsUncleanedAK8_Hbb_ = jetSelectorAK8_Hbb_->operator()(event_.jet_ptrsAK8_, isHigherPt<RecoJetAK8>);
    isUpdatedJetsAK8_Hbb = true;
  }
  if ( isUpdatedJetsAK8_Hbb || isUpdatedLeptons )
  {
    // CV: clean AK8_Hbb jets wrt leptons only (not wrt hadronic taus)
    event_.selJetsAK8_Hbb_ = jetCleanerAK8_dR08_(event_.selJetsUncleanedAK8_Hbb_, event_.fakeableLeptons_);
    jetAK8_Hbb_lastSystematic_ = isJetSystematicAK8_Hbb ? current_central_or_shift_ : "central";
  }
  if ( isDEBUG_ )
  {
    printCollection("selJetsUncleanedAK8_Hbb", event_.selJetsUncleanedAK8_Hbb_);
    printCollection("selJetsAK8_Hbb", event_.selJetsAK8_Hbb_);
  }
  jetAK8_Hbb_isInvalid_ = false;

  bool isJetSystematicAK8_Wjj = contains(jetsAK8_Wjj_supported_systematics_, current_central_or_shift_);
  bool jetAK8_Wjj_needsUpdate = isJetSystematicAK8_Wjj || isNewEvent || jetAK8_Wjj_isInvalid_ || (jetAK8_Wjj_lastSystematic_ != "central" && !isJetSystematicAK8_Wjj);
  if ( jetAK8_Wjj_needsUpdate || isUpdatedLeptons )
  {
    // CV: AK8_Wjj jets must NOT be cleaned wrt leptons,
    //     as the lepton produced in H->WW*->lnu qq decays often ends up near the two quarks in the detector (in dR)
    if ( event_.fakeableLeptons_.size() > 0 )
    {
      jetSelectorAK8_Wjj_->getSelector().set_leptons(event_.fakeableLeptons_);
      event_.selJetsAK8_Wjj_ = jetSelectorAK8_Wjj_->operator()(event_.jet_ptrsAK8_, isHigherPt<RecoJetAK8>);
    }
    else
    {
      event_.selJetsAK8_Wjj_.clear();
    }
    jetAK8_Wjj_lastSystematic_ = isJetSystematicAK8_Wjj ? current_central_or_shift_ : "central";
  }
  if ( isDEBUG_ )
  {
    printCollection("selJetsAK8_Wjj", event_.selJetsAK8_Wjj_);
  }
  jetAK8_Wjj_isInvalid_ = false;
  bool isVertexSystematic = contains(vertex_supported_systematics_, current_central_or_shift_);
  bool isUpdatedVertex = false;
  bool vertex_needsUpdate = isVertexSystematic || isNewEvent || vertex_isInvalid_ || (vertex_lastSystematic_ != "central" && !isVertexSystematic);
  if ( vertex_needsUpdate )
  {
    event_.vertex_ = vertexReader_->read();
    isUpdatedVertex = true;
    vertex_lastSystematic_ = ( isVertexSystematic ) ? current_central_or_shift_ : "central";
  }
  vertex_isInvalid_ = false;

  bool isMEtSystematic = contains(met_supported_systematics_, current_central_or_shift_);
  bool met_needsUpdate = isMEtSystematic || isUpdatedVertex || isNewEvent || met_isInvalid_ || (met_lastSystematic_ != "central" && !isMEtSystematic);
  if ( met_needsUpdate )
  {
    event_.met_ = std::move(metReader_->read());
    event_.rawmet_ = std::move(rawmetReader_->read());
    jmeCorrector_->correct(event_.met_, event_.rawmet_, &event_.vertex_);
    met_lastSystematic_ = ( isMEtSystematic ) ? current_central_or_shift_ : "central";
    if(isDEBUG_)
    {
      std::cout
        << "Raw MEt: " << event_.rawmet_ << "\n"
           "RecoMEt: " << event_.met_ << '\n'
      ;
    }
  }
  met_isInvalid_ = false;

  bool isMEtFilterSystematic = contains(metFilter_supported_systematics_, current_central_or_shift_);
  bool metFilter_needsUpdate = isMEtFilterSystematic || isNewEvent || metFilter_isInvalid_ || (metFilter_lastSystematic_ != "central" && !isMEtFilterSystematic);
  if ( metFilter_needsUpdate )
  {
    event_.metFilters_ = metFilterReader_->read();
    metFilter_lastSystematic_ = ( isMEtFilterSystematic ) ? current_central_or_shift_ : "central";
  }
  metFilter_isInvalid_ = false;

  if ( isNewEvent )
  {
    event_.isValid_ = true;
  }

  return event_;
}

void
EventReader::set_tauEScset(correction::Correction::Ref cset)
{
  hadTauReader_->set_tauEScset(cset);
}

std::vector<std::string>
EventReader::get_supported_systematics(const edm::ParameterSet & cfg)
{
  std::vector<std::string> supported_systematics;
  merge_systematic_shifts(supported_systematics, EventInfoReader::get_supported_systematics(cfg));
  merge_systematic_shifts(supported_systematics, RecoElectronReader::get_supported_systematics(cfg));
  merge_systematic_shifts(supported_systematics, RecoHadTauReader::get_supported_systematics(cfg));
  merge_systematic_shifts(supported_systematics, RecoJetReaderAK4::get_supported_systematics(cfg));
  merge_systematic_shifts(supported_systematics, RecoJetReaderAK8::get_supported_systematics(cfg));
  merge_systematic_shifts(supported_systematics, RecoMEtReader::get_supported_systematics(cfg));
  merge_systematic_shifts(supported_systematics, RecoMuonReader::get_supported_systematics(cfg));
  merge_systematic_shifts(supported_systematics, RecoVertexReader::get_supported_systematics(cfg));
  merge_systematic_shifts(supported_systematics, TriggerInfoReader::get_supported_systematics(cfg));
  return supported_systematics;
}

void
EventReader::clearEvent(Level level) const
{
  if ( level == Level::kLepton )
  {
    event_.fakeableHadTaus_.clear();
    event_.tightHadTaus_.clear();
    hadTau_isInvalid_ = true;
  }
  event_.selJetsAK4_.clear();
  event_.selJetsAK4_btagLoose_.clear();
  event_.selJetsAK4_btagMedium_.clear();
  jetAK4_isInvalid_ = true;
  event_.selJetsAK8_Hbb_.clear();
  jetAK8_Hbb_isInvalid_ = true;
  event_.selJetsAK8_Wjj_.clear();
  jetAK8_Wjj_isInvalid_ = true;
  met_isInvalid_ = true;
  metFilter_isInvalid_ = true;
  vertex_isInvalid_ = true;
  event_.isValid_ = false;
  // TODO do we have to reset the gen-matches here also?
}
