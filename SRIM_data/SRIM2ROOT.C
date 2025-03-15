#include "TH3D.h"
#include "TH1D.h"
#include <iostream>
#include <fstream>
#include <string>
#include "TFile.h"
#include "TObjString.h"


int SRIM2ROOT(string filename = "")
{

    if (filename == "")
    {
        std::cerr << "Usage: SRIM2ROOT <SRIM_input_file>" << std::endl;
        return 1;
    }

    std::ifstream SRIMTXT;
    SRIMTXT.open(filename);

    if (!SRIMTXT.is_open())
    {
        std::cerr << "Error: unable to open SRIM input file" << std::endl;
        return 1;
    }

    // get txt filename remove .txt
    std::string txt_filename = filename;
    txt_filename = txt_filename.substr(0, txt_filename.find_last_of('.'));
    TFile *output = new TFile((txt_filename + ".root").c_str(), "RECREATE");

    
    // FIRST READ TO GET BOUNDARIES
    std::cout << "Reading SRIM file to get boundaries of TH3D" << std::endl;
    std::string line;
    std::string text;
    
    int x_max = 0, y_max = 0, z_max = 0;
    while (std::getline(SRIMTXT, line)) {
        
        std::istringstream iss(line);
        std::string x_str, y_str, z_str;
        int num;
        std::string text_in_line;
        
        if (iss >> num >> z_str >> y_str >> x_str) 
        {
            
           double x = std::stod(x_str.replace(x_str.find(','), 1, "."))/10; // A to nm
           double y = std::stod(y_str.replace(y_str.find(','), 1, "."))/10; // A to nm
           double z = std::stod(z_str.replace(z_str.find(','), 1, "."))/10; // A to nm

            if (abs(x) > x_max) x_max = abs(x);
            if (abs(y) > y_max) y_max = abs(y);
            if (abs(z) > z_max) z_max = abs(z);
        }
        else
        {
            text += line + "\n";
        }
    }

    cout << "x_max: " << x_max << endl;
    cout << "y_max: " << y_max << endl;
    cout << "z_max: " << z_max << endl;

    

    SRIMTXT.clear();
    SRIMTXT.seekg(0, std::ios::beg);

    int x_bins = 2*x_max;
    int y_bins = 2*y_max;
    int z_bins = z_max;

    while (x_bins * y_bins * z_bins > 1e8)
    {
        x_bins /= 2;
        y_bins /= 2;
    }

    cout << "x_bins: " << x_bins << endl;
    cout << "y_bins: " << y_bins << endl;
    cout << "z_bins: " << z_bins << endl;

    TH3D *FINAL = new TH3D("SRIM", "SRIM", x_bins, -x_max, x_max, y_bins, -y_max, y_max, z_bins, 0, z_max);
    TH1D *x_hist = new TH1D("x", "x", x_bins, -x_max, x_max);
    TH1D *y_hist = new TH1D("y", "y", 2*y_max, -y_max, y_max);
    TH1D *z_hist = new TH1D("z", "z", z_bins, 0, z_max);

    // FILLING HISTOGRAMS
    std::cout << "Filling TH3D" << std::endl;
    while (std::getline(SRIMTXT, line)) {
        
        std::istringstream iss(line);
        std::string x_str, y_str, z_str;
        int num;
        
        if (iss >> num >> z_str >> y_str >> x_str) 
        {
            
           double x = std::stod(x_str.replace(x_str.find(','), 1, "."))/10; // A to nm
           double y = std::stod(y_str.replace(y_str.find(','), 1, "."))/10; // A to nm
           double z = std::stod(z_str.replace(z_str.find(','), 1, "."))/10; // A to nm

            FINAL->Fill(x, y, z);
            x_hist->Fill(x);
            y_hist->Fill(y);
            z_hist->Fill(z);
        }
    }

    output->cd();
    TObjString textROOT(text.c_str());
    textROOT.Write("Info");
    FINAL->Write();
    x_hist->Write();
    y_hist->Write();
    z_hist->Write();
    output->Close();

    std::cout << "ROOT file created: " << txt_filename << ".root" << std::endl;
    return 0;
}