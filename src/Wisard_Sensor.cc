#include "Wisard_Sensor.hh"
#include "G4VProcess.hh"
#include "G4EventManager.hh"
#include "G4TrackingManager.hh"

//----------------------------------------------------------------------

// constructor of the sensitive detector associated to the Silicon Detector
Wisard_Sensor::Wisard_Sensor()
    : G4VSensitiveDetector("WisardSensor")
{
  energy = 0.L;
  energy_positron = 0.L;
  position = G4ThreeVector(DBL_MAX, DBL_MAX, DBL_MAX);
  position_positron = G4ThreeVector(DBL_MAX, DBL_MAX, DBL_MAX);
  counter = 0;
}

// destructor
Wisard_Sensor::~Wisard_Sensor()
{
}

//----------------------------------------------------------------------

// Function called at the beginning of the event processing
// (to initialise the list of "hits" associated to the detector)
void Wisard_Sensor::Initialize(G4HCofThisEvent *hit_collection)
{
  // just to avoid compilation warning
  if (hit_collection == NULL)
  {
  }

  cerr << "Wisard_Sensor Initialisation" << endl;

  energy = 0.L;
  energy_positron = 0.L;
  position = G4ThreeVector(DBL_MAX, DBL_MAX, DBL_MAX);
  position_positron = G4ThreeVector(DBL_MAX, DBL_MAX, DBL_MAX);
  counter = 0;

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
      PrimaryInfo_init.HitAngle = std::asin(step->GetPreStepPoint()->GetTouchableHandle()->GetSolid()->SurfaceNormal(step->GetPreStepPoint()->GetPosition()) * track->GetMomentumDirection()) / deg;
      PrimaryInfo_init.HitPosition = step->GetPreStepPoint()->GetPosition();
      PrimaryInfo_init.DepositEnergy = 0;
      PrimaryDictionnary[index] = PrimaryInfo_init;
    }
  }
  else
  {
    index = track->GetParentID();
  }

  //PrimaryDictionnary[index].DepositEnergy += step->GetTotalEnergyDeposit() / keV;

  ////new for pl
  if (track->GetVolume()->GetName() == "PlasticScintillator")
    {
      if (track->GetParentID() == 0)
      {
        PrimaryDictionnary[index].DepositEnergy += step->GetTotalEnergyDeposit() / keV;
      }
    }
  else
  {
    PrimaryDictionnary[index].DepositEnergy += step->GetTotalEnergyDeposit() / keV;
  }


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
