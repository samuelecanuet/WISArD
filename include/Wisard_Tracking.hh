#include "G4UserTrackingAction.hh"
#include "ParticleInformation.hh"

class Wisard_Tracking : public G4UserTrackingAction
{
public:
  Wisard_Tracking(ParticleInformation *);

  virtual ~Wisard_Tracking();

  virtual void PreUserTrackingAction(const G4Track *);
  virtual void PostUserTrackingAction(const G4Track *);

  ParticleInformation *Part_Info;
};