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

Wisard_Sensor::Wisard_Sensor( ParticleInformation* PartInfos, G4int code) : G4VSensitiveDetector("WisardSensor"), PartInfo(PartInfos), DetCode(code)
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

  //new 
  if (PartInfo->FirstHit(index, DetCode) && DetCode < 1000)
  {
    PartInfo->SetHitPosition(index, DetCode, step->GetPreStepPoint()->GetPosition() / mm);
    if (DetCode == 99)
    {
      PartInfo->SetHitAngle(index, DetCode, std::acos(G4ThreeVector(0, 0, 1) * track->GetMomentumDirection()) / deg);
    }
    else
    {
      PartInfo->SetHitAngle(index, DetCode, std::acos(step->GetPreStepPoint()->GetTouchableHandle()->GetSolid()->SurfaceNormal(step->GetPreStepPoint()->GetPosition()) * track->GetMomentumDirection()) / deg);
    }
  }

  if (DetCode > 1000) // interstrip
    PartInfo->AddHitPosition(index, DetCode, step->GetPreStepPoint()->GetPosition() / mm);

  PartInfo->AddEnergyDeposit(index, DetCode, step->GetTotalEnergyDeposit() / keV);

  // ####################################################### ///
  // !!! KILLING down-positron/electron for performance !!!  ///
  // ####################################################### ///

  // if (step->GetPreStepPoint()->GetPhysicalVolume()->GetCopyNo() == 1 && step->GetPostStepPoint()->GetPosition().z() < step->GetTrack()->GetVertexPosition().z())
  // {
  //   if (step->GetPostStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume()->GetName() == "World" || step->GetPostStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume()->GetName() == "logic_mother_catcher")
  //   {
  //     if (step->GetTrack()->GetDefinition()->GetPDGEncoding () == -11 || step->GetTrack()->GetDefinition()->GetPDGEncoding () == 11)
  //     {
  //       step->GetTrack()->SetTrackStatus(fStopAndKill);
  //     }
  //   }
  // }

  return (true);
}
