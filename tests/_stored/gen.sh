#!/bin/bash
NUM_REC=$2
for i in `seq 0 $1`;
do
    ((BEG=$NUM_REC*i))
    /home/ubuntu/gensort -t12 -b$BEG $NUM_REC "out""$i"
done
