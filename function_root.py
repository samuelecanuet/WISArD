from ROOT import *
import numpy as np
from array import array
import sys
import re

def HistoStrip_dir(**kwargs): #sum of all strip number n in one direction
    file = kwargs["file"]
    dir = kwargs["direction"]
    n_Strip = kwargs["Strip_number"]
    coincidence = kwargs["coincidence"]
    det_name = kwargs["det_name"]

    try:
        if kwargs["option"] == "dl":
            volume = "DeadLayer"


    except KeyError:
        volume = "DetProton"

    histo = TH1D(f"{det_name}{dir}_Strip_{n_Strip}{coincidence}{volume}", f"{det_name}{dir}_Strip_{n_Strip}{coincidence}{volume}", 100000, 0, 10000)

    if not coincidence:
        string = ""
    else:
        string = f"&& e_PlasticScintillator > {coincidence}"

    if det_name != "all":
        file.Get("MyTree").Project(f"{det_name}{dir}_Strip_{n_Strip}{coincidence}{volume}", f"e_{volume}", f"e_{volume} > 0. && n_DetProton == \"{det_name}_Strip_{n_Strip}\" {string}")

    else:
        string1=""
        for direction in ["Up", "Down"]:
            for det in range(1,5):
                for i in range(1,6):
                    if direction == dir and i == n_Strip:
                        continue
                    else:
                        string1+= f"&& n_DetProton != \"{det}{direction}_Strip_{i}\""

        file.Get("MyTree").Project(f"{det_name}{dir}_Strip_{n_Strip}{coincidence}{volume}", f"e_{volume}", f"e_{volume} > 0. {string1} {string}")

    return histo



def PlotPosition(**kwargs): #initial position of event
    file = kwargs["file"]
    Canvas = TCanvas("Position", "Position", 1600, 800)
    gStyle.SetOptStat(1111);

    padxy = TPad("padxy", "padxy", 0.0, 0.0, 0.50, 1.0)
    padz = TPad("padz", "padz", 0.60, 0.0, 1.0, 1.0)
    padxy.Draw()
    padz.Draw()

    padxy.cd()
    h2d = TH2D("hxy", "hxy", 200, -2, 2, 200, -2, 2)
    file.Get("MyTree").Project("hxy", "y.y:x.x")
    h2d.SetContour(1000)
    h2d.SetXTitle("x (mm)")
    h2d.SetYTitle("y (mm)")
    h2d.SetTitle("xy")
    h2d.SetName("Stats")
    h2d.Draw("col")

    padz.cd()
    h1d = TH1D("z", "z", 1000000, 0, 0.00015)
    file.Get("MyTree").Project("z", "z.z")
    h1d.SetXTitle("z (mm)")
    h1d.SetYTitle(f"Count/{h1d.GetBinWidth(1)}mm")
    h1d.SetTitle("z")
    h1d.SetName("Stats")
    h1d.Draw("HIST")

    return (Canvas, h2d, h1d, padxy, padz)

def plotCatcher(**kwargs):#total energy deposit in the catcher
    file = kwargs["file"]
    Canvas = TCanvas("Catcher", "Catcher", 1600, 800)

    pad1 = TPad("pad1", "pad1", 0.0, 0.0, 0.50, 1.0)
    pad2 = TPad("pad2", "pad2", 0.50, 0.0, 1.0, 1.0)
    pad1.Draw()
    pad2.Draw()

    string=""
    for det in range(1,5):
        for strip in range(1, 6):
            string += f"&& n_DetProton != \"{det}Up_Strip_{strip}\""

    pad1.cd()
    nbin=1000
    max=10000

    gStyle.SetOptStat(0)
    h2d = TH1D("h2d", "h2d", nbin, 0, max)
    file.Get("MyTree").Project("h2d", "e_Catcher", f"e_DetProton > 0. ")
    bin = [h2d.GetBin(i)*max/nbin for i in range(1, h2d.GetNbinsX()) if h2d.GetBinContent(i) > 0.]


    h2d1 = TH2D("hxy1", "hxy1", nbin, 0, max, nbin, 0, max)
    file.Get("MyTree").Project("hxy1", "e_DetProton:e_Catcher", f"e_DetProton > 0. {string}")
    h2d1.SetContour(1000)
    h2d1.SetXTitle("Energy Catcher (keV)")
    h2d1.SetYTitle("Energy Silicon Detector (keV)")
    h2d1.SetTitle("Down")
    h2d1.SetName("Stats")
    h2d1.GetXaxis().SetRangeUser(0, bin[-1]+np.std(bin))
    h2d1.GetYaxis().SetRangeUser(0, bin[-1]+np.std(bin))
    h2d1.Draw("col")

    pad2.cd()
    h2d2 = TH2D("hxy2", "hxy2", nbin, 0, max, nbin, 0, max)
    file.Get("MyTree").Project("hxy2", "e_DetProton:e_Catcher", f"e_DetProton > 0. {string.replace('Up', 'Down')}")
    h2d2.SetContour(1000)
    h2d2.SetXTitle("Energy Catcher (keV)")
    h2d2.SetYTitle("Energy Silicon Detector (keV)")
    h2d2.SetTitle("Up")
    h2d2.SetName("Stats")
    h2d2.GetXaxis().SetRangeUser(0, bin[-1]+np.std(bin))
    h2d2.GetYaxis().SetRangeUser(0, bin[-1]+np.std(bin))
    h2d2.Draw("col")

    return (Canvas, h2d1, h2d2)

def plot_catcher_strip(**kwargs):#energy deposit in the catcher for a strip and a direction contribution
    file = kwargs["file"]
    strip = kwargs["strip"]
    Canvas = TCanvas(f"Catcher {strip}", f"Catcher {strip}", 1600, 800)

    pad1 = TPad("pad1", "pad1", 0.0, 0.0, 0.50, 1.0)
    pad2 = TPad("pad2", "pad2", 0.50, 0.0, 1.0, 1.0)
    pad1.Draw()
    pad2.Draw()

    string1=""
    for det in range(1,5):
        for dir in ["Up", "Down"]:
            for i in range(1,6):
                if dir in strip and str(i) in strip:
                    continue
                else:
                    string1+= f"&& n_DetProton != \"{det}{dir}_Strip_{i}\""


    pad2.cd()
    pad2.SetLogy()
    gStyle.SetOptStat(1111)
    nbin=1000
    max=10000
    h2d2 = TH1D("hxy2", "hxy2", nbin, 0, max)
    file.Get("MyTree").Project("hxy2", "e_Catcher", f"e_DetProton > 0. {string1} ")
    h2d2.SetXTitle("Energy Catcher (keV)")
    h2d2.SetYTitle(f"Count/{int(h2d2.GetBinWidth(1))}keV")
    h2d2.SetTitle(strip)
    h2d2.SetName("Stats")
    bin = [h2d2.GetBin(i)*max/nbin for i in range(1, h2d2.GetNbinsX()) if h2d2.GetBinContent(i) > 0.]
    h2d2.GetXaxis().SetRangeUser(0, bin[-1]+np.std(bin))
    h2d2.Draw()

    pad1.cd()
    gStyle.SetOptStat(0)
    h2d1 = TH2D("hxy1", "hxy1", nbin, 0, max, nbin, 0, max)
    file.Get("MyTree").Project("hxy1", "e_DetProton:e_Catcher", f"e_DetProton > 0. {string1}")
    h2d1.SetContour(1000)
    h2d1.SetXTitle("Energy Catcher (keV)")
    h2d1.SetYTitle("Energy Silicon Detector (keV)")
    h2d1.SetTitle(strip)
    h2d1.SetName("Stats")
    h2d1.GetXaxis().SetRangeUser(0, bin[-1]+np.std(bin))
    h2d1.GetYaxis().SetRangeUser(0, bin[-1]+np.std(bin))
    h2d1.Draw("col")



    return (Canvas, h2d1, h2d2)

def PlotEnergy(**kwargs):#initial energy of emited particle
    file = kwargs['file']
    c = []

    Canvas_proton = TCanvas("Energy_proton", "Energy_proton", 1600, 800)
    c.append(Canvas_proton)
    h2 = TH1D("h2", "h2", 10000, 0, 10000)
    file.Get("MyTree").Project("h2", "e_proton")
    h2.SetXTitle("Energy (keV)")
    h2.SetYTitle(f"Count/{int(h2.GetBinWidth(1))}keV")
    h2.SetTitle("Proton")
    h2.SetName("Stats")
    bin = [h2.GetBin(i) for i in range(1, h2.GetNbinsX()) if h2.GetBinContent(i) > 0.]
    h2.GetXaxis().SetRangeUser(2*bin[0]-np.mean(bin), 2*bin[-1]-np.mean(bin))
    h2.Draw()

    Canvas_positron = TCanvas("Energy_positron", "Energy_positron", 1600, 800)
    c.append(Canvas_positron)
    h1 = TH1D("h1", "h1", 10000, 0, 10000)
    file.Get("MyTree").Project("h1", "e_positron")
    h1.SetXTitle("Energy (keV)")
    h1.SetYTitle(f"Count/{int(h1.GetBinWidth(1))}keV")
    h1.SetTitle("Positron")
    h1.SetName("Stats")
    bin = [h1.GetBin(i) for i in range(1, h1.GetNbinsX()) if h1.GetBinContent(i) > 0.]
    h1.GetXaxis().SetRangeUser(2*bin[0]-np.mean(bin), 2*bin[-1]-np.mean(bin))
    h1.Draw()

    return (c, h2, h1)

def PlotPlasticScintillator(**kwargs):
    file = kwargs["file"]

    Canvas = TCanvas("PlasticScintillator", "PlasticScintillator", 1600, 800)
    Canvas.SetLogy()
    gStyle.SetOptStat(0)
    h1 = TH1D("h1", "h1", 10000, 0, 10000)
    file.Get("MyTree").Project("h1", "e_PlasticScintillator", "e_PlasticScintillator > 0.")
    h1.SetXTitle("Energy (keV)")
    h1.SetYTitle(f"Count/{int(h1.GetBinWidth(1))}keV")
    h1.SetTitle("Plastic Scintillator")
    h1.SetName("Stats")
    bin = [h1.GetBin(i) for i in range(1, h1.GetNbinsX()) if h1.GetBinContent(i) > 0.]
    h1.GetXaxis().SetRangeUser(0, bin[-1]+np.mean(bin)/2)
    h1.Draw()

    h2 = TH1D("h2", "h2", 10000, 0, 10000)
    file.Get("MyTree").Project("h2", "e_PlasticScintillator", "e_DetProton > 0. && e_PlasticScintillator > 0.")
    h2.SetXTitle("Energy (keV)")
    h2.SetYTitle(f"Count/{int(h2.GetBinWidth(1))}keV")
    h2.SetTitle("Plastic Scintillator")
    h2.SetName("Stats")
    h2.SetLineColor(632)
    h2.Draw("same")

    legend = TLegend(0.8, 0.8, 0.9, 0.9)
    legend.AddEntry(h1, "Single")
    legend.AddEntry(h2, "Coincidence")
    legend.Draw()

    print("------------------------------")
    print("Coincidence : {:.2f}%".format(h2.Integral()*100/h1.Integral()))
    print("------------------------------")
    return (Canvas, h1, h2, legend)

def PlotHistoDeadLayer(**kwargs):
    file = kwargs['file']
    Canvas=[]
    h=[]
    text=[]
    count=0

    for i, dir in enumerate(["Up", "Down"]):


        if kwargs['det_name'] == "all":
            titre = "all"+dir+"_DeadLayer"
        else:
            titre = kwargs['det_name']
        Canvas.append(TCanvas(titre, titre, 800 , 600)) #2200, 250
        Canvas[i].Divide(3,2)

        for strip in range(1,6):
            Canvas[i].cd(strip)
            gStyle.SetOptStat(0);

            #récupération des hists
            h.append(HistoStrip_dir(file = file, direction = dir, Strip_number = strip, coincidence = False, det_name = kwargs["det_name"], option = "dl"))

            h[count].SetXTitle("Energy (keV)")
            h[count].SetYTitle(f"Count/{h[count].GetBinWidth(1)}keV")
            h[count].SetTitle(f"{dir}_Strip_{strip}_DeadLayer")
            max = 10000
            nbin = 100000
            if dir == "Up":
                coef = 5
            else:
                coef = 1

            if strip == 1:
                bin =  [h[count].GetBin(i)*max/nbin for i in range(1, h[count].GetNbinsX()) if h[count].GetBinContent(i) > 1.]
                maxi = 1.2*h[count].GetMaximum()

            h[count].GetYaxis().SetRangeUser(0, maxi)
            h[count].GetXaxis().SetRangeUser(0, bin[-1]-coef*np.std(bin))

            Canvas[i].SetLogy()
            h[count].Draw()


            Canvas[i].cd(6)
            mean = h[count].GetMean()

            E="\overline{E}"
            text.append(TLatex().DrawLatex(0.1, 1-0.15*strip, "Strip \- {} : {} = {:.2f} keV".format(strip, E, mean)))
            count+=1

    # Canvas.append(TCanvas("Incidence Angle"+" DeadLayer", "Incidence Angle"+" DeadLayer", 800 , 600))
    # Canvas[2].Divide(3,2)

    # zl = np.array([0,0,1])
    # yl = np.array([0,1,0])
    # xl = np.array([1,0,0])
    # angle={}

    # for dir, coef in zip(["Up", "Down"], [-1., 1.]):
    #     for det in range(1,5):
    #         if det == 1 or det == 3:
    #             theta = 60*np. pi/180
    #             phi = 90*np. pi/180
    #
    #             xs = zl*np.cos(90+theta) + xl*sin(90+theta)
    #             ys = yl
    #             zs = zl*cos(theta) + xl*sin(theta)
    #
    #             n = coef*xs
    #         else:
    #             theta = 90*np. pi/180
    #             phi = 60*np. pi/180
    #
    #             xs = xl
    #             ys = zl*cos(np.pi/2+phi) + yl*sin(np.pi/2+phi)
    #             zs = zl*cos(phi) + yl*sin(phi)
    #
    #             n = coef*zs
    #
    #         for strip in range(1,6):
    #             angle[f"{det}{dir}_Strip_{strip}"] = []
    #             tree = file.Get("MyTree")
    #             for i in range(tree.GetEntries()):
    #                 tree.GetEntry(i)
    #                 if tree.n_DetProton == "{det}{dir}_Strip_{strip}":
    #                     vp = np.array([tree.px_DeadLayer, tree.py_DeadLayer, tree.pz_DeadLayer])
    #                     norm = np.linalg.norm(vp, axis=0)
    #                     if norm != 0.:
    #                         angle[f"{det}{dir}_Strip_{strip}"].append(np.arccos(np.dot(n, vp)/norm))
    #
    #
    #
    #             print(f"{det}{dir}_Strip_{strip}", 180*np.array(angle[f"{det}{dir}_Strip_{strip}"])/np.pi)
    #             print("\n")

    return Canvas, h

def PlotHistoStrip(**kwargs):
    try:
        nocoinc = kwargs["nocoinc"]
    except KeyError:
        nocoinc = False


    if kwargs['show']:
        file = kwargs['file']
        h_nocoinc = [[], []]
        legend1 = [[], []]
        text=[]
        Canvas=[]
        list_shift=array('d')
        list_sigma_shift=array('d')

        federicaRange = [[(3200, 3300), (3190, 3300), (3180, 3290), (3160, 3270), (3140, 3250)],
                         [(3260, 3430), (3260, 3430), (3260, 3430), (3260, 3430), (3260, 3430)]]

        for i, dir in enumerate(["Up", "Down"]):
            sigma_dir=0
            shift_dir=0

            if kwargs['det_name'] == "all":
                titre = "all"+dir
            else:
                titre = kwargs['det_name']
            Canvas.append(TCanvas(titre, titre, 800 , 600)) #2200, 250
            Canvas[i].Divide(3,2)
            Canvas[i].SetLogy()

            for strip in range(1,6):
                Canvas[i].cd(strip)
                gStyle.SetOptStat(0);

                #récupération des histos
                h = HistoStrip_dir(file = file, direction = dir, Strip_number = strip, coincidence = False, det_name = kwargs["det_name"])
                h_coinc = HistoStrip_dir(file = file, direction = dir, Strip_number = strip, coincidence = 100., det_name = kwargs["det_name"])

                h_nocoinc[i].append(h.Clone("no_coinc"))
                h_nocoinc[i][strip-1].Add(h_coinc.Clone("coinc"), -1.0)

                if h.GetBinWidth(1) == 0.1:
                    h = h.Rebin(10, titre+"_"+str(strip))
                    h_coinc = h_coinc.Rebin(10, titre+"_"+str(strip)+"_coinc")
                    h_nocoinc[i][strip-1] = h_nocoinc[i][strip-1].Rebin(10, titre+"_"+str(strip)+"_nocoinc")

                #affichage
                if strip == 1:
                    maxi = 1.5*h.GetMaximum()

                bin_max = h.GetMaximumBin()
                if dir == 'Up' and 'all' in kwargs['det_name'] or 'Up' in kwargs['det_name']:
                    min_x, max_x, Ndiv = bin_max-100, bin_max+100, 205
                else:
                    min_x, max_x, Ndiv = bin_max-75, bin_max+75, 205



                h.GetYaxis().SetRangeUser(0, maxi)
                h.GetXaxis().SetRangeUser(min_x, max_x)
                h_coinc.GetXaxis().SetRangeUser(min_x, max_x)
                h_nocoinc[i][strip-1].GetXaxis().SetRangeUser(min_x, max_x)

                # h.GetXaxis().SetRangeUser(federicaRange[i][strip-1][0], federicaRange[i][strip-1][1])
                # h_coinc.GetXaxis().SetRangeUser(federicaRange[i][strip-1][0], federicaRange[i][strip-1][1])
                # h_nocoinc[i][strip-1].GetXaxis().SetRangeUser(federicaRange[i][strip-1][0], federicaRange[i][strip-1][1])



                h.GetXaxis().SetNdivisions(Ndiv)
                h.SetXTitle("Energy (keV)")
                h.SetYTitle("Count/keV")
                h.SetTitle(f"{dir}_Strip_{strip}")
                h_coinc.SetLineColor(632)
                h_coinc.GetXaxis().SetRangeUser(min_x, max_x)
                h_nocoinc[i][strip-1].SetLineColor(kBlue)

                h.Draw()
                h_coinc.Draw("same")

                legend1[i].append(TLegend(0.65, 0.7, 0.9, 0.9))
                legend1[i][strip-1].SetTextSize(0.03)
                legend1[i][strip-1].AddEntry(h, "Single")
                legend1[i][strip-1].AddEntry(h_coinc, "Coincidence")

                if nocoinc:
                    h_nocoinc[i][strip-1].Draw("same")
                    legend1[i][strip-1].AddEntry(h_nocoinc[i][strip-1], "No Coincidence")


                legend1[i][strip-1].Draw("same")



                #calculs
                E = "$\overload{E}_{shift}$"

                N_single, E_single = h.Integral(), h.GetMean()
                N_coinc, E_coinc = h_coinc.Integral(), h_coinc.GetMean()
                N_nocoinc, E_nocoinc = N_single-N_coinc, h_nocoinc[i][strip-1].GetMean()


                shift = abs((E_coinc-E_nocoinc)*(1-N_coinc/N_single))
                sigma_shift = np.sqrt(0.5*(h_coinc.GetRMS()/np.sqrt(N_coinc))**2+1/N_single*shift**2)

                list_shift.append(shift)
                list_sigma_shift.append(sigma_shift)

                Canvas[i].cd(6)
                text.append(TLatex().DrawLatex(0.1, 1-0.15*strip, "Strip \- {} : {} = {:.2f} \pm {:.2f} keV".format(strip, E, shift, sigma_shift)))

                print(f"-------------- Strip {strip} ----------------------------------")
                print(" Single : \t\t N = {:.0f}  E = {:.2f} keV \t |".format(N_single, E_single))
                print(" Coincidence : \t\t N = {:.0f}  E = {:.2f} keV \t |".format(N_coinc, E_coinc))
                print(" NoCoincidence : \t N = {:.0f}  E = {:.2f} keV \t |".format(N_nocoinc, E_nocoinc))
                print(" Results : \t\t E = {:.2f} +/- {:.2f} keV \t\t |".format(shift, sigma_shift))
                if (strip == 5): print("---------------------------------------------------------\n")

            if kwargs["det_name"] != "all":
                break

        if kwargs["det_name"] == "all":
            Canvas.append(TCanvas("Dot Plot", "Summary", 800, 600))
            plot_up = TGraphErrors(len(list_shift[:5]), np.linspace(1, len(list_shift[:5]), len(list_shift[:5])),list_shift[:5], np.zeros(len(list_shift[:5])), list_sigma_shift[:5])
            plot_down = TGraphErrors(len(list_shift[5:]), np.linspace(1, len(list_shift[5:]), len(list_shift[5:])),list_shift[5:], np.zeros(len(list_shift[5:])), list_sigma_shift[5:])
            plot_down.GetXaxis().SetRangeUser(0, 10)
            plot_up.GetXaxis().SetRangeUser(0, 10)

            plot_up.GetYaxis().SetRangeUser(2.5, 5)

            plot_down.SetTitle("E_{shift} per strip")
            plot_down.GetYaxis().SetTitle("E_{shift} (keV)")
            plot_down.GetXaxis().SetTitle("Strip")
            plot_down.GetXaxis().SetNdivisions(6)
            plot_up.SetTitle("E_{shift} per strip")
            plot_up.GetYaxis().SetTitle("E_{shift} (keV)")
            plot_up.GetXaxis().SetTitle("Strip")
            plot_up.GetXaxis().SetNdivisions(6)
            plot_up.SetMarkerStyle(22)
            plot_up.SetMarkerColor(kRed)
            plot_up.SetMarkerSize(1.5)
            plot_down.SetMarkerStyle(23)
            plot_down.SetMarkerColor(kBlue)
            plot_down.SetMarkerSize(1.5)

            plot_up.Draw("AP")
            plot_down.Draw("P same")

            legend = TLegend(0.8, 0.8, 0.9, 0.9)
            legend.AddEntry(plot_down, "Down", "p")
            legend.AddEntry(plot_up, "Up", "p")


            ##federicaRange
            plot_up_f=0
            plot_down_f=0
            # plot_up_f = TGraphErrors(len(list_shift[:5]), np.linspace(1, len(list_shift[:5]), len(list_shift[:5])),array('d', [4.072, 3.676, 3.278, 2.958, 2.648]),np.zeros(len(list_shift[:5])),np.zeros(len(list_shift[:5])))
            # plot_down_f = TGraphErrors(len(list_shift[:5]), np.linspace(1, len(list_shift[5:]), len(list_shift[5:])),array('d', [3.96, 3.555, 3.204, 2.892, 2.623]),np.zeros(len(list_shift[:5])),np.zeros(len(list_shift[:5])))
            #
            # plot_up_f.SetMarkerStyle(22)
            # plot_up_f.SetMarkerColor(kOrange)
            # plot_up_f.SetMarkerSize(1.5)
            # plot_down_f.SetMarkerStyle(23)
            # plot_down_f.SetMarkerColor(kCyan)
            # plot_down_f.SetMarkerSize(1.5)
            #
            #
            # legend.AddEntry(plot_down_f, "Federica Down", "p")
            #legend.AddEntry(plot_up_f, "Federica Up", "p")

            #plot_up_f.Draw("P same")
            #plot_down_f.Draw("P same")

            legend.Draw("same")

            return Canvas, (h_nocoinc, plot_up, plot_down, plot_up_f, plot_down_f, legend, legend1)

        else:
            return Canvas, (legend1)




    else:
        file = kwargs['file']
        h_nocoinc = [[], []]
        list_shift=array('d')
        list_sigma_shift=array('d')



        for i, dir in enumerate(["Up", "Down"]):
            sigma_dir=0
            shift_dir=0

            if kwargs['det_name'] == "all":
                titre = "all"+dir
            else:
                titre = kwargs['det_name']



            for strip in range(1,6):
                #récupération des histos
                h = HistoStrip_dir(file = file, direction = dir, Strip_number = strip, coincidence = False, det_name = kwargs["det_name"])
                h_coinc = HistoStrip_dir(file = file, direction = dir, Strip_number = strip, coincidence = 100., det_name = kwargs["det_name"],)
                h_nocoinc[i].append(h.Clone("no_coinc"))
                h_nocoinc[i][strip-1].Add(h_coinc.Clone("coinc"), -1.0)

                if h.GetBinWidth(1) == 0.1:
                    h = h.Rebin(10, titre+"_"+str(strip))
                    h_coinc = h_coinc.Rebin(10, titre+"_"+str(strip)+"_coinc")

                bin_max = h.GetMaximumBin()
                if dir == 'Up' and 'all' in kwargs['det_name'] or 'Up' in kwargs['det_name']:
                    min_x, max_x, Ndiv = bin_max-100, bin_max+100, 205
                else:
                    min_x, max_x, Ndiv = bin_max-75, bin_max+75, 205

                h.GetXaxis().SetRangeUser(min_x, max_x)
                h_coinc.GetXaxis().SetRangeUser(min_x, max_x)
                h_nocoinc[i][strip-1].GetXaxis().SetRangeUser(min_x, max_x)

                N_single, E_single = h.Integral(), h.GetMean()
                N_coinc, E_coinc = h_coinc.Integral(), h_coinc.GetMean()
                N_nocoinc, E_nocoinc = N_single-N_coinc, h_nocoinc[i][strip-1].GetMean()


                shift = abs((h_coinc.GetMean()-h_nocoinc[i][strip-1].GetMean())*(1-N_coinc/N_single))
                sigma_shift = np.sqrt(0.5*(h_coinc.GetStdDev()/np.sqrt(N_coinc))**2+1/N_single*shift**2)

                list_shift.append(shift)
                list_sigma_shift.append(sigma_shift)

                h.Draw()

            if kwargs["det_name"] !="all":
                break

        return (list_shift, list_sigma_shift), (h, h_coinc)

def PlotFit_a(data, **kwargs):
    if kwargs["det_num"]=="all":
        list_dir = ["Up", "Down"]
        number=""
    else:
        list_dir = [kwargs["det_num"][1:]]
        number=kwargs["det_num"][0]

    if kwargs["show"]:
        Canvas1=[]
        graph=[]
        linear_fit=[]
        text=[]
        for i, dir in enumerate(list_dir):
            Canvas1.append(TCanvas(f"{number}{dir}", f"{number}{dir}", 1500 , 600))
            Canvas1[i].Divide(3,2)
            graph.append([])
            linear_fit.append([])
            for strip in range(1,6):
                Canvas1[i].cd(strip)
                graph[i].append(TGraphErrors(len(data[f"{strip}{dir}"][0]), array('d', data[f"{strip}{dir}"][0]), array('d', data[f"{strip}{dir}"][2]), array('d', data[f"{strip}{dir}"][1])))
                linear_fit[i].append(TF1(f"linear_fit{dir}{strip}", "[0] + [1]*x", -2, 2))
                graph[i][strip-1].SetTitle(f"{number}{dir}Strip_{strip}")
                #graph[i][strip-1].SetTitleSize(0.5)
                graph[i][strip-1].GetXaxis().SetTitle("Energy shift (keV)")
                graph[i][strip-1].GetYaxis().SetTitle("a-factor")
                graph[i][strip-1].Fit(linear_fit[i][strip-1], "EX0Q")
                graph[i][strip-1].SetMarkerStyle(20)

                graph[i][strip-1].Draw("APE")
                linear_fit[i][strip-1].Draw("SAME")
                Canvas1[i].cd(6)
                text.append(TLatex().DrawLatex(0.1, 1-0.15*strip, "Strip \- {} : {} = {:.2f} \pm {:.2f}".format(strip, "slope", linear_fit[i][strip-1].GetParameter(1), linear_fit[i][strip-1].GetParError(1))))

        return Canvas1, graph, linear_fit
    else:
        graph=[]
        linear_fit=[]
        text=[]
        coef=[]
        err=[]
        for i, dir in enumerate(["Up", "Down"]):
            graph.append([])
            linear_fit.append([])
            for strip in range(1,6):
                graph[i].append(TGraphErrors(len(data[f"{strip}{dir}"][0]), array('d', data[f"{strip}{dir}"][0]), array('d', data[f"{strip}{dir}"][2]), array('d', data[f"{strip}{dir}"][1])))
                linear_fit[i].append(TF1(f"linear_fit{dir}{strip}", "[0] + [1]*x", -10, 10))
                graph[i][strip-1].SetTitle(f"Strip_{strip}")
                graph[i][strip-1].GetXaxis().SetTitle("Energy shift (keV)")
                graph[i][strip-1].GetYaxis().SetTitle("a-factor")
                graph[i][strip-1].Fit(linear_fit[i][strip-1], "EX0Q")


                coef.append(linear_fit[i][strip-1].GetParameter(1))
                err.append(linear_fit[i][strip-1].GetParError(1))

        return (coef, err), graph, linear_fit

def Plot_Sensi_Up_Down(dic_coeff, **kwargs):
    parameter = kwargs["parameter"]
    list_fit = []
    text=[]
    if parameter == "Mass":
        unit = "(ua)"
        fit = TF1("inverse", "[0]/x", 0, 80)
    elif "Q" in parameter:
        unit = "(keV)"
    else:
        unit = "(ua)"

    if kwargs['show']:
        Canvas1=[]
        graph=[]
        text=[]
        for i, dir in enumerate(["Up", "Down"]):
            if i == 0:
                direction = 0
            else:
                direction = 5
            list_fit.append([])

            if parameter != "all":
                Canvas1.append(TCanvas(f"sensitivity in {parameter} {dir}", f"sensitivity in {parameter} {dir}", 1500 , 600))
                Canvas1[i].Divide(3,2)
                graph.append([])
                for strip in range(1,6):
                    Canvas1[i].cd(strip)
                    graph[i].append(TGraphErrors())
                    for j, variable in enumerate(dic_coeff):
                        graph[i][strip-1].SetPoint(j, [float(s) for s in re.findall(r'-?\d+\.?\d*', list(dic_coeff.keys())[j])][0], dic_coeff[variable][0][strip-1+direction])
                        graph[i][strip-1].SetPointError(j, 0, dic_coeff[variable][1][strip-1+direction])

                    list_fit[i].append(TF1("inverse", "[1]+[0]*x", 0, 80))
                    graph[i][strip-1].SetTitle(f"Strip_{strip}")
                    graph[i][strip-1].GetXaxis().SetTitle(parameter+" "+unit)
                    graph[i][strip-1].GetYaxis().SetTitle("da/dE_{shift}")
                    graph[i][strip-1].SetMarkerStyle(20)
                    graph[i][strip-1].Draw("APE")
                    graph[i][strip-1].Fit(list_fit[i][strip-1], "EX0Q")
                    list_fit[i][strip-1].Draw("SAME")

                    #print(list_fit[i][strip-1].GetParameter(0))
                Canvas1[i].cd(6)
                    #text.append(TLatex().DrawLatex(0.1, 1-0.15*strip, "Strip \- {} : {} = {:.2f} \pm {:.2f}".format(strip, "slope", linear_fit[i][strip-1].GetParameter(1), linear_fit[i][strip-1].GetParError(1))))
                for i, default_param in enumerate(kwargs["default_param"].keys()):
                    if default_param != parameter:
                        text.append(TLatex().DrawLatex(0.1+0.4*(i-1), 0.75, f"{default_param} : {kwargs['default_param'][default_param]}"))

            else:
                Canvas1.append(TCanvas(f"sensitivity {dir}", f"sensitivity in {parameter} {dir}", 1500 , 600))
                Canvas1[i].Divide(3,2)
                graph.append([])
                for strip in range(1,6):
                    Canvas1[i].cd(strip)
                    graph[i].append(TGraphErrors())
                    for j, variable in enumerate(dic_coeff):
                        graph[i][strip-1].SetPoint(j, kwargs["data"][variable]["Mass"], dic_coeff[variable][0][strip-1+direction])
                        graph[i][strip-1].SetPointError(j, 0, dic_coeff[variable][1][strip-1+direction])

                    graph[i][strip-1].SetTitle(f"Strip_{strip}")
                    graph[i][strip-1].GetXaxis().SetTitle(parameter+" "+unit)
                    graph[i][strip-1].GetYaxis().SetTitle("da/dE_{shift}")
                    graph[i][strip-1].SetMarkerStyle(20)
                    graph[i][strip-1].GetYaxis().SetRangeUser(0.7, 1.7)
                    graph[i][strip-1].Draw("APE")

                    for j, variable in enumerate(dic_coeff):
                        text.append(TLatex().DrawLatex(kwargs["data"][variable]["Mass"]+0.5, dic_coeff[variable][0][strip-1+direction]+0.03, variable))

                    #text.append(TLatex().DrawLatex(0.1, 1-0.15*strip, "Strip \- {} : {} = {:.2f} \pm {:.2f}".format(strip, "slope", linear_fit[i][strip-1].GetParameter(1), linear_fit[i][strip-1].GetParError(1))))
                # for i, default_param in enumerate(kwargs["default_param"].keys()):
                #     if default_param != parameter:
                #         text.append(TLatex().DrawLatex(0.1+0.4*(i-1), 0.75, f"{default_param} : {kwargs['default_param'][default_param]}"))



        return Canvas1, (graph, text)
