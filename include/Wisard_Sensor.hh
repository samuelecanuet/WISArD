#ifndef WITCH_SENSOR_HH
#define WITCH_SENSOR_HH

#include "../Wisard_Global.hh"
#include "G4VSensitiveDetector.hh"

//----------------------------------------------------------------------
// This class is associated with a volume of the geometry to make this
// volume "sensitive"
// When a particle track produces a "hit" in the corresponding volume,
// the ProcessHits function is called
class Wisard_Sensor: public G4VSensitiveDetector
{

  //------------------------------------------------------------
  // internal variables definition
  protected:

    // energy collected in the germanium
    // it is reset after storing the data
    double    energy;
    double    energy_positron;
    G4ThreeVector position;
    int count;

  //------------------------------------------------------------
  // class functions definition
  public:
    // constructor and destructor
    Wisard_Sensor ( );
    ~Wisard_Sensor();

    // current event energy
    double  GetEventEnergy   ( );   // inline
    void    ResetEventEnergy ( );   // inline

    double  GetEventEnergy_positron   ( );   // inline
    void    ResetEventEnergy_positron ( );   // inline

    G4ThreeVector GetPositionEnter ();
    void ResetPositionEnter ();

    // hits processing
		void    Initialize  ( G4HCofThisEvent * );
		G4bool  ProcessHits ( G4Step *, G4TouchableHistory * );

};

//----------------------------------------------------------------------
// inline functions for the class

// Returns the current event energy (in the germanium)
// This function is called for event analysis by run manager
inline double Wisard_Sensor::GetEventEnergy ( ) {return ( energy ); }
inline double Wisard_Sensor::GetEventEnergy_positron ( ) { return ( energy_positron ); }
inline G4ThreeVector Wisard_Sensor::GetPositionEnter ( ) { return ( position ); }

// Reset the current event energy (in the germanium)
// This function is called at end of event analysis by run manager
inline void Wisard_Sensor::ResetEventEnergy ( ) { energy = 0.L; }
inline void Wisard_Sensor::ResetEventEnergy_positron ( ) { energy_positron = 0.L; }
inline void Wisard_Sensor::ResetPositionEnter ( ) { position = G4ThreeVector(-1.,-1.,-1.) ; }



#endif
