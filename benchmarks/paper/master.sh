#!/bin/bash
#
# Args:
#   1) benchmark
#   2) algo
#   3) numWorkers
#   4) localWorkers
#   5) files
#
g++ -std=c++11 -O3 -ffast-math -mavx Master.cpp -o master -lpthread -lzmq
if [ $? -eq 0 ]; then
    ./master $1 $2 $3 $4 $5
fi
