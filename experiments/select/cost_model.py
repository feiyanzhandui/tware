import math

EPS = 1
D = 33554432
N = 8
B = 1024
C1 = 15
C2 = 5
C3 = 10
C4 = 0.435
C5 = 20

def main():
    print "\n\n"
    for p,a in [(1,1), (1,2), (1,4), (2,2), (1,8), (8,8)]:
        for s in [0.01] + [x / 10.0 for x in range(1, 10)] + [.99]:
            #for m in [x / 100.0 for x in range(0, 101, 10)]:
                m = .01
                best = (None, float("inf"))
                for e in ['branch', 'nobranch']:
                    for (r,b) in [('block',B), ('max',D), ('exact',B)]:
                        if b*(1-s)/D <= m:
                            strat = (e,r,b)
                            c = cost_model(strat, p, a, s)
                            if c < best[1]:
                                best = (strat, c)
                print str((p,a,s,m)) + ": " + str(best)

def cost_model((e,r,b), p, a, s):
    t1 = 0
    if e == 'branch':
        t1 = C1*1/(2*math.fabs(s-0.5)+EPS)
    else:
        t1 = C2*((p-1)/(s*N)) + C3*(a-p)*(1-s)
    
    t2 = 2**(2*math.log(C4*math.ceil(D/b))/math.log(2))/D
    if r == 'exact':
        t2 += C5*s*D/(b*a)
    elif r == 'block':
        t2 += C5*D/a

    cost = t1 + t2
    #print "(" + e + "," + alloc + ")  " + "cost = " + str(cost)
    #print "\t" + str(t1) + ", " + str(t2) + ", " + str(t3)
    return cost

if __name__ == "__main__":
    main()
