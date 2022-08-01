# TallinnNtupleProducer

Code, python scripts and config files for producing "plain" Tallinn Ntuples

## Setup

### To checkout the code

```bash
source /cvmfs/cms.cern.ch/cmsset_default.sh
cmsrel CMSSW_12_3_1 && cd $_
cmsenv
cd $CMSSW_BASE/src

git clone git@github.com:HEP-KBFI/TallinnNtupleProducer.git
git clone -b run2_SFs https://github.com/HEP-KBFI/TauTriggerSFs TauAnalysisTools/TauTriggerSFs
git clone https://github.com/cms-tau-pog/TauIDSFs TauPOG/TauIDSFs

scram b -j 8
```

### To run the code:

```bash
source /cvmfs/cms.cern.ch/cmsset_default.sh
cd CMSSW_12_3_1
cmsenv
cd $CMSSW_BASE/src/TallinnNtupleProducer/Framework/test
produceNtuple produceNtuple_cfg.py
```

## Additional documentation

### Generate documentation from input NanoAOD:

```bash
$CMSSW_RELEASE_BASE/src/PhysicsTools/NanoAOD/test/inspectNanoFile.py -d nano.html nano.root
```

Example output [here](https://kaehatah.web.cern.ch/nanodoc/nano.html)

### JSON POG interface

Available [here](https://kaehatah.web.cern.ch/json_prelegacy/)

If you want to generate the API documents yourself, run the following:

```bash
correction --html out.html /path/to/pog.json.gz
```

