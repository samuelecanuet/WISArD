import os

import numpy as np
import subprocess
import multiprocessing as mp

THREAD = 60
thread = 1

M = [-500, 500, -200, 200, 0, -1000, 1000]
N = len(M)

import os

def run_simulations(i):
    print("Running G4-Scanning32ArMass with parameters:")
    print("G4-Scanning32ArMass events=10000000 N={} mass={:.0f}".format(int(THREAD/thread), M[i]))
    subprocess.run("setenv Radiationdata ../../Mass/{:.0f} ; setenv AMEdata ../../Mass/{:.0f}/AMEdata.txt ; ./G4-ScanningMass events=10000000 N={} mass={:.0f}".format(M[i], M[i], int(THREAD/thread), M[i]), shell=True, executable='/bin/tcsh')

    command = "cd ../2024_Analysis/Simulation/; ReaderNew ../../../../{}{}; cd -".format("data333/lecanuet/Result/Mass/", "32Ar_Mass{:.0f}".format(M[i]))
    subprocess.run(command, shell=True)

## from N-1 to 0
for i in range(N-1, 0, -1):
    print("Running simulation for mass index: {}".format(M[i]))
    run_simulations(i)
