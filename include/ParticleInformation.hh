#ifndef ParticleINFO
#define ParticleINFO

#include <vector>
#include <map>

#include <G4ThreeVector.hh>
#include "G4ParticleTable.hh"
using namespace std;



struct Detector
{
    G4double EnergyDeposit = 0;
    G4double HitAngle = 0;
    G4ThreeVector HitPosition = G4ThreeVector(0, 0, 0);
    G4double HitTime = 0;
};

struct InterStripDetector
{
    vector<G4double> EnergyDeposit = {};
    vector<G4ThreeVector> HitPosition = {G4ThreeVector(0, 0, 0)};
};

struct Particle
{
    G4int TrackID = 0;
    G4int Particle_PDG = 0;
    G4double E0 = 0;
    G4ThreeVector Dir = G4ThreeVector(0, 0, 0);
    G4ThreeVector Pos = G4ThreeVector(0, 0, 0);

    map<G4int, Detector> Detectors;
    map<G4int, InterStripDetector> InterStripDetectors;
};

class ParticleInformation 
{
public:
    ParticleInformation();
    ~ParticleInformation();

    map<G4int, Particle> Particles;
    G4ParticleTable *particleTable = G4ParticleTable::GetParticleTable();


    static map<G4int, Detector> AllEmptyDetectors;
    static map<G4int, InterStripDetector> AllEmptyInterStripDetectors;

    void AddParticle(G4int TrackID);
    void SetParticle(G4int TrackID, G4int Particle_PDG, G4double E0, G4ThreeVector Dir, G4ThreeVector Pos);
    void AddEnergyDeposit(G4int TrackID, G4int SensorID, G4double EnergyDeposit);
    void SetHitAngle(G4int TrackID, G4int SensorID, G4double HitAngle);
    void SetHitPosition(G4int TrackID, G4int SensorID, G4ThreeVector HitPosition);
    bool FirstHit(G4int TrackID, G4int SensorID);
    void AddHitPosition(G4int TrackID, G4int SensorID, G4ThreeVector HitPosition);
    void SetHitTime(G4int TrackID, G4int SensorID, G4double HitTime);

    map<int, Particle> GetInfo();
    void Parse();
    void Clear();
};

#endif