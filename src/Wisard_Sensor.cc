#include "Wisard_Sensor.hh"
#include "G4VProcess.hh"

//----------------------------------------------------------------------

// constructor of the sensitive detector associated to the Silicon Detector
Wisard_Sensor::Wisard_Sensor( )
         : G4VSensitiveDetector ( "WisardSensor" )
{
  cout << "Constructor Wisard_Sensor" << endl;
  energy = 0.L;
  energy_positron = 0.L;
  position = G4ThreeVector(DBL_MAX,DBL_MAX,DBL_MAX);
  position_positron = G4ThreeVector(DBL_MAX,DBL_MAX,DBL_MAX);

}

// destructor
Wisard_Sensor::~Wisard_Sensor()
{
  cout << "Destructor Wisard_Sensor" << endl;
}

//----------------------------------------------------------------------

// Function called at the beginning of the event processing
// (to initialise the list of "hits" associated to the detector)
void Wisard_Sensor::Initialize  ( G4HCofThisEvent * hit_collection )
{
  // just to avoid compilation warning
  if ( hit_collection == NULL) { }

  cerr << "Wisard_Sensor Initialisation" << endl;

  energy = 0.L;
  energy_positron = 0.L;
  position = G4ThreeVector(DBL_MAX,DBL_MAX,DBL_MAX);
  position_positron = G4ThreeVector(DBL_MAX,DBL_MAX,DBL_MAX);
}


// Function called when the track step occurs in the detector
G4bool Wisard_Sensor::ProcessHits ( G4Step * step, G4TouchableHistory * )
{

  if (step->GetTrack()->GetDefinition()->GetParticleName() == "e+")
  {
    if (energy == 0)
    {
      position_positron = step->GetPreStepPoint()->GetPosition();
    }

    energy_positron += step->GetPreStepPoint()->GetKineticEnergy() - step->GetPostStepPoint()->GetKineticEnergy();
    // energy_positron += step->GetTotalEnergyDeposit();
  }

  if (step->GetTrack()->GetDefinition()->GetParticleName() == "proton" || step->GetTrack()->GetDefinition()->GetParticleName() == "alpha")
  {

    if (energy == 0)
    {
      position = step->GetPreStepPoint()->GetPosition();
    }

    energy += step->GetPreStepPoint()->GetKineticEnergy() - step->GetPostStepPoint()->GetKineticEnergy();
    // energy += step->GetTotalEnergyDeposit();
  }


  // ####################################################################
  // !!! KILLING eletron, gamma and down-positron for performance /// !!!  
  // ####################################################################
  if (step->GetTrack()->GetDefinition()->GetParticleName() == "gamma" ||  step->GetTrack()->GetDefinition()->GetParticleName() == "e-")
  {
    step->GetTrack()->SetTrackStatus(fStopAndKill);
  }

  //if (step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume()->GetName() == "LogicMylarSource" && step->GetPostStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume()->GetName() == "World" && step->GetTrack()->GetPosition().z() < -0.*mm)
	if (step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume()->GetName() == "LogicAlSource1" && step->GetPostStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume()->GetName() == "World")
  {
    if ( step->GetTrack()->GetDefinition()->GetParticleName() == "e+")
    {
      step->GetTrack()->SetTrackStatus(fStopAndKill);
    }
  }


  return (true);
}
