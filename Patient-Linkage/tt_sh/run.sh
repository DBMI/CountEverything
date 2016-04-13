#!/usr/bin/env bash

if [ ! $# -eq 2 ]; then
  echo "format: ./run.sh num_parties config_dir"
  echo "example: ./run.sh 4 tt_input"
  exit
fi

num_parties=$1
input_dir=$2

#execute
COUNTER=1
while [ $COUNTER -lt $num_parties ]; do
    echo "run party-$COUNTER"
    #no windows
    ./mpc.exe $input_dir/configs${COUNTER}.txt &
    #with windows, but close automatically at the end
    #gnome-terminal -e "bash -c \"!!; ./mpc.exe $input_dir/configs${COUNTER}.txt  \""  &
    #with windows, but need to close manually at the end
    #gnome-terminal -e "bash -c \"!!; ./mpc.exe $input_dir/configs${COUNTER}.txt; read -p 'click anything to close the window'  \""  &
    sleep 0.25
    let COUNTER=COUNTER+1 
done

xfce4-terminal -e "bash -c \"
begin=\$(date +%s.%N);
./mpc.exe $input_dir/configc.txt; 
end=\$(date +%s.%N);
duration=\$(echo \"\$end-\$begin\"|bc);
echo \"\${duration}-sec\";
read -p 'click anything to close the window' \""

