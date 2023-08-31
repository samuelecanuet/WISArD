from ROOT import *
import numpy as np
from array import array
import sys
from function_root import *
import os
import re
import matplotlib.pyplot as plt
import seaborn as sns
from scipy.optimize import curve_fit

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
sns.set_theme(style="ticks", rc=custom_params)

def infos(value, bins, maxi):
    bins = [bin for bin in bins if bin > maxi-40]
    bins = [bin for bin in bins if bin < maxi+40]
    value = [value[i] for i in range(len(value)) if bins[-1]*10 >= i]
    value = [value[i] for i in range(len(value)) if bins[0]*10 <= i]

    mean = np.sum((np.array(bins[:-1])) * np.array(value)) / np.sum(value)
    rms = np.sqrt(np.sum(bins[:-1] * np.array(value)**2)) / np.sum(value)
    integral = np.sum(value)

    return mean, rms, int(integral)

def display_infos( bins, coinc, nocoinc, single ):

    maxi = bins[list(single).index(max(single))]

    E_coinc, RMS_coinc, N_coinc = infos(coinc, x, maxi)
    E_nocoinc, RMS_nocoinc, N_nocoinc = infos(nocoinc, x, maxi)
    E_single, RMS_single, N_single = infos(single, x, maxi)

    shift = abs((E_coinc-E_nocoinc)*(1-N_coinc/N_single))
    sigma_shift = np.sqrt((1-N_coinc/N_single)**2*((RMS_coinc/sqrt(N_coinc))**2+(RMS_nocoinc/sqrt(N_nocoinc))**2) + ((abs(E_coinc-E_nocoinc))*(N_nocoinc/N_single**2))**2*(N_coinc+N_coinc**2/N_nocoinc))
    print("SINGLE : \tN = {} \t E = {:.2f}".format(N_single, E_single))
    print("COINC : \tN = {} \t E = {:.2f}".format(N_coinc, E_coinc))
    print("NOCOINC : \tN = {} \t E = {:.2f}".format(N_nocoinc, E_nocoinc))
    print("SHIFT : \tEshift = {:.3f} +/- {:.3f}\n".format(shift, sigma_shift))

    return shift, sigma_shift

def root_hist_to_matplotlib(root_hist):
    n_bins = root_hist.GetNbinsX()
    x_edges = np.array([root_hist.GetBinCenter(i) for i in range(0, n_bins + 1)])
    y_values = np.array([root_hist.GetBinContent(i) for i in range(0, n_bins + 1)])
    return x_edges, y_values

def linear(x, a, b):
    return a*x+b

list_nuclei = ["Ar", "Mg"]
list_color = ["C0", "C1"]
list_b = [-1, -0.25, -0.75, -0.5, 0, 0.25, 0.5, 0.75, 1]
dic_shift={}
for nuclei in list_nuclei:
    dic_shift[nuclei]={}
    # result=[]
    # pool = multiprocessing.Pool()
    for b in list_b:
        dic_shift[nuclei][b]={}
        filename = f"../../../../../../mnt/hgfs/shared/save_ab/Nuclei/{nuclei}/{nuclei}_a1_b{b}.root"
        file = TFile(filename)
        liste_name = sorted([i.GetName() for i in file.GetListOfKeys() if i.GetName() != "MyTree"])
        print(f"{filename} Running ...")
        for dir in ["Up", "Down"]:
            sum_dir_coinc = np.sum(root_hist_to_matplotlib(file.Get(title))[1] for title in liste_name if dir in title and '_coinc' in title)
            sum_dir_nocoinc = np.sum(root_hist_to_matplotlib(file.Get(title))[1] for title in liste_name if dir in title and '_nocoinc' in title)
            sum_dir_single = sum_dir_coinc + sum_dir_nocoinc
            x = root_hist_to_matplotlib(file.Get("1Up_Strip_1_coinc"))[0]

            print(f"### {dir} ###")
            dic_shift[nuclei][b][dir] = display_infos(x, sum_dir_coinc, sum_dir_nocoinc, sum_dir_single)

            # plt.step(x, sum_dir_coinc)
            # plt.step(x, sum_dir_nocoinc)
            # plt.step(x, sum_dir_single)
            # off=40
            # maxi = list(sum_dir_single).index(max(sum_dir_single))/10
            # plt.xlim(maxi-off, maxi+off)
            # plt.show()
    # pool.close()
    # pool.join()


list_b = np.array(list_b)
dic_param={}
for dir in ["Up", "Down"]:
    for nuclei, color in zip (list_nuclei, list_color):
        plt.errorbar(list_b, [dic_shift[nuclei][b][dir][0] for b in list_b], yerr=[dic_shift[nuclei][b][dir][1] for b in list_b], fmt=".", capsize=5, color=color)
        param, err = curve_fit(linear, list_b, [dic_shift[nuclei][b][dir][0] for b in list_b], sigma = [dic_shift[nuclei][b][dir][1] for b in list_b])
        plt.plot(list_b, linear(list_b, param[0], param[1]), color=color, label = nuclei)
        dic_param[nuclei] = [abs(param[0]), np.sqrt(np.diag(err))[0]]
    plt.title(dir)
    plt.xlabel("b")
    plt.ylabel(r"$\overline{E}_{shift}$")
    plt.legend()
    plt.show()
    plt.savefig(dir)

    Ar, err_Ar = dic_param['Ar'][0], dic_param['Ar'][1]
    Mg, err_Mg = dic_param['Mg'][0], dic_param['Mg'][1]
    res = (Mg-Ar)/Ar
    err = np.sqrt((err_Mg/Ar)**2+(err_Ar*Mg/Ar**2)**2)
    print("{} : {:.2f} +/- {:.2f}".format(dir, res, err))
print("\n")
