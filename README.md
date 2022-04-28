# TallinnNtupleProducer
code, python scripts and config files for producing "plain" Tallinn Ntuples

# To checkout the code:
source /cvmfs/cms.cern.ch/cmsset_default.sh

cmsrel CMSSW_12_3_1

cd CMSSW_12_3_1

cmsenv

git clone             https://github.com/HEP-KBFI/TallinnNtupleProducer $CMSSW_BASE/src/TallinnNtupleProducer

git clone             https://github.com/HEP-KBFI/nanoAOD-tools         $CMSSW_BASE/src/PhysicsTools/NanoAODTools

git clone -b run2_SFs https://github.com/HEP-KBFI/TauTriggerSFs         $CMSSW_BASE/src/TauAnalysisTools/TauTriggerSFs

git clone             https://github.com/cms-tau-pog/TauIDSFs           $CMSSW_BASE/src/TauPOG/TauIDSFs

cd $CMSSW_BASE/src/TallinnNtupleProducer

git remote set-url origin git+ssh://git@github.com/HEP-KBFI/TallinnNtupleProducer.git

cd $CMSSW_BASE/src/

scram b -j 8

# To run the code:
source /cvmfs/cms.cern.ch/cmsset_default.sh

cmsrel CMSSW_12_3_1

cd CMSSW_12_3_1

cmsenv

cd $CMSSW_BASE/src/TallinnNtupleProducer/Framework/test

produceNtuple produceNtuple_cfg.py

