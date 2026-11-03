import os

import numpy as np
import subprocess
import multiprocessing as mp

mini = 1100
maxi = 10000+100
step = 100

THREAD = 60
thread = 1

E = range(mini, maxi, step)
N = len(E)

import os

def run_simulations(i):
    print("Running G4-ScanningBetaSpectrum with parameters:")
    print("G4-ScanningBetaSpectrum events=3000000 N={} energy={:.0f}".format(int(THREAD/thread), E[i]))
    subprocess.run("setenv Radiationdata ../../BetaSpectrum/{:.0f} ; ./G4-Scanning_BetaSpectrum events=3000000 N={} energy={:.0f}".format(E[i], int(THREAD/thread), E[i]), shell=True, executable='/bin/tcsh')

    command = "cd ../2024_Analysis/Simulation/; ReaderNew ../../../../{}{}; cd -".format("data333/lecanuet/Result/Qbeta/", "32Ar_Qbeta{:.0f}".format(E[i]))
    subprocess.run(command, shell=True)


## from N-1 to 0  
for i in range(N-2, -1, -1):
    run_simulations(i)
