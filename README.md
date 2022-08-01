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

Prelegacy corrections:

|   **POG**  | BTV | JERC | JMAR  | TAU | LUM |
|:----------:|:---:|:---:|:-:|:---:|:---:|
|    2016    |  [link](https://kaehatah.web.cern.ch/json_prelegacy/BTV_btagging_2016_prelegacy.html) ([CP5](https://kaehatah.web.cern.ch/json_prelegacy/BTV_btagging_2016_prelegacy_CP5.html))  |  [link](https://kaehatah.web.cern.ch/json_prelegacy/JERC_2016Legacy.html)   | [link](https://kaehatah.web.cern.ch/json_prelegacy/JMAR_2016_eoy.html)  |  [link](https://kaehatah.web.cern.ch/json_prelegacy/TAU_tau_2016_prelegacy.html)   |  [link](https://kaehatah.web.cern.ch/json_prelegacy/LUM_pu_2016_prelegacy.html)   |
|    2017    |  [link](https://kaehatah.web.cern.ch/json_prelegacy/BTV_btagging_2017_prelegacy.html)   |  [link](https://kaehatah.web.cern.ch/json_prelegacy/JERC_2017Legacy.html)   | [link](https://kaehatah.web.cern.ch/json_prelegacy/JMAR_2017_eoy.html)  | [link](https://kaehatah.web.cern.ch/json_prelegacy/TAU_tau_2017_prelegacy.html)    | [link](https://kaehatah.web.cern.ch/json_prelegacy/LUM_pu_2017_prelegacy.html)    |
|    2018    |  [link](https://kaehatah.web.cern.ch/json_prelegacy/BTV_btagging_2018_prelegacy.html)   |  [link](https://kaehatah.web.cern.ch/json_prelegacy/JERC_2018Legacy.html)   | [link](https://kaehatah.web.cern.ch/json_prelegacy/JMAR_2018_eoy.html)  |  [link](https://kaehatah.web.cern.ch/json_prelegacy/TAU_tau_2018_prelegacy.html)   |  [link](https://kaehatah.web.cern.ch/json_prelegacy/LUM_pu_2018_prelegacy.html)   |
| Sourced from |  [link](https://gitlab.cern.ch/kaehatah/btv-json-sf/-/tree/prelegacy)   |  [link](https://github.com/ktht/JECDatabase/tree/prelegacy/scripts/JERC2JSON)   |  [link](https://gitlab.cern.ch/cms-nanoAOD/jsonpog-integration/-/tree/c1f3742d67274039ce4a65859641994493e65d9c/POG/JME)  |  [link](https://gitlab.cern.ch/cms-nanoAOD/jsonpog-integration/-/tree/c1f3742d67274039ce4a65859641994493e65d9c/POG/TAU)   |  [link](https://gitlab.cern.ch/kaehatah/jsonpog-integration/-/tree/puPreLegacy)   |

All but Tau SF and JMAR corrections are custom. If you want to generate the API documents yourself, run the following:

```bash
correction --html out.html /path/to/pog.json.gz
```

### Configuration file

The primary `PSet` (`process`) is ignored in the following.

| Parameter in `fwliteInput` | Type            | Possible values | Description                                    |
|:---------------------------|:----------------|:----------------|:-----------------------------------------------|
| `fileNames`                | `cms.vstring()` |                 | Full paths to input NanoAOD files              |
| `skipEvents`               | `cms.int32()`   | >= 0            | Number of events to skip in the job            |
| `maxEvents`                | `cms.int32`     | -1; > 0         | How many events to process (-1 = all events)   |
| `outputEvery`              | `cms.uint32`    | > 0             | How often to log run:lumi:event nrs and memory |

| Parameter in `fwliteOutput` | Type            | Possible values | Description      |
|:----------------------------|:----------------|:----------------|:-----------------|
| `fileName`                  | `cms.string`    |                 | Output file name |

| Parameter in `produceNtuple` | Type            | Possible values  | When to change | Description                                                           |
|:-----------------------------|:----------------|:-----------------|:---------------|:----------------------------------------------------------------------|
| `treeName`                   | `cms.string`    | `Events`         | Never          | Input `TTree` name                                                    |
| `process`                    | `cms.string`    |                  | Every dataset  | Unique human-readable identifier for the dataset (MC sample, data PD) |
| `era`                        | `cms.string`    | 2016, 2017, 2018 | Every era      | The data-taking era                                                   |

(TBC)
