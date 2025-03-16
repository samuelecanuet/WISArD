#include "Wisard_EventAction.hh"
#include "G4Event.hh"
#include "G4AnalysisManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4EventManager.hh"
#include "G4RunManager.hh"
#include "Wisard_RunAction.hh"

Wisard_EventAction::Wisard_EventAction() 
{
    G4cout << "\033[32m" << "Constructor Wisard_EventAction" << "\033[0m" << G4endl;
    PartInfo = new ParticleInformation();
    RunAction = (Wisard_RunAction*)G4RunManager::GetRunManager()->GetUserRunAction();
}

Wisard_EventAction::~Wisard_EventAction()
{
    G4cout << "\033[31m" << "Destructor Wisard_EventAction" << "\033[0m" << G4endl;
}

void Wisard_EventAction::BeginOfEventAction(const G4Event*) {
    // G4cout << "### Event " << event->GetEventID() << " start" << G4endl;

    
}

void Wisard_EventAction::EndOfEventAction(const G4Event*) {
    // G4cout << "### Event " << event->GetEventID() << " end" << G4endl;

    RunAction->UpdateTree(PartInfo);

    PartInfo->Clear();
}