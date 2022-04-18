#include "TallinnNtupleProducer/EvtWeightTools/interface/HadTauFakeRateWeightEntry.h"

#include "TallinnNtupleProducer/CommonTools/interface/cmsException.h"                  // cmsException()
#include "TallinnNtupleProducer/CommonTools/interface/sysUncertOptions.h"              // kFRjt_*
#include "TallinnNtupleProducer/EvtWeightTools/interface/hadTauFakeRateAuxFunctions.h" // getEtaBin()

#include "TF1.h"                                                                       // TF1
#include "TFile.h"                                                                     // TFile
#include "TGraphAsymmErrors.h"                                                         // TGraphAsymmErrors
#include "TString.h"                                                                   // TString

#include <boost/algorithm/string/replace.hpp>                                          // boost::replace_all_copy()

namespace
{
  std::string
  getEtaHadTauSelectionName(const std::string & pattern,
                            const std::string & etaBin,
                            const std::string & hadTauSelection)
  {
    return boost::replace_all_copy(
      boost::replace_all_copy(pattern,
         "$etaBin",          etaBin
      ), "$hadTauSelection", hadTauSelection
    );
  }

  TGraphAsymmErrors *
  loadGraph(TFile * inputFile,
            const std::string & graphName,
            const std::string & etaBin,
            const std::string & hadTauSelection)
  {
    const std::string graphName_etaBin = getEtaHadTauSelectionName(graphName, etaBin, hadTauSelection);
    const TGraphAsymmErrors * const graph = dynamic_cast<const TGraphAsymmErrors * const>(
      inputFile->Get(graphName_etaBin.data())
    );
    if(! graph)
    {
      throw cmsException(__func__, __LINE__)
        << "Failed to load graph = " << graphName_etaBin << " from file = " << inputFile->GetName();
    }
    TGraphAsymmErrors * graph_cloned = static_cast<TGraphAsymmErrors *>(graph->Clone());
    return graph_cloned;
  }

  TF1 *
  loadFitFunction(TFile * inputFile,
                  const std::string & fitFunctionName,
                  const std::string & etaBin,
                  const std::string & hadTauSelection)
  {
    const std::string fitFunctionName_etaBin = getEtaHadTauSelectionName(fitFunctionName, etaBin, hadTauSelection);
    const TF1 * const fitFunction = dynamic_cast<const TF1 * const>(inputFile->Get(fitFunctionName_etaBin.data()));
    if(! fitFunction)
    {
      throw cmsException(__func__, __LINE__)
        << "Failed to load fitFunction = " << fitFunctionName_etaBin << " from file = " << inputFile->GetName();
    }
    TF1 * fitFunction_cloned = static_cast<TF1 *>(fitFunction->Clone());
    return fitFunction_cloned;
  }
}

HadTauFakeRateWeightEntry::HadTauFakeRateWeightEntry(double absEtaMin,
                                                     double absEtaMax,
                                                     const std::string & hadTauSelection,
                                                     TFile * inputFile,
                                                     const edm::ParameterSet & cfg,
                                                     const std::string& trigMatching,
                                                     int central_or_shift)
  : absEtaMin_(absEtaMin)
  , absEtaMax_(absEtaMax)
  , hadTauSelection_(hadTauSelection)
  , graph_(nullptr)
  , applyGraph_(cfg.getParameter<bool>("applyGraph"))
  , fitFunction_(nullptr)
  , applyFitFunction_(cfg.getParameter<bool>("applyFitFunction"))
{
  const std::string etaBin = getEtaBin(absEtaMin_, absEtaMax_);
  TString graphName = cfg.getParameter<std::string>("graphName").data();
  graphName.ReplaceAll("jetToTauFakeRate/", Form("jetToTauFakeRate_%s/", trigMatching.data()));
  graphName_ = graphName.Data();
  graph_ = loadGraph(inputFile, graphName_, etaBin, hadTauSelection_);
  int idxPoint_first = 0;
  double x_first, y_first;
  graph_->GetPoint(idxPoint_first, x_first, y_first);
  graph_xMin_ = x_first - graph_->GetErrorXlow(idxPoint_first);
  int idxPoint_last = graph_->GetN() - 1;
  double x_last, y_last;
  graph_->GetPoint(idxPoint_last, x_last, y_last);
  graph_xMax_ = x_last + graph_->GetErrorXhigh(idxPoint_last);
  assert(graph_xMax_ > graph_xMin_);

  TString fitFunctionName = cfg.getParameter<std::string>("fitFunctionName").data();
  fitFunctionName.ReplaceAll("jetToTauFakeRate/", Form("jetToTauFakeRate_%s/", trigMatching.data()));
  switch(central_or_shift)
  {
    case kFRjt_central:   fitFunctionName_ = fitFunctionName.Data();                      break;
    case kFRjt_normUp:    fitFunctionName_ = Form("%s_par1Up",   fitFunctionName.Data()); break;
    case kFRjt_normDown:  fitFunctionName_ = Form("%s_par1Down", fitFunctionName.Data()); break;
    case kFRjt_shapeUp:   fitFunctionName_ = Form("%s_par2Up",   fitFunctionName.Data()); break;
    case kFRjt_shapeDown: fitFunctionName_ = Form("%s_par2Down", fitFunctionName.Data()); break;
    default: throw cmsException(this)
               << "Invalid Configuration parameter 'central_or_shift' = " << central_or_shift;
  }
  fitFunction_ = loadFitFunction(inputFile, fitFunctionName_, etaBin, hadTauSelection_);
  fitFunction_xMin_ = fitFunction_->GetXmin();
  fitFunction_xMax_ = fitFunction_->GetXmax();
  assert(fitFunction_xMax_ > fitFunction_xMin_);
}

HadTauFakeRateWeightEntry::~HadTauFakeRateWeightEntry()
{
  delete graph_;
  delete fitFunction_;
}

double
HadTauFakeRateWeightEntry::absEtaMin() const
{
  return absEtaMin_;
}

double
HadTauFakeRateWeightEntry::absEtaMax() const
{
  return absEtaMax_;
}

double
HadTauFakeRateWeightEntry::getWeight(double pt) const
{
  double weight = 1.;
  if(applyGraph_)
  {
    double graph_x = pt;
    if ( graph_x < graph_xMin_ ) graph_x = graph_xMin_;
    if ( graph_x > graph_xMax_ ) graph_x = graph_xMax_;
    weight *= graph_->Eval(graph_x);
  }
  if(applyFitFunction_)
  {
    double fitFunction_x = pt;
    if ( fitFunction_x < fitFunction_xMin_ ) fitFunction_x = fitFunction_xMin_;
    if ( fitFunction_x > fitFunction_xMax_ ) fitFunction_x = fitFunction_xMax_;
    weight *= fitFunction_->Eval(fitFunction_x);
  }
  if ( weight < 0. ) weight = 0.;
  // CV: set upper limit on fake-rates (FR), 
  //     as event weights, which are computed as FR/(1 - FR), grow indefinitely as FR approaches 1
  const double weight_max = 0.9;
  if ( weight > weight_max ) weight = weight_max;
  return weight;
}

double
HadTauFakeRateWeightEntry::getSF(double pt) const
{
  double sf = 1.;
  if(applyFitFunction_)
  {
    sf *= fitFunction_->Eval(pt);
  }
  sf = std::max(sf, 0.);
  return sf;
}
