#include "TallinnNtupleProducer/EvtWeightTools/interface/HHWeightInterfaceLO.h"

#include "TallinnNtupleProducer/CommonTools/interface/cmsException.h"                  // cmsException()
#include "TallinnNtupleProducer/CommonTools/interface/get_fullpath.h"                  // get_fullpath()
#include "TallinnNtupleProducer/EvtWeightTools/interface/HHWeightInterfaceCouplings.h" // HHWeightInterfaceCouplings

#include <TH2.h>                                                                       // TH2

#include <fstream>                                                                     // std::ifstream
#include <iostream>                                                                    // std::cerr, std::fixed
#include <streambuf>                                                                   // std::istreambuf_iterator

HHWeightInterfaceLO::HHWeightInterfaceLO(const HHWeightInterfaceCouplings * const couplings,
                                         const edm::ParameterSet & cfg)
  : couplings_(couplings)
  , modeldata_(nullptr)
  , moduleMainString_(nullptr)
  , moduleMain_(nullptr)
  , func_Weight_(nullptr)
  , nof_sumEvt_entries_(0)
  , sumEvt_(nullptr)
{
  assert(couplings_);

  // AC: limit number of threads running in python to one
  setenv("OMP_NUM_THREADS", "1", 0);

  const std::string coefFile = cfg.getParameter<std::string>("coefFile");

  // read the python file that we're about to execute
  const std::string applicationLoadPath = get_fullpath("hhAnalysis/multilepton/python/do_weight.py");
  std::ifstream applicationLoadFile(applicationLoadPath);
  std::string applicationLoadStr;

  applicationLoadFile.seekg(0, std::ios::end);
  applicationLoadStr.reserve(applicationLoadFile.tellg());
  applicationLoadFile.seekg(0, std::ios::beg);
  applicationLoadStr.assign(std::istreambuf_iterator<char>(applicationLoadFile), std::istreambuf_iterator<char>());

  // https://ubuntuforums.org/archive/index.php/t-324544.html
  // https://stackoverflow.com/questions/4060221/how-to-reliably-open-a-file-in-the-same-directory-as-a-python-script
  // https://gist.github.com/rjzak/5681680
  Py_SetProgramName((wchar_t*)("do_weight"));
  moduleMainString_ = PyUnicode_FromString("__main__");
  Py_Initialize();
  moduleMain_ = PyImport_Import(moduleMainString_);
  PyRun_SimpleString(applicationLoadStr.c_str());

  // General: Load the class with the functions to calculate the different parts of the weights
  PyObject * func_load = PyObject_GetAttrString(moduleMain_, "load");
  const std::string coefFilePath = get_fullpath(coefFile);
  PyObject * coef_path = PyUnicode_FromString(coefFilePath.c_str());
  PyObject * args_load = PyTuple_Pack(1, coef_path);
  modeldata_ = PyObject_CallObject(func_load, args_load);

  // function to calculate and return parts of the weights
  func_Weight_ = PyObject_GetAttrString(moduleMain_, "evaluate_weight");

  // function to compute normalization constant
  func_norm_ = PyObject_GetAttrString(moduleMain_, "get_norm");

  sumEvt_ = HHWeightInterfaceCouplings::loadDenominatorHist(
    couplings_->denominator_file_lo(), couplings_->histtitle()
  );
  nof_sumEvt_entries_ = static_cast<int>(sumEvt_->Integral());
  assert(nof_sumEvt_entries_ > 0);

  Py_XDECREF(coef_path);
  Py_XDECREF(args_load);
  Py_XDECREF(func_load);

  const std::map<std::string, HHCoupling> couplingArray = couplings_->getCouplings();
  norm_.clear();
  for(const auto & kv: couplingArray)
  {
    assert(kv.first == kv.second.name());
    norm_[kv.first] = getNorm(&kv.second);
  }
}

HHWeightInterfaceLO::~HHWeightInterfaceLO()
{
  Py_XDECREF(modeldata_);
  Py_XDECREF(moduleMainString_);
  Py_XDECREF(moduleMain_);
  Py_XDECREF(func_Weight_);
  Py_XDECREF(func_norm_);
  delete sumEvt_;
}

double
HHWeightInterfaceLO::getDenom(double mHH, double cosThetaStar) const
{
  return HHWeightInterfaceCouplings::getBinContent(sumEvt_, mHH, cosThetaStar);
}

double
HHWeightInterfaceLO::getWeight(const std::string & bmName,
                               double mHH,
                               double cosThetaStar,
                               bool isDEBUG) const
{
  const HHCoupling coupling = couplings_->getCoupling(bmName);
  const double denominator = getDenom(mHH, cosThetaStar);

  PyObject* kl_py = PyFloat_FromDouble(static_cast<double>(coupling.kl()));
  PyObject* kt_py = PyFloat_FromDouble(static_cast<double>(coupling.kt()));
  PyObject* c2_py = PyFloat_FromDouble(static_cast<double>(coupling.c2()));
  PyObject* cg_py = PyFloat_FromDouble(static_cast<double>(coupling.cg()));
  PyObject* c2g_py = PyFloat_FromDouble(static_cast<double>(coupling.c2g()));
  PyObject* mHH_py = PyFloat_FromDouble(static_cast<double>(mHH));
  PyObject* cosThetaStar_py = PyFloat_FromDouble(static_cast<double>(cosThetaStar));
  PyObject* norm_py = PyFloat_FromDouble(static_cast<double>(norm_.at(bmName)));
  PyObject* denominator_py = PyFloat_FromDouble(static_cast<double>(denominator));
  PyObject* args_BM_list = PyTuple_Pack(10,
    kl_py,
    kt_py,
    c2_py,
    cg_py,
    c2g_py,
    mHH_py,
    cosThetaStar_py,
    norm_py,
    denominator_py,
    modeldata_
  );
  PyObject* weight_ptr = PyObject_CallObject(func_Weight_, args_BM_list);
  const double weight = PyFloat_AsDouble(weight_ptr) * nof_sumEvt_entries_;

  Py_XDECREF(kl_py);
  Py_XDECREF(kt_py);
  Py_XDECREF(c2_py);
  Py_XDECREF(cg_py);
  Py_XDECREF(c2g_py);
  Py_XDECREF(mHH_py);
  Py_XDECREF(cosThetaStar_py);
  Py_XDECREF(norm_py);
  Py_XDECREF(denominator_py);
  Py_XDECREF(args_BM_list);
  Py_XDECREF(weight_ptr);

  if(isDEBUG)
  {
    std::cout << "denominator = " << denominator << "\nbmName = " << bmName << ") = " << weight << '\n';
  }
  return weight;
}

double
HHWeightInterfaceLO::getRelativeWeight(const std::string & bmName,
                                       double mHH,
                                       double cosThetaStar,
                                       bool isDEBUG) const
{
  double reWeight = 1.;
  if ( bmName == "SM" )
  {
    reWeight = 1.;
  }
  else
  {
    double smWeight = getWeight("SM", mHH, cosThetaStar, isDEBUG);
    double bmWeight = getWeight(bmName, mHH, cosThetaStar, isDEBUG);
    reWeight = ( smWeight > 0. ) ? bmWeight/smWeight : 1.;
  }
  return reWeight;
}

std::map<std::string, double>
HHWeightInterfaceLO::getNorm() const
{
  return norm_;
}

double
HHWeightInterfaceLO::getNorm(const HHCoupling * const coupling) const
{
  PyObject* kl_py = PyFloat_FromDouble(static_cast<double>(coupling->kl()));
  PyObject* kt_py = PyFloat_FromDouble(static_cast<double>(coupling->kt()));
  PyObject* c2_py = PyFloat_FromDouble(static_cast<double>(coupling->c2()));
  PyObject* cg_py = PyFloat_FromDouble(static_cast<double>(coupling->cg()));
  PyObject* c2g_py = PyFloat_FromDouble(static_cast<double>(coupling->c2g()));
  PyObject* denom_py = PyUnicode_FromString(get_fullpath(couplings_->denominator_file_lo()).data());
  PyObject* hist_py = PyUnicode_FromString(couplings_->histtitle().data());

  PyObject* args_norm_list = PyTuple_Pack(8,
    kl_py,
    kt_py,
    c2_py,
    cg_py,
    c2g_py,
    denom_py,
    hist_py,
    modeldata_
  );
  PyObject* norm_ptr = PyObject_CallObject(func_norm_, args_norm_list);
  const double norm = PyFloat_AsDouble(norm_ptr);

  Py_XDECREF(kl_py);
  Py_XDECREF(kt_py);
  Py_XDECREF(c2_py);
  Py_XDECREF(cg_py);
  Py_XDECREF(c2g_py);
  Py_XDECREF(denom_py);
  Py_XDECREF(hist_py);
  Py_XDECREF(args_norm_list);
  Py_XDECREF(norm_ptr);

  return norm;
}
