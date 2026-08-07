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

#include "G4Trap.hh"

#include "Wisard_EventAction.hh"
#include "G4Track.hh"


Wisard_Sensor::Wisard_Sensor(G4int code, G4String DetName = "Sensor") : G4VSensitiveDetector(DetName), DetCode(code)
{

  
  
}

// destructor
Wisard_Sensor::~Wisard_Sensor()
{
}

void Wisard_Sensor::Initialize(G4HCofThisEvent *)
{
}

G4bool Wisard_Sensor::ProcessHits(G4Step *step, G4TouchableHistory *)
{
  // G4cout << "ProcessHits" << G4endl;

  // cout << "ProcessHits in Wisard_Sensor for DetCode = " << DetCode << G4endl;

  G4EventManager *evtman = G4EventManager::GetEventManager();
  Wisard_EventAction *evtac = (Wisard_EventAction *)evtman->GetUserEventAction();
  ParticleInformation* PartInfo = (ParticleInformation *)evtac->GetParticleInformation();


  G4Track *track = step->GetTrack();
  G4int index;

  // -------- Getting the primary track id -------- //
  if (track->GetParentID() == 0)
  {
    index =track->GetTrackID();
  }
  else
  {
    index = track->GetParentID();
  }
  // ---------------------------------------------- //

  // -------- Creating Hit for a detector -------- //
  if (PartInfo->FirstHit(index, DetCode) && DetCode < 1000)
  {
    PartInfo->SetHitPosition(index, DetCode, step->GetPreStepPoint()->GetPosition() / mm);
    PartInfo->SetHitTime(index, DetCode, step->GetPreStepPoint()->GetGlobalTime() / ns);
    if (DetCode == 99)
    {
      PartInfo->SetHitAngle(index, DetCode, std::acos(G4ThreeVector(0, 0, 1) * track->GetMomentumDirection()) / deg);
    }
    else if (DetCode >= 11 && DetCode <= 85)
    {
      PartInfo->SetHitAngle(index, DetCode, std::acos(step->GetPreStepPoint()->GetTouchableHandle()->GetSolid()->SurfaceNormal(step->GetPreStepPoint()->GetPosition()) * track->GetMomentumDirection()) / deg);

      G4ThreeVector localPos = step->GetPreStepPoint()->GetTouchableHandle()->GetHistory()->GetTopTransform().TransformPoint(step->GetPreStepPoint()->GetPosition());
      G4Trap *trap = (G4Trap *)(step->GetPreStepPoint()->GetTouchableHandle()->GetSolid());
      G4double min_x = std::min(trap->DistanceToOut(localPos, ex), trap->DistanceToOut(localPos, -ex));
      G4double min_y = std::min(trap->DistanceToOut(localPos, ey), trap->DistanceToOut(localPos, -ey));
      G4double min_z = std::min(trap->DistanceToOut(localPos, ez), trap->DistanceToOut(localPos, -ez));
      G4ThreeVector DistanceBoundary = G4ThreeVector(min_x, min_y, min_z);
      PartInfo->SetHitDistanceBoundary(index, DetCode, DistanceBoundary / mm);
    }
    else
    {
      PartInfo->SetHitAngle(index, DetCode, std::acos(step->GetPreStepPoint()->GetTouchableHandle()->GetSolid()->SurfaceNormal(step->GetPreStepPoint()->GetPosition()) * track->GetMomentumDirection()) / deg);
    }
  }
  // ------------------------------------------- //

  // // -------- BACKSCATTERING -------- //
  if (track->GetParentID() == 0)
  {
    // - Saving the z momentum of the particle when it enters the volume
    if (track->GetCurrentStepNumber() == 1) // for the primary particle generation
    {
      // G4cout << "------ The particle **ENTER** the volume with Name = " << step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetName() << G4endl;
      PartInfo->UpdateLastEntryInVolume(index, DetCode, track->GetVertexMomentumDirection().z());
    }
    else if (step->GetPreStepPoint()->GetProcessDefinedStep()->GetProcessType() == fTransportation)
    {
      // G4cout << "------ The particle **ENTER** the volume with Name = " << step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetName() << G4endl;
      PartInfo->UpdateLastEntryInVolume(index, DetCode, step->GetPreStepPoint()->GetMomentumDirection().z());
    }
  
    // - Checking if the particle is backscattered when it leaves the volume
    if (step->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessType() == fTransportation)
    {
      // G4cout << "------ The particle **LEAVE** the volume with Name = " << step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetName() << G4endl;
      G4double z_momentum = step->GetPostStepPoint()->GetMomentumDirection().z();
      G4double z_momentum_entry = PartInfo->Particles[index].Detectors[DetCode].LastEntryInVolume;
      if (z_momentum * z_momentum_entry < 0) // The particle has changed direction in z, so it is backscattered
      {
        // G4cout << "########### The particle is backscattered in the volume with Name = " << step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetName() << " ########### " << G4endl;
        PartInfo->SetBackScattering(index, DetCode);
      }
      // else the particle only go through the volume without backscattering
    }
  }
  // -------------------------------- //

  // -------- Adding tenergy deposit to the sensor -------- //
  if (DetCode == 99)
  {
    PartInfo->AddEnergyDeposit(index, DetCode, step->GetTotalEnergyDeposit() / keV, emSaturation->VisibleEnergyDepositionAtAStep(step) / keV);
    // Only for primary particle which leave the plasticscintillator with a direction along -z axis (backscattering)
    // if (step->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessType() == fTransportation && track->GetParentID() == 0 && track->GetMomentumDirection().z() < 0)
    // {
    //     PartInfo->SetBackScattering(index, DetCode);
    // }
  }
  else if (DetCode >= 11 && DetCode <= 85)
  {
    PartInfo->AddEnergyDeposit(index, DetCode, (step->GetTotalEnergyDeposit()-step->GetNonIonizingEnergyDeposit()) / keV);    
  }
  else if (DetCode == 100)
  {
    PartInfo->AddEnergyDeposit(index, DetCode, step->GetTotalEnergyDeposit() / keV);
    // if (step->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessType() == fTransportation && track->GetParentID() == 0 && track->GetMomentumDirection().z() > 0)
    // {
    //     PartInfo->SetBackScattering(index, DetCode);
    // }
  }
  else
  {
    PartInfo->AddEnergyDeposit(index, DetCode, step->GetTotalEnergyDeposit() / keV);
  }


  // Killing the track if it is an electron or positron with a momentum along the x-y plane (z momentum direction = < 1e-10) to avoid infinite loop in the simulation
  if (abs(track->GetDefinition()->GetPDGEncoding()) == 11 && abs(track->GetMomentumDirection().z()) < 1e-10 && track->GetMaterial() == Material_Vacuum) // e- or e+
  {
    track->SetTrackStatus(fStopAndKill);
  }

  // ----------------------------------------------------- //

  return (true);
}
