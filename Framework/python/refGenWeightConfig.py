from tthAnalysis.HiggsToTauTau.jobTools import create_if_not_exists, run_cmd, get_log_version, check_submission_cmd, record_software_state
from tthAnalysis.HiggsToTauTau.analysisTools import initDict, getKey, createFile, generateInputFileList
from tthAnalysis.HiggsToTauTau.analysisTools import createMakefile as tools_createMakefile
from tthAnalysis.HiggsToTauTau.sbatchManagerTools import createScript_sbatch as tools_createScript_sbatch
from tthAnalysis.HiggsToTauTau.safe_root import ROOT
from tthAnalysis.HiggsToTauTau.common import logging, DEPENDENCIES

import os
import uuid
import itertools

DKEY_SCRIPTS = "scripts"
DKEY_CFGS    = "cfgs"
DKEY_RESULTS = "results"
DKEY_PLOTS   = "plots"
DKEY_LOGS    = "logs"

def validate_refGenWeight(output_file, samples):
    error_code = 0
    if not os.path.isfile(output_file):
        logging.error('File {} does not exist'.format(output_file))
        return 1
    histogram_file = ROOT.TFile.Open(output_file, 'read')
    if not histogram_file:
        logging.error('Not a valid ROOT file: {}'.format(output_file))
        return 2
    for sample_name, sample_info in samples.items():
        if not sample_info["use_it"]:
            continue
        process_name = sample_info["process_name_specific"]
        expected_nof_events = sample_info["nof_tree_events"]
        logging.info('Validating {} (expecting {} events)'.format(process_name, expected_nof_events))
        histogram = histogram_file.Get(process_name)
        if not histogram:
            logging.error("Could not find histogram '{}' in file {}".format(process_name, output_file))
            error_code = 3
            continue
        nof_events = int(histogram.GetEntries())
        if nof_events != expected_nof_events:
            logging.error(
                'Histogram {} in file {} has {} events, but expected {} events'.format(
                    process_name, output_file, nof_events, expected_nof_events,
                )
            )
            error_code = 4
        else:
            logging.info('Validation successful for sample {}'.format(process_name))
    histogram_file.Close()
    if error_code == 0:
        logging.info("Validation successful!")
    else:
        logging.error("Validation failed!")
    return error_code

class refGenWeightConfig:
    """Configuration metadata needed to run refGenWeight jobs

    Args:
        configDir:             The root config dir -- all configuration files are stored in its subdirectories
        outputDir:             The root output dir -- all log and output files are stored in its subdirectories
        executable:            Name of the executable that runs the PU profile production
        check_output_files:     if True, checks each input root file (Ntuple) before creating the python configuration files
        running_method:        either `sbatch` (uses SLURM) or `Makefile`
        num_parallel_jobs:     number of jobs that can be run in parallel on local machine
                               (does not limit number of PU profile production jobs running in parallel on batch system)

    """
    def __init__(self,
            configDir,
            localDir,
            outputDir,
            output_file,
            executable,
            samples,
            era,
            check_output_files,
            running_method,
            num_parallel_jobs,
            pool_id  = '',
            verbose  = False,
            dry_run  = False,
            use_home = False,
            keep_logs = False,
            submission_cmd = None,
          ):

        self.configDir             = configDir
        self.localDir              = localDir
        self.outputDir             = outputDir
        self.executable            = executable
        self.samples               = samples
        self.era                   = era
        self.check_output_files    = check_output_files
        self.verbose               = verbose
        self.dry_run               = dry_run
        self.use_home              = use_home
        self.keep_logs             = keep_logs
        if running_method.lower() not in ["sbatch", "makefile"]:
          raise ValueError("Invalid running method: %s" % running_method)

        self.running_method    = running_method
        self.is_sbatch         = self.running_method.lower() == "sbatch"
        self.is_makefile       = not self.is_sbatch
        self.makefile          = os.path.join(self.localDir, "Makefile_refGenWeight")
        self.num_parallel_jobs = num_parallel_jobs
        self.pool_id           = pool_id if pool_id else uuid.uuid4()

        self.workingDir = os.getcwd()
        logging.info("Working directory is: %s" % self.workingDir)
        self.template_dir = os.path.join(
            os.getenv('CMSSW_BASE'), 'src', 'tthAnalysis', 'HiggsToTauTau', 'test', 'templates'
        )
        logging.info("Templates directory is: %s" % self.template_dir)

        create_if_not_exists(self.configDir)
        create_if_not_exists(self.localDir)
        create_if_not_exists(self.outputDir)
        self.output_file      = os.path.join(self.localDir, output_file)
        self.stdout_file_path = os.path.join(self.localDir, "stdout.log")
        self.stderr_file_path = os.path.join(self.localDir, "stderr.log")
        self.sw_ver_file_cfg  = os.path.join(self.localDir, "VERSION.log")
        self.sw_ver_file_out  = os.path.join(self.outputDir, "VERSION.log")
        self.submission_out   = os.path.join(self.localDir, "SUBMISSION.log")
        self.stdout_file_path, self.stderr_file_path, self.sw_ver_file_cfg, self.sw_ver_file_out, self.submission_out = get_log_version((
            self.stdout_file_path, self.stderr_file_path, self.sw_ver_file_cfg, self.sw_ver_file_out, self.submission_out
        ))
        check_submission_cmd(self.submission_out, submission_cmd)

        self.sbatchFile = os.path.join(self.localDir, "sbatch.py")
        self.cfgFiles          = {}
        self.logFiles          = {}
        self.plotFiles         = {}
        self.scriptFiles       = {}
        self.jobOptions_sbatch = {}

        self.inputFiles  = {}
        self.outputFiles = {}

        self.phoniesToAdd = []
        self.filesToClean = [ self.configDir ]
        self.targets = []

        self.makefile_target = "sbatch_refGenWeight"

        self.dirs = {}
        all_dirs = [ DKEY_CFGS, DKEY_RESULTS, DKEY_LOGS, DKEY_PLOTS, DKEY_SCRIPTS ]
        cfg_dirs = [ DKEY_CFGS, DKEY_RESULTS, DKEY_LOGS, DKEY_PLOTS, DKEY_SCRIPTS ]

        for sample_name, sample_info in self.samples.items():
            if not sample_info['use_it']:
                continue
            process_name = sample_info["process_name_specific"]
            key_dir = getKey(process_name)
            for dir_type in all_dirs:
                initDict(self.dirs, [ key_dir, dir_type ])
                if dir_type in cfg_dirs:
                    dir_choice = self.configDir if dir_type == DKEY_CFGS else self.localDir
                    self.dirs[key_dir][dir_type] = os.path.join(dir_choice, dir_type, process_name)
                else:
                    self.dirs[key_dir][dir_type] = os.path.join(self.outputDir, dir_type, process_name)
        for dir_type in cfg_dirs:
            initDict(self.dirs, [ dir_type ])
            dir_choice = self.configDir if dir_type == DKEY_CFGS else self.localDir
            self.dirs[dir_type] = os.path.join(dir_choice, dir_type)
            if dir_choice != self.configDir:
                self.filesToClean.append(self.dirs[dir_type])

        self.cvmfs_error_log = {}
        self.num_jobs = {
            'hadd'         : 0,
            'refGenWeight' : 0,
            'plot'         : 0,
        }


    def createCfg(self, jobOptions):
        """Create python configuration file for the refGenWeight script

        Args:
          inputFiles: list of input files (Ntuples)
          outputFile: output file of the job -- a ROOT file containing histogram
        """
        lines = jobOptions['inputFiles']
        createFile(jobOptions['cfgFile_path'], lines, nofNewLines = 1)

    def createScript_sbatch(self,
                            executable,
                            sbatchFile,
                            jobOptions,
                            key_cmd_params = 'cmdParams',
                            key_input_file = 'inputFiles',
                            key_output_file = 'outputFile',
                            key_log_file = 'logFile',
                            key_script_file = 'scriptFile',
                           ):
        num_jobs = tools_createScript_sbatch(
            sbatch_script_file_name = sbatchFile,
            executable              = executable,
            command_line_parameters = { key: value[key_cmd_params]  for key, value in jobOptions.items() },
            input_file_names        = { key: value[key_input_file]  for key, value in jobOptions.items() },
            output_file_names       = { key: value[key_output_file] for key, value in jobOptions.items() },
            script_file_names       = { key: value[key_script_file] for key, value in jobOptions.items() },
            log_file_names          = { key: value[key_log_file]    for key, value in jobOptions.items() },
            keep_logs               = self.keep_logs,
            working_dir             = self.workingDir,
            cvmfs_error_log         = self.cvmfs_error_log,
            pool_id                 = uuid.uuid4(),
            verbose                 = self.verbose,
            dry_run                 = self.dry_run,
            job_template_file       = 'sbatch-node.produce.sh.template',
            validate_outputs        = self.check_output_files,
            min_file_size           = -1,
            use_home                = self.use_home,
            copy_output_file        = False,
        )
        return num_jobs

    def addToMakefile(self, lines_makefile):
        """Adds the commands to Makefile that are necessary for running the PU profile production code
        """
        if self.is_sbatch:
            lines_makefile.extend([
                "%s:" % self.makefile_target,
                "\t%s %s" % ("python", self.sbatchFile),
                "",
            ])
        for key_file, output_file in self.outputFiles.items():
            if not key_file in self.cfgFiles:
                # output file already exists
                continue
            cfg_file = self.cfgFiles[key_file]
            if self.is_makefile:
                log_file = self.logFiles[key_file]
                plot_files = self.plotFiles[key_file]
                lines_makefile.extend([
                    "%s:" % output_file,
                    "\t%s -i %s -o %s -p %s &> %s" % (self.executable, cfg_file, output_file, plot_files, log_file),
                    "",
                ])
            elif self.is_sbatch:
                lines_makefile.extend([
                    "%s: %s" % (output_file, self.makefile_target),
                    "\t%s" % ":",
                    "",
                ])
            self.targets.append(output_file)
        self.phoniesToAdd.append(self.makefile_target)

    def addToMakefile_final(self, lines_makefile):
        self.targets.append(self.output_file)
        lines_makefile.extend([
            "%s: %s" % (self.output_file, ' '.join([ self.outputFiles[key_file] for key_file in self.outputFiles ])),
            "\trm -f %s" % self.output_file,
        ])
        for key_file in sorted(self.outputFiles.keys()):
            lines_makefile.append("\techo %s $(shell cat %s) >> %s" % (key_file, self.outputFiles[key_file], self.output_file))

    def createMakefile(self, lines_makefile):
        """Creates Makefile that runs the PU profile production.
        """
        tools_createMakefile(
            makefileName   = self.makefile,
            targets        = self.targets,
            lines_makefile = lines_makefile,
            filesToClean   = self.filesToClean,
            isSbatch       = self.is_sbatch,
            phoniesToAdd   = self.phoniesToAdd
        )
        logging.info("Run it with:\tmake -f %s -j %i " % (self.makefile, self.num_parallel_jobs))

    def create(self):
        """Creates all necessary config files and runs the PU profile production -- either locally or on the batch system
        """

        for key in self.dirs.keys():
            if type(self.dirs[key]) == dict:
                for dir_type in self.dirs[key].keys():
                    create_if_not_exists(self.dirs[key][dir_type])
            else:
                create_if_not_exists(self.dirs[key])

        self.inputFileIds = {}
        for sample_name, sample_info in self.samples.items():
            if not sample_info['use_it']:
                continue

            process_name = sample_info["process_name_specific"]
            is_mc = (sample_info["type"] == "mc")

            if not is_mc:
              continue

            logging.info("Creating configuration files to run '%s' for sample %s" % (self.executable, process_name))

            inputFileList_map = generateInputFileList(sample_info, 1)
            key_dir = getKey(process_name)
            key_file = getKey(process_name)

            self.inputFiles[key_file] = list(itertools.chain(*inputFileList_map.values()))
            if len(self.inputFiles[key_file]) == 0:
                logging.warning(
                    "'%s' = %s --> skipping job !!" % (key_file, self.inputFiles[key_file])
                )
                continue
            
            outputFile = os.path.join(
                self.dirs[key_dir][DKEY_RESULTS], "%s.txt" % process_name
            )
            self.outputFiles[key_file] = outputFile
            if os.path.isfile(outputFile):
                logging.info('File {} already exists --> skipping job'.format(outputFile))
                continue

            self.cfgFiles[key_file] = os.path.join(
                self.dirs[key_dir][DKEY_CFGS], "refGenWeight_%s_cfg.txt" % (process_name)
            )
            self.logFiles[key_file] = os.path.join(
                self.dirs[key_dir][DKEY_LOGS], "refGenWeight_%s.log" % (process_name)
            )
            self.scriptFiles[key_file] = os.path.join(
                self.dirs[key_dir][DKEY_CFGS], "refGenWeight_%s_cfg.sh" % (process_name)
            )
            self.plotFiles[key_file] = ' '.join([
                os.path.join(self.dirs[key_dir][DKEY_PLOTS], "refGenWeight_%s.%s" % (process_name, extension))
                for extension in [ 'pdf', 'png' ]
            ])

            self.jobOptions_sbatch[key_file] = {
                'inputFiles'   : self.inputFiles[key_file],
                'cfgFile_path' : self.cfgFiles[key_file],
                'cmdParams'    : "-i {} -o {} -p {} -v".format(
                    self.cfgFiles[key_file],
                    self.outputFiles[key_file],
                    self.plotFiles[key_file],
                ),
                'outputFile'   : self.outputFiles[key_file],
                'logFile'      : self.logFiles[key_file],
                'scriptFile'   : self.scriptFiles[key_file],
            }
            self.createCfg(self.jobOptions_sbatch[key_file])

        if self.is_sbatch:
          logging.info("Creating script for submitting '%s' jobs to batch system" % self.executable)
          self.num_jobs['refGenWeight'] += self.createScript_sbatch(
              self.executable, self.sbatchFile, self.jobOptions_sbatch
          )

        logging.info("Creating Makefile")
        lines_makefile = []
        self.addToMakefile(lines_makefile)
        self.addToMakefile_final(lines_makefile)
        self.createMakefile(lines_makefile)
        logging.info("Done")

        return self.num_jobs

    def run(self):
        """Runs all PU profile production jobs -- either locally or on the batch system.
        """
        record_software_state(self.sw_ver_file_cfg, self.sw_ver_file_out, DEPENDENCIES)
        run_cmd(
            "make -f %s -j %i 2>%s 1>%s" % \
            (self.makefile, self.num_parallel_jobs, self.stderr_file_path, self.stdout_file_path),
            False
        )
