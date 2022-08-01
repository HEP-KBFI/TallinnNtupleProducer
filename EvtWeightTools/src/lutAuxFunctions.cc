#include "TallinnNtupleProducer/EvtWeightTools/interface/lutAuxFunctions.h"

#include "TallinnNtupleProducer/CommonTools/interface/cmsException.h"                 // cmsException()
#include "TallinnNtupleProducer/EvtWeightTools/interface/hadTauTriggerTurnOnCurves.h" // integralCrystalBall()

#include <TAxis.h>                                                                    // TAxis
#include <TF1.h>                                                                      // TF1
#include <TFile.h>                                                                    // TFile
#include <TGraph.h>                                                                   // TGraph
#include <TH1.h>                                                                      // TH1
#include <TH2.h>                                                                      // TH2

#include <assert.h>                                                                   // assert()

using namespace lut;

const double epsilon = 1.e-3;

namespace
{
  template <typename T,
            typename = typename std::enable_if<std::is_arithmetic<T>::value>>
  /**
   * @brief Constrains value between the lower and the upper bound (both included)
   * @param value      The value to be constrained
   * @param lowerBound The lower bound
   * @param upperBound The upper bound
   * @return Value between the lower and the upper bound (lowerBound <= value <= upperBound)
   */
  T
  constrainValue(T value,
                 T lowerBound,
                 T upperBound)
  {
    assert(lowerBound <= upperBound);
    value = std::max(value, lowerBound);
    value = std::min(value, upperBound);
    return value;
  }
}

/**
 * @brief Open ROOT file the name of which is given as function argument
 * @param fileName: name of ROOT file
 * @return pointer to TFile object
 */
TFile *
openFile(const edm::FileInPath & fileName)
{
  if(fileName.fullPath().empty())
  {
    throw cmsException(__func__, __LINE__)
      << " Failed to find file = '" << fileName << "' !!\n";
  }
  TFile * inputFile = new TFile(fileName.fullPath().data());
  return inputFile;
}

/**
 * @brief Load one-dimensional histogram (TH1) from ROOT file 
 * @param fileName:      name of ROOT file
 * @param histogramName: name of the histogram
 * @return pointer to TH1 object
 */
TH1 *
loadTH1(TFile * inputFile,
        const std::string & histogramName)
{
  TH1* histogram = dynamic_cast<TH1 *>(inputFile->Get(histogramName.data()));
  if (! histogram)
  {
    throw cmsException(__func__, __LINE__)
      << " Failed to load TH1 = " << histogramName.data() << " from file = " << inputFile->GetName();
  }
  return histogram;
}

/**
 * @brief Load two-dimensional histogram (TH2) from ROOT file 
 * @param fileName:      name of ROOT file
 * @param histogramName: name of the histogram
 * @return pointer to TH2 object
 */
TH2 *
loadTH2(TFile * inputFile,
        const std::string & histogramName)
{
  TH2 * histogram = dynamic_cast<TH2 *>(inputFile->Get(histogramName.data()));
  if(! histogram)
  {
    throw cmsException(__func__, __LINE__)
      << " Failed to load TH2 = " << histogramName.data() << " from file = " << inputFile->GetName();
  }
  return histogram;
}

/**
 * @brief Load graph (TGraph) from ROOT file 
 * @param fileName:  name of ROOT file;
 * @param graphName: name of the graph
 * @return pointer to TGraph object
 */
TGraph *
loadTGraph(TFile * inputFile,
           const std::string & graphName)
{
  TGraph * graph = dynamic_cast<TGraph *>(inputFile->Get(graphName.data()));
  if(! graph)
  {
    throw cmsException(__func__, __LINE__)
      << " Failed to load TGraph = " << graphName.data() << " from file = " << inputFile->GetName();
  }
  return graph;
}

/**
 * @brief Load function (TF1) from ROOT file 
 * @param fileName:     name of ROOT file
 * @param functionName: name of the function
 * @return pointer to TF1 object
 */
TF1 *
loadTF1(TFile * inputFile,
        const std::string & functionName)
{
  TF1 * function = dynamic_cast<TF1 *>(inputFile->Get(functionName.data()));
  if (! function)
  {
    throw cms::Exception("loadTF1") 
      << " Failed to load TF1 = " << functionName.data() << " from file = " << inputFile->GetName();
  }
  return function;
}

/**
 * @brief Retrieve data/MC scale-factor for given pT or eta value from one-dimensional histogram
 * @param lut:         pointer to histogram storing data/MC scale-factors as function of pT or eta
 * @param x:           transverse momentum respectively pseudo-rapidity of lepton
 * @param error_shift: number of sigmas to shift the sf
 * @return data/MC scale-factor
 */
double
getSF_from_TH1(TH1 * lut,
               double x,
               int error_shift)
{
  const TAxis * const xAxis = lut->GetXaxis();
  const int idxBin_x = constrainValue(xAxis->FindBin(x), 1, xAxis->GetNbins());
  const double sf = lut->GetBinContent(idxBin_x);
  const double sf_err = lut->GetBinError(idxBin_x);
  return sf + error_shift * sf_err;
}

/**
 * @brief Retrieve data/MC scale-factor for given pT and eta value from two-dimensional histogram
 * @param lut:         pointer to histogram storing data/MC scale-factors as function of pT and eta
 * @param x, y:        transverse momentum respectively pseudo-rapidity of lepton
 * @param error_shift: number of sigmas to shift the sf
 * @return data/MC scale-factor
 */
double
getSF_from_TH2(TH2 * lut,
               double x,
               double y,
               int error_shift)
{
  const TAxis * const xAxis = lut->GetXaxis();
  const TAxis * const yAxis = lut->GetYaxis();
  const int idxBin_x = constrainValue(xAxis->FindBin(x), 1, xAxis->GetNbins());
  const int idxBin_y = constrainValue(yAxis->FindBin(y), 1, yAxis->GetNbins());
  const double sf = lut->GetBinContent(idxBin_x, idxBin_y);
  const double sf_err = lut->GetBinError(idxBin_x, idxBin_y);
  return sf + error_shift * sf_err;
}

double
getSF_from_TH2Poly(TH2 * lut,
                   double x,
                   double y,
                   int error_shift)
{
  const TAxis * const xAxis = lut->GetXaxis();
  const TAxis * const yAxis = lut->GetYaxis();
  x = constrainValue(x, xAxis->GetXmin() + epsilon, xAxis->GetXmax() - epsilon);
  y = constrainValue(y, yAxis->GetXmin() + epsilon, yAxis->GetXmax() - epsilon);
  const int idxBin = lut->FindBin(x, y);
  const double sf = lut->GetBinContent(idxBin);
  const double sf_err = lut->GetBinError(idxBin);
  return sf + error_shift * sf_err;
}

/**
 * @brief Retrieve data/MC scale-factor for given pT or eta value from a graph
 * @param lut: pointer to graph storing data/MC scale-factors as function of pT or eta
 * @param x:   transverse momentum respectively pseudo-rapidity of lepton
 * @return data/MC scale-factor
 */
double
getSF_from_TGraph(TGraph * lut,
                  double x,
                  int __attribute__((unused)) error_shift)
{
  const TAxis * xAxis = lut->GetXaxis();
  x = constrainValue(x, xAxis->GetXmin(), xAxis->GetXmax());
  return lut->Eval(x);
}

//-------------------------------------------------------------------------------
lutWrapperBase::lutWrapperBase()
  : inputFile_(nullptr)
  , lutTypeX_(kUndefined)
  , lutTypeY_(kUndefined)
  , xMin_(-1.)
  , xMax_(-1.)
  , yMin_(-1.)
  , yMax_(-1.)
{}

lutWrapperBase::lutWrapperBase(const std::string & lutName,
                               int lutType,
                               double xMin,
                               double xMax,
                               int xAction,
                               double yMin,
                               double yMax,
                               int yAction)
  : inputFile_(nullptr)
  , lutName_(lutName)
  , xMin_(xMin)
  , xMax_(xMax)
  , xAction_(xAction)
  , yMin_(yMin)
  , yMax_(yMax)
  , yAction_(yAction)
{
  initialize(lutType);
}

lutWrapperBase::lutWrapperBase(std::map<std::string, TFile *> & inputFiles,
                               const std::string & inputFileName,
                               const std::string & lutName,
                               int lutType,
                               double xMin,
                               double xMax,
                               int xAction,
                               double yMin,
                               double yMax,
                               int yAction)
  : inputFileName_(inputFileName)
  , inputFile_(nullptr)
  , lutName_(lutName)
  , xMin_(xMin)
  , xMax_(xMax)
  , xAction_(xAction)
  , yMin_(yMin)
  , yMax_(yMax)
  , yAction_(yAction)
{
  if(inputFiles.count(inputFileName))
  {
    inputFile_ = inputFiles[inputFileName];
  }
  else
  {
    inputFile_ = openFile(edm::FileInPath(inputFileName_));
    inputFiles[inputFileName] = inputFile_;
  }
  initialize(lutType);
}

void
lutWrapperBase::initialize(int lutType)
{
  if     (lutType == kXpt        || lutType == kXptYpt     || lutType == kXptYeta    || lutType == kXptYabsEta) lutTypeX_ = kPt;
  else if(lutType == kXeta       || lutType == kXetaYpt                                                      ) lutTypeX_ = kEta;
  else if(lutType == kXabsEta    || lutType == kXabsEtaYpt                                                   ) lutTypeX_ = kAbsEta;
  else assert(0);
  if(xMin_ != -1. && xMax_ != -1.)
  {
    assert(xMax_ > xMin_);
  }

  if     (lutType == kXpt        || lutType == kXeta       || lutType == kXabsEta                            ) lutTypeY_ = kUndefined;
  else if(lutType == kXptYpt     || lutType == kXetaYpt    || lutType == kXabsEtaYpt                         ) lutTypeY_ = kPt;
  else if(lutType == kXptYeta                                                                                ) lutTypeY_ = kEta;
  else if(lutType == kXptYabsEta                                                                             ) lutTypeY_ = kAbsEta;
  else assert(0);

  if(lutTypeY_ == kUndefined)
  {
    yMin_ = -1.;
    yMax_ = -1.;
  }
  if(yMin_ != -1. && yMax_ != -1.)
  {
    assert(yMax_ > yMin_);
  }
}

double
lutWrapperBase::getSF(double pt,
                      double eta,
                      int error_shift)
{
  double x = 0.;
  switch(lutTypeX_)
  {
    case kPt:     x = pt;             break;
    case kEta:    x = eta;            break;
    case kAbsEta: x = std::fabs(eta); break;
    default:                          break;
  }
  if(xAction_ == kCut || xAction_ == kLimit_and_Cut)
  {
    if(xMin_ != -1. && x < xMin_) return 1.;
    if(xMax_ != -1. && x > xMax_) return 1.;
  }
  double y = 0.;
  switch(lutTypeY_)
  {
    case kPt:     y = pt;             break;
    case kEta:    y = eta;            break;
    case kAbsEta: y = std::fabs(eta); break;
    default:                          break;
  }
  if(yAction_ == kCut || yAction_ == kLimit_and_Cut)
  {
    if(yMin_ != -1. && y < yMin_) return 1.;
    if(yMax_ != -1. && y > yMax_) return 1.;
  }
  return getSF_private(x, y, error_shift);
}

const std::string &
lutWrapperBase::inputFileName() const
{
  return inputFileName_;
}

const std::string &
lutWrapperBase::lutName() const
{
  return lutName_;
}

double
get_from_lut_err(const vLutWrapperBase & corrections,
                 double pt,
                 double eta,
                 int error_shift,
                 bool isDEBUG)
{
  if(isDEBUG)
  {
    std::cout << "<get_from_lut_err>:\n";
  }
  double sf = 1.;
  for (lutWrapperBase * correction: corrections)
  {
    sf *= correction->getSF(pt, eta, error_shift);
    if(isDEBUG)
    {
      std::cout << "pT = "  << pt  << ", "
                   "eta = " << eta << "\n"
                   "LUT: "
                   "shift by = "      << error_shift                              << ", "
                   "inputFileName = " << correction->inputFileName()              << ", "
                   "lutName = "       << correction->lutName()                    << " --> "
                   "SF = "            << correction->getSF(pt, eta, error_shift)  << '\n';
    }
  }
  return sf;
}

double
get_from_lut(const vLutWrapperBase & corrections,
             double pt,
             double eta,
             bool isDEBUG)
{
  return get_from_lut_err(corrections, pt, eta, 0, isDEBUG);
}

double
get_from_lut(const std::map<int, vLutWrapperBase> & corrections,
             double hadTau_pt,
             double hadTau_eta,
             int hadTau_decayMode,
             bool isDEBUG)
{
  double sf = 1.;
  const std::map<int, vLutWrapperBase>::const_iterator correction = corrections.find(hadTau_decayMode);
  if(correction != corrections.end())
  {
    sf = get_from_lut(correction->second, hadTau_pt, hadTau_eta, isDEBUG);
  }
  else
  {
    throw cmsException(__func__, __LINE__)
      << "Invalid parameter 'hadTauDecayMode' = " << hadTau_decayMode
    ;
  }
  return sf;
}
//-------------------------------------------------------------------------------


//-------------------------------------------------------------------------------
lutWrapperTH1::lutWrapperTH1(std::map<std::string, TFile *> & inputFiles,
                             const std::string & inputFileName,
                             const std::string & lutName,
                             int lutType,
                             double xMin,
                             double xMax,
                             int xAction,
                             double yMin,
                             double yMax,
                             int yAction)
  : lutWrapperBase(inputFiles, inputFileName, lutName, lutType, xMin, xMax, xAction, yMin, yMax, yAction)
{
  if(yAction_ == kLimit || yAction_ == kLimit_and_Cut)
  {
    throw cmsException(__func__, __LINE__)
      << " Configuration parameter 'yAction' = " << yAction << " not supported for objects of class lutWrapperTH1"
    ;
  }
  lut_ = loadTH1(inputFile_, lutName_);
}

double
lutWrapperTH1::getSF_private(double x,
                             double y,
                             int error_shift)
{
  double sf = 1.;
  if((y >= yMin_ || yMin_ == -1.) && (y < yMax_ || yMax_ == -1.))
  {
    if(xMin_ != -1. && x < xMin_) x = xMin_;
    if(xMax_ != -1. && x > xMax_) x = xMax_;
    sf = getSF_from_TH1(lut_, x, error_shift);
  }
  return sf;
}
//-------------------------------------------------------------------------------


//-------------------------------------------------------------------------------
lutWrapperTH2::lutWrapperTH2(std::map<std::string, TFile *> & inputFiles,
                             const std::string & inputFileName,
                             const std::string & lutName,
                             int lutType,
                             double xMin,
                             double xMax,
                             int xAction,
                             double yMin,
                             double yMax,
                             int yAction)
  : lutWrapperBase(inputFiles, inputFileName, lutName, lutType, xMin, xMax, xAction, yMin, yMax, yAction)
{
  lut_ = loadTH2(inputFile_, lutName_);
}

double
lutWrapperTH2::getSF_private(double x,
                             double y,
                             int error_shift)
{
  if(xMin_ != -1. && x < xMin_) x = xMin_;
  if(xMax_ != -1. && x > xMax_) x = xMax_;
  if(yMin_ != -1. && y < yMin_) y = yMin_;
  if(yMax_ != -1. && y > yMax_) y = yMax_;
  double sf = getSF_from_TH2(lut_, x, y, error_shift);
  return sf;
}
//-------------------------------------------------------------------------------


//-------------------------------------------------------------------------------
lutWrapperTH2Poly::lutWrapperTH2Poly(std::map<std::string, TFile *> & inputFiles,
                                     const std::string & inputFileName,
                                     const std::string & lutName,
                                     int lutType,
                                     double xMin,
                                     double xMax,
                                     int xAction,
                                     double yMin,
                                     double yMax,
                                     int yAction)
  : lutWrapperBase(inputFiles, inputFileName, lutName, lutType, xMin, xMax, xAction, yMin, yMax, yAction)
{
  lut_ = loadTH2(inputFile_, lutName_);
}

double
lutWrapperTH2Poly::getSF_private(double x,
                                 double y,
                                 int error_shift)
{
  if(xMin_ != -1. && x < xMin_) x = xMin_;
  if(xMax_ != -1. && x > xMax_) x = xMax_;
  if(yMin_ != -1. && y < yMin_) y = yMin_;
  if(yMax_ != -1. && y > yMax_) y = yMax_;
  double sf = getSF_from_TH2Poly(lut_, x, y, error_shift);
  return sf;
}
//-------------------------------------------------------------------------------


//-------------------------------------------------------------------------------
lutWrapperTGraph::lutWrapperTGraph(std::map<std::string, TFile *> & inputFiles,
                                   const std::string & inputFileName,
                                   const std::string & lutName,
                                   int lutType,
                                   double xMin,
                                   double xMax,
                                   int xAction,
                                   double yMin,
                                   double yMax,
                                   int yAction)
  : lutWrapperBase(inputFiles, inputFileName, lutName, lutType, xMin, xMax, xAction, yMin, yMax, yAction)
{
  if(yAction_ == kLimit || yAction_ == kLimit_and_Cut)
  {
    throw cmsException(__func__, __LINE__)
      << " Configuration parameter 'yAction' = " << yAction << " not supported for objects of class lutWrapperTGraph"
    ;
  }
  lut_ = loadTGraph(inputFile_, lutName_);
}

double
lutWrapperTGraph::getSF_private(double x,
                                double y,
                                int error_shift)
{
  double sf = 1.;
  if((y >= yMin_ || yMin_ == -1.) && (y < yMax_ || yMax_ == -1.))
  {
    if(xMin_ != -1. && x < xMin_) x = xMin_;
    if(xMax_ != -1. && x > xMax_) x = xMax_;
    sf = getSF_from_TGraph(lut_, x, error_shift);
  }
  return sf;
}
//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------
lutWrapperCrystalBall::lutWrapperCrystalBall(const std::string & lutName,
                                             const edm::ParameterSet & cfg,
                                             int lutType,
                                             double xMin,
                                             double xMax,
                                             int xAction,
                                             double yMin,
                                             double yMax,
                                             int yAction)
  : lutWrapperBase(lutName, lutType, xMin, xMax, xAction, yMin, yMax, yAction)
  , m0_   (cfg.getParameter<double>("m_{0}"))
  , sigma_(cfg.getParameter<double>("sigma"))
  , alpha_(cfg.getParameter<double>("alpha"))
  , n_    (cfg.getParameter<double>("n"))
  , norm_ (cfg.getParameter<double>("norm"))
{
  if(yAction_ == kLimit || yAction_ == kLimit_and_Cut)
  {
    throw cmsException(__func__, __LINE__)
      << " Configuration parameter 'yAction' = " << yAction << " not supported for objects of class lutWrapperCrystalBall"
    ;
  }
}

double
lutWrapperCrystalBall::getSF_private(double x,
                                     double y,
                                     int __attribute__((unused)) error_shift)
{
  double sf = 1.;
  if((y >= yMin_ || yMin_ == -1.) && (y < yMax_ || yMax_ == -1.))
  {
    if(xMin_ != -1. && x < xMin_) x = xMin_;
    if(xMax_ != -1. && x > xMax_) x = xMax_;

    sf = integralCrystalBall(x, m0_, sigma_, alpha_, n_, norm_);
    sf = constrainValue(sf, 0., 1.);
  }
  return sf;
}
//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------
lutWrapperData_to_MC::lutWrapperData_to_MC(lutWrapperBase * lutData,
                                           lutWrapperBase * lutMC)
  : lutData_(lutData)
  , lutMC_(lutMC)
{}

lutWrapperData_to_MC::~lutWrapperData_to_MC()
{
  if(lutData_)
  {
    delete lutData_;
  }
  if(lutMC_)
  {
    delete lutMC_;
  }
}

double
lutWrapperData_to_MC::getSF_private(double x,
                                    double y,
                                    int error_shift)
{
  const double effData = lutData_->getSF(x, y, error_shift);
  const double effMC   = lutMC_->getSF(x, y, error_shift);
  double sf = effMC > 0. ? effData / effMC : 1.;
  sf = std::min(sf, 1.e+1);
  return sf;
}
//-------------------------------------------------------------------------------
