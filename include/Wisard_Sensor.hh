#ifndef WITCH_SENSOR_HH
#define WITCH_SENSOR_HH

#include "../Wisard_Global.hh"
#include "G4VSensitiveDetector.hh"

struct PrimaryInfo
{
  G4double DepositEnergy;
  G4double HitAngle;
  G4ThreeVector HitPosition;
  G4String ParticleName;
};

//----------------------------------------------------------------------
// This class is associated with a volume of the geometry to make this
// volume "sensitive"
// When a particle track produces a "hit" in the corresponding volume,
// the ProcessHits function is called
class Wisard_Sensor : public G4VSensitiveDetector
{

  //------------------------------------------------------------
  // internal variables definition
protected:

  //------------------------------------------------------------
  // class functions definition
public:
  // constructor and destructor
  Wisard_Sensor();
  ~Wisard_Sensor();

  // hits processing
  void Initialize(G4HCofThisEvent *);
  G4bool ProcessHits(G4Step *, G4TouchableHistory *);

  std::unordered_map<G4int, PrimaryInfo> PrimaryDictionnary;

  void ResetDictionnary() { PrimaryDictionnary.clear(); }
  std::unordered_map<G4int, PrimaryInfo> GetDictionnary() { return PrimaryDictionnary; }

  PrimaryInfo PrimaryInfo_init;

  int index;
};
#endif
