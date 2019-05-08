#/bin/bash
# https://twiki.cern.ch/twiki/bin/view/Main/BatchJobs#JobSub

#jq=8nm
#jq=8nh
jq=1nd
#jq=2nd

#echo $1
#echo $2
#echo $3
#echo $4
#echo $5
#echo $6
#echo $7
#echo $8
#echo $9

debug=echo
bsub -R "pool>30000" -q $jq -J $9 run.sh $1 $2 $3 $4 $5 $6 $7 $8
#./run.sh $1 $2 $3 $4 $5 $6 $7 $8  
sleep 1
