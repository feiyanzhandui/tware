#!/bin/bash
./run.py -w udf -p increment -e adaptive -a exact -n 1 -c 1 -i $1
./run.py -w udf -p increment -e operator -a max -n 1 -c 1 -i $1
./run.py -w udf -p increment -e pipeline -a block -n 1 -c 1 -i $1
./run.py -w udf -p sqrt -e adaptive -a exact -n 1 -c 1 -i $1
./run.py -w udf -p sqrt -e operator -a max -n 1 -c 1 -i $1
./run.py -w udf -p sqrt -e pipeline -a block -n 1 -c 1 -i $1
paste -d"," results/udf/increment*.tsv > results/increment.csv
paste -d"," results/udf/sqrt*.tsv > results/sqrt.csv
