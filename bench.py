import time
import vtk
import datetime
import math
import random
import os
import csv
import sys

import rpg

def timeRPG(N):
    t_before = time.time() 
    polygon= rpg.rpg(N)
    t_after = time.time() 
    times=[]
    times.append(t_after-t_before)
    return times
    
if __name__ == "__main__":  
    print rpg.version()
    Nmax_exp_start = 5
    Nmax_exp_end = 20
    # 10 -> 32 linesites
    # 14 -> 128
    # 18 -> 512
    # 20 -> 1024        timeVoronoiSegs() segtype=1 (Release-build) (398-splitverts)
    # 24 -> 4096
    # 28 -> 16384
    # 32 -> 65536
    # 33 -> 92681
    # 34 -> 131072      timeVoronoiPoints() (Release-build)
    # 35 -> 185363
    # 36 -> 262144
    
    random.seed(1)
    exp_list = [0.5*x for x in range(Nmax_exp_start,Nmax_exp_end+1)]
    Nmax_list=[]
    n=5
    # generate the list of problem sizes to run
    for e in exp_list:
        Nmax_list.append( [ n, int( math.floor( (math.pow(2,e) ) ) ) ] )
        n=n+1
        
    #print Nmax_list
    #exit()
    csvWriter = csv.writer(open('bench_results.csv', 'wb'), delimiter=',' )
    for case in Nmax_list:
        n=case[0]
        Nmax=case[1]
        #times = timeVoronoiPoints(Nmax)
        times = timeRPG(Nmax)
        print n," RPG for ",Nmax," took {0:.3f} seconds".format(sum(times)) 

        row = []
        row.append(Nmax)
        for t in times:
            row.append(t)
        csvWriter.writerow( row )
        
    print "PYTHON All DONE."


