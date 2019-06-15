#!/bin/bash

format='+%Y/%m/%d-%H:%M:%S'

date $format

job_num=$(($SLURM_ARRAY_TASK_ID))
output_file=$output_dir/$job_num
input_file=`sed -n "${job_num}p" < $file_list`

echo "loading " $hadesroot
source $hadesroot

echo "executing $root -l rootlogon.c 'reader_resolution.cpp(file)' $cmd --signal $signal --perchannel $channelSelection --min $minSignal --max $maxSignal --pid $pidCode $input_file $output_file"

$root -l rootlogon.c 'reader_resolution.cpp(file)' $cmd --signal $signal --perchannel $channelSelection --min $minSignal --max $maxSignal --pid $pidCode $input_file $output_file

echo JOB FINISHED!
date $format