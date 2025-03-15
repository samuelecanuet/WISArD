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

  if (DetCode >= 11 && DetCode <= 85)
  {
  G4StepPoint *preStepPoint = step->GetPreStepPoint();
  G4ThreeVector globalPos = preStepPoint->GetPosition();

  // Get the local coordinates
  G4TouchableHandle touchable = preStepPoint->GetTouchableHandle();
  G4ThreeVector localPos = touchable->GetHistory()->GetTopTransform().TransformPoint(globalPos);
  G4ThreeVector localDir1 = G4ThreeVector(1, 0, 0);
  G4ThreeVector localDir2 = G4ThreeVector(-1, 0, 0);
  G4ThreeVector localDir3 = G4ThreeVector(0, 1, 0);
  G4ThreeVector localDir4 = G4ThreeVector(0, -1, 0);

  const G4VPhysicalVolume* physVol = touchable->GetVolume();
  const G4VSolid *solid = physVol->GetLogicalVolume()->GetSolid();
  if (auto trap = dynamic_cast<const G4Trap *>(solid))
  {
    G4bool validNorm;
    G4ThreeVector normal;

    G4double distanceToExit1 = trap->DistanceToOut(localPos, localDir1, true, &validNorm, &normal);
    // G4cout << "Distance to exit1: " << distanceToExit1 / mm << " mm" << G4endl;
    G4double distanceToExit2 = trap->DistanceToOut(localPos, localDir2, true, &validNorm, &normal);
    // G4cout << "Distance to exit2: " << distanceToExit2 / mm << " mm" << G4endl;
    G4double distanceToExit3 = trap->DistanceToOut(localPos, localDir3, true, &validNorm, &normal);
    // G4cout << "Distance to exit3: " << distanceToExit3 / mm << " mm" << G4endl;
    G4double distanceToExit4 = trap->DistanceToOut(localPos, localDir4, true, &validNorm, &normal);
    // G4cout << "Distance to exit4: " << distanceToExit4 / mm << " mm" << G4endl;


    G4double minx;
    if (distanceToExit1 < distanceToExit2)
    {
      minx = distanceToExit1;
    }
    else
    {
      minx = distanceToExit2;
    }

    G4double miny;
    if (distanceToExit3 < distanceToExit4)
    {
      miny = distanceToExit3;
    }
    else
    {
      miny = distanceToExit4;
    }

    G4double std = 0.025 * mm;
    // G4double offsetfrom_boundary = 0.5 * mm;

    // weigting with erf
    G4double weightingx = 0.5*(1+std::erf((minx+std) / (std)));
    G4double weightingy = 0.5*(1+std::erf((miny+std) / (std)));

    PartInfo->AddEnergyDeposit(index, DetCode, weightingx*weightingy*step->GetTotalEnergyDeposit() / keV);
  }

  }
  else
  {
    PartInfo->AddEnergyDeposit(index, DetCode, step->GetTotalEnergyDeposit() / keV);
  }

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
