#!/bin/bash
echo "Generation config file for:"
echo "MB path ~~~> $1"
echo "Version ~~~> $2"

echo "### this is an example for running on RECO" > crab3_chargeptcorrPbPb5tev_Data_2015MB${1}_v${2}.py  
echo "### the name must be changed crab.cfg for actual running" >> crab3_chargeptcorrPbPb5tev_Data_2015MB${1}_v${2}.py 
echo "" >> crab3_chargeptcorrPbPb5tev_Data_2015MB${1}_v${2}.py 
echo "from CRABClient.UserUtilities import config, getUsernameFromSiteDB" >> crab3_chargeptcorrPbPb5tev_Data_2015MB${1}_v${2}.py 
echo "config = config()" >> crab3_chargeptcorrPbPb5tev_Data_2015MB${1}_v${2}.py 
echo "" >> crab3_chargeptcorrPbPb5tev_Data_2015MB${1}_v${2}.py 
echo "config.General.requestName = 'PbPb5TeVPixReco_2015_MB${1}_v${2}'" >> crab3_chargeptcorrPbPb5tev_Data_2015MB${1}_v${2}.py 
echo "config.General.workArea = 'PbPb2015PixRecoAna_data'" >> crab3_chargeptcorrPbPb5tev_Data_2015MB${1}_v${2}.py 
echo "config.General.transferOutputs = True" >> crab3_chargeptcorrPbPb5tev_Data_2015MB${1}_v${2}.py 
echo "config.General.transferLogs = False" >> crab3_chargeptcorrPbPb5tev_Data_2015MB${1}_v${2}.py 

echo "config.JobType.pluginName = 'Analysis'" >> crab3_chargeptcorrPbPb5tev_Data_2015MB${1}_v${2}.py 
if [ ${2} -eq 0 ]
then
  echo "config.JobType.psetName = '$CMSSW_BASE/src/Analyzers/ChargeDepAndPtCorr/cfg/chargedepptcorr_cfg.py'" >> crab3_chargeptcorrPbPb5tev_Data_2015MB${1}_v${2}.py
else
  echo "WARNING!!! unkown version!!!!"
fi

echo "" >> crab3_chargeptcorrPbPb5tev_Data_2015MB${1}_v${2}.py 
echo "config.Data.inputDataset ='/HIMinimumBias${1}/HIRun2015-25Aug2016-v1/AOD'" >> crab3_chargeptcorrPbPb5tev_Data_2015MB${1}_v${2}.py 
echo "" >> crab3_chargeptcorrPbPb5tev_Data_2015MB${1}_v${2}.py 
echo "config.Data.lumiMask = '$CMSSW_BASE/src/Analyzers/ChargeDepAndPtCorr/data/JSON/Cert_262548-263757_PromptReco_HICollisions15_JSON_v2.txt'" >> crab3_chargeptcorrPbPb5tev_Data_2015MB${1}_v${2}.py
echo "config.Data.inputDBS = 'global'" >> crab3_chargeptcorrPbPb5tev_Data_2015MB${1}_v${2}.py 
echo "config.Data.splitting = 'LumiBased'" >> crab3_chargeptcorrPbPb5tev_Data_2015MB${1}_v${2}.py 
echo "config.Data.unitsPerJob = 10" >> crab3_chargeptcorrPbPb5tev_Data_2015MB${1}_v${2}.py 
echo "config.Data.outLFNDirBase = '/store/user/%s/' % (getUsernameFromSiteDB())" >> crab3_chargeptcorrPbPb5tev_Data_2015MB${1}_v${2}.py 
#echo "config.Data.outLFNDirBase = '/store/group/phys_heavyions/flowcorr/'" >> crab3_chargeptcorrPbPb5tev_Data_2015MB${1}_v${2}.py 
echo "config.Data.publication = False" >> crab3_chargeptcorrPbPb5tev_Data_2015MB${1}_v${2}.py 
echo "config.Site.storageSite = 'T2_CH_CERN'" >> crab3_chargeptcorrPbPb5tev_Data_2015MB${1}_v${2}.py 
