#!/bin/bash

format='+%Y/%m/%d-%H:%M:%S'

date $format

job_num=$(($SLURM_ARRAY_TASK_ID))
input_file=`sed -n "${job_num}p" < $file_list`
file="$input_file"

echo "loading " $hadesroot
source $hadesroot

echo executing $root -l "rootlogon.c" "reader_resolution.cpp($file)"

$root -l "rootlogon.c" "reader_resolution.cpp($file, $out_file)" 
echo JOB FINISHED!
date $format