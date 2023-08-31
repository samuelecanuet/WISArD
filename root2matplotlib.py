from ROOT import *
import numpy as np
from array import array
import sys
from function_root import *
import os

import matplotlib.pyplot as plt
import seaborn as sns

#scipt permettant de rcupré des histo déja créer dans uun fichier root pour les afficher dans matplotlib
filename = "Nuclei/Ar/Ar_a1_b1_1.root"
#
# custom_params = {
#         "xtick.direction" : "in",
#         "ytick.direction" : "in",
#         "lines.markeredgecolor" : "k",
#         "lines.markeredgewidth" : 0.5,
#         "lines.linewidth" : 1,
#         "lines.markersize" : 5,
#         "figure.figsize" : (5.5,5),
#         "font.family" : "serif",
#         "ytick.labelsize" : 25,
#         "xtick.labelsize" : 25,
#         "axes.labelsize" : 30,
#         "axes.titlesize" : 30,
#         "legend.fontsize" : 15,
#         "text.usetex" : True,
#         }
#
# sns.set_theme(style="ticks", rc=custom_params)
#
# file = TFile(filename)
# canvas_name = "allUp"
# canvas = file.Get(canvas_name)
#
# obj_list = canvas.GetListOfPrimitives()
#
# dic_hist={}
# dic_bins={}
# # Parcourir la liste des objets
# for obj in obj_list:
#     b = obj.GetListOfPrimitives()
#     for i in b:
#         try:
#             if canvas_name in i.GetName():
#                 dic_hist[i.GetName()] = []
#                 dic_bins[i.GetName()] = []
#                 for j in range(1, i.GetNbinsX()):
#                     dic_hist[i.GetName()].append(i.GetBinContent(j))
#                     dic_bins[i.GetName()].append(i.GetBin(j))
#         except:
#             continue
#
# fig, axis = plt.subplots(1, 5, figsize=(25,6), gridspec_kw={'wspace': 0.15}, sharey = True)
#
# count = 0
# pad = 1
# for key, values in dic_bins.items():
#     count += 1
#     if count == 4:
#         count = 1
#         pad += 1
#         if pad == 6:
#             break
#     if "_coinc" in key:
#         color = "C1"
#         label = "Coïncidence"
#     elif "_nocoinc" in key:
#         color = "C0"
#         label = "Non Coïncidence"
#     else:
#         color = "black"
#         label = "Total"
#
#     ax = axis[pad-1]
#     ax.step(dic_bins[key], dic_hist[key], color = color, linewidth=0.75, label = label)
#
#     # dic_hist[key] = [dic_hist[key][i] for i in range(len(dic_hist[key])) if dic_bins[key][i] > 3350-40]
#     # dic_bins[key] = [dic_bins[key][i] for i in range(len(dic_bins[key])) if dic_bins[key][i] > 3350-40]
#     # bin_centers = 0.5 * (np.array(dic_bins[key][:-1]) + np.array(dic_bins[key][1:]))
#     # mean = np.sum(bin_centers * dic_hist[key][:-1]) / np.sum(dic_hist[key][:-1])
#     # string = str(mean)[:4]
#     # print(string)
#     # ax.annotate(r"$\overline{E}_p =$"+ str(mean)[:4] +"keV", (3317, 22000), xytext=(20, 20),
#     #     textcoords='offset points', ha='center', va='center',
#     #     bbox=dict(boxstyle='round,pad=0.5', fc='white', edgecolor='black'))
#     if pad == 5:
#         ax.legend()
#     if pad == 1:
#         ax.set_ylabel("Comptage /keV")
#     ax.set_title(f"Strip {pad}")
#     ax.set_xlabel(r"$E_p$ (keV)")
#     min = 3350-40
#     max = 3350+40
#     ax.set_xlim(min, max)
#     ax.set_xticks(np.linspace(min+10, max-10, 3))
#     ax.set_yticks(np.linspace(0, 25000, 5))
#     ax.set_ylim(0, 25000)
#
#
#
# plt.subplots_adjust(left=0.06, bottom=0.15, right=0.98, top=0.9)
# plt.savefig("strip.png", dpi=300)
# plt.show()

###########EACH STRIP DIR##########


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

file = TFile(filename)
dir="Up"
canvas_name = "all"+dir
canvas = file.Get(canvas_name)

obj_list = canvas.GetListOfPrimitives()

dic_hist={}
dic_bins={}
# Parcourir la liste des objets
for obj in obj_list:
    b = obj.GetListOfPrimitives()
    for i in b:
        try:
            if canvas_name in i.GetName():
                print(i.GetName())
                dic_hist[i.GetName()] = []
                dic_bins[i.GetName()] = []
                for j in range(1, i.GetNbinsX()):
                    dic_hist[i.GetName()].append(i.GetBinContent(j))
                    dic_bins[i.GetName()].append(i.GetBin(j))
        except:
            continue

fig, axis = plt.subplots(1, 1, figsize=(4,4))
hist_= {}
for key, value in dic_hist.items():
    try:
        hist_[key[:4+len(dir)]+key[5+len(dir):]] += np.array(value)
    except:
        hist_[key[:4+len(dir)]+key[5+len(dir):]] = np.zeros(len(value))
        hist_[key[:4+len(dir)]+key[5+len(dir):]] += np.array(value)

bins_= {}
for key, value in dic_bins.items():
    try:
        bins_[key[:4+len(dir)]+key[5+len(dir):]] = np.array(value)
    except:
        bins_[key[:4+len(dir)]+key[5+len(dir):]] = np.zeros(len(value))
        bins_[key[:4+len(dir)]+key[5+len(dir):]] = np.array(value)



for key, values in hist_.items():
    if "_coinc" in key:
        color = "red"
        label = "Coïncidence"
        E_coinc, RMS_coinc, N_coinc = infos(hist_, bins_, key)

    elif "_nocoinc" in key:
        color = "blue"
        label = "Anti-coïncidence"
        E_nocoinc, RMS_nocoinc, N_nocoinc = infos(hist_, bins_, key)
    else:
        color = "black"
        label = "Total"
        E_single, RMS_single, N_single = infos(hist_, bins_, key)

    ax = axis
    ax.step(bins_[key], hist_[key], color = color, label = label)
    ax.set_ylim(0, 120000)
    ax.set_ylabel("Comptage /keV")
    ax.set_xlabel(r"$E_p$ (keV)")

    #plt.legend()
    # plt.legend(bbox_to_anchor=(1., 1.7), loc='upper')
    # plt.tick_params(axis='x')
    # plt.tick_params(axis='y)
    min = 3350-40
    max = 3350+40
    ax.set_xlim(min, max)
    ax.set_xticks(np.linspace(min, max, 3))

shift = abs((E_coinc-E_nocoinc)*(1-N_coinc/N_single))
sigma_shift = np.sqrt((1-N_coinc/N_single)**2*((RMS_coinc/sqrt(N_coinc))**2+(RMS_nocoinc/sqrt(N_nocoinc))**2) + ((abs(E_coinc-E_nocoinc))*(N_nocoinc/N_single**2))**2*(N_coinc+N_coinc**2/N_nocoinc))
print(f"SINGLE : \tN = {N_single} \t E = {E_single}")
print(f"COINC : \tN = {N_coinc} \t E = {E_coinc}")
print(f"NCOINC : \tN = {N_nocoinc} \t E = {E_nocoinc}\n")
print(f"SHIFT : \tEshift = {shift} +/- {sigma_shift}\n")
plt.subplots_adjust(left=0.3, bottom=0.16, right=0.90, top=0.95)
plt.savefig(f"All{dir}.png", dpi=300)
plt.show()
######"All Strip dir########


######E shft sur diff fichier ##########
# liste = ["Ep3500/Ep3500_b0.root"]
# # liste = ["Qec7000/Qec7000_b0.root"]
# # liste = ["M44/M44_b0.root"]
# # liste = ["Ar/Ar_DGcs0.root", "Mg/Mg_DGcs0.root"]
# custom_params = {
#         "xtick.direction" : "in",
#         "ytick.direction" : "in",
#         "lines.markeredgecolor" : "k",
#         "lines.markeredgewidth" : 0.5,
#         "lines.linewidth" : 3,
#         "lines.markersize" : 5,
#         "figure.figsize" : (5.5,5),
#         "font.family" : "serif",
#         "ytick.labelsize" : 20,
#         "xtick.labelsize" : 20,
#         "axes.labelsize" : 20,
#         "axes.titlesize" : 20,
#         "legend.fontsize" : 15,
#         "text.usetex" : True,
#         }
#
# sns.set_theme(style="ticks", rc=custom_params)
# fig = plt.figure(figsize=(6, 6))
# axis = plt.subplot(111)
#
# for filename in liste:
#     file = TFile(filename)
#     canvas_name = "allUp"
#     canvas = file.Get(canvas_name)
#
#     obj_list = canvas.GetListOfPrimitives()
#
#     dic_hist={}
#     dic_bins={}
#     # Parcourir la liste des objets
#     for obj in obj_list:
#         b = obj.GetListOfPrimitives()
#         for i in b:
#             try:
#                 if canvas_name in i.GetName():
#                     dic_hist[i.GetName()] = []
#                     dic_bins[i.GetName()] = []
#                     for j in range(1, i.GetNbinsX()):
#                         dic_hist[i.GetName()].append(i.GetBinContent(j))
#                         dic_bins[i.GetName()].append(i.GetBin(j))
#             except:
#                 continue
#
#     for key, values in dic_bins.items():
#         print(key)
#         if "2" in key:
#             if "_coinc" in key:
#                 color = "C1"
#                 label = "Coïncidence"
#                 alpha = 1
#                 E_coinc, RMS_coinc, N_coinc = infos(dic_hist, dic_bins, key)
#
#             elif "_nocoinc" in key:
#                 color = "C0"
#                 label = "Non Coïncidence"
#                 E_nocoinc, RMS_nocoinc, N_nocoinc = infos(dic_hist, dic_bins, key)
#                 alpha = 1
#             else:
#                 color = "black"
#                 alpha = 0.2
#                 label = "Total"
#                 E_single, RMS_single, N_single = infos(dic_hist, dic_bins, key)
#
#             axis.step(dic_bins[key], dic_hist[key], color = color, linewidth=1., label = label, alpha = alpha)
#
# # axis.text(3481, 5500, r"$\approx 2\overline{E}_{shift}$", fontsize = 15)
# # axis.plot((3482, 3482), (4450, 5500), color="black", linestyle='--', linewidth = 1.5)
# # axis.plot((3495, 3495), (4700, 5500), color="black", linestyle='--', linewidth = 1.5)
# shift = abs((E_coinc-E_nocoinc)*(1-N_coinc/N_single))
# sigma_shift = np.sqrt((1-N_coinc/N_single)**2*((RMS_coinc/sqrt(N_coinc))**2+(RMS_nocoinc/sqrt(N_nocoinc))**2) + ((abs(E_coinc-E_nocoinc))*(N_nocoinc/N_single**2))**2*(N_coinc+N_coinc**2/N_nocoinc))
# axis.annotate(r"\overline{E}_{shift} = $"+"{:.2f}".format(shift)+r" $\pm$ "+"{:.2f} keV".format(sigma_shift), (3518, 4600), xytext=(20, 20),
#  textcoords='offset points', ha='center', va='center',
#  bbox=dict(boxstyle='round,pad=0.5', fc='white', edgecolor='black'))
#
# axis.legend()
# axis.set_xlim(3500-40, 3500+40)
# axis.set_ylabel("Comptage /keV")
# axis.set_xlabel(r"$E_p$ (keV)")
# plt.subplots_adjust(left=0.16, bottom=0.15, right=0.94, top=0.9)
# plt.savefig(f"{filename[:2]}_dist_avec.png", dpi=300)
# plt.show()
