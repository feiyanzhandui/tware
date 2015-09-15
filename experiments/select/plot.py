#!/usr/bin/python
import csv
import matplotlib
import numpy as np
import os
import sys

matplotlib.use("Agg")
import matplotlib.pyplot as plt

def main():
    indir = sys.argv[1]
    outdir = sys.argv[2]
    ls = ['-','--','-.',':','-','--','-.',':','-','--','-.',':',]
    for f in os.listdir(indir):
        if f.endswith(".csv"):
            lines = open(indir + f,'r').readlines()
            cols = [[] for i in range(0,len(lines[0].split(',')))]
            for line in lines[1:]:
                parts = line.split(',')
                for i in range(0,len(parts)):
                    cols[i].append(parts[i])
            x = range(0, 101, 100 / (len(lines) - 2))
            i = 0
            for col in cols:
                plt.plot(x, col, linewidth=2.0, linestyle=ls[i])
                i += 1
            plt.legend(lines[0].split(","), loc=2, prop={'size':10})
            fig = plt.gcf()
            fig.set_size_inches(11, 9)
            plt.xlabel('Selectivity', fontsize=12)
            plt.ylabel('Runtime (s)', fontsize=12)
            fig.savefig(outdir + "/" + f.replace(".csv", ".png"), dpi = 300)
            fig.clf()

if __name__ == "__main__":
    main()
