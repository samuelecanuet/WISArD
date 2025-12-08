import numpy as np
import subprocess
from parameters import *
from time import *
import multiprocessing as mp

#year
#where?
#how many

########## PARAMETERS ##########
Events = 10e6
## Geant4
YEAR = "2024"
FIELD_MAP = False
CAD_MESH = False 
Catcher_type = "THIN"
## CRADLE
PeakConfig = "IAS" # "ENSDF" or ?
Nucleus = "32Ar"
CreatingNewCRADLEFile = False
## Names of variables
Var_Names = ["Beam", "DL", "B", "Catcher_e", "Catcher_z", "Detectors", "CV", "CS", "CA", "CT"]

########## RUNNING ##########
THREAD = 60
N_simulatenous = 1

if THREAD % N_simulatenous != 0:
    print("N must be multiple of thread")
    exit(1)

#############################
######## VARIABLES ##########
#############################
## format CS CV in filename
Variable = {}
# if value is True doing MC sampling / if not taking array of values (same len)
    # example 1:
# Variable["Beam"] = True
    
    # example 2:
# Variable["CV"] = np.linspace(0.9, 1.1, 10)
# Variable["CS"] = np.linspace(0.9, 1.1, 10)

    # example 3:
# Variable["Catcher_e"] = {}
# Variable["Catcher_e"]["Al"] = [80, 90, 85-20, 85+20]
# Variable["Catcher_e"]["Mylar"] = [518, 532, 525-30, 525+30]

    # example 4:
# Variable = {
#     "Detectors" : True

# Variable["Detectors"] = {}
# Variable["Detectors"]["x"] = []
# Variable["Detectors"]["y"] = []
# Variable["Detectors"]["z"] = []
# Variable["Detectors"]["Rx"] = []
# Variable["Detectors"]["Ry"] = []


## 2024 with 2025 beam
# x = [0.95, 1.0, 1.05, 1.1]
# y = [2.3, 2.35, 2.4, 2.45, 2.5]
# z = [0.35, 0.4, 0.45, 0.5, 0.55]
# for xi in x:
#     for yi in y:
#         for zi in z:
#             Variable["Detectors"]["x"].append(xi)
#             Variable["Detectors"]["y"].append(yi)
#             Variable["Detectors"]["z"].append(zi)
#             Variable["Detectors"]["Rx"].append(0)
#             Variable["Detectors"]["Ry"].append(0)

## 2025 with 2025 beam
# x = [-0.6, -0.55, -0.5, -0.45, -0.4]
# y = [2.6, 2.65, 2.7, 2.75, 2.8]
# z = [-0.6, -0.65, -0.7]
# for xi in x:
#     for yi in y:
#         for zi in z:
#             Variable["Detectors"]["x"].append(xi)
#             Variable["Detectors"]["y"].append(yi)
#             Variable["Detectors"]["z"].append(zi)
#             Variable["Detectors"]["Rx"].append(0)
#             Variable["Detectors"]["Ry"].append(0)


CheckDirectory(Geant4_DATA_Path, True)

N_sim = VerifyVariable(Variable, Var_Names)

def run_simulations(i):
    
    ParametersForSim = SetParameters(Variable, YEAR, Catcher_type, i)
    suffixe = Get_Suffixe(ParametersForSim, Variable)

    Parse(Variable, ParametersForSim)
    sleep(1)

    # check if CRADLE file and create it if option
    CRADLE_filename = Check_CRADLE_File(Nucleus, ParametersForSim, PeakConfig, CreatingNewCRADLEFile, Events, THREAD//N_simulatenous)

    if not CRADLE_filename:
        print("Error in CRADLE file creation")
        exit(1)

    # run Geant4     
    Running_Geant4(suffixe, ParametersForSim, CRADLE_filename, YEAR, FIELD_MAP, CAD_MESH, Events, THREAD//N_simulatenous, Nucleus, PeakConfig)

    return 

# for i in range(0, N_sim, N_simulatenous):
#     string = ""
    # for j in range(N_simulatenous):
    #     n = i + j
    #     if n >= N_sim:
    #         break
        
    #     ParametersForSim = SetParameters(Variable, YEAR, Catcher_type, n)
    #     suffixe = Get_Suffixe(ParametersForSim, Variable)

    #     Parse(Variable, ParametersForSim)
    #     sleep(1)

    #     # check if CRADLE file and create it if option
    #     CRADLE_filename = Check_CRADLE_File(Nucleus, ParametersForSim, PeakConfig, CreatingNewCRADLEFile, Events, THREAD//N_simulatenous)

    #     if not CRADLE_filename:
    #         print("Error in CRADLE file creation")
    #         exit(1)

    #     # run Geant4     
    #     Running_Geant4(suffixe, ParametersForSim, CRADLE_filename, YEAR, FIELD_MAP, Events, THREAD//N_simulatenous, Nucleus, PeakConfig)

    ### MULTITHREAD LOOP
    
with mp.Pool(processes=N_simulatenous) as pool:
    results = pool.map(run_simulations, range(N_sim))