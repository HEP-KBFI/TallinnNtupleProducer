#include "TallinnNtupleProducer/Readers/interface/EventReader.h"

#include "TallinnNtupleProducer/CommonTools/interface/cmsException.h"             // cmsException
#include "TallinnNtupleProducer/CommonTools/interface/contains.h"                 // contains()
#include "TallinnNtupleProducer/CommonTools/interface/Era.h"                      // Era, get_era()
#include "TallinnNtupleProducer/CommonTools/interface/get_ignore_ak8_sys.h"       // get_ignore_ak8_sys()
#include "TallinnNtupleProducer/CommonTools/interface/hadTauDefinitions.h"        // get_tau_id_wp_int()
#include "TallinnNtupleProducer/CommonTools/interface/isHigherPt.h"               // isHigherPt()
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

  edm::ParameterSet
  make_cfg_jetsAK8(const edm::ParameterSet& cfg, const std::string & attr_branchName_jet, const std::string & attr_branchName_subjet)
  {
    edm::ParameterSet cfg_modified(cfg);
    std::string branchName_jet = cfg.getParameter<std::string>(attr_branchName_jet);
    cfg_modified.addParameter<std::string>("branchName_jet", branchName_jet);
    std::string branchName_subjet = cfg.getParameter<std::string>(attr_branchName_subjet);
    cfg_modified.addParameter<std::string>("branchName_subjet", branchName_subjet);
    return cfg_modified;
  }
}

EventReader::EventReader(const edm::ParameterSet& cfg)
  : ReaderBase(cfg)
  , cfg_(cfg)
  , numNominalLeptons_(0)
  , numNominalHadTaus_(0)
  , era_(Era::kUndefined)
  , isMC_(false)
  , readGenMatching_(false)
  , eventInfoReader_(nullptr)
  , triggerInfoReader_(nullptr)
  , muonReader_(nullptr)
  , looseMuonSelector_(nullptr)
  , fakeableMuonSelector_(nullptr)
  , tightMuonSelector_(nullptr)
  , electronReader_(nullptr)
  , electronCleaner_(nullptr)
  , looseElectronSelector_(nullptr)
  , fakeableElectronSelector_(nullptr)
  , tightElectronSelector_(nullptr)
  , hadTauReader_(nullptr)
  , hadTauCleaner_(nullptr)
  , looseHadTauSelector_(nullptr)
  , fakeableHadTauSelector_(nullptr)
  , tightHadTauSelector_(nullptr)
  , jetReaderAK4_(nullptr)
  , jetCleanerAK4_dR04_(nullptr)
  , jetCleanerAK4_dR12_(nullptr)
  , jetSelectorAK4_(nullptr)
  , jetSelectorAK4_btagLoose_(nullptr)
  , jetSelectorAK4_btagMedium_(nullptr)
  , genLeptonReader_(nullptr)
  , genHadTauReader_(nullptr)
  , genPhotonReader_(nullptr)
  , genJetReader_(nullptr)
  , muonGenMatcher_(nullptr)
  , electronGenMatcher_(nullptr)
  , hadTauGenMatcher_(nullptr)
  , jetGenMatcherAK4_(nullptr)
  , jetReaderAK8_Hbb_(nullptr)
  , jetReaderAK8_Wjj_(nullptr)
  , jetCleanerAK8_dR08_(nullptr)
  , jetSelectorAK8_Hbb_(nullptr)
  , jetSelectorAK8_Wjj_(nullptr)
  , metReader_(nullptr)
  , metFilterReader_(nullptr)
  , vertexReader_(nullptr)
  , isDEBUG_(cfg.getParameter<bool>("isDEBUG"))
{
  numNominalLeptons_ = cfg.getParameter<unsigned>("numNominalLeptons");
  numNominalHadTaus_ = cfg.getParameter<unsigned>("numNominalHadTaus");

  era_ = get_era(cfg.getParameter<std::string>("era"));
  isMC_ = cfg.getParameter<bool>("isMC");
  readGenMatching_ = isMC_ && !cfg.getParameter<bool>("redoGenMatching");

  eventInfoReader_ = new EventInfoReader(cfg);
  const std::string apply_topPtReweighting_str = cfg.getParameter<std::string>("apply_topPtReweighting");
  const bool apply_topPtReweighting = ! apply_topPtReweighting_str.empty();
  if ( apply_topPtReweighting )
  {
    eventInfoReader_->setTopPtRwgtBranchName(apply_topPtReweighting_str);
  }

  edm::ParameterSet cfg_triggers = cfg.getParameter<edm::ParameterSet>("triggers");
  triggerInfoReader_ = new TriggerInfoReader(cfg_triggers);

  muonReader_ = new RecoMuonReader(make_cfg(cfg, "branchName_muons"));
  const double lep_mva_cut_mu = cfg.getParameter<double>("lep_mva_cut_mu");
  muonReader_->set_mvaTTH_wp(lep_mva_cut_mu);
  looseMuonSelector_ = new RecoMuonCollectionSelectorLoose(era_, -1, isDEBUG_);
  fakeableMuonSelector_ = new RecoMuonCollectionSelectorFakeable(era_, -1, isDEBUG_);
  tightMuonSelector_ = new RecoMuonCollectionSelectorTight(era_, -1, isDEBUG_);

  electronReader_ = new RecoElectronReader(make_cfg(cfg, "branchName_electrons"));
  const double lep_mva_cut_e = cfg.getParameter<double>("lep_mva_cut_e");
  electronReader_->set_mvaTTH_wp(lep_mva_cut_e);
  electronCleaner_ = new RecoElectronCollectionCleaner(0.3, isDEBUG_);
  looseElectronSelector_ = new RecoElectronCollectionSelectorLoose(era_, -1, isDEBUG_);
  fakeableElectronSelector_ = new RecoElectronCollectionSelectorFakeable(era_, -1, isDEBUG_);
  tightElectronSelector_ = new RecoElectronCollectionSelectorTight(era_, -1, isDEBUG_);

  hadTauReader_ = new RecoHadTauReader(make_cfg(cfg, "branchName_hadTaus"));
  hadTauCleaner_ = new RecoHadTauCollectionCleaner(0.3, isDEBUG_);
  looseHadTauSelector_ = new RecoHadTauCollectionSelectorLoose(era_, -1, isDEBUG_);
  fakeableHadTauSelector_ = new RecoHadTauCollectionSelectorFakeable(era_, -1, isDEBUG_);
  tightHadTauSelector_ = new RecoHadTauCollectionSelectorTight(era_, -1, isDEBUG_);
  std::string hadTauWP_againstJets_fakeable = cfg.getParameter<std::string>("hadTauWP_againstJets_fakeable");
  std::string hadTauWP_againstJets_tight = cfg.getParameter<std::string>("hadTauWP_againstJets_tight");
  if ( get_tau_id_wp_int(hadTauWP_againstJets_tight) <= get_tau_id_wp_int(hadTauWP_againstJets_fakeable) )
    throw cmsException("EventReader", __LINE__) << "Selection of 'tight' taus must be more stringent than selection of 'fakeable' taus !!";
  fakeableHadTauSelector_->set(hadTauWP_againstJets_fakeable);
  tightHadTauSelector_->set(hadTauWP_againstJets_tight);
  int hadTauWP_againstElectrons = get_tau_id_wp_int(cfg.getParameter<std::string>("hadTauWP_againstElectrons"));
  fakeableHadTauSelector_->set_min_antiElectron(hadTauWP_againstElectrons);
  tightHadTauSelector_->set_min_antiElectron(hadTauWP_againstElectrons);
  int hadTauWP_againstMuons = get_tau_id_wp_int(cfg.getParameter<std::string>("hadTauWP_againstMuons"));
  fakeableHadTauSelector_->set_min_antiMuon(hadTauWP_againstMuons);
  tightHadTauSelector_->set_min_antiMuon(hadTauWP_againstMuons);

  jetReaderAK4_ = new RecoJetReaderAK4(make_cfg(cfg, "branchName_jets_ak4"));
  bool jetCleaningByIndex = cfg.getParameter<bool>("jetCleaningByIndex");
  if ( jetCleaningByIndex )
  {
    jetCleanerAK4_dR04_ = new RecoJetCollectionCleanerByIndexAK4(isDEBUG_);
  }
  else
  {
    jetCleanerAK4_dR04_ = new RecoJetCollectionCleanerAK4(0.4, isDEBUG_);
  }
  jetCleanerAK4_dR12_ = new RecoJetCollectionCleanerAK4(1.2, isDEBUG_);
  jetSelectorAK4_ = new RecoJetCollectionSelectorAK4(era_, -1, isDEBUG_);
  jetSelectorAK4_btagLoose_ = new RecoJetCollectionSelectorAK4_btagLoose(era_, -1, isDEBUG_);
  jetSelectorAK4_btagMedium_ = new RecoJetCollectionSelectorAK4_btagMedium(era_, -1, isDEBUG_);

  if ( readGenMatching_ )
  {
    genLeptonReader_ = new GenLeptonReader(make_cfg(cfg, "branchName_genLeptons"));
    genHadTauReader_ = new GenHadTauReader(make_cfg(cfg, "branchName_genHadTaus"));
    genPhotonReader_ = new GenPhotonReader(make_cfg(cfg, "branchName_genPhotons"));
    genJetReader_ = new GenJetReader(make_cfg(cfg, "branchName_genJets"));
    muonGenMatcher_ = new RecoMuonCollectionGenMatcher();
    electronGenMatcher_ = new RecoElectronCollectionGenMatcher();
    hadTauGenMatcher_ = new RecoHadTauCollectionGenMatcher();
    jetGenMatcherAK4_ = new RecoJetCollectionGenMatcherAK4();
  }

  jetReaderAK8_Hbb_ = new RecoJetReaderAK8(make_cfg_jetsAK8(cfg, "branchName_jets_ak8_Hbb", "branchName_subjets_ak8_Hbb"));
  jetReaderAK8_Wjj_ = new RecoJetReaderAK8(make_cfg_jetsAK8(cfg, "branchName_jets_ak8_Wjj", "branchName_subjets_ak8_Wjj"));
  const std::vector<std::string> disable_ak8_corr = cfg.getParameter<std::vector<std::string>>("disable_ak8_corr");
  const int ignore_ak8_sys = get_ignore_ak8_sys(disable_ak8_corr);
  jetReaderAK8_Hbb_->ignoreSys(ignore_ak8_sys);
  jetReaderAK8_Wjj_->ignoreSys(ignore_ak8_sys);
  jetCleanerAK8_dR08_ = new RecoJetCollectionCleanerAK8(0.8, isDEBUG_);
  jetSelectorAK8_Hbb_ = new RecoJetCollectionSelectorAK8_Hbb(era_, -1, isDEBUG_);
  jetSelectorAK8_Wjj_ = new RecoJetCollectionSelectorAK8_Wjj(era_, -1, isDEBUG_);

  metReader_ = new RecoMEtReader(make_cfg(cfg, "branchName_met"));
  metFilterReader_ = new MEtFilterReader(cfg);

  vertexReader_ = new RecoVertexReader(make_cfg(cfg, "branchName_vertex"));
}

EventReader::~EventReader()
{
  delete muonReader_;
  delete looseMuonSelector_;
  delete fakeableMuonSelector_;
  delete tightMuonSelector_;
  delete electronReader_;
  delete electronCleaner_;
  delete looseElectronSelector_;
  delete fakeableElectronSelector_;
  delete tightElectronSelector_;
  delete hadTauReader_;
  delete hadTauCleaner_;
  delete looseHadTauSelector_;
  delete fakeableHadTauSelector_;
  delete tightHadTauSelector_;
  delete jetReaderAK4_;
  delete jetCleanerAK4_dR04_;
  delete jetCleanerAK4_dR12_;
  delete jetSelectorAK4_;
  delete jetSelectorAK4_btagLoose_;
  delete jetSelectorAK4_btagMedium_;
  delete genLeptonReader_;
  delete genHadTauReader_;
  delete genPhotonReader_;
  delete genJetReader_;
  delete muonGenMatcher_;
  delete electronGenMatcher_;
  delete hadTauGenMatcher_;
  delete jetGenMatcherAK4_;
  delete jetReaderAK8_Hbb_;
  delete jetReaderAK8_Wjj_;
  delete jetCleanerAK8_dR08_;
  delete jetSelectorAK8_Hbb_;
  delete jetSelectorAK8_Wjj_;
  delete metReader_;
  delete metFilterReader_;
  delete vertexReader_;
}

void
EventReader::set_central_or_shift(const std::string& central_or_shift)
{
  eventInfoReader_->set_central_or_shift(central_or_shift);
  if ( central_or_shift == "central" || contains(muonReader_->get_supported_systematics(cfg_), central_or_shift) )
  {
    // CV: muon momentum scale uncertainty not implemented yet
  }
  if ( central_or_shift == "central" || contains(electronReader_->get_supported_systematics(cfg_), central_or_shift) )
  {
    // CV: electron energy scale uncertainty not implemented yet 
  }
  if ( contains(hadTauReader_->get_supported_systematics(cfg_), central_or_shift) )
  {
    const int hadTauPt_option = getHadTauPt_option(central_or_shift);
    hadTauReader_->setHadTauPt_central_or_shift(hadTauPt_option);
  }
  if ( central_or_shift == "central" || contains(jetReaderAK4_->get_supported_systematics(cfg_), central_or_shift) )
  {
    const int jetPt_option = getJet_option(central_or_shift, isMC_);
    jetReaderAK4_->setPtMass_central_or_shift(jetPt_option);
    const int btagWeight_option = ( isMC_ ) ? getBTagWeight_option(central_or_shift) : kBtag_central;
    jetReaderAK4_->setBtagWeight_central_or_shift(btagWeight_option);
  }
  if ( central_or_shift == "central" || contains(jetReaderAK8_Hbb_->get_supported_systematics(cfg_), central_or_shift) )
  {
    const int fatJetPt_option = getFatJet_option(central_or_shift, isMC_);
    jetReaderAK8_Hbb_->set_central_or_shift(fatJetPt_option);
  }
  if ( central_or_shift == "central" || contains(jetReaderAK8_Wjj_->get_supported_systematics(cfg_), central_or_shift) )
  {
    const int fatJetPt_option = getFatJet_option(central_or_shift, isMC_);
    jetReaderAK8_Wjj_->set_central_or_shift(fatJetPt_option);
  }
  if ( central_or_shift == "central" || contains(metReader_->get_supported_systematics(cfg_), central_or_shift) )
  {
    const int met_option = getMET_option(central_or_shift, isMC_);
    metReader_->setMEt_central_or_shift(met_option);
  }
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
  const std::vector<std::string> genLeptonBranches = genLeptonReader_->setBranchAddresses(inputTree);
  const std::vector<std::string> genHadTauBranches = genHadTauReader_->setBranchAddresses(inputTree);
  const std::vector<std::string> genPhotonBranches = genPhotonReader_->setBranchAddresses(inputTree);
  const std::vector<std::string> genJetBranches = genJetReader_->setBranchAddresses(inputTree);
  const std::vector<std::string> jetBranchesAK8_Hbb = jetReaderAK8_Hbb_->setBranchAddresses(inputTree);
  const std::vector<std::string> jetBranchesAK8_Wjj = jetReaderAK8_Wjj_->setBranchAddresses(inputTree);
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
  bound_branches.insert(bound_branches.end(), genLeptonBranches.begin(), genLeptonBranches.end());
  bound_branches.insert(bound_branches.end(), genHadTauBranches.begin(), genHadTauBranches.end());
  bound_branches.insert(bound_branches.end(), genPhotonBranches.begin(), genPhotonBranches.end());
  bound_branches.insert(bound_branches.end(), genJetBranches.begin(), genJetBranches.end());
  bound_branches.insert(bound_branches.end(), jetBranchesAK8_Hbb.begin(), jetBranchesAK8_Hbb.end());
  bound_branches.insert(bound_branches.end(), jetBranchesAK8_Wjj.begin(), jetBranchesAK8_Wjj.end());
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
   * @brief Return first N objects from collection given as function argument. In case the input
   *        collection contains fewer than N objects, the whole input collection is returned
   */
  template <typename T>
  std::vector<T>
  pickFirstNobjects(const std::vector<T> & objects_input,
                    std::size_t N)
  {
    const std::size_t N_input = std::min(objects_input.size(), N);
    std::vector<T> objects_output;
    std::copy_n(objects_input.begin(), N_input, std::back_inserter(objects_output));
    return objects_output;
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

Event
EventReader::read() const
{
  const EventInfo& eventInfo = eventInfoReader_->read();

  const TriggerInfo& triggerInfo = triggerInfoReader_->read();

  Event event(eventInfo, triggerInfo);

  event.muons_ = muonReader_->read();
  RecoMuonPtrCollection muon_ptrs = convert_to_ptrs(event.muons_);
  RecoMuonPtrCollection cleanedMuons = muon_ptrs; // CV: no cleaning needed for muons, as they have the highest priority in the overlap removal
  event.looseMuons_ = looseMuonSelector_->operator()(cleanedMuons, isHigherConePt<RecoMuon>);
  RecoMuonPtrCollection fakeableMuonsFull = fakeableMuonSelector_->operator()(event.looseMuons_, isHigherConePt<RecoMuon>);
  RecoMuonPtrCollection tightMuonsFull = tightMuonSelector_->operator()(fakeableMuonsFull, isHigherConePt<RecoMuon>);
  event.fakeableMuons_ = pickFirstNobjects(fakeableMuonsFull, numNominalLeptons_);
  event.tightMuons_ = getIntersection(event.fakeableMuons_, tightMuonsFull, isHigherConePt<RecoMuon>);

  event.electrons_ = electronReader_->read();
  RecoElectronPtrCollection electron_ptrs = convert_to_ptrs(event.electrons_);
  RecoElectronPtrCollection cleanedElectrons = electronCleaner_->operator()(electron_ptrs, event.looseMuons_);
  event.looseElectrons_ = looseElectronSelector_->operator()(cleanedElectrons, isHigherConePt<RecoElectron>);
  RecoElectronPtrCollection fakeableElectronsFull = fakeableElectronSelector_->operator()(event.looseElectrons_, isHigherConePt<RecoElectron>);
  RecoElectronPtrCollection tightElectronsFull = tightElectronSelector_->operator()(fakeableElectronsFull, isHigherConePt<RecoElectron>);
  event.fakeableElectrons_ = pickFirstNobjects(fakeableElectronsFull, numNominalLeptons_);
  event.tightElectrons_ = getIntersection(event.fakeableElectrons_, tightElectronsFull, isHigherConePt<RecoElectron>);

  event.looseLeptons_ = mergeLeptonCollections(event.looseElectrons_, event.looseMuons_, isHigherConePt<RecoLepton>);
  RecoLeptonPtrCollection fakeableLeptonsFull = mergeLeptonCollections(fakeableElectronsFull, fakeableMuonsFull, isHigherConePt<RecoLepton>);
  RecoLeptonPtrCollection tightLeptonsFull = mergeLeptonCollections(tightElectronsFull, tightMuonsFull, isHigherConePt<RecoLepton>);
  event.fakeableLeptons_ = pickFirstNobjects(fakeableLeptonsFull, numNominalLeptons_);
  event.tightLeptons_ = getIntersection(event.fakeableLeptons_, tightLeptonsFull, isHigherConePt<RecoLepton>);

  event.hadTaus_ = hadTauReader_->read();
  RecoHadTauPtrCollection hadTau_ptrs = convert_to_ptrs(event.hadTaus_);
  RecoHadTauPtrCollection cleanedHadTaus = hadTauCleaner_->operator()(hadTau_ptrs, event.looseMuons_, event.looseElectrons_);
  RecoHadTauPtrCollection fakeableHadTausFull = fakeableHadTauSelector_->operator()(cleanedHadTaus, isHigherPt<RecoHadTau>);
  RecoHadTauPtrCollection tightHadTausFull = tightHadTauSelector_->operator()(fakeableHadTausFull, isHigherPt<RecoHadTau>);
  event.fakeableHadTaus_ = pickFirstNobjects(fakeableHadTausFull, numNominalHadTaus_);
  event.tightHadTaus_ = getIntersection(event.fakeableHadTaus_, tightHadTausFull, isHigherPt<RecoHadTau>);

  event.jetsAK4_ = jetReaderAK4_->read();
  RecoJetPtrCollectionAK4 jet_ptrsAK4 = convert_to_ptrs(event.jetsAK4_);
  RecoJetPtrCollectionAK4 cleanedJetsAK4 = jetCleanerAK4_dR04_->operator()(jet_ptrsAK4, event.fakeableLeptons_, event.fakeableHadTaus_);
  event.selJetsAK4_ = jetSelectorAK4_->operator()(cleanedJetsAK4, isHigherPt<RecoJetAK4>);
  event.selJetsAK4_btagLoose_ = jetSelectorAK4_btagLoose_->operator()(cleanedJetsAK4, isHigherPt<RecoJetAK4>);
  event.selJetsAK4_btagMedium_ = jetSelectorAK4_btagMedium_->operator()(cleanedJetsAK4, isHigherPt<RecoJetAK4>);

  if ( readGenMatching_ )
  {
    event.genLeptons_ = genLeptonReader_->read();
    event.genElectrons_.clear();
    event.genMuons_.clear();
    for ( auto genLepton : event.genLeptons_ )
    {
      const int abs_pdgId = std::abs(genLepton.pdgId());
      switch ( abs_pdgId )
      {
        case 11: event.genElectrons_.push_back(genLepton); break;
        case 13: event.genMuons_.push_back(genLepton);     break;
        default: assert(0);
      }
    }
    event.genHadTaus_ = genHadTauReader_->read();
    event.genPhotons_ = genPhotonReader_->read();
    event.genJets_ = genJetReader_->read();

    muonGenMatcher_->addGenLeptonMatch(event.looseMuons_, event.genMuons_);
    muonGenMatcher_->addGenHadTauMatch(event.looseMuons_, event.genHadTaus_);
    muonGenMatcher_->addGenJetMatch(event.looseMuons_, event.genJets_);

    electronGenMatcher_->addGenLeptonMatch(event.looseElectrons_ , event.genElectrons_);
    electronGenMatcher_->addGenPhotonMatch(event.looseElectrons_ , event.genPhotons_);
    electronGenMatcher_->addGenHadTauMatch(event.looseElectrons_ , event.genHadTaus_);
    electronGenMatcher_->addGenJetMatch(event.looseElectrons_ , event.genJets_);

    hadTauGenMatcher_->addGenLeptonMatch(event.fakeableHadTaus_, event.genLeptons_);
    hadTauGenMatcher_->addGenHadTauMatch(event.fakeableHadTaus_, event.genHadTaus_);
    hadTauGenMatcher_->addGenJetMatch(event.fakeableHadTaus_, event.genJets_);

    // CV: performing the gen-matching on the cleanedJetsAK4 collection
    //     adds gen-matching information to three collections of AK4 jets at once (selJetsAK4, selJetsAK4_btagLoose, selJetsAK4_btagMedium)
    jetGenMatcherAK4_->addGenLeptonMatch(cleanedJetsAK4, event.genLeptons_);
    jetGenMatcherAK4_->addGenHadTauMatch(cleanedJetsAK4, event.genHadTaus_);
    jetGenMatcherAK4_->addGenJetMatch(cleanedJetsAK4, event.genJets_);
  }

  event.jetsAK8_Hbb_ = jetReaderAK8_Hbb_->read();
  RecoJetPtrCollectionAK8 jet_ptrsAK8_Hbb = convert_to_ptrs(event.jetsAK8_Hbb_);
  // CV: clean AK8_Hbb jets wrt leptons only (not wrt hadronic taus)
  RecoJetPtrCollectionAK8 cleanedJetsAK8_Hbb = jetCleanerAK8_dR08_->operator()(jet_ptrsAK8_Hbb, event.fakeableLeptons_);
  event.selJetsAK8_Hbb_ = jetSelectorAK8_Hbb_->operator()(cleanedJetsAK8_Hbb, isHigherPt<RecoJetAK8>);
  event.jetsAK8_Wjj_ = jetReaderAK8_Wjj_->read();
  RecoJetPtrCollectionAK8 jet_ptrsAK8_Wjj = convert_to_ptrs(event.jetsAK8_Wjj_);
  // CV: AK8_Wjj jets must NOT be cleaned wrt leptons,
  //     as the lepton produced in H->WW*->lnu qq decays often ends up near the two quarks in the detector (in dR)
  if ( event.fakeableLeptons_.size() > 0 )
  {
    jetSelectorAK8_Wjj_->getSelector().set_leptons(event.fakeableLeptons_);
    event.selJetsAK8_Wjj_ = jetSelectorAK8_Wjj_->operator()(jet_ptrsAK8_Wjj, isHigherPt<RecoJetAK8>);
  }
  else
  {
    event.selJetsAK8_Wjj_.clear();
  }

  event.vertex_ = vertexReader_->read();

  metReader_->set_phiModulationCorrDetails(&eventInfo, &event.vertex_);
  event.met_ = metReader_->read();

  event.metFilters_ = metFilterReader_->read();

  return event;
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
