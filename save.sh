#!/bin/sh
# This is used for making buck up in Raspberry PI 3

rsync -auv /local/Analyzer/* rpi3:/VILLAGE/crabat/ 2>/dev/null
if [ $? -gt 0 ]
then
    rsync -auv /local/Analyzer/* rpi:/VILLAGE/crabat/
    printf "\e[31mSave it in rpi\e[m\n"
else
  printf "\e[31mSave it in rpi3\e[m\n"
fi
