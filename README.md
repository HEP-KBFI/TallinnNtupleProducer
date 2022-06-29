# TallinnNtupleProducer

Code, python scripts and config files for producing "plain" Tallinn Ntuples

# To checkout the code

```bash
source /cvmfs/cms.cern.ch/cmsset_default.sh
cmsrel CMSSW_12_3_1 && cd $_
cmsenv
cd $CMSSW_BASE/src

git clone git@github.com:HEP-KBFI/TallinnNtupleProducer.git
git clone https://github.com/HEP-KBFI/nanoAOD-tools PhysicsTools/NanoAODTools
git clone -b run2_SFs https://github.com/HEP-KBFI/TauTriggerSFs TauAnalysisTools/TauTriggerSFs
git clone https://github.com/cms-tau-pog/TauIDSFs TauPOG/TauIDSFs

scram b -j 8
```

# To run the code:

```bash
source /cvmfs/cms.cern.ch/cmsset_default.sh
cd CMSSW_12_3_1
cmsenv
cd $CMSSW_BASE/src/TallinnNtupleProducer/Framework/test
produceNtuple produceNtuple_cfg.py
```
