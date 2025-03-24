#include "Wisard_Killer.hh"

Wisard_Killer::Wisard_Killer() : G4VSensitiveDetector("WisardKiller")
{
    G4cout << "\033[32m" << "Constructor Wisard_Killer"  << "\033[0m" << G4endl;
}

// destructor
Wisard_Killer::~Wisard_Killer()
{
}

void Wisard_Killer::Initialize(G4HCofThisEvent *)
{
}

G4bool Wisard_Killer::ProcessHits(G4Step *step, G4TouchableHistory *)
{
    // ############################################# ///
    //        !!! KILLING for performance !!!        ///
    // ############################################# ///

    // kill the particle
    step->GetTrack()->SetTrackStatus(fStopAndKill);

    
    return (true);
}
