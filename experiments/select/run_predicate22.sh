#!/bin/bash
./run.py -w predicate -p bitmap -e branch -a block -n 2 -c 2 -i $1
./run.py -w predicate -p bitmap -e branch -a exact -n 2 -c 2 -i $1
./run.py -w predicate -p bitmap -e branch -a max -n 2 -c 2 -i $1
./run.py -w predicate -p bitmap -e nobranch -a block -n 2 -c 2 -i $1
./run.py -w predicate -p bitmap -e nobranch -a exact -n 2 -c 2 -i $1
./run.py -w predicate -p bitmap -e nobranch -a max -n 2 -c 2 -i $1
./run.py -w predicate -p none -e branch -a block -n 2 -c 2 -i $1
./run.py -w predicate -p none -e branch -a max -n 2 -c 2 -i $1
./run.py -w predicate -p none -e nobranch -a block -n 2 -c 2 -i $1
./run.py -w predicate -p none -e nobranch -a max -n 2 -c 2 -i $1
paste -d"," results/predicate/*block_22.tsv > results/block_22.csv
paste -d"," results/predicate/*exact_22.tsv > results/exact_22.csv
paste -d"," results/predicate/*max_22.tsv > results/max_22.csv
