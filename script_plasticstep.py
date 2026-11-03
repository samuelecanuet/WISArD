import numpy as np
import subprocess
import multiprocessing as mp

THREAD = 48
thread = 4



def run_simulations(i, step):
    print("Running G4-Scanning_Step with parameters:")
    print("G4-Scanning_Step events=1000000 step={:.3f} N={}".format(step, int(THREAD/thread)))
    subprocess.run("G4-Scanning_Step events=1000000 step={:.3f} N={}".format(step, int(THREAD/thread)), shell=True)

l = [0.05, 0.5, 0.8]
with mp.Pool(processes=thread) as pool:
    results = pool.starmap(run_simulations, [(i, l[i]) for i in range(len(l))])
