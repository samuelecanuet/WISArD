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
  cerr << "Wisard_Sensor Initialisation" << endl;

  ResetDetector();
}

G4bool Wisard_Sensor::ProcessHits(G4Step *step, G4TouchableHistory *)
{
  G4Track *track = step->GetTrack();

  if (track->GetParentID() == 0)
  {
    if (step->IsFirstStepInVolume())
    {
      if (track->GetTouchable()->GetVolume()->GetCopyNo() == 99)
      {
        if (track->GetDefinition()->GetPDGEncoding() == 2212)
        {
          ProtonHitPosition = step->GetPreStepPoint()->GetPosition();
          ProtonHitAngle = acos(G4ThreeVector(0, 0, 1) * track->GetMomentumDirection()) / deg;
        }
        else if (track->GetDefinition()->GetPDGEncoding() == -11)
        {
          PositronHitPosition = step->GetPreStepPoint()->GetPosition();
          PositronHitAngle = acos(G4ThreeVector(0, 0, 1) * track->GetMomentumDirection()) / deg;
        }
      }
      else
      {
        if (track->GetDefinition()->GetPDGEncoding() == 2212)
        {
          ProtonHitPosition = step->GetPreStepPoint()->GetPosition();
          ProtonHitAngle = acos(step->GetPreStepPoint()->GetTouchableHandle()->GetSolid()->SurfaceNormal(step->GetPreStepPoint()->GetPosition()) * track->GetMomentumDirection()) / deg;
        }
        else if (track->GetDefinition()->GetPDGEncoding() == -11)
        {
          PositronHitPosition = step->GetPreStepPoint()->GetPosition();
          PositronHitAngle = acos(step->GetPreStepPoint()->GetTouchableHandle()->GetSolid()->SurfaceNormal(step->GetPreStepPoint()->GetPosition()) * track->GetMomentumDirection()) / deg;
        }
      }
    }
  }

  DepositEnergy += step->GetTotalEnergyDeposit() / keV;

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
