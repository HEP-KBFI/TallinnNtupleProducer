#ifndef TallinnNtupleProducer_Readers_RecoMEtReader_h
#define TallinnNtupleProducer_Readers_RecoMEtReader_h

#include "FWCore/ParameterSet/interface/ParameterSet.h"         // edm::ParameterSet

#include "TallinnNtupleProducer/Objects/interface/RecoMEt.h"    // RecoMEt
#include "TallinnNtupleProducer/Readers/interface/ReaderBase.h" // ReaderBase

#include <map>                                                  // std::map
#include <string>                                               // std::string

// forward declarations
class TTree;
class EventInfo;
class RecoVertex;

enum class Era;

class RecoMEtReader : public ReaderBase
{
 public:
  RecoMEtReader(const edm::ParameterSet & cfg);
  ~RecoMEtReader();

  void
  setMEt_central_or_shift(int met_option);

  void
  read_ptPhi_systematics(bool flag);

  /**
   * @brief Call tree->SetBranchAddress for all RecoMEt branches
   */
  std::vector<std::string>
  setBranchAddresses(TTree * tree) override;

  void
  set_phiModulationCorrDetails(const EventInfo * const eventInfo,
                               const RecoVertex * const recoVertex,
                               bool enable_phiModulationCorr = true);

  /**
   * @brief Read branches from tree and use information to fill RecoMEt object
   * @return RecoMEt object
   */
  RecoMEt
  read() const;

 protected:
 /**
   * @brief Initialize names of branches to be read from tree
   */
  void
  setBranchNames();

  Era era_;
  bool isMC_;
  std::string branchName_obj_;
  std::string branchName_cov_;

  std::map<int, std::string> branchName_pt_;
  std::map<int, std::string> branchName_phi_;
  std::string branchName_sumEt_;
  std::string branchName_covXX_;
  std::string branchName_covXY_;
  std::string branchName_covYY_;

  RecoMEt met_;
  const EventInfo * eventInfo_;
  const RecoVertex * recoVertex_;
  bool enable_phiModulationCorr_;

  int ptPhiOption_;
  bool read_ptPhi_systematics_;

  // CV: make sure that only one RecoMEtReader instance exists for a given branchName,
  //     as ROOT cannot handle multiple TTree::SetBranchAddress calls for the same branch.
  static std::map<std::string, int> numInstances_;
  static std::map<std::string, RecoMEtReader *> instances_;
};

#endif // TallinnNtupleProducer_Readers_RecoMEtReader_h
