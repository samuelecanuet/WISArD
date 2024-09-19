#ifndef WITCH_SENSOR_HH
#define WITCH_SENSOR_HH

#include "Wisard_Global.hh"
#include "G4VSensitiveDetector.hh"
#include "G4EventManager.hh"

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
  Wisard_Sensor();
  ~Wisard_Sensor();

  void Initialize(G4HCofThisEvent *);
  G4bool ProcessHits(G4Step *, G4TouchableHistory *);

  std::unordered_map<G4int, StructInfo> PrimaryDictionnary;
  StructInfo PrimaryInfo_init;

  std::unordered_map<G4int, StructInfo> GetDictionnary() { return PrimaryDictionnary; }
  void ResetDictionnary() { PrimaryDictionnary.clear(); }
  int index;
  
};




#endif
