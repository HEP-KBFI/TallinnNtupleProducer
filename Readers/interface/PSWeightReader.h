#ifndef tthAnalysis_HiggsToTauTau_PSWeightReader_h
#define tthAnalysis_HiggsToTauTau_PSWeightReader_h

#include "tthAnalysis/HiggsToTauTau/interface/ReaderBase.h" // ReaderBase

#include <Rtypes.h> // *_t

#include <map> // std::map<,>

class PSWeightReader
  : public ReaderBase
{
public:
  PSWeightReader(bool has_PS_weights,
                 bool apply_LHE_nom);
  ~PSWeightReader();

  /**
   * @brief Call tree->SetBranchAddress for all branches containing parton shower (ISR and FSR) information
   */
  std::vector<std::string>
  setBranchAddresses(TTree * tree) override;

  /**
   * @brief Read branches from tree and return values
   * @return Weights for estimating systematic uncertainties related to scale and PDF variations
   */
  void
  read() const;

  double getWeight_ps_ISRUp() const;
  double getWeight_ps_ISRDown() const;
  double getWeight_ps_FSRUp() const;
  double getWeight_ps_FSRDown() const;
  double getWeight_ps_Up() const;
  double getWeight_ps_Down() const;

  double getWeight_ps(int central_or_shift) const;

  double get_LHE_nom() const;

protected:
  /**
    * @brief Initialize names of branches to be read from tree
    */
   void
   setBranchNames();

   const unsigned int max_nPSWeights_;
   std::string branchName_nPSWeights_;
   std::string branchName_PSweights_;
   std::string branchName_LHE_nom_;

   UInt_t ps_nWeights_;
   Float_t * ps_weights_;
   Float_t LHE_nom_;

   mutable double weight_ps_ISRUp_;
   mutable double weight_ps_ISRDown_;
   mutable double weight_ps_FSRUp_;
   mutable double weight_ps_FSRDown_;
   mutable double weight_ps_Up_;
   mutable double weight_ps_Down_;

   bool has_PS_weights_;
   bool apply_LHE_nom_;

   static std::map<std::string, int> numInstances_;
   static std::map<std::string, PSWeightReader *> instances_;
};

#endif // tthAnalysis_HiggsToTauTau_PSWeightReader_h
