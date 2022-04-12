#ifndef TallinnNtupleProducer_Readers_EventReader_h
#define TallinnNtupleProducer_Readers_EventReader_h

#include "FWCore/ParameterSet/interface/ParameterSet.h"                                       // edm::ParameterSet

#include "TallinnNtupleProducer/Cleaners/interface/ParticleCollectionCleaner.h"               // RecoElectronCollectionCleaner, RecoHadTauCollectionCleaner, RecoJetCollectionCleanerAK4, RecoMuonCollectionCleaner
#include "TallinnNtupleProducer/Cleaners/interface/RecoJetCollectionCleanerByIndexAK4.h"      // RecoJetCollectionCleanerByIndexAK4
#include "TallinnNtupleProducer/Objects/interface/Event.h"                                    // Event
#include "TallinnNtupleProducer/Readers/interface/EventInfoReader.h"                          // EventInfoReader
#include "TallinnNtupleProducer/Readers/interface/MEtFilterReader.h"                          // MEtFilterReader
#include "TallinnNtupleProducer/Readers/interface/ReaderBase.h"                               // ReaderBase
#include "TallinnNtupleProducer/Readers/interface/RecoElectronReader.h"                       // RecoElectronReader
#include "TallinnNtupleProducer/Readers/interface/RecoHadTauReader.h"                         // RecoHadTauReader
#include "TallinnNtupleProducer/Readers/interface/RecoJetReaderAK4.h"                         // RecoJetReaderAK4
#include "TallinnNtupleProducer/Readers/interface/RecoJetReaderAK8.h"                         // RecoJetReaderAK8
#include "TallinnNtupleProducer/Readers/interface/RecoMEtReader.h"                            // RecoMEtReader
#include "TallinnNtupleProducer/Readers/interface/RecoMuonReader.h"                           // RecoMuonReader
#include "TallinnNtupleProducer/Readers/interface/RecoVertexReader.h"                         // RecoVertexReader
#include "TallinnNtupleProducer/Selectors/interface/MEtFilterSelector.h"                      // MEtFilterSelector
#include "TallinnNtupleProducer/Selectors/interface/RecoElectronCollectionSelectorFakeable.h" // RecoElectronCollectionSelectorFakeable
#include "TallinnNtupleProducer/Selectors/interface/RecoElectronCollectionSelectorLoose.h"    // RecoElectronCollectionSelectorLoose
#include "TallinnNtupleProducer/Selectors/interface/RecoElectronCollectionSelectorTight.h"    // RecoElectronCollectionSelectorTight
#include "TallinnNtupleProducer/Selectors/interface/RecoHadTauCollectionSelectorFakeable.h"   // RecoHadTauCollectionSelectorFakeable
#include "TallinnNtupleProducer/Selectors/interface/RecoHadTauCollectionSelectorLoose.h"      // RecoHadTauCollectionSelectorLoose
#include "TallinnNtupleProducer/Selectors/interface/RecoHadTauCollectionSelectorTight.h"      // RecoHadTauCollectionSelectorTight
#include "TallinnNtupleProducer/Selectors/interface/RecoJetCollectionSelectorAK4.h"           // RecoJetCollectionSelectorAK4
#include "TallinnNtupleProducer/Selectors/interface/RecoJetCollectionSelectorAK4_btag.h"      // RecoJetCollectionSelectorAK4_btag
#include "TallinnNtupleProducer/Selectors/interface/RecoJetCollectionSelectorAK8.h"           // RecoJetCollectionSelectorAK8
#include "TallinnNtupleProducer/Selectors/interface/RecoMuonCollectionSelectorFakeable.h"     // RecoMuonCollectionSelectorFakeable
#include "TallinnNtupleProducer/Selectors/interface/RecoMuonCollectionSelectorLoose.h"        // RecoMuonCollectionSelectorLoose
#include "TallinnNtupleProducer/Selectors/interface/RecoMuonCollectionSelectorTight.h"        // RecoMuonCollectionSelectorTight

#include <set>                                                                                // std::set
#include <string>                                                                             // std::string
#include <vector>                                                                             // std::vector

// forward declarations
class TTree;

//namespace
//{
//  void
//  add_systematic_shifts(std::set<std::string> & systematic_shifts, const std::vector<std::string> & shifts_to_add)
//  {
//    for ( auto shift : shifts_to_add )
//    {
//      systematic_shifts.insert(shift);
//    }
//  }
//
//  std::vector<std::string>
//  convert_to_vector(std::set<std::string> & systematic_shifts)
//  {
//    std::vector<std::string> retVal;
//    for ( auto shift : systematic_shifts )
//    {
//      retVal.push_back(shift);
//    }
//    return retVal;
//  }
//}

class EventReader : public ReaderBase
{
 public:
  EventReader(const edm::ParameterSet& cfg);
  ~EventReader();

  void
  set_central_or_shift(int central_or_shift);

  /**
   * @brief Call tree->SetBranchAddress for all particle-collection reader classes
   */
  std::vector<std::string>
  setBranchAddresses(TTree * tree) override;

  /**
   * @brief Read branches from tree and use information to fill Event object
   * @return Event object
   */
  Event
  read() const;

 protected:
  unsigned numNominalLeptons_;
  unsigned numNominalHadTaus_;

  EventInfoReader * eventInfoReader_;

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
};

#endif // TallinnNtupleProducer_Readers_EventReader_h