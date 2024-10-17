#ifndef WISARD_GENERATOR_HH
#define WISARD_GENERATOR_HH

#include "Wisard_Global.hh"
#include "G4VUserPrimaryGeneratorAction.hh"
#include "Wisard_RunManager.hh"

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

#include "TF2.h"



//----------------------------------------------------------------------
struct Histogram
{
    G4double value_x;
    G4double value_y;
    G4double value_z;
    G4int count;
};

struct TupleHash {
    template <class T1, class T2, class T3>
    std::size_t operator () (const std::tuple<T1, T2, T3>& t) const {
        auto h1 = std::hash<T1>{}(std::get<0>(t));
        auto h2 = std::hash<T2>{}(std::get<1>(t));
        auto h3 = std::hash<T3>{}(std::get<2>(t));
        return h1 ^ h2 ^ h3;
    }
};

class Wisard_Generator : public G4VUserPrimaryGeneratorAction
{
    //------------------------------------------------------------
    // internal variables definition
protected:
    Wisard_RunManager *manager_ptr;
   G4double energy;
    int ievent, iev_len = 0, isubevent, isubev_len = 0;

    G4ParticleTable *particle_table = G4ParticleTable::GetParticleTable();
    G4ParticleDefinition *part_electron;
    G4ParticleDefinition *part_positron;
    G4ParticleDefinition *part_gamma;
    G4ParticleDefinition *part_proton;
    G4ParticleDefinition *part_enubar;
    G4ParticleDefinition *part_alpha;
    G4ParticleDefinition *part_geantino;

    G4GenericMessenger* BeamMessenger;
    G4GenericMessenger* InputMessenger;
   G4double X, Y, Sigma_X, Sigma_Y, Q, Position_catcher_z, Radius;
    G4int Z, A;
    G4String InputFileName, SRIMFileName, Nucleus;
    ifstream InputTXT, SRIMTXT;
    TFile *InputROOT;

    G4ThreeVector position_array[10001];
    G4ThreeVector direction_array[10001];
    G4double energy_array[10001];
    G4int event_counter = 0;
    
    std::tuple<G4double, G4double, G4double> tuple;
    G4double x_SRIM, y_SRIM, z_SRIM;

    G4ParticleDefinition *particle;
    G4ThreeVector dir;
    G4ParticleGun gun;

    std::pair<std::vector<Histogram>, G4double> res;

    Long64_t current_entry = 0;

    //------------------------------------------------------------
    // class functions definition
private:
    TFile* root_file;
    unique_ptr<TTreeReader> Reader;

    unique_ptr<TTreeReaderValue<Int_t>> code;
    unique_ptr<TTreeReaderValue<Int_t>> eventid;
    unique_ptr<TTreeReaderValue<double>> ekin_;
    unique_ptr<TTreeReaderValue<double>> px;
    unique_ptr<TTreeReaderValue<double>> py;
    unique_ptr<TTreeReaderValue<double>> pz;

    TH1D* Energy_Hist;

    TF2 *Gauss2D;
public:
    Wisard_Generator(Wisard_RunManager *mgr);
    ~Wisard_Generator();

    void GeneratePrimaries(G4Event *event);
    void TXT_GENERATOR(G4Event *event);
    void ROOT_GENERATOR(G4Event *event);
    void ROOT_DISTRIBUTION_GENERATOR(G4Event *event);
    void ION_GENERATOR(G4Event *event);
    std::function<void(G4Event*)> GENERATOR;

    std::pair<std::vector<Histogram>, G4double>GetSRIM_hist();
    std::tuple<G4double, G4double, G4double> GetSRIM_data(std::vector<Histogram>, G4double);

    void SetCatcherPosition_z(G4double catcher_z);
    void ChooseGENERATOR();
    void InitBeam();
    G4ThreeVector Beam();

};

inline std::pair<std::vector<Histogram>, G4double> Wisard_Generator::GetSRIM_hist()
{
    SRIMTXT.open(SRIMFileName.c_str());
    if (SRIMTXT.fail())
    {
        G4Exception("Wisard_Generator::GetSRIM_hist", "Impossible to open SRIM File", JustWarning, "");
    }

    std::unordered_map<std::tuple<int, int, int>, int, TupleHash> value_counts;
    G4double sum = 0;
    
    
    std::string ligne;
    while (std::getline(SRIMTXT, ligne)) {
        
        std::istringstream iss(ligne);
        std::string x_str, y_str, z_str;
       G4double num;
        
        if (iss >> num >> z_str >> y_str >> x_str) {
            
           G4double x = std::stod(x_str.replace(x_str.find(','), 1, ".")); 
           G4double y = std::stod(y_str.replace(y_str.find(','), 1, "."));
           G4double z = std::stod(z_str.replace(z_str.find(','), 1, "."));
        
            int rounded_x = static_cast<int>(std::round(x));
            int rounded_y = static_cast<int>(std::round(y));
            int rounded_z = static_cast<int>(std::round(z));
            
            std::tuple<int, int, int> key(rounded_x, rounded_y, rounded_z);
            value_counts[key]++;
        }
    }
    std::vector<Histogram> histogram_vector;
    
    for (const auto& pair : value_counts) {
        Histogram histogram;
        histogram.value_x = static_cast<G4double>(std::get<0>(pair.first));
        histogram.value_y = static_cast<G4double>(std::get<1>(pair.first));
        histogram.value_z = static_cast<G4double>(std::get<2>(pair.first));
        histogram.count = pair.second;
        sum += pair.second;
        
        
        histogram_vector.push_back(histogram);
    }

    return std::make_pair(histogram_vector, sum);
}

inline std::tuple<G4double, G4double, G4double> Wisard_Generator::GetSRIM_data(std::vector<Histogram> histogram,G4double totalWeight)
{
    G4double randomValue = G4RandFlat::shoot(0.0, totalWeight);
    G4double cumulativeWeight = 0.0;
    for (const Histogram &bin : histogram)
    {
        cumulativeWeight += bin.count;
        if (randomValue <= cumulativeWeight)
        {
            return std::make_tuple(bin.value_x, bin.value_y, bin.value_z);
        }
    }
    return std::make_tuple(0.0, 0.0, 0.0);
}

inline void Wisard_Generator::SetCatcherPosition_z(G4double catcher_z)
{
    Position_catcher_z = catcher_z;
}

inline void Wisard_Generator::ChooseGENERATOR()
{
    if (Nucleus != "" && InputFileName != "")
    {
        G4Exception("Wisard_Generator::ChooseGENERATOR", "Both Nucleus and InputFile set in the macro", JustWarning, "");
    }
    else if (Nucleus != "")
    {
        A = stoi(Nucleus.substr(0, Nucleus.find_first_not_of("0123456789")));
        G4String symbol = Nucleus.substr(Nucleus.find_first_not_of("0123456789"));
        G4IonTable *ionTable = G4IonTable::GetIonTable();
        Z = -1;
        for (int i = 1 ; i < A ; i++)
        {
            G4ParticleDefinition *ion = ionTable->GetIon(i, A, 0);
            if (ion->GetParticleName().substr(0, ion->GetParticleName().find_first_of("0123456789")) == symbol)
            {
                Z = ion->GetAtomicNumber();
                break;
            }
        }
        if (Z == -1)
        {
            G4Exception("Wisard_Generator::ChooseGENERATOR", "Nucleus not found", JustWarning, "");
        }
        GENERATOR = std::bind(&Wisard_Generator::ION_GENERATOR, this, std::placeholders::_1);
    }
    else if (InputFileName.find(".txt") != std::string::npos)
    {
        InputTXT.open(InputFileName.c_str());
        if (InputTXT.fail())
        {
            G4Exception("Wisard_Generator::ChooseGENERATOR", "Impossible to open input TXT file", JustWarning, "");
        }
        GENERATOR = std::bind(&Wisard_Generator::TXT_GENERATOR, this, std::placeholders::_1);
    }
    else if (InputFileName.find(".root") != std::string::npos)
    {
        InputROOT = new TFile(InputFileName.c_str(), "READ");
        if (InputROOT->IsZombie())
        {
            G4Exception("Wisard_Generator::ChooseGENERATOR", "Impossible to open input ROOT file", JustWarning, "");
        }
        if (InputFileName.find("RMATRIX") != std::string::npos)
        {
            if (InputFileName.find("32Ar") == std::string::npos && InputFileName.find("33Ar") == std::string::npos)
                particle = part_alpha;
            else
                particle = part_proton;
            
            GENERATOR = std::bind(&Wisard_Generator::ROOT_DISTRIBUTION_GENERATOR, this, std::placeholders::_1);
            Energy_Hist = (TH1D *)root_file->Get("histogram");
        }
        else
        {
            GENERATOR = std::bind(&Wisard_Generator::ROOT_GENERATOR, this, std::placeholders::_1);
            Reader = std::make_unique<TTreeReader>("ParticleTree", InputROOT);
            code = std::make_unique<TTreeReaderValue<int>>(*Reader, "code");
            eventid = std::make_unique<TTreeReaderValue<int>>(*Reader, "event");
            ekin_ = std::make_unique<TTreeReaderValue<double>>(*Reader, "energy");
            px = std::make_unique<TTreeReaderValue<double>>(*Reader, "px");
            py = std::make_unique<TTreeReaderValue<double>>(*Reader, "py");
            pz = std::make_unique<TTreeReaderValue<double>>(*Reader, "pz");
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
    Gauss2D->SetParameters(X, Sigma_X, Y, Sigma_Y);
}

inline G4ThreeVector Wisard_Generator::Beam()
{
    G4double x, y, z;
    x = 0;
    y = 0;

    //Shoot in Beam profile
    // Gauss2D->GetRandom2(x, y);
    z = Position_catcher_z;

    //Adding SRIM 
    x += get<0>(tuple) * angstrom;
    y += get<1>(tuple) * angstrom;
    z += get<2>(tuple) * angstrom;

    return G4ThreeVector(x, y, z);
}
#endif
