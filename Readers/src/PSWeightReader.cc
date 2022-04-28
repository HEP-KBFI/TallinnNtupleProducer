#include "TallinnNtupleProducer/Readers/interface/PSWeightReader.h"

#include "TallinnNtupleProducer/CommonTools/interface/cmsException.h"         // cmsException
#include "TallinnNtupleProducer/CommonTools/interface/sysUncertOptions.h"     // kPartonShower_*
#include "TallinnNtupleProducer/Readers/interface/BranchAddressInitializer.h" // BranchAddressInitializer
#include "TallinnNtupleProducer/Readers/interface/clip.h"                     // clip()

#include <TString.h>                                                          // Form
#include <TTree.h>                                                            // TTree

#include <assert.h>                                                           // assert()

std::map<std::string, int> PSWeightReader::numInstances_;
std::map<std::string, PSWeightReader*> PSWeightReader::instances_;

PSWeightReader::PSWeightReader(const edm::ParameterSet & cfg)
  : ReaderBase(cfg)
  , max_nPSWeights_(4)
  , branchName_nPSWeights_("nPSWeight")
  , branchName_PSweights_("PSWeight")
  , branchName_LHE_nom_("LHEWeight_originalXWGTUP")
  , ps_nWeights_(0)
  , ps_weights_(nullptr)
  , LHE_nom_(1.)
  , weight_ps_ISRUp_(1.)
  , weight_ps_ISRDown_(1.)
  , weight_ps_FSRUp_(1.)
  , weight_ps_FSRDown_(1.)
  , weight_ps_Up_(1.)
  , weight_ps_Down_(1.)
  , has_PS_weights_(false)
  , apply_LHE_nom_(false)
{
  has_PS_weights_ = cfg.getParameter<bool>("has_PS_weights");
  apply_LHE_nom_ = cfg.getParameter<bool>("apply_LHE_nom");
  setBranchNames();
}

PSWeightReader::~PSWeightReader()
{
  --numInstances_[branchName_PSweights_];
  assert(numInstances_[branchName_PSweights_] >= 0);

  if(numInstances_[branchName_PSweights_] == 0)
  {
    PSWeightReader * const gInstance = instances_[branchName_PSweights_];
    assert(gInstance);
    delete[] gInstance->ps_weights_;
    instances_[branchName_PSweights_] = nullptr;
  }
}

void
PSWeightReader::setBranchNames()
{
  if(numInstances_[branchName_PSweights_] == 0)
  {
    instances_[branchName_PSweights_] = this;
  }
  else
  {
    const PSWeightReader * const gInstance = instances_[branchName_PSweights_];
    if(branchName_PSweights_ != gInstance->branchName_PSweights_ ||
       branchName_LHE_nom_   != gInstance->branchName_LHE_nom_    )
    {
      throw cmsException(this)
        << "Association between configuration parameters 'branchName_scale_weights' and 'branchName_pdf_weights'"
           " must be unique: present association 'branchName_PSweights' = " << branchName_PSweights_
        << " with 'branchName_LHE_nom' = " << branchName_LHE_nom_
        << " does not match previous association 'branchName_PSweights' = " << gInstance->branchName_PSweights_
        << " with 'branchName_LHE_nom' = " << gInstance->branchName_LHE_nom_ << " !!\n";
    }
  }
  ++numInstances_[branchName_PSweights_];
}

std::vector<std::string>
PSWeightReader::setBranchAddresses(TTree * tree)
{
  if(instances_[branchName_PSweights_] == this)
  {
    BranchAddressInitializer bai(tree);
    if(has_PS_weights_)
    {
      bai.setBranchAddress(ps_nWeights_, branchName_nPSWeights_);
      bai.setLenVar(max_nPSWeights_).setBranchAddress(ps_weights_, branchName_PSweights_);
    }
    if(apply_LHE_nom_)
    {
      bai.setBranchAddress(LHE_nom_, branchName_LHE_nom_, 1.);
    }
    return bai.getBoundBranchNames();
  }
  return {};
}

void
PSWeightReader::read() const
{
  const PSWeightReader * const gInstance = instances_[branchName_PSweights_];
  assert(gInstance);
  if(has_PS_weights_)
  {
    if(gInstance->ps_nWeights_ == max_nPSWeights_)
    {
      const double corrFactor = apply_LHE_nom_ ? LHE_nom_ : 1.;
      weight_ps_ISRUp_   = gInstance->ps_weights_[2] * corrFactor;
      weight_ps_ISRDown_ = gInstance->ps_weights_[0] * corrFactor;
      weight_ps_FSRUp_   = gInstance->ps_weights_[3] * corrFactor;
      weight_ps_FSRDown_ = gInstance->ps_weights_[1] * corrFactor;
      weight_ps_Up_      = std::max({ weight_ps_ISRUp_, weight_ps_ISRDown_, weight_ps_FSRUp_, weight_ps_FSRDown_ });
      weight_ps_Down_    = std::min({ weight_ps_ISRUp_, weight_ps_ISRDown_, weight_ps_FSRUp_, weight_ps_FSRDown_ });
    }
    else
    {
      weight_ps_ISRUp_   = 1.;
      weight_ps_ISRDown_ = 1.;
      weight_ps_FSRUp_   = 1.;
      weight_ps_FSRDown_ = 1.;
      weight_ps_Up_      = 1.;
      weight_ps_Down_    = 1.;
      std::cerr << "Unexpected number of PS weights: " << gInstance->ps_nWeights_ << '\n';
      return;
    }
  }
}

double
PSWeightReader::getWeight_ps_ISRUp() const
{
  return clip(weight_ps_ISRUp_);
}

double
PSWeightReader::getWeight_ps_ISRDown() const
{
  return clip(weight_ps_ISRDown_);
}

double
PSWeightReader::getWeight_ps_FSRUp() const
{
  return clip(weight_ps_FSRUp_);
}

double
PSWeightReader::getWeight_ps_FSRDown() const
{
  return clip(weight_ps_FSRDown_);
}

double
PSWeightReader::getWeight_ps_Up() const
{
  return clip(weight_ps_Up_);
}

double
PSWeightReader::getWeight_ps_Down() const
{
  return clip(weight_ps_Down_);
}

double
PSWeightReader::getWeight_ps(int central_or_shift) const
{
  switch(central_or_shift)
  {
    case kPartonShower_central: return 1.;
    case kPartonShower_ISRDown: return getWeight_ps_ISRDown();
    case kPartonShower_ISRUp:   return getWeight_ps_ISRUp();
    case kPartonShower_FSRDown: return getWeight_ps_FSRDown();
    case kPartonShower_FSRUp:   return getWeight_ps_FSRUp();
    case kPartonShower_Down:    return getWeight_ps_Down();
    case kPartonShower_Up:      return getWeight_ps_Up();
    default: throw cmsException(this, __func__, __LINE__)
               << "Invalid PS systematics option: " << central_or_shift;
  }
}

double
PSWeightReader::get_LHE_nom() const
{
  return LHE_nom_;
}

