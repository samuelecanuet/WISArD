from ROOT import *
import numpy as np
from array import array
import sys
from function_root import *
import os

#setenv PYTHONPATH /usr/local/root/6.24.06/lib:
#setenv LD_LIBRARY_PATH /usr/local/root/6.24.06/lib:/lib:

dic_data_element={}
dic_data_element["Ar"] = {"Mass" : 32 , "Qbeta" : 6087.9,"Ep" : 3352.7}
dic_data_element["Mg"] = {"Mass" : 20. , "Qbeta" : 4131.0,"Ep" : 4306.0}
dic_data_element["Si"] = {"Mass" : 24. , "Qbeta" : 4859.0,"Ep" : 3910}
dic_data_element["S"] =  {"Mass" : 28. , "Qbeta" : 5320.0,"Ep" : 2984}


dic_default_param = {}
dic_default_param["Mass"] = "32 ua"
dic_default_param["Ep"] = "3352.7 keV"
dic_default_param["Qbeta"] = "6087.9 keV"


list_el=[]
list_root=[]
list_a = ["a1", "a0", "a-1", "all"]

if "all" in sys.argv and len(sys.argv) == 2:
    condition_all = True
    current_el = "all"
else:
    condition_all = False

if "sensitivity" in sys.argv and len(sys.argv) == 2:
    condition_sensitivity = True
    current_el = "sensitivity"
else:
    condition_sensitivity = False

for type in ["Nuclei", "Sensitivity"]:
    for root, subdirs, files in os.walk(type+"/"):
        for i in subdirs:
            if not i in dic_default_param.keys():

                if condition_all == True or condition_sensitivity:
                    list_el.append(i)
                    list_root.append(root)

                else:
                    try:
                        if sys.argv.index(i) != -1:
                            list_el.append(i)
                            list_root.append(root)
                            current_el = i
                    except:
                            None
    list_el.append("\n")

for a in list_a:
    try:
        if sys.argv.index(a) != -1:
            current_a = a
            break
    except:
        current_a = False

#good argument(s) ?
if not current_el:
    print("Select a nuclei in this list please.")
    print(" "+" ".join(list_el))
    sys.exit("Retry with good nuclei")

number_nuclei = len(list_el[:list_el.index("\n")])
list_el=[i for i in list_el if not "\n" in i ]

if not current_a and "sensitivity" not in sys.argv:
    print("Select a a-factor in this list please.")
    print(" ".join(list_a))
    sys.exit("Retry with good a-factor")


# select file(s)
dic_data={}
if current_el == "sensitivity":
    for param in dic_default_param.keys():
        dic_data[param] = {}
        for variable, root in zip(list_el[number_nuclei:], list_root[number_nuclei:]):
            if param in root:
                dic_data[param][variable] = []
                for filename in os.listdir(root+"/"+variable):
                    dic_data[param][variable].append(root+"/"+variable+"/"+filename)

elif current_el == "all":
    for el in list_el[:number_nuclei]:
        dic_data[el] = []
        for filename in os.listdir(list_root[list_el.index(el)]+"/"+el):
            if not "3356" in filename and not "federica" in filename and not "catcher" in filename and not "pir" in filename:
                dic_data[el].append(list_root[list_el.index(el)]+el+"/"+filename)

else:
    dic_data[current_el] = []
    if current_a == "all":
        for filename in os.listdir(list_root[list_el.index(current_el)]+"/"+current_el):
            if "Ar32_e6_a-1_tree" in filename or "Ar32_e6_a0_tree" in filename or "Ar32_e6_a1_federica_tree.root" in filename:
                dic_data[current_el].append(list_root[list_el.index(current_el)]+"/"+current_el+"/"+filename)


    else:
        for filename in os.listdir(list_root[list_el.index(current_el)]+"/"+current_el):
            if "Ar_b0_a1.root" in filename:
                if filename.find(current_a) != -1:
                    dic_data[current_el].append(list_root[list_el.index(current_el)]+"/"+current_el+"/"+filename)
                    break

#good number of file(s) ?
for key, value in dic_data.items():
    if len(value) > 3 and current_el != "sensitivity":
        print(f"Too much file in the folder {key} ( {len(value)} instead of 3)")
        sys.exit("Too much file")
    elif len(value) < 1 and current_el != "sensitivity":
        print(f"No file in the folder {key} with this a_factor --> {sys.argv[2]} ")
        sys.exit("No file")

#display of files selected
print("---------Selected File(s)----------")
for key, value in dic_data.items():
    print(f"### {key} ###")
    for file in value:
        print(f"| {file}")
print("-----------------------------------")




if len(dic_data) > 1 and current_el == "sensitivity": #for multiple nuclei selection
    param_selection = input("What kind of sensitivity do you want ?\n\n - Mass \n - Qbeta \n - Qp \n - all\n\n Selection : ")

    #plot sensitivity on param
    if "all" == param_selection:
        list_param = dic_data_element["Ar"].keys()
        show_condition = False
    elif "all" in param_selection:
        list_param = dic_data_element["Ar"].keys()
        show_condition = True
    else:
        list_param = [i for i in list(param_selection.split(" ")) if i != " "]
        show_condition = True

    canvas=[]

    dic_slope = {}
    for param in list_param:
        dic_slope[param] = {}
        for variable, list_file in dic_data[param].items():
            list_strip = {} #will give a list for each a and each strip for Eshift value

            for i in range(1,6):
                list_strip[f'{i}Up'] = [[], [], []]
                list_strip[f'{i}Down'] = [[], [], []]

            for file in list_file:
                #if "M80" in file:#and "a-1" in file:
                    f = TFile(file)
                    out, y = PlotHistoStrip(file = f, show = False, el = variable, det_name = "all")
                    for a in list_a:
                        if file.find(a) != -1:
                            if a == "a-1":
                                a = -1
                            if a == "a0":
                                a = 0
                            if a == "a1":
                                a = 1
                            for i in range(1,6):
                                list_strip[f'{i}Up'][2].append(a)
                                list_strip[f'{i}Up'][0].append(out[0][i-1])
                                list_strip[f'{i}Up'][1].append(out[1][i-1])
                                list_strip[f'{i}Down'][2].append(a)
                                list_strip[f'{i}Down'][0].append(out[0][i-1+5])
                                list_strip[f'{i}Down'][1].append(out[1][i-1+5])

                    coeff, graph, fits = PlotFit_a(list_strip, show = False, el = variable, det_num="all")
                    dic_slope[param][variable] = coeff


        canvas.append(Plot_Sensi_Up_Down(dic_slope[param], parameter = param, show = show_condition, data=dic_data_element, default_param = dic_default_param))




elif len(dic_data) > 1 and current_el == "all":
    canvas=[]
    dic_slope = {}
    for nuclei, list_file in dic_data.items():
        list_strip = {} #will give a list for each a and each strip for Eshift value

        for i in range(1,6):
            list_strip[f'{i}Up'] = [[], [], []]
            list_strip[f'{i}Down'] = [[], [], []]

        for file in list_file:
            #if "M80" in file:#and "a-1" in file:
                f = TFile(file)
                out, y = PlotHistoStrip(file = f, show = False, el = nuclei, det_name = "all")
                for a in list_a:
                    if file.find(a) != -1:
                        if a == "a-1":
                            a = -1
                        if a == "a0":
                            a = 0
                        if a == "a1":
                            a = 1
                        for i in range(1,6):
                            list_strip[f'{i}Up'][2].append(a)
                            list_strip[f'{i}Up'][0].append(out[0][i-1])
                            list_strip[f'{i}Up'][1].append(out[1][i-1])
                            list_strip[f'{i}Down'][2].append(a)
                            list_strip[f'{i}Down'][0].append(out[0][i-1+5])
                            list_strip[f'{i}Down'][1].append(out[1][i-1+5])

                coeff, graph, fits = PlotFit_a(list_strip, show = False, el = nuclei, det_num="all")
                dic_slope[nuclei] = coeff


    canvas.append(Plot_Sensi_Up_Down(dic_slope, parameter = "all", show = True, data=dic_data_element, default_param = dic_default_param))

#for simple nuclei selection but multiple a_factor selection
elif len(dic_data) == 1 and len(dic_data[current_el]) > 1:
    choice = float(input("\nWhich type of calculation ? \n 1 - For each Silicon Detector \n 2 - All same Strip and Direction summed\n\n Selection : "))

    if choice == 1:
        choicef = list(input("\nWhich type of calculation ? \n - All \n - 1Up \t - 1Down \n - 2Up \t - 2Down \n - 3Up \t - 3Down \n - 4Up \t - 4Down \n\n Selection : ").split(" "))
        if "All" in choicef:
            choicef =[]
            for det in range(1,5):
                for dir in ["Up", "Down"]:
                    choicef.append(str(det)+dir)
    elif choice == 2:
        choicef = ["all"]

    else:
        print("Wrong choice :(")

    canvas_a =[]
    graph_=[]
    for det in choicef:
        if det != "all":
            number = det[0]
            direction = det[1:]
            list_dir = [direction]
        else:
            number="all"
            direction=""
            list_dir = ["Up", "Down"]


        show_condition = True
        list_strip = {}

        for i in range(1,6):
            for dir in list_dir:
                list_strip[f'{i}{dir}'] = [[], [], []]

        for file in dic_data[current_el]:
            f = TFile(file)
            update_f = TFile(file, "update")
            if number == "all":
                if not update_f.Get(f"{number}Up") and not update_f.Get(f"{number}Down"):
                    out, y = PlotHistoStrip(file = f, show = True, el = current_el, det_name=f"{number}{direction}", nocoinc = True)
                    for j in range(len(list_dir)):
                        out[j].Write("", TFile.kOverwrite)
            else:
                if not update_f.Get(f"{det}"):
                    out, y = PlotHistoStrip(file = f, show = True, el = current_el, det_name=f"{number}{direction}", nocoinc = True)
                    for j in range(len(list_dir)):
                        out[j].Write("", TFile.kOverwrite)
                    print("e")


            out=[[], []]
            for dir in list_dir:
                c = update_f.Get(f"{number}{dir}")

                for strip in range(1,6):
                    N_single = c.FindObject(f"{number}{dir}_{strip}").FindObject(f"{number}{dir}_{strip}").Integral()
                    shift = abs((c.FindObject(f"{number}{dir}_{strip}").FindObject(f"{number}{dir}_{strip}_coinc").GetMean()-c.FindObject(f"{number}{dir}_{strip}").FindObject(f"{number}{dir}_{strip}_nocoinc").GetMean())*(1-c.FindObject(f"{number}{dir}_{strip}").FindObject(f"{number}{dir}_{strip}_coinc").Integral()/N_single))
                    sigma_shift = np.sqrt(0.5*(c.FindObject(f"{number}{dir}_{strip}").FindObject(f"{number}{dir}_{strip}_coinc").GetRMS()/np.sqrt(c.FindObject(f"{number}{dir}_{strip}").FindObject(f"{number}{dir}_{strip}_coinc").Integral()))**2+1/N_single*shift**2)
                    out[0].append(shift)
                    out[1].append(sigma_shift)

            for a in list_a:
                if file.find(a) != -1:
                    if a == "a-1":
                        a = -1
                    if a == "a0":
                        a = 0
                    if a == "a1":
                        a = 1
                    for i in range(1,6):
                        for dir in list_dir:
                            if len(list_dir) == 2 and dir == "Down":
                                offset = 5
                            else:
                                offset = 0

                            list_strip[f'{i}{dir}'][2].append(a)
                            list_strip[f'{i}{dir}'][0].append(out[0][i-1+offset])
                            list_strip[f'{i}{dir}'][1].append(out[1][i-1+offset])

        update_f.Close()
        canvas_, graph, list_coeff = PlotFit_a(list_strip, show = True, el = current_el, det_num=f"{number}{direction}")
        graph_.append(graph)
        canvas_a.append(canvas_)

    new_file = TFile(list_root[list_el.index(current_el)]+"/"+current_el+f"/{current_el}_plot_a.root", "update")
    for j in range(len(canvas_a)):
        for i in range(len(list_dir)):
            canvas_a[j][i].Write("", TFile.kOverwrite)
    new_file.Close()


else: #for simple nuclei and simple a_factor selection
    f = TFile(dic_data[current_el][0])
    update_f = TFile(dic_data[current_el][0], "update")
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
            list_det = list(input("\nWhat detector you want ? (you can make multiple selection with space between each of them) \n\n - 1Up \t - 1Down\n - 2Up \t - 2Down\n - 3Up \t - 3Down\n - 4Up \t - 4Down\n\n Selection : ").split(" "))
            for j, det in enumerate(list_det):
                if not update_f.Get(det):
                    c.append(PlotHistoStrip(file = f, show = show_condition, el = current_el, det_name = det, nocoinc = True))
                    c[count][0][0].Write()
                    count+=1
                update_f.Get(det).Draw()

        elif int(i) == 5:
            if not update_f.Get("allUp") and not update_f.Get("allDown"):
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
