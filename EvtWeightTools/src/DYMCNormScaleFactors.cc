#include "TallinnNtupleProducer/EvtWeightTools/interface/DYMCNormScaleFactors.h"

#include "TallinnNtupleProducer/CommonTools/interface/cmsException.h"     // cmsException()
#include "TallinnNtupleProducer/CommonTools/interface/Era.h"              // Era
#include "TallinnNtupleProducer/CommonTools/interface/sysUncertOptions.h" // kDYMCNormScaleFactors_central, kDYMCNormScaleFactors_Up, kDYMCNormScaleFactors_Down

DYMCNormScaleFactors::DYMCNormScaleFactors(Era era,
                                           bool debug)
  : era_(era)
  , debug_(debug)
{}

DYMCNormScaleFactors::~DYMCNormScaleFactors()
{}

double
DYMCNormScaleFactors::getWeight(const std::vector<GenParticle> & genTauLeptons,
                                int nJets,
                                int nBLoose,
                                int nBMedium,
                                int central_or_shift) const
{
  const GenParticle * genTauLeptonPlus = nullptr;
  const GenParticle * genTauLeptonMinus = nullptr;
  for(const GenParticle & genTauLepton: genTauLeptons)
  {
    if(genTauLepton.charge() > 0 && (! genTauLeptonPlus || genTauLepton.pt() > genTauLeptonPlus->pt()))
    {
      genTauLeptonPlus = &genTauLepton;
    }
    if(genTauLepton.charge() < 0 && (! genTauLeptonMinus || genTauLepton.pt() > genTauLeptonMinus->pt()))
    {
      genTauLeptonMinus = &genTauLepton;
    }
  }

  double weight = 1.;
  double weight_error = 0.;
  if(genTauLeptonPlus && genTauLeptonMinus && genTauLeptons.size() == 2)
  {
    const Particle::LorentzVector dileptonP4 = genTauLeptonPlus->p4() + genTauLeptonMinus->p4();
    const double dileptonMass = dileptonP4.mass();

    if(era_ == Era::k2016)
    {
      if     (nBMedium >= 2 && nJets >= 4)                 { weight = 0.974; weight_error = 0.096; }
      else if(nBMedium >= 2 && nJets == 3)                 { weight = 0.757; weight_error = 0.101; }
      else if(nBMedium >= 2 && nJets == 2)                 { weight = 0.728; weight_error = 0.073; }
      else if(nBMedium == 1 && nBLoose >= 2 && nJets >= 4) { weight = 1.117; weight_error = 0.030; }
      else if(nBMedium == 1 && nBLoose >= 2 && nJets == 3) { weight = 0.896; weight_error = 0.015; }
      else if(nBMedium == 1 && nBLoose >= 2 && nJets == 2) { weight = 0.788; weight_error = 0.032; }
      else if(nBMedium == 1 && nBLoose == 1 && nJets >= 4) { weight = 1.165; weight_error = 0.050; }
      else if(nBMedium == 1 && nBLoose == 1 && nJets == 3) { weight = 0.980; weight_error = 0.018; }
      else if(nBMedium == 1 && nBLoose == 1 && nJets == 2) { weight = 0.892; weight_error = 0.004; }
      else if(nBMedium == 0 && nBLoose >= 2 && nJets >= 4) { weight = 1.094; weight_error = 0.023; }
      else if(nBMedium == 0 && nBLoose >= 2 && nJets == 3) { weight = 0.878; weight_error = 0.033; }
      else if(nBMedium == 0 && nBLoose >= 2 && nJets == 2) { weight = 0.797; weight_error = 0.010; }
    }
    else if(era_ == Era::k2017)
    {
      if     (nBMedium >= 2 && nJets >= 4)                 { weight = 1.871; weight_error = 0.167; }
      else if(nBMedium >= 2 && nJets == 3)                 { weight = 1.165; weight_error = 0.052; }
      else if(nBMedium >= 2 && nJets == 2)                 { weight = 0.883; weight_error = 0.022; }
      else if(nBMedium == 1 && nBLoose >= 2 && nJets >= 4) { weight = 1.598; weight_error = 0.026; }
      else if(nBMedium == 1 && nBLoose >= 2 && nJets == 3) { weight = 1.166; weight_error = 0.040; }
      else if(nBMedium == 1 && nBLoose >= 2 && nJets == 2) { weight = 0.931; weight_error = 0.012; }
      else if(nBMedium == 1 && nBLoose == 1 && nJets >= 4) { weight = 1.682; weight_error = 0.028; }
      else if(nBMedium == 1 && nBLoose == 1 && nJets == 3) { weight = 1.229; weight_error = 0.021; }
      else if(nBMedium == 1 && nBLoose == 1 && nJets == 2) { weight = 0.998; weight_error = 0.009; }
      else if(nBMedium == 0 && nBLoose >= 2 && nJets >= 4) { weight = 1.535; weight_error = 0.019; }
      else if(nBMedium == 0 && nBLoose >= 2 && nJets == 3) { weight = 1.143; weight_error = 0.010; }
      else if(nBMedium == 0 && nBLoose >= 2 && nJets == 2) { weight = 0.996; weight_error = 0.039; }
    }
    else if(era_ == Era::k2018)
    {
      if     (nBMedium >= 2 && nJets >= 4)                 { weight = 1.999; weight_error = 0.237; }
      else if(nBMedium >= 2 && nJets == 3)                 { weight = 1.202; weight_error = 0.031; }
      else if(nBMedium >= 2 && nJets == 2)                 { weight = 0.915; weight_error = 0.017; }
      else if(nBMedium == 1 && nBLoose >= 2 && nJets >= 4) { weight = 1.763; weight_error = 0.031; }
      else if(nBMedium == 1 && nBLoose >= 2 && nJets == 3) { weight = 1.116; weight_error = 0.054; }
      else if(nBMedium == 1 && nBLoose >= 2 && nJets == 2) { weight = 0.910; weight_error = 0.021; }
      else if(nBMedium == 1 && nBLoose == 1 && nJets >= 4) { weight = 1.860; weight_error = 0.038; }
      else if(nBMedium == 1 && nBLoose == 1 && nJets == 3) { weight = 1.231; weight_error = 0.048; }
      else if(nBMedium == 1 && nBLoose == 1 && nJets == 2) { weight = 0.995; weight_error = 0.004; }
      else if(nBMedium == 0 && nBLoose >= 2 && nJets >= 4) { weight = 1.704; weight_error = 0.065; }
      else if(nBMedium == 0 && nBLoose >= 2 && nJets == 3) { weight = 1.136; weight_error = 0.017; }
      else if(nBMedium == 0 && nBLoose >= 2 && nJets == 2) { weight = 0.946; weight_error = 0.005; }
    }
    else
    {
      throw cmsException(this, __func__, __LINE__) << "Invalid era: " << static_cast<int>(era_);
    }

    if(debug_)
    {
      std::cout << get_human_line(this, __func__, __LINE__)
                << ": dilepton mass = " << dileptonMass
                << " nBMedium = "       << nBMedium
                << " nBLoose = "        << nBLoose
                << " nJets = "          << nJets
                << " --> weight = "     << weight
                << " +/- "              << weight_error
                << '\n'
      ;
    }
  }

  switch(central_or_shift)
  {
    case kDYMCNormScaleFactors_central:                           break;
    case kDYMCNormScaleFactors_shiftUp:   weight += weight_error; break; // cover difference of SFs from Z->ee and Z->mumu
    case kDYMCNormScaleFactors_shiftDown: weight -= weight_error; break; // cover difference of SFs from Z->ee and Z->mumu
    default: assert(0);
  }
  assert(weight > 0.);
  return weight;
}
