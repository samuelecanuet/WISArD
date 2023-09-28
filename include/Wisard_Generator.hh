#ifndef WITCH_GENERATOR_HH
#define WITCH_GENERATOR_HH

#include "../Wisard_Global.hh"
#include "G4VUserPrimaryGeneratorAction.hh"
#include "Wisard_RunManager.hh"
#include "Wisard_Tracking.hh"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <random>
//----------------------------------------------------------------------
struct Histogram
{
    double value;
    int count;
};

class Wisard_Generator : public G4VUserPrimaryGeneratorAction
{

    //------------------------------------------------------------
    // internal variables definition
protected:
    Wisard_RunManager *manager_ptr;
    int ievent, iev_len = 0, isubevent, isubev_len = 0;

    G4ParticleDefinition *part_electron;
    G4ParticleDefinition *part_positron;
    G4ParticleDefinition *part_gamma;
    G4ParticleDefinition *part_proton;
    G4ParticleDefinition *part_enubar;
    G4ParticleDefinition *part_alpha;
    G4ParticleDefinition *part_geantino;

    G4double radius;
    G4double radius_std;
    G4double x_beam;
    G4double y_beam;
    G4double position_catcher_z;

    std::tuple<std::pair<std::vector<Histogram>, double>, std::pair<std::vector<Histogram>, double>, std::pair<std::vector<Histogram>, double>> res;

    //------------------------------------------------------------
    // class functions definition
public:
    // constructor and destructor
    Wisard_Generator(Wisard_RunManager *mgr);
    ~Wisard_Generator();

    // function generating the event primary vertex with emitted particles
    void GeneratePrimaries(G4Event *event);

    std::tuple<std::pair<std::vector<Histogram>, double>, std::pair<std::vector<Histogram>, double>, std::pair<std::vector<Histogram>, double>> GetSRIM_hist();
    double GetSRIM_data(std::vector<Histogram>, double);

    void SetBeamSize(G4double r, G4double r_std);
    void SetBeamPosition(G4double x, G4double y);
    void SetCatcherPosition_z(G4double catcher_z);
};

inline std::tuple<std::pair<std::vector<Histogram>, double>, std::pair<std::vector<Histogram>, double>, std::pair<std::vector<Histogram>, double>> Wisard_Generator::GetSRIM_hist()
{
    if (manager_ptr->GetInputSRIM().fail())
    {
        G4Exception("Wisard_Generator::GetSRIM_hist", "SRIM file wrong", JustWarning, "");
    }

    // Lire les données des histogrammes dans des vecteurs
    std::vector<Histogram> histogrammeX, histogrammeY, histogrammeZ;
    double x, y, z, sumX, sumY, sumZ;
    int countX, countY, countZ;

    while (manager_ptr->GetInputSRIM() >> z >> countZ >> y >> countY >> x >> countX)
    {
        histogrammeX.push_back({x, countX});
        histogrammeY.push_back({y, countY});
        histogrammeZ.push_back({z, countZ});

        sumX += countX;
        sumY += countY;
        sumZ += countZ;
    }

    return std::make_tuple(std::make_pair(histogrammeX, sumX), std::make_pair(histogrammeY, sumY), std::make_pair(histogrammeZ, sumZ));
}

inline double Wisard_Generator::GetSRIM_data(std::vector<Histogram> histogram, double totalWeight)
{
    std::random_device rd;
    std::mt19937 gen(rd());

    // Générer un nombre aléatoire pondéré entre 0 et la somme totale des poids
    std::uniform_real_distribution<> distrib(0.0, totalWeight);
    double randomValue = distrib(gen);

    // Parcourir les bins de l'histogramme et sélectionner le bin en fonction du nombre aléatoire
    double cumulativeWeight = 0.0;
    for (const Histogram &bin : histogram)
    {
        cumulativeWeight += bin.count;
        if (randomValue <= cumulativeWeight)
        {
            return bin.value;
        }
    }
}
inline void Wisard_Generator::SetBeamSize(G4double r, G4double r_std)
{
    radius = r;
    radius_std = r_std;
}

inline void Wisard_Generator::SetBeamPosition(G4double x, G4double y)
{
    x_beam = x;
    y_beam = y;
}

inline void Wisard_Generator::SetCatcherPosition_z(G4double catcher_z)
{
    position_catcher_z = catcher_z;
}
#endif
