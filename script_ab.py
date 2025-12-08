import numpy as np
import subprocess

N = 6

al = [0.995, 0.985, 0.975, 0.965, 0.955, 1.0]
bl = [0., 0., 0., 0., 0., 0.]

THREAD = 36
thread = 3

if THREAD % thread != 0:
    print("THREAD must be multiple of thread for each sim")
    exit(1)

for i in range(0, N, thread):
    string = ""
    for j in range(thread):
        n = i + j
        if n >= N:
            break
        # Cv = CV[n]
        # Cs = CS[n]
        # Csp = Cs
        # Cvp = Cv
        a = al[n]
        b = bl[n]
        
        print("Runing G4-Scanning_Beam with parameters:")
        # print("CV={:.4f} CS={:.4f}".format(Cv, Cs))

        string += "CRADLE-G4 nucleus=32Ar a={} b={} events=100000000 N={}".format(a, b, int(THREAD/thread))
        if j != thread - 1:
            string += " & "
    subprocess.run(string, shell=True)