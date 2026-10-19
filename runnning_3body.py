import os
from multiprocessing import Pool

NTHREAD = 80  # Number of threads/processes
NSIMULTANEOUS = 3  # Number of simultaneous simulations

a_values = [
    0.987,
    # 0.988,
    0.989,
    0.991,
    # 0.992,
    0.993,
    # 0.994,
    # 0.996,
    # 0.997,
    # 0.998,
    # 1.002,
    1.003,
    # 1.004,
    # 1.006,
    1.007,
    # 1.008,
    1.009,
    # 1.020,
]

def process_simulation(a):
    """Process a single simulation for value a"""
    with open("macro3body.mac", "r") as f:
        macro_content = f.read()
    
    a_str = "{:.3f}".format(a)
    a_strp = a_str.replace(".", "p")
    macro_content = macro_content.replace("%p%", a_strp)
    macro_content = macro_content.replace("%.%", a_str)
    
    with open(f"tempory/macro_temp_{a}.mac", "w") as f:
        f.write(macro_content)
    
    Geant4command = f"wisard tempory/macro_temp_{a}.mac"
    Readercommand = f"cd /home/lecanuet/2024_Analysis/Simulation/; ReaderNew ../../4body/32Ar_IAS_2025_a{a_str}_radiative_off_noBetaSpectrumCorrections"
    
    os.system(Geant4command)
    os.system(Readercommand)

if __name__ == "__main__":
    with Pool(processes=NSIMULTANEOUS) as pool:
        pool.map(process_simulation, a_values)
