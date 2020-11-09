## Dump  10  events in CSC rechit builder - Tim Cox - 07.11.2012
## This version runs in 6_0_1_PostLS1 on a simulated data DIGI sample.

import FWCore.ParameterSet.Config as cms

process = cms.Process("RECO")

process.load("Configuration.Geometry.GeometryIdeal_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.load("Configuration.StandardSequences.RawToDigi_Data_cff")
process.load("Configuration.StandardSequences.Reconstruction_cff")
process.load("Configuration.StandardSequences.EndOfProcess_cff")
process.load('Configuration.StandardSequences.Services_cff')

##################################################
# --- MATCH GT TO RELEASE AND DATA SAMPLE
#process.GlobalTag.globaltag = "POSTLS161_V11::All"
process.GlobalTag.globaltag = "START53_V19D::All"
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
isSIMDIGI = bool(False)
isRAW = bool(True)
###################################################


process.options   = cms.untracked.PSet( SkipEvent = cms.untracked.vstring("ProductNotFound") )
process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )
process.source    = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(

#'file:./RelValJpsiMM_GEN-SIM-DIGI-RAW-HLTDEBUG_PRE_ST62_V8-v1/7412617A-E2E0-E211-8DB9-003048FEADCC.root',
#'file:./RelValJpsiMM_GEN-SIM-DIGI-RAW-HLTDEBUG_PRE_ST62_V8-v1/F0558878-E4E0-E211-8D59-02163E007A13.root'
#'file:/cms/data/store/mc/Summer13dr53X/DYToMuMu_M_20_TuneZ2star_13TeV-pythia6/GEN-SIM-RAW/PU25bx25_START53_V19D-v1/20000/BAB7C472-6ADF-E211-8702-20CF3027A5E9.root'

    )
)

# ME1/1A is  u n g a n g e d  Post-LS1
process.CSCGeometryESModule.useGangedStripsInME1a = True
##process.CSCGeometryESModule.debugV = True
##process.idealForDigiCSCGeometry.useGangedStripsInME1a = False

# Turn off some flags for CSCRecHitD that are turned ON in default config
process.csc2DRecHits.readBadChannels = cms.bool(False)
process.csc2DRecHits.CSCUseTimingCorrections = cms.bool(False)
process.csc2DRecHits.CSCUseGasGainCorrection = cms.bool(False)

# Switch input for CSCRecHitD to  s i m u l a t e d  digis
process.csc2DRecHits.wireDigiTag  = cms.InputTag("simMuonCSCDigis","MuonCSCWireDigi")
process.csc2DRecHits.stripDigiTag = cms.InputTag("simMuonCSCDigis","MuonCSCStripDigi")


process.out = cms.OutputModule("PoolOutputModule",
                               fastCloning = cms.untracked.bool(False),
                               fileName = cms.untracked.string('/cms/data/store/user/patTuples/CSC/mc/DYToMuMu_13TeV_GSR_localRECO.root'),
                               outputCommands = cms.untracked.vstring('keep *')
                               )


# --- TO ACTIVATE LogTrace IN CSCRecHitD NEED TO COMPILE IT WITH scram b -j8 USER_CXXFLAGS="-DEDM_ML_DEBUG"
# LogTrace output goes to cout; all other output to "junk.log"

process.load("FWCore.MessageLogger.MessageLogger_cfi")
##process.MessageLogger.categories.append("CSCGeometry")
process.MessageLogger.categories.append("CSCRecHit")
process.MessageLogger.categories.append("CSCRecHitDBuilder")
process.MessageLogger.categories.append("CSCMake2DRecHit")
process.MessageLogger.categories.append("CSCHitFromStripOnly")
## process.MessageLogger.categories.append("CSCRecoConditions")
# module label is something like "muonCSCDigis"...
process.MessageLogger.debugModules = cms.untracked.vstring("*")
process.MessageLogger.destinations = cms.untracked.vstring("cout","junk")
process.MessageLogger.cout = cms.untracked.PSet(
    threshold = cms.untracked.string("DEBUG"),
    default   = cms.untracked.PSet( limit = cms.untracked.int32(0)  ),
    FwkReport = cms.untracked.PSet( limit = cms.untracked.int32(-1) )
##    , CSCGeometry = cms.untracked.PSet( limit = cms.untracked.int32(-1) )
    , CSCRecHit = cms.untracked.PSet( limit = cms.untracked.int32(-1) )
    , CSCRecHitDBuilder = cms.untracked.PSet( limit = cms.untracked.int32(-1) )
    , CSCMake2DRecHit = cms.untracked.PSet( limit = cms.untracked.int32(-1) )
    , CSCHitFromStripOnly = cms.untracked.PSet( limit = cms.untracked.int32(-1) )
##    , CSCRecoConditions = cms.untracked.PSet( limit = cms.untracked.int32(-1) )
)



# Path and EndPath def
process.unpack = cms.Path(process.muonCSCDigis * process.gtDigis)
process.reco = cms.Path(process.csc2DRecHits * process.cscSegments )
#process.reco = cms.Path(process.reconstruction)
process.out_step = cms.EndPath(process.out)

# Schedule definition
process.schedule = cms.Schedule(process.reco, process.out_step)

if isRAW:
    process.reco.replace(process.csc2DRecHits,process.muonCSCDigis * process.gtDigis * process.csc2DRecHits * process.cscSegments)
