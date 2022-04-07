#include "TallinnNtupleProducer/Readers/interface/metPhiModulation.h"

#include "TallinnNtupleProducer/CommonTools/interface/Era.h"    // Era
#include "TallinnNtupleProducer/Objects/interface/EventInfo.h"  // EventInfo
#include "TallinnNtupleProducer/Objects/interface/RecoVertex.h" // RecoVertex

/**
 * @see https://lathomas.web.cern.ch/lathomas/METStuff/XYCorrections/XYMETCorrection_withUL17andUL18.h
 */

enum class TheRunEra
{
  yUnknown,
  y2016B, y2016C, y2016D, y2016E, y2016F, y2016G, y2016H,
  y2017B, y2017C, y2017D, y2017E, y2017F,
  y2018A, y2018B, y2018C, y2018D,
  y2016MC,
  y2017MC,
  y2018MC,
  yUL2017B, yUL2017C, yUL2017D, yUL2017E, yUL2017F,
  yUL2018A, yUL2018B, yUL2018C, yUL2018D,
  yUL2017MC,
  yUL2018MC
};

std::pair<double, double>
METXYCorr_Met_MetPhi(const EventInfo * const eventInfo,
                     const RecoVertex * const recoVertex,
                     Era year,
                     bool isUL,
                     bool ispuppi)
{
  assert(eventInfo);
  assert(recoVertex);

  const int npv = std::min(recoVertex->npvs(), 100);
  const bool isMC = eventInfo->isMC();
  const unsigned runnb = eventInfo->run;

  TheRunEra runera = TheRunEra::yUnknown;
  bool usemetv2 = false;

  if     (isMC && year == Era::k2016 && ! isUL){ runera = TheRunEra::y2016MC; }
  else if(isMC && year == Era::k2017 && ! isUL){ runera = TheRunEra::y2017MC; usemetv2 = true; }
  else if(isMC && year == Era::k2018 && ! isUL){ runera = TheRunEra::y2018MC; }
  else if(isMC && year == Era::k2017 &&   isUL){ runera = TheRunEra::yUL2017MC; }
  else if(isMC && year == Era::k2018 &&   isUL){ runera = TheRunEra::yUL2018MC; }


  else if(!isMC && runnb >= 272007 && runnb <= 275376 && ! isUL){ runera = TheRunEra::y2016B; }
  else if(!isMC && runnb >= 275657 && runnb <= 276283 && ! isUL){ runera = TheRunEra::y2016C; }
  else if(!isMC && runnb >= 276315 && runnb <= 276811 && ! isUL){ runera = TheRunEra::y2016D; }
  else if(!isMC && runnb >= 276831 && runnb <= 277420 && ! isUL){ runera = TheRunEra::y2016E; }
  else if(!isMC && runnb >= 277772 && runnb <= 278808 && ! isUL){ runera = TheRunEra::y2016F; }
  else if(!isMC && runnb >= 278820 && runnb <= 280385 && ! isUL){ runera = TheRunEra::y2016G; }
  else if(!isMC && runnb >= 280919 && runnb <= 284044 && ! isUL){ runera = TheRunEra::y2016H; }

  else if(!isMC && runnb >= 297020 && runnb <= 299329 && ! isUL){ runera = TheRunEra::y2017B; usemetv2 = true; }
  else if(!isMC && runnb >= 299337 && runnb <= 302029 && ! isUL){ runera = TheRunEra::y2017C; usemetv2 = true; }
  else if(!isMC && runnb >= 302030 && runnb <= 303434 && ! isUL){ runera = TheRunEra::y2017D; usemetv2 = true; }
  else if(!isMC && runnb >= 303435 && runnb <= 304826 && ! isUL){ runera = TheRunEra::y2017E; usemetv2 = true; }
  else if(!isMC && runnb >= 304911 && runnb <= 306462 && ! isUL){ runera = TheRunEra::y2017F; usemetv2 = true; }

  else if(!isMC && runnb >= 315252 && runnb <= 316995 && ! isUL){ runera = TheRunEra::y2018A; }
  else if(!isMC && runnb >= 316998 && runnb <= 319312 && ! isUL){ runera = TheRunEra::y2018B; }
  else if(!isMC && runnb >= 319313 && runnb <= 320393 && ! isUL){ runera = TheRunEra::y2018C; }
  else if(!isMC && runnb >= 320394 && runnb <= 325273 && ! isUL){ runera = TheRunEra::y2018D; }

  else if(!isMC && runnb >= 315252 && runnb <= 316995 && isUL){ runera = TheRunEra::yUL2018A; }
  else if(!isMC && runnb >= 316998 && runnb <= 319312 && isUL){ runera = TheRunEra::yUL2018B; }
  else if(!isMC && runnb >= 319313 && runnb <= 320393 && isUL){ runera = TheRunEra::yUL2018C; }
  else if(!isMC && runnb >= 320394 && runnb <= 325273 && isUL){ runera = TheRunEra::yUL2018D; }

  else if(!isMC && runnb >= 297020 && runnb <= 299329 && isUL){ runera = TheRunEra::yUL2017B; usemetv2 = false; }
  else if(!isMC && runnb >= 299337 && runnb <= 302029 && isUL){ runera = TheRunEra::yUL2017C; usemetv2 = false; }
  else if(!isMC && runnb >= 302030 && runnb <= 303434 && isUL){ runera = TheRunEra::yUL2017D; usemetv2 = false; }
  else if(!isMC && runnb >= 303435 && runnb <= 304826 && isUL){ runera = TheRunEra::yUL2017E; usemetv2 = false; }
  else if(!isMC && runnb >= 304911 && runnb <= 306462 && isUL){ runera = TheRunEra::yUL2017F; usemetv2 = false; }

  else
  {
    //Couldn't find data/MC era => no correction applied
    return { 0., 0. };
  }

  double METxcorr = 0.;
  double METycorr = 0.;

  if(! usemetv2)
  {
    //Current recommendation for 2016 and 2018
    if(! ispuppi)
    {
      if(runera == TheRunEra::y2016B) METxcorr = -(-0.0478335*npv -0.108032);
      if(runera == TheRunEra::y2016B) METycorr = -(0.125148*npv +0.355672);
      if(runera == TheRunEra::y2016C) METxcorr = -(-0.0916985*npv +0.393247);
      if(runera == TheRunEra::y2016C) METycorr = -(0.151445*npv +0.114491);
      if(runera == TheRunEra::y2016D) METxcorr = -(-0.0581169*npv +0.567316);
      if(runera == TheRunEra::y2016D) METycorr = -(0.147549*npv +0.403088);
      if(runera == TheRunEra::y2016E) METxcorr = -(-0.065622*npv +0.536856);
      if(runera == TheRunEra::y2016E) METycorr = -(0.188532*npv +0.495346);
      if(runera == TheRunEra::y2016F) METxcorr = -(-0.0313322*npv +0.39866);
      if(runera == TheRunEra::y2016F) METycorr = -(0.16081*npv +0.960177);
      if(runera == TheRunEra::y2016G) METxcorr = -(0.040803*npv -0.290384);
      if(runera == TheRunEra::y2016G) METycorr = -(0.0961935*npv +0.666096);
      if(runera == TheRunEra::y2016H) METxcorr = -(0.0330868*npv -0.209534);
      if(runera == TheRunEra::y2016H) METycorr = -(0.141513*npv +0.816732);
      if(runera == TheRunEra::y2017B) METxcorr = -(-0.259456*npv +1.95372);
      if(runera == TheRunEra::y2017B) METycorr = -(0.353928*npv -2.46685);
      if(runera == TheRunEra::y2017C) METxcorr = -(-0.232763*npv +1.08318);
      if(runera == TheRunEra::y2017C) METycorr = -(0.257719*npv -1.1745);
      if(runera == TheRunEra::y2017D) METxcorr = -(-0.238067*npv +1.80541);
      if(runera == TheRunEra::y2017D) METycorr = -(0.235989*npv -1.44354);
      if(runera == TheRunEra::y2017E) METxcorr = -(-0.212352*npv +1.851);
      if(runera == TheRunEra::y2017E) METycorr = -(0.157759*npv -0.478139);
      if(runera == TheRunEra::y2017F) METxcorr = -(-0.232733*npv +2.24134);
      if(runera == TheRunEra::y2017F) METycorr = -(0.213341*npv +0.684588);
      if(runera == TheRunEra::y2018A) METxcorr = -(0.362865*npv -1.94505);
      if(runera == TheRunEra::y2018A) METycorr = -(0.0709085*npv -0.307365);
      if(runera == TheRunEra::y2018B) METxcorr = -(0.492083*npv -2.93552);
      if(runera == TheRunEra::y2018B) METycorr = -(0.17874*npv -0.786844);
      if(runera == TheRunEra::y2018C) METxcorr = -(0.521349*npv -1.44544);
      if(runera == TheRunEra::y2018C) METycorr = -(0.118956*npv -1.96434);
      if(runera == TheRunEra::y2018D) METxcorr = -(0.531151*npv -1.37568);
      if(runera == TheRunEra::y2018D) METycorr = -(0.0884639*npv -1.57089);
      if(runera == TheRunEra::y2016MC) METxcorr = -(-0.195191*npv -0.170948);
      if(runera == TheRunEra::y2016MC) METycorr = -(-0.0311891*npv +0.787627);
      if(runera == TheRunEra::y2017MC) METxcorr = -(-0.217714*npv +0.493361);
      if(runera == TheRunEra::y2017MC) METycorr = -(0.177058*npv -0.336648);
      if(runera == TheRunEra::y2018MC) METxcorr = -(0.296713*npv -0.141506);
      if(runera == TheRunEra::y2018MC) METycorr = -(0.115685*npv +0.0128193);

      //UL2017
      if(runera == TheRunEra::yUL2017B) METxcorr = -(-0.211161*npv +0.419333);
      if(runera == TheRunEra::yUL2017B) METycorr = -(0.251789*npv +-1.28089);
      if(runera == TheRunEra::yUL2017C) METxcorr = -(-0.185184*npv +-0.164009);
      if(runera == TheRunEra::yUL2017C) METycorr = -(0.200941*npv +-0.56853);
      if(runera == TheRunEra::yUL2017D) METxcorr = -(-0.201606*npv +0.426502);
      if(runera == TheRunEra::yUL2017D) METycorr = -(0.188208*npv +-0.58313);
      if(runera == TheRunEra::yUL2017E) METxcorr = -(-0.162472*npv +0.176329);
      if(runera == TheRunEra::yUL2017E) METycorr = -(0.138076*npv +-0.250239);
      if(runera == TheRunEra::yUL2017F) METxcorr = -(-0.210639*npv +0.72934);
      if(runera == TheRunEra::yUL2017F) METycorr = -(0.198626*npv +1.028);
      if(runera == TheRunEra::yUL2017MC) METxcorr = -(-0.300155*npv +1.90608);
      if(runera == TheRunEra::yUL2017MC) METycorr = -(0.300213*npv +-2.02232);

      //UL2018
      if(runera == TheRunEra::yUL2018A) METxcorr = -(0.263733*npv +-1.91115);
      if(runera == TheRunEra::yUL2018A) METycorr = -(0.0431304*npv +-0.112043);
      if(runera == TheRunEra::yUL2018B) METxcorr = -(0.400466*npv +-3.05914);
      if(runera == TheRunEra::yUL2018B) METycorr = -(0.146125*npv +-0.533233);
      if(runera == TheRunEra::yUL2018C) METxcorr = -(0.430911*npv +-1.42865);
      if(runera == TheRunEra::yUL2018C) METycorr = -(0.0620083*npv +-1.46021);
      if(runera == TheRunEra::yUL2018D) METxcorr = -(0.457327*npv +-1.56856);
      if(runera == TheRunEra::yUL2018D) METycorr = -(0.0684071*npv +-0.928372);
      if(runera == TheRunEra::yUL2018MC) METxcorr = -(0.183518*npv +0.546754);
      if(runera == TheRunEra::yUL2018MC) METycorr = -(0.192263*npv +-0.42121);

    }
    // UL2017Puppi
    else
    {
      if(runera == TheRunEra::yUL2017B) METxcorr = -(-0.00382117*npv +-0.666228);
      if(runera == TheRunEra::yUL2017B) METycorr = -(0.0109034*npv +0.172188);
      if(runera == TheRunEra::yUL2017C) METxcorr = -(-0.00110699*npv +-0.747643);
      if(runera == TheRunEra::yUL2017C) METycorr = -(-0.0012184*npv +0.303817);
      if(runera == TheRunEra::yUL2017D) METxcorr = -(-0.00141442*npv +-0.721382);
      if(runera == TheRunEra::yUL2017D) METycorr = -(-0.0011873*npv +0.21646);
      if(runera == TheRunEra::yUL2017E) METxcorr = -(0.00593859*npv +-0.851999);
      if(runera == TheRunEra::yUL2017E) METycorr = -(-0.00754254*npv +0.245956);
      if(runera == TheRunEra::yUL2017F) METxcorr = -(0.00765682*npv +-0.945001);
      if(runera == TheRunEra::yUL2017F) METycorr = -(-0.0154974*npv +0.804176);
      if(runera == TheRunEra::yUL2017MC) METxcorr = -(-0.0102265*npv +-0.446416);
      if(runera == TheRunEra::yUL2017MC) METycorr = -(0.0198663*npv +0.243182);


      if(runera == TheRunEra::yUL2018A) METxcorr = -(-0.0073377*npv +0.0250294);
      if(runera == TheRunEra::yUL2018A) METycorr = -(-0.000406059*npv +0.0417346);
      if(runera == TheRunEra::yUL2018B) METxcorr = -(0.00434261*npv +0.00892927);
      if(runera == TheRunEra::yUL2018B) METycorr = -(0.00234695*npv +0.20381);
      if(runera == TheRunEra::yUL2018C) METxcorr = -(0.00198311*npv +0.37026);
      if(runera == TheRunEra::yUL2018C) METycorr = -(-0.016127*npv +0.402029);
      if(runera == TheRunEra::yUL2018D) METxcorr = -(0.00220647*npv +0.378141);
      if(runera == TheRunEra::yUL2018D) METycorr = -(-0.0160244*npv +0.471053);
      if(runera == TheRunEra::yUL2018MC) METxcorr = -(-0.0214557*npv +0.969428);
      if(runera == TheRunEra::yUL2018MC) METycorr = -(0.0167134*npv +0.199296);
    }
  }
  else
  {
    //these are the corrections for v2 MET recipe (currently recommended for 2017)
    if(runera == TheRunEra::y2016B) METxcorr = -(-0.0374977*npv +0.00488262);
    if(runera == TheRunEra::y2016B) METycorr = -(0.107373*npv +-0.00732239);
    if(runera == TheRunEra::y2016C) METxcorr = -(-0.0832562*npv +0.550742);
    if(runera == TheRunEra::y2016C) METycorr = -(0.142469*npv +-0.153718);
    if(runera == TheRunEra::y2016D) METxcorr = -(-0.0400931*npv +0.753734);
    if(runera == TheRunEra::y2016D) METycorr = -(0.127154*npv +0.0175228);
    if(runera == TheRunEra::y2016E) METxcorr = -(-0.0409231*npv +0.755128);
    if(runera == TheRunEra::y2016E) METycorr = -(0.168407*npv +0.126755);
    if(runera == TheRunEra::y2016F) METxcorr = -(-0.0161259*npv +0.516919);
    if(runera == TheRunEra::y2016F) METycorr = -(0.141176*npv +0.544062);
    if(runera == TheRunEra::y2016G) METxcorr = -(0.0583851*npv +-0.0987447);
    if(runera == TheRunEra::y2016G) METycorr = -(0.0641427*npv +0.319112);
    if(runera == TheRunEra::y2016H) METxcorr = -(0.0706267*npv +-0.13118);
    if(runera == TheRunEra::y2016H) METycorr = -(0.127481*npv +0.370786);
    if(runera == TheRunEra::y2017B) METxcorr = -(-0.19563*npv +1.51859);
    if(runera == TheRunEra::y2017B) METycorr = -(0.306987*npv +-1.84713);
    if(runera == TheRunEra::y2017C) METxcorr = -(-0.161661*npv +0.589933);
    if(runera == TheRunEra::y2017C) METycorr = -(0.233569*npv +-0.995546);
    if(runera == TheRunEra::y2017D) METxcorr = -(-0.180911*npv +1.23553);
    if(runera == TheRunEra::y2017D) METycorr = -(0.240155*npv +-1.27449);
    if(runera == TheRunEra::y2017E) METxcorr = -(-0.149494*npv +0.901305);
    if(runera == TheRunEra::y2017E) METycorr = -(0.178212*npv +-0.535537);
    if(runera == TheRunEra::y2017F) METxcorr = -(-0.165154*npv +1.02018);
    if(runera == TheRunEra::y2017F) METycorr = -(0.253794*npv +0.75776);
    if(runera == TheRunEra::y2018A) METxcorr = -(0.362642*npv +-1.55094);
    if(runera == TheRunEra::y2018A) METycorr = -(0.0737842*npv +-0.677209);
    if(runera == TheRunEra::y2018B) METxcorr = -(0.485614*npv +-2.45706);
    if(runera == TheRunEra::y2018B) METycorr = -(0.181619*npv +-1.00636);
    if(runera == TheRunEra::y2018C) METxcorr = -(0.503638*npv +-1.01281);
    if(runera == TheRunEra::y2018C) METycorr = -(0.147811*npv +-1.48941);
    if(runera == TheRunEra::y2018D) METxcorr = -(0.520265*npv +-1.20322);
    if(runera == TheRunEra::y2018D) METycorr = -(0.143919*npv +-0.979328);
    if(runera == TheRunEra::y2016MC) METxcorr = -(-0.159469*npv +-0.407022);
    if(runera == TheRunEra::y2016MC) METycorr = -(-0.0405812*npv +0.570415);
    if(runera == TheRunEra::y2017MC) METxcorr = -(-0.182569*npv +0.276542);
    if(runera == TheRunEra::y2017MC) METycorr = -(0.155652*npv +-0.417633);
    if(runera == TheRunEra::y2018MC) METxcorr = -(0.299448*npv +-0.13866);
    if(runera == TheRunEra::y2018MC) METycorr = -(0.118785*npv +0.0889588);
  }

  return { METxcorr, METycorr };
}
