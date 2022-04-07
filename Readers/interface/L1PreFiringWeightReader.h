#ifndef TallinnNtupleProducer_Readers_L1PreFiringWeightReader_h
#define TallinnNtupleProducer_Readers_L1PreFiringWeightReader_h

#include "TallinnNtupleProducer/Readers/interface/ReaderBase.h" // ReaderBase

#include <Rtypes.h>                                             // Float_t

#include <map>                                                  // std::map
#include <string>                                               // std::string

// forward declarations
enum class Era;
enum class L1PreFiringWeightSys;

/**
 * @brief Reads L1 prefiring weights for 2016 & 2017 MC
 *
 * @see https://github.com/cms-sw/cmssw/pull/25380
 */
class L1PreFiringWeightReader : public ReaderBase
{
 public:
  L1PreFiringWeightReader(const edm::ParameterSet & cfg);
  ~L1PreFiringWeightReader();

  /**
   * @brief Call tree->SetBranchAddress for all branches containing LHE (scale and PDF) information
   */
  std::vector<std::string>
  setBranchAddresses(TTree * tree) override;

  double
  getWeight(L1PreFiringWeightSys option) const;

 protected:
  /**
    * @brief Initialize names of branches to be read from tree
    */
  void
  setBranchNames();

  Era era_;

  std::string branchName_;

  Float_t l1PreFiringWeight_nominal_;
  Float_t l1PreFiringWeight_up_;
  Float_t l1PreFiringWeight_down_;

  // make sure that only one L1PreFiringWeightReader instance exists for a given branchName,
  // as ROOT cannot handle multiple TTree::SetBranchAddress calls for the same branch.
  static std::map<std::string, int> numInstances_;
  static std::map<std::string, L1PreFiringWeightReader *> instances_;
};

#endif // TallinnNtupleProducer_Readers_L1PreFiringWeightReader_h

