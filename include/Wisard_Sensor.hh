#ifndef WITCH_SENSOR_HH
#define WITCH_SENSOR_HH

#include "Wisard_Global.hh"
#include "G4VSensitiveDetector.hh"
#include "G4EventManager.hh"

class Wisard_Sensor : public G4VSensitiveDetector
{
public:
  Wisard_Sensor();
  ~Wisard_Sensor();

  void Initialize(G4HCofThisEvent *);
  G4bool ProcessHits(G4Step *, G4TouchableHistory *);

  G4double DepositEnergy;
  G4double ProtonHitAngle;
  G4double PositronHitAngle;
  G4ThreeVector ProtonHitPosition;
  G4ThreeVector PositronHitPosition;

  void inline ResetDetector();
  G4double GetDepositEnergy() { return DepositEnergy; }
  G4double GetProtonHitAngle() { return ProtonHitAngle; }
  G4double GetPositronHitAngle() { return PositronHitAngle; }
  vector<G4double> GetProtonHitPosition() { return {ProtonHitPosition.x(), ProtonHitPosition.y(), ProtonHitPosition.z()}; }
  vector<G4double> GetPositronHitPosition() { return {PositronHitPosition.x(), PositronHitPosition.y(), PositronHitPosition.z()}; }
  
};

void Wisard_Sensor::ResetDetector()
{
  DepositEnergy = 0;
  ProtonHitAngle = 0;
  PositronHitAngle = 0;
  ProtonHitPosition = G4ThreeVector(0, 0, 0);
  PositronHitPosition = G4ThreeVector(0, 0, 0);
}


#endif
