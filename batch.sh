#!/bin/bash

hadesroot=/cvmfs/hades.gsi.de/install/6.12.06/hydra2-4.9w/defall.sh 

current_dir=$(pwd)
partition=main
time=8:00:00

log_dir=r/log

file_list=${1}


mkdir -p $r

n_runs=$(cat $file_list | wc -l)

job_range=1-$n_runs

echo executing job range $job_range

sbatch -J resolution -p $partition -t $time -a $job_range -e ${log_dir}/%A_%a.e -o ${log_dir}/%A_%a.o --export=executable=$executable,output_dir=$output_dir,file_list=$file_list,hadesroot=$hadesroot,cmd=$cmd,signal=$signal,channelSelection=$channelSelection,minSignal=$minSignal,maxSignal=$maxSignal,pidCode=$pidCode batch_run.sh