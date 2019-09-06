import os
from argparse import ArgumentParser

import scheduler_base
from scheduler_base import JobConf

p = ArgumentParser()
p.add_argument( '--run'     , dest='run'     , default=False, action='store_true', help='Run filtering'              )
p.add_argument( '--check'   , dest='check'   , default=False, action='store_true', help='Run check of completion'    )
p.add_argument( '--clean'   , dest='clean'   , default=False, action='store_true', help='Run cleanup of extra files' )
p.add_argument( '--resubmit', dest='resubmit', default=False, action='store_true', help='Only submit missing output' )
p.add_argument( '--local'   , dest='local'   , default=False , action='store_true', help='Run locally'                )
options = p.parse_args()

if not options.check :
    options.run = True
else :
    options.run = False

options.batch = ( not options.local )
options.year = 2016

### ATTENTION! Here you specify the directory containing the ntuples that you want to run over.
base = '/data/users/friccita/WGammaNtuple/'


jobs = [
        #--------------------------
        JobConf(base, 'SingleMuon', isData=True, year=2016         ),
        #JobConf(base, 'SingleElectron', isData=True, year=2016        ),
        #JobConf(base, 'WGToLNuG_TuneCUETP8M1_13TeV-madgraphMLM-pythia8', year=2016                  ),
        #JobConf(base, 'WGToLNuG_PtG-130_TuneCUETP8M1_13TeV-madgraphMLM-pythia8', year=2016          ),
        #JobConf(base, 'WGToLNuG_PtG-130_TuneCUETP8M1_13TeV-madgraphMLM-pythia8PhCutMaxPhCutMin', year=2016          ),
        #JobConf(base, 'WGToLNuG_PtG-500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8', year=2016          ),
        #JobConf(base, 'WGToLNuG_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8', year=2016           ),
        #JobConf(base, 'WGToLNuG_PtG-130_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8', year=2016         ),
        #JobConf(base, 'WGToLNuG_PtG-130_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8PhCutMaxPhCutMin', year=2016         ),
        #JobConf(base, 'WGToLNuG_PtG-500_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8', year=2016        ),
        #JobConf(base, 'WJetsToLNu_TuneCUETP8M1_13TeV-madgraphMLM-pythia8TrueHTOlapPhOlap', year=2016     ),
        #JobConf(base, 'WJetsToLNu_TuneCUETP8M1_13TeV-madgraphMLM-pythia8', year=2016     ),
        #JobConf(base, 'WJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8', year=2016        ),
        #JobConf(base, 'WJetsToLNu_HT-100To200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8PhOlap', year=2016   ),
        #JobConf(base, 'WJetsToLNu_HT-200To400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8PhOlap', year=2016   ),  
        #JobConf(base, 'WJetsToLNu_HT-400To600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8PhOlap', year=2016   ),  
        #JobConf(base, 'WJetsToLNu_HT-600To800_TuneCUETP8M1_13TeV-madgraphMLM-pythia8PhOlap', year=2016  ),  
        #JobConf(base, 'WJetsToLNu_HT-800To1200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8PhOlap', year=2016  ), 
        #JobConf(base, 'WJetsToLNu_HT-1200To2500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8PhOlap', year=2016 ),
        #JobConf(base, 'WJetsToLNu_HT-2500ToInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8PhOlap', year=2016  ), 
        JobConf(base, 'DYJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8PhOlap', year=2016          ),
        JobConf(base, 'DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8PhOlap', year=2016          ),
        JobConf(base, 'DYJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8', year=2016          ),
        JobConf(base, 'DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8', year=2016          ),
        JobConf(base, 'ZGTo2LG_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8', year=2016 ),
        #JobConf(base, 'TTJets_DiLept_TuneCUETP8M1_13TeV-madgraphMLM-pythia8PhOlap', year=2016            ),
        #JobConf(base, 'TTJets_SingleLeptFromTbar_TuneCUETP8M1_13TeV-madgraphMLM-pythia8PhOlap', year=2016 ),
        #JobConf(base, 'TTJets_SingleLeptFromT_TuneCUETP8M1_13TeV-madgraphMLM-pythia8PhOlap', year=2016  ),
        #JobConf(base, 'TTGJets_TuneCUETP8M1_13TeV-amcatnloFXFX-madspin-pythia8', year=2016 ),
        #JobConf( base, 'GJets_HT-100To200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8', year=2016      ),
        #JobConf( base, 'GJets_HT-200To400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8', year=2016      ),
        #JobConf( base, 'GJets_HT-400To600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8', year=2016      ),
        #JobConf( base, 'GJets_HT-40To100_TuneCUETP8M1_13TeV-madgraphMLM-pythia8', year=2016       ),
        #JobConf( base, 'GJets_HT-600ToInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8', year=2016      ),
        #JobConf( base, 'DiPhotonJets_MGG-80toInf_13TeV_amcatnloFXFX_pythia8', year=2016 ),
        #JobConf( base, 'WWTo2L2Nu_13TeV-powheg', year=2016    ),
        #JobConf(base, 'WWG_TuneCUETP8M1_13TeV-amcatnlo-pythia8', year=2016  ),
        #JobConf(base, 'WZG_TuneCUETP8M1_13TeV-amcatnlo-pythia8', year=2016  ),

        #JobConf(base,'MadGraphChargedResonance_WGToLNu_M1000_width5'     ),
        #JobConf(base,'MadGraphChargedResonance_WGToLNu_M1200_width5'    ),
        #JobConf(base,'MadGraphChargedResonance_WGToLNu_M1400_width5'    ),
        #JobConf(base,'MadGraphChargedResonance_WGToLNu_M1600_width5'    ),
        #JobConf(base,'MadGraphChargedResonance_WGToLNu_M1800_width5'    ),
        #JobConf(base,'MadGraphChargedResonance_WGToLNu_M2000_width5'    ),
        #JobConf(base,'MadGraphChargedResonance_WGToLNu_M200_width5'     ),
        #JobConf(base,'MadGraphChargedResonance_WGToLNu_M2200_width5'    ),
        #JobConf(base,'MadGraphChargedResonance_WGToLNu_M2400_width5'    ),
        #JobConf(base,'MadGraphChargedResonance_WGToLNu_M250_width5'     ),
        #JobConf(base,'MadGraphChargedResonance_WGToLNu_M2800_width5'    ),
        #JobConf(base,'MadGraphChargedResonance_WGToLNu_M300_width5'     ),
        #JobConf(base,'MadGraphChargedResonance_WGToLNu_M3500_width5'    ),
        #JobConf(base,'MadGraphChargedResonance_WGToLNu_M350_width5'     ),
        #JobConf(base,'MadGraphChargedResonance_WGToLNu_M4000_width5'    ),
        #JobConf(base,'MadGraphChargedResonance_WGToLNu_M400_width5'     ),
        #JobConf(base,'MadGraphChargedResonance_WGToLNu_M450_width5'     ),
        #JobConf(base,'MadGraphChargedResonance_WGToLNu_M500_width5'     ),
        #JobConf(base,'MadGraphChargedResonance_WGToLNu_M600_width5'     ),
        #JobConf(base,'MadGraphChargedResonance_WGToLNu_M700_width5'     ),
        #JobConf(base,'MadGraphChargedResonance_WGToLNu_M800_width5'     ),
        #JobConf(base,'MadGraphChargedResonance_WGToLNu_M900_width5'     ),

        #JobConf(base,'MadGraphChargedResonance_WGToLNu_M1000_width0p01' ),
        #JobConf(base,'MadGraphChargedResonance_WGToLNu_M1200_width0p01' ),
        #JobConf(base,'MadGraphChargedResonance_WGToLNu_M1400_width0p01' ),
        #JobConf(base,'MadGraphChargedResonance_WGToLNu_M1600_width0p01' ),
        #JobConf(base,'MadGraphChargedResonance_WGToLNu_M1800_width0p01' ),
        #JobConf(base,'MadGraphChargedResonance_WGToLNu_M2000_width0p01' ),
        #JobConf(base,'MadGraphChargedResonance_WGToLNu_M200_width0p01'  ),
        #JobConf(base,'MadGraphChargedResonance_WGToLNu_M2200_width0p01' ),
        #JobConf(base,'MadGraphChargedResonance_WGToLNu_M2400_width0p01' ),
        #JobConf(base,'MadGraphChargedResonance_WGToLNu_M250_width0p01'  ),
        #JobConf(base,'MadGraphChargedResonance_WGToLNu_M2600_width0p01' ),
        #JobConf(base,'MadGraphChargedResonance_WGToLNu_M2800_width0p01' ),
        #JobConf(base,'MadGraphChargedResonance_WGToLNu_M3000_width0p01' ),
        #JobConf(base,'MadGraphChargedResonance_WGToLNu_M300_width0p01'  ),
        #JobConf(base,'MadGraphChargedResonance_WGToLNu_M3500_width0p01' ),
        #JobConf(base,'MadGraphChargedResonance_WGToLNu_M350_width0p01'  ),
        #JobConf(base,'MadGraphChargedResonance_WGToLNu_M4000_width0p01' ),
        #JobConf(base,'MadGraphChargedResonance_WGToLNu_M400_width0p01'  ),
        #JobConf(base,'MadGraphChargedResonance_WGToLNu_M450_width0p01'  ),
        #JobConf(base,'MadGraphChargedResonance_WGToLNu_M500_width0p01'  ),
        #JobConf(base,'MadGraphChargedResonance_WGToLNu_M600_width0p01'  ),
        #JobConf(base,'MadGraphChargedResonance_WGToLNu_M700_width0p01'  ),
        #JobConf(base,'MadGraphChargedResonance_WGToLNu_M800_width0p01'  ),
        #JobConf(base,'MadGraphChargedResonance_WGToLNu_M900_width0p01'  ),

        #JobConf(base,'PythiaChargedResonance_WGToLNu_M1000_width5'   ),
        #JobConf(base,'PythiaChargedResonance_WGToLNu_M1200_width5'   ),
        #JobConf(base,'PythiaChargedResonance_WGToLNu_M1400_width5'   ),
        #JobConf(base,'PythiaChargedResonance_WGToLNu_M1600_width5'   ),
        #JobConf(base,'PythiaChargedResonance_WGToLNu_M1800_width5'   ),
        #JobConf(base,'PythiaChargedResonance_WGToLNu_M2000_width5'   ),
        #JobConf(base,'PythiaChargedResonance_WGToLNu_M200_width5'    ),
        #JobConf(base,'PythiaChargedResonance_WGToLNu_M2200_width5'   ),
        #JobConf(base,'PythiaChargedResonance_WGToLNu_M2400_width5'   ),
        #JobConf(base,'PythiaChargedResonance_WGToLNu_M250_width5'    ),
        #JobConf(base,'PythiaChargedResonance_WGToLNu_M2800_width5'   ),
        #JobConf(base,'PythiaChargedResonance_WGToLNu_M300_width5'    ),
        #JobConf(base,'PythiaChargedResonance_WGToLNu_M3500_width5'   ),
        #JobConf(base,'PythiaChargedResonance_WGToLNu_M350_width5'    ),
        #JobConf(base,'PythiaChargedResonance_WGToLNu_M4000_width5'   ),
        #JobConf(base,'PythiaChargedResonance_WGToLNu_M400_width5'    ),
        #JobConf(base,'PythiaChargedResonance_WGToLNu_M450_width5'    ),
        #JobConf(base,'PythiaChargedResonance_WGToLNu_M500_width5'    ),
        #JobConf(base,'PythiaChargedResonance_WGToLNu_M600_width5'    ),
        #JobConf(base,'PythiaChargedResonance_WGToLNu_M700_width5'    ),
        #JobConf(base,'PythiaChargedResonance_WGToLNu_M800_width5'    ),
        #JobConf(base,'PythiaChargedResonance_WGToLNu_M900_width5'    ),
        #JobConf(base,'PythiaChargedResonance_WGToLNu_M1000_width0p01'),
        #JobConf(base,'PythiaChargedResonance_WGToLNu_M1200_width0p01'),
        #JobConf(base,'PythiaChargedResonance_WGToLNu_M1400_width0p01'),
        #JobConf(base,'PythiaChargedResonance_WGToLNu_M1600_width0p01'),
        #JobConf(base,'PythiaChargedResonance_WGToLNu_M1800_width0p01'),
        #JobConf(base,'PythiaChargedResonance_WGToLNu_M2000_width0p01'),
        #JobConf(base,'PythiaChargedResonance_WGToLNu_M200_width0p01' ),
        #JobConf(base,'PythiaChargedResonance_WGToLNu_M2200_width0p01'),
        #JobConf(base,'PythiaChargedResonance_WGToLNu_M2400_width0p01'),
        #JobConf(base,'PythiaChargedResonance_WGToLNu_M250_width0p01' ),
        #JobConf(base,'PythiaChargedResonance_WGToLNu_M2600_width0p01'),
        #JobConf(base,'PythiaChargedResonance_WGToLNu_M2800_width0p01'),
        #JobConf(base,'PythiaChargedResonance_WGToLNu_M3000_width0p01'),
        #JobConf(base,'PythiaChargedResonance_WGToLNu_M300_width0p01' ),
        #JobConf(base,'PythiaChargedResonance_WGToLNu_M3500_width0p01'),
        #JobConf(base,'PythiaChargedResonance_WGToLNu_M350_width0p01' ),
        #JobConf(base,'PythiaChargedResonance_WGToLNu_M4000_width0p01'),
        #JobConf(base,'PythiaChargedResonance_WGToLNu_M400_width0p01' ),
        #JobConf(base,'PythiaChargedResonance_WGToLNu_M450_width0p01' ),
        #JobConf(base,'PythiaChargedResonance_WGToLNu_M500_width0p01' ),
        #JobConf(base,'PythiaChargedResonance_WGToLNu_M600_width0p01' ),
        #JobConf(base,'PythiaChargedResonance_WGToLNu_M700_width0p01' ),
        #JobConf(base,'PythiaChargedResonance_WGToLNu_M800_width0p01' ),
        #JobConf(base,'PythiaChargedResonance_WGToLNu_M900_width0p01' ),
]

options.nFilesPerJob = 1

options.nproc = 4
options.treename='UMDNTuple/EventTree'
options.exename='RunAnalysis'
options.copyInputFiles=True
options.enableKeepFilter=False

### ATTENTION! Here you specify the type of ntuple you want to run over.
input_dirs = [
#              'LepLep_elel_2019_04_11','LepLep_mumu_2019_04_11',
#              'LepGammaNoPhId_elg_2019_03_17','LepGammaNoPhId_mug_2019_03_17',
#              'LepGamma_elg_2019_04_11','LepGamma_mug_2019_04_11',
#              'LepLep_mumu_2019_07_12',
              'LepGammaNoPhId_mug_2019_09_04',
]

configs = []

### ATTENTION! Here you specify the types of scale factors you want to calculate and save.
### In the data folder and in scripts/Conf.py, make sure that the scale factor files you use are correct and up to date!
for input_dir in input_dirs:
    configs.append(
                    {
                     'module'      : 'Conf%i.py' %options.year,
                     #'args'        : {'functions' : 'get_muon_sf,get_electron_sf,get_photon_sf,get_pileup_sf' },
                     'args'        : {'functions' : 'get_muon_sf,get_photon_sf,get_electron_sf'},
                     'input'       : input_dir,
                     'output'      : base + input_dir + '/WithSF',
                     'tag'         : 'FinalSF'
                    },

                  )


scheduler_base.RunJobs( jobs, configs, options)
