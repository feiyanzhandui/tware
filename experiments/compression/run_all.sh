#!/bin/bash

./run.py -c predicate -s batch -a block -n 1
#./run.py -c predicate -s batch -a block -n 2
#./run.py -c predicate -s batch -a block -n 4
./run.py -c predicate -s batch -a block -n 8
./run.py -c predicate -s batch -a max -n 1
#./run.py -c predicate -s batch -a max -n 2
#./run.py -c predicate -s batch -a max -n 4
./run.py -c predicate -s batch -a max -n 8

./run.py -c predicate -s branch -a block -n 1
#./run.py -c predicate -s branch -a block -n 2
#./run.py -c predicate -s branch -a block -n 4
./run.py -c predicate -s branch -a block -n 8
#./run.py -c predicate -s branch -a buffer -n 1
./run.py -c predicate -s branch -a max -n 1
#./run.py -c predicate -s branch -a max -n 2
#./run.py -c predicate -s branch -a max -n 4
./run.py -c predicate -s branch -a max -n 8
./run.py -c predicate -s branch -a tuple -n 1
#./run.py -c predicate -s branch -a tuple -n 2
#./run.py -c predicate -s branch -a tuple -n 4
./run.py -c predicate -s branch -a tuple -n 8

#./run.py -c predicate -s nobranch -a buffer -n 1

./run.py -c predicate -s nobranch -a block -n 1
#./run.py -c predicate -s nobranch -a block -n 2
#./run.py -c predicate -s nobranch -a block -n 4
./run.py -c predicate -s nobranch -a block -n 8
./run.py -c predicate -s nobranch -a max -n 1
#./run.py -c predicate -s nobranch -a max -n 2
#./run.py -c predicate -s nobranch -a max -n 4
./run.py -c predicate -s nobranch -a max -n 8

./run.py -c predicate -s special -a block -n 1
#./run.py -c predicate -s special -a block -n 2
#./run.py -c predicate -s special -a block -n 4
./run.py -c predicate -s special -a block -n 8

#paste -d"," results/*1.tsv > results/results1.csv
#paste -d"," results/*2.tsv > results/results2.csv
#paste -d"," results/*4.tsv > results/results4.csv
#paste -d"," results/*8.tsv > results/results8.csv
#rm *.tsv
