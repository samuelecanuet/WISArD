#ifndef WISARD_EVENT_ACTION_HH
#define WISARD_EVENT_ACTION_HH

#include "G4UserEventAction.hh"
#include "G4Event.hh"

#include "ParticleInformation.hh"
#include "Wisard_RunAction.hh"

class Wisard_EventAction : public G4UserEventAction
{
public:
    Wisard_EventAction();
    virtual ~Wisard_EventAction();

    virtual void BeginOfEventAction(const G4Event *event) override;
    virtual void EndOfEventAction(const G4Event *event) override;

    Wisard_RunAction *RunAction;

    ParticleInformation *PartInfo;
    ParticleInformation *GetParticleInformation() { return PartInfo; }
};

#endif // WISARD_EVENT_ACTION_HH