#ifndef TallinnNtupleProducer_Readers_EventReader2_h
#define TallinnNtupleProducer_Readers_EventReader2_h

#include "FWCore/ParameterSet/interface/ParameterSet.h"                                       // edm::ParameterSet

#include "TallinnNtupleProducer/Cleaners/interface/ParticleCollectionCleaner.h"               // RecoElectronCollectionCleaner, RecoHadTauCollectionCleaner, RecoJetCollectionCleanerAK4, RecoMuonCollectionCleaner
#include "TallinnNtupleProducer/Cleaners/interface/RecoJetCollectionCleanerByIndexAK4.h"      // RecoJetCollectionCleanerByIndexAK4
#include "TallinnNtupleProducer/Readers/interface/ReaderBase.h"                               // ReaderBase
#include "TallinnNtupleProducer/Readers/interface/RecoMEtReader.h"                            // RecoMEtReader

#include <set>                                                                                // std::set
#include <string>                                                                             // std::string
#include <vector>                                                                             // std::vector

// forward declarations
class TTree;

enum class Era;

class EventReader2 : public ReaderBase
{
 public:
  EventReader2(const edm::ParameterSet& cfg);
  ~EventReader2();

  void
  set_central_or_shift(const std::string& central_or_shift);

  /**
   * @brief Call tree->SetBranchAddress for all particle-collection reader classes
   */
  std::vector<std::string>
  setBranchAddresses(TTree * inputTree) override;

  /**
   * @brief Read branches from tree and use information to fill Event object
   * @return Event object
   */
  void
  read() const;

  /**
    * @brief Return list of systematic uncertainties supported by EventReader class
    */
  static
  std::vector<std::string>
  get_supported_systematics(const edm::ParameterSet & cfg);

 protected:
  //edm::ParameterSet cfg_;

  unsigned numNominalLeptons_;
  unsigned numNominalHadTaus_;

  Era era_;
  bool isMC_;
  bool readGenMatching_;
  
  RecoMEtReader * metReader_;

  bool isDEBUG_;
};

#endif // TallinnNtupleProducer_Readers_EventReader2_h
