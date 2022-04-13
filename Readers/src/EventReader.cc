#include "TallinnNtupleProducer/Readers/interface/EventReader.h"

#include "TallinnNtupleProducer/Readers/interface/convert_to_ptrs.h" // convert_to_ptrs()

EventReader::EventReader(const edm::ParameterSet& cfg)
  : ReaderBase(cfg)
  , eventInfoReader_(nullptr)
  , numNominalLeptons_(0)
  , numNominalHadTaus_(0)
  , era_(Era::kUndefined)
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
  , jetReaderAK8_(nullptr)
  , jetCleanerAK8_dR08_(nullptr)
  , jetSelectorAK8_(nullptr)
  , jetSelectorAK8_Hbb_(nullptr)
  , jetSelectorAK8_Wjj_(nullptr)
  , metReader_(nullptr)
  , metFilterReader_(nullptr)
  , metFilterSelector_(nullptr)
  , vertexReader_(nullptr)
  , isDEBUG_(cfg.getParameter<bool>("isDEBUG"))
{
  numNominalLeptons_ = cfg.getParameter<unsigned>("numNominalLeptons");
  numNominalHadTaus_ = cfg.getParameter<unsigned>("numNominalHadTaus");

  era_ = get_era(cfg.getParameter<std::string>("era"));
  isMC_ = cfg.getParameter<bool>("isMC");

  eventInfoReader_ = new EventInfoReader(cfg);

  triggerInfoReader_ = new TriggerInfoReader(cfg);

  muonReader_ = new RecoMuonReader(cfg);
  looseMuonSelector_ = new RecoMuonCollectionSelectorLoose(era_, -1, isDEBUG_);
  fakeableMuonSelector_ = new RecoMuonCollectionSelectorFakeable(era_, -1, isDEBUG_);
  tightMuonSelector_ = new RecoMuonCollectionSelectorTight(era_, -1, isDEBUG_);

  electronReader_ = new RecoElectronReader(cfg);
  electronCleaner_ = new RecoElectronCollectionCleaner(0.3, isDEBUG_);
  looseElectronSelector_ = new RecoElectronCollectionSelectorLoose(era_, -1, isDEBUG_);
  fakeableElectronSelector_ = new RecoElectronCollectionSelectorFakeable(era_, -1, isDEBUG_);
  tightElectronSelector_ = new RecoElectronCollectionSelectorTight(era_, -1, isDEBUG_);

  hadTauReader_ = new RecoHadTauReader(cfg);
  hadTauCleaner_ = new RecoHadTauCollectionCleaner(0.3, isDEBUG_);
  looseHadTauSelector_ = new RecoHadTauCollectionSelectorLoose(era_, -1, isDEBUG_);
  fakeableHadTauSelector_ = new RecoHadTauCollectionSelectorFakeable(era_, -1, isDEBUG_);
  tightHadTauSelector_ = new RecoHadTauCollectionSelectorTight(era_, -1, isDEBUG_);

  jetReaderAK4_ = new RecoJetReaderAK4(cfg);
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

  jetReaderAK8_ = new RecoJetReaderAK8(cfg);
  jetCleanerAK8_dR08_ = new RecoJetCollectionCleanerAK8(0.8, isDEBUG_);
  jetSelectorAK8_ = new RecoJetCollectionSelectorAK8(era_, -1, isDEBUG_);
  jetSelectorAK8_Hbb_ = new RecoJetCollectionSelectorAK8_Hbb(era_, -1, isDEBUG_);
  jetSelectorAK8_Wjj_ = new RecoJetCollectionSelectorAK8_Wjj(era_, -1, isDEBUG_);

  metReader_ = new RecoMEtReader(cfg);
  metFilterReader_ = new MEtFilterReader(cfg);
  edm::ParameterSet cfg_metFilters = cfg.getParameter<edm::ParameterSet>("metFilters");
  metFilterSelector_ = new MEtFilterSelector(cfg_metFilters, isMC_);

  vertexReader_ = new RecoVertexReader(cfg);
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
  delete jetReaderAK8_;
  delete jetCleanerAK8_dR08_;
  delete jetSelectorAK8_;
  delete jetSelectorAK8_Hbb_;
  delete jetSelectorAK8_Wjj_;
  delete metReader_;
  delete metFilterReader_;
  delete metFilterSelector_;
  delete vertexReader_;
}

void
EventReader::set_central_or_shift(int central_or_shift)
{
 
}

std::vector<std::string>
EventReader::setBranchAddresses(TTree * tree)
{
  eventInfoReader_->setBranchAddresses(tree);
  triggerInfoReader_->setBranchAddresses(tree);
  muonReader_->setBranchAddresses(tree);
  electronReader_->setBranchAddresses(tree);
  hadTauReader_->setBranchAddresses(tree);
  jetReaderAK4_->setBranchAddresses(tree);
  jetReaderAK8_->setBranchAddresses(tree);
  metReader_->setBranchAddresses(tree);
  metFilterReader_->setBranchAddresses(tree);
  vertexReader_->setBranchAddresses(tree);
  return {};
}

//namespace
//{
//  template <typename T>
//  void
//  add_to_vector(std::vector<T>& vec, const std::vector<T>& to_add)
//  {
//    vec.insert(vec.end(), to_add.begin(), to_add.end());
//  }
//}
//
//std::vector<std::string>
//EventReader::setBranchAddresses(TTree * tree)
//{
//inputTree -> registerReader(muonReader);
//
// CV: this code is probably not needed -> call inputTree->registerReader(eventReader) in produceNtuple.cc instead !!
//
//  std::vector<std::string> branchNames;
//  add_to_vector(branchNames, muonReader_->setBranchAddresses(tree));
//  add_to_vector(branchNames, electronReader_->setBranchAddresses(tree));
//  add_to_vector(branchNames, hadTauReader_->setBranchAddresses(tree));
//  add_to_vector(branchNames, jetReaderAK4_->setBranchAddresses(tree));
//  add_to_vector(branchNames, jetReaderAK8_->setBranchAddresses(tree));
//  add_to_vector(branchNames, metReader_->setBranchAddresses(tree));
//  add_to_vector(branchNames, metFilterReader_->setBranchAddresses(tree));
//  add_to_vector(branchNames, vertexReader_->setBranchAddresses(tree));
//}

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
   * @brief Auxiliary function used for sorting hadronic taus and jets by decreasing pT
   * @param Given pair of either hadronic taus or jets
   * @return True, if first particle has higher pT; false if second particle has higher pT
   */
  template <typename T>
  bool
  isHigherPt(const T * particle1, const T * particle2)
  {
    return particle1->pt() > particle2->pt();
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

  RecoMuonCollection muons = muonReader_->read();
  RecoMuonPtrCollection muon_ptrs = convert_to_ptrs(muons);
  RecoMuonPtrCollection cleanedMuons = muon_ptrs; // CV: no cleaning needed for muons, as they have the highest priority in the overlap removal
  event.looseMuons_ = looseMuonSelector_->operator()(cleanedMuons, isHigherConePt<RecoMuon>);
  RecoMuonPtrCollection fakeableMuonsFull = fakeableMuonSelector_->operator()(event.looseMuons_, isHigherConePt<RecoMuon>);
  RecoMuonPtrCollection tightMuonsFull = tightMuonSelector_->operator()(fakeableMuonsFull, isHigherConePt<RecoMuon>);
  event.fakeableMuons_ = pickFirstNobjects(fakeableMuonsFull, numNominalLeptons_);
  event.tightMuons_ = getIntersection(event.fakeableMuons_, tightMuonsFull, isHigherConePt<RecoMuon>);

  RecoElectronCollection electrons = electronReader_->read();
  RecoElectronPtrCollection electron_ptrs = convert_to_ptrs(electrons);
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

  RecoHadTauCollection hadTaus = hadTauReader_->read();
  RecoHadTauPtrCollection hadTau_ptrs = convert_to_ptrs(hadTaus);
  RecoHadTauPtrCollection cleanedHadTaus = hadTauCleaner_->operator()(hadTau_ptrs, event.looseMuons_, event.looseElectrons_);
  RecoHadTauPtrCollection fakeableHadTausFull = fakeableHadTauSelector_->operator()(cleanedHadTaus, isHigherPt<RecoHadTau>);
  RecoHadTauPtrCollection tightHadTausFull = tightHadTauSelector_->operator()(fakeableHadTausFull, isHigherPt<RecoHadTau>);
  event.fakeableHadTaus_ = pickFirstNobjects(fakeableHadTausFull, numNominalHadTaus_);
  event.tightHadTaus_ = getIntersection(event.fakeableHadTaus_, tightHadTausFull, isHigherPt<RecoHadTau>);

  RecoJetCollectionAK4 jetsAK4 = jetReaderAK4_->read();
  RecoJetPtrCollectionAK4 jet_ptrsAK4 = convert_to_ptrs(jetsAK4);
  RecoJetPtrCollectionAK4 cleanedJetsAK4 = jetCleanerAK4_dR04_->operator()(jet_ptrsAK4, event.fakeableLeptons_, event.fakeableHadTaus_);
  event.selJetsAK4_ = jetSelectorAK4_->operator()(cleanedJetsAK4, isHigherPt<RecoJetAK4>);
  event.selJetsAK4_btagLoose_ = jetSelectorAK4_btagLoose_->operator()(cleanedJetsAK4, isHigherPt<RecoJetAK4>);
  event.selJetsAK4_btagMedium_ = jetSelectorAK4_btagMedium_->operator()(cleanedJetsAK4, isHigherPt<RecoJetAK4>);

  RecoJetCollectionAK8 jetsAK8 = jetReaderAK8_->read();
  RecoJetPtrCollectionAK8 jet_ptrsAK8 = convert_to_ptrs(jetsAK8);
  RecoJetPtrCollectionAK8 cleanedJetsAK8 = jetCleanerAK8_dR08_->operator()(jet_ptrsAK8, event.fakeableLeptons_, event.fakeableHadTaus_);
  event.selJetsAK8_ = jetSelectorAK8_->operator()(cleanedJetsAK8, isHigherPt<RecoJetAK8>);
  // CV: clean AK8_Hbb jets wrt leptons only (not wrt hadronic taus)
  RecoJetPtrCollectionAK8 cleanedJetsAK8_Hbb = jetCleanerAK8_dR08_->operator()(jet_ptrsAK8, event.fakeableLeptons_);
  event.selJetsAK8_Hbb_ = jetSelectorAK8_Hbb_->operator()(cleanedJetsAK8_Hbb, isHigherPt<RecoJetAK8>);
  // CV: AK8_Wjj jets must NOT be cleaned wrt leptons,
  //     as the lepton produced in H->WW*->lnu qq decays often ends up near the two quarks in the detector (in dR)
  jetSelectorAK8_Wjj_->getSelector().set_leptons(event.fakeableLeptons_);
  event.selJetsAK8_Wjj_ = jetSelectorAK8_Wjj_->operator()(jet_ptrsAK8, isHigherPt<RecoJetAK8>);

  event.met_ = metReader_->read();
  event.metFilters_ = metFilterReader_->read();

  event.vertex_ = vertexReader_->read();

  return event;
}
