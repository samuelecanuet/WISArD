import numpy as np
import subprocess


# sigma_x = 0.76 
# sigma_y = 0.45  
# mu_y = 0.46     
# mu_x = -0.02 
# rho = 0.0

# for i in range(N):
#     n = i
#     print("Runing G4-Scanning_Stat with parameters:")
#     print("sx={:.4f} sy={:.4f} x={:.4f} y={:.4f}".format(sigma_x, sigma_y, mu_x, mu_y))
#     subprocess.run("G4-Scanning_Stat nucleus=32Ar CV=1 n={} sx={:.4f} sy={:.4f} x={:.4f} y={:.4f} events=100000000 N=50".format(n, sigma_x, sigma_y, mu_x, mu_y), shell=True)

sigma_x = 0.336411
sigma_y = 0.393268
mu_x = -0.05363
mu_y = 0.4103

N = 900


# a = 1
# subprocess.run("G4-Scanning_Stat nucleus=32Ar CV=1 sx={:.4f} sy={:.4f} x={:.4f} y={:.4f} events=100000000 N=30".format(sigma_x, sigma_y, mu_x, mu_y), shell=True)
# a = 0
#subprocess.run("G4-Scanning_Stat nucleus=32Ar CV=1 CVP=1 CS=1 CSP=1 sx={:.4f} sy={:.4f} x={:.4f} y={:.4f} events=100000000 N=50".format(sigma_x, sigma_y, mu_x, mu_y), shell=True)
# # a = -1
#subprocess.run("G4-Scanning_Stat nucleus=32Ar CV=0 CVP=0 CS=1 CSP=1 sx={:.4f} sy={:.4f} x={:.4f} y={:.4f} events=100000000 N=50".format(sigma_x, sigma_y, mu_x, mu_y), shell=True)

THREAD = 60
thread = 3

if N % thread != 0:
    print("N must be multiple of thread")
    exit(1)

for i in range(3, N, thread):
    string = ""

    ## Run simulations in parallel
    for j in range(thread):
        n = i + j
        if n >= N:
            break
        print("Running G4-Scanning_Stat with parameters:")
        string += "G4-Scanning_Stat nucleus=32Ar a=1.0 b=0.0 n={} events=100000000 N={}".format(n, int(THREAD/thread))
        if j != thread - 1:
            string += " & "
    subprocess.run(string, shell=True)

    ## Run analysis in parrallel
    string = ""
    string += "cd /home/lecanuet/2024_Analysis/Simulation/ ; "

    for j in range(thread):
        n = i + j
        if n >= N:
            break
        string += f"Reader2 32Ar_n{n}_a1.0_b0.0"
        if j != thread - 1:
            string += " & "
    subprocess.run(string, shell=True)

    ## removing sim files
    for j in range(thread):
        n = i + j
        if n >= N:
            break
        subprocess.run(f"rm -f /data333/lecanuet/Result/G4Stat/32Ar_n{n}_a1.0_b0.0.root", shell=True)

    ## going back to simulation folder
    subprocess.run("cd /home/lecanuet/WISArD/", shell=True)
    
