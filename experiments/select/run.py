#!/usr/bin/python
import argparse
import os
from subprocess import call

def main():
    DATA = 33554432
    TILE = 1024
    COMPILE = "g++ -std=c++11 -O3 -ffast-math -mavx 1?.cpp -o tmp"
    CONST = """
#define DATA 1?
#define TILE 2?
#define LOAD 3?
#define PVAR 4?

#define PMAX 100
#define TILES DATA / TILE
#define EXTRA DATA % TILE
#define APPROX DATA * PVAR
#define REMAIN DATA - APPROX"""

    parser = argparse.ArgumentParser()
    parser.add_argument("-w", "--workload", required=True)
    parser.add_argument("-p", "--prepass",  required=True)
    parser.add_argument("-e", "--eval",     required=True)
    parser.add_argument("-a", "--alloc",    required=True)
    parser.add_argument("-n", "--numpred",  default="1")
    parser.add_argument("-c", "--numcols",  default="1")
    parser.add_argument("-d", "--data",     default=DATA)
    parser.add_argument("-t", "--tile",     default=TILE)
    parser.add_argument("-l", "--load",     default=4)
    parser.add_argument("-i", "--interval", default=10)
    args = parser.parse_args()

    experiment = args.numpred + args.numcols
    algo = args.prepass + "_" + args.eval + "_" + args.alloc + "_" + experiment
    algodir = args.workload + "/" + experiment + "/" + args.prepass + "/"
    resdir = "results/" + args.workload + "/"
    resfile = resdir + algo + ".tsv"
    if not os.path.exists(resdir):
        os.makedirs(resdir)
    fres = open(resfile, "a")
    fres.write(algo + "\n")
    fres.close()

    for i in ([1] + range(0, 101, int(args.interval))[1:-1] + [99]):
        fconst = open("const.h", "w")
        fconst.write(CONST.replace("1?", str(args.data), 1)
                          .replace("2?", str(args.tile), 1)
                          .replace("3?", str(args.load), 1)
                          .replace("4?", str(i) + ".0f", 1))
        fconst.close()
        call([COMPILE.replace("1?", algodir + algo)], shell=True)
        call(["./tmp 2>> " + resfile], shell=True)

if __name__ == "__main__":
    main()
