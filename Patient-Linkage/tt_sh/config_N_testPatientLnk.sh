#!/usr/bin/env bash

if [ ! $# -eq 1 ]; then
  echo "format: ./run.sh num_parties "
  echo "example: ./run.sh 2"
  exit
fi

num_parties=$1
input_dir="config_patientLnk"

echo "====== #parties = $num_parties .. Directory = $input_dir"


#execute

COUNTER=1
while [ $COUNTER -le $num_parties ]; do
    echo "===== [DEBUG] run party: $COUNTER -- $input_dir/configs${COUNTER}.txt ===="
    #no windows
    ##.mpc.exe $input_dir/configs${COUNTER}.txt &
    #with windows, but close automatically at the end
    #gnome-terminal -e "bash -c \"!!; ./mpc.exe $input_dir/configs${COUNTER}.txt  \""  &
    #with windows, but need to close manually at the end
    xfce4-terminal -e "bash -c \" ./mpc.exe $input_dir/configs${COUNTER}.txt; read -p 'click anything to close the window'  \""  &
    sleep 4.0
    let COUNTER=COUNTER+1
done


