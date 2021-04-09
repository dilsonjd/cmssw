# Auto generated configuration file
# using: 
# Revision: 1.19 
# Source: /local/reps/CMSSW/CMSSW/Configuration/Applications/python/ConfigBuilder.py,v 
# with command line options: step2 --conditions auto:phase1_2017_realistic --pileup_input das:/RelValMinBias_13/CMSSW_10_5_0_pre1-103X_mc2017_realistic_v2-v1/GEN-SIM -n 10 --era Run2_2017 --eventcontent FEVTDEBUGHLT -s DIGI:pdigi_valid,L1,DIGI2RAW,HLT:@relval2017 --datatier GEN-SIM-DIGI-RAW --pileup AVE_35_BX_25ns --geometry DB:Extended --filein filelist:step1_dasquery.log --fileout file:step2.root
import FWCore.ParameterSet.Config as cms

from Configuration.StandardSequences.Eras import eras

process = cms.Process('HLT',eras.Run2_2017, eras.run3_RPC)

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
#process.load('SimGeneral.MixingModule.mix_POISSON_average_cfi')
process.load('SimGeneral.MixingModule.mixNoPU_cfi')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_cff')
process.load('Configuration.StandardSequences.Digi_cff')
process.load('Configuration.StandardSequences.SimL1Emulator_cff')
process.load('Configuration.StandardSequences.DigiToRaw_cff')
process.load('HLTrigger.Configuration.HLT_Fake2_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

# Input source
process.source = cms.Source("PoolSource",
    dropDescendantsOfDroppedBranches = cms.untracked.bool(False),
    fileNames = cms.untracked.vstring('file:ttbarstep1.root'),
    inputCommands = cms.untracked.vstring(
        'keep *', 
        'drop *_genParticles_*_*', 
        'drop *_genParticlesForJets_*_*', 
        'drop *_kt4GenJets_*_*', 
        'drop *_kt6GenJets_*_*', 
        'drop *_iterativeCone5GenJets_*_*', 
        'drop *_ak4GenJets_*_*', 
        'drop *_ak7GenJets_*_*', 
        'drop *_ak8GenJets_*_*', 
        'drop *_ak4GenJetsNoNu_*_*', 
        'drop *_ak8GenJetsNoNu_*_*', 
        'drop *_genCandidatesForMET_*_*', 
        'drop *_genParticlesForMETAllVisible_*_*', 
        'drop *_genMetCalo_*_*', 
        'drop *_genMetCaloAndNonPrompt_*_*', 
        'drop *_genMetTrue_*_*', 
        'drop *_genMetIC5GenJs_*_*'
    ),
    secondaryFileNames = cms.untracked.vstring()
)

process.options = cms.untracked.PSet(

)

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    annotation = cms.untracked.string('step2 nevts:10'),
    name = cms.untracked.string('Applications'),
    version = cms.untracked.string('$Revision: 1.19 $')
)

# Output definition

process.FEVTDEBUGHLToutput = cms.OutputModule("PoolOutputModule",
    dataset = cms.untracked.PSet(
        dataTier = cms.untracked.string('GEN-SIM-DIGI-RAW'),
        filterName = cms.untracked.string('')
    ),
    fileName = cms.untracked.string('file:ttbarstep2.root'),
    outputCommands = process.FEVTDEBUGHLTEventContent.outputCommands,
    splitLevel = cms.untracked.int32(0)
)

# Additional output definition

# Other statements
#process.mix.input.nbPileupEvents.averageNumber = cms.double(35.000000)
#process.mix.bunchspace = cms.int32(25)
#process.mix.minBunch = cms.int32(-12)
#process.mix.maxBunch = cms.int32(3)
#process.mix.input.fileNames = cms.untracked.vstring(['/store/relval/CMSSW_10_5_0_pre1/RelValMinBias_13/GEN-SIM/103X_mc2017_realistic_v2-v1/20000/224DA2A6-BA5A-FE44-8888-9608E3C3491F.root', '/store/relval/CMSSW_10_5_0_pre1/RelValMinBias_13/GEN-SIM/103X_mc2017_realistic_v2-v1/20000/22947840-2543-6A4A-B2BD-B9155495D0B3.root', '/store/relval/CMSSW_10_5_0_pre1/RelValMinBias_13/GEN-SIM/103X_mc2017_realistic_v2-v1/20000/2C3F1D6A-E892-1D44-9A70-E4E0E609694B.root', '/store/relval/CMSSW_10_5_0_pre1/RelValMinBias_13/GEN-SIM/103X_mc2017_realistic_v2-v1/20000/3342EB73-02EB-6346-B81B-3A7635DE89C2.root', '/store/relval/CMSSW_10_5_0_pre1/RelValMinBias_13/GEN-SIM/103X_mc2017_realistic_v2-v1/20000/53101FB7-F71D-2D46-BF45-FE15E6D0E5E2.root', '/store/relval/CMSSW_10_5_0_pre1/RelValMinBias_13/GEN-SIM/103X_mc2017_realistic_v2-v1/20000/ABFA3921-52E7-134C-87B6-D58052402997.root', '/store/relval/CMSSW_10_5_0_pre1/RelValMinBias_13/GEN-SIM/103X_mc2017_realistic_v2-v1/20000/ACB3CEC8-09F0-174B-AD93-84BA32EED0C5.root'])
#process.mix.digitizers = cms.PSet(process.theDigitizersValid)

from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:phase1_2017_realistic', '')

# Other statements
process.load("CondCore.CondDB.CondDB_cfi")
process.CondDB.connect = "frontier://FrontierProd/CMS_CONDITIONS"

process.cppfnewmap = cms.ESSource("PoolDBESSource",
        process.CondDB,
        DumpStat = cms.untracked.bool(True),
        toGet = cms.VPSet(
          cms.PSet(
            record = cms.string('RPCCPPFLinkMapRcd'),
            tag = cms.string("RPCCPPFLinkMap_L1T_v1_update_v2")
           )
        )
)

process.es_prefer_cppfnewmap =cms.ESPrefer("PoolDBESSource","cppfnewmap")
 
# Path and EndPath definitions
process.digitisation_step = cms.Path(process.pdigi_valid)
process.L1simulation_step = cms.Path(process.SimL1Emulator)
process.digi2raw_step = cms.Path(process.DigiToRaw)
process.endjob_step = cms.EndPath(process.endOfProcess)
process.FEVTDEBUGHLToutput_step = cms.EndPath(process.FEVTDEBUGHLToutput)

# Schedule definition
process.schedule = cms.Schedule(process.digitisation_step,process.L1simulation_step,process.digi2raw_step)
process.schedule.extend(process.HLTSchedule)
process.schedule.extend([process.endjob_step,process.FEVTDEBUGHLToutput_step])
from PhysicsTools.PatAlgos.tools.helpers import associatePatAlgosToolsTask
associatePatAlgosToolsTask(process)

# customisation of the process.

# Automatic addition of the customisation function from HLTrigger.Configuration.customizeHLTforMC
from HLTrigger.Configuration.customizeHLTforMC import customizeHLTforMC 

#call to customisation function customizeHLTforMC imported from HLTrigger.Configuration.customizeHLTforMC
process = customizeHLTforMC(process)

# End of customisation functions

# Customisation from command line

# Add early deletion of temporary data products to reduce peak memory need
from Configuration.StandardSequences.earlyDeleteSettings_cff import customiseEarlyDelete
process = customiseEarlyDelete(process)
# End adding early deletion
