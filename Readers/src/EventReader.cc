#include "TallinnNtupleProducer/Readers/interface/EventReader.h"

EventReader::EventReader(const edm::ParameterSet& cfg)
  : numNominalLeptons_(cfg.getParameter<unsigned>("numNominalLeptons"))
  , numNominalHadTaus_(cfg.getParameter<unsigned>("numNominalHadTaus"))
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
  , jetCleanerAK4_(nullptr)
  , jetSelectorAK4_(nullptr)
  , jetSelectorAK4_btagLoose_(nullptr)
  , jetSelectorAK4_btagMedium_(nullptr)
  , jetReaderAK8_(nullptr)
  , jetCleanerAK8_(nullptr)
  , jetSelectorAK8_(nullptr)
  , metReader_(nullptr)
  , metFilterReader_(nullptr)
  , metFilterSelector_(nullptr)
  , vertexReader_(nullptr)
{
RecoMuonReader * muonReader_;
  RecoMuonCollectionSelectorLoose * looseMuonSelector_;
  RecoMuonCollectionSelectorFakeable * fakeableMuonSelector_;
  RecoMuonCollectionSelectorTight * tightMuonSelector_;

  RecoElectronReader * electronReader_;
  RecoElectronCollectionCleaner * electronCleaner_;
  RecoElectronCollectionSelectorLoose * looseElectronSelector_;
  RecoElectronCollectionSelectorFakeable * fakeableElectronSelector_;
  RecoElectronCollectionSelectorTight * tightElectronSelector_;

  RecoHadTauReader * hadTauReader_;
  RecoHadTauCollectionCleaner * hadTauCleaner_;
  RecoHadTauCollectionSelectorLoose * looseHadTauSelector_;
  RecoHadTauCollectionSelectorFakeable * fakeableHadTauSelector_;
  RecoHadTauCollectionSelectorTight * tightHadTauSelector_;

  RecoJetReaderAK4 * jetReaderAK4_;
  RecoJetCollectionCleanerAK4 * jetCleanerAK4_;
  RecoJetCollectionSelectorAK4 * jetSelectorAK4_;
  RecoJetCollectionSelectorAK4_btagLoose * jetSelectorAK4_btagLoose_;
  RecoJetCollectionSelectorAK4_btagMedium * jetSelectorAK4_btagMedium_;

  RecoJetReaderAK8 * jetReaderAK8_;
  RecoJetCollectionCleanerAK8 * jetCleanerAK8_;
  RecoJetCollectionSelectorAK8 * jetSelectorAK8_;

  RecoMEtReader * metReader_;
  MEtFilterReader * metFilterReader_;
  MEtFilterSelector * metFilterSelector_;

  RecoVertexReader * vertexReader_;
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
  delete jetCleanerAK4_;
  delete jetSelectorAK4_;
  delete jetSelectorAK4_btagLoose_;
  delete jetSelectorAK4_btagMedium_;
  delete jetReaderAK8_;
  delete jetCleanerAK8_;
  delete jetSelectorAK8_;
  delete metReader_;
  delete metFilterReader_;
  delete metFilterSelector_;
  delete vertexReader_;
}

void
EventReader::setMEt_central_or_shift(int central_or_shift)
{

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

}

Event
EventReader::read() const
{
  Event event;

  event.eventInfo_ = ;

// TO-DO: add HLT information
  

  RecoMuonCollection muons = muonReader_->read();
  RecoMuonPtrCollection muon_ptrs = convert_to_ptrs(muons);
  RecoMuonPtrCollection cleanedMuons = muon_ptrs; // CV: no cleaning needed for muons, as they have the highest priority in the overlap removal
  event.looseMuons_ = looseMuonSelector_->operator()(cleanedMuons, isHigherConePt);
  event.fakeableMuons_ = fakeableMuonSelector_->operator()(event.looseMuons_, isHigherConePt);
  event.tightMuons = tightMuonSelector_->operator()(fakeableMuons, isHigherConePt);

  RecoElectronCollection electrons = electronReader->read();
  RecoElectronPtrCollection electron_ptrs = convert_to_ptrs(electrons);
  RecoElectronPtrCollection cleanedElectrons = electronCleaner(electron_ptrs, preselMuons);
  event.looseElectrons_ = looseElectronSelector_->operator()(cleanedElectrons, isHigherConePt);
  event.fakeableElectrons_ = fakeableElectronSelector_->operator()(preselElectrons, isHigherConePt);
  event.tightElectrons_ = tightElectronSelector_->operator()(fakeableElectrons, isHigherConePt);

  RecoLeptonPtrCollection looseLeptonsFull = mergeLeptonCollections(looseElectrons, looseMuons, isHigherConePt);
  RecoLeptonPtrCollection fakeableLeptonsFull = mergeLeptonCollections(fakeableElectrons, fakeableMuons, isHigherConePt);
  RecoLeptonPtrCollection tightLeptonsFull = mergeLeptonCollections(tightElectrons, tightMuons, isHigherConePt);
  event.looseLeptons_ = pickFirstNobjects(preselLeptonsFull, numNominalLeptons_);
  event.fakeableLeptons_ = pickFirstNobjects(fakeableLeptonsFull, numNominalLeptons_);
  event.tightLeptons_ = getIntersection(event.fakeableLeptons_, tightLeptonsFull, isHigherConePt);

// CV: make sure that pickFirstNobjects and getIntersection functions work in case numNominalLeptons and/or numNominalHadTaus is zero !!

  RecoHadTauCollection hadTaus = hadTauReader_->read();
  RecoHadTauPtrCollection hadTau_ptrs = convert_to_ptrs(hadTaus);
  RecoHadTauPtrCollection cleanedHadTaus = hadTauCleaner(hadTau_ptrs, preselMuons, preselElectrons);
  RecoHadTauPtrCollection looseHadTaus = looseHadTauSelector(cleanedHadTaus, isHigherPt);
  RecoHadTauPtrCollection fakeableHadTausFull = fakeableHadTauSelector_->operator()(looseHadTaus, isHigherPt);
  RecoHadTauPtrCollection tightHadTausFull = tightHadTauSelector_->operator()(fakeableHadTausFull, isHigherPt);
  event.fakeableHadTaus_ = pickFirstNobjects(fakeableHadTausFull, numNominalHadTaus_);
  event.tightHadTaus_ = getIntersection(event.fakeableHadTaus_, tightHadTausFull, isHigherPt);

  RecoJetCollectionAK4 jetsAK4 = jetReaderAK4_->read();
  RecoJetPtrCollectionAK4 jet_ptrsAK4 = convert_to_ptrs(jetsAK4);
  RecoJetPtrCollectionAK4 cleanedJetsAK4 = jetCleanerAK4_->operator()(jet_ptrsAK4, event.fakeableLeptons_, event._fakeableHadTaus_);
  event.selJetsAK4_ = jetSelectorAK4_->operator()(cleanedJetsAK4, isHigherPt);

  event.selBJetsAK4_loose_ = jetSelectorAK4_btagLoose_->operator()(cleanedJets, isHigherPt);
  event.selBJetsAK4_medium_ = jetSelectorAK4_btagMedium_->operator()(cleanedJets, isHigherPt);

  RecoJetCollectionAK8 jetsAK8 = jetReaderAK8_->read();
  RecoJetPtrCollectionAK4 jet_ptrsAK8 = convert_to_ptrs(jetsAK8);
  RecoJetPtrCollectionAK4 cleanedJetsAK8 = jetCleanerAK8_->operator()(jet_ptrsAK8, event.fakeableLeptons_, event._fakeableHadTaus_);
  event.selJetsAK8_ = jetSelectorAK8_->operator()(cleanedJetsAK8, isHigherPt);

  event.met_ = metReader_->read();
// TO-DO: add MET-filter information

  event.vertex_ = vertexReader_->read();

  return event;
}
