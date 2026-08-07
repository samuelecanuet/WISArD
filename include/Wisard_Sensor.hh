#ifndef WISARD_SENSOR_HH
#define WISARD_SENSOR_HH

#include "Wisard_Global.hh"
#include "G4VSensitiveDetector.hh"
#include "G4EventManager.hh"
#include "ParticleInformation.hh"
#include "G4LossTableManager.hh"
#include "G4EmSaturation.hh"
#include "G4ProcessType.hh"

#include "ParticleInformation.hh"

class Wisard_Sensor : public G4VSensitiveDetector
{
public:
  Wisard_Sensor(G4int, G4String);
  ~Wisard_Sensor();

  G4EmSaturation *emSaturation = G4LossTableManager::Instance()->EmSaturation();

  G4int DetCode;

  const G4ThreeVector ex = G4ThreeVector(1., 0., 0.);
  const G4ThreeVector ey = G4ThreeVector(0., 1., 0.);
  const G4ThreeVector ez = G4ThreeVector(0., 0., 1.);

  void Initialize(G4HCofThisEvent *);
  G4bool ProcessHits(G4Step *, G4TouchableHistory *);

private:
  G4Material *Material_Vacuum = G4Material::GetMaterial("G4_Galactic");
};

#endif
