#!/bin/bash
g++-4.9 -std=c++11 -O3 -ffast-math -fno-tree-vectorize $1.cpp -o $1; ./$1 $2 $3
