#include "TallinnNtupleProducer/Readers/interface/LHEInfoReader.h"

#include "TallinnNtupleProducer/CommonTools/interface/cmsException.h"         // cmsException
#include "TallinnNtupleProducer/CommonTools/interface/jetDefinitions.h"       // Btag
#include "TallinnNtupleProducer/CommonTools/interface/sysUncertOptions.h"     // kLHE_scale_*, PDFSys
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
  , has_pdf_weights_(false)
  , nof_pdf_members_(0)
  , nof_alphaS_members_(0)
  , pdf_is_replicas_(false)
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
  else
  {
    const LHEInfoReader * const gInstance = instances_[branchName_scale_weights_];
    if(has_LHE_weights_)
    {

      if(branchName_scale_nWeights_ != gInstance->branchName_scale_nWeights_ ||
         branchName_scale_weights_ != gInstance->branchName_scale_weights_)
      {
        throw cmsException(this)
          << "Association between configuration parameters 'branchName_scale_weights'"
             " must be unique: present association 'branchName_scale_weights' = " << branchName_scale_weights_
          << " does not match previous association 'branchName_scale_weights' = " << gInstance->branchName_scale_weights_;
      }
    }
    if(has_pdf_weights_)
    {
      if(branchName_pdf_nWeights_   != gInstance->branchName_pdf_nWeights_ ||
         branchName_pdf_weights_    != gInstance->branchName_pdf_weights_   )
      {
        throw cmsException(this)
          << "Association between configuration parameters branchName_pdf_weights'"
             " must be unique: present association 'branchName_pdf_weights' = " << branchName_pdf_weights_
          << " does not match previous association 'branchName_pdf_weights' = " << gInstance->branchName_pdf_weights_;
      }
    }
  }
  ++numInstances_[branchName_scale_weights_];
}

std::vector<std::string>
LHEInfoReader::setBranchAddresses(TTree * tree)
{
  if(instances_[branchName_scale_weights_] == this)
  {
    BranchAddressInitializer bai(tree);
    if(has_LHE_weights_)
    {
      bai.setBranchAddress(scale_nWeights_, branchName_scale_nWeights_);
      bai.setLenVar(max_scale_nWeights_).setBranchAddress(scale_weights_, branchName_scale_weights_);
      bai.setBranchAddress(weight_scale_Up_, branchName_envelope_weight_up_, 1.);
      bai.setBranchAddress(weight_scale_Down_, branchName_envelope_weight_down_, 1.);
    }
    if(has_pdf_weights_)
    {
      bai.setBranchAddress(pdf_nWeights_, branchName_pdf_nWeights_);
      bai.setLenVar(max_pdf_nWeights_).setBranchAddress(pdf_weights_, branchName_pdf_weights_);
    }
    return bai.getBoundBranchNames();
  }
  return {};
}

void
LHEInfoReader::read() const
{
  const LHEInfoReader * const gInstance = instances_[branchName_scale_weights_];
  assert(gInstance);
  if(! has_LHE_weights_ && ! has_pdf_weights_)
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

  if(gInstance->has_pdf_weights_ && gInstance->pdfNorms_.size() != gInstance->pdf_nWeights_)
  {
    throw cmsException(this)
      << "Number of PDF weights stored in Ntuple = " << gInstance->pdf_nWeights_
      << " does not correspond to the expected number of PDF weights = " << gInstance->pdfNorms_.size();
  }
}

void
LHEInfoReader::set_pdfNorm(const edm::ParameterSet & cfg)
{
  pdfNorms_ = cfg.getParameter<std::vector<double>>("norm");
  if(pdfNorms_.empty())
  {
    return;
  }
  has_pdf_weights_ = true;
  const int pdf_lhaid = cfg.getParameter<int>("lhaid");
  switch(pdf_lhaid)
  {
    case 91400:  nof_pdf_members_ = 33;                           nof_alphaS_members_ = 2; break; // https://lhapdfsets.web.cern.ch/current/PDF4LHC15_nnlo_30_pdfas/PDF4LHC15_nnlo_30_pdfas.info
    case 306000: nof_pdf_members_ = 103;                          nof_alphaS_members_ = 2; break; // https://lhapdfsets.web.cern.ch/current/NNPDF31_nnlo_hessian_pdfas/NNPDF31_nnlo_hessian_pdfas.info
    case 260000: nof_pdf_members_ = 101; pdf_is_replicas_ = true;                          break; // https://lhapdfsets.web.cern.ch/current/NNPDF30_nlo_as_0118/NNPDF30_nlo_as_0118.info
    case 262000: nof_pdf_members_ = 101; pdf_is_replicas_ = true;                          break; // https://lhapdfsets.web.cern.ch/current/NNPDF30_lo_as_0118/NNPDF30_lo_as_0118.info
    case 292000: nof_pdf_members_ = 103; pdf_is_replicas_ = true; nof_alphaS_members_ = 2; break; // https://lhapdfsets.web.cern.ch/current/NNPDF30_nlo_nf_4_pdfas/NNPDF30_nlo_nf_4_pdfas.info
    case 292200: nof_pdf_members_ = 103; pdf_is_replicas_ = true; nof_alphaS_members_ = 2; break; // https://lhapdfsets.web.cern.ch/current/NNPDF30_nlo_nf_5_pdfas/NNPDF30_nlo_nf_5_pdfas.info
    default: throw cmsException(this, __func__, __LINE__) << "Unknown LHA ID: " << pdf_lhaid;
  }
  assert(nof_pdf_members_ <= max_pdf_nWeights_);
  if((pdfNorms_.size() != nof_pdf_members_) &&
     (pdfNorms_.size() != (nof_pdf_members_ - 1)))
  {
    throw cmsException(this, __func__, __LINE__)
      << "Expected " << nof_pdf_members_ << " from PDF set " << pdf_lhaid
      << " but got " << pdfNorms_.size() << " normalization factors instead"
    ;
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
  return has_pdf_weights_ ? pdf_nWeights_ : 0;
}

int
LHEInfoReader::getPdfSize() const
{
  return pdfNorms_.size();
}

double
LHEInfoReader::getWeight_pdf(unsigned int idx) const
{
  if(! has_pdf_weights_)
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

double
LHEInfoReader::getWeightNorm_pdf(unsigned int idx) const
{
  return pdfNorms_.at(idx) * getWeight_pdf(idx);
}

double
LHEInfoReader::getWeight_pdf(PDFSys option) const
{
  switch(option)
  {
    case PDFSys::central: return 1.;
    case PDFSys::up:      return 1 + comp_pdf_unc();
    case PDFSys::down:    return 1 - comp_pdf_unc();
    default: assert(0); // should never happen
  }
  assert(0); // should never happen
}

namespace
{
  double
  square(double x)
  {
    return x*x;
  }
}

double
LHEInfoReader::comp_pdf_unc() const
{
  assert(has_pdf_weights_);
  double pdf_unc_stat = 0.;
  const int first_member = pdfNorms_.size() == nof_pdf_members_ ? 1 : 0; // skip the 1st member if possible
  const int last_member = getPdfSize() - nof_alphaS_members_;
  const double nominal_weight = pdfNorms_.size() == nof_pdf_members_ ? getWeight_pdf(0) : 1.;

  // Estimate the hessian/replica component of the uncertainty, where we exclude:
  // 1) the nominal PDF member that represents the average over hessian/replica members
  // 2) the members that correspond to different values of alpha_S
  //
  // The dispersion formulas are given in https://arxiv.org/pdf/1510.03865.pdf
  // In particular:
  // - hessian uncertainty by equation (20)
  // - replica uncertainty by equation (21)
  // - alpha_S uncertainty by equation (27)
  // The normalization coefficients in each of the above case are different:
  // - 1 for hessian
  // - 1 / sqrt(Nrep - 1) for replicas (given Nrep replicas)
  // - 1 / Nmem(alphaS)^2 for alpha_S uncertainty (given Nmem(alphaS) variations)
  // Note that Nmem(alphaS) is squared because the equationnn (27) is in linear form.
  for(int member_idx = first_member; member_idx < last_member; ++member_idx)
  {
    pdf_unc_stat += square(getWeightNorm_pdf(member_idx) - nominal_weight);
  }
  double pdf_unc_alphaS = 0.;
  for(int member_idx = last_member; member_idx < getPdfSize(); ++member_idx)
  {
    pdf_unc_alphaS += square(getWeightNorm_pdf(member_idx) - nominal_weight);
  }
  // Subtracting 2 because nof_pdf_members_ also includes the average PDF set
  const int denom = pdf_is_replicas_ ? nof_pdf_members_ - nof_alphaS_members_ - 2 : 1;
  assert(denom > 0);
  double pdf_unc2 = pdf_unc_stat / denom;
  if(nof_alphaS_members_ > 0)
  {
    pdf_unc2 += pdf_unc_alphaS / square(nof_alphaS_members_);
  }
  return std::sqrt(pdf_unc2);
}
