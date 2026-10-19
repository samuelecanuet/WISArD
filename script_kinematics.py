import os

import numpy as np
import subprocess
import multiprocessing as mp

THREAD = 88
thread = 4
DATA = [
    {"name": "mAr-10"},
    {"name": "mAr-8"},
    {"name": "mAr-6"},
    {"name": "mAr-5"},
    {"name": "mAr-3"},
    {"name": "mAr-1"},
    {"name": "mAr0"},
    {"name": "mAr1"},
    {"name": "mAr3"},
    {"name": "mAr5"},
    {"name": "mAr6"},
    {"name": "mAr8"},
    {"name": "mAr10"},

    # {"name": "mCl-100"},
    # {"name": "mCl-50"},
    # {"name": "mCl-10"},
    # {"name": "mCl-5"},
    # {"name": "mCl5"},
    # {"name": "mCl10"},
    # {"name": "mCl50"},
    # {"name": "mCl100"},

    # {"name": "mS-100"},
    # {"name": "mS-50"},
    # {"name": "mS-10"},
    # {"name": "mS-5"},
    # {"name": "mS5"},
    # {"name": "mS10"},
    # {"name": "mS50"},
    # {"name": "mS100"},

    # {"name": "Ex-20"},
    # {"name": "Ex-10"},
    # {"name": "Ex-3"},
    # {"name": "Ex-1"},
    # {"name": "Ex1"},
    # {"name": "Ex3"},
    # {"name": "Ex10"},
    # {"name": "Ex20"}
]
N = len(DATA)

import os

def run_simulations(i):

    name = DATA[i]["name"]

    print("Running G4-Scanning32ArMass with parameters:")
    print("G4-Scanning32ArMass events=100000000 N={} name={}".format(int(THREAD/thread), DATA[i]["name"]))
    subprocess.run("./G4-Scanning32ArMass events=100000000 N={} name={}".format(int(THREAD/thread), DATA[i]["name"]), shell=True, executable='/bin/tcsh')

    command = "cd ../2024_Analysis/Simulation/; ReaderNew ../../../../{}{}; cd -".format("data333/lecanuet/Result/Final/Systematics/Kinematics/", "32Ar_Kinematics{}".format(DATA[i]["name"]))
    subprocess.run(command, shell=True)

with mp.Pool(processes=thread) as pool:
    results = pool.starmap(run_simulations, [(i,) for i in range(N)])
