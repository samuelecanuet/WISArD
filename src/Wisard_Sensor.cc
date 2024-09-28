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

Wisard_Sensor::Wisard_Sensor()
    : G4VSensitiveDetector("WisardSensor")
{
}

// destructor
Wisard_Sensor::~Wisard_Sensor()
{
}

void Wisard_Sensor::Initialize(G4HCofThisEvent *)
{
  ResetDictionnary();
}

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
  // == if first time seen 
  if (PrimaryDictionnary.find(index) == PrimaryDictionnary.end())
  {
    // Initial info for this volume, particle event name to get the name of the initial particle, position hit and angle if the particle is primary 
    PrimaryDictionnary[index] = PrimaryInfo_init;
    if (track->GetParentID() == 0)
    {
      PrimaryDictionnary[index].HitPosition = step->GetPreStepPoint()->GetPosition() / mm;
      if (track->GetTouchable()->GetVolume()->GetCopyNo() == 99) // plastic scintillator
      {
        PrimaryDictionnary[index].HitAngle = std::acos(G4ThreeVector(0, 0, 1) * track->GetMomentumDirection()) / deg;
      }
      else 
      {
        PrimaryDictionnary[index].HitAngle = std::acos(step->GetPreStepPoint()->GetTouchableHandle()->GetSolid()->SurfaceNormal(step->GetPreStepPoint()->GetPosition()) * track->GetMomentumDirection()) / deg;
      }
    }
  }

  /// If is an interstrip
  if (step->GetPreStepPoint()->GetPhysicalVolume()->GetCopyNo() > 1000)
  {
    G4ThreeVector localPos = step->GetPreStepPoint()->GetTouchableHandle()->GetHistory()->GetTopTransform().TransformPoint(step->GetPreStepPoint()->GetPosition());
    PrimaryDictionnary[index].InterStrip_HitPosition.push_back(localPos /mm);
    PrimaryDictionnary[index].InterStrip_EnergyDeposit.push_back(step->GetTotalEnergyDeposit() / keV);
  }


  PrimaryDictionnary[index].EnergyDeposit += step->GetTotalEnergyDeposit() / keV ;
  
  
  

  // ####################################################### ///
  // !!! KILLING down-positron/electron for performance !!!  ///
  // ####################################################### ///

  if (step->GetPreStepPoint()->GetPhysicalVolume()->GetCopyNo() == 1 && step->GetPostStepPoint()->GetPosition().z() < step->GetTrack()->GetVertexPosition().z())
  {
    if (step->GetPostStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume()->GetName() == "World" || step->GetPostStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume()->GetName() == "logic_mother_catcher")
    {
      if (step->GetTrack()->GetDefinition()->GetPDGEncoding () == -11 || step->GetTrack()->GetDefinition()->GetPDGEncoding () == 11)
      {
        step->GetTrack()->SetTrackStatus(fStopAndKill);
      }
    }
  }

  return (true);
}
