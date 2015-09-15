#!/usr/bin/python
import argparse
import math
import sys

def dot(b, x):
    return reduce(lambda n,m: n + m, map(lambda i: b[i] + x[i], range(len(b))))

def train(B, x):
    a = 0.005
    for i in range(len(B)):
        d = dot(B[i], x[0])
        k = float(x[1] == i) - float(x[1] != i)
        Q = (1.0 / (1.0 + math.exp(-k * d)) - 1.0) * k
        B[i] = [B[i][j] - a * Q * x[0][j] for j in range(len(x[0]))]
    return B

def test(B, x, K):
    ds = [math.exp(dot(b, x[0])) for b in B]
    total = 1 + sum(ds)
    b = max([(ds[k] / total,k) for k in range(len(ds))] + [(1 / total,len(ds))])
    print b
    key = x[1]
    val = (0,{})
    if x[1] in K:
        val = K[key]
    else:
        K[key] = val
    if b[1] in val[1]:
        val[1][b[1]] += 1
    else:
        val[1][b[1]] = 1
    K[key] = (val[0] + 1, val[1])
    return K

def main():
    #parser = argparse.ArgumentParser()
    #parser.add_argument("-c", "--classifier", required=True)
    #parser.add_argument("-f", "--file", required=True)
    #parser.add_argument()
    X = []
    for line in open("test.data"):
        vals = line.split(",")
        X += [([float(x) for x in vals[:-1]],int(vals[-1]))]
    B = [[0.5 for k in X[0][0]] for k in range(2)]
    K = {}
    print B
    for i in range(10000):
        for x in X:
            B = train(B, x)
    print B
    for x in X:
        K = test(B, x, K)
    print K

if __name__ == "__main__":
    main()
