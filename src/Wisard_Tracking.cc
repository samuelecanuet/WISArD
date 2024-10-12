#include "Wisard_Tracking.hh"

#include "G4Track.hh"
#include "G4TrackVector.hh"
#include "G4TrackingManager.hh"

#include "Wisard_Global.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

Wisard_Tracking::Wisard_Tracking(ParticleInformation *Part_Infos) : G4UserTrackingAction(), Part_Info(Part_Infos)
{
    G4cout << "Constructor Wisard_Tracking" << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

Wisard_Tracking::~Wisard_Tracking()
{
    G4cout << "Destructor Wisard_Tracking" << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

void Wisard_Tracking::PreUserTrackingAction(const G4Track *)
{
}

void Wisard_Tracking::PostUserTrackingAction(const G4Track *track)
{

    G4TrackVector *childrens = fpTrackingManager->GimmeSecondaries();
    if (track->GetParentID() == 0)
    {
        for (unsigned int index = 0; index < childrens->size(); ++index)
        {
            G4Track *tr = (*childrens)[index];
            if (tr->GetCreatorProcess() && tr->GetCreatorProcess()->GetProcessType() == fDecay)
            {
                tr->SetParentID(tr->GetTrackID());
            }
        }
        Part_Info->SetParticle(track->GetTrackID(), track->GetDefinition()->GetPDGEncoding(), track->GetVertexKineticEnergy() / keV, track->GetVertexMomentumDirection(), track->GetVertexPosition (), track->GetGlobalTime());
    }
    else
    {
        for (unsigned int index = 0; index < childrens->size(); ++index)
        {
            G4Track *tr = (*childrens)[index];
            tr->SetParentID(track->GetParentID());
        }
    }
}