#!/bin/bash

FilesPerJob=5

if [ "$1" == "" ]; then
    echo "Please pass a txt file with root files to be processed!"
    exit 1;
fi

if [ "$#" == "2" ]; then
    FilesPerJob=$2
fi
echo "$FilesPerJob files to be processed per job"

dir=${1%%.txt*}

if [ ! -d "Submission_${dir}" ]
    then mkdir Submission_${dir}
else
    echo Submission_${dir} exists!
    exit 1;
fi


txtFile=${1%%.txt*}
workDir=Submission_${dir}
nLines=$(cat $1 | wc -l)

tmpCounter=1
jobCounter=0
linesLeft=$nLines

while read line
  do
  
  echo $line >> Submission_${dir}/${dir}_$jobCounter.txt

  let tmpCounter=$tmpCounter+1
  let linesLeft=$linesLeft-1
  
  if [ "$tmpCounter" == "$FilesPerJob" ];then let jobCounter=$jobCounter+1; tmpCounter=0; fi


done < $1




