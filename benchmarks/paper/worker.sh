#!/bin/bash
#
# Args:
#   1) benchmark
#   2) algo
#   3) numWorkers
#   4) localAddr
#   5) masterAddr
#   6) files
#
g++ -std=c++11 -O3 -ffast-math -mavx Worker.cpp -o worker -lpthread -lzmq
./worker $1 $2 $3 $4 $5 $6
