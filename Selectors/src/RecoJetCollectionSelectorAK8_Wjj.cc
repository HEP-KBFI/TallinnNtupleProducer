#include "TallinnNtupleProducer/Selectors/interface/RecoJetCollectionSelectorAK8_Wjj.h"

#include "DataFormats/Math/interface/deltaR.h"                          // deltaR()

#include "TallinnNtupleProducer/CommonTools/interface/cmsException.h"   // get_human_line()
#include "TallinnNtupleProducer/CommonTools/interface/Era.h"            // Era
#include "TallinnNtupleProducer/Objects/interface/RecoLepton.h"         // RecoLepton

#include <TString.h>                                                    // Form()

RecoJetSelectorAK8_Wjj::RecoJetSelectorAK8_Wjj(Era era, int index, bool debug)
  : min_pt_(100.)
  , max_absEta_(2.4)
  , min_msoftdrop_(-1.e+3)
  , max_msoftdrop_(+1.e+3)
  , max_tau2_div_tau1_(0.75)
  , max_dR_lepton_(1.2)
  , min_subJet1_pt_(20.)
  , max_subJet1_absEta_(2.4)
  , min_subJet2_pt_(20.)
  , max_subJet2_absEta_(2.4)
  , apply_dR_lepton_cut_(true)
  , debug_(debug)
{
  switch(era)
  {
    case Era::kUndefined: throw cmsException(this) << "Undefined era!";
    case Era::k2016: min_jetId_ = 1; break; // 1 means loose
    case Era::k2018:
    case Era::k2017: min_jetId_ = 2; break; // 2 means tight (loose jet ID deprecated since 94x)
    default: throw cmsException(this) << "Implement me!";
  }
}

void
RecoJetSelectorAK8_Wjj::set_min_pt(double min_pt)
{
  min_pt_ = min_pt;
}

void
RecoJetSelectorAK8_Wjj::set_max_absEta(double max_absEta)
{
  max_absEta_ = max_absEta;
}

void
RecoJetSelectorAK8_Wjj::set_min_msoftdrop(double min_msoftdrop)
{
  min_msoftdrop_ = min_msoftdrop;
}

void
RecoJetSelectorAK8_Wjj::set_max_msoftdrop(double max_msoftdrop)
{
  max_msoftdrop_ = max_msoftdrop;
}

void
RecoJetSelectorAK8_Wjj::set_max_tau2_div_tau1(double max_tau2_div_tau1)
{
  max_tau2_div_tau1_ = max_tau2_div_tau1;
}

void
RecoJetSelectorAK8_Wjj::set_max_dR_lepton(double max_dR_lepton)
{
  max_dR_lepton_ = max_dR_lepton;
}

void
RecoJetSelectorAK8_Wjj::set_min_subJet1_pt(double min_pt)
{
  min_subJet1_pt_ = min_pt;
}

void
RecoJetSelectorAK8_Wjj::set_max_subJet1_absEta(double max_absEta)
{
  max_subJet1_absEta_ = max_absEta;
}

void
RecoJetSelectorAK8_Wjj::set_min_subJet2_pt(double min_pt)
{
  min_subJet2_pt_ = min_pt;
}

void
RecoJetSelectorAK8_Wjj::set_max_subJet2_absEta(double max_absEta)
{
  max_subJet2_absEta_ = max_absEta;
}

void 
RecoJetSelectorAK8_Wjj::set_min_jetId(int min_jetId)
{
  min_jetId_ = min_jetId;
}

void
RecoJetSelectorAK8_Wjj::enable_dR_lepton_cut()
{
  apply_dR_lepton_cut_ = true;
}

void
RecoJetSelectorAK8_Wjj::disable_dR_lepton_cut()
{
  apply_dR_lepton_cut_ = false;
}

void
RecoJetSelectorAK8_Wjj::set_lepton(const RecoLepton * lepton) const
{
  if ( lepton->isLeptonSF3() )
    leptons_ = { lepton } ;
  else leptons_ = { };
}

void
RecoJetSelectorAK8_Wjj::set_leptons(const std::vector<const RecoLepton *> & leptons) const
{
  for( const auto lepton : leptons)
    if ( lepton->isLeptonSF3() ) leptons_.push_back(lepton);
}

double
RecoJetSelectorAK8_Wjj::get_min_pt() const
{
  return min_pt_;
}

double
RecoJetSelectorAK8_Wjj::get_max_absEta() const
{
  return max_absEta_;
}

double
RecoJetSelectorAK8_Wjj::get_min_msoftdrop() const
{
  return min_msoftdrop_;
}

double
RecoJetSelectorAK8_Wjj::get_max_msoftdrop() const
{
  return max_msoftdrop_;
}

double
RecoJetSelectorAK8_Wjj::get_max_tau2_div_tau1() const
{
  return max_tau2_div_tau1_;
}

double
RecoJetSelectorAK8_Wjj::get_max_dR_lepton() const
{
  return max_dR_lepton_;
}

double
RecoJetSelectorAK8_Wjj::get_min_subJet1_pt() const
{
  return min_subJet1_pt_;
}

double
RecoJetSelectorAK8_Wjj::get_max_subJet1_absEta() const
{
  return max_subJet1_absEta_;
}

double
RecoJetSelectorAK8_Wjj::get_min_subJet2_pt() const
{
  return min_subJet2_pt_;
}

double
RecoJetSelectorAK8_Wjj::get_max_subJet2_absEta() const
{
  return max_subJet2_absEta_;
}

int 
RecoJetSelectorAK8_Wjj::get_min_jetId() const
{
  return min_jetId_;
}

bool
RecoJetSelectorAK8_Wjj::operator()(const RecoJetAK8 & jet, std::string & returnType) const
{
  returnType = "";
  const RecoLepton * lepton = nullptr;
  double min_dR_lepton = 1.e+3;
  for(const RecoLepton * lepton_: leptons_)
  {
    const double dR_lepton = deltaR(jet.p4(), lepton_->p4());
    if(dR_lepton < min_dR_lepton)
    {
      min_dR_lepton = dR_lepton;
      lepton = lepton_;
    }
  }
  if(! lepton)
  {
    throw cmsException(this, __func__, __LINE__)
      << "Value of 'lepton' has not been set. Did you call the 'set_leptons' function prior to calling operator() ?"
    ;
  }
  else if(debug_)
  {
    std::cout << "Closest lepton to the AK8 jet " << jet << " is " << *lepton << '\n';
  }

  if(jet.pt() < min_pt_)
  {
    if( debug_)
    {
      std::cout << "FAILS pT = " << jet.pt() << " >= " << min_pt_ << " cut\n";
    }
    returnType = Form("FAILS pT >= %g cut", min_pt_);
    return false;
  }
  if(jet.absEta() > max_absEta_)
  {
    if(debug_)
    {
      std::cout << "FAILS abs(eta) = " << jet.absEta() << " <= " << max_absEta_ << " cut\n";
    }
    returnType = Form("FAILS abs(eta) <= %g cut", max_absEta_);
    return false;
  }
  if(jet.jetId() < min_jetId_)
  {
    if(debug_)
    {
      std::cout << "FAILS jet ID = " << jet.jetId() << " >= " << min_jetId_ << " cut\n";
    }
    returnType = Form("FAILS jet ID mod 4 >= %i cut", min_jetId_);
    return false;
  }
  if(jet.msoftdrop() < min_msoftdrop_)
  {
    if(debug_)
    {
      std::cout << "FAILS m(softdrop) = " << jet.msoftdrop() << " >= " << min_msoftdrop_ << " cut\n";
    }
    returnType = Form("FAILS m(softdrop) >= %g cut", min_msoftdrop_);
    return false;
  }
  if(jet.msoftdrop() > max_msoftdrop_)
  {
    if(debug_)
    {
      std::cout << "FAILS m(softdrop) = " << jet.msoftdrop() << " <= " << max_msoftdrop_ << " cut\n";
    }
    returnType = Form("FAILS m(softdrop) <= %g cut", max_msoftdrop_);
    return false;
  }
  const double tau2_div_tau1 = jet.tau2() / jet.tau1();
  if(tau2_div_tau1 > max_tau2_div_tau1_)
  {
    if(debug_)
    {
      std::cout << "FAILS N-subjettiness ratio tau2/tau1 = " << tau2_div_tau1 << " <= " << max_tau2_div_tau1_ << " cut\n";
    }
    returnType = Form("FAILS N-subjettiness ratio tau2/tau1 <= %g cut",max_tau2_div_tau1_);
    return false;
  }
  if(min_dR_lepton > max_dR_lepton_)
  {
    if(debug_)
    {
      std::cout << "FAILS dR(lepton) = " << min_dR_lepton << " <= " << max_dR_lepton_ << " cut\n";
    }
    returnType = Form("FAILS dR(lepton) <= %g cut", max_dR_lepton_);
    return false;
  }
  if(!(jet.subJet1() && jet.subJet2()                    &&
       // CV: make sure that lepton is not contained in either subjet
       // (neccessary, as we do not yet reconstruct AK8 jets on nanoAOD level, which are cleaned with respect to leptons)
       // SN: useDeltaRCut_bet_subjet_lep=False when reading AK8-LeptonSubtracted branch
       ( (apply_dR_lepton_cut_ && deltaR(jet.subJet1()->p4(), lepton->p4()) > 0.1) || !apply_dR_lepton_cut_ ) &&
       ( (apply_dR_lepton_cut_ && deltaR(jet.subJet2()->p4(), lepton->p4()) > 0.1) || !apply_dR_lepton_cut_ ) && 
       (
         (
           jet.subJet1()->pt()      >= min_subJet1_pt_     &&
           jet.subJet1()->absEta()  <= max_subJet1_absEta_ &&
           jet.subJet2()->pt()      >= min_subJet2_pt_     &&
           jet.subJet2()->absEta()  <= max_subJet2_absEta_
         ) ||
         (
           jet.subJet1()->pt()      >= min_subJet2_pt_     &&
           jet.subJet1()->absEta()  <= max_subJet2_absEta_ &&
           jet.subJet2()->pt()      >= min_subJet1_pt_     &&
           jet.subJet2()->absEta()  <= max_subJet1_absEta_
         )
       )
     ))
  {
    if(debug_)
    {
      std::cout << "FAILS subjet selection criteria\njet: " << jet;
      returnType = "FAILS subjet selection criteria";

      if(! jet.subJet1())
      {
        returnType += "  first subjet missing";
      }
      else if(! jet.subJet2())
      {
        returnType += "  second subjet missing";
      }
      else
      {
        if(! (deltaR(jet.subJet1()->p4(), lepton->p4()) > 0.1  &&
              deltaR(jet.subJet2()->p4(), lepton->p4()) > 0.1 ))
        {
          returnType += "  dR(subjet, lep) < 0.1";
        }
        if(! ((jet.subJet1()->pt()      >= min_subJet1_pt_    &&
               jet.subJet2()->pt()      >= min_subJet2_pt_     ) ||
              (jet.subJet1()->pt()      >= min_subJet2_pt_    &&
               jet.subJet2()->pt()      >= min_subJet1_pt_     ) ))
        {
          returnType += "  pT < trsh";
        }
        if(! ((jet.subJet1()->absEta()  <= max_subJet1_absEta_ &&
               jet.subJet2()->absEta()  <= max_subJet2_absEta_  ) ||
              (jet.subJet1()->absEta()  <= max_subJet2_absEta_ &&
               jet.subJet2()->absEta()  <= max_subJet1_absEta_  ) ))
        {
          returnType += "  |eta| > trsh";
        }
      }
    }
    return false;
  }

  return true;
}

bool
RecoJetSelectorAK8_Wjj::operator()(const RecoJetAK8 & jet) const
{
  std::string returnType = "";
  return this->operator()(jet, returnType);
}
