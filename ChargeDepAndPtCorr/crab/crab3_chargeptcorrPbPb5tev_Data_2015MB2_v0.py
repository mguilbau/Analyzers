### this is an example for running on RECO
### the name must be changed crab.cfg for actual running

from CRABClient.UserUtilities import config, getUsernameFromSiteDB
config = config()

config.General.requestName = 'PbPb5TeVPixReco_2015_MB2_v0'
config.General.workArea = 'PbPb2015PixRecoAna_data'
config.General.transferOutputs = True
config.General.transferLogs = False
config.JobType.pluginName = 'Analysis'
config.JobType.psetName = '/afs/cern.ch/user/m/mguilbau/ForPrabhat/CMSSW_7_5_8_patch3/src/Analyzers/ChargeDepAndPtCorr/cfg/chargedepptcorr_cfg.py'

config.Data.inputDataset ='/HIMinimumBias2/HIRun2015-25Aug2016-v1/AOD'

config.Data.lumiMask = '/afs/cern.ch/user/m/mguilbau/ForPrabhat/CMSSW_7_5_8_patch3/src/Analyzers/ChargeDepAndPtCorr/data/Cert_262548-263757_PromptReco_HICollisions15_JSON_v2.txt'
config.Data.inputDBS = 'global'
config.Data.splitting = 'LumiBased'
config.Data.unitsPerJob = 10
config.Data.outLFNDirBase = '/store/user/%s/' % (getUsernameFromSiteDB())
config.Data.publication = False
config.Site.storageSite = 'T2_CH_CERN'
