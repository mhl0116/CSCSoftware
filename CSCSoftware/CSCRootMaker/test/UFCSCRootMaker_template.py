######################################################################
#                                                                    #
# This version works with CMSSW_6_2_X                                #
#                                                                    #
######################################################################
import FWCore.ParameterSet.Config as cms
doUnpacking = bool(True)

########## Options ############
isDATA = bool(True)
isRAW = bool(True)
isDIGI = bool(True)

isSIM = bool(False)
isGEN = bool(False)

isLocalRECO = bool(True)
isFullRECO = bool(True)

addMuonInfo = bool(True)
addTrackInfo = bool(True)
addRecHitInfo = bool(True)
addSegmentInfo = bool(True)
addTriggerInfo = bool(False)

addDigiInfo = bool(True)

addTimeMonitoringInfo = bool(True)
addCalibrationInfo = bool(True)

maxEvents = -1

#MCGlobalTag='PH2_1K_FB_V6::All' for DYmumu_PU140
#DataGlobalTag='76X_dataRun2_v19'
#DataGlobalTag='76X_dataRun2_v15'
DataGlobalTag='92X_dataRun2_Prompt_v11'
doDebug = bool(False)
###############################

### Debug Printing ###
if not isDATA :
    print "Sample Type: MC"
else :
    print "Sample Type: Data"
                
#####################
process = cms.Process("UFCSCRootMaker")

process.Timing = cms.Service("Timing",
                             summaryOnly = cms.untracked.bool(True)
                             )

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(maxEvents))

# initialize MessageLogger and output report
process.load("FWCore.MessageLogger.MessageLogger_cfi")
#process.MessageLogger.cerr.threshold = 'ERROR' # Options: INFO, WARNING, ERROR
process.MessageLogger.cerr.FwkReport.reportEvery = 1000
process.MessageLogger.destinations = ['cout', 'cerr']
process.MessageLogger.suppressWarning.append('classByHitsGlb') # kill stupid RPC hit associator warning
#process.MessageLogger.cerr.FwkJob.limit=1
#process.MessageLogger.cerr.ERROR = cms.untracked.PSet( limit = cms.untracked.int32(1))
                                                       
######################### Frontier Conditions #########################
# Conditions data for calibration and alignment                       #
# are defined in the Offline Conditions Database (ORCOF),             #
# which is read in CMSSW applications via Frontier caching servers.   #
# https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideFrontierConditions  #
#######################################################################
#process.load('Configuration.StandardSequences.Services_cff')
#process.load('Configuration.StandardSequences.Geometry_cff')
#process.load('Configuration.StandardSequences.MagneticField_cff')
#process.load('Configuration.StandardSequences.Reconstruction_cff')
#process.load('Configuration.StandardSequences.EndOfProcess_cff')
#process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
#process.load('Configuration.EventContent.EventContent_cff')
###

process.load('Configuration.Geometry.GeometryRecoDB_cff')

#process.load("Configuration/StandardSequences/Geometry_cff")
process.load("Configuration/StandardSequences/MagneticField_cff")
process.load("Configuration/StandardSequences/FrontierConditions_GlobalTag_cff")
process.load("CondCore.CondDB.CondDB_cfi")
process.load("Configuration/StandardSequences/RawToDigi_Data_cff")
process.load("Configuration.StandardSequences.Reconstruction_cff")
###
if not isDATA:
    process.GlobalTag.globaltag=MCGlobalTag 
    
else:
    process.GlobalTag.globaltag=DataGlobalTag


process.out = cms.OutputModule("PoolOutputModule",
                               #fileName = cms.untracked.string('test.root'),
                               # save only events passing the full path
                               outputCommands = cms.untracked.vstring('drop *')
                               )


process.TFileService = cms.Service("TFileService",
#                                   fileName = cms.string("DUMMYFILENAME.root")
                                   fileName = cms.string("DUMMYFILENAME.root")
                                   )

# Primary Vertices
from PhysicsTools.SelectorUtils.pvSelector_cfi import pvSelector
process.goodOfflinePrimaryVertices = cms.EDFilter("VertexSelector",
                                                  src = cms.InputTag('offlinePrimaryVertices'),
                                                  cut = cms.string('!isFake && ndof > 4 && abs(z) <= 24 && position.Rho <= 2'),
                                                  filter = cms.bool(True)
                                                  )


process.source = cms.Source ("PoolSource",
                             # Disable duplicate event check mode because the run and event -numbers
                             # are incorrect in current Madgraph samples (Dec 16, 2008)
                             # processingMode = cms.untracked.string('RunsAndLumis'),
                             duplicateCheckMode = cms.untracked.string('noDuplicateCheck'),
                             fileNames = cms.untracked.vstring(),      
                             )

if isDATA:
    process.source.fileNames = cms.untracked.vstring(DUMMYFILELIST
# 'root://cmsxrootd.fnal.gov//store/data/Run2015D/SingleMuon/RAW-RECO/ZMu-16Dec2015-v1/10000/005D37B2-3CA9-E511-B9AF-001E67398223.root',
# 'root://cmsxrootd.fnal.gov//store/data/Run2015D/SingleMuon/RAW-RECO/ZMu-16Dec2015-v1/10000/005D37B2-3CA9-E511-B9AF-001E67398223.root',
# 'root://cmsxrootd.fnal.gov//store/data/Run2015D/SingleMuon/RAW-RECO/ZMu-16Dec2015-v1/10000/005D37B2-3CA9-E511-B9AF-001E67398223.root',
# 'root://cmsxrootd.fnal.gov//store/data/Run2015D/SingleMuon/RAW-RECO/ZMu-16Dec2015-v1/10000/005D37B2-3CA9-E511-B9AF-001E67398223.root',
# 'root://cmsxrootd.fnal.gov//store/data/Run2015D/SingleMuon/RAW-RECO/ZMu-16Dec2015-v1/10000/005D37B2-3CA9-E511-B9AF-001E67398223.root',
#'root://cmsxrootd.fnal.gov//store/data/Run2016C/SingleMuon/RECO/PromptReco-v2/000/275/657/00000/00DFFD18-693B-E611-BDF2-02163E0140F2.root'
        )
else:
    process.source.fileNames = cms.untracked.vstring(DUMMYFILELIST)
    process.source.fileNames.extend( [
#        '/store/relval/CMSSW_7_0_0/RelValTTbar/GEN-SIM-DIGI-RECO/START70_V6_FastSim-v2/00000/00743452-B498-E311-AD84-02163E00EAC9.root',
#        'file:/cms/data/store/data/Run2012B/SingleMu/RECO/22Jan2013-v1/20040/F04E9749-2D74-E211-9EE4-E0CB4E19F9BC.root'
        'root://cmsxrootd.fnal.gov//store/group/upgrade/muon/ME0GlobalReco/ME0MuonReRun_DY_SLHC23patch1_SegmentReRunFullRun_ForPublish/M-20_TuneZ2star_14TeV_6_2_0_SLHC23patch1_2023/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_2023SHCalNoTaper_PU140_Selectors_RECO/b52ce42d5986c94dc336f39e015d825e/output_100_2_p9i.root'
        ]
    )
    
process.options = cms.untracked.PSet(wantSummary = cms.untracked.bool(False)
                                     #,SkipEvent = cms.untracked.vstring('ProductNotFound')
                                     )

### Physics Declared Filter (for data)
process.load('HLTrigger.special.hltPhysicsDeclared_cfi')
process.hltPhysicsDeclared.L1GtReadoutRecordTag = 'gtDigis'

### No scraping
process.noscraping = cms.EDFilter("FilterOutScraping",
                                  applyfilter = cms.untracked.bool(True),
                                  debugOn = cms.untracked.bool(doDebug),
                                  numtrack = cms.untracked.uint32(10),
                                  thresh = cms.untracked.double(0.2)
                                  )


#process.LumiCorrectionSource = cms.ESSource("LumiCorrectionSource",
                                            #authpath=cms.untracked.string('/afs/cern.ch'file:/cms/lumi/DB'),
                                            #connect=cms.string('oracle:/'file:/cms_orcon_adg'file:/cms_lumi_prod')
#                                            connect=cms.string('frontier://LumiCalc_LUMI_PROD')
                                            #normtag=cms.untracked.string('HFV2a')
                                            #datatag=cms.untracked.string('v3')
#                                            )


# HB + HE noise filtering
#process.load('CommonTools.RecoAlgos.HBHENoiseFilter_cfi')
#process.HBHENoiseFilter.minIsolatedNoiseSumE        = 999999.
#process.HBHENoiseFilter.minNumIsolatedNoiseChannels = 999999
#process.HBHENoiseFilter.minIsolatedNoiseSumEt       = 999999.

#EventCount                                                                                                                             
process.nEventsTotal = cms.EDProducer("EventCountProducer")

#RM
process.load("UFCSCSoftware.UFCSCRootMaker.cscRootMaker_cfi")
process.cscRootMaker.isFullRECO = cms.untracked.bool(isFullRECO)
process.cscRootMaker.isLocalRECO = cms.untracked.bool(isLocalRECO)
process.cscRootMaker.isGEN = cms.untracked.bool(isGEN)
process.cscRootMaker.isSIM = cms.untracked.bool(isSIM)
process.cscRootMaker.isRAW = cms.untracked.bool(isRAW)
process.cscRootMaker.isDIGI = cms.untracked.bool(isDIGI)
process.cscRootMaker.isDATA = cms.untracked.bool(isDATA)
process.cscRootMaker.addMuons = cms.untracked.bool(addMuonInfo)
process.cscRootMaker.addTracks = cms.untracked.bool(addTrackInfo)
process.cscRootMaker.addRecHits = cms.untracked.bool(addRecHitInfo)
process.cscRootMaker.addSegments = cms.untracked.bool(addSegmentInfo)
process.cscRootMaker.addTrigger = cms.untracked.bool(addTriggerInfo)
process.cscRootMaker.addDigis = cms.untracked.bool(addDigiInfo)
process.cscRootMaker.addTimeMonitoring = cms.untracked.bool(addTimeMonitoringInfo)
process.cscRootMaker.addCalibrations = cms.untracked.bool(addCalibrationInfo)

process.unpack = cms.Sequence(process.muonCSCDigis*process.gtDigis)

process.p = cms.Path(
    process.nEventsTotal
#    *process.goodOfflinePrimaryVertices
#    *process.noscraping
    *process.cscRootMaker
    )

if not isDATA:
    process.load("GeneratorInterface.GenFilters.TotalKinematicsFilter_cfi")
    process.totalKinematicsFilter.tolerance=5.0
    process.p.replace(process.nEventsTotal,process.nEventsTotal*process.totalKinematicsFilter)

if isDATA:
    process.p.replace(process.nEventsTotal,process.nEventsTotal)
#    process.p.replace(process.nEventsTotal,process.hltPhysicsDeclared*process.nEventsTotal)

if doUnpacking:
    process.p.replace(process.nEventsTotal,process.unpack*process.nEventsTotal)

