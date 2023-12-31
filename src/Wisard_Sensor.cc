#include "Wisard_Sensor.hh"
#include "G4VProcess.hh"
#include "G4TrackingManager.hh"
#include "G4Event.hh"
#include <G4Field.hh>
#include "G4FieldManager.hh"
#include "G4TransportationManager.hh"
#include "G4LogicalVolume.hh"
#include "G4TouchableHandle.hh"
#include "G4Navigator.hh"
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

  //Getting the primary track id
  if (track->GetParentID() == 0)
  {
    index = track->GetTrackID();
  }
  else
  {
    index = track->GetParentID();
  }

  // Cheking if the primary or a secondary already enter in this volume
  if (PrimaryDictionnary.find(index) == PrimaryDictionnary.end())
  {
    // Initial info for this volume, particle event name to get the name of the initial particle, position hit and angle if the particle is primary 
    PrimaryDictionnary[index] = PrimaryInfo_init;
    PrimaryDictionnary[index].ParticleName = G4EventManager::GetEventManager()->GetNonconstCurrentEvent()->GetPrimaryVertex(index - 1)->GetPrimary()->GetG4code()->GetParticleName();
    if (track->GetParentID() == 0)
    {
      PrimaryDictionnary[index].HitPosition = step->GetPreStepPoint()->GetPosition() / mm;
      if (step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume()->GetName() == "PlasticScintillator") {PrimaryDictionnary[index].HitAngle = std::acos(G4ThreeVector(0, 0, 1) * track->GetMomentumDirection()) / deg;}
      else { PrimaryDictionnary[index].HitAngle = std::acos(step->GetPreStepPoint()->GetTouchableHandle()->GetSolid()->SurfaceNormal(step->GetPreStepPoint()->GetPosition()) * track->GetMomentumDirection()) / deg;}
    }
  }

  PrimaryDictionnary[index].DepositEnergy += step->GetTotalEnergyDeposit() / keV;

  // ####################################################################
  // !!! KILLING down-positron/electron for performance !!!
  // ####################################################################

  // if (step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume()->GetName() == "LogicMylarSource" && step->GetPostStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume()->GetName() == "World" && step->GetTrack()->GetPosition().z() < -0.*mm)
  if ((step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume()->GetName() == "LogicAlSource1_side" || step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume()->GetName() == "LogicAlSource1_central") && step->GetTrack()->GetTrackStatus() == fAlive)
  {
    if (step->GetPostStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume()->GetName() == "World" || step->GetPostStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume()->GetName() == "logic_mother_catcher")
    {
      if (step->GetTrack()->GetDefinition()->GetParticleName() == "e+" || step->GetTrack()->GetDefinition()->GetParticleName() == "e-")
      {
        step->GetTrack()->SetTrackStatus(fStopAndKill);
      }
    }
  }

  return (true);
}
