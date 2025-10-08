import numpy as np
import subprocess

N= 50

CV = np.random.uniform(0, 1, 100)
CS = np.sqrt(1 - CV*CV)

THREAD = 60
thread = 4

if THREAD % thread != 0:
    print("THREAD must be multiple of thread for each sim")
    exit(1)

for i in range(0, N, thread):
    string = ""
    for j in range(thread):
        n = i + j
        if n >= N:
            break
        Cv = CV[n]
        Cs = CS[n]
        Csp = Cs
        Cvp = Cv
        
        print("Runing G4-Scanning_Beam with parameters:")
        print("CV={:.4f} CS={:.4f}".format(Cv, Cs))

        string += "CRADLE-G4 nucleus=32Ar CV={:.4f} CS={:.4f} CVP={:.4f} CSP={:.4f} events=100000000 N={}".format(Cv, Cs, Cvp, Csp, int(THREAD/thread))
        if j != thread - 1:
            string += " & "
    subprocess.run(string, shell=True)