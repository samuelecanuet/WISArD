from ROOT import *
import numpy as np
from array import array
import sys
from function_root import *
import os

filename = sys.argv[1]

#verifier si le fichier existe
# for root, folder, i in os.walk("./"):
#     enter_nuc = filename[:filename.index('_')]
#     print(enter_nuc)
#     if enter_nuc in folder and filename in os.listdir(enter_nuc+"/"):
#         nuclei = enter_nuc
#         param  = filename[filename.index('_')+1:filename.index('.'):]
#         break
# else:
#     print("Wrong file or no folder")

f = TFile(f"{filename}")
update_f = TFile(f"{filename}", "update")
show_condition = True

wrong_choice = True
choice = list(input("\nWhat do you want ? (you can make multiple selection) \n\n 1 - Source Position\n 2 - Source Energy\n 3 - Catcher\n 4 - One Silicon Detector \n 5 - All Silicon Detector \n 6 - Dead Layer Silicon Detector \n 7 - Plastic Scintillator \n\n Selection : "))
count = 0

for i in choice:
    if int(i) == 1:
        if update_f.Get("Position"):
            update_f.Get("Position").Draw()
        else:
            c1 = PlotPosition(file = f)
            c1[0].Write("", TFile.kOverwrite)
        update_f.Get("Position").Draw()

    elif int(i) == 2:
        if update_f.Get("Energy_proton") or update_f.Get("Energy_positron"):
            update_f.Get("Energy_proton").Draw()
            update_f.Get("Energy_positron").Draw()
        else:
            c_part = PlotEnergy(file = f)
            c_part[0][0].Write("", TFile.kOverwrite)
            c_part[0][1].Write("", TFile.kOverwrite)

    elif int(i) == 3:
        canvas_catcher = []
        count=0
        catcher_choice = input("Which proton energy deposit contribution do you want ? \n\n - all\n - Up_Strip_1 \t - Down_Strip_1 \n - Up_Strip_2 \t - Down_Strip_2 \n - Up_Strip_3 \t - Down_Strip_3 \n - Up_Strip_4 \t - Down_Strip_4 \n - Up_Strip_5 \t - Down_Strip_5 \n\n Selection : ")
        if "all" not in catcher_choice:
            for j, contrib in enumerate([i for i in catcher_choice.split(" ") if i != " "]):
                if not update_f.Get("Catcher "+contrib):
                    canvas_catcher.append(plot_catcher_strip(file = f, strip = contrib))
                    canvas_catcher[count][0].Write("", TFile.kOverwrite)
                    count+=1
                update_f.Get("Catcher "+contrib).Draw()

        else:
            if not update_f.Get("Catcher"):
                canvas_catcher.append(plotCatcher(file = f))
                canvas_catcher[0][0].Write("", TFile.kOverwrite)
            update_f.Get("Catcher").Draw()

    elif int(i) == 4:
        c=[]
        count=0
        list_det = list(input("\nWhich detector you want ? (you can make multiple selection with space between each of them) \n\n - 1Up \t - 1Down\n - 2Up \t - 2Down\n - 3Up \t - 3Down\n - 4Up \t - 4Down\n\n Selection : ").split(" "))
        for j, det in enumerate(list_det):
            if not update_f.Get(det):
                c.append(PlotHistoStrip(file = f, show = show_condition, el = nuclei, det_name = det, nocoinc = True))
                c[count][0][0].Write()
                count+=1
            update_f.Get(det).Draw()

    elif int(i) == 5:
        #if not update_f.Get("allUp") and not update_f.Get("allDown"):
            out, y = PlotHistoStrip(file = f, show = show_condition, det_name = "all", nocoinc = True)
            out[0].Write("", TFile.kOverwrite)
            out[1].Write("", TFile.kOverwrite)
            update_f.Get("allUp").Draw()
            update_f.Get("allDown").Draw()

    elif int(i) == 6:
        out, y = PlotHistoDeadLayer(file = f, det_name = "all")
        out[0].Write("", TFile.kOverwrite)
        out[1].Write("", TFile.kOverwrite)
        if not update_f.Get("allUp_DeadLayer") and not update_f.Get("allDown_DeadLayer"): #and not update_f.Get("Incidence Angle DeadLayer"):
            out, y = PlotHistoDeadLayer(file = f, det_name = "all")
            out[0].Write("", TFile.kOverwrite)
            out[1].Write("", TFile.kOverwrite)
            #out[3].Write("", TFile.kOverwrite)
        update_f.Get("allUp_DeadLayer").Draw()
        update_f.Get("allDown_DeadLayer").Draw()
            #update_f.Get("Incidence Angle DeadLayer").Draw()

    elif int(i) == 7:
        if not update_f.Get("PlasticScintillator"):
            c5 = PlotPlasticScintillator(file = f)
            c5[0].Write("", TFile.kOverwrite)
        update_f.Get("PlasticScintillator").Draw()

update_f.Close()
