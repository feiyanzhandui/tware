#!/bin/bash
./run.py -w predicate -p bitmap -e branch -a block -n 1 -c 2 -i $1
./run.py -w predicate -p bitmap -e branch -a exact -n 1 -c 2 -i $1
./run.py -w predicate -p bitmap -e branch -a max -n 1 -c 2 -i $1
./run.py -w predicate -p bitmap -e nobranch -a block -n 1 -c 2 -i $1
./run.py -w predicate -p bitmap -e nobranch -a exact -n 1 -c 2 -i $1
./run.py -w predicate -p bitmap -e nobranch -a max -n 1 -c 2 -i $1
./run.py -w predicate -p none -e branch -a block -n 1 -c 2 -i $1
./run.py -w predicate -p none -e branch -a max -n 1 -c 2 -i $1
./run.py -w predicate -p none -e nobranch -a block -n 1 -c 2 -i $1
./run.py -w predicate -p none -e nobranch -a max -n 1 -c 2 -i $1
paste -d"," results/predicate/*block_12.tsv > results/block_12.csv
paste -d"," results/predicate/*exact_12.tsv > results/exact_12.csv
paste -d"," results/predicate/*max_12.tsv > results/max_12.csv
