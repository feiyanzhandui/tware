#!/usr/bin/python
import argparse
import os
import sys
from subprocess import call

def main():
    DATA = 33554432
    TILE = 1024
    COMPILE = "g++ -std=c++11 -O3 -ffast-math -mavx 1?.cpp -o 1?"
    CONST = """
#define DATA 1?
#define TILE 2?
#define KEYS 3?

#define TILES (DATA / TILE)
#define EXTRA (DATA % TILE)"""

    algo1 = "hash"
    algo2 = "vhash"
    for i in range(1, 7):
        fconst = open("const.h", "w")
        fconst.write(CONST.replace("1?", str(DATA), 1)
                          .replace("2?", str(TILE), 1)
                          .replace("3?", str(10**i), 1))
        fconst.close()
        call([COMPILE.replace("1?", algo1)], shell=True)
        call(["./" + algo1 + " 2>> " + algo1 + ".tsv"], shell=True)
        call([COMPILE.replace("1?", algo2)], shell=True)
        call(["./" + algo2 + " 2>> " + algo2 + ".tsv"], shell=True)

if __name__ == "__main__":
    main()
