import numpy as np
import subprocess
import multiprocessing as mp

mini = 0.1
maxi = 10.0
step = 0.05

THREAD = 80
thread = 4

E = np.arange(mini, maxi + step, step)
N = len(E)

def run_simulations(i, particle, B):
    print("Running G4-Scanning_Stat with parameters:")
    print("G4-Scanning_Energy particle={} events=10000000 N={} energy={:.2f} B={:.1f}".format(particle, int(THREAD/thread), E[i], B))
    subprocess.run("G4-Scanning_Energy particle={} events=10000000 N={} energy={:.2f} B={:.1f}".format(particle, int(THREAD/thread), E[i], B), shell=True)

   

for particle in ["proton"]:
    for B in [4.0, 1.5, 2.5, 6.0, 7.5, 9.0]:
        with mp.Pool(processes=thread) as pool:
            results = pool.starmap(run_simulations, [(i, particle, B) for i in range(N)])

        # for i in range(0, N, thread):
        #     string = ""
        #     for j in range(thread):
        #         n = i + j
        #         if n >= N:
        #             break
        #         print("Running G4-Scanning_Stat with parameters:")
        #         string += "G4-Scanning_Energy particle={} events=10000000 N={} energy={:.2f} B={:.1f}".format(particle, int(THREAD/thread), E[n], B)
        #         if j != thread - 1:
        #             string += " & "
        #     subprocess.run(string, shell=True)