#include "Wisard_Sensor.hh"
#include "G4VProcess.hh"
#include "G4EventManager.hh"
#include "G4TrackingManager.hh"

//----------------------------------------------------------------------

// constructor of the sensitive detector associated to the Silicon Detector
Wisard_Sensor::Wisard_Sensor()
    : G4VSensitiveDetector("WisardSensor")
{
}

// destructor
Wisard_Sensor::~Wisard_Sensor()
{
}

//----------------------------------------------------------------------

// Function called at the beginning of the event processing
// (to initialise the list of "hits" associated to the detector)
void Wisard_Sensor::Initialize(G4HCofThisEvent *)
{
  cerr << "Wisard_Sensor Initialisation" << endl;

  PrimaryInfo_init.DepositEnergy = 0;
  PrimaryInfo_init.HitPosition = G4ThreeVector(0, 0, 0);
  PrimaryInfo_init.HitAngle = 0;
  PrimaryInfo_init.ParticleName = "Unknown";
}

// Function called when the track step occurs in the detector
G4bool Wisard_Sensor::ProcessHits(G4Step *step, G4TouchableHistory *)
{
  G4Track *track = step->GetTrack();

  if (track->GetParentID() == 0)
  {
    index = track->GetTrackID();
    if (step->IsFirstStepInVolume())
    {
      PrimaryInfo_init.ParticleName = track->GetDefinition()->GetParticleName();
      PrimaryInfo_init.HitAngle = std::acos(step->GetPreStepPoint()->GetTouchableHandle()->GetSolid()->SurfaceNormal(step->GetPreStepPoint()->GetPosition()) * track->GetMomentumDirection()) / deg;
      PrimaryInfo_init.HitPosition = step->GetPreStepPoint()->GetPosition() / mm;
      PrimaryInfo_init.DepositEnergy = 0;
      PrimaryDictionnary[index] = PrimaryInfo_init;
    }
  }
  else
  {
    index = track->GetParentID();
  }

  PrimaryDictionnary[index].DepositEnergy += step->GetTotalEnergyDeposit() / keV;


  // ####################################################################
  // !!! KILLING down-positron/electron for performance !!!
  // ####################################################################

  // if (step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume()->GetName() == "LogicMylarSource" && step->GetPostStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume()->GetName() == "World" && step->GetTrack()->GetPosition().z() < -0.*mm)
  if (step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume()->GetName() == "LogicAlSource1" && step->GetTrack()->GetTrackStatus() == fAlive)
  {
    if (step->GetPostStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume()->GetName() == "World")
    {
      if (step->GetTrack()->GetDefinition()->GetParticleName() == "e+" || step->GetTrack()->GetDefinition()->GetParticleName() == "e-")
      {
        step->GetTrack()->SetTrackStatus(fStopAndKill);
      }
    }
  }

  return (true);
}
