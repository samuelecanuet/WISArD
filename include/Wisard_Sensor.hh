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
  
    // it is reset after storing the data
    double    energy;
    double    energy_positron;
    G4ThreeVector position;
    G4ThreeVector position_positron;
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

    G4ThreeVector GetPositionEnter_positron ();
    void ResetPositionEnter_positron ();

    // hits processing
		void    Initialize  ( G4HCofThisEvent * );
		G4bool  ProcessHits ( G4Step *, G4TouchableHistory * );

};

//----------------------------------------------------------------------
// inline functions for the class

// Returns
// This function is called for event analysis by run manager
inline double Wisard_Sensor::GetEventEnergy ( ) {return ( energy ); }
inline double Wisard_Sensor::GetEventEnergy_positron ( ) { return ( energy_positron ); }
inline G4ThreeVector Wisard_Sensor::GetPositionEnter ( ) { return ( position ); }
inline G4ThreeVector Wisard_Sensor::GetPositionEnter_positron ( ) { return ( position_positron ); }

// Reset
// This function is called at end of event analysis by run manager
inline void Wisard_Sensor::ResetEventEnergy ( ) { energy = 0.; }
inline void Wisard_Sensor::ResetEventEnergy_positron ( ) { energy_positron = 0.; }
inline void Wisard_Sensor::ResetPositionEnter ( ) { position = G4ThreeVector(DBL_MAX,DBL_MAX,DBL_MAX) ; }
inline void Wisard_Sensor::ResetPositionEnter_positron ( ) { position_positron = G4ThreeVector(DBL_MAX,DBL_MAX,DBL_MAX) ; }




#endif
