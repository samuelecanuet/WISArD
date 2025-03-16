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

  G4EventManager *evtman = G4EventManager::GetEventManager();
  Wisard_EventAction *evtac = (Wisard_EventAction *)evtman->GetUserEventAction();
  ParticleInformation* PartInfo = (ParticleInformation *)evtac->GetParticleInformation();


  G4Track *track = step->GetTrack();
  G4int index;

  // //Getting the primary track id
  if (track->GetParentID() == 0)
  {
    index =track->GetTrackID();
  }
  else
  {
    index = track->GetParentID();
  }

  //new 
  if (PartInfo->FirstHit(index, DetCode) && DetCode < 1000)
  {
    PartInfo->SetHitPosition(index, DetCode, step->GetPreStepPoint()->GetPosition() / mm);
    PartInfo->SetHitTime(index, DetCode, step->GetPreStepPoint()->GetGlobalTime() / ns);
    if (DetCode == 99)
    {
      PartInfo->SetHitAngle(index, DetCode, std::acos(G4ThreeVector(0, 0, 1) * track->GetMomentumDirection()) / deg);
    }
    else
    {
      PartInfo->SetHitAngle(index, DetCode, std::acos(step->GetPreStepPoint()->GetTouchableHandle()->GetSolid()->SurfaceNormal(step->GetPreStepPoint()->GetPosition()) * track->GetMomentumDirection()) / deg);
    }
  }

  PartInfo->AddEnergyDeposit(index, DetCode, step->GetTotalEnergyDeposit() / keV);
  

  // TODO : Add Birks law for scintillator
  // if (DetCode == 99)
  // {
  //   PartInfo->AddEnergyDeposit(index, DetCode, emSaturation->VisibleEnergyDepositionAtAStep(step) / keV);
  // }
  // else
  // {
  //   PartInfo->AddEnergyDeposit(index, DetCode, step->GetTotalEnergyDeposit() / keV);
  // }

  // if (DetCode == 99)
  // {
  //   double edep = step->GetTotalEnergyDeposit() / keV;
  //   double Vedep = emSaturation->VisibleEnergyDepositionAtAStep(step) / keV;

  //   if (edep != Vedep)
  //   {
  //     G4cout << "edep = " << edep << " Vedep = " << Vedep << G4endl;
  //   }
  // }

  return (true);
}
