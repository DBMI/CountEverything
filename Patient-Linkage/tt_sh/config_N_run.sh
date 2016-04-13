#!/usr/bin/env bash

if [ ! $# -eq 2 ]; then
  echo "format: ./run.sh num_parties num_inputs"
  echo "format: ./run.sh 4 10"
  exit
fi

num_parties=$1
num_input=$2
echo "num_parties=$num_parties"

input_dir="tt_input"

#config-auto
mkdir -p $input_dir
rm $input_dir/config*.txt
rm $input_dir/address.txt
port_init=6676

 #worker config
COUNTER=0
while [ $COUNTER -lt $num_input ]; do
    unit_input="1 "$unit_input
    worker_input="13 "$worker_input
    let COUNTER=COUNTER+1 
done

echo $worker_input > $input_dir/inputs1000.txt

COUNTER=1
while [ $COUNTER -lt $num_parties ]; do
    consumer_input=$unit_input""$consumer_input
    rm -f temp1 && cat > temp1 <<MYEOF
address-book	tt_input/address.txt
MYEOF
    rm -f temp2 && cat > temp2 <<MYEOF
input tt_input/inputs1000.txt

seed 4373984170943781378434349	

p 8936097950764538541647693880373941060412422053330581106416547022143872696986309618392492140173286901378451169263228782167065246660381840799235731486130087

g 7660915846360464914746169975675514711978996378800371841846530440188167304050642048045944447898172503094027848348928324282770185052123039167705188057761352
MYEOF

    echo " " >> $input_dir/configs${COUNTER}.txt
    echo "num_parties $num_parties" >> $input_dir/configs${COUNTER}.txt
    echo "pid $((COUNTER - 1))" >> $input_dir/configs${COUNTER}.txt
    cat temp1 >> $input_dir/configs${COUNTER}.txt
    echo "create-circuit p2p $num_input 65535" >> $input_dir/configs${COUNTER}.txt
    echo "" >> $input_dir/configs${COUNTER}.txt
    echo "num_input $num_input" >> $input_dir/configs${COUNTER}.txt
    cat temp2 >> $input_dir/configs${COUNTER}.txt
    #add address
    echo -e "$((COUNTER - 1))\t127.0.0.1\t$((port_init+COUNTER-1))" >> $input_dir/address.txt
    echo $COUNTER
    let COUNTER=COUNTER+1 
done

 #consumer config
echo $consumer_input > tt_input/inputc.txt
num_workers=$((num_parties - 1))
total_input=$((num_workers * num_input))
    rm -f temp1 && cat > temp1 <<MYEOF
address-book	tt_input/address.txt
MYEOF
    rm -f temp2 && cat > temp2 <<MYEOF
input tt_input/inputc.txt

seed 4373984170943781378434349	

p 8936097950764538541647693880373941060412422053330581106416547022143872696986309618392492140173286901378451169263228782167065246660381840799235731486130087

g 7660915846360464914746169975675514711978996378800371841846530440188167304050642048045944447898172503094027848348928324282770185052123039167705188057761352
MYEOF

    echo " " >> $input_dir/configc.txt
    echo "num_parties $num_parties" >> $input_dir/configc.txt
    echo "pid $((num_parties-1))" >> $input_dir/configc.txt
    cat temp1 >> $input_dir/configc.txt
    echo "create-circuit p2p $num_input 65535" >> $input_dir/configc.txt
    echo "" >> $input_dir/configc.txt
    echo "num_input $total_input" >> $input_dir/configc.txt
    cat temp2 >> $input_dir/configc.txt
    echo -e "$((num_parties-1))\t127.0.0.1\t$((port_init+num_parties-1))" >> $input_dir/address.txt

sleep 0.5

#execute
COUNTER=1
while [ $COUNTER -lt $num_parties ]; do
    echo "run party-$COUNTER"
    xfce4-terminal -e "bash -c \"!!; ./mpc.exe $input_dir/configs${COUNTER}.txt  \""  &
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

