#include "G4UserTrackingAction.hh"
#include "ParticleInformation.hh"
#include "G4EventManager.hh"
#include "Wisard_EventAction.hh"

class Wisard_Tracking : public G4UserTrackingAction
{
public:
  Wisard_Tracking();

  virtual ~Wisard_Tracking();

  virtual void PreUserTrackingAction(const G4Track *);
  virtual void PostUserTrackingAction(const G4Track *);
};