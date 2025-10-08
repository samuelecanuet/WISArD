import numpy as np
import subprocess

# old
# sigma_x = 0.76 
# u_sigma_x = 0.05

# sigma_y = 0.45  
# u_sigma_y = 0.05

# mu_y = 0.46     
# u_mu_y = 0.06 

# mu_x = -0.02 
# u_mu_x = 0.08  

# new 
#  double sigma_x = 0.336411; //+/- 0.0402;
#     double sigma_y = 0.393268; //+/- 0.0447;
#     double Amplitude_gauss = 86.55; //+/- 11.766;
#     double mu_gx = -0.05363; //+/- 0.100;
#     double mu_gy = 0.4103; //+/- 0.07466;
#     double sigma_gx = 1.05279; //+/- 0.0824;
#     double sigma_gy = 0.611; //+/- 0.0884;
#     double bkg = 2.30825; //+/- 0.416;

sigma_x = 0.336411
u_sigma_x = 0.0402
sigma_y = 0.393268
u_sigma_y = 0.0447
mu_x = -0.05363
u_mu_x = 0.100
mu_y = 0.4103
u_mu_y = 0.07466


rho = 0.0

N = 1000

# FOR MEAN UNCERTAINTY SAMPLING
mu_cov = np.array([[u_mu_y**2, rho*u_mu_y*u_mu_y], [rho*u_mu_y*u_mu_y, u_mu_y**2]])
mu_mean = np.array([mu_x, mu_y])
mu_samples = np.random.multivariate_normal(mu_mean, mu_cov, N)

# FOR SIGMA UNCERTAINTY SAMPLING
sigma_cov = np.array([[u_sigma_x**2, rho*u_sigma_x*u_sigma_y], [rho*u_sigma_x*u_sigma_y, u_sigma_y**2]])
sigma_mean = np.array([sigma_x, sigma_y])
sigma_samples = np.random.multivariate_normal(sigma_mean, sigma_cov, N)

THREAD = 60
thread = 4

if N % thread != 0:
    print("N must be multiple of thread")
    exit(1)

for i in range(0, N, thread):
    string = ""
    for j in range(thread):
        n = i + j
        if n >= N:
            break
        mu_x, mu_y = mu_samples[n]
        sigma_x, sigma_y = sigma_samples[n]
        print("Runing G4-Scanning_Beam with parameters:")
        print("sx={:.4f} sy={:.4f} x={:.4f} y={:.4f}".format(sigma_x, sigma_y, mu_x, mu_y))
        string += "G4-Scanning_Beam nucleus=32Ar CV=1 sx={:.4f} sy={:.4f} x={:.4f} y={:.4f} events=100000000 N={}".format(sigma_x, sigma_y, mu_x, mu_y, int(THREAD/thread))
        if j != thread - 1:
            string += " & "
    subprocess.run(string, shell=True)
    # mu_x, mu_y = mu_samples[i]
    # sigma_x, sigma_y = sigma_samples[i]
    # print("Runing G4-Scanning_Beam with parameters:")
    # print("sx={:.4f} sy={:.4f} x={:.4f} y={:.4f}".format(sigma_x, sigma_y, mu_x, mu_y))
    # subprocess.run("G4-Scanning_Beam nucleus=32Ar CV=1 sx={:.4f} sy={:.4f} x={:.4f} y={:.4f} events=100000000 N=THREAD".format(sigma_x, sigma_y, mu_x, mu_y), shell=True)
