import numpy as np
import subprocess
from parameters import *
from time import *
import multiprocessing as mp

#year
#where?
#how many

########## PARAMETERS ##########
Events = 1e8
## Geant4
YEAR = "2025"
FIELD_MAP = False
CAD_MESH = False
Catcher_type = "THIN"
## CRADLE
PeakConfig = "IAS" # "ENSDF" / ENSDFP / IAS
Nucleus = "32Ar"
CreatingNewCRADLEFile = False
abMode = True
## Names of variables
Var_Names = ["Beam", "DL", "B", "Catcher_e", "Catcher_Position", "Catcher_Angle", "Detectors", "CV", "CS", "CA", "CT", "a", "b", "Cuts", "SiliconCuts", "StepMax", "PlasticStep"]
## MODE SAMPLING / ARRAY
SAMPLING=False

########## RUNNING ##########
THREAD = 40
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

########################################
# Variable["Detectors"] = {}
# Variable["Detectors"]["x"] = []
# Variable["Detectors"]["y"] = []
# Variable["Detectors"]["z"] = []
# Variable["Detectors"]["Rx"] = []
# Variable["Detectors"]["Ry"] = []
# Variable["Catcher_z"] = []
# Variable["Catcher_Angle"] = []
# Variable["Beam"] = {}
# Variable["Beam"]["x"] = []
# Variable["Beam"]["y"] = []
# Variable["Beam"]["sx"] = []
# Variable["Beam"]["sy"] = []

# ########### BEAM 1D #########
# X = [-0.3, -0.2, -0.1, 0.0, 0.05, 0.1, 0.15, 0.2, 0.3, 0.4]
# for xi in X:
#     Variable["Beam"]["x"].append(xi)
#     Variable["Beam"]["y"].append(Beam[YEAR]["y"][0])
#     Variable["Beam"]["sx"].append(Beam[YEAR]["sx"][0])
#     Variable["Beam"]["sy"].append(Beam[YEAR]["sy"][0])
# Y = [-0.7, -0.6, -0.5, -0.45, -0.4, -0.35, -0.3, -0.2, -0.1]
# for yi in Y:
#     Variable["Beam"]["x"].append(Beam[YEAR]["x"][0])
#     Variable["Beam"]["y"].append(yi)
#     Variable["Beam"]["sx"].append(Beam[YEAR]["sx"][0])
#     Variable["Beam"]["sy"].append(Beam[YEAR]["sy"][0])

## 2024 with 2024 beam
# x = [1.05]
# y = [1.5, 1.6, 1.7, 1.8, 1.9, 2.0, 2.1, 2.2, 2.3]
# z = [-0.5]
# # for xi in x:
# for yi in y:
#         # for zi in z:
#             Variable["Detectors"]["x"].append(x[0])
#             Variable["Detectors"]["y"].append(yi)
#             Variable["Detectors"]["z"].append(0)
#             Variable["Detectors"]["Rx"].append(0)
#             Variable["Detectors"]["Ry"].append(0)
#             Variable["Catcher_z"].append(z[0])

## 2025 with 2025 beam
# x = [-0.5, -0.45, -0.4, -0.35, -0.3]
# y = [2.6, 2.65, 2.7, 2.75, 2.8]
# z = [0.6, 0.65, 0.7]
# for xi in x:
#     for yi in y:
#         for zi in z:
#             Variable["Detectors"]["x"].append(xi)
#             Variable["Detectors"]["y"].append(yi)
#             Variable["Detectors"]["z"].append(0)
#             Variable["Detectors"]["Rx"].append(0)
#             Variable["Detectors"]["Ry"].append(0)
#             Variable["Catcher_z"].append(zi)    

# Variable["Catcher_Position"] = {}
# Variable["Catcher_Position"]["x"] = []
# Variable["Catcher_Position"]["y"] = []
# Variable["Catcher_Position"]["z"] = []
# Variable["Detectors"] = {}
# Variable["Detectors"]["x"] = []
# Variable["Detectors"]["y"] = []
# Variable["Detectors"]["z"] = []
# Variable["Detectors"]["Rx"] = []
# Variable["Detectors"]["Ry"] = []
 
## A) catcher position
# x = [0, 1, -1, 2, -2, 3, -3, 4, -4]
# y = [0, 1, 2, 3, 4, -1, -2, -3, -4]
# z = [0.0]
# more precise around minimum
# x = [-1, -0.5, 0, 0.5, 1]
# y = [1.5, 1.75, 2.0, 2.25, 2.5, 2.75]
# z = [0.0]
#
# x = [-0.75, -0.5, -0.25, 0, 0.25, 0.5, 0.75]
# y = [1.8, 1.9, 2.0, 2.1, 2.2, 2.3]
# z = [0.0]
#
# for zi in z:
#     for yi in y:    
#         for xi in x:
#             Variable["Catcher_Position"]["x"].append(Catcher_Position[YEAR]["x"] + xi)
#             Variable["Catcher_Position"]["y"].append(Catcher_Position[YEAR]["y"] + yi)
#             Variable["Catcher_Position"]["z"].append(Catcher_Position[YEAR]["z"] + zi)

## A') DIFFERENT CRADLE FILE at the same position
# Variable["a"] = [0.975, 0.98, 0.985, 0.99, 0.995]
# Variable["b"] = [0.0, 0.0, 0.0, 0.0, 0.0]
# Variable["Catcher_Position"]["x"] = [0.0, 0.0, 0.0, 0.0, 0.0] 
# Variable["Catcher_Position"]["y"] = [2.2, 2.2, 2.2, 2.2, 2.2]
# Variable["Catcher_Position"]["z"] = [0.0, 0.0, 0.0, 0.0, 0.0]

## B) Silicon position + catcher z
# Catcher_Position[YEAR]["x"] = 0.0  ####### DETERMINED BY A
# Catcher_Position[YEAR]["y"] = 2.1  ####### DETERMINED BY A
# Catcher_Position[YEAR]["z"] = 0.0
# x = [-4, -3, -2, -1, 0, 1, 2, 3, 4]
# y = [-4, -3, -2, -1, 0, 1, 2, 3, 4]
# z = [-2.0, -1.0, 0.0, 1.0, 2.0]
# for zi in z:
#     for yi in y:    
#         for xi in x:
#             Variable["Catcher_Position"]["x"].append(Catcher_Position[YEAR]["x"])
#             Variable["Catcher_Position"]["y"].append(Catcher_Position[YEAR]["y"])
#             Variable["Catcher_Position"]["z"].append(Catcher_Position[YEAR]["z"] + zi)
#             Variable["Detectors"]["x"].append(xi)
#             Variable["Detectors"]["y"].append(yi)
#             Variable["Detectors"]["z"].append(0.0)
#             Variable["Detectors"]["Rx"].append(0.0)
#             Variable["Detectors"]["Ry"].append(0.0)
# # more precise around minimum
# x = [-1, -0.5, 0, 0.5, 1]
# y = [1.25, 1.5, 1.75, 2, 2.25]
# z = [0.0, 0.25, 0.5, 0.75, 1.0]
# for zi in z:
#     for yi in y:    
#         for xi in x:
#             Variable["Catcher_Position"]["x"].append(Catcher_Position[YEAR]["x"])
#             Variable["Catcher_Position"]["y"].append(Catcher_Position[YEAR]["y"])
#             Variable["Catcher_Position"]["z"].append(Catcher_Position[YEAR]["z"] + zi)
#             Variable["Detectors"]["x"].append(xi)
#             Variable["Detectors"]["y"].append(yi)
#             Variable["Detectors"]["z"].append(0.0)
#             Variable["Detectors"]["Rx"].append(0.0)
#             Variable["Detectors"]["Ry"].append(0.0)
#
# x = [0.15, 0.0, -0.15]
# y = [1.5, 1.6, 1.7, 1.8, 1.9]
# z = [0.5, 0.6, 0.7, 0.8]
# for zi in z:
#     for yi in y:    
#         for xi in x:
#             Variable["Catcher_Position"]["x"].append(Catcher_Position[YEAR]["x"])
#             Variable["Catcher_Position"]["y"].append(Catcher_Position[YEAR]["y"])
#             Variable["Catcher_Position"]["z"].append(Catcher_Position[YEAR]["z"] + zi)
#             Variable["Detectors"]["x"].append(xi)
#             Variable["Detectors"]["y"].append(yi)
#             Variable["Detectors"]["z"].append(0.0)
#             Variable["Detectors"]["Rx"].append(0.0)
#             Variable["Detectors"]["Ry"].append(0.0)

# ## C) Asymetry relative efficiency
# z = [0., 0.5, 1.0, 1.5]
# for zi in z:
#     Variable["Catcher_Position"]["x"].append(0.0)
#     Variable["Catcher_Position"]["y"].append(2.2)
#     Variable["Catcher_Position"]["z"].append(zi)
#     Variable["Detectors"]["x"].append(-0.45)
#     Variable["Detectors"]["y"].append(1.82)
#     Variable["Detectors"]["z"].append(0.0)
#     Variable["Detectors"]["Rx"].append(0.0)
#     Variable["Detectors"]["Ry"].append(0.0)
########################################

### CALIBRATION
# Variable["a"] = [0.96, 0.97, 0.98, 1.02, 1.03, 1.04]
# Variable["b"] = [0., 0., 0., 0., 0., 0.]
    
### DL
# Variable["DL"] = [190]

### B field
# Variable["B"] = [B[0] - B[1], B[0] + B[1], B[0] - 2*B[1], B[0] + 2*B[1], B[0] - 3*B[1], B[0] + 3*B[1]]
# Variable["B"] = [0.0, 0.5, 1.0, 1.5, 2.0, 2.5, 3.0, 3.5, 4.0, 4.5, 5.0, 5.5, 6.0, 6.5, 7.0, 7.5, 8.0, 8.5, 9.0]
# Variable["a"] = [0.9, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9]
### Catcher thickness

# Variable["Catcher_e"] = {
    # "Al" : [Catcher_e["THIN"]["Al"][0] - 1*Catcher_e["THIN"]["Al"][1], 
            # Catcher_e["THIN"]["Al"][0] + 1*Catcher_e["THIN"]["Al"][1], 
            # Catcher_e["THIN"]["Al"][0] - 2*Catcher_e["THIN"]["Al"][1], 
            # Catcher_e["THIN"]["Al"][0] + 2*Catcher_e["THIN"]["Al"][1], 
            # Catcher_e["THIN"]["Al"][0] - 3*Catcher_e["THIN"]["Al"][1], 
            # Catcher_e["THIN"]["Al"][0] + 3*Catcher_e["THIN"]["Al"][1],
            # ],
    # "Mylar" : [Catcher_e["THIN"]["Mylar"][0] - 1*Catcher_e["THIN"]["Mylar"][1],
                # Catcher_e["THIN"]["Mylar"][0] + 1*Catcher_e["THIN"]["Mylar"][1], 
                # Catcher_e["THIN"]["Mylar"][0] - 2*Catcher_e["THIN"]["Mylar"][1], 
                # Catcher_e["THIN"]["Mylar"][0] + 2*Catcher_e["THIN"]["Mylar"][1], 
                # Catcher_e["THIN"]["Mylar"][0] - 3*Catcher_e["THIN"]["Mylar"][1], 
                # Catcher_e["THIN"]["Mylar"][0] + 3*Catcher_e["THIN"]["Mylar"][1],
#                 ]
# }

### Plotting
# Variable["Detectors"] = {}
# Variable["Detectors"]["x"] = []
# Variable["Detectors"]["y"] = []
# Variable["Detectors"]["z"] = []
# Variable["Detectors"]["Rx"] = []
# Variable["Detectors"]["Ry"] = []
# Variable["Catcher_Position"] = {}
# Variable["Catcher_Position"]["x"] = []
# Variable["Catcher_Position"]["y"] = []
# Variable["Catcher_Position"]["z"] = []

# Variable["Beam"] = {}
# Variable["Beam"]["x"] = []
# Variable["Beam"]["y"] = []
# Variable["Beam"]["sx"] = []
# Variable["Beam"]["sy"] = []

# centered 
# Variable["Detectors"]["x"].append(0)
# Variable["Detectors"]["y"].append(0)
# Variable["Detectors"]["z"].append(0)
# Variable["Detectors"]["Rx"].append(0)
# Variable["Detectors"]["Ry"].append(0)   
# Variable["Catcher_Position"]["x"].append(0)
# Variable["Catcher_Position"]["y"].append(0)
# Variable["Catcher_Position"]["z"].append(0)
# Variable["Beam"]["x"].append(0)
# Variable["Beam"]["y"].append(0)
# Variable["Beam"]["sx"].append(0)
# Variable["Beam"]["sy"].append(0)

# Detector position (X, Y)
# Variable["Detectors"]["x"].append(Detectors[YEAR]["x"])
# Variable["Detectors"]["y"].append(Detectors[YEAR]["y"])
# Variable["Detectors"]["z"].append(0)
# Variable["Detectors"]["Rx"].append(0)
# Variable["Detectors"]["Ry"].append(0)
# Variable["Catcher_Position"]["x"].append(0)
# Variable["Catcher_Position"]["y"].append(0)
# Variable["Catcher_Position"]["z"].append(0)
# Variable["Beam"]["x"].append(0)
# Variable["Beam"]["y"].append(0)
# Variable["Beam"]["sx"].append(0)
# Variable["Beam"]["sy"].append(0)

# Catcher position z
# Variable["Detectors"]["x"].append(Detectors[YEAR]["x"])
# Variable["Detectors"]["y"].append(Detectors[YEAR]["y"])
# Variable["Detectors"]["z"].append(4)
# Variable["Detectors"]["Rx"].append(0)
# Variable["Detectors"]["Ry"].append(0)
# Variable["Catcher_Position"]["x"].append(0)
# Variable["Catcher_Position"]["y"].append(0)
# Variable["Catcher_Position"]["z"].append(0)
# Variable["Beam"]["x"].append(0)
# Variable["Beam"]["y"].append(0)
# Variable["Beam"]["sx"].append(0)
# Variable["Beam"]["sy"].append(0)

# Catcher Position (x, y)
# Variable["Detectors"]["x"].append(0)
# Variable["Detectors"]["y"].append(0)
# Variable["Detectors"]["z"].append(0)
# Variable["Detectors"]["Rx"].append(0)
# Variable["Detectors"]["Ry"].append(0)   
# Variable["Catcher_Position"]["x"].append(Catcher_Position[YEAR]["x"])
# Variable["Catcher_Position"]["y"].append(Catcher_Position[YEAR]["y"])
# Variable["Catcher_Position"]["z"].append(Catcher_Position[YEAR]["z"])
# Variable["Beam"]["x"].append(Beam[YEAR]["x"][0])
# Variable["Beam"]["y"].append(Beam[YEAR]["y"][0])
# Variable["Beam"]["sx"].append(Beam[YEAR]["sx"][0])
# Variable["Beam"]["sy"].append(Beam[YEAR]["sy"][0])

# x = [-5, 5]
# y = [-5, 5]

# x0 = 1.15
# y0 = 1.83
# z0 = 0.5

# for zi in [-2, -1, 1]:
#             Variable["Detectors"]["x"].append(Detectors["2025"]["x"])
#             Variable["Detectors"]["y"].append(Detectors["2025"]["y"])
#             Variable["Detectors"]["z"].append(Detectors["2025"]["z"] + zi)
#             Variable["Detectors"]["Rx"].append(0)
#             Variable["Detectors"]["Ry"].append(0)

# for yi in [-1, -0.5, -0.25, 0, 0.25, 0.5, 1]:
#             Variable["Detectors"]["x"].append(x0)
#             Variable["Detectors"]["y"].append(y0 + yi)
#             Variable["Detectors"]["z"].append(z0)
#             Variable["Detectors"]["Rx"].append(0)
#             Variable["Detectors"]["Ry"].append(0)
# for xi in [-1, -0.5, -0.25, 0, 0.25, 0.5, 1]:
#             Variable["Detectors"]["x"].append(x0 + xi)
#             Variable["Detectors"]["y"].append(y0)
#             Variable["Detectors"]["z"].append(z0)
#             Variable["Detectors"]["Rx"].append(0)
#             Variable["Detectors"]["Ry"].append(0)

# Variable["Catcher_Position"] = {}
# Variable["Catcher_Position"]["x"] = []
# Variable["Catcher_Position"]["y"] = []
# Variable["Catcher_Position"]["z"] = []
# for yi in [4.5, 5.0, 5.5, 6.0]:
#     Variable["Catcher_Position"]["x"].append(0)
#     Variable["Catcher_Position"]["y"].append(yi)
#     Variable["Catcher_Position"]["z"].append(0)


## CUTS ##
# Variable["StepMax"] = [1000, 100, 10, 1, 0.1, 0.01, 0.001]
# Variable["SiliconCuts"] = [1000, 100, 10, 1, 0.1, 0.01, 0.001, 0.0001, 0.00001]
# Variable["PlasticStep"] = [1000, 100, 10, 1, 0.1, 0.01, 0.001, 0.0001, 0.00001]

## CATCHER POSITION Y ##
# Variable["Catcher_Position"] = {}
# Variable["Catcher_Position"]["x"] = []
# Variable["Catcher_Position"]["y"] = []
# Variable["Catcher_Position"]["z"] = []
# y = [-0.5, 0, 0.5]
# y = sorted(y, key=lambda x: abs(x), reverse=True)
# for yi in y:    
    
#         Variable["Catcher_Position"]["x"].append(Catcher_Position[YEAR]["x"])
#         Variable["Catcher_Position"]["y"].append(Catcher_Position[YEAR]["y"] + yi)
#         Variable["Catcher_Position"]["z"].append(Catcher_Position[YEAR]["z"])

# Variable["Catcher_Angle"] = []
# Variable["Catcher_Position"] = {}
# Variable["Catcher_Position"]["x"] = []
# Variable["Catcher_Position"]["y"] = []
# Variable["Catcher_Position"]["z"] = []
# y = [-2, -1, -0.5, -0.25, 0, 0.25]
# for t in [-5, -3, -2, -1, 0, 1, 2, 3, 5]:
#     for yi in y:  
#         Variable["Catcher_Angle"].append(Catcher_Angle[YEAR][0] + t)
#         Variable["Catcher_Position"]["x"].append(Catcher_Position[YEAR]["x"])
#         Variable["Catcher_Position"]["y"].append(Catcher_Position[YEAR]["y"] + yi)
#         Variable["Catcher_Position"]["z"].append(Catcher_Position[YEAR]["z"])

# Variable["Catcher_Position"] = {}
# Variable["Catcher_Position"]["x"] = []
# Variable["Catcher_Position"]["y"] = []
# Variable["Catcher_Position"]["z"] = []
# for zi in z:
#     for yi in y:  
#         Variable["Catcher_Position"]["x"].append(Catcher_Position[YEAR]["x"])
#         Variable["Catcher_Position"]["y"].append(Catcher_Position[YEAR]["y"] + yi)
#         Variable["Catcher_Position"]["z"].append(Catcher_Position[YEAR]["z"])
#         Variable["Detectors"]["x"].append(Detectors[YEAR]["x"])
#         Variable["Detectors"]["y"].append(Detectors[YEAR]["y"])
#         Variable["Detectors"]["z"].append(Detectors[YEAR]["z"] + zi)
#         Variable["Detectors"]["Rx"].append(0)
#         Variable["Detectors"]["Ry"].append(0)

######## Dead layer
# Variable["DL"] = [0, 30, 60, 90, 120, 150, 180, 210]

######## 2024 new position ########
# y = [2.5, 3, 3.5, 4.0]
# x = [-0.5, -0.25, 0, 0.25, 0.5]
# for yi in y:    
#         for xi in x:
#             Variable["Detectors"]["x"].append(xi)
#             Variable["Detectors"]["y"].append(yi)
#             Variable["Detectors"]["z"].append(1.0)
#             Variable["Detectors"]["Rx"].append(0.0)
#             Variable["Detectors"]["Ry"].append(0.0)

############# PRODUCTION ############
# Variable["a"] = [0.9, 0.95, 0.98, 1.0, 1.02, 1.05, 1.1]
# Variable["b"] = [0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0]

# Variable["a"] = [0.925, 0.975, 0.985, 0.99, 0.995, 1.005, 1.01, 1.015, 1.025, 1.075]
# Variable["b"] = [0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0]

# Variable["a"] = [0.9, 0.925, 0.95, 0.975, 0.98, 0.985, 0.99, 0.995, 1.0, 1.005, 1.01, 1.015, 1.02, 1.025, 1.05, 1.075, 1.1]
# Variable["b"] = [0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0]

# Variable["a"] = [0.91, 0.92, 0.93, 0.94, 0.96, 0.965, 0.97, 0.985, 0.9925, 0.9975, 1.0025, 1.0075, 1.03, 1.035, 1.04, 1.06, 1.07, 1.08, 1.09]
# Variable["b"] = [0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0]

# Variable["a"] = [0.5, 0.6, 0.7, 0.8, 1.2, 1.3]
# Variable["b"] = [0.0, 0.0, 0.0, 0.0, 0.0, 0.0]

CheckDirectory(Geant4_DATA_Path, True)
CheckDirectory(CRADLE_DATA_Path, True)

N_sim = VerifyVariable(Variable, Var_Names)
if (SAMPLING): N_sim = Sampling

def run_simulations(i):

    # if (i in [0, 1, 2, 3, 4, 7, 8, 9, 10, 11, 14, 15, 18, 19, 20, 21, 22]):
    #     return 
    
    ParametersForSim = SetParameters(Variable, YEAR, Catcher_type, abMode, i)
    suffixe = Get_Suffixe(ParametersForSim, Variable, i, SAMPLING)

    Parse(Variable, ParametersForSim)
    sleep(1)

    # check if CRADLE file and create it if option
    CRADLE_filename = Check_CRADLE_File(Nucleus, ParametersForSim, PeakConfig, abMode, CreatingNewCRADLEFile, Events, THREAD//N_simulatenous)

    if not CRADLE_filename:
        print("Error in CRADLE file creation")
        exit(1)

    # run Geant4     
    filename = Running_Geant4(suffixe, ParametersForSim, CRADLE_filename, YEAR, FIELD_MAP, CAD_MESH, Events, THREAD//N_simulatenous, Nucleus, PeakConfig, Catcher_type)

    command = "cd /home/lecanuet/2024_Analysis/Simulation; ReaderNew ../../../../{}{}; cd -".format(Geant4_DATA_Path, filename[filename.find("Ar_")-3:filename.find(".root")])
    os.system(command)

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