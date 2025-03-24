#ifndef WISARD_ACTION_INITIALIZATION_HH
#define WISARD_ACTION_INITIALIZATION_HH

#include "G4VUserActionInitialization.hh"
#include "ParticleInformation.hh"
#include "Wisard_Generator.hh"

#include "G4GenericMessenger.hh"
#include "Wisard_RunAction.hh"


class Wisard_ActionInitialization : public G4VUserActionInitialization {
public:
    Wisard_ActionInitialization(G4String);
    virtual ~Wisard_ActionInitialization();

    virtual void BuildForMaster() const override;
    virtual void Build() const override;

    G4String GetFileName();

    Wisard_Generator *gptr;
    G4GenericMessenger *ActionMessenger;
    G4String MacroFileName;
private:
    mutable Wisard_RunAction* run = nullptr;

};

#endif // WISARD_ACTION_INITIALIZATION_HH