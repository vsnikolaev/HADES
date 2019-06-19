#!/bin/bash

hadesroot=/cvmfs/hades.gsi.de/install/6.12.06/hydra2-4.9w/defall.sh 

current_dir=$(pwd)
partition=main
time=8:00:00

curentmod=${2}
if [ $curentmod -eq 2 ]
then
mkdir -p f/log
log_dir=f/log
else
mkdir -p r/log
log_dir=r/log
fi

file_list=${1}

n_runs=$(cat $file_list | wc -l)

job_range=1-$n_runs

echo executing job range $job_range

sbatch -J resolution -p $partition -t $time -a $job_range -e ${log_dir}/%A_%a.e -o ${log_dir}/%A_%a.o --export=file_list=$file_list,hadesroot=$hadesroot,curentmod=$curentmod batch_run.sh