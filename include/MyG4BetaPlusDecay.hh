#ifndef MY_G4_BETAPLUS_DECAY_HH
#define MY_G4_BETAPLUS_DECAY_HH

#include "G4BetaPlusDecay.hh"
#include "globals.hh"
#include <random>

// #include "MyBetaSpectrumGenerator.hh"
#include "MyG4RadioactiveDecay.hh"

class MyG4BetaPlusDecay : public G4BetaPlusDecay {
public:
    MyG4BetaPlusDecay(const G4ParticleDefinition* theParentNucleus,
                    const G4double& theBR, const G4double& endpointE,
                    const G4double& ex, 
                    const G4Ions::G4FloatLevelBase& flb,
                    const G4BetaDecayType& type
                    //   MyG4RadioactiveDecay* theDecay
                      );

    virtual ~MyG4BetaPlusDecay();

    G4DecayProducts* DecayIt(G4double);
    // MyBetaSpectrumGenerator* betaSpectrumGenerator;

    double GetAngle(double);  
    G4double RandomFromDistribution(std::vector<std::vector<double>>);
    G4double ConvertingHalflifeToWidth(G4double halflife);
    double a = 1.0;

    double EnergyBroadering;
    G4double ShootBreitWigner(G4double width);
    G4double endpointEnergy_New;



private:

    void SetUpBetaSpectrumSampler(const G4int& parentZ, const G4int& parentA,
                                  const G4BetaDecayType& type);

    const G4double endpointEnergy;
    const G4double ExcitationEnergy;
    const G4Ions::G4FloatLevelBase FLB;
    G4RandGeneral* spectrumSampler;

    G4int daughterZ;
    G4int daughterA;
    G4BetaDecayType BETATYPE;

};

#endif // MY_G4_PROTON_DECAY_HH
