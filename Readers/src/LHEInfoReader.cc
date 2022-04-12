#include "TallinnNtupleProducer/Readers/interface/LHEInfoReader.h"

#include "TallinnNtupleProducer/CommonTools/interface/cmsException.h"         // cmsException
#include "TallinnNtupleProducer/CommonTools/interface/jetDefinitions.h"       // Btag
#include "TallinnNtupleProducer/CommonTools/interface/sysUncertOptions.h"     // kLHE_scale_*
#include "TallinnNtupleProducer/Readers/interface/BranchAddressInitializer.h" // BranchAddressInitializer
#include "TallinnNtupleProducer/Readers/interface/clip.h"                     // clip()

#include <TString.h>                                                          // Form
#include <TTree.h>                                                            // TTree

#include <assert.h>                                                           // assert()
#include <iostream>                                                           // std::cerr

std::map<std::string, int> LHEInfoReader::numInstances_;
std::map<std::string, LHEInfoReader*> LHEInfoReader::instances_;

LHEInfoReader::LHEInfoReader(const edm::ParameterSet & cfg)
  : ReaderBase(cfg)
  , max_scale_nWeights_(9)
  , branchName_scale_nWeights_("")
  , branchName_scale_weights_("")
  , max_pdf_nWeights_(103)
  , branchName_pdf_nWeights_("")
  , branchName_pdf_weights_("")
  , branchName_envelope_weight_up_("")
  , branchName_envelope_weight_down_("")
  , scale_nWeights_(0)
  , scale_weights_(nullptr)
  , pdf_nWeights_(0)
  , pdf_weights_(nullptr)
  , weight_scale_nominal_(1.)
  , weight_scale_xUp_(1.)
  , weight_scale_xDown_(1.)
  , weight_scale_yUp_(1.)
  , weight_scale_yDown_(1.)
  , weight_scale_xyUp_(1.)
  , weight_scale_xyDown_(1.)
  , weight_scale_Up_(1.)
  , weight_scale_Down_(1.)
  , has_LHE_weights_(false)
  , correctiveFactor_(1.)
{
  branchName_scale_weights_ = cfg.getParameter<std::string>("branchName_scale_weights");               // default = "LHEScaleWeight"
  branchName_scale_nWeights_ = Form("n%s", branchName_scale_weights_.data());
  branchName_pdf_weights_ = cfg.getParameter<std::string>("branchName_pdf_weights");                   // default = "LHEPdfWeight"
  branchName_pdf_nWeights_ = Form("n%s", branchName_pdf_weights_.data());
  branchName_envelope_weight_up_ = cfg.getParameter<std::string>("branchName_envelope_weight_up");     // default = "LHEEnvelopeWeightUp"
  branchName_envelope_weight_down_ = cfg.getParameter<std::string>("branchName_envelope_weight_down"); // default = "LHEEnvelopeWeightDown"
  has_LHE_weights_ = cfg.getParameter<bool>("has_LHE_weights");
  setBranchNames();
}

LHEInfoReader::~LHEInfoReader()
{
  --numInstances_[branchName_scale_weights_];
  assert(numInstances_[branchName_scale_weights_] >= 0);

  if(numInstances_[branchName_scale_weights_] == 0)
  {
    LHEInfoReader * const gInstance = instances_[branchName_scale_weights_];
    assert(gInstance);
    delete[] gInstance->scale_weights_;
    delete[] gInstance->pdf_weights_;
    instances_[branchName_scale_weights_] = nullptr;
  }
}

void
LHEInfoReader::setBranchNames()
{
  if(numInstances_[branchName_scale_weights_] == 0)
  {
    instances_[branchName_scale_weights_] = this;
  }
  else if(has_LHE_weights_)
  {
    const LHEInfoReader * const gInstance = instances_[branchName_scale_weights_];
    if(branchName_scale_nWeights_ != gInstance->branchName_scale_nWeights_ ||
       branchName_pdf_nWeights_   != gInstance->branchName_pdf_nWeights_   ||
       branchName_pdf_weights_    != gInstance->branchName_pdf_weights_     )
    {
      throw cmsException(this)
        << "Association between configuration parameters 'branchName_scale_weights' and 'branchName_pdf_weights'"
           " must be unique: present association 'branchName_scale_weights' = " << branchName_scale_weights_
        << " with 'branchName_pdf_weights' = " << branchName_pdf_weights_
        << " does not match previous association 'branchName_scale_weights' = " << gInstance->branchName_scale_weights_
        << " with 'branchName_pdf_weights' = " << gInstance->branchName_pdf_weights_ << " !!\n";
    }
  }
  ++numInstances_[branchName_scale_weights_];
}

std::vector<std::string>
LHEInfoReader::setBranchAddresses(TTree * tree)
{
  if(instances_[branchName_scale_weights_] == this && has_LHE_weights_)
  {
    BranchAddressInitializer bai(tree);
    bai.setBranchAddress(scale_nWeights_, branchName_scale_nWeights_);
    bai.setBranchAddress(pdf_nWeights_, branchName_pdf_nWeights_);
    bai.setLenVar(max_scale_nWeights_).setBranchAddress(scale_weights_, branchName_scale_weights_);
    bai.setLenVar(max_pdf_nWeights_).setBranchAddress(pdf_weights_, branchName_pdf_weights_);
    bai.setBranchAddress(weight_scale_Up_, branchName_envelope_weight_up_, 1.);
    bai.setBranchAddress(weight_scale_Down_, branchName_envelope_weight_down_, 1.);
    return bai.getBoundBranchNames();
  }
  return {};
}

void
LHEInfoReader::read() const
{
  const LHEInfoReader * const gInstance = instances_[branchName_scale_weights_];
  assert(gInstance);
  if(! has_LHE_weights_)
  {
    return;
  }
  if(gInstance->scale_nWeights_ > max_scale_nWeights_)
  {
    throw cmsException(this)
      << "Number of Scale weights stored in Ntuple = " << gInstance->scale_nWeights_
      << ", exceeds max_scale_nWeights_ = " << max_scale_nWeights_ << " !!\n";
  }

  // additional factor of two: https://github.com/HEP-KBFI/tth-htt/issues/149#issuecomment-653760494
  correctiveFactor_ = gInstance->scale_nWeights_ == 8 ? 2. : 1.;

  // Karl: the nomenclature depends on the MG version used
  // below MG ver 2.6:
  //    [0] is muR=0.5 muF=0.5
  //    [1] is muR=0.5 muF=1.0
  //    [2] is muR=0.5 muF=2.0
  //    [3] is muR=1.0 muF=0.5
  //    [4] is muR=1.0 muF=1.0 <- missing if there are only 8 weights available
  //    [5] is muR=1.0 muF=2.0
  //    [6] is muR=2.0 muF=0.5
  //    [7] is muR=2.0 muF=1.0
  //    [8] is muR=2.0 muF=2.0
  if(gInstance->scale_nWeights_ == 9)
  {
    weight_scale_xyDown_  = gInstance->scale_weights_[0]; // muR=0.5 muF=0.5
    weight_scale_yDown_   = gInstance->scale_weights_[1]; // muR=0.5 muF=1.0
    weight_scale_xDown_   = gInstance->scale_weights_[3]; // muR=1.0 muF=0.5
    weight_scale_nominal_ = gInstance->scale_weights_[4]; // muR=1.0 muF=1.0
    weight_scale_xUp_     = gInstance->scale_weights_[5]; // muR=1.0 muF=2.0
    weight_scale_yUp_     = gInstance->scale_weights_[7]; // muR=2.0 muF=1.0
    weight_scale_xyUp_    = gInstance->scale_weights_[8]; // muR=2.0 muF=2.0
  }
  else if(gInstance->scale_nWeights_ == 8)
  {
    weight_scale_xyDown_  = gInstance->scale_weights_[0]; // muR=0.5 muF=0.5
    weight_scale_yDown_   = gInstance->scale_weights_[1]; // muR=0.5 muF=1.0
    weight_scale_xDown_   = gInstance->scale_weights_[3]; // muR=1.0 muF=0.5
    weight_scale_nominal_ = 1.;                           // muR=1.0 muF=1.0
    weight_scale_xUp_     = gInstance->scale_weights_[4]; // muR=1.0 muF=2.0
    weight_scale_yUp_     = gInstance->scale_weights_[6]; // muR=2.0 muF=1.0
    weight_scale_xyUp_    = gInstance->scale_weights_[7]; // muR=2.0 muF=2.0
  }
  else
  {
    weight_scale_xyDown_  = 1.; // muR=0.5 muF=0.5
    weight_scale_yDown_   = 1.; // muR=0.5 muF=1.0
    weight_scale_xDown_   = 1.; // muR=1.0 muF=0.5
    weight_scale_nominal_ = 1.; // muR=1.0 muF=1.0
    weight_scale_xUp_     = 1.; // muR=1.0 muF=2.0
    weight_scale_yUp_     = 1.; // muR=2.0 muF=1.0
    weight_scale_xyUp_    = 1.; // muR=2.0 muF=2.0
    weight_scale_Up_      = 1.; // envelope
    weight_scale_Down_    = 1.; // envelope
    std::cerr << "Unexpected number of LHE scale weights: " << gInstance->scale_nWeights_ << '\n';
    return;
  }

  if(gInstance->pdf_nWeights_ > max_pdf_nWeights_)
  {
    throw cmsException(this)
      << "Number of PDF weights stored in Ntuple = " << gInstance->pdf_nWeights_
      << ", exceeds max_pdf_nWeights_ = " << max_pdf_nWeights_ << " !!\n";
  }
}

double
LHEInfoReader::getWeight(double weight,
                         bool correct) const
{
  const double correctiveFactor = correct ? correctiveFactor_ : 1.;
  return clip(correctiveFactor * weight / getWeight_scale_nominal());
}

double
LHEInfoReader::getWeight_scale_nominal() const
{
  assert(std::fpclassify(weight_scale_nominal_) != FP_ZERO);
  return weight_scale_nominal_;
}

double
LHEInfoReader::getWeight_scale_xUp() const
{ 
  return getWeight(weight_scale_xUp_);
}

double
LHEInfoReader::getWeight_scale_xDown() const
{ 
  return getWeight(weight_scale_xDown_);
}

double
LHEInfoReader::getWeight_scale_yUp() const
{
  return getWeight(weight_scale_yUp_);
}

double
LHEInfoReader::getWeight_scale_yDown() const
{ 
  return getWeight(weight_scale_yDown_);
}

double
LHEInfoReader::getWeight_scale_xyUp() const
{
  return getWeight(weight_scale_xyUp_);
}

double
LHEInfoReader::getWeight_scale_xyDown() const
{
  return getWeight(weight_scale_xyDown_);
}

double
LHEInfoReader::getWeight_scale_Up() const
{
  // the envelope values already take the corrective factor into account
  return getWeight(weight_scale_Up_, false);
}

double
LHEInfoReader::getWeight_scale_Down() const
{
  // the envelope values already take the corrective factor into account
  return getWeight(weight_scale_Down_, false);
}

double
LHEInfoReader::getWeight_scale(int central_or_shift) const
{
  switch(central_or_shift)
  {
    case kLHE_scale_central: return 1.; // [*]
    case kLHE_scale_xDown:   return getWeight_scale_xDown();
    case kLHE_scale_xUp:     return getWeight_scale_xUp();
    case kLHE_scale_yDown:   return getWeight_scale_yDown();
    case kLHE_scale_yUp:     return getWeight_scale_yUp();
    case kLHE_scale_xyDown:  return getWeight_scale_xyDown();
    case kLHE_scale_xyUp:    return getWeight_scale_xyUp();
    case kLHE_scale_Down:    return getWeight_scale_Down();
    case kLHE_scale_Up:      return getWeight_scale_Up();
    default: throw cmsException(this, __func__, __LINE__)
               << "Invalid LHE scale systematics option: " << central_or_shift;
  }
  // [*] do not return getWeight_scale_nominal() since we return shifts wrt this value, see
  //     https://github.com/HEP-KBFI/tth-htt/issues/174
  //     for more
}

int
LHEInfoReader::getNumWeights_pdf() const
{
  // If the # of PDF error sets is
  // a) 33 -- PDF4LHC15_nnlo_30_pdfas (LHAID = 91400) (http://www.hepforge.org/archive/lhapdf/pdfsets/6.2/PDF4LHC15_nnlo_30_pdfas.tar.gz)
  //          mem=0 => alphas(MZ)=0.118 central value; mem=1-30 => PDF symmetric eigenvectors; mem=31 => alphas(MZ)=0.1165 central value; mem=32 => alphas(MZ)=0.1195
  // b) 103
  //    i) NNPDF31_nnlo_hessian_pdfas (LHAID = 306000) (http://www.hepforge.org/archive/lhapdf/pdfsets/6.2/NNPDF31_nnlo_hessian_pdfas.tar.gz)
  //       mem=0 central value => Alphas(MZ)=0.118; mem=1-100 => PDF eig.; mem=101 => central value Alphas(MZ)=0.116; mem=102 => central value Alphas(MZ)=0.120
  //
  //    ii) NNPDF30_nlo_nf_4_pdfas (LHAID = 292000) for some FXFX 80X samples (http://www.hepforge.org/archive/lhapdf/pdfsets/6.2/NNPDF30_nlo_nf_4_pdfas.tar.gz)
  //        mem=0 to mem=100 with alphas(MZ)=0.118, mem=0 => average on replicas 1-100; mem=1-100 => PDF replicas with  alphas(MZ)=0.118;
  //        mem=101 => central value for alphas=0.117; mem=102 => central value for alphas=0.119; maximum number of active flavors NF=4
  //
  //    iii) NNPDF30_nlo_nf_5_pdfas (LHAID = 292200) for some FXFX 80X samples (http://www.hepforge.org/archive/lhapdf/pdfsets/6.2/NNPDF30_nlo_nf_5_pdfas.tar.gz)
  //         mem=0 to mem=100 with alphas(MZ)=0.118, mem=0 => average on replicas 1-100; mem=1-100 => PDF replicas with  alphas(MZ)=0.118;
  //         mem=101 => central value for alphas=0.117; mem=102 => central value for alphas=0.119
  // c) 101
  //    i) NNPDF30_nlo_as_0118 (LHAID = 260000) for some 92X samples (http://www.hepforge.org/archive/lhapdf/pdfsets/6.2/NNPDF30_nlo_as_0118.tar.gz)
  //       alphas(MZ)=0.118. mem=0 => average on replicas; mem=1-100 => PDF replicas

  //    ii) NNPDF30_lo_as_0130 (LHAID = 262000) for some MLM 80X samples (http://www.hepforge.org/archive/lhapdf/pdfsets/6.2/NNPDF30_lo_as_0130.tar.gz)
  //        alphas(MZ)=0.130. mem=0 => average on replicas; mem=1-100 => PDF replicas
  //
  // In order to find out which PDF error set the sample has, open the Ntuple, read the LHEPDFweight array branch and look for LHEID in the title of the branch.
  return has_LHE_weights_ ? pdf_nWeights_ : 1;
}

double
LHEInfoReader::getWeight_pdf(unsigned int idx) const
{
  if(! has_LHE_weights_)
  {
    return 1.;
  }
  if(idx >= pdf_nWeights_)
  {
    throw cmsException(this)
      << "Given index = " << idx << ", exceeds number of PDF weights stored in Ntuple = "
      << pdf_nWeights_ << " !!\n";
  }
  return clip(correctiveFactor_ * pdf_weights_[idx]);
}
