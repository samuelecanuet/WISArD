#include "Wisard_MCP.hh"

Wisard_MCP::Wisard_MCP() : G4VSensitiveDetector("WisardKiller")
{
    G4cout << "\033[32m" << "Constructor Wisard_MCP"  << "\033[0m" << G4endl;
    RunAction = (Wisard_RunAction*)G4RunManager::GetRunManager()->GetUserRunAction();
}

// destructor
Wisard_MCP::~Wisard_MCP()
{
}

void Wisard_MCP::Initialize(G4HCofThisEvent *)
{
}

G4bool Wisard_MCP::ProcessHits(G4Step *step, G4TouchableHistory *)
{
    // ############################################# ///
    //        !!! KILLING for performance !!!        ///
    // ############################################# ///

    RunAction->FillMCP(step->GetTrack()->GetPosition().x(), step->GetTrack()->GetPosition().y());

    // kill the particle
    step->GetTrack()->SetTrackStatus(fStopAndKill);

    return (true);
}
