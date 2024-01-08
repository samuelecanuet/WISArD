from ROOT import *
import numpy as np
import re
import os
import subprocess
import matplotlib.pyplot as plt
import seaborn as sns
from matplotlib.ticker import MaxNLocator


custom_params = {
        "xtick.direction" : "out",
        "ytick.direction" : "out",
        "lines.markeredgecolor" : "k",
        "lines.markeredgewidth" : 0.5,
        "lines.linewidth" : 1,
        "lines.markersize" : 5,
        "figure.figsize" : (16,9),
        "font.family" : "serif",
        "ytick.labelsize" : 20,
        "xtick.labelsize" : 20,
        "axes.labelsize" : 20,
        "axes.titlesize" : 20,
        "legend.fontsize" : 15,
        "text.usetex" : True,
        # 'figure.subplot.left': 0.20, 
        # 'figure.subplot.bottom': 0.15, 
        # 'figure.subplot.right': 0.95, 
        # 'figure.subplot.top': 0.90
        }
sns.set_theme(style="ticks", rc=custom_params)


class ROOT_HISTO_Analyzer:
    def __init__(self, filename):
        self.filename = filename
        self.open_file()

    def open_file(self):
        self.rootfile = TFile.Open(self.filename)

        if not self.rootfile or self.rootfile.IsZombie():
            raise FileNotFoundError(f"ROOT file {self.filename} can not be open")
    
    def FindZ(self, nuclei):
        return ["H", "He", "Li", "Be", "B", "C", "N", "O", "F", "Ne", "Na", "Mg", "Al", "Si", "P", "S", "Cl", "Ar", "K", "Ca", "Sc", "Ti", "V", "Cr", "Mn", "Fe", "Co", "Ni", "Cu", "Zn", "Ga", "Ge", "As", "Se", "Br", "Kr", "Rb", "Sr", "Y", "Zr", "Nb", "Mo", "Tc", "Ru", "Rh", "Pd", "Ag", "Cd", "In", "Sn", "Sb", "Te", "I", "Xe", "Cs", "Ba", "La", "Ce", "Pr", "Nd", "Pm", "Sm", "Eu", "Gd", "Tb", "Dy", "Ho", "Er", "Tm", "Yb", "Lu", "Hf", "Ta", "W", "Re", "Os", "Ir", "Pt", "Au", "Hg", "Tl", "Pb", "Bi", "Po", "At", "Rn", "Fr", "Ra", "Ac", "Th", "Pa", "U", "Np", "Pu", "Am", "Cm", "Bk", "Cf", "Es", "Fm", "Md", "No", "Lr", "Rf", "Db", "Sg", "Bh", "Hs", "Mt"].index(nuclei)+1
    
    def GetMaximum(self):
        match = re.match(r'(\d+)([A-Za-z]+)', os.path.basename(self.filename))
        self.A = int(match.group(1))
        self.nuclei = match.group(2)
        self.Z = int(self.FindZ(self.nuclei))

        if self.A-self.Z < self.Z:
            self.Z_daughter = self.Z-1
        else:
            self.Z_daughter = self.Z+1

        ###Spin Parent
        with open(f"../../CRADLE-master/GammaData/z{self.Z}.a{self.A}", "r") as file:
            for line in file.readlines():
                columns = line.split()
                if columns and columns[0][0] == '0' and columns[1][0] == '-':
                    Spin_Parent = columns[4]
        
        ###Beta Decay
        dic_beta = {}
        with open(f"../../CRADLE-master/RadiationData/z{self.Z}.a{self.A}", "r") as file:
            for line in file.readlines():
                if ("BetaPlus" in line or "BetaMinus" in line) and len(line.split()) == 5:
                    dic_beta[round(float(line.split()[1]))] = []

        ###Spin Daughter

        ###Delayed Decay
        dic_delayed = {}
        with open(f"../../CRADLE-master/RadiationData/z{self.Z_daughter}.a{self.A}", "r") as file:
            for line in file.readlines():
                if line.split()[0] == 'P':
                    key = round(float(line.split()[1]))
                elif (line.split()[0] == 'Proton' or line.split()[0] == 'Alpha') and len(line.split()) >= 4:
                    for key_dic in dic_beta.keys():
                        if key_dic+10 > key and key_dic-10 < key:
                                dic_beta[key_dic].append([line.split()[3], line.split()[4]])
                                break
        
class ROOT_DISPLAY:
    def __init__(self, rootfile):
        self.rootfile = rootfile
    
    ###ROOT File informations
    def GetHist(self, name):
        return self.rootfile.Get(name)
    
    def ViewHist(self):
        for key in self.rootfile.GetListOfKeys():
            print("Name : "+self.rootfile.Get(key.GetName()).GetName()+"\t\t\t Title : "+self.rootfile.Get(key.GetName()).GetTitle())
            print(self.rootfile.Get(key.GetName()).ClassName())

    ###Calculation
    def Eshift(self, hist):
        hist_coinc, hist_nocoinc = hist[0], hist[1]
        strip = int(hist_coinc.GetName()[-1])
        dir = "".join(list(hist_coinc.GetName())[0:hist_coinc.GetName().index("_")])
        single = sum([hist_coinc, hist_nocoinc], TH1D(f"{dir}_single_Strip_{strip}", f"{dir}_single_Strip_{strip}", 100000, 0, 10000))
        maxi = single.GetMaximumBin()*single.GetBinWidth(1)

        
        E_coinc, RMS_coinc, N_coinc = self.Infos(hist_coinc, maxi)
        E_nocoinc, RMS_nocoinc, N_nocoinc = self.Infos(hist_nocoinc, maxi)
        E_single, RMS_single, N_single = self.Infos(single, maxi)
        shift = abs((E_coinc-E_nocoinc)*(1-N_coinc/N_single))
        sigma_shift = np.sqrt((1-N_coinc/N_single)**2*((RMS_coinc/sqrt(N_coinc))**2+(RMS_nocoinc/sqrt(N_nocoinc))**2) + ((abs(E_coinc-E_nocoinc))*(N_nocoinc/N_single**2))**2*(N_coinc+N_coinc**2/N_nocoinc))
        return shift, sigma_shift
    
    def Infos(self, hist, maxi):
        min_x, max_x = maxi-100, maxi+100
        hist.GetXaxis().SetRangeUser(min_x, max_x)
        
        return hist.GetMean(), hist.GetRMS(), hist.Integral()
    
    def GetStripHisto(self, dir, strip, hist_name):
        
        hist_coinc = sum([self.rootfile.Get(i) for i in hist_name if f"{dir}_Strip_{strip}_coinc" in i], TH1D(f"{dir}_coinc_Strip_{strip}", f"{dir}_coinc_Strip_{strip}", 100000, 0, 10000))
        hist_nocoinc = sum([self.rootfile.Get(i) for i in hist_name if f"{dir}_Strip_{strip}_nocoinc" in i], TH1D(f"{dir}_nocoinc_Strip_{strip}", f"{dir}_nocoinc_Strip_{strip}", 100000, 0, 10000))
        return [hist_coinc, hist_nocoinc]
    
    def GetEshiftDictionnary(self):
        dic={}
        for dir in ["Up", "Down"]:
            for strip in range(1, 6):
                dic[dir+str(strip)] = self.Eshift(self.GetStripHisto(dir, strip, [key.GetName() for key in self.rootfile.GetListOfKeys() if isinstance(key.ReadObj(), TH1D)]))
        return dic


    ###DISPLAY
    def Display(self, histname, axs, **kwargs):
        Hist = self.rootfile.Get(histname)
        type = Hist.ClassName()
        
        if type == "TH1D":
            self.DisplayTH1D(Hist, axs, **kwargs)
        if type == "TH2D":
            self.DisplayTH2D(Hist, axs, **kwargs)

    def DisplayTH2D(self, Hist, ax, color='plasma', label=None, title=None, xlabel=None, ylabel=None, xlim=None, ylim=None, xtick=None, ytick=None, ylog=None, xlog=None, zlog=None, rebinx=None, rebiny=None):
        if rebinx   != None: Hist.RebinX(rebinx)
        if rebiny   != None: Hist.RebinY(rebiny)

    
        nbins_x = Hist.GetNbinsX()
        nbins_y = Hist.GetNbinsY()

        hist_data = np.zeros((nbins_x, nbins_y))
        bin_centers_x = np.zeros(nbins_x)
        bin_centers_y = np.zeros(nbins_y)

        for i in range(1, nbins_x + 1):
            for j in range(1, nbins_y + 1):
                hist_data[i - 1, j - 1] = Hist.GetBinContent(i, j)
                bin_centers_x[i - 1] = Hist.GetXaxis().GetBinCenter(i)
                bin_centers_y[j - 1] = Hist.GetYaxis().GetBinCenter(j)

        if label  == None: label = Hist.GetTitle()
        if title  == None: title = Hist.GetTitle()
        if xlabel == None: xlabel = Hist.GetXaxis().GetTitle()
        if ylabel == None: ylabel = Hist.GetYaxis().GetTitle()
        if xlim   == None: xlim = ( bin_centers_x.min(), bin_centers_x.max() )
        if ylim   == None: ylim = (bin_centers_y.min(), bin_centers_y.max())
        if xtick  != None: ax.set_xticks(np.linspace(xlim[0], xlim[1], xtick))
        if ytick  != None: ax.set_yticks(np.linspace(ylim[0], ylim[1], ytick))
        if xlog   != None: ax.set_xscale('log')
        if ylog   != None: ax.set_yscale('log')

        cax = ax.imshow(hist_data.T, extent=(bin_centers_x.min(), bin_centers_x.max(), bin_centers_y.min(), bin_centers_y.max()), origin='lower', aspect='auto', cmap=color)
        cbar = plt.colorbar(cax)
        ax.set_xlabel(xlabel)
        ax.set_ylabel(ylabel)
        ax.set_title(title)
        ax.set_xlim(xlim)
        ax.set_ylim(ylim)

        if zlog     != None: cbar.set_scale('log')

        return ax

    def DisplayTH1D(self, Hist, ax, color=None, label=None, title=None, xlabel=None, ylabel=None, xlim=None, ylim=None, xtick=None, ytick=None, ylog=None, xlog=None, rebin=None, normalized=None):
        if rebin   != None: Hist.Rebin(rebin)
        if normalized == True: integral = Hist.Integral()
        else : integral = 1.

        

        nbins_x = Hist.GetNbinsX()

        hist_data = np.zeros(nbins_x)
        bin_centers_x = np.zeros(nbins_x)

        for i in range(1, nbins_x + 1):
            hist_data[i - 1] = Hist.GetBinContent(i)/integral
            bin_centers_x[i - 1] = Hist.GetXaxis().GetBinCenter(i)

        if color    == None: color = "black"
        if label    == None: label = Hist.GetTitle()               
        if title    == None: title = Hist.GetTitle()
        if xlabel   == None: xlabel = Hist.GetXaxis().GetTitle()
        if ylabel   == None: ylabel = Hist.GetYaxis().GetTitle()
        if normalized==True: ylabel = "Normalized" + ylabel
        if xlim     == None: xlim = ( bin_centers_x.min(), bin_centers_x.max() )
        if ylim     == None and hist_data.max()*1.1 > ax.get_ylim()[1] : ylim = ( 0, hist_data.max()*1.1 )
        if xtick    != None: ax.set_xticks(np.linspace(xlim[0], xlim[1], xtick))
        if ytick    != None: ax.set_yticks(np.linspace(ylim[0], ylim[1], ytick))
        if xlog     != None: ax.set_xscale('log')
        if ylog     != None: 
            ax.set_yscale('log')
            ylim = ( 1, hist_data.max()*1.1 )
        
        ax.step(bin_centers_x, hist_data, label = label, color=color)
        ax.set_xlabel(xlabel)
        ax.set_ylabel(ylabel)
        ax.set_title(title)
        ax.set_xlim(xlim)
        ax.set_ylim(ylim)

        return ax
    
if __name__ == "__main__":

    file = TFile(f"test_txt.root")
    analyse = ROOT_DISPLAY(file)
    print(analyse.GetEshiftDictionnary())
    
    # resultat = subprocess.run("g++ -o tree_reader tree_reader.cpp `root-config --cflags --libs`", shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)

    ##/////////////////////////////////////////////////////for ./back diff material
    # fig, axs = plt.subplots()
    # dic={}
    # for mat, thres, color in zip(["Pl", "Si"], [100, 100], ["blue", "orange"]):
    #     dic[mat] = [[], [], []]
    #     for i in [0.5, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10]:
    #         filename = f"32Ar_a1_b0_{i}_{mat}.root"

            
    #         file = TFile(filename)
    #         hist = file.Get("h1d_PL_Edep_positron")
    #         dic[mat][2].append(hist.Integral())
    #         hist.GetXaxis().SetRangeUser(0, thres)
    #         dic[mat][1].append(hist.Integral())
    #         dic[mat][0].append(i)

    #     print(dic[mat][0])
    #     axs.errorbar(dic[mat][0], np.array(dic[mat][1])/np.array(dic[mat][2])*100, label = mat+" Threshoold = "+str(thres)+" keV", markersize = 10, yerr = np.array(np.sqrt(dic[mat][1]))/np.array(dic[mat][2])*100, capsize=5, ecolor="black", color=color)
    #     axs.set_xticks(np.linspace(0, 10, 11))
    #     axs.set_xlabel("Inital Kinetic energy (MeV)")
    #     axs.set_ylabel("\% Losses")
    
    #     axs.set_title(r"$\beta$ Detector Material Study")
    # plt.legend()
    # plt.show()
    ##/////////////////////////////////////////////////////////////// for ./back diff T
    # fig, axs = plt.subplots()
    # dic={}
    # for tesla, thres, color in zip(["1T", "2T", "3T", "Pl"], [100, 100, 100, 100], ["blue", "orange", "red", "green"]):
    #     dic[tesla] = [[], [], []]
    #     for i in [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]:
    #         filename = f"32Ar_a1_b0_{i}_{tesla}.root"
    #         if tesla == "Pl": label = "4T" 
    #         else: label = tesla

    #         file = TFile(filename)
    #         hist = file.Get("h1d_PL_Edep_positron")
    #         hist1 = file.Get("h1d_E0_positron")
    #         dic[tesla][2].append(hist.Integral())
    #         hist.GetXaxis().SetRangeUser(0, thres)
    #         dic[tesla][1].append(hist.Integral())
    #         dic[tesla][0].append(i)
        
    #     axs.errorbar(dic[tesla][0], np.array(dic[tesla][1])/np.array(dic[tesla][2])*100, label = label, markersize = 10, yerr = np.array(np.sqrt(np.array(dic[tesla][1]))/np.array(dic[tesla][2]))*100, fmt=".", capsize=5, ecolor="black", color=color)
    #     axs.set_xticks(np.linspace(0, 10, 11))
    #     axs.set_xlabel("Inital Kinetic energy (MeV)")
    #     axs.set_ylabel("\% Losses")
    #     axs.set_title(r"$\beta$ Detector Magnetic Field Study")
    # plt.legend()
    # plt.show()
    #plt.savefig(f"Pl_position.png", dpi=300)

    ###
    # colors=["blue", "orange", "red", "green", "purple", "brown", "black", "cyan", "grey", "yellow"]
    
    # dic={}
    # for i in [1,2]:
    #     fig, axs = plt.subplots()
    #     for tesla, color in zip(["1T", "2T", "3T", "4T"],  ["blue", "orange", "red", "green"]):
    #         filename = f"32Ar_a1_b0_{i}_{tesla}.root"
    #         if tesla == "Pl": label = "4T" 
    #         else: label = tesla

    #         file = ROOT_DISPLAY(TFile(filename))
    #         print(file.GetHist("h1d_PL_Angle_positron").Integral())
    #         print(file.GetHist("h1d_PL_E0_positron").Integral())
    #         ax = file.Display("h1d_PL_Angle_positron", axs, normalized = True, label = label, color=color, title=f"Plastic Scintillator {i} MeV Positron Hit Angle")
    #     plt.legend()
    #     plt.show()
        #plt.savefig(f"Pl_position_{i}_MeV.png", dpi=300)


    ###larmor
    # B = np.linspace(1, 4, 4)
    # E = np.linspace(0, 10e6, 1e3)
    # me = 511*10**3
    # c = 3.0*10**8
    # def R(E, B):
    #     return me/B/c*np.sqrt(((E+me)/me)**2-1)*10**2
    
    # for b in B:
    #     plt.plot(E*1e-6, R(E, b), label=f"{b} T")
    # plt.xlabel("Positron energy MeV")
    # plt.ylabel("Lamor Radius (cm)")
    #plt.show()

    # analyzer = ROOT_DISPLAY(TFile("../../../../../../../mnt/hgfs/shared-2/32Ar_a1_b0_1.root"))
    # for key, value in analyzer.GetEshiftDictionnary().items():
    #     print(key[:-1] + " Strip "+key[-1] +": Eshift = {:.2f} +/- {:.2f} keV".format(value[0], value[1]))



