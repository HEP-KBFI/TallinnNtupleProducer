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

The following lists primary `PSet`s in the configuration file of `produceNtuple` jobs. The `cms` module is ignored when listing the type of parameter.

| Parameter in `fwliteInput` | Type      | Possible values | Description                                    |
|:---------------------------|:----------|:----------------|:-----------------------------------------------|
| `fileNames`                | `vstring` |                 | Full paths to input NanoAOD files              |
| `skipEvents`               | `int32`   | `>= 0`          | Number of events to skip in the job            |
| `maxEvents`                | `int32`   | `-1`; `> 0`     | How many events to process (-1 = all events)   |
| `outputEvery`              | `uint32`  | `> 0`           | How often to log run:lumi:event nrs and memory |

| Parameter in `fwliteOutput` | Type     | Possible values | Description      |
|:----------------------------|:---------|:----------------|:-----------------|
| `fileName`                  | `string` |                 | Output file name |

| Parameter in `produceNtuple`                      | Type      | Possible values                     | Modify depending on the...? | Description                                                                                                 |
|:--------------------------------------------------|:----------|:------------------------------------|:----------------------------|:------------------------------------------------------------------------------------------------------------|
| `treeName`                                        | `string`  | Events                              | Never                       | Input `TTree` name                                                                                          |
| `process`                                         | `string`  |                                     | Dataset                     | Unique human-readable identifier for the dataset (MC sample, data PD)                                       |
| `era`                                             | `string`  | 201[678]                            | Era                         | The data-taking era                                                                                         |
| `numNominalLeptons`                               | `uint32`  | `>= 0`                              | Channel                     | # fakeable leptons in the output; determines data/MC SF and charge flip rates                               |
| `numNominalHadTaus`                               | `uint32`  | `>= 0`                              | Channel                     | # fakeable had taus in the output; determines data/MC SF and charge flip rates                              |
| `applyNumNominalLeptonsCut`                       | `bool`    |                                     | Channel                     | If `True`, cut on `numNominalLeptons`                                                                       |
| `applyNumNominalHadTausCut`                       | `bool`    |                                     | Channel                     | If `True`, cut on `numNominalHadTaus`                                                                       |
| `lep_mva_cut_mu`                                  | `double`  | `> 0.`                              | Analysis                    | Cut on prompt muon MVA WP                                                                                   |
| `lep_mva_cut_e`                                   | `double`  | `> 0.`                              | Analysis                    | Cut on prompt electron MVA WP                                                                               |
| `lep_mva_wp`                                      | `string`  | hh_multilepton, default             | Analysis                    | Label for the choices of prompt lepton MVA WPs that tells which data/MC SF, flip rate or FR to load         |
| `hadTauWP_againstJets_tight`                      | `string`  | [VV]Loose, Medium [VV]Tight         | Channel                     | DeepTau ID WP for selecting tight had taus and rejecting jets                                               |
| `hadTauWP_againstJets_fakeable`                   | `string`  | [V]VVLoose                          | Never?                      | DeepTau ID WP for selecting fakeable had taus and rejecting jets                                            |
| `hadTauWP_againstElectrons`                       | `string`  | [VVV]Loose, Medium [VV]Tight        | Never                       | DeepTau ID WP for selecting fakeable had taus and rejecting electrons                                       |
| `hadTauWP_againstMuons`                           | `string`  | [V]Loose, Medium Tight              | Never                       | DeepTau ID WP for selecting fakeable had taus and rejecting muons                                           |
| `leptonFakeRateWeight.inputFileName`              | `string`  |                                     | Analysis, era               | Path to lepton FR file (depends on prompt lepton MVA WP and on `apply_chargeMisIdRate`)                     |
| `leptonFakeRateWeight.histogramName_e`            | `string`  |                                     | Analysis                    | Name of the histogram where electron FR are stored                                                          |
| `leptonFakeRateWeight.histogramName_mu`           | `string`  |                                     | Analysis                    | Name of the histogram where muon FR are stored                                                              |
| `leptonFakeRateWeight.applyNonClosureCorrection`  | `bool`    |                                     | Analysis                    | If `True`, apply non-closure corrections on the FR                                                          |
| `hadTauFakeRateWeight.inputFileName`              | `string`  |                                     | Analysis, era               | Path to tau FR file                                                                                         |
| `hadTauFakeRateWeight.(sub)lead.absEtaBins`       | `vdouble` |                                     | Never                       | Binning for tau FR, applicable to (sub)leading tau                                                          |
| `hadTauFakeRateWeight.(sub)lead.graphName`        | `string`  |                                     | Channel                     | Tau FR, applied to (sub)leading tau                                                                         |
| `hadTauFakeRateWeight.(sub)lead.fitFunctionName`  | `string`  |                                     | Channel                     | Data/MC SF of the tau FR, applied to (sub)leading tau                                                       |
| `hadTauFakeRateWeight.(sub)lead.applyGraph`       | `bool`    |                                     | Channel                     | If `True`, apply tau FR on (sub)leading tau                                                                 |
| `hadTauFakeRateWeight.(sub)lead.applyFitFunction` | `bool`    |                                     | Channel                     | If `True`, apply data/MC SF of the tau FR on (sub)leading tau                                               |
| `triggers`                                        | `PSet`    | `triggers_201[678]`                 | Era                         | HLT information                                                                                             |
| `triggers.type_*.use_it`                          | `bool`    |                                     | Channel                     | Set to `True` in order to apply the trigger cut                                                             |
| `isMC`                                            | `bool`    |                                     | Dataset                     | Set to `True` for MC                                                                                        |
| `lumiScale`                                       | `VPSet`   |                                     | Dataset                     | Lumiscale factors for MC (cross section x int luminosity in data / MC event statistics)                     |
| `lumiScale{.central_or_shift}`                    | `string`  | central, CMS_ttHl_*                 | Dataset                     | Label for the systematic shift (QCD scale, PS or PDF)                                                       |
| `lumiScale{.lumi}`                                | `double`  |                                     | Dataset                     | Lumiscale factor (nominal lumiscale x shifted / nominal MC event statistics)                                |
| `ref_genWeight`                                   | `double`  |                                     | Dataset                     | Most frequently occurring generator-level weight in absolute value                                          |
| `apply_genWeight`                                 | `bool`    |                                     | Dataset                     | Set to `True` for MC in order to enable generator-level weights                                             |
| `apply_DYMCReweighting`                           | `bool`    |                                     | Dataset                     | Set to `True` for LO DY MC in order to improve its pT spectrum [**DEPRECATE**]                              |
| `apply_DYMCNormScaleFactors`                      | `bool`    |                                     | Dataset, analysis           | Set to `True` for LO DY MC in order to improve its normalization in the presence of b-jets [**DEPRECATE**]  |
| `apply_LHEVpt_rwgt`                               | `bool`    |                                     | Dataset                     | Set to `True` for LO W+jets MC in order to improve its pT spectrum [**DEPRECATE**]                          |
| `apply_topPtReweighting`                          | `string`  | HighPt, Quadratic, Linear, TOP16011 | Dataset                     | Set to HighPt for NLO ttbar MC in order to improve its pT spectrum                                          |
| `apply_l1PreFireWeight`                           | `bool`    |                                     | Era, dataset                | Set to `True` for 2017, 2018 MC in order to enable L1 prefiring weights [**DEPRECATE**]                     |
| `apply_pileupJetID`                               | `string`  | disabled, loose, medium, tight      | Analysis                    | Set to loose, medium or tight in order to apply PU jet ID at specified WP                                   |
| `fatJet_corrections`                              | `vstring` | JMS, JMR, PUPPI                     | Dataset, analysis           | Specify in order to enable et mass smearing (JMS), resolution (JMR) and softdrop mass (PUPPI) corrections   |
| `apply_chargeMisIdRate`                           | `bool`    |                                     | Channel                     | Set to `True` in order to calculate electron charge mis-identification probabilities (only for 2lss+<=1tau) |
| `split_jes`                                       | `bool`    |                                     | User preference             | If `True`, split JES into 11 components in MC                                                               |
| `split_jer`                                       | `bool`    |                                     | User preference             | If `True`, split JER into 6 components in MC                                                                |
| `isCP5`                                           | `bool`    |                                     | Era, dataset                | Set to `True` for 2016 CP5 MC in order to apply the correct b-tagging SF [**DEPRECATE**]                    |
| `reapply_JEC`                                     | `bool`    |                                     | Never                       | Set to `True` in order to redo JEC in MC                                                                    |
| `apply_smearing`                                  | `bool`    |                                     | User preference             | Set to `True` in order to smear jets in MC                                                                  |
| `enable_phiModulationCorr`                        | `bool`    |                                     | Never                       | Set to `True` in order to correct MET phi modulation in Run 2 [**DEPRECATE**]                               |
| `evtWeight`                                       | `PSet`    |                                     | Dataset                     | Settings for propagating stitching weights (for eg DY and W+jets MC)                                        |
| `evtWeight.apply`                                 | `bool`    |                                     | Dataset                     | Set to `True` in order to apply the stitching weights                                                       |
| `evtWeight.histogramFile`                         | `string`  |                                     | Dataset                     | Path to the file where stitching weights are stored                                                         |
| `evtWeight.histogramName`                         | `string`  |                                     | Dataset                     | Histogram name from the file where stitching weights are stored                                             |
| `evtWeight.branchNameXaxis`                       | `string`  | LHE_Njets, LHE_HT                   | Dataset                     | LHE-level variable name that parametrizes the x-coordinate of the histogram                                 |
| `evtWeight.branchNameYaxis`                       | `string`  | LHE_Njets, LHE_HT                   | Dataset                     | LHE-level variable name that parametrizes the y-coordinate of the histogram                                 |
| `evtWeight.branchTypeXaxis`                       | `string`  | Int_t, Float_t                      | Dataset                     | Type of the LHE-level variable that parametrizes the x-coordinate                                           |
| `evtWeight.branchTypeYaxis`                       | `string`  | Int_t, Float_t                      | Dataset                     | Type of the LHE-level variable that parametrizes the y-coordinate                                           |
| `branchName_electrons`                            | `string`  | Electron                            | Never                       | Name of the electron collection in NanoAOD                                                                  |
| `branchName_muons`                                | `string`  | Muon                                | Never                       | Name of the muon collection in NanoAOD                                                                      |
| `branchName_hadTaus`                              | `string`  | Tau                                 | Never                       | Name of the tau collection in NanoAOD                                                                       |
| `branchName_corrT1METJet`                         | `string`  | CorrT1METJet                        | Never                       | Name of the low-pT AK4 jets in NanoAOD (needed for calculating type-1 MET)                                  |
| `branchName_jets_ak4`                             | `string`  | Jet                                 | Never                       | Name of the AK4 jet collection in NanoAOD                                                                   |
| `branchName_jets_ak8`                             | `string`  | FatJet                              | Never                       | Name of the AK8 jet collection in NanoAOD                                                                   |
| `branchName_subjets_ak8`                          | `string`  | SubJet                              | Never                       | Name of the AK8 subjet collection in NanoAOD                                                                |
| `branchName_met`                                  | `string`  | MET                                 | Never                       | Name of the reconstructed MET in NanoAOD                                                                    |
| `branchName_rawMet`                               | `string`  | RawMET                              | Never                       | Name of the reconstructed MET without (L2+?) JEC in NanoAOD                                                 |
| `branchName_vertex`                               | `string`  | PV                                  | Never                       | Name of the primary vertex information in NanoAOD                                                           |
| `branchName_genJets`                              | `string`  | GenJet                              | Never                       | Name of the generator-level AK4 jets in NanoAOD                                                             |
| `branchName_genJetAK8`                            | `string`  | GenJetAK8                           | Never                       | Name of the generator-level AK8 jets in NanoAOD                                                             |
| `branchName_genSubJetAK8`                         | `string`  | SubGenJetAK8                        | Never                       | Name of the generator-level AK8 subjets in NanoAOD                                                          |
| `branchName_genPart`                              | `string`  | GenPart                             | Never                       | Name of the generator-level Pythia particles in NanoAOD                                                     |
| `branchName_genHadTaus`                           | `string`  | GenVisTau                           | Never                       | Name of the generator-level hadronic taus in NanoAOD                                                        |
| `branchName_PSweights `                           | `string`  | PSWeight                            | Never                       | Name of the parton shower weights in NanoAOD                                                                |
| `branchName_LHE_nom `                             | `string`  | LHEWeight_originalXWGTUP            | Never                       | Name of the nominal LHE weight in NanoAOD                                                                   |
| `has_PS_weights`                                  | `bool`    |                                     | Dataset                     | Set to `True` if the MC dataset has PS weights                                                              |
| `apply_LHE_nom`                                   | `bool`    |                                     | Dataset                     | Set to `True` if the nominal LHE weight should be applied on top of the PS weights                          |
| `branchName_scale_weights`                        | `string`  | LHEScaleWeight                      | Never                       | Name of the weights for different choices of QCD scale                                                      |
| `branchName_pdf_weights`                          | `string`  | LHEPdfWeight                        | Never                       | Name of the PDF weights                                                                                     |
| `has_LHE_weights`                                 | `bool`    |                                     | Dataset                     | Set to `True` if the MC dataset has weights for different choices of QCD scale                              |
| `has_pdf_weights`                                 | `bool`    |                                     | User preference, dataset    | Set to `True` if the MC dataset has PDF weights                                                             |
| `pdfSettings`                                     | `PSet`    |                                     | User preference, dataset    | Extra information regarding the PDF weights                                                                 |
| `pdfSettings.lhaid`                               | `uint32`  |                                     | User preference, dataset    | LHA ID of the PDF set for which the weights are stored                                                      |
| `pdfSettings.norm`                                | `vdouble` |                                     | User preference, dataset    | Ratio of sum of PDF weights for a given member to the sum of nominal PDF weights                            |
| `pdfSettings.saveAllMembers`                      | `bool`    |                                     | User preference, dataset    | If set to `True`, save the event weights for each PDF member (multiplied by `pdfSettings.norm`              |
| `branchName_LHE_particle`                         | `string`  |                                     | Never                       | Name of the LHE-level (ie ME-level) particle collection in NanoAOD                                          |
| `jetCleaningByIndex`                              | `bool`    |                                     | Never                       | If set to `True`, clean jets by their constituent leptons                                                   |
| `genMatchRecoJets`                                | `bool`    |                                     | Never?                      | If set to `True`, dR-match generator-level AK4 jets to reconstructed AK4 jets (expensive to run)            |
| `writerPlugins`                                   | `VPSet`   |                                     | User preference, channel    | Various plugins that dictate the list of branches in the output file                                        |
| `selEventsFileName`                               | `string`  |                                     | Debugging                   | Path to a text file containing a list of run:lumi:event numbers (one per line) for debugging single events  |
| `selection`                                       | `string`  |                                     | User preference             | Custom selection string to filter out events based on the output branch names                               |
| `isDEBUG`                                         | `bool`    |                                     | Debugging                   | If set to `True`, prints out very verbose information about every processed event (use with care)           |
