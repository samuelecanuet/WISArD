#include <iostream>
#include <TFile.h>
#include <TTree.h>
#include <TH2D.h>
#include <TH1D.h>
#include <TCanvas.h>
#include <TApplication.h>
#include <TROOT.h>
#include "TTreeReader.h"
#include "TTreeReaderValue.h"

using namespace std;

int main(int argc, char **argv) {

    cout<<argv[1]<<endl;

    // Ouvrir le fichier ROOT
    TFile *file = new TFile(argv[1], "READ");
    if (!file->IsOpen()) {
        std::cerr << "Erreur : Impossible d'ouvrir le fichier ROOT." << std::endl;
        return 1;
    }

    TFile *outfile = new TFile("test.root", "RECREATE");

    TH2D hit_xy = TH2D("h2d_hit_xy", "Hit Position; x (mm); y (mm)", 300, -15, 15, 300, -15, 15);
    TH2D e_ini_dep = TH2D("h2d_ini_edep", "Initial Energy and Energy Deposited; Initial Energy (keV); Energy Deposited (keV)", 10000, 0, 10000, 10000, 0, 10000);
    TH1D h1d_angle = TH1D("h1d_angle", "Hit Angle; Angle (deg); Counts/deg", 180, 0, 180);
    TH2D h2d_r_angle = TH2D("h2d_r_angle", "Hit Angle and Radius Position; Radius (mm); Angle (deg)", 150, 0, 15, 180, 0, 180);
    TH1D h1d_edep = TH1D("h1d_edep", " ; Energy Deposit (keV); Counts/keV", 10000, 0, 10000);

    TTreeReader myReader("Tree_Positron", file);
    TTreeReaderValue<double> initial_ekin(myReader, "Initial_Positron_Energy");
    TTreeReaderValue<double> catcher_energydeposit(myReader, "Catcher_Positron_EnergyDeposit");
    TTreeReaderValue<double> pl_energydeposit(myReader, "PlasticScintillator_Positron_EnergyDeposit");
    TTreeReaderValue<double> angle(myReader, "PlasticScintillator_Positron_HitAngle");
    TTreeReaderValue<double> x(myReader, "PlasticScintillator_Positron_HitPosition_x");
    TTreeReaderValue<double> y(myReader, "PlasticScintillator_Positron_HitPosition_y");
    TTreeReaderValue<double> z(myReader, "PlasticScintillator_Positron_HitPosition_z");

    while (myReader.Next())
    {
        if (*z != 0)
        {
            hit_xy.Fill(*x,*y);
            h1d_angle.Fill(*angle);
            h2d_r_angle.Fill(sqrt(*x * *y), *angle);
            h1d_edep.Fill(*pl_energydeposit);
        }
        
        e_ini_dep.Fill(*initial_ekin, *pl_energydeposit);
        

    }



    hit_xy.Write();
    e_ini_dep.Write();
    h1d_angle.Write();
    h2d_r_angle.Write();
    h1d_edep.Write();










    file->Close();
    outfile->Close();

    return 0;
}
