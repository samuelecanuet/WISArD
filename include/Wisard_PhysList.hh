#include "../Wisard_Global.hh"
#include "G4VUserPhysicsList.hh"
#include "StepMax.hh"

//----------------------------------------------------------------------

class Wisard_PhysList: public G4VUserPhysicsList
{

  //------------------------------------------------------------
  // internal variables definition
  protected:
    // no variable defined

    StepMax  * step_max;     ///< Step max process (if defined)

  //------------------------------------------------------------
  // class functions definition
  public:
    // constructor and destructor
    Wisard_PhysList();
    ~Wisard_PhysList();

  protected:
    // construct particle and associated physics process
    void ConstructParticle();
    void ConstructProcess();
    void SetCuts();

  public:
    void AddStepMax ( G4double step = 0., u_short flag = 0x7 );
    void SetStepMax ( G4double step = 1.*um );

};
