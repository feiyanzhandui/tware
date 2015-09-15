#!/bin/bash
#
# Args:
#   1) numWorkers
#
g++ -std=c++11 -O3 -ffast-math -mavx Master.cpp -o master -lpthread -lzmq
if [ $? -eq 0 ]; then
    ./master $1
fi
