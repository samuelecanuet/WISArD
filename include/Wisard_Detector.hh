#ifndef GE_DETECTOR_HH
#define GE_DETECTOR_HH

#include "../Wisard_Global.hh"
#include "G4VUserDetectorConstruction.hh"
#include "Wisard_RunManager.hh"
#include "G4ThreeVector.hh"
#include "G4UnionSolid.hh"
#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"
#include "G4SubtractionSolid.hh"
#include "G4ExtrudedSolid.hh"
#include "G4MultiUnion.hh"

//----------------------------------------------------------------------

class Wisard_Detector: public G4VUserDetectorConstruction
{

  //------------------------------------------------------------
  // internal variables definition
  protected:

    // pointer to the run manager
    Wisard_RunManager * manager_ptr;

    // WITCH
    //double radius_wisard;
    // bore paramters
    //double radius_bore, length_bore, radius_wall;
    // detectors
    double pos_detp1, pos_detp2, pos_dete;
    double radius1_detp, radius2_detp, radius1_dete, radius2_dete;
    double thickness_detp, thickness_dete;
    // catcher
    double pos_catcher, thickness_catcher, radius_catcher;


  //------------------------------------------------------------
  // class functions definition
  public:
    // constructor with arguments
    Wisard_Detector( Wisard_RunManager * mgr );

    // destructor
    ~Wisard_Detector();

		// setup construction (mandatory class function)
    G4VPhysicalVolume * Construct  ( );

  private:

    G4Tubs* fSolidWorld;
    G4LogicalVolume* fLogicWorld;
    G4VPhysicalVolume* fPhysiWorld;
    G4Material* fDefaultMaterial;

    G4double fLength_PlasticScintillator = 5*cm;
    G4double fRadius_PlasticScintillator = 1.5*cm;

    G4bool event;
    void               SetMagField();

    // // definition of the material used in the setup
    // void DefineMaterial ( );
};

#endif
