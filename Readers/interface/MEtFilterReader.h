#ifndef TallinnNtupleProducer_Readers_MEtFilterReader_h
#define TallinnNtupleProducer_Readers_MEtFilterReader_h

#include "TallinnNtupleProducer/Objects/interface/MEtFilterFlag.h" // MEtFilterFlag::
#include "TallinnNtupleProducer/Readers/interface/ReaderBase.h"    // ReaderBase

#include <array>                                                   // std::array

// forward declarations
class MEtFilter;
enum class Era;

class MEtFilterReader : public ReaderBase
{
 public:
  MEtFilterReader(MEtFilter * metFilter,
                  Era era);
  ~MEtFilterReader();

  std::vector<std::string>
  setBranchAddresses(TTree * tree) override;

 protected:
 /**
   * @brief Initialize names of branches to be read from tree
   */
  void
  setBranchNames();

  std::array<std::string, MEtFilterFlag::LAST> branchNames_;

  // CV: make sure that only one MEtFilterReader instance exists,
  static int numInstances_;
  static MEtFilterReader * instance_;

  MEtFilter metFilter_;
  Era era_;
};

#endif // TallinnNtupleProducer_Readers_MEtFilterReader_h
