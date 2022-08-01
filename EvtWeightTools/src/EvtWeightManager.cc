#include "TallinnNtupleProducer/EvtWeightTools/interface/EvtWeightManager.h"

#include "TallinnNtupleProducer/CommonTools/interface/BranchAddressInitializer.h" // BranchAddressInitializer
#include "TallinnNtupleProducer/CommonTools/interface/cmsException.h"             // cmsException()
#include "TallinnNtupleProducer/EvtWeightTools/interface/lutAuxFunctions.h"       // openFile(), loadTH1(), loadTH2()

#include <TFile.h>                                                                // TFile
#include <TH1.h>                                                                  // TH1
#include <TH2.h>                                                                  // TH2
#include <TTree.h>                                                                // TTree

#include <assert.h>                                                               // assert()
#include <iostream>                                                               // std::cout

EvtWeightManager::EvtWeightManager(const edm::ParameterSet & cfg, bool isDebug)
  : ReaderBase(cfg)
  , histogram_file_(nullptr)
  , isDebug_(isDebug)
  , central_or_shift_("central")
{
  const std::string histogramFile = cfg.getParameter<std::string>("histogramFile");
  const std::string branchNameXaxis = cfg.getParameter<std::string>("branchNameXaxis");
  const std::string branchNameYaxis = cfg.getParameter<std::string>("branchNameYaxis");
  const std::string branchTypeXaxis = cfg.getParameter<std::string>("branchTypeXaxis");
  const std::string branchTypeYaxis = cfg.getParameter<std::string>("branchTypeYaxis");
  const edm::VParameterSet histograms = cfg.getParameter<edm::VParameterSet>("histograms");

  assert(! histogramFile.empty() && ! branchNameXaxis.empty() && ! branchTypeXaxis.empty());
  histogram_file_ = openFile(edm::FileInPath(histogramFile));

  binnedHistogram_varName_x_ = branchNameXaxis;
  binnedHistogram_varType_x_ = branchTypeXaxis;
  if(! branchNameXaxis.empty() && ! branchNameYaxis.empty())
  {
    binnedHistogram_varName_y_ = branchNameYaxis;
    binnedHistogram_varType_y_ = branchTypeYaxis;
    for(const edm::ParameterSet & histogram_cfg: histograms)
    {
      const std::string central_or_shift = histogram_cfg.getParameter<std::string>("central_or_shift");
      const std::string histogramName = histogram_cfg.getParameter<std::string>("histogramName");
      assert(! binnedHistogram_2var_.count(central_or_shift));
      binnedHistogram_2var_[central_or_shift] = loadTH2(histogram_file_, histogramName);
      std::cout << "Loaded histogram '" << histogramName << "' from file " << histogramFile << '\n';
    }
  }
  else
  {
    for(const edm::ParameterSet & histogram_cfg: histograms)
    {
      const std::string central_or_shift = histogram_cfg.getParameter<std::string>("central_or_shift");
      const std::string histogramName = histogram_cfg.getParameter<std::string>("histogramName");
      assert(! binnedHistogram_1var_.count(central_or_shift));
      binnedHistogram_1var_[central_or_shift] = loadTH1(histogram_file_, histogramName);
      std::cout << "Loaded histogram '" << histogramName << "' from file " << histogramFile << '\n';
    }
  }
  assert(binnedHistogram_1var_.empty() != binnedHistogram_2var_.empty());
}

EvtWeightManager::~EvtWeightManager()
{
  if(histogram_file_)
  {
    histogram_file_->Close();
  }
  delete histogram_file_;
}

std::vector<std::string>
EvtWeightManager::setBranchAddresses(TTree * tree)
{
  BranchAddressInitializer bai(tree);
  if(binnedHistogram_varType_x_ == "UChar_t")
  {
    bai.setBranchAddress(var_x_uchar_, binnedHistogram_varName_x_);
  }
  else if(binnedHistogram_varType_x_ == "Float_t")
  {
    bai.setBranchAddress(var_x_float_, binnedHistogram_varName_x_);
  }

  if(! binnedHistogram_2var_.empty())
  {
    if(binnedHistogram_varType_y_ == "UChar_t")
    {
      bai.setBranchAddress(var_y_uchar_, binnedHistogram_varName_y_);
    }
    else if(binnedHistogram_varType_y_ == "Float_t")
    {
      bai.setBranchAddress(var_y_float_, binnedHistogram_varName_y_);
    }
  }
  return bai.getBoundBranchNames_read();
}

int
EvtWeightManager::getBinIdx_1D() const
{
  return getBinIdx_1D(central_or_shift_);
}

int
EvtWeightManager::getBinIdx_1D(const std::string & central_or_shift) const
{
  Double_t val_x = 0.;
  if(binnedHistogram_varType_x_ == "UChar_t")
  {
    val_x = static_cast<Double_t>(var_x_uchar_);
  }
  else if(binnedHistogram_varType_x_ == "Float_t")
  {
    val_x = static_cast<Double_t>(var_x_float_);
  }
  else
  {
    throw cmsException(this, __func__, __LINE__)
      << "Unknown type for the x-axis quantity: " << binnedHistogram_varType_x_
    ;
  }
  const TAxis * const xAxis = binnedHistogram_1var_.at(central_or_shift)->GetXaxis();
  assert(xAxis);
  const Int_t bin_x = xAxis->FindBin(val_x);
  assert(bin_x >= 0);

  if(isDebug_)
  {
    std::cout << get_human_line(this, __func__, __LINE__)
              << ": (" << central_or_shift << ") bin_x("
              << xAxis->GetBinLowEdge(bin_x) << " <= " << val_x << " < "
              << xAxis->GetBinUpEdge(bin_x) << "; "
              << binnedHistogram_varName_x_ << ") = " << bin_x << '\n'
    ;
  }

  return bin_x;
}

std::pair<int, int>
EvtWeightManager::getBinIdx_2D() const
{
  return getBinIdx_2D(central_or_shift_);
}

std::pair<int, int>
EvtWeightManager::getBinIdx_2D(const std::string & central_or_shift) const
{
  Double_t val_x = 0.;
  if(binnedHistogram_varType_x_ == "UChar_t")
  {
    val_x = static_cast<Double_t>(var_x_uchar_);
  }
  else if(binnedHistogram_varType_x_ == "Float_t")
  {
    val_x = static_cast<Double_t>(var_x_float_);
  }
  else
  {
    throw cmsException(this, __func__, __LINE__)
      << "Unknown type for the x-axis quantity: " << binnedHistogram_varType_x_
    ;
  }
  const TAxis * const xAxis = binnedHistogram_2var_.at(central_or_shift)->GetXaxis();
  assert(xAxis);
  const Int_t bin_x = xAxis->FindBin(val_x);
  assert(bin_x >= 0);

  Double_t val_y = 0.;
  if(binnedHistogram_varType_y_ == "UChar_t")
  {
    val_y = static_cast<Double_t>(var_y_uchar_);
  }
  else if(binnedHistogram_varType_y_ == "Float_t")
  {
    val_y = static_cast<Double_t>(var_y_float_);
  }
  else
  {
    throw cmsException(this, __func__, __LINE__)
      << "Unknown type for the x-axis quantity: " << binnedHistogram_varType_y_
    ;
  }
  const TAxis * const yAxis = binnedHistogram_2var_.at(central_or_shift)->GetYaxis();
  assert(yAxis);
  const Int_t bin_y = yAxis->FindBin(val_y);
  assert(bin_y >= 0);

  if(isDebug_)
  {
    std::cout << get_human_line(this, __func__, __LINE__)
              << ": (" << central_or_shift << ") binIdx("
              << xAxis->GetBinLowEdge(bin_x) << " <= " << val_x << " < "
              << xAxis->GetBinUpEdge(bin_x) << " x "
              << yAxis->GetBinLowEdge(bin_y) << " <= " << val_y << " < "
              << yAxis->GetBinUpEdge(bin_y) << "; "
              << binnedHistogram_varName_x_ << " x " << binnedHistogram_varName_y_
              << ") = " << bin_x << ", " << bin_y << '\n'
    ;
  }

  return std::make_pair(bin_x, bin_y);
}

void
EvtWeightManager::set_central_or_shift(const std::string & central_or_shift)
{
  central_or_shift_ = central_or_shift;
}

bool
EvtWeightManager::has_central_or_shift(const std::string & central_or_shift) const
{
  return binnedHistogram_1var_.count(central_or_shift) || binnedHistogram_2var_.count(central_or_shift);
}

std::string
EvtWeightManager::get_x_var() const
{
  return binnedHistogram_varName_x_;
}

std::string
EvtWeightManager::get_y_var() const
{
  return binnedHistogram_varName_y_;
}

double
EvtWeightManager::getWeight() const
{
  return getWeight(central_or_shift_);
}

double
EvtWeightManager::getWeight(const std::string & central_or_shift) const
{
  double weight = 0.;
  if(binnedHistogram_1var_.count(central_or_shift))
  {
    const int bin_x = getBinIdx_1D();
    weight = binnedHistogram_1var_.at(central_or_shift)->GetBinContent(bin_x);

    if(isDebug_)
    {
      std::cout << get_human_line(this, __func__, __LINE__)
                << ": weight (" << central_or_shift << ") = " << weight << '\n'
      ;
    }
  }
  else if(binnedHistogram_2var_.count(central_or_shift))
  {
    const std::pair<int, int> bin_xy = getBinIdx_2D();

    weight = binnedHistogram_2var_.at(central_or_shift)->GetBinContent(bin_xy.first, bin_xy.second);

    if(isDebug_)
    {
      std::cout << get_human_line(this, __func__, __LINE__)
                << ": weight (" << central_or_shift << ") = " << weight << '\n'
      ;
    }
  }
  else
  {
    throw cmsException(this, __func__, __LINE__)
      << "None of the histograms are initialized for " << central_or_shift
    ;
  }
  if(std::fpclassify(weight) == FP_ZERO)
  {
    // can only happen if the stitching weight is not computed for a phase space region that has no overlap
    // with the phase space of other samples
    if(isDebug_)
    {
      std::cout << get_human_line(this, __func__, __LINE__) << ": weight is 0 -> setting it to 1 instead";
    }
    weight = 1.;
  }

  return weight;
}

int
EvtWeightManager::getDimension() const
{
  if(! binnedHistogram_1var_.empty())
  {
    return 1;
  }
  else if(! binnedHistogram_2var_.empty())
  {
    return 2;
  }
  return 0;
}

TH1 *
EvtWeightManager::getHistogram_1D() const
{
  return getHistogram_1D(central_or_shift_);
}

TH1 *
EvtWeightManager::getHistogram_1D(const std::string & central_or_shift) const
{
  if(! binnedHistogram_1var_.count(central_or_shift))
  {
    throw cmsException(this, __func__, __LINE__) << "No 1D histogram available for systematics: " << central_or_shift;
  }
  return binnedHistogram_1var_.at(central_or_shift);
}

TH2 *
EvtWeightManager::getHistogram_2D() const
{
  return getHistogram_2D(central_or_shift_);
}

TH2 *
EvtWeightManager::getHistogram_2D(const std::string & central_or_shift) const
{
  if(! binnedHistogram_2var_.count(central_or_shift))
  {
    throw cmsException(this, __func__, __LINE__) << "No 2D histogram available for systematics: " << central_or_shift;
  }
  return binnedHistogram_2var_.at(central_or_shift);
}
