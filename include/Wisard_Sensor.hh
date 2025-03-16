#ifndef WISARD_SENSOR_HH
#define WISARD_SENSOR_HH

#include "Wisard_Global.hh"
#include "G4VSensitiveDetector.hh"
#include "G4EventManager.hh"
#include "ParticleInformation.hh"
#include "G4LossTableManager.hh"
#include "G4EmSaturation.hh"

#include "ParticleInformation.hh"


class Wisard_Sensor : public G4VSensitiveDetector
{
public:
  Wisard_Sensor(G4int, G4String);
  ~Wisard_Sensor();

  // G4EmSaturation *emSaturation = G4LossTableManager::Instance()->EmSaturation();

  G4int DetCode;

  void Initialize(G4HCofThisEvent *);
  G4bool ProcessHits(G4Step *, G4TouchableHistory *);

};




#endif
