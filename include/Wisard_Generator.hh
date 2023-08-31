#ifndef WITCH_GENERATOR_HH
#define WITCH_GENERATOR_HH

#include "../Wisard_Global.hh"
#include "G4VUserPrimaryGeneratorAction.hh"
#include "Wisard_RunManager.hh"
//----------------------------------------------------------------------

class Wisard_Generator: public G4VUserPrimaryGeneratorAction//, public Wisard_RunManager
{

  //------------------------------------------------------------
  // internal variables definition
  protected:

          ////--------------------------------------------------
          ////  JG 2013/07/08   Added pointer to RunManager
          ////                  to access input file
    Wisard_RunManager * manager_ptr;
          ////--------------------------------------------------
          ////  JG 2013/07/08   Memorize particle definitions
    G4ParticleDefinition  * part_electron;
    G4ParticleDefinition  * part_positron;
    G4ParticleDefinition  * part_gamma;
    G4ParticleDefinition  * part_proton;
    G4ParticleDefinition  * part_enubar;
    G4ParticleDefinition  * part_alpha;
    G4ParticleDefinition  * part_geantino;

    double implantation;
    double std_implantation;


  //------------------------------------------------------------
  // class functions definition
  public:
          ////--------------------------------------------------
          ////  JG 2013/07/08   Added pointer to RunManager
          ////                  in constructor
    // constructor and destructor
    Wisard_Generator ( Wisard_RunManager * mgr, double implantation_, double std_implantation_);
    ~Wisard_Generator();

    // function generating the event primary vertex with emitted particles
    void GeneratePrimaries ( G4Event * event );

};
#endif
