import numpy as np
import subprocess

mini = 0.1
maxi = 7.0
step = 0.05

THREAD = 60
thread = 3

E = np.arange(mini, maxi + step, step)
N = len(E)

for particle in ["proton", "alpha"]:
    for i in range(0, N, thread):
        string = ""
        for j in range(thread):
            n = i + j
            if n >= N:
                break
            print("Running G4-Scanning_Stat with parameters:")
            string += "G4-Scanning_Energy particle={} events=10000000 N={} energy={:.2f}".format(particle, int(THREAD/thread), E[n])
            if j != thread - 1:
                string += " & "
        subprocess.run(string, shell=True)