#!/bin/bash

format='+%Y/%m/%d-%H:%M:%S'

date $format

job_num=$(($SLURM_ARRAY_TASK_ID))
input_file=`sed -n "${job_num}p" < $file_list`
file="$input_file"

echo "loading " $hadesroot
source $hadesroot

#echo executing $root -l ""reader_resolution.cpp""(""$file"")""

if [ $curent_mod -eq 2 ]
then
root -l -b "reader_flows.cpp(\"$input_file\",\"$output_file\")" 
else
root -l -b "reader_resolution.cpp(\"$input_file\",\"$output_file\")" 
fi
echo JOB FINISHED!
date $format