#include "TallinnNtupleProducer/Writers/plugins/BDTVarWriter_HH_2lss.h"

#include "DataFormats/Math/interface/deltaR.h"                                    // deltaR()

#include "TallinnNtupleProducer/CommonTools/interface/BranchAddressInitializer.h" // BranchAddressInitializer
#include "TallinnNtupleProducer/CommonTools/interface/cmsException.h"             // cmsException()
#include "TallinnNtupleProducer/CommonTools/interface/merge_systematic_shifts.h"  // merge_systematic_shifts()
#include "TallinnNtupleProducer/CommonTools/interface/isHigherPt.h"               // isHigherPt()
#include "TallinnNtupleProducer/Readers/interface/convert_to_ptrs.h"              // convert_to_ptrs()
#include "TallinnNtupleProducer/Readers/interface/RecoElectronReader.h"           // RecoElectronReader::get_supported_systematics()
#include "TallinnNtupleProducer/Readers/interface/RecoJetReaderAK4.h"             // RecoJetReaderAK4::get_supported_systematics()
#include "TallinnNtupleProducer/Readers/interface/RecoJetReaderAK8.h"             // RecoJetReaderAK8::get_supported_systematics()
#include "TallinnNtupleProducer/Readers/interface/RecoMEtReader.h"                // RecoMEtReader::get_supported_systematics()
#include "TallinnNtupleProducer/Readers/interface/RecoMuonReader.h"               // RecoMuonReader::get_supported_systematics()
#include "TallinnNtupleProducer/Writers/interface/bdtVarAuxFunctions.h"           // comp_min_dR_jet(), comp_mT(), comp_sumP4()
#include "TallinnNtupleProducer/Writers/interface/createRecoJetPairs_Wjj.h"       // createRecoJetPairs_Wjj();
#include "TallinnNtupleProducer/Writers/interface/RecoJetPair_Wjj.h"              // RecoJetPair_Wjj

#include "TTree.h"                                                                // TTree

#include <algorithm>                                                              // std::sort
#include <vector>                                                                 // std::vector

BDTVarWriter_HH_2lss::BDTVarWriter_HH_2lss(const edm::ParameterSet & cfg)
  : WriterBase(cfg)
{}

BDTVarWriter_HH_2lss::~BDTVarWriter_HH_2lss()
{}

void
BDTVarWriter_HH_2lss::setBranches(TTree * outputTree)
{
  BranchAddressInitializer bai(outputTree);
  bai.setBranch(m_ll_, "m_ll");
  bai.setBranch(dR_ll_, "dR_ll");
  bai.setBranch(mT_lep1_, "mT_lep1");
  bai.setBranch(mT_lep2_, "mT_lep2");
  bai.setBranch(min_dR_lep1_jet_, "min_dR_lep1_jet");
  bai.setBranch(min_dR_lep2_jet_, "min_dR_lep2_jet");
  bai.setBranch(min_dR_lep_Wjets_, "min_dR_lep_Wjets");
  bai.setBranch(max_dR_lep_Wjets_, "max_dR_lep_Wjets");
  bai.setBranch(min_dR_lep_leadWjet_, "min_dR_lep_leadWjet");
  bai.setBranch(max_dR_lep_leadWjet_, "max_dR_lep_leadWjet");  
  bai.setBranch(dR_Wjj1_, "dR_Wjj1");
  bai.setBranch(m_Wjj1_, "m_Wjj1");
  bai.setBranch(dR_Wjj2_, "dR_Wjj2");
  bai.setBranch(m_Wjj2_, "m_Wjj2");
  bai.setBranch(mHH_vis_, "mHH_vis");
}

void
BDTVarWriter_HH_2lss::resetBranches()
{
  m_ll_ = -1.;
  dR_ll_ = -1.;
  mT_lep1_ = -1.;
  mT_lep2_ = -1.;
  min_dR_lep1_jet_ = 1.e+3;
  min_dR_lep2_jet_ = 1.e+3;
  min_dR_lep_Wjets_ = 1.e+3;
  max_dR_lep_Wjets_ = -1.;
  min_dR_lep_leadWjet_ =  1.e+3;
  max_dR_lep_leadWjet_ = -1.;
  dR_Wjj1_ = -1.;
  m_Wjj1_ = -1.;
  dR_Wjj2_ = -1.;
  m_Wjj2_ = -1.;
  mHH_vis_ = -1.;
}

void
BDTVarWriter_HH_2lss::writeImp(const Event & event, const EvtWeightRecorder & evtWeightRecorder)
{
  this->resetBranches();

  const RecoLeptonPtrCollection & leptons = event.fakeableLeptons();
  if ( leptons.size() >= 2 )
  {
    const RecoLepton * lepton_lead    = leptons.at(0);
    const RecoLepton * lepton_sublead = leptons.at(1);

    m_ll_ = comp_sumP4(std::vector<const RecoLepton *>({ lepton_lead, lepton_sublead })).mass();
    dR_ll_ = deltaR(lepton_lead->p4(), lepton_sublead->p4());

    const RecoMEt & met = event.met();

    mT_lep1_ = comp_mT(lepton_lead, &met);
    mT_lep2_ = comp_mT(lepton_sublead, &met);

    const RecoJetPtrCollectionAK4 & jetsAK4 = event.selJetsAK4();

    min_dR_lep1_jet_ = comp_min_dR_jet(*lepton_lead, jetsAK4);
    min_dR_lep2_jet_ = comp_min_dR_jet(*lepton_sublead, jetsAK4);

    const RecoJetPtrCollectionAK8 & jetsAK8_Wjj = event.selJetsAK8_Wjj();
  
    RecoJetPairCollection_Wjj tmp = createRecoJetPairs_Wjj(jetsAK4, jetsAK8_Wjj, 2);
    RecoJetPairPtrCollection_Wjj jetPairs_Wjj = convert_to_ptrs(tmp);

    min_dR_lep_Wjets_ =  1.e+3;
    max_dR_lep_Wjets_ = -1.;
    for ( auto jetPair_Wjj : jetPairs_Wjj )
    {
      std::vector<const RecoJetBase *> Wjets = jetPair_Wjj->jets();
      for ( auto Wjet : Wjets )
      {
        for ( auto lepton : leptons )
        {
          double dR = deltaR(lepton->p4(), Wjet->p4());
          if ( dR < min_dR_lep_Wjets_ ) min_dR_lep_Wjets_ = dR;
          if ( dR > max_dR_lep_Wjets_ ) max_dR_lep_Wjets_ = dR;        
        }
      }
    }

    std::vector<const RecoJetBase *> Wjets_allPairs;
    for ( auto jetPair_Wjj : jetPairs_Wjj )
    {
      if ( jetPair_Wjj->jet_lead()    ) Wjets_allPairs.push_back(jetPair_Wjj->jet_lead());
      if ( jetPair_Wjj->jet_sublead() ) Wjets_allPairs.push_back(jetPair_Wjj->jet_sublead());
    }
    std::sort(Wjets_allPairs.begin(), Wjets_allPairs.end(), isHigherPt<RecoJetBase>);

    const RecoJetBase * leadWJet = ( Wjets_allPairs.size() >= 1 ) ? Wjets_allPairs.at(0) : nullptr;
    if ( leadWJet )
    {
      min_dR_lep_leadWjet_ =  1.e+3;
      max_dR_lep_leadWjet_ = -1.;
      for ( auto lepton : leptons )
      {
        double dR = deltaR(lepton->p4(), leadWJet->p4());
        if ( dR < min_dR_lep_leadWjet_ ) min_dR_lep_leadWjet_ = dR;
        if ( dR > max_dR_lep_leadWjet_ ) max_dR_lep_leadWjet_ = dR;
      }
    }

    const RecoJetPair_Wjj * jetPair_Wjj_lead    = ( jetPairs_Wjj.size() >= 1 ) ? jetPairs_Wjj.at(0) : nullptr;
    const RecoJetPair_Wjj * jetPair_Wjj_sublead = ( jetPairs_Wjj.size() >= 2 ) ? jetPairs_Wjj.at(1) : nullptr;
    if ( jetPair_Wjj_lead && jetPair_Wjj_lead->jet_lead() && jetPair_Wjj_lead->jet_sublead() )
    {
      dR_Wjj1_ = deltaR(jetPair_Wjj_lead->jet_lead()->p4(), jetPair_Wjj_lead->jet_sublead()->p4());
      m_Wjj1_  = jetPair_Wjj_lead->mass();
    }
    if ( jetPair_Wjj_sublead && jetPair_Wjj_sublead->jet_lead() && jetPair_Wjj_sublead->jet_sublead() )
    {
      dR_Wjj2_ = deltaR(jetPair_Wjj_sublead->jet_lead()->p4(), jetPair_Wjj_sublead->jet_sublead()->p4());
      m_Wjj2_  = jetPair_Wjj_sublead->mass();
    }

    mHH_vis_ = (lepton_lead->p4() + lepton_sublead->p4() + comp_sumP4(jetPairs_Wjj)).mass();
  }
}

std::vector<std::string>
BDTVarWriter_HH_2lss::get_supported_systematics(const edm::ParameterSet & cfg)
{
  std::vector<std::string> supported_systematics;
  merge_systematic_shifts(supported_systematics, RecoElectronReader::get_supported_systematics(cfg));
  merge_systematic_shifts(supported_systematics, RecoElectronReader::get_supported_systematics(cfg));
  merge_systematic_shifts(supported_systematics, RecoJetReaderAK4::get_supported_systematics(cfg));
  merge_systematic_shifts(supported_systematics, RecoJetReaderAK8::get_supported_systematics(cfg));
  merge_systematic_shifts(supported_systematics, RecoMEtReader::get_supported_systematics(cfg));
  return supported_systematics;
}

DEFINE_EDM_PLUGIN(WriterPluginFactory, BDTVarWriter_HH_2lss, "BDTVarWriter_HH_2lss");
