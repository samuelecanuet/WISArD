#ifndef WISARD_GENERATOR_HH
#define WISARD_GENERATOR_HH

#include "Wisard_Global.hh"
#include "G4VUserPrimaryGeneratorAction.hh"

#include "G4ParticleGun.hh"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

#include <unordered_map>
#include <tuple>
#include <functional>

#include "Randomize.hh"
#include "G4ThreeVector.hh"

#include "G4GenericMessenger.hh"
#include "G4IonTable.hh"
#include "G4ParticleTable.hh"
#include "G4Element.hh"
#include "G4Event.hh"

#include "TFile.h"
#include "TTreeReader.h"
#include "TTreeReaderValue.h"
#include "TTreeReaderArray.h"
#include "TF2.h"
#include "TH2D.h"
#include "TH3D.h"

#include "G4AutoLock.hh"
#include <mutex>
#include <condition_variable>
#include <thread>

class Wisard_Generator : public G4VUserPrimaryGeneratorAction
{
    //------------------------------------------------------------
    // internal variables definition
protected:
    int ievent, iev_len = 0, isubevent, isubev_len = 0;

    G4ParticleTable *particle_table = G4ParticleTable::GetParticleTable();
    G4ParticleDefinition *part_electron;
    G4ParticleDefinition *part_positron;
    G4ParticleDefinition *part_gamma;
    G4ParticleDefinition *part_proton;
    G4ParticleDefinition *part_enubar;
    G4ParticleDefinition *part_alpha;
    G4ParticleDefinition *part_geantino;
    G4ParticleDefinition *Gun_Particle;

    G4GenericMessenger *BeamMessenger;
    G4GenericMessenger *InputMessenger;
    G4double X, Y, Sigma_X, Sigma_Y, Q, Position_catcher_z, Radius;
    G4int Z, A;
    G4double energy = 0;
    G4String particle_string;
    G4String nucleus_string;
    G4String dir_string = "0 0 1";
    G4String pos_string = "0 0 0";
    G4String InputFileName, SRIMFileName;
    ifstream InputTXT, SRIMTXT;
    TFile *InputROOT;

    G4ThreeVector position_array[10001];
    G4ThreeVector direction_array[10001];
    G4double energy_array[10001];
    G4int event_counter = 0;

    G4double x_SRIM, y_SRIM, z_SRIM;

    G4ParticleDefinition *particle;
    G4ThreeVector dir;
    G4ThreeVector pos;
    G4ParticleGun gun;

    TH3D *SRIM_HISTOGRAM;

    Long64_t current_entry = 0;

    //------------------------------------------------------------
    // class functions definition
private:
    bool INIT = false;

    unique_ptr<TTreeReader> Reader;

    unique_ptr<TTreeReaderArray<Int_t>> code;
    unique_ptr<TTreeReaderArray<double>> ekin_;
    unique_ptr<TTreeReaderArray<double>> px;
    unique_ptr<TTreeReaderArray<double>> py;
    unique_ptr<TTreeReaderArray<double>> pz;
    unique_ptr<TTreeReaderArray<double>> time_;

    TH1D *Energy_Hist;

    TF2 *Gauss2D;
    TH2D *HGauss2D;

public:
    Wisard_Generator();
    ~Wisard_Generator();

    void GeneratePrimaries(G4Event *event);
    void TXT_GENERATOR(G4Event *event);
    void ROOT_GENERATOR(G4Event *event);
    void ROOT_DISTRIBUTION_GENERATOR(G4Event *event);
    void ION_GENERATOR(G4Event *event);
    std::function<void(G4Event *)> GENERATOR;

    TH3D *GetSRIM_hist();

    void SetCatcherPosition_z(G4double catcher_z);
    void ChooseGENERATOR();
    void InitBeam();
    G4ThreeVector ConvertStringToG4ThreeVector(G4String);
    G4ThreeVector GetDirection(G4ThreeVector);
    G4ThreeVector Beam();
    G4ThreeVector Catcher_Implementation();
};

inline TH3D *Wisard_Generator::GetSRIM_hist()
{

    TFile *output = new TFile((SRIMFileName).c_str(), "READ");

    if (!output || output->IsZombie())
    {
        G4Exception("Wisard_Generator::GetSRIM_hist", "Unable to open SRIM file", JustWarning, "");
        return nullptr;
    }

    TH3D *histogram = (TH3D *)output->Get("SRIM");

    if (!histogram)
    {
        G4Exception("Wisard_Generator::GetSRIM_hist", "Impossible to open SRIM Histogram", JustWarning, "");
        return nullptr;
    }

    return histogram;
}

inline void Wisard_Generator::SetCatcherPosition_z(G4double catcher_z)
{
    Position_catcher_z = catcher_z;
}

inline void Wisard_Generator::ChooseGENERATOR()
{
    if (nucleus_string != "" && InputFileName != "" && particle_string != "")
    {
        // G4cout << "Both Nucleus and InputFile set in the macro" << G4endl;
        G4Exception("Wisard_Generator::ChooseGENERATOR", "Both Nucleus and InputFile set in the macro", JustWarning, "");
    }
    else if (nucleus_string != "")
    {
        // G4cout << "Nucleus GENERATOR" << G4endl;
        A = stoi(nucleus_string.substr(0, nucleus_string.find_first_not_of("0123456789")));
        G4String symbol = nucleus_string.substr(nucleus_string.find_first_not_of("0123456789"));
        G4IonTable *ionTable = G4IonTable::GetIonTable();
        Z = -1;
        for (int i = 1; i < A; i++)
        {
            G4ParticleDefinition *ion = ionTable->GetIon(i, A, 0);
            if (ion->GetParticleName().substr(0, ion->GetParticleName().find_first_of("0123456789")) == symbol)
            {
                Z = ion->GetAtomicNumber();
                Gun_Particle = ion;
                break;
            }
        }
        if (Z == -1)
        {
            G4Exception("Wisard_Generator::ChooseGENERATOR", "Nucleus not found", JustWarning, "");
        }

        dir = ConvertStringToG4ThreeVector(dir_string);
        pos = ConvertStringToG4ThreeVector(pos_string);
        GENERATOR = std::bind(&Wisard_Generator::ION_GENERATOR, this, std::placeholders::_1);
    }
    else if (particle_string != "")
    {
        // G4cout << "Particle GENERATOR" << G4endl;
        G4ParticleDefinition *particlee = G4ParticleTable::GetParticleTable()->FindParticle(particle_string);
        if (!particlee)
        {
            G4Exception("Wisard_Generator::ChooseGENERATOR", "Particle not found", JustWarning, "");
        }
        else
        {
            Gun_Particle = particlee;
            dir = ConvertStringToG4ThreeVector(dir_string);
            pos = ConvertStringToG4ThreeVector(pos_string);
            GENERATOR = std::bind(&Wisard_Generator::ION_GENERATOR, this, std::placeholders::_1);
        }
    }
    else if (InputFileName.find(".txt") != std::string::npos)
    {
        // G4cout << "TXT GENERATOR" << G4endl;
        InputTXT.open(InputFileName.c_str());
        if (InputTXT.fail())
        {
            G4Exception("Wisard_Generator::ChooseGENERATOR", "Impossible to open input TXT file", JustWarning, "");
        }
        GENERATOR = std::bind(&Wisard_Generator::TXT_GENERATOR, this, std::placeholders::_1);
    }
    else if (InputFileName.find(".root") != std::string::npos)
    {
        // G4cout << "ROOT GENERATOR" << G4endl;
        InputROOT = new TFile(InputFileName.c_str(), "READ");
        if (InputROOT->IsZombie())
        {
            G4Exception("Wisard_Generator::ChooseGENERATOR", "Impossible to open input ROOT file", JustWarning, "");
        }

        // prijnt all the object of the tfile 
        for (auto key : *InputROOT->GetListOfKeys())
        {
            cout << key->GetName() << endl;
        }


        Energy_Hist = (TH1D *)InputROOT->Get("histogram");
        if (Energy_Hist != NULL)
        {
            if (InputFileName.find("32Ar") == std::string::npos && InputFileName.find("33Ar") == std::string::npos)
                particle = part_alpha;
            else
                particle = part_proton;
            
            GENERATOR = std::bind(&Wisard_Generator::ROOT_DISTRIBUTION_GENERATOR, this, std::placeholders::_1);
        }
        else
        {
            GENERATOR = std::bind(&Wisard_Generator::ROOT_GENERATOR, this, std::placeholders::_1);
            Reader = std::make_unique<TTreeReader>("ParticleTree", InputROOT);
            code = std::make_unique<TTreeReaderArray<int>>(*Reader, "code");
            ekin_ = std::make_unique<TTreeReaderArray<double>>(*Reader, "energy");
            px = std::make_unique<TTreeReaderArray<double>>(*Reader, "px");
            py = std::make_unique<TTreeReaderArray<double>>(*Reader, "py");
            pz = std::make_unique<TTreeReaderArray<double>>(*Reader, "pz");
            time_ = std::make_unique<TTreeReaderArray<double>>(*Reader, "time");
        }
    }
    else
    {
        G4Exception("Wisard_Generator::ChooseGENERATOR", "No InputFile or Nucleus set in the macro", JustWarning, "");
    }
}

inline void Wisard_Generator::InitBeam()
{
    Gauss2D = new TF2("Gauss2D", "exp(-0.5*((x-[0])/(sqrt(2)*[1]))**2)*exp(-0.5*((y-[2])/(sqrt(2)*[3]))**2)", -100, 100, -100, 100);
    Gauss2D->SetParameters(0, Sigma_X, 0, Sigma_Y);
    Gauss2D->SetNpx(10000);
    Gauss2D->SetNpy(10000);
    HGauss2D = (TH2D *)Gauss2D->GetHistogram();
}

inline G4ThreeVector Wisard_Generator::Beam()
{
    G4double x, y;
    x = 0;
    y = 0;

    // Shoot in Beam profile
    HGauss2D->GetRandom2(x, y);
    while (sqrt(x * x + y * y) > Radius)
    {
        HGauss2D->GetRandom2(x, y);
    }

    return G4ThreeVector(x, y, 0);
}

inline G4ThreeVector Wisard_Generator::Catcher_Implementation()
{
    // Adding SRIM
    double x, y, z;
    SRIM_HISTOGRAM->GetRandom3(x, y, z);

    return G4ThreeVector(x * nm, y * nm, z * nm);
}

inline G4ThreeVector Wisard_Generator::ConvertStringToG4ThreeVector(G4String str)
{
    if (str != "iso")
    {
        std::istringstream iss(str);
        G4double x, y, z;
        if (iss >> x >> y >> z)
        {
            return G4ThreeVector(x, y, z);
        }
        else
        {
            G4Exception("Wisard_Generator::ConvertStringToG4ThreeVector", "Impossible to convert string to G4ThreeVector", JustWarning, "");
            return G4ThreeVector(0, 0, 0);
        }
    }
    else
    {
        return G4ThreeVector(0, 0, 0);
    }
}

inline G4ThreeVector Wisard_Generator::GetDirection(G4ThreeVector dirr)
{
    if (dirr == G4ThreeVector(0, 0, 0))
    {
        // RANDOM DIRECTION
        G4double phi = G4UniformRand() * 2 * M_PI;
        G4double costheta = 2 * G4UniformRand() - 1;
        G4double theta = acos(costheta);
        // G4double theta = G4UniformRand() * 2 * M_PI;
        return G4ThreeVector(sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta));
    }
    else
    {
        return dirr;
    }
}
#endif
