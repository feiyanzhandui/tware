#!/bin/bash

#init
rm -rf results

#predicate
./run_predicate11.sh $1
./run_predicate12.sh $1
./run_predicate18.sh $1
./run_predicate22.sh $1
./run_predicate28.sh $1
./run_predicate88.sh $1

#udf
./run_udf.sh $1

#cleanup
rm results/*/*.tsv

#plot
rm -rf ~/figs
mkdir ~/figs
./plot.py results/ ~/figs
