#!/bin/bash
./run.py -w predicate -p bitmap -e branch -a block -n 8 -c 8 -i $1
./run.py -w predicate -p bitmap -e branch -a exact -n 8 -c 8 -i $1
./run.py -w predicate -p bitmap -e branch -a max -n 8 -c 8 -i $1
./run.py -w predicate -p bitmap -e nobranch -a block -n 8 -c 8 -i $1
./run.py -w predicate -p bitmap -e nobranch -a exact -n 8 -c 8 -i $1
./run.py -w predicate -p bitmap -e nobranch -a max -n 8 -c 8 -i $1
./run.py -w predicate -p none -e branch -a block -n 8 -c 8 -i $1
./run.py -w predicate -p none -e branch -a max -n 8 -c 8 -i $1
./run.py -w predicate -p none -e nobranch -a block -n 8 -c 8 -i $1
./run.py -w predicate -p none -e nobranch -a max -n 8 -c 8 -i $1
paste -d"," results/predicate/*block_88.tsv > results/block_88.csv
paste -d"," results/predicate/*exact_88.tsv > results/exact_88.csv
paste -d"," results/predicate/*max_88.tsv > results/max_88.csv
