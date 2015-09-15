#!/usr/bin/python
import os
import sys
from subprocess import call

def main():
    MAX = 25
    COMPILE = "g++ -std=c++11 -O3 -ffast-math -mavx alloc.cpp -o tmp"
    CONST = """
#define DATA 1?
#define TILE 2?

#define TILES DATA / TILE"""

    for i in range(0, 26):
        fconst = open("const.h", "w")
        fconst.write(CONST.replace("1?", str(2**MAX), 1)
                          .replace("2?", str(2**i), 1))
        fconst.close()
        call([COMPILE], shell=True)
        call(["./tmp"], shell=True)

if __name__ == "__main__":
    main()
