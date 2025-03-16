#include "Wisard_ActionInitialization.hh"
#include "Wisard_Tracking.hh"
#include "Wisard_EventAction.hh"
#include "ParticleInformation.hh"
#include "Wisard_RunAction.hh"

Wisard_ActionInitialization::Wisard_ActionInitialization(G4String filename) : G4VUserActionInitialization(), MacroFileName(filename)
{
    G4cout << "\033[32m" << "Constructor Wisard_ActionInitialization"  << "\033[0m" << G4endl;
}

Wisard_ActionInitialization::~Wisard_ActionInitialization() 
{
    G4cout << "\033[31m" << "Destructor Wisard_ActionInitialization"  << "\033[0m" << G4endl;
}

void Wisard_ActionInitialization::BuildForMaster() const {

    run = new Wisard_RunAction(MacroFileName);
    SetUserAction(run);
}

void Wisard_ActionInitialization::Build() const 
{
    // G4cout << "Build" << G4endl;
    run = new Wisard_RunAction(MacroFileName);

    SetUserAction(run);
    SetUserAction(new Wisard_Generator());
    SetUserAction(new Wisard_Tracking());
    SetUserAction(new Wisard_EventAction());
}

G4String Wisard_ActionInitialization::GetFileName()
{
    return run->GetFileName();
}