import numpy as np
import subprocess
# from parameters import *
import os

Sampling_MC = 1000
VariableForSim = {}

########## PATHS ##########
CRADLE_Path = "/home/lecanuet/CRADLE/"
Geant4_Path = "/home/lecanuet/WISArD/"
CRADLE_DATA_Path = "/data333/lecanuet/data/"
Geant4_DATA_Path = "/data333/lecanuet/Result/ReCheckPositionDetector/"

########## DATA ##########
Catcher_type = ""

## --- Beam --- ##
Beam = {
    "2024": {},
    "2025": {}
}

Beam["2024"]["x"] = (-0.1, 0.1)
Beam["2024"]["y"] = (0.1, 0.1)
Beam["2024"]["sx"] = (0.7, 0.2)
Beam["2024"]["sy"] = (0.8, 0.2)

Beam["2025"]["x"] = (-0.05363, 0.100)
Beam["2025"]["y"] = (0.4103, 0.07466)
Beam["2025"]["sx"] = (0.336411, 0.0402)
Beam["2025"]["sy"] = (0.393268, 0.0447)

## --- DEAD LAYER --- ##
DL = {}
DL = (100, 50)

## --- MAGNETIC FIELD --- ##
B = {}
B = (4.0, 0.5e-4)

## --- CATCHER THICKNESS --- ##
Catcher_e = {
    "THIN": {},
    "THICK": {}
}
Catcher_e["THIN"]["Al"] = (85, 10)
Catcher_e["THIN"]["Mylar"] = (525, 15)
Catcher_e["THICK"]["Al"] = (110, 25)
Catcher_e["THICK"]["Mylar"] = (6100, 35)

## --- CATCHER POSITION --- ##
Catcher_z = {
    "2024" : (-0.5, 0.3),
    "2025" : (0.65, 0.3)
}

## --- CATCHER ANGLE --- ##
Catcher_Angle = {
    "2024" : (0., 0.),
    "2025" : (0., 0.)
}

## --- DETECTORS POSITION --- ##
# Detectors = {
#     "2024": {},
#     "2025": {}
# }

# Detectors["2024"]["x"] = 0.0
# Detectors["2024"]["y"] = 0.0
# Detectors["2024"]["z"] = 0.0
# Detectors["2024"]["Rx"] = 0.0
# Detectors["2024"]["Ry"] = 0.0

# Detectors["2025"]["x"] = 0.0
# Detectors["2025"]["y"] = 0.0
# Detectors["2025"]["z"] = 0.0
# Detectors["2025"]["Rx"] = 0.0
# Detectors["2025"]["Ry"] = 0.0

Detectors = {
    "2024" : {},
    "2025" : {}
}

#old
# Detectors["2024"]["x"] = 1.05
# Detectors["2024"]["y"] = 2.7
# Detectors["2024"]["z"] = 0.0
# Detectors["2024"]["Rx"] = 0.0
# Detectors["2024"]["Ry"] = 0.0

Detectors["2024"]["x"] = 1.05
Detectors["2024"]["y"] = 2.0
Detectors["2024"]["z"] = 0.0
Detectors["2024"]["Rx"] = 0.0
Detectors["2024"]["Ry"] = 0.0

#old
# Detectors["2025"]["x"] = -0.5
# Detectors["2025"]["y"] = 2.7
# Detectors["2025"]["z"] = 0.0
# Detectors["2025"]["Rx"] = 0.0
# Detectors["2025"]["Ry"] = 0.0

Detectors["2025"]["x"] = -0.4
Detectors["2025"]["y"] = 2.65
Detectors["2025"]["z"] = 0.0
Detectors["2025"]["Rx"] = 0.0
Detectors["2025"]["Ry"] = 0.0

## --- CRADLE PARAMETERS --- ##
CRADLE = {}
CRADLE["CV"] = 1.0
CRADLE["CVP"] = 1.0
CRADLE["CS"] = 0.0
CRADLE["CSP"] = 0.0
CRADLE["CA"] = 1.27
CRADLE["CAP"] = 1.27
CRADLE["CT"] = 0.0
CRADLE["CTP"] = 0.0

CRADLE["a"] = 1.0
CRADLE["b"] = 0.0

def GetA(Nucleus):
    return int(''.join(filter(str.isdigit, Nucleus)))

def GetZ(Nucleus):
    name = ''.join(filter(str.isalpha, Nucleus))
    periodic_table = {
        'H': 1, 'He': 2, 'Li': 3, 'Be': 4, 'B': 5, 'C': 6, 'N': 7, 'O': 8, 'F': 9, 'Ne': 10,
        'Na': 11, 'Mg': 12, 'Al': 13, 'Si': 14, 'P': 15, 'S': 16, 'Cl': 17, 'Ar': 18, 'K': 19, 'Ca': 20,
        'Sc': 21, 'Ti': 22, 'V': 23, 'Cr': 24, 'Mn': 25, 'Fe': 26, 'Co': 27, 'Ni': 28, 'Cu': 29, 'Zn': 30,
        'Ga': 31, 'Ge': 32, 'As': 33, 'Se': 34, 'Br': 35, 'Kr': 36, 'Rb': 37, 'Sr': 38, 'Y': 39, 'Zr': 40,
        'Nb': 41, 'Mo': 42, 'Tc': 43, 'Ru': 44, 'Rh': 45, 'Pd': 46, 'Ag': 47, 'Cd': 48, 'In': 49, 'Sn': 50,
        'Sb': 51, 'Te': 52, 'I': 53, 'Xe': 54, 'Cs': 55, 'Ba': 56, 'La': 57, 'Ce': 58, 'Pr': 59, 'Nd': 60,
        'Pm': 61, 'Sm': 62, 'Eu': 63, 'Gd': 64, 'Tb': 65, 'Dy': 66, 'Ho': 67, 'Er': 68, 'Tm': 69, 'Yb': 70,
        'Lu': 71, 'Hf': 72, 'Ta': 73, 'W': 74, 'Re': 75, 'Os': 76, 'Ir': 77, 'Pt': 78, 'Au': 79, 'Hg': 80,
        'Tl': 81, 'Pb': 82, 'Bi': 83, 'Po': 84, 'At': 85, 'Rn': 86, 'Fr': 87, 'Ra': 88, 'Ac': 89, 'Th': 90,
        'Pa': 91, 'U': 92, 'Np': 93, 'Pu': 94, 'Am': 95, 'Cm': 96, 'Bk': 97, 'Cf': 98, 'Es': 99, 'Fm': 100,
        'Md': 101, 'No': 102, 'Lr': 103, 'Rf': 104, 'Db': 105, 'Sg': 106, 'Bh': 107, 'Hs': 108, 'Mt': 109,
        'Ds': 110, 'Rg': 111, 'Cn': 112, 'Nh': 113, 'Fl': 114, 'Mc': 115, 'Lv': 116, 'Ts': 117, 'Og': 118
    }
    return periodic_table.get(name, None)

def CheckDirectory(path, createifnotexist):
    # check if exist else create
    try:
        os.makedirs(path, exist_ok=createifnotexist)
    except Exception as e:
        print("Error creating directory {}: {}".format(path, e))

def VerifyVariable(map, Var_Names):

    # empty
    if (len(map) == 0):
        print("No variable to vary")
        return 1
    
    # check if all values are arrays or bool (only one type)
    type_ = None
    for key, value in map.items():
        if type_ is None:
            if isinstance(value, bool):
                type_ = bool
            elif isinstance(value, (list, np.ndarray)):
                type_ = list
            elif isinstance(value, dict):
                for subkey, subvalue in value.items():
                    if isinstance(subvalue, bool):
                        type_ = bool
                    elif isinstance(subvalue, (list, np.ndarray)):
                        type_ = list
                    else:
                        print("<VerifyVariable> Variable values must be bool or array")
                        exit(1)
            else:
                print("<VerifyVariable> Variable values must be bool or array")
                exit(1)
        else:
            if type_ == bool and not isinstance(value, bool):
                print("<VerifyVariable> All variable values must be bool or array (first is bool)")
                exit(1)
            elif type_ == list and not isinstance(value, (list, np.ndarray, dict)):
                print("<VerifyVariable> All variable values must be bool or array (first is array)")
                exit(1)
            elif type_ == list and isinstance(value, dict):
                for subkey, subvalue in value.items():
                    if not isinstance(subvalue, (list, np.ndarray)):
                        print("<VerifyVariable> All variable values must be arrays")
                        exit(1)

    # if array check all the same len
    if type_ == list:
        length = None
        for key, value in map.items():
            ##init length
            if length is None and not isinstance(value, dict):
                length = len(value)
            elif length is None and isinstance(value, dict):
                for subkey, subvalue in value.items():
                    length = len(subvalue)
                    break
            ## compare length
            else:
                if (not isinstance(value, dict)):
                    if length != len(value):
                        print("<VerifyVariable> All variable arrays must have the same length")
                        print("Variable {} has length {}, expected {}".format(key, len(value), length))
                        exit(1)
                else:   
                    for subkey, subvalue in value.items():
                        if length != len(subvalue):
                            print("<VerifyVariable> All variable arrays must have the same length")
                            print("Variable {}.{} has length {}, expected {}".format(key, subkey, len(subvalue), length))
                            exit(1)

    # check if variable exists
    for key, value in map.items():
        if key not in Var_Names:
            print("<VerifyVariable> Variable {} not valid".format(key))
            exit(1)

    # return number of simulations
    if type_ == bool:
        return Sampling_MC
    else:
        return length

def SetParameters(Map_Variable, YEAR, Catcher_type, abMode, iter):
    Parameters = {}

    # -- CRADLE PARAMETERS -- #
    for c in ["CV", "CVP", "CS", "CSP", "CA", "CAP", "CT", "CTP"]:
        if c in Map_Variable.keys() and abMode == False:
            if isinstance(Map_Variable[c], (list, np.ndarray)):
                Parameters[c] = Map_Variable[c][iter]
            else:
                print("Error: CRADLE variable must be array/list of values")
                exit(1)
        else:
            Parameters[c] = CRADLE[c]

    for c in ["a", "b"]:
        if c in Map_Variable.keys() and abMode == True:
            if isinstance(Map_Variable[c], (list, np.ndarray)):
                Parameters[c] = Map_Variable[c][iter]
            else:
                print("Error: CRADLE variable must be array/list of values")
                exit(1)
        else:
            Parameters[c] = CRADLE[c]

    # -- Beam -- #
    if "Beam" in Map_Variable.keys():
        ## only MCMC for Beam
        if Map_Variable["Beam"] == True:
            mu_x = np.random.normal(Beam[YEAR]["x"][0], Beam[YEAR]["x"][1])
            mu_y = np.random.normal(Beam[YEAR]["y"][0], Beam[YEAR]["y"][1])
            sigma_x = np.random.normal(Beam[YEAR]["sx"][0], Beam[YEAR]["sx"][1])
            sigma_y = np.random.normal(Beam[YEAR]["sy"][0], Beam[YEAR]["sy"][1])
            Parameters["Beam"] = {
                "x": mu_x,
                "y": mu_y,
                "sx": sigma_x,
                "sy": sigma_y
            }
        else:
            Parameters["Beam"] = {
                "x": Map_Variable["Beam"]["x"][iter],
                "y": Map_Variable["Beam"]["y"][iter],
                "sx": Map_Variable["Beam"]["sx"][iter],
                "sy": Map_Variable["Beam"]["sy"][iter]
            }

    else:
        Parameters["Beam"] = {
            "x": Beam[YEAR]["x"][0],
            "y": Beam[YEAR]["y"][0],
            "sx": Beam[YEAR]["sx"][0],
            "sy": Beam[YEAR]["sy"][0]
        }      

    # -- DEAD LAYER -- #
    if "DL" in Map_Variable.keys():
        if Map_Variable["DL"] == True:
            thickness = np.random.normal(DL[0], DL[1])
            Parameters["DL"] = thickness
        else:
            Parameters["DL"] = Map_Variable["DL"][iter]
    else:
        Parameters["DL"] = DL[0]  

    # -- MAGNETIC FIELD -- #
    if "B" in Map_Variable.keys():
        if Map_Variable["B"] == True:
            field = np.random.normal(B[0], B[1])
            Parameters["B"] = field
        else:
            Parameters["B"] = Map_Variable["B"][iter]
    else:
        Parameters["B"] = B[0]

    # -- CATCHER -- #
    if "Catcher_e" in Map_Variable.keys():
        if Map_Variable["Catcher_e"] == True:
            thickness_Al = np.random.normal(Catcher_e[Catcher_type]["Al"][0], Catcher_e[Catcher_type]["Al"][1])
            thickness_Mylar = np.random.normal(Catcher_e[Catcher_type]["Mylar"][0], Catcher_e[Catcher_type]["Mylar"][1])
            Parameters["Catcher_e"] = {
                "Al": thickness_Al,
                "Mylar": thickness_Mylar
            }
        else:
            Parameters["Catcher_e"] = {
                "Al": Map_Variable["Catcher_e"]["Al"][iter],
                "Mylar": Map_Variable["Catcher_e"]["Mylar"][iter]
            }
    else:
        Parameters["Catcher_e"] = {
            "Al": Catcher_e[Catcher_type]["Al"][0],
            "Mylar": Catcher_e[Catcher_type]["Mylar"][0]
        }

    # -- CATCHER Z -- #
    if "Catcher_z" in Map_Variable.keys():
        if Map_Variable["Catcher_z"] == True:
            z_pos = np.random.normal(Catcher_z[YEAR][0], Catcher_z[YEAR][1])
            Parameters["Catcher_z"] = z_pos
        else:
            Parameters["Catcher_z"] = Map_Variable["Catcher_z"][iter]
    else:
        Parameters["Catcher_z"] = Catcher_z[YEAR][0]

    # -- CATCHER ANGLE -- #
    if "Catcher_Angle" in Map_Variable.keys():
        if Map_Variable["Catcher_Angle"] == True:
            angle = np.random.normal(Catcher_Angle[YEAR][0], Catcher_Angle[YEAR][1])
            Parameters["Catcher_Angle"] = angle
        else:
            Parameters["Catcher_Angle"] = Map_Variable["Catcher_Angle"][iter]
    else:
        Parameters["Catcher_Angle"] = Catcher_Angle[YEAR][0]

    # -- DETECTORS -- #
    Parameters["Detectors"] = {}
    if "Detectors" in Map_Variable.keys():
        Parameters["Detectors"] = {}
        for param in Detectors[YEAR].keys():
            Parameters["Detectors"][param] = Map_Variable["Detectors"][param][iter]
    else:
        Parameters["Detectors"] = {}
        for param in Detectors[YEAR].keys():
            Parameters["Detectors"][param] = Detectors[YEAR][param]

    return Parameters


def Get_Suffixe(ParametersForSim, Map_Variable):
    suffixe = ""
    for key, value in Map_Variable.items():
            if key == "Beam":
                suffixe += "_x{:.4f}_y{:.4f}_sx{:.4f}_sy{:.4f}".format(
                    ParametersForSim["Beam"]["x"],
                    ParametersForSim["Beam"]["y"],
                    ParametersForSim["Beam"]["sx"],
                    ParametersForSim["Beam"]["sy"]
                )
            elif key == "Catcher_e":
                suffixe += "_Al{:.0f}_Mylar{:.0f}".format(
                    ParametersForSim["Catcher_e"]["Al"],
                    ParametersForSim["Catcher_e"]["Mylar"]
                )
            elif key == "Detectors":
                suffixe += "_Det_x{:.2f}_y{:.2f}_z{:.2f}_Rx{:.2f}_Ry{:.2f}".format(
                    ParametersForSim["Detectors"]["x"],
                    ParametersForSim["Detectors"]["y"],
                    ParametersForSim["Detectors"]["z"],
                    ParametersForSim["Detectors"]["Rx"],
                    ParametersForSim["Detectors"]["Ry"]
                )
            elif key == "DL":
                suffixe += "_DL{:.0f}".format(ParametersForSim["DL"])
            elif key == "Catcher_z":
                suffixe += "_catcherz{:.2f}".format(ParametersForSim["Catcher_z"])
            elif key == "Catcher_Angle":
                suffixe += "_catcher{:.1f}deg".format(ParametersForSim["Catcher_Angle"])
            else:
                suffixe += "_{}{:.4f}".format(key, ParametersForSim[key])
    if suffixe == "":
        suffixe = "_Default"
    return suffixe


def Check_CRADLE_File(Nucleus, ParametersForSim, PeakConfig, abMode, CreatingNewCRADLEFile, Events, N_simulatenous):
    
    # create filename from ParametersForSim concerning CRADLE
    filename = "{}_{}".format(Nucleus, PeakConfig)
    filename_add = ""
    if not abMode:
        if CRADLE["CS"] != ParametersForSim["CS"] or CRADLE["CSP"] != ParametersForSim["CSP"]:
            filename_add += "_CS{:.4f}_CSP{:.4f}".format(ParametersForSim["CS"], ParametersForSim["CSP"])
        if CRADLE["CV"] != ParametersForSim["CV"] or CRADLE["CVP"] != ParametersForSim["CVP"]:
            filename_add += "_CV{:.4f}_CVP{:.4f}".format(ParametersForSim["CV"], ParametersForSim["CVP"])
        if CRADLE["CA"] != ParametersForSim["CA"] or CRADLE["CAP"] != ParametersForSim["CAP"]:
            filename_add += "_CA{:.4f}_CAP{:.4f}".format(ParametersForSim["CA"], ParametersForSim["CAP"])
        if CRADLE["CT"] != ParametersForSim["CT"] or CRADLE["CTP"] != ParametersForSim["CTP"]:
            filename_add += "_CT{:.4f}_CTP{:.4f}".format(ParametersForSim["CT"], ParametersForSim["CTP"])
    else:
        if CRADLE["a"] != ParametersForSim["a"]:
            filename_add += "_a{:.4f}".format(ParametersForSim["a"])
        if CRADLE["b"] != ParametersForSim["b"]:
            filename_add += "_b{:.4f}".format(ParametersForSim["b"])

    # if (PeakConfig == "IAS"):
    #     filename_add = "_a1.0_b0.0"
    # if (filename_add == "" and PeakConfig == "ENSDF"):
    #     filename_add = "_CS0_CSP0_CV1_CVP1"
            
    if filename_add == "":
        filename_add = "_a1.0_b0.0"
    filename += filename_add + ".root"

    filepath = CRADLE_DATA_Path + filename

    ## looking for existing file in the dir
    try:
        with open(filepath, 'r') as f:
            print("CRADLE file check: {}".format(filepath))
            return filepath
    except FileNotFoundError:
        print("CRADLE file not found: {}".format(filepath))
        if CreatingNewCRADLEFile:
            print("Creating new CRADLE file: {}".format(filepath))
            flagorfilename = Create_CRADLE_file(Nucleus, ParametersForSim, Events, N_simulatenous, filepath, abMode)
            return filepath
        else:
            print("Creating new CRADLE file is disabled.")
            exit(1)


def CreateMacro_CRADLE(ParametersForSim, abMode):
    macro_base_filename = CRADLE_Path + "config/config_base.txt"
    ## copy the base macro to a new file

    if not abMode:
        macro_filename = CRADLE_Path + "config/macro_CS{:.4f}_CSP{:.4f}_CV{:.4f}_CVP{:.4f}_CA{:.4f}_CAP{:.4f}_CT{:.4f}_CTP{:.4f}.txt".format(
            ParametersForSim["CS"], ParametersForSim["CSP"],
            ParametersForSim["CV"], ParametersForSim["CVP"],
            ParametersForSim["CA"], ParametersForSim["CAP"],
            ParametersForSim["CT"], ParametersForSim["CTP"]
        )
    else:
        macro_filename = CRADLE_Path + "config/macro_a{:.4f}_b{:.4f}.txt".format(
            ParametersForSim["a"], ParametersForSim["b"]
        )

    ## rewrite and replace in the macro
    with open(macro_base_filename, 'r') as base_file:
        with open(macro_filename, 'w') as macro_file:
            for line in base_file:
                if "%" in line:
                    if "CV" in line:
                        line = "CV={:.4f}\n".format(ParametersForSim["CV"])
                    elif "CVP" in line:
                        line = "CVP={:.4f}\n".format(ParametersForSim["CVP"])
                    elif "CS" in line:
                        line = "CS={:.4f}\n".format(ParametersForSim["CS"])
                    elif "CSP" in line:
                        line = "CSP={:.4f}\n".format(ParametersForSim["CSP"])
                    elif "CA" in line:
                        line = "CA={:.4f}\n".format(ParametersForSim["CA"])
                    elif "CAP" in line:
                        line = "CAP={:.4f}\n".format(ParametersForSim["CAP"])
                    elif "CT" in line:
                        line = "CT={:.4f}\n".format(ParametersForSim["CT"])
                    elif "CTP" in line:
                        line = "CTP={:.4f}\n".format(ParametersForSim["CTP"])
                    
                    if not abMode:
                        if "a=" in line :
                            line = "a=NaN\n"
                        elif "b=" in line :
                            line = "b=NaN\n"
                    else:       
                        if "a=" in line :
                            line = "a={:.4f}\n".format(ParametersForSim["a"])
                        elif "b=" in line :
                            line = "b={:.4f}\n".format(ParametersForSim["b"])                    

                macro_file.write(line)

    return macro_filename   


def Create_CRADLE_file(Nucleus, ParametersForSim, events, therad, CRADLE_filename, abMode):

    ## CREATE the macro
    macro_filename = CreateMacro_CRADLE(ParametersForSim, abMode)

    ## RUN CRADLE
    ## cd in CRADLE path
    os.chdir(CRADLE_Path+"build/")
    command = f"CRADLE++ nucleus --name {Nucleus} -Z {GetZ(Nucleus)} -A {GetA(Nucleus)} -c {macro_filename} general -l {events:.0f} -t {therad} -o {CRADLE_filename}"
    print("Running command: {}".format(command))
    os.system(command)
    ## delete macro
    os.remove(macro_filename)

    return True

def CreateMacro_Geant4(suffixe, ParametersForSim, CRADLE_filename, YEAR, FIELD_MAP, CAD_MESH, events, N_simulatenous, Nucleus, PeakConfig, Catcher_type):
    macro_base_filename = Geant4_Path + "macro_base_python.mac"
    ## copy the base macro to a new file
    macro_filename = Geant4_Path + f"tempory/macro{suffixe}" + ".mac"

    ## rewrite and replace in the macro
    with open(macro_base_filename, 'r') as base_file:
        with open(macro_filename, 'w') as macro_file:
            for line in base_file:
                if "%" in line:
                    if "/Input/CRADLE" in line:
                        line = f"/Input/CRADLE {CRADLE_filename}\n"
                    elif "/Run/Threads" in line:
                        line = f"/Run/Threads {N_simulatenous}\n"
                    elif "/Beam/X" in line:
                        line = "/Beam/X {:.4f} mm\n".format(ParametersForSim["Beam"]["x"])
                    elif "/Beam/Y" in line:
                        line = "/Beam/Y {:.4f} mm\n".format(ParametersForSim["Beam"]["y"])
                    elif "/Beam/Sigma_X" in line:
                        line = "/Beam/Sigma_X {:.4f} mm\n".format(ParametersForSim["Beam"]["sx"])
                    elif "/Beam/Sigma_Y" in line:
                        line = "/Beam/Sigma_Y {:.4f} mm\n".format(ParametersForSim["Beam"]["sy"])
                    elif "/Geometry/Collimator" in line:
                        if YEAR == "2024":
                            line = f"/Geometry/Collimator true\n"
                        elif YEAR == "2025":
                            line = f"/Geometry/Collimator false\n"
                    elif "/Beam/Radius" in line:
                        if YEAR == "2024":
                            line = f"/Beam/Radius 5.0 mm\n"
                        elif YEAR == "2025":
                            line = f"/Beam/Radius 1000 mm\n"
                    elif "/Geometry/Magnetic_Field_Value" in line:
                        line = "/Geometry/Magnetic_Field_Value {:.4f} tesla\n".format(ParametersForSim["B"])
                    elif "/Geometry/Magnetic_Field_Mapping" in line:
                        if FIELD_MAP:
                            line = f"/Geometry/Magnetic_Field_Mapping true\n"
                        else:
                            line = f"/Geometry/Magnetic_Field_Mapping false\n"
                    elif "/Geometry/CAD_MESH" in line:
                        if CAD_MESH:
                            line = f"/Geometry/CAD_MESH true\n"
                        else:
                            line = f"/Geometry/CAD_MESH false\n"
                    elif "/Geometry/SiDeadLayer_Thickness" in line:
                       line = "/Geometry/SiDeadLayer_Thickness {:.2f} nm\n".format(ParametersForSim["DL"])
                    elif f"/Geometry/Detectors" in line:
                        line = "/Geometry/Detectors {:.2f} {:.2f} {:.2f} {:.2f} {:.2f}\n".format(
                                ParametersForSim["Detectors"]["x"],
                                ParametersForSim["Detectors"]["y"],
                                ParametersForSim["Detectors"]["z"],
                                ParametersForSim["Detectors"]["Rx"],
                                ParametersForSim["Detectors"]["Ry"]
                            )
                    elif "/Geometry/Catcher_Position_z" in line:
                        line = "/Geometry/Catcher_Position_z {:.2f} mm\n".format(ParametersForSim["Catcher_z"])
                    elif "/Geometry/Catcher_Angle" in line:
                        line = "/Geometry/Catcher_Angle {:.2f} deg\n".format(ParametersForSim["Catcher_Angle"])
                    elif "/Geometry/Catcher_Thickness_Al1" in line:
                        line = "/Geometry/Catcher_Thickness_Al1 {} nm\n".format(ParametersForSim["Catcher_e"]["Al"])
                    elif "/Geometry/Catcher_Thickness_Mylar" in line:
                        line = "/Geometry/Catcher_Thickness_Mylar {} nm\n".format(ParametersForSim["Catcher_e"]["Mylar"])
                    elif "/Geometry/Catcher_Thickness_Al2" in line:
                        line = "/Geometry/Catcher_Thickness_Al2 {} nm\n".format(ParametersForSim["Catcher_e"]["Al"])
                    elif "/Run/File" in line:
                        if (Catcher_type == "THIN"):
                            if (CAD_MESH):
                                output_filename = f"{Geant4_DATA_Path}{Nucleus}_{PeakConfig}_{YEAR}_CAD{suffixe}.root"
                            else:
                                output_filename = f"{Geant4_DATA_Path}{Nucleus}_{PeakConfig}_{YEAR}{suffixe}.root"
                        else:
                            if (CAD_MESH):
                                output_filename = f"{Geant4_DATA_Path}{Nucleus}_{PeakConfig}_{YEAR}_CAD_{Catcher_type}{suffixe}.root"
                            else:
                                output_filename = f"{Geant4_DATA_Path}{Nucleus}_{PeakConfig}_{YEAR}_{Catcher_type}{suffixe}.root"
                        line = f"/Run/File {output_filename}\n"                
                    elif "/run/beamOn" in line:
                        line = f"/run/beamOn {events:.0f}"
                    
                macro_file.write(line)

    return macro_filename, output_filename

def Running_Geant4(suffixe, ParametersForSim, CRADLE_filename, YEAR, FIELD_MAP, CAD_MESH, Events, N_simulatenous, Nucleus, PeakConfig, Catcher_type):
    ## CREATE MACRO
    macro_filename, output_filename = CreateMacro_Geant4(suffixe, ParametersForSim, CRADLE_filename, YEAR, FIELD_MAP, CAD_MESH, Events, N_simulatenous, Nucleus, PeakConfig, Catcher_type)
    
    #check if output file already exists
    try:
        with open(output_filename, 'r') as f:
            print("Output file already exists, skipping Geant4 simulation: {}".format(output_filename))
            #remove macro file
            os.remove(macro_filename)
            return output_filename
    except FileNotFoundError:
        print("Creating Geant4 output: {}".format(output_filename))

    ## RUN GEANT4
    ## cd in Geant4 path
    os.chdir(Geant4_Path)
    command = f"wisard {macro_filename}"
    print("Running command: {}".format(command))
    os.system(command)

    ## REMOVE MACRO
    os.remove(macro_filename)

    return output_filename

def Parse(Variable, map_var):
    for key, value in map_var.items():
        prefix = ""
        suffix = "\033[0m"
        if key in Variable.keys():
            prefix = '\033[31m'
        if key == "Beam":
            print(prefix, "Beam: x={:.4f} y={:.4f} sx={:.4f} sy={:.4f}".format(value["x"], value["y"], value["sx"], value["sy"]), suffix)
        elif key == "DL":
            print(prefix, "Dead Layer Thickness: {:.2f} nm".format(value), suffix)
        elif key == "B":
            print(prefix, "Magnetic Field: {:.4f} T".format(value), suffix)
        elif key == "Catcher_e":
            print(prefix, "Catcher Thickness: Al={:.2f} nm Mylar={:.2f} nm".format(value["Al"], value["Mylar"]), suffix)
        elif key == "Catcher_z":
            print(prefix, "Catcher Z Position: {:.2f} mm".format(value), suffix)
        elif key == "Catcher_Angle":
            print(prefix, "Catcher Angle: {:.2f} deg".format(value), suffix)
        elif key == "Detectors":
            print(prefix, "Detectors : x={:.2f} y={:.2f} z={:.2f} Rx={:.2f} Ry={:.2f}".format(value["x"], value["y"], value["z"], value["Rx"], value["Ry"]), suffix)
        else:
            print(prefix, "{}: {:.4f}".format(key, value), suffix)
