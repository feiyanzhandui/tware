#!/bin/bash
#
# Args:
#   1) numWorkers
#   2) ordersFile
#   3) lineitemFile
#   4) outFile
#   5) masterAddr
#   6) workerAddrs
#
g++ -std=c++11 -O3 -ffast-math -mavx Worker.cpp -o worker -lpthread -lzmq
if [ $? -eq 0 ]; then
    ./worker $1 $2 $3 $4 $5 $6
fi
