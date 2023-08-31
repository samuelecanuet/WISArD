from ROOT import *
import numpy as np
from array import array
import sys
from function_root import *
import os
import re
import matplotlib.pyplot as plt
import seaborn as sns



custom_params = {
        "xtick.direction" : "in",
        "ytick.direction" : "in",
        "lines.markeredgecolor" : "k",
        "lines.markeredgewidth" : 0.5,
        "lines.linewidth" : 1,
        "lines.markersize" : 5,
        "figure.figsize" : (5.5,5),
        "font.family" : "serif",
        "ytick.labelsize" : 20,
        "xtick.labelsize" : 20,
        "axes.labelsize" : 20,
        "axes.titlesize" : 20,
        "legend.fontsize" : 15,
        "text.usetex" : True,
        }

def infos(value, bins):
    maxi =     maxi = list(value).index(max(value))/10-250
    value = [value[i] for i in range(len(value)) if bins[i] > maxi]
    bins = [bin for bin in bins if bin > maxi]
    bin_centers = 0.5 * (np.array(bins[:-1]) + np.array(bins[1:]))
    mean = np.sum(bin_centers * np.array(value[:-1])) / np.sum(value[:-1])
    rms = np.sqrt(np.sum(bin_centers * np.array(value[:-1])**2)) / np.sum(value[:-1])
    integral = np.sum(value)

    return mean, rms, int(integral)

def display_infos( bins, coinc, nocoinc, single ):

    E_coinc, RMS_coinc, N_coinc = infos(coinc, x)
    E_nocoinc, RMS_nocoinc, N_nocoinc = infos(nocoinc, x)
    E_single, RMS_single, N_single = infos(single, x)

    shift = abs((E_coinc-E_nocoinc)*(1-N_coinc/N_single))
    sigma_shift = np.sqrt((1-N_coinc/N_single)**2*((RMS_coinc/sqrt(N_coinc))**2+(RMS_nocoinc/sqrt(N_nocoinc))**2) + ((abs(E_coinc-E_nocoinc))*(N_nocoinc/N_single**2))**2*(N_coinc+N_coinc**2/N_nocoinc))
    print("SINGLE : \tN = {} \t E = {:.2f}".format(N_single, E_single))
    print("COINC : \tN = {} \t E = {:.2f}".format(N_coinc, E_coinc))
    print("NCOINC : \tN = {} \t E = {:.2f}".format(N_nocoinc, E_nocoinc))
    print("SHIFT : \tEshift = {:.2f} +/- {:.2f}\n".format(shift, sigma_shift))

    return shift, sigma_shift

def root_hist_name(title):
    index=[]
    for i in title:
        try:
            int(i)
            index.append(title.index(i))
        except:
            continue
    return title[max(index)+1:]

def root_hist_to_matplotlib(root_hist):
    n_bins = root_hist.GetNbinsX()
    x_edges = np.array([root_hist.GetBin(i) for i in range(0, n_bins + 1)])/10
    y_values = np.array([root_hist.GetBinContent(i) for i in range(0, n_bins + 1)])

    return x_edges, y_values

filename = sys.argv[1]
file = TFile(filename)
liste_name = sorted([i.GetName() for i in file.GetListOfKeys() if i.GetName() != "MyTree"])

dic_hist={}
dic_hist["Up"]=[0,[]]
dic_hist["Down"]=[0,[]]

for dir in dic_hist.keys():
    sum_dir_coinc = np.sum(root_hist_to_matplotlib(file.Get(title))[1] for title in liste_name if dir in title and '_coinc' in title)
    sum_dir_nocoinc = np.sum(root_hist_to_matplotlib(file.Get(title))[1] for title in liste_name if dir in title and '_nocoinc' in title)
    sum_dir_single = sum_dir_coinc + sum_dir_nocoinc
    x = root_hist_to_matplotlib(file.Get("1Up_Strip_1_coinc"))[0]

    display_infos(x, sum_dir_coinc, sum_dir_nocoinc, sum_dir_single)

    plt.step(x, sum_dir_coinc, color="red", label="Coïncidence")
    plt.step(x, sum_dir_nocoinc, color='blue', label="Anti-Coïncidence")
    plt.step(x, sum_dir_single, color='black', label="Single")
    plt.title(dir)
    off=150
    plt.xlim(list(sum_dir_single).index(max(sum_dir_single))/10-off, list(sum_dir_single).index(max(sum_dir_single))/10+off)
    plt.ylabel("Comptage /keV")
    plt.xlabel(r"$E_p$ (keV)")
    plt.legend(loc="best")
    plt.show()
