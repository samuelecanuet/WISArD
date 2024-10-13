#ifndef WISARD_SENSOR_HH
#define WISARD_SENSOR_HH

#include "Wisard_Global.hh"
#include "G4VSensitiveDetector.hh"
#include "G4EventManager.hh"
#include "ParticleInformation.hh"

struct StructInfo
{
  G4double EnergyDeposit = 0.;
  G4double HitAngle = 0.;
  G4ThreeVector HitPosition = G4ThreeVector(0, 0, 0);
  vector<G4ThreeVector> InterStrip_HitPosition = {};
  vector<G4double> InterStrip_EnergyDeposit = {};
};

class Wisard_Sensor : public G4VSensitiveDetector
{
public:
  Wisard_Sensor(ParticleInformation *, G4int );
  ~Wisard_Sensor();

  ParticleInformation* PartInfo;
  G4int DetCode;

  void Initialize(G4HCofThisEvent *);
  G4bool ProcessHits(G4Step *, G4TouchableHistory *);

  std::unordered_map<G4int, StructInfo> PrimaryDictionnary;
  StructInfo PrimaryInfo_init;

  std::unordered_map<G4int, StructInfo> GetDictionnary() { return PrimaryDictionnary; }
  vector<G4int> GetIndex()
  {
    vector<G4int> indexx;
    for (auto const &x : PrimaryDictionnary)
    {
      indexx.push_back(x.first);
    }
    return indexx;
  }
  void ResetDictionnary() { PrimaryDictionnary.clear(); }
  int index;
  void AddEnergyDeposit(G4int indexx, G4double energy) { PrimaryDictionnary[indexx].EnergyDeposit += energy; }
  
};




#endif
