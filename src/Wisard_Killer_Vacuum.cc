#include "Wisard_Killer_Vacuum.hh"

Wisard_Killer_Vacuum::Wisard_Killer_Vacuum() : G4VSensitiveDetector("WisardKillerVacuum")
{
    G4cout << "\033[32m" << "Constructor Wisard_Killer_Vacuum" << "\033[0m" << G4endl;
}

// destructor
Wisard_Killer_Vacuum::~Wisard_Killer_Vacuum()
{
}

void Wisard_Killer_Vacuum::Initialize(G4HCofThisEvent *)
{
}

G4bool Wisard_Killer_Vacuum::ProcessHits(G4Step *step, G4TouchableHistory *)
{
    // ############################################# ///
    //        !!! KILLING for performance !!!        ///
    // ############################################# ///

    // Killing the track if it is an electron or positron with a momentum along the x-y plane (z momentum direction = < 1e-10) to avoid infinite loop in the simulation (only for G4_Galatic material)
    if (abs(step->GetTrack()->GetDefinition()->GetPDGEncoding()) == 11 && abs(step->GetTrack()->GetMomentumDirection().z()) < 1e-10 && step->GetTrack()->GetMaterial() == Material_Vacuum) // e- or e+
    {
        step->GetTrack()->SetTrackStatus(fStopAndKill);
    }

    return (true);
}
