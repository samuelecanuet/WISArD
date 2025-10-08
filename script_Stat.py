import numpy as np
import subprocess


sigma_x = 0.76 
sigma_y = 0.45  
mu_y = 0.46     
mu_x = -0.02 

rho = 0.0

N = 1000


# a = 1
subprocess.run("G4-Scanning_Stat nucleus=32Ar CV=1 sx={:.4f} sy={:.4f} x={:.4f} y={:.4f} events=100000000 N=50".format(sigma_x, sigma_y, mu_x, mu_y), shell=True)
# a = 0
# subprocess.run("G4-Scanning_Stat nucleus=32Ar CV=1 CVP=1 CS=1 CSP=1 sx={:.4f} sy={:.4f} x={:.4f} y={:.4f} events=100000000 N=30".format(sigma_x, sigma_y, mu_x, mu_y), shell=True)
# # a = -1
# subprocess.run("G4-Scanning_Stat nucleus=32Ar CV=0 CVP=0 CS=1 CSP=1 sx={:.4f} sy={:.4f} x={:.4f} y={:.4f} events=100000000 N=30".format(sigma_x, sigma_y, mu_x, mu_y), shell=True)

for i in range(N):
    n = i
    print("Runing G4-Scanning_Stat with parameters:")
    print("sx={:.4f} sy={:.4f} x={:.4f} y={:.4f}".format(sigma_x, sigma_y, mu_x, mu_y))
    subprocess.run("G4-Scanning_Stat nucleus=32Ar CV=1 n={} sx={:.4f} sy={:.4f} x={:.4f} y={:.4f} events=100000000 N=50".format(n, sigma_x, sigma_y, mu_x, mu_y), shell=True)
