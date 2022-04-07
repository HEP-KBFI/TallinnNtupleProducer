#ifndef TallinnNtupleProducer_Readers_LHEInfoReader_h
#define TallinnNtupleProducer_Readers_LHEInfoReader_h

#include "TallinnNtupleProducer/Readers/interface/ReaderBase.h" // ReaderBase

#include <Rtypes.h>                                             // UInt_t, Float_t

#include <map>                                                  // std::map
#include <string>                                               // std::string
#include <vector>                                               // std::vector

class LHEInfoReader : public ReaderBase
{
public:
  LHEInfoReader(const edm::ParameterSet & cfg);
  ~LHEInfoReader();

  /**
   * @brief Call tree->SetBranchAddress for all branches containing LHE (scale and PDF) information
   */
  std::vector<std::string>
  setBranchAddresses(TTree * tree) override;

  /**
   * @brief Read branches from tree and return values
   * @return Weights for estimating systematic uncertainties related to scale and PDF variations
   */
  void
  read() const;

  double getWeight_scale_nominal() const;
  double getWeight_scale_xUp() const;
  double getWeight_scale_xDown() const;
  double getWeight_scale_yUp() const;
  double getWeight_scale_yDown() const;
  double getWeight_scale_xyUp() const;
  double getWeight_scale_xyDown() const;
  double getWeight_scale_Up() const;
  double getWeight_scale_Down() const;

  double getWeight_scale(int central_or_shift) const;

  int getNumWeights_pdf() const;
  double getWeight_pdf(unsigned int idx) const;

protected:
 /**
   * @brief Initialize names of branches to be read from tree
   */
  void
  setBranchNames();

  double
  getWeight(double weight,
            bool correct = true) const;

  const unsigned int max_scale_nWeights_;
  std::string branchName_scale_nWeights_;
  std::string branchName_scale_weights_;
  const unsigned int max_pdf_nWeights_;
  std::string branchName_pdf_nWeights_;
  std::string branchName_pdf_weights_;
  std::string branchName_envelope_weight_up_;
  std::string branchName_envelope_weight_down_;

  UInt_t scale_nWeights_;
  Float_t * scale_weights_;
  UInt_t pdf_nWeights_;
  Float_t * pdf_weights_;

  mutable double weight_scale_nominal_;
  mutable double weight_scale_xUp_;
  mutable double weight_scale_xDown_;
  mutable double weight_scale_yUp_;
  mutable double weight_scale_yDown_;
  mutable double weight_scale_xyUp_;
  mutable double weight_scale_xyDown_;
  mutable Float_t weight_scale_Up_;
  mutable Float_t weight_scale_Down_;

  bool has_LHE_weights_;
  mutable double correctiveFactor_;

  // CV: make sure that only one LHEInfoReader instance exists for a given branchName,
  //     as ROOT cannot handle multiple TTree::SetBranchAddress calls for the same branch.
  static std::map<std::string, int> numInstances_;
  static std::map<std::string, LHEInfoReader *> instances_;
};

#endif // tthAnalysis_HiggsToTauTau_LHEInfoReader_h

