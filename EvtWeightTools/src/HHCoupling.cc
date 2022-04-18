#include "TallinnNtupleProducer/EvtWeightTools/interface/HHCoupling.h"

#include <iostream> // std::ostream

HHCoupling::HHCoupling(double kl,
                       double kt,
                       double c2,
                       double cg,
                       double c2g,
                       const std::string & name,
                       const std::string & training)
  : kl_(kl)
  , kt_(kt)
  , c2_(c2)
  , cg_(cg)
  , c2g_(c2g)
  , name_(name)
  , training_(training)
{}

HHCoupling::HHCoupling(const std::vector<double> & couplings,
                       const std::string & name,
                       const std::string & training)
  : HHCoupling(couplings.at(0), couplings.at(1), couplings.at(2), couplings.at(3), couplings.at(4), name, training)
{}

HHCoupling::HHCoupling(const std::map<std::string, double> & couplings,
                       const std::string & name,
                       const std::string & training)
  : HHCoupling(couplings.at("kl"), couplings.at("kt"), couplings.at("c2"), couplings.at("cg"), couplings.at("c2g"), name, training)
{}

double
HHCoupling::kl() const
{
  return kl_;
}

double
HHCoupling::kt() const
{
  return kt_;
}

double
HHCoupling::c2() const
{
  return c2_;
}

double
HHCoupling::cg() const
{
  return cg_;
}

double
HHCoupling::c2g() const
{
  return c2g_;
}

std::string
HHCoupling::name() const
{
  return name_;
}

std::string
HHCoupling::weightName() const
{
  return "Weight_" + name_;
}

std::string
HHCoupling::training() const
{
  return training_;
}

std::ostream &
operator<<(std::ostream & os,
            const HHCoupling & coupling)
{
  os <<
    "name = "     << coupling.name_     << ": "
    "kl = "       << coupling.kl_       << ", "
    "kt = "       << coupling.kt_       << ", "
    "c2 = "       << coupling.c2_       << ", "
    "cg = "       << coupling.cg_       << ", "
    "c2g = "      << coupling.c2g_      << "; "
    "training = " << coupling.training_
  ;
  return os;
}
