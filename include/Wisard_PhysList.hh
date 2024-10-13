#include "G4VModularPhysicsList.hh"
#include "G4VUserPhysicsList.hh"
#include "G4ProcessManager.hh"  
#include "G4ParticleTypes.hh"
#include "G4ParticleDefinition.hh"
#include "G4EmStandardPhysics.hh"
#include "G4EmStandardPhysics_option4.hh"
#include "G4EmLivermorePhysics.hh"
#include "G4EmPenelopePhysics.hh"
#include "G4EmStandardPhysicsGS.hh"
#include "G4EmStandardPhysics.hh"
#include "G4DecayPhysics.hh"
#include "G4OpticalPhysics.hh"
#include "G4RadioactiveDecayPhysics.hh"
#include "G4TransportationManager.hh"

#include "Wisard_Global.hh"
#include "StepMax.hh"

//----------------------------------------------------------------------

class Wisard_PhysList: public G4VUserPhysicsList
{
  protected:
    StepMax  * step_max;

  public:
    Wisard_PhysList();
    ~Wisard_PhysList();


    G4VPhysicsConstructor *emPhysicsList;
    G4VPhysicsConstructor *decayPhysicList;

  protected:
    void ConstructParticle();
    void ConstructProcess();
    void SetCuts();

  public:
    void AddStepMax ( G4double step = 0., u_short flag = 0x7 );
    void SetStepMax ( G4double step = 1.*um );

};
