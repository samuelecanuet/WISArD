#include "Wisard_Detector.hh"
#include "Wisard_Sensor.hh"
#include "Wisard_MagnetField.hh"

// declaration of the classes used in the functions
#include "G4VisAttributes.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4Torus.hh"
#include "G4TransportationManager.hh"
#include "G4EqMagElectricField.hh"
#include "G4ChordFinder.hh"
#include "G4FieldManager.hh"
#include "G4NystromRK4.hh"
#include "G4Mag_UsualEqRhs.hh"
#include "G4Trap.hh"


//----------------------------------------------------------------------
// constructor of the detector definition
// the pointer to the run manager is specified so that we can
// go back to the manager data (ie energy histogram)
Wisard_Detector::Wisard_Detector ( Wisard_RunManager * mgr)
{
  cout << "Constructor Wisard_Detectors" << endl;
  manager_ptr = mgr;
}

// destructor
Wisard_Detector::~Wisard_Detector()
{
  cout << "Destructor Wisard_Detectors" << endl;
}

//----------------------------------------------------------------------

// Function that builds all the simulation geometry
G4VPhysicalVolume * Wisard_Detector::Construct  ( )
{


  static G4bool fieldIsInitialized = false;

  if ( !fieldIsInitialized )
  {
    G4FieldManager   *pFieldMgr;
    // G4MagIntegratorStepper *pStepper;

    //Field grid in A9.TABLE. File must be in accessible from run directory.
    G4MagneticField* WisardMagField = new WisardMagnetField("wisard_field.txt");
    ////G4MagneticField* WisardMagField= new WisardMagnetField("/exo73c/Blank/WITCH/simulations/geant4/wisard_field.txt");

    pFieldMgr = G4TransportationManager::GetTransportationManager()->GetFieldManager();

    //G4Mag_UsualEqRhs* fEquation = new G4Mag_UsualEqRhs(WisardMagField);
    //G4NystromRK4* fStepper = new G4NystromRK4(fEquation);

    G4ChordFinder *pChordFinder = new G4ChordFinder(WisardMagField);
    pChordFinder->SetDeltaChord(1*um);
    pFieldMgr->SetChordFinder( pChordFinder );

    pFieldMgr->SetDetectorField(WisardMagField);

    // JG: setting precision
    pFieldMgr->GetChordFinder( )->SetDeltaChord ( 0.1*um );

    fieldIsInitialized = true;
}



//#endif

G4GeometryManager::GetInstance()->OpenGeometry();
G4PhysicalVolumeStore::GetInstance()->Clean();
G4LogicalVolumeStore::GetInstance()->Clean();
G4SolidStore::GetInstance()->Clean();

G4double Angle = NULL;
G4double off = NULL;
std::ifstream config;
  config.open("src/WISArD.cfg");
  if ( !config.is_open() )
    G4cout << "Error opening file " << "WISArD.cfg" << G4endl;
  else{
    while( config.is_open() ){
      G4String variable;
      G4String unit;
      G4double value;

      config >> variable;
      if(!config.good()) break;
      //####################### COMMON variables ###########################
      if(variable == "Angle"){
  	config >> value >> unit;
  	Angle = value*G4UnitDefinition::GetValueOf(unit);
      }
      else if(variable == "Off"){
  	config >> value >> unit;
  	off = value*G4UnitDefinition::GetValueOf(unit);
      }
    }
  }
  config.close();

  cout<<"ANGLE = "<<Angle<<"\t"<<"OFFSET = "<<off<<endl;

//--------------------------------------------------------------------------------------
//------------------------------------ World -------------------------------------------
//--------------------------------------------------------------------------------------
G4double innerRadius = 0*cm;
G4double outerRadius = 6.5*cm; // réduit au raypon du Bore pour opti6.5
G4double length      = 30.*cm; // réduit pour opti
G4double theta       = 90.0*deg;
G4double phi         = 360.0*deg;

G4Material* vide  = G4NistManager::Instance()->FindOrBuildMaterial("G4_Galactic");
fSolidWorld = new G4Tubs("World", innerRadius, outerRadius, length/2, theta, phi);

fLogicWorld = new G4LogicalVolume(fSolidWorld, //its solid
                                  vide,        //its material
                                  "World");                //its name

fPhysiWorld = new G4PVPlacement(0,                        //no rotation
                                G4ThreeVector(),        //at (0,0,0)
                                fLogicWorld, //its fLogical volume
                                "World",                //its name
                                0,                        //its mother  volume
                                false, //no boolean operation
                                0); //copy number

 G4VisAttributes* visAtt_World = new G4VisAttributes(G4Colour(1,1,1)); //white
 visAtt_World->SetVisibility(false);
 fLogicWorld->SetVisAttributes(visAtt_World);




//==================================================================================================
   //========================================  WISArD MAGNET =========================================
//==================================================================================================
   G4Material*   materialIron  = G4NistManager::Instance()->FindOrBuildMaterial("G4_Fe");
   G4double radius_wisard = 70.0*cm;
   G4double radius_bore   = 6.5*cm;
   G4double length_bore   = 80*cm;
   G4double radius_wall   = 0.3*cm;

   G4Tubs * wisard = new G4Tubs ( "Wisard_magnet",
                                  radius_bore + radius_wall,
                                  radius_wisard,
                                  0.5L*(length_bore),
                                  0.L, 360.L*deg );
   G4LogicalVolume * wisard_logic  = new G4LogicalVolume
                                     ( wisard,      // forme
                                       materialIron,      // matiere
                                       "WisardLogic",   // nom
                                       0, 0, 0 );
   // - physical volume
   G4VPhysicalVolume * wisard_phys = new G4PVPlacement ( 0,
                                      G4ThreeVector(0.,0.,0.),
                                      wisard_logic,   // ptr sur vol. log.
                                      "Wisard",       // nom du volume
                                      fLogicWorld, // volume parent
                                      false, true );

   // - make the bore volume "visible"
   G4VisAttributes* visAtt_WITCHMagnet = new G4VisAttributes(G4Colour(0.5,0.7,0.7,0.2));
   visAtt_WITCHMagnet->SetVisibility(true);
   visAtt_WITCHMagnet->SetForceWireframe(false);
   visAtt_WITCHMagnet->SetForceSolid(true);
   wisard_logic->SetVisAttributes (visAtt_WITCHMagnet);

   if(wisard_phys == NULL) {}

//==================================================================================================
   //=======================================  BORE - MAGNET ==========================================
//==================================================================================================
   G4Material*   materialAluminum  = G4NistManager::Instance()->FindOrBuildMaterial("G4_Al");

   G4Tubs * bore = new G4Tubs ( "Bore_Tube",
                                  radius_bore,
                                  radius_bore+radius_wall,
                                  0.5L*(length_bore),
                                  0.L, 360.L*deg );
   G4LogicalVolume * bore_logic  = new G4LogicalVolume
                                     ( bore,      // forme
                                       materialAluminum,      // matiere
                                       "BoreLogic",   // nom
                                       0, 0, 0 );
   // - physical volume
   G4VPhysicalVolume * bore_phys = new G4PVPlacement ( 0,
                                      G4ThreeVector(0.,0.,0.),
                                      bore_logic,   // ptr sur vol. log.
                                      "Bore",       // nom du volume
                                      fLogicWorld, // volume parent
                                      false, true );

   // - make the bore volume "visible"
   G4VisAttributes* visAttr_Bore = new G4VisAttributes(G4Colour(0.4,0.4,0.4,0.2));
   visAttr_Bore->SetVisibility(true);
   visAttr_Bore->SetForceWireframe(false);
   visAttr_Bore->SetForceSolid(true);
   bore_logic->SetVisAttributes (visAttr_Bore);

   if(bore_phys == NULL) {}


//======================================================================================================
//====================================== CYLINDER ======================================================
//======================================================================================================
   innerRadius = 71.*cm;
   outerRadius = 72.*cm;
   length      = 100.*cm;
   theta       = 90.0*deg;
   phi         = 360.0*deg;

   G4Material* fMaterial_Al = G4NistManager::Instance()->FindOrBuildMaterial("G4_Al");
   G4Tubs *fSolidMYCILINDER = new G4Tubs("MYCILINDER", innerRadius, outerRadius, length/2, theta, phi);

   G4LogicalVolume* fLogicMYCILINDER = new G4LogicalVolume(fSolidMYCILINDER,                //its solid
                                     fMaterial_Al,        //its material
"MYCILINDER");                //its name

   G4VPhysicalVolume* fPhysiMYCILINDER = new G4PVPlacement(0,                        //no rotation
                                   G4ThreeVector(0,0,0), //at (0,0,0)
fLogicMYCILINDER,                //its fLogical volume
                                   "MYCILINDER", //its name
fLogicWorld,                        //its mother  volume
                                   false, //no boolean operation
                                   0,
false);                        //copy number

    G4VisAttributes* visAtt_MYCILINDER  = new G4VisAttributes(G4Colour(1,1,0)); //white
    visAtt_MYCILINDER->SetVisibility(true);
    fLogicMYCILINDER->SetVisAttributes(visAtt_MYCILINDER );

   if(fPhysiMYCILINDER == NULL) {}

   //BASE 1
   innerRadius = 0*mm;
   outerRadius = 72*cm;
   G4double lengthB      =  1.*cm;
   theta       = 90.0*deg;
   phi         = 360.0*deg;

   G4Tubs *fSolidMYCILINDER_B1 = new G4Tubs("MYCILINDER_B1", innerRadius, outerRadius, lengthB/2, theta, phi);

   G4LogicalVolume* fLogicMYCILINDER_B1 = new G4LogicalVolume(fSolidMYCILINDER_B1,                //its solid
                                     fMaterial_Al,        //its material
"MYCILINDER_B1");                //its name

   G4VPhysicalVolume* fPhysiMYCILINDER_B1 = new G4PVPlacement(0,                        //no rotation
G4ThreeVector(0,0,-(length/2)-(lengthB/2)),        //at (0,0,0)
fLogicMYCILINDER_B1,                //its fLogical volume
"MYCILINDER_B1",                //its name
fLogicMYCILINDER,                        //its mother  volume
                                   false, //no boolean operation
                                   0,
false);                        //copy number

    G4VisAttributes* visAtt_MYCILINDER_B1  = new G4VisAttributes(G4Colour(1,1,0)); //white
    visAtt_MYCILINDER_B1 ->SetVisibility(true);
    fLogicMYCILINDER_B1 ->SetVisAttributes(visAtt_MYCILINDER_B1);

   if(fPhysiMYCILINDER_B1 == NULL) {}

    //BASE 2
   G4Tubs *fSolidMYCILINDER_B2 = new G4Tubs("MYCILINDER_B2", innerRadius, outerRadius, lengthB/2, theta, phi);

   G4LogicalVolume* fLogicMYCILINDER_B2 = new G4LogicalVolume(fSolidMYCILINDER_B2,                //its solid
                                     fMaterial_Al,        //its material
"MYCILINDER_B2");                //its name

   G4VPhysicalVolume* fPhysiMYCILINDER_B2 = new G4PVPlacement(0,                        //no rotation
G4ThreeVector(0,0,(length/2)+(lengthB/2)),        //at (0,0,0)
fLogicMYCILINDER_B2,                //its fLogical volume
"MYCILINDER_B2",                //its name
fLogicMYCILINDER,                        //its mother  volume
                                   false, //no boolean operation
                                   0,
false);                        //copy number

    G4VisAttributes* visAtt_MYCILINDER_B2  = new G4VisAttributes(G4Colour(1,1,0)); //white
    visAtt_MYCILINDER_B2 ->SetVisibility(true);
    fLogicMYCILINDER_B2 ->SetVisAttributes(visAtt_MYCILINDER_B2 );

    if(fPhysiMYCILINDER_B2 == NULL) {}


      // ===========================================================================================================
      // ================================ CATCHER/SORGENTE RAD./SUPPORTO  ==========================================
      // ===========================================================================================================
      // Guardiamo il supporto del catcher dalla parte attaccata al perno verso la sorgente.
      // La parte 1 è il supporto rettangolare lungo, la parte 2 è il il supporto rettangolare largo su cui si trovano i buchi per il catcher e il posizionamento sorgente radioattiva.
      // Per maggiori dettagli vedere mail M. Roche 10/06/2021 alle ore 11-50 circa (e foto su cui ho scritto ed evidenziato salvata su telegram).
       G4double x_baseCatcher_Spessa_1          = 14.*mm;
       G4double y_baseCatcher_Spessa_1          = 16.*mm;
       G4double thickness_baseCatcher_Spessa_1  = 12.*mm;
       G4double radius_buco_baseSpessa          = 5.*mm;
       G4double pos_buco_baseSpessa             = 0.8963*mm;
       G4double x_baseCatcher_Sottile_1         = x_baseCatcher_Spessa_1;
       G4double y_baseCatcher_Sottile_1         = 24.*mm;
       G4double thickness_baseCatcher_Sottile_1 = 4.*mm;
       G4double x_baseCatcher_2                 = 60.*mm;
       G4double y_baseCatcher_2                 = 32.*mm;
       G4double z_baseCatcher_2                 = thickness_baseCatcher_Sottile_1; //NB: tutte le misure precedenti sono state prese direttamente dai disegni tecnici di M. Roche
       G4double radius_ext_bucoCatcher          = (25./2)*mm;
       G4double radius_bucoCatcher              = (15./2)*mm;
       G4double thickness_supp_catcher          = 2.*mm;
       G4double radius_bucoSource               = (20./2)*mm;
       //G4double thickness_supp_radSource        = 3.*mm;
       //G4double altezza_supp_radSource          = 0.5*mm; //Da Verificare
       G4double x_pos_bucoCatcher               = (25.88/2)*mm;
      // G4double x_pos_bucoSource                = x_pos_bucoCatcher;
       G4double x_y_triangularCut_baseSpessa    = 5.*mm;
       G4double x_y_triangularCut_baseSottile   = 8.*mm;
       G4Material* materialSupport_Catcher_source  = G4NistManager::Instance()->FindOrBuildMaterial("G4_Al");

       G4RotationMatrix* myRotation_trSupp = new G4RotationMatrix();
       myRotation_trSupp->rotateX(0.*deg);
       myRotation_trSupp->rotateY(180.*deg);
       myRotation_trSupp->rotateZ(0.*rad);

       G4RotationMatrix* myRotation_trSupp2 = new G4RotationMatrix();
       myRotation_trSupp2->rotateX(180.*deg);
       myRotation_trSupp2->rotateY(180.*deg);
       myRotation_trSupp2->rotateZ(0.*rad);

       G4RotationMatrix* myRotation_trSupp3 = new G4RotationMatrix();
       myRotation_trSupp3->rotateX(180.*deg);
       myRotation_trSupp3->rotateY(0.*deg);
       myRotation_trSupp3->rotateZ(0.*rad);

       G4Box* SuppCatcher_baseSpessa_senzaBuco  = new G4Box("SuppCatcher_baseSpessa_senzaBuco", x_baseCatcher_Spessa_1/2, y_baseCatcher_Spessa_1/2, thickness_baseCatcher_Spessa_1/2);
       G4Tubs* bucoBaseSpessa                   = new G4Tubs("bucoBaseSpessa", 0., radius_buco_baseSpessa, 2*thickness_baseCatcher_Spessa_1, 0., 360*deg);
       G4VSolid* SuppCatcher_baseSpessa_intera  = new G4SubtractionSolid("SuppCatcher_baseSpessa_intera", SuppCatcher_baseSpessa_senzaBuco, bucoBaseSpessa, 0, G4ThreeVector(0.,-0.2962*mm,0.));

       const G4int nsect = 3;
       std::vector<G4TwoVector> polygon(nsect);
       polygon[0].set(x_y_triangularCut_baseSpessa, 0.);
       polygon[1].set(0., 0.);
       polygon[2].set(x_y_triangularCut_baseSpessa, x_y_triangularCut_baseSpessa);

       G4TwoVector offsetA(0,0), offsetB(0,0);
       G4double scaleA = 1, scaleB = 1;
       G4VSolid* triangularCut_baseSpessa = new G4ExtrudedSolid("triangularCut_baseSpessa", polygon, thickness_baseCatcher_Spessa_1, offsetA, scaleA, offsetB, scaleB);

       G4VSolid* SuppCatcher_baseSpessa_parziale  = new G4SubtractionSolid("SuppCatcher_baseSpessa_parziale", SuppCatcher_baseSpessa_intera, triangularCut_baseSpessa, 0, G4ThreeVector(x_baseCatcher_Spessa_1/2 - x_y_triangularCut_baseSpessa+0.5*mm, -y_baseCatcher_Spessa_1/2, 0.));
       G4VSolid* SuppCatcher_baseSpessa           = new G4SubtractionSolid("SuppCatcher_baseSpessa", SuppCatcher_baseSpessa_parziale, triangularCut_baseSpessa, myRotation_trSupp, G4ThreeVector(-x_baseCatcher_Spessa_1/2 + x_y_triangularCut_baseSpessa-0.5*mm, -y_baseCatcher_Spessa_1/2 -0.5*mm, 0.));

       G4ThreeVector pos_SuppCatcher_baseSpessa = G4ThreeVector(0., pos_buco_baseSpessa, 0.);
       G4Transform3D tr_SuppCatcher_baseSpessa  = G4Transform3D(G4RotationMatrix(), pos_SuppCatcher_baseSpessa);

       G4Box* SuppCatcher_baseSottile            = new G4Box("SuppCatcher_baseSottile", x_baseCatcher_Sottile_1/2, y_baseCatcher_Sottile_1/2, thickness_baseCatcher_Sottile_1/2);
       G4ThreeVector pos_SuppCatcher_baseSottile = G4ThreeVector(0., y_baseCatcher_Spessa_1/2 + y_baseCatcher_Sottile_1/2, thickness_baseCatcher_Spessa_1/2 - thickness_baseCatcher_Sottile_1/2);
       G4Transform3D tr_SuppCatcher_baseSottile  = G4Transform3D(G4RotationMatrix(), pos_SuppCatcher_baseSottile);

       G4Box* SuppCatcher_base_2_senzaBuco         = new G4Box("SuppCatcher_base_2_senzaBuco", x_baseCatcher_2/2, y_baseCatcher_2/2, z_baseCatcher_2/2);
       G4Tubs* bucoCatcher                         = new G4Tubs("bucoCatcher", 0., radius_bucoCatcher, 2*z_baseCatcher_2, 0., 360*deg);
       G4VSolid* SuppCatcher_base_2_conBucoCatcher = new G4SubtractionSolid("SuppCatcher_baseSpessa_conbucoCatcher", SuppCatcher_base_2_senzaBuco, bucoCatcher, 0, G4ThreeVector(-x_pos_bucoCatcher, 0., 0.));
       G4Tubs* bucoSource                          = new G4Tubs("bucoSource", 0., radius_bucoSource, 2*z_baseCatcher_2, 0., 360*deg);
       G4VSolid* SuppCatcher_base_2_intero         = new G4SubtractionSolid("SuppCatcher_base_2_intero", SuppCatcher_base_2_conBucoCatcher, bucoSource, 0, G4ThreeVector(x_pos_bucoCatcher, 0., 0.));

       std::vector<G4TwoVector> polygon2(nsect);
       polygon2[0].set(x_y_triangularCut_baseSottile, 0.);
       polygon2[1].set(0., 0.);
       polygon2[2].set(x_y_triangularCut_baseSottile, x_y_triangularCut_baseSottile);

       G4VSolid* triangularCut_baseSottile     = new G4ExtrudedSolid("triangularCut_baseSottile", polygon2, thickness_baseCatcher_Spessa_1, offsetA, scaleA, offsetB, scaleB);
       G4VSolid* SuppCatcher_base_2_unAngolo   = new G4SubtractionSolid("SuppCatcher_base_2_unAngolo", SuppCatcher_base_2_intero, triangularCut_baseSottile, 0, G4ThreeVector(+x_baseCatcher_2/2 - x_y_triangularCut_baseSottile +0.5*mm, -y_baseCatcher_2/2 -0.5*mm, 0.));
       G4VSolid* SuppCatcher_base_2_dueAngoli  = new G4SubtractionSolid("SuppCatcher_base_2_dueAngoli", SuppCatcher_base_2_unAngolo, triangularCut_baseSottile, myRotation_trSupp, G4ThreeVector(-x_baseCatcher_2/2 + x_y_triangularCut_baseSottile -0.5*mm, -y_baseCatcher_2/2 -0.5*mm, 0.));
       G4VSolid* SuppCatcher_base_2_treAngoli  = new G4SubtractionSolid("SuppCatcher_base_2_treAngoli", SuppCatcher_base_2_dueAngoli, triangularCut_baseSottile, myRotation_trSupp2, G4ThreeVector(-x_baseCatcher_2/2 + x_y_triangularCut_baseSottile -0.5*mm, +y_baseCatcher_2/2 +0.5*mm, 0.));
       G4VSolid* SuppCatcher_base_2            = new G4SubtractionSolid("SuppCatcher_base_2", SuppCatcher_base_2_treAngoli, triangularCut_baseSottile, myRotation_trSupp3, G4ThreeVector(x_baseCatcher_2/2 - x_y_triangularCut_baseSottile +0.5*mm, +y_baseCatcher_2/2 +0.5*mm, 0.));

       G4ThreeVector pos_SuppCatcher_base_2 = G4ThreeVector(0., y_baseCatcher_Spessa_1/2 + y_baseCatcher_Sottile_1 + y_baseCatcher_2/2, thickness_baseCatcher_Spessa_1/2 - z_baseCatcher_2/2);
       G4Transform3D tr_SuppCatcher_base_2  = G4Transform3D(G4RotationMatrix(), pos_SuppCatcher_base_2);

       G4MultiUnion* munion_supportCatcher_Source = new G4MultiUnion("munion_supportCatcher_Source");
munion_supportCatcher_Source->AddNode(*SuppCatcher_baseSpessa, tr_SuppCatcher_baseSpessa);
munion_supportCatcher_Source->AddNode(*SuppCatcher_baseSottile, tr_SuppCatcher_baseSottile);
munion_supportCatcher_Source->AddNode(*SuppCatcher_base_2     , tr_SuppCatcher_base_2);
       munion_supportCatcher_Source->Voxelize();

       G4LogicalVolume* logic_munion_supportCatcher_Source  = new G4LogicalVolume(munion_supportCatcher_Source, materialSupport_Catcher_source, "logic_munion_supportCatcher_Source");
       G4VPhysicalVolume* phys_munion_supportCatcher_Source = new G4PVPlacement(0,                        //no rotation
G4ThreeVector(x_pos_bucoCatcher, - (y_baseCatcher_Spessa_1/2 + y_baseCatcher_Sottile_1 + y_baseCatcher_2/2), -(thickness_baseCatcher_Spessa_1/2 + 0/2 + 0/2)),     //+thickness_supp_catcher/2   //at (0,0,0) //thickness_baseCatcher_Sottile_1
logic_munion_supportCatcher_Source,                //its fLogical volume
"phys_munion_supportCatcher_Source",                //its name
fLogicWorld,                        //its mother  volume
false,                        //no boolean operation
0,
false);                        //copy number

       G4VisAttributes* visAtt_munion_supportCatcher_Source  = new G4VisAttributes(G4Colour(0.8,0.8,0.8));
visAtt_munion_supportCatcher_Source->SetVisibility(true);
visAtt_munion_supportCatcher_Source->SetForceWireframe(false);
visAtt_munion_supportCatcher_Source->SetForceSolid(true);
       logic_munion_supportCatcher_Source ->SetVisAttributes(visAtt_munion_supportCatcher_Source);

       if(phys_munion_supportCatcher_Source == NULL) {}

       //G4double x_pos_Catcher_risp_munion   = x_pos_bucoCatcher;
       G4double y_pos_Catcher_risp_munion   = y_baseCatcher_Spessa_1/2 + y_baseCatcher_Sottile_1 + y_baseCatcher_2/2;

       G4Tubs* Supp_catcher                 = new G4Tubs("Supp_catcher", radius_bucoCatcher, radius_ext_bucoCatcher, thickness_supp_catcher/2, 0., 360*deg);
       G4LogicalVolume* logic_Supp_catcher  = new G4LogicalVolume(Supp_catcher, materialSupport_Catcher_source, "logic_Supp_catcher");
       G4VPhysicalVolume* phys_Supp_catcher = new G4PVPlacement(0,                        //no rotation
G4ThreeVector(-x_pos_bucoCatcher, y_pos_Catcher_risp_munion, thickness_baseCatcher_Spessa_1/2),        //at (0,0,0)
logic_Supp_catcher,                //its fLogical volume
"phys_Supp_catcher",                //its name
logic_munion_supportCatcher_Source,                        //its mother  volume
false,                        //no boolean operation
0,
false);                        //copy number

      G4VisAttributes* visAtt_Supp_catcher  = new G4VisAttributes(G4Colour(0.3,0.3,0.3));
      visAtt_Supp_catcher->SetVisibility(true);
      visAtt_Supp_catcher->SetForceWireframe(false);
      visAtt_Supp_catcher->SetForceSolid(true);
      logic_Supp_catcher ->SetVisAttributes(visAtt_Supp_catcher );

      if(phys_Supp_catcher == NULL) {}

      G4Material* Mylar = G4NistManager::Instance()->FindOrBuildMaterial("G4_MYLAR");

      //G4double thicknessMylarSource    = 500.*nm;
      G4double thicknessMylarSource    = 6.*um;
      G4double outerRadius_MylarSource = radius_bucoCatcher;

      G4Tubs*              MylarSource          = new G4Tubs("MylarSource", 0., outerRadius_MylarSource, thicknessMylarSource/2, 0., 360.*deg);
      G4LogicalVolume*     Logic_MylarSource    = new G4LogicalVolume(MylarSource, Mylar, "LogicMylarSource"); //solid, material, name
      G4VPhysicalVolume*   Physics_MylarSource  = new G4PVPlacement(0,                //no rotation
G4ThreeVector(0., 0., 0.),    //position
Logic_MylarSource,"LogicMylarSource",        //its fLogical volume
fLogicWorld,            //its mother volume
false,                //no boolean op.
0,
true);                //copy nb.

      G4VisAttributes* MylarSource_att = new G4VisAttributes(G4Colour(0.94, 0.5, 0.5)); //pink
      MylarSource_att->SetVisibility(true);
      MylarSource_att->SetForceWireframe(false);
      MylarSource_att->SetForceSolid(true);
      Logic_MylarSource->SetVisAttributes(MylarSource_att);

     if(Physics_MylarSource == NULL) {}

/////////////////////////NEW SUPPORT CATCHER//////////////////
// G4double x_baseCatcher_Spessa_1          = 14.*mm;
// G4double y_baseCatcher_Spessa_1          = 16.*mm;
// G4double thickness_baseCatcher_Spessa_1  = 12.*mm;
// G4double radius_buco_baseSpessa          = 5.*mm;
// G4double pos_buco_baseSpessa             = 0.8963*mm;
// G4double x_baseCatcher_Sottile_1         = x_baseCatcher_Spessa_1;
// G4double y_baseCatcher_Sottile_1         = 24.*mm;
// G4double thickness_baseCatcher_Sottile_1 = 2.*mm;
// G4double x_baseCatcher_2                 = 60.*mm;
// G4double y_baseCatcher_2                 = 32.*mm;
// G4double z_baseCatcher_2                 = thickness_baseCatcher_Sottile_1; //NB: tutte le misure precedenti sono state prese direttamente dai disegni tecnici di M. Roche
// G4double radius_int_bucoCatcher          = 7.5*mm;  /// a revoir
// G4double radius_bucoCatcher              = 9*mm;
// G4double thickness_supp_catcher          = 1.5*mm;
// G4double radius_bucoSource               = (20./2)*mm;
// //G4double thickness_supp_radSource        = 3.*mm;
// //G4double altezza_supp_radSource          = 0.5*mm; //Da Verificare
// G4double x_pos_bucoCatcher               = (25.88/2)*mm;
// // G4double x_pos_bucoSource                = x_pos_bucoCatcher;
// G4double x_y_triangularCut_baseSpessa    = 5.*mm;
// G4double x_y_triangularCut_baseSottile   = 8.*mm;
// G4Material* materialSupport_Catcher_source  = G4NistManager::Instance()->FindOrBuildMaterial("G4_Al");
//
// G4RotationMatrix* myRotation_trSupp = new G4RotationMatrix();
// myRotation_trSupp->rotateX(0.*deg);
// myRotation_trSupp->rotateY(180.*deg);
// myRotation_trSupp->rotateZ(0.*rad);
//
// G4RotationMatrix* myRotation_trSupp2 = new G4RotationMatrix();
// myRotation_trSupp2->rotateX(180.*deg);
// myRotation_trSupp2->rotateY(180.*deg);
// myRotation_trSupp2->rotateZ(0.*rad);
//
// G4RotationMatrix* myRotation_trSupp3 = new G4RotationMatrix();
// myRotation_trSupp3->rotateX(180.*deg);
// myRotation_trSupp3->rotateY(0.*deg);
// myRotation_trSupp3->rotateZ(0.*rad);
//
// G4Box* SuppCatcher_baseSpessa_senzaBuco  = new G4Box("SuppCatcher_baseSpessa_senzaBuco", x_baseCatcher_Spessa_1/2, y_baseCatcher_Spessa_1/2, thickness_baseCatcher_Spessa_1/2);
// G4Tubs* bucoBaseSpessa                   = new G4Tubs("bucoBaseSpessa", 0., radius_buco_baseSpessa, 2*thickness_baseCatcher_Spessa_1, 0., 360*deg);
// G4VSolid* SuppCatcher_baseSpessa_intera  = new G4SubtractionSolid("SuppCatcher_baseSpessa_intera", SuppCatcher_baseSpessa_senzaBuco, bucoBaseSpessa, 0, G4ThreeVector(0.,-0.2962*mm,0.));
//
// const G4int nsect = 3;
// std::vector<G4TwoVector> polygon(nsect);
// polygon[0].set(x_y_triangularCut_baseSpessa, 0.);
// polygon[1].set(0., 0.);
// polygon[2].set(x_y_triangularCut_baseSpessa, x_y_triangularCut_baseSpessa);
//
// G4TwoVector offsetA(0,0), offsetB(0,0);
// G4double scaleA = 1, scaleB = 1;
// G4VSolid* triangularCut_baseSpessa = new G4ExtrudedSolid("triangularCut_baseSpessa", polygon, thickness_baseCatcher_Spessa_1, offsetA, scaleA, offsetB, scaleB);
//
// G4VSolid* SuppCatcher_baseSpessa_parziale  = new G4SubtractionSolid("SuppCatcher_baseSpessa_parziale", SuppCatcher_baseSpessa_intera, triangularCut_baseSpessa, 0, G4ThreeVector(x_baseCatcher_Spessa_1/2 - x_y_triangularCut_baseSpessa+0.5*mm, -y_baseCatcher_Spessa_1/2, 0.));
// G4VSolid* SuppCatcher_baseSpessa           = new G4SubtractionSolid("SuppCatcher_baseSpessa", SuppCatcher_baseSpessa_parziale, triangularCut_baseSpessa, myRotation_trSupp, G4ThreeVector(-x_baseCatcher_Spessa_1/2 + x_y_triangularCut_baseSpessa-0.5*mm, -y_baseCatcher_Spessa_1/2 -0.5*mm, 0.));
//
// G4ThreeVector pos_SuppCatcher_baseSpessa = G4ThreeVector(0., pos_buco_baseSpessa, 0.);
// G4Transform3D tr_SuppCatcher_baseSpessa  = G4Transform3D(G4RotationMatrix(), pos_SuppCatcher_baseSpessa);
//
// G4Box* SuppCatcher_baseSottile            = new G4Box("SuppCatcher_baseSottile", x_baseCatcher_Sottile_1/2, y_baseCatcher_Sottile_1/2, thickness_baseCatcher_Sottile_1/2);
// G4ThreeVector pos_SuppCatcher_baseSottile = G4ThreeVector(0., y_baseCatcher_Spessa_1/2 + y_baseCatcher_Sottile_1/2, thickness_baseCatcher_Spessa_1/2 - thickness_baseCatcher_Sottile_1/2);
// G4Transform3D tr_SuppCatcher_baseSottile  = G4Transform3D(G4RotationMatrix(), pos_SuppCatcher_baseSottile);
//
// G4Box* SuppCatcher_base_2_senzaBuco         = new G4Box("SuppCatcher_base_2_senzaBuco", x_baseCatcher_2/2, y_baseCatcher_2/2, z_baseCatcher_2/2);
// G4Tubs* bucoCatcher                         = new G4Tubs("bucoCatcher", 0., radius_bucoCatcher, thickness_supp_catcher/2, 0., 360*deg);
// G4VSolid* SuppCatcher_base_2_conBucoCatcher = new G4SubtractionSolid("SuppCatcher_baseSpessa_conbucoCatcher", SuppCatcher_base_2_senzaBuco, bucoCatcher, 0, G4ThreeVector(-x_pos_bucoCatcher, 0., thickness_baseCatcher_Sottile_1-thickness_supp_catcher-0.25*mm));
//
// G4Tubs* bucoCatcher1                         = new G4Tubs("bucoCatcher1", 0., radius_int_bucoCatcher, thickness_baseCatcher_Sottile_1-thickness_supp_catcher, 0., 360*deg);
// G4VSolid* SuppCatcher_base_2_conBucoCatcher1 = new G4SubtractionSolid("SuppCatcher_baseSpessa_conbucoCatcher1", SuppCatcher_base_2_conBucoCatcher, bucoCatcher1, 0, G4ThreeVector(-x_pos_bucoCatcher, 0., thickness_baseCatcher_Sottile_1/2-thickness_supp_catcher));
//
//
//
// G4Tubs* bucoSource                          = new G4Tubs("bucoSource", 0., radius_bucoSource, 2*z_baseCatcher_2, 0., 360*deg);
// G4VSolid* SuppCatcher_base_2_intero         = new G4SubtractionSolid("SuppCatcher_base_2_intero", SuppCatcher_base_2_conBucoCatcher1, bucoSource, 0, G4ThreeVector(x_pos_bucoCatcher, 0., 0.));
//
// std::vector<G4TwoVector> polygon2(nsect);
// polygon2[0].set(x_y_triangularCut_baseSottile, 0.);
// polygon2[1].set(0., 0.);
// polygon2[2].set(x_y_triangularCut_baseSottile, x_y_triangularCut_baseSottile);
//
// G4VSolid* triangularCut_baseSottile     = new G4ExtrudedSolid("triangularCut_baseSottile", polygon2, thickness_baseCatcher_Spessa_1, offsetA, scaleA, offsetB, scaleB);
// G4VSolid* SuppCatcher_base_2_unAngolo   = new G4SubtractionSolid("SuppCatcher_base_2_unAngolo", SuppCatcher_base_2_intero, triangularCut_baseSottile, 0, G4ThreeVector(+x_baseCatcher_2/2 - x_y_triangularCut_baseSottile +0.5*mm, -y_baseCatcher_2/2 -0.5*mm, 0.));
// G4VSolid* SuppCatcher_base_2_dueAngoli  = new G4SubtractionSolid("SuppCatcher_base_2_dueAngoli", SuppCatcher_base_2_unAngolo, triangularCut_baseSottile, myRotation_trSupp, G4ThreeVector(-x_baseCatcher_2/2 + x_y_triangularCut_baseSottile -0.5*mm, -y_baseCatcher_2/2 -0.5*mm, 0.));
// G4VSolid* SuppCatcher_base_2_treAngoli  = new G4SubtractionSolid("SuppCatcher_base_2_treAngoli", SuppCatcher_base_2_dueAngoli, triangularCut_baseSottile, myRotation_trSupp2, G4ThreeVector(-x_baseCatcher_2/2 + x_y_triangularCut_baseSottile -0.5*mm, +y_baseCatcher_2/2 +0.5*mm, 0.));
// G4VSolid* SuppCatcher_base_2            = new G4SubtractionSolid("SuppCatcher_base_2", SuppCatcher_base_2_treAngoli, triangularCut_baseSottile, myRotation_trSupp3, G4ThreeVector(x_baseCatcher_2/2 - x_y_triangularCut_baseSottile +0.5*mm, +y_baseCatcher_2/2 +0.5*mm, 0.));
//
// G4ThreeVector pos_SuppCatcher_base_2 = G4ThreeVector(0., y_baseCatcher_Spessa_1/2 + y_baseCatcher_Sottile_1 + y_baseCatcher_2/2, thickness_baseCatcher_Spessa_1/2 - z_baseCatcher_2/2);
// G4Transform3D tr_SuppCatcher_base_2  = G4Transform3D(G4RotationMatrix(), pos_SuppCatcher_base_2);
//
// G4MultiUnion* munion_supportCatcher_Source = new G4MultiUnion("munion_supportCatcher_Source");
// munion_supportCatcher_Source->AddNode(*SuppCatcher_baseSpessa, tr_SuppCatcher_baseSpessa);
// munion_supportCatcher_Source->AddNode(*SuppCatcher_baseSottile, tr_SuppCatcher_baseSottile);
// munion_supportCatcher_Source->AddNode(*SuppCatcher_base_2     , tr_SuppCatcher_base_2);
// munion_supportCatcher_Source->Voxelize();
//
// G4LogicalVolume* logic_munion_supportCatcher_Source  = new G4LogicalVolume(munion_supportCatcher_Source, materialSupport_Catcher_source, "logic_munion_supportCatcher_Source");
// G4VPhysicalVolume* phys_munion_supportCatcher_Source = new G4PVPlacement(0,                        //no rotation
// G4ThreeVector(x_pos_bucoCatcher, - (y_baseCatcher_Spessa_1/2 + y_baseCatcher_Sottile_1 + y_baseCatcher_2/2), -thickness_baseCatcher_Spessa_1/2+thickness_baseCatcher_Sottile_1+thickness_baseCatcher_Sottile_1/2-thickness_supp_catcher),     //+thickness_supp_catcher/2   //at (0,0,0) //thickness_baseCatcher_Sottile_1
// logic_munion_supportCatcher_Source,                //its fLogical volume
// "phys_munion_supportCatcher_Source",                //its name
// fLogicWorld,                        //its mother  volume
// false,                        //no boolean operation
// 0,
// false);                        //copy number
//
// G4VisAttributes* visAtt_munion_supportCatcher_Source  = new G4VisAttributes(G4Colour(0.8,0.8,0.8));
// visAtt_munion_supportCatcher_Source->SetVisibility(true);
// visAtt_munion_supportCatcher_Source->SetForceWireframe(false);
// visAtt_munion_supportCatcher_Source->SetForceSolid(true);
// logic_munion_supportCatcher_Source ->SetVisAttributes(visAtt_munion_supportCatcher_Source);
//
// if(phys_munion_supportCatcher_Source == NULL) {}
//
// //G4double x_pos_Catcher_risp_munion   = x_pos_bucoCatcher;
// G4double y_pos_Catcher_risp_munion   = y_baseCatcher_Spessa_1/2 + y_baseCatcher_Sottile_1 + y_baseCatcher_2/2;
//
// G4NistManager* man = G4NistManager::Instance();
// G4Material* material_Catcher_Ring  = new G4Material("PEEK", 1.32*g/cm3, 3); //Plastic
// material_Catcher_Ring->AddElement(man->FindOrBuildElement("C"), 19);
// material_Catcher_Ring->AddElement(man->FindOrBuildElement("H"), 12);
// material_Catcher_Ring->AddElement(man->FindOrBuildElement("O"), 3);
//
// G4Tubs* Supp_catcher                 = new G4Tubs("Supp_catcher", radius_int_bucoCatcher, radius_bucoCatcher, thickness_supp_catcher/2, 0., 360*deg);
// G4LogicalVolume* logic_Supp_catcher  = new G4LogicalVolume(Supp_catcher, material_Catcher_Ring, "logic_Supp_catcher");
// G4VPhysicalVolume* phys_Supp_catcher = new G4PVPlacement(0,                        //no rotation
// G4ThreeVector(-x_pos_bucoCatcher, y_pos_Catcher_risp_munion, thickness_baseCatcher_Spessa_1/2-thickness_supp_catcher/2),        //at (0,0,0)
// logic_Supp_catcher,                //its fLogical volume
// "phys_Supp_catcher",                //its name
// logic_munion_supportCatcher_Source,                        //its mother  volume
// false,                        //no boolean operation
// 0,
// false);                        //copy number
//
// G4VisAttributes* visAtt_Supp_catcher  = new G4VisAttributes(G4Colour(1.,1.,1.));
// visAtt_Supp_catcher->SetVisibility(true);
// visAtt_Supp_catcher->SetForceWireframe(false);
// visAtt_Supp_catcher->SetForceSolid(true);
// logic_Supp_catcher ->SetVisAttributes(visAtt_Supp_catcher );
//
// if(phys_Supp_catcher == NULL) {}
//
// G4Material* Mylar = G4NistManager::Instance()->FindOrBuildMaterial("G4_MYLAR");
// G4Material* Al = G4NistManager::Instance()->FindOrBuildMaterial("G4_Al");
//
// G4double thicknessMylarSource    = 0.5*um;
// G4double thicknessAlSource    = 50.*nm;
// G4double outerRadius_MylarSource = radius_bucoCatcher;
//
// G4Tubs*              AlSource1          = new G4Tubs("AlSource1", 0., outerRadius_MylarSource, thicknessAlSource/2, 0., 360.*deg);
// G4LogicalVolume*     Logic_AlSource1    = new G4LogicalVolume(AlSource1, Al, "LogicAlSource1"); //solid, material, name
// G4VPhysicalVolume*   Physics_AlSource1  = new G4PVPlacement(0,                //no rotation
// G4ThreeVector(0., 0., thicknessAlSource/2),    //position
// Logic_AlSource1,"LogicAlSource1",        //its fLogical volume
// fLogicWorld,            //its mother volume
// false,                //no boolean op.
// 0,
// true);                //copy nb.
//
// G4Tubs*              MylarSource          = new G4Tubs("MylarSource", 0., outerRadius_MylarSource, thicknessMylarSource/2, 0., 360.*deg);
// G4LogicalVolume*     Logic_MylarSource    = new G4LogicalVolume(MylarSource, Mylar, "LogicMylarSource"); //solid, material, name
// G4VPhysicalVolume*   Physics_MylarSource  = new G4PVPlacement(0,                //no rotation
// G4ThreeVector(0., 0., thicknessMylarSource/2+thicknessAlSource),    //position
// Logic_MylarSource,"Logic_MylarSource",        //its fLogical volume
// fLogicWorld,            //its mother volume
// false,                //no boolean op.
// 0,
// true);                //copy nb.
//
// G4Tubs*              AlSource2         = new G4Tubs("AlSource2", 0., outerRadius_MylarSource, thicknessAlSource/2, 0., 360.*deg);
// G4LogicalVolume*     Logic_AlSource2    = new G4LogicalVolume(AlSource2, Al, "LogicAlSource2"); //solid, material, name
// G4VPhysicalVolume*   Physics_AlSource2  = new G4PVPlacement(0,                //no rotation
// G4ThreeVector(0., 0., thicknessAlSource/2+thicknessMylarSource+thicknessAlSource),    //position
// Logic_AlSource2,"LogicAlSource2",        //its fLogical volume
// fLogicWorld,            //its mother volume
// false,                //no boolean op.
// 0,
// true);                //copy nb.
//
// // G4Tubs*              MylarSource          = new G4Tubs("MylarSource", 0., outerRadius_MylarSource, thicknessMylarSource/2, 0., 360.*deg);
// // G4LogicalVolume*     Logic_MylarSource    = new G4LogicalVolume(MylarSource, Mylar, "LogicMylarSource"); //solid, material, name
// // G4VPhysicalVolume*   Physics_MylarSource  = new G4PVPlacement(0,                //no rotation
// // G4ThreeVector(0., 0., thicknessMylarSource/2),    //position
// // Logic_MylarSource,"Logic_MylarSource",        //its fLogical volume
// // fLogicWorld,            //its mother volume
// // false,                //no boolean op.
// // 0,
// // true);                //copy nb.
//
// G4VisAttributes* MylarSource_att = new G4VisAttributes(G4Colour(0.94, 0.5, 0.5)); //pink
// MylarSource_att->SetVisibility(true);
// MylarSource_att->SetForceWireframe(false);
// MylarSource_att->SetForceSolid(true);
// Logic_MylarSource->SetVisAttributes(MylarSource_att);
//
// G4VisAttributes* AlSource_att = new G4VisAttributes(G4Colour(0.94, 0.2, 0.5)); //pink
// AlSource_att->SetVisibility(true);
// AlSource_att->SetForceWireframe(false);
// AlSource_att->SetForceSolid(true);
// Logic_AlSource1->SetVisAttributes(AlSource_att);
// Logic_AlSource2->SetVisAttributes(AlSource_att);
//
//
// if(Physics_MylarSource == NULL) {}

//=========================================================================================================================
 //========================================== SILICON DETECTORS _ COMMON PARAMETERS ========================================
//=========================================================================================================================
 G4double a, z, density;
 G4String name, symbol;
 G4int    nelements, natoms;

 // Il materiale che compone il supporto dei detector al silicio é il PCB. Non esiste nella libreria di G4, occorre fabbricarselo
 G4Element * elSi = new G4Element(name="Silicon",  symbol="Si", z=14., a= 28.0855*g/mole);
 G4Element * elO  = new G4Element(name="Oxigen" ,  symbol="O", z=8. , a= 15.9994 *g/mole);
 G4Material* materialSupportSiliconDetector  = new G4Material(name="Vetronite", density=2.*g/cm3, nelements= 2);//vetro epossidico, ossia PCB
 materialSupportSiliconDetector->AddElement(elSi, natoms=1);
 materialSupportSiliconDetector->AddElement(elO , natoms=2);



 G4double z_height_Source_biggerBaseSiDet_inVerticale = 24.92*mm;
 G4double angle_between_xAxis_AND_zAxis               = 40.*degree; //in generale, poi bisogna vedere il segno e cose varie a seconda della posizione del detector rispetto alla terna

 if (Angle != NULL){
   angle_between_xAxis_AND_zAxis = Angle;
 }

 //Common parameteres to all Si detectors
 G4double pDz    = 1.5*mm;    // Spessore del supporto (su cui soo messe le strip
 G4double pTheta = 0.*degree;
 G4double pPhi   = 0.*degree;
 G4double pDy1   = 34.12*mm;  // Altezza (y) del trapezio sulla prima faccia. Se le facce non sono svasate é uguale a pDy2
 G4double pDx1   = 83.3*mm;   // Lunghezza (x) della base maggiore del trapezio sulla prima faccia. Se le facce non sono svasate é uguale a pDx3
 G4double pDx2   = 31.*mm;    // Lunghezza (x) della base minore del trapezio sulla prima faccia. Se le facce non sono svasate é uguale a pDx4
 G4double pAlp1  = 0*degree;  // Angolo tra i centri delle due facce
 G4double pDy2   = 34.12*mm;  // Altezza (y) del trapezio sulla prima faccia
 G4double pDx3   = 83.3*mm;   // Lunghezza (x) della base maggiore del trapezio sulla prima faccia
 G4double pDx4   = 31.*mm;    // Lunghezza (x) della base minore del trapezio sulla prima faccia
 G4double pAlp2  = 0.*degree;

 G4Trap* supportSiliconDetector= new G4Trap("supportSiliconDetector",
           pDz/2,pTheta,pPhi,pDy1/2,
           pDx1/2,pDx2/2,pAlp1,pDy2/2,
           pDx3/2,pDx4/2,pAlp2);


// G4Trap* supportSiliconDetectorCu= new G4Trap("supportSiliconDetectorCu",
//           pDz/2,pTheta,pPhi,pDy1/2,
//           pDx1/2,pDx2/2,pAlp1,pDy2/2,
//           pDx3/2,pDx4/2,pAlp2);



 G4VisAttributes* SupportSiliconDetector_att = new G4VisAttributes(G4Colour(0.5, 0.5, 0.5));//red (r,g,b) ->grey
 SupportSiliconDetector_att->SetForceWireframe(false);
 SupportSiliconDetector_att->SetForceSolid(true);

//========================================================================================================================
 //============================================= PISTE DETECTOR SILICIO  ==================================================
//========================================================================================================================
double offset = 0.0*cm;
if (off != NULL){
  offset = off;
}



  G4Material*   materialSiliconDetector  = G4NistManager::Instance()->FindOrBuildMaterial("G4_Si");
  G4VisAttributes* SiliconDetector_att   = new G4VisAttributes(G4Colour(1., 0., 0.));//red (r,g,b) ->red
  SiliconDetector_att->SetForceWireframe(false);
  SiliconDetector_att->SetForceSolid(true);


  G4double thicknessSiDetector                        = 300.*um;
  G4double spazio_tra_Bordo_e_strip5                  = 1.29*mm + 0.225*mm; //spazio tra bordo superiore e strip 5 (la più lunga). Per dettagli guardare disegno di Sean.
  G4double spazio_tra_Strip                           = 0.07*mm; //spazio tra ciascuna strip
  G4double thetaInclinazione_SiDetector               = 52.64*degree;
  G4double spazio_tra_Scintillatore_e_BordoSiDetector = 3.745*mm;
  //G4double lunghezzaLatoObliquoSupporto               = pDy1 / sin(thetaInclinazione_SiDetector); //giusto calcolo semplice di trigonometria (su postit giallo)
  //G4double x_CentralLength_SupportSiliconDetector     = pDx1 - (2 * (pDy1/2) * (cos(thetaInclinazione_SiDetector)/sin(thetaInclinazione_SiDetector)));

  // Riferito al disegno di Sean e di Mathieu (foto su telegram, piani stampati in data 17/05/2021 su formato A1)
  G4double xLow_SiDet_Strip_5  = 63.63*mm;
  G4double y_SiDet_Strip_5     = 4.11*mm;
  G4double xHigh_SiDet_Strip_5 = xLow_SiDet_Strip_5 + (2 * y_SiDet_Strip_5 * (cos(thetaInclinazione_SiDetector) / sin(thetaInclinazione_SiDetector)));
  G4double xLow_SiDet_Strip_4  = 56.58*mm;
  G4double y_SiDet_Strip_4     = 4.55*mm;
  G4double xHigh_SiDet_Strip_4 = xLow_SiDet_Strip_4 + (2 * y_SiDet_Strip_4 * (cos(thetaInclinazione_SiDetector) / sin(thetaInclinazione_SiDetector)));
  G4double xLow_SiDet_Strip_3  = 48.52*mm;
  G4double y_SiDet_Strip_3     = 5.21*mm;
  G4double xHigh_SiDet_Strip_3 = xLow_SiDet_Strip_3 + (2 * y_SiDet_Strip_3 * (cos(thetaInclinazione_SiDetector) / sin(thetaInclinazione_SiDetector)));
  G4double xLow_SiDet_Strip_2  = 38.82*mm;
  G4double y_SiDet_Strip_2     = 6.28*mm;
  G4double xHigh_SiDet_Strip_2 = xLow_SiDet_Strip_2 + (2 * y_SiDet_Strip_2 * (cos(thetaInclinazione_SiDetector) / sin(thetaInclinazione_SiDetector)));
  G4double xLow_SiDet_Strip_1  = 25.65*mm;
  G4double y_SiDet_Strip_1     = 8.56*mm;
  G4double xHigh_SiDet_Strip_1 = xLow_SiDet_Strip_1 + (2 * y_SiDet_Strip_1 * (cos(thetaInclinazione_SiDetector) / sin(thetaInclinazione_SiDetector)));

  //Mother volume
  G4Trap* supportSiliconDetectorvide= new G4Trap("supportSiliconDetectorvide",
            (thicknessSiDetector)/2,pTheta,pPhi,pDy1/2,
            pDx1/2,pDx2/2,pAlp1,pDy2/2,
            pDx3/2,pDx4/2,pAlp2);

  // Strip n. 5
  G4Trap* SiDet_Strip_5 = new G4Trap("SiDet_Strip_5",
                                     thicknessSiDetector/2, 0.*degree, 0.*degree, y_SiDet_Strip_5/2,
                                     xHigh_SiDet_Strip_5/2, xLow_SiDet_Strip_5/2, 0.*degree, y_SiDet_Strip_5/2,
                                     xHigh_SiDet_Strip_5/2, xLow_SiDet_Strip_5/2, 0.*degree);

  // Strip n. 4
  G4Trap* SiDet_Strip_4= new G4Trap("SiDet_Strip_4",
                                     thicknessSiDetector/2, 0.*degree, 0.*degree, y_SiDet_Strip_4/2,
                                     xHigh_SiDet_Strip_4/2, xLow_SiDet_Strip_4/2, 0.*degree, y_SiDet_Strip_4/2,
                                     xHigh_SiDet_Strip_4/2, xLow_SiDet_Strip_4/2, 0.*degree);

  // Strip n. 3
  G4Trap* SiDet_Strip_3 = new G4Trap("SiDet_Strip_3",
                                    thicknessSiDetector/2, 0.*degree, 0.*degree, y_SiDet_Strip_3/2,
                                    xHigh_SiDet_Strip_3/2, xLow_SiDet_Strip_3/2, 0.*degree, y_SiDet_Strip_3/2,
                                    xHigh_SiDet_Strip_3/2, xLow_SiDet_Strip_3/2, 0.*degree);

  // Strip n. 2
  G4Trap* SiDet_Strip_2 = new G4Trap("SiDet_Strip_2",
                                    thicknessSiDetector/2, 0.*degree, 0.*degree, y_SiDet_Strip_2/2,
                                    xHigh_SiDet_Strip_2/2, xLow_SiDet_Strip_2/2, 0.*degree, y_SiDet_Strip_2/2,
                                    xHigh_SiDet_Strip_2/2, xLow_SiDet_Strip_2/2, 0.*degree);

  // Strip n. 1
  G4Trap* SiDet_Strip_1 = new G4Trap("SiDet_Strip_1",
                                    thicknessSiDetector/2, 0.*degree, 0.*degree, y_SiDet_Strip_1/2,
                                    xHigh_SiDet_Strip_1/2, xLow_SiDet_Strip_1/2, 0.*degree, y_SiDet_Strip_1/2,
                                    xHigh_SiDet_Strip_1/2, xLow_SiDet_Strip_1/2, 0.*degree);

  ////////////////////////////Dead Layer//////////////////////////////////
  double thicknessSiDetectorDeadLayer = 100*nm;
  // Strip n. 5
  G4Trap* SiDet_Strip_5_dl = new G4Trap("SiDet_Strip_5_dl",
                                    thicknessSiDetectorDeadLayer/2, 0.*degree, 0.*degree, y_SiDet_Strip_5/2,
                                    xHigh_SiDet_Strip_5/2, xLow_SiDet_Strip_5/2, 0.*degree, y_SiDet_Strip_5/2,
                                    xHigh_SiDet_Strip_5/2, xLow_SiDet_Strip_5/2, 0.*degree);

  // Strip n. 4
  G4Trap* SiDet_Strip_4_dl = new G4Trap("SiDet_Strip_4_dl",
                                    thicknessSiDetectorDeadLayer/2, 0.*degree, 0.*degree, y_SiDet_Strip_4/2,
                                    xHigh_SiDet_Strip_4/2, xLow_SiDet_Strip_4/2, 0.*degree, y_SiDet_Strip_4/2,
                                    xHigh_SiDet_Strip_4/2, xLow_SiDet_Strip_4/2, 0.*degree);

  // Strip n. 3
  G4Trap* SiDet_Strip_3_dl = new G4Trap("SiDet_Strip_3_dl",
                                    thicknessSiDetectorDeadLayer/2, 0.*degree, 0.*degree, y_SiDet_Strip_3/2,
                                    xHigh_SiDet_Strip_3/2, xLow_SiDet_Strip_3/2, 0.*degree, y_SiDet_Strip_3/2,
                                    xHigh_SiDet_Strip_3/2, xLow_SiDet_Strip_3/2, 0.*degree);

  // Strip n. 2
  G4Trap* SiDet_Strip_2_dl = new G4Trap("SiDet_Strip_2_dl",
                                    thicknessSiDetectorDeadLayer/2, 0.*degree, 0.*degree, y_SiDet_Strip_2/2,
                                    xHigh_SiDet_Strip_2/2, xLow_SiDet_Strip_2/2, 0.*degree, y_SiDet_Strip_2/2,
                                    xHigh_SiDet_Strip_2/2, xLow_SiDet_Strip_2/2, 0.*degree);

  // Strip n. 1
  G4Trap* SiDet_Strip_1_dl = new G4Trap("SiDet_Strip_1_dl",
                                    thicknessSiDetectorDeadLayer/2, 0.*degree, 0.*degree, y_SiDet_Strip_1/2,
                                    xHigh_SiDet_Strip_1/2, xLow_SiDet_Strip_1/2, 0.*degree, y_SiDet_Strip_1/2,
                                    xHigh_SiDet_Strip_1/2, xLow_SiDet_Strip_1/2, 0.*degree);


  //___________ Supporto in rame della placca in PBC sulla quale é montato il Si detector  ____________
  G4double length_x_SupportoRame_SiDetector                            = 56.*mm;
  G4double height_y_SupportoRame_SiDetector                            = 33.548*mm;
  G4double thickness_z_SupportoRame_SiDetector                         = 1.5*mm;
  G4double x_smallBox_daTagliare_SupportoRame_SiDetector               = 10.*mm; //6
  G4double y_smallBox_daTagliare_SupportoRame_SiDetector               = 10.*mm; //5
  G4double distanza_latoDxBoxTagliata_e_bordoDxSupportoRame_SiDetector = 3.2*mm;

  //G4double y_AngoloInferioreSx_BordoDaTagliare   = (pDy1/2) - y_smallBox_daTagliare_SupportoRame_SiDetector;
  //G4double angCoeff_retta_LatoObliquoDx_Trapezio = (-2*pDy1)/(pDx1 - pDx2);
  //G4double intercetta_retta_LatoObliquoTrapezio  = (-pDy1/2) - (angCoeff_retta_LatoObliquoDx_Trapezio * (pDx1/2));
  //G4double x_AngoloInferioreSx_BordoDaTagliare   = (y_AngoloInferioreSx_BordoDaTagliare - intercetta_retta_LatoObliquoTrapezio)/angCoeff_retta_LatoObliquoDx_Trapezio;

  G4Material*   material_SupportoRame_SiliconDetector = G4NistManager::Instance()->FindOrBuildMaterial("G4_Cu");
  G4VSolid*     biggerBox_material_SupportoRame_SiliconDetector = new G4Box("biggerBox_material_SupportoRame_SiliconDetector" , length_x_SupportoRame_SiDetector/2, height_y_SupportoRame_SiDetector/2, thickness_z_SupportoRame_SiDetector/2);
  G4VSolid*     smallerBox_material_SupportoRame_SiliconDetector = new G4Box("smallerBox_material_SupportoRame_SiliconDetector", x_smallBox_daTagliare_SupportoRame_SiDetector/2, y_smallBox_daTagliare_SupportoRame_SiDetector/2, thickness_z_SupportoRame_SiDetector);
  G4VSolid*     Box_material_SupportoRame_SiliconDetector = new G4SubtractionSolid("biggerBox_material_SupportoRame_SiliconDetector-smallerBox_material_SupportoRame_SiliconDetector",
biggerBox_material_SupportoRame_SiliconDetector, smallerBox_material_SupportoRame_SiliconDetector,
0,
G4ThreeVector(length_x_SupportoRame_SiDetector/2 - distanza_latoDxBoxTagliata_e_bordoDxSupportoRame_SiDetector - x_smallBox_daTagliare_SupportoRame_SiDetector/2,
height_y_SupportoRame_SiDetector/2 - y_smallBox_daTagliare_SupportoRame_SiDetector/2,
0.));

  G4VisAttributes* Box_material_SupportoRame_SiliconDetector_att = new G4VisAttributes(G4Colour(0.7, 0, 0.7));//(r,g,b) => magenta
  G4VisAttributes* Box_material_SupportoRame_SiliconDetector_attvide = new G4VisAttributes(G4Colour(0.5, 0.2, 0.8));//(r,g,b) => magenta
Box_material_SupportoRame_SiliconDetector_att->SetForceWireframe(false);
Box_material_SupportoRame_SiliconDetector_att->SetForceSolid(true);

// Box_material_SupportoRame_SiliconDetector_attvide->SetForceWireframe(false);
// Box_material_SupportoRame_SiliconDetector_attvide->SetForceSolid(true);


//========================================================================================================================
  //=================================================== DETECTOR 1Up  ========================================================
//========================================================================================================================
  G4RotationMatrix* rotationMatrix_1Up = new G4RotationMatrix();
  rotationMatrix_1Up->rotateX(angle_between_xAxis_AND_zAxis);
  rotationMatrix_1Up->rotateY(180.*deg);
  rotationMatrix_1Up->rotateZ(180.*deg);

  G4LogicalVolume* logicSupportSiliconDetector_1Up = new G4LogicalVolume(supportSiliconDetector,
materialSupportSiliconDetector,
"logicSupportSiliconDetector_1Up",
                                                      0, 0, 0);
  G4VPhysicalVolume* physSupportSiliconDetector_1Up = new G4PVPlacement(rotationMatrix_1Up,
G4ThreeVector(0, fRadius_PlasticScintillator + spazio_tra_Scintillatore_e_BordoSiDetector + pDy1/2 - (pDy1/2*(1-cos(angle_between_xAxis_AND_zAxis))), z_height_Source_biggerBaseSiDet_inVerticale + pDz/2 + ((pDy1/2)*sin(thetaInclinazione_SiDetector))-offset), // ultima espressione implementata nella coord z per correggere per effetto di rotazione risp al baricentro (sempre la solita vecchia storia)
logicSupportSiliconDetector_1Up,"physSupportSiliconDetector_1Up",
                                                   fLogicWorld,
                                                   false,
                                                   0);
logicSupportSiliconDetector_1Up->SetVisAttributes(SupportSiliconDetector_att);
 if(physSupportSiliconDetector_1Up == NULL) {}


 ///////
 G4LogicalVolume* logicSupportSiliconDetector_1Upvide = new G4LogicalVolume(supportSiliconDetectorvide,
vide,
"logicSupportSiliconDetector_1Upvide",
                                                     0, 0, 0);
 G4VPhysicalVolume* physSupportSiliconDetector_1Upvide = new G4PVPlacement(rotationMatrix_1Up,
G4ThreeVector(0, fRadius_PlasticScintillator + spazio_tra_Scintillatore_e_BordoSiDetector + pDy1/2 - ((pDy1/2)*(1-cos(angle_between_xAxis_AND_zAxis))) + (pDz+thicknessSiDetector)/2*-sin(angle_between_xAxis_AND_zAxis), -(pDz+thicknessSiDetector)/2*cos(angle_between_xAxis_AND_zAxis) + z_height_Source_biggerBaseSiDet_inVerticale + pDz/2 + ((pDy1/2)*sin(thetaInclinazione_SiDetector))-offset), // ultima espressione implementata nella coord z per correggere per effetto di rotazione risp al baricentro (sempre la solita vecchia storia)
logicSupportSiliconDetector_1Upvide,"logicSupportSiliconDetector_1Upvide",
                                                  fLogicWorld,
                                                  false,
                                                  0);
logicSupportSiliconDetector_1Upvide->SetVisAttributes(Box_material_SupportoRame_SiliconDetector_attvide);
if(physSupportSiliconDetector_1Upvide == NULL) {}
/////////////

  // STRIP 5
  G4LogicalVolume* logicSiDet_1Up_Strip_5 = new G4LogicalVolume(SiDet_Strip_5,
materialSiliconDetector,
"logicSiDet_1Up_Strip_5",
                                               0, 0, 0);
  G4VPhysicalVolume* physSiDet_1Up_Strip_5 = new G4PVPlacement(0,
                                          G4ThreeVector(0, -pDy1/2 + spazio_tra_Bordo_e_strip5 + y_SiDet_Strip_5/2,0.),
logicSiDet_1Up_Strip_5,"logicSiDet_1Up_Strip_5",
logicSupportSiliconDetector_1Upvide,
                                           false,
                                           0,
                                         true);
logicSiDet_1Up_Strip_5->SetVisAttributes(SiliconDetector_att);
 if(physSiDet_1Up_Strip_5 == NULL) {}


 // Strip n. 4
 G4LogicalVolume* logicSiDet_1Up_Strip_4 = new G4LogicalVolume(SiDet_Strip_4,
materialSiliconDetector,
"logicSi_1Up_Det_Strip_4",
                                                              0, 0, 0);
 G4VPhysicalVolume* physSiDet_1Up_Strip_4 = new G4PVPlacement(0,
G4ThreeVector(0, -pDy1/2 + spazio_tra_Bordo_e_strip5 + y_SiDet_Strip_5 + spazio_tra_Strip + y_SiDet_Strip_4/2, 0.),
logicSiDet_1Up_Strip_4,"logicSiDet_1Up_Strip_4",
logicSupportSiliconDetector_1Upvide,
false,
                                                             0);
logicSiDet_1Up_Strip_4->SetVisAttributes(SiliconDetector_att);
 if(physSiDet_1Up_Strip_4 == NULL) {}

  // Strip n. 3
  G4LogicalVolume* logicSiDet_1Up_Strip_3 = new G4LogicalVolume(SiDet_Strip_3,
materialSiliconDetector,
"logicSi_1Up_Det_Strip_3",
0, 0, 0);
  G4VPhysicalVolume* physSiDet_1Up_Strip_3 = new G4PVPlacement(0,
G4ThreeVector(0, -pDy1/2 + spazio_tra_Bordo_e_strip5 + y_SiDet_Strip_5 + spazio_tra_Strip + y_SiDet_Strip_4 + spazio_tra_Strip + y_SiDet_Strip_3/2, 0.),
logicSiDet_1Up_Strip_3,"logicSiDet_1Up_Strip_3",
logicSupportSiliconDetector_1Upvide,
false,
0);
logicSiDet_1Up_Strip_3->SetVisAttributes(SiliconDetector_att);
  if(physSiDet_1Up_Strip_3 == NULL) {}

  // Strip n. 2
  G4LogicalVolume* logicSiDet_1Up_Strip_2 = new G4LogicalVolume(SiDet_Strip_2,
materialSiliconDetector,
"logicSi_1Up_Det_Strip_2",
0, 0, 0);
  G4VPhysicalVolume* physSiDet_1Up_Strip_2 = new G4PVPlacement(0,
G4ThreeVector(0, -pDy1/2 + spazio_tra_Bordo_e_strip5 + y_SiDet_Strip_5 + spazio_tra_Strip + y_SiDet_Strip_4 + spazio_tra_Strip + y_SiDet_Strip_3 + spazio_tra_Strip + y_SiDet_Strip_2/2, 0.),
logicSiDet_1Up_Strip_2,"logicSiDet_1Up_Strip_2",
logicSupportSiliconDetector_1Upvide,
false,
0);
logicSiDet_1Up_Strip_2->SetVisAttributes(SiliconDetector_att);
  if(physSiDet_1Up_Strip_2 == NULL) {}

  // Strip n. 1
  G4LogicalVolume* logicSiDet_1Up_Strip_1 = new G4LogicalVolume(SiDet_Strip_1,
materialSiliconDetector,
"logicSi_1Up_Det_Strip_1",
0, 0, 0);
  G4VPhysicalVolume* physSiDet_1Up_Strip_1 = new G4PVPlacement(0,
G4ThreeVector(0, -pDy1/2 + spazio_tra_Bordo_e_strip5 + y_SiDet_Strip_5 + spazio_tra_Strip + y_SiDet_Strip_4 + spazio_tra_Strip + y_SiDet_Strip_3 + spazio_tra_Strip + y_SiDet_Strip_2 + spazio_tra_Strip + y_SiDet_Strip_1/2, 0.),
logicSiDet_1Up_Strip_1,"logicSiDet_1Up_Strip_1",
logicSupportSiliconDetector_1Upvide,
false,
0);

logicSiDet_1Up_Strip_1->SetVisAttributes(SiliconDetector_att);
  if(physSiDet_1Up_Strip_1 == NULL) {}

  /////////////////Dead Layer 1Up
  //Strip n. 5
  G4LogicalVolume* logicSiDet_1Up_Strip_5_dl = new G4LogicalVolume(SiDet_Strip_5_dl,
    materialSiliconDetector,
    "logicSiDet_1Up_Strip_5_dl",                                               0, 0, 0);
  G4VPhysicalVolume* physSiDet_1Up_Strip_5_dl = new G4PVPlacement(0,
    G4ThreeVector(0., 0., thicknessSiDetector/2 - thicknessSiDetectorDeadLayer/2),
    logicSiDet_1Up_Strip_5_dl,"logicSiDet_1Up_Strip_5_dl",
    logicSiDet_1Up_Strip_5,
    false,
    0);
  if(physSiDet_1Up_Strip_5_dl == NULL) {}

  //Strip n. 4
  G4LogicalVolume* logicSiDet_1Up_Strip_4_dl = new G4LogicalVolume(SiDet_Strip_4_dl,
    materialSiliconDetector,
    "logicSiDet_1Up_Strip_4_dl",                                              0, 0, 0);
  G4VPhysicalVolume* physSiDet_1Up_Strip_4_dl = new G4PVPlacement(0,
    G4ThreeVector(0., 0., thicknessSiDetector/2 - thicknessSiDetectorDeadLayer/2),
    logicSiDet_1Up_Strip_4_dl,"logicSiDet_1Up_Strip_4_dl",
    logicSiDet_1Up_Strip_4,
    false,
    0);

    if(physSiDet_1Up_Strip_4_dl == NULL) {}

  //Strip n. 3
  G4LogicalVolume* logicSiDet_1Up_Strip_3_dl = new G4LogicalVolume(SiDet_Strip_3_dl,
    materialSiliconDetector,
    "logicSiDet_1Up_Strip_3_dl",                                               0, 0, 0);
  G4VPhysicalVolume* physSiDet_1Up_Strip_3_dl = new G4PVPlacement(0,
    G4ThreeVector(0., 0., thicknessSiDetector/2 - thicknessSiDetectorDeadLayer/2),
    logicSiDet_1Up_Strip_3_dl,"logicSiDet_1Up_Strip_3_dl",
    logicSiDet_1Up_Strip_3,
    false,
    0);

    if(physSiDet_1Up_Strip_3_dl == NULL) {}

  //Strip n. 2
  G4LogicalVolume* logicSiDet_1Up_Strip_2_dl = new G4LogicalVolume(SiDet_Strip_2_dl,
    materialSiliconDetector,
    "logicSiDet_1Up_Strip_2_dl",                                               0, 0, 0);
  G4VPhysicalVolume* physSiDet_1Up_Strip_2_dl = new G4PVPlacement(0,
    G4ThreeVector(0., 0., thicknessSiDetector/2 - thicknessSiDetectorDeadLayer/2),
    logicSiDet_1Up_Strip_2_dl,"logicSiDet_1Up_Strip_2_dl",
    logicSiDet_1Up_Strip_2,
    false,
    0);

    if(physSiDet_1Up_Strip_2_dl == NULL) {}

//Strip n. 1
G4LogicalVolume* logicSiDet_1Up_Strip_1_dl = new G4LogicalVolume(SiDet_Strip_1_dl,
  materialSiliconDetector,
  "logicSiDet_1Up_Strip_1_dl",                                               0, 0, 0);
G4VPhysicalVolume* physSiDet_1Up_Strip_1_dl = new G4PVPlacement(0,
  G4ThreeVector(0., 0., thicknessSiDetector/2 - thicknessSiDetectorDeadLayer/2),
  logicSiDet_1Up_Strip_1_dl,"logicSiDet_1Up_Strip_1_dl",
  logicSiDet_1Up_Strip_1,
  false,
  0);

  if(physSiDet_1Up_Strip_1_dl == NULL) {}

  // SUPPORTO IN RAME
  G4LogicalVolume* logic_Box_material_SupportoRame_SiliconDetector_1Up   = new G4LogicalVolume(Box_material_SupportoRame_SiliconDetector, material_SupportoRame_SiliconDetector, "logic_Box_material_SupportoRame_SiliconDetector_1Up"); //solid, material, name
  G4VPhysicalVolume* physics_Box_material_SupportoRame_SiliconDetector_1Up = new G4PVPlacement(0,                //no rotation
G4ThreeVector(
(length_x_SupportoRame_SiDetector/2 - x_smallBox_daTagliare_SupportoRame_SiDetector - distanza_latoDxBoxTagliata_e_bordoDxSupportoRame_SiDetector - (y_smallBox_daTagliare_SupportoRame_SiDetector/tan(thetaInclinazione_SiDetector) -1.*mm)),
//(length_x_SupportoRame_SiDetector/2 - distanza_latoDxBoxTagliata_e_bordoDxSupportoRame_SiDetector - x_smallBox_daTagliare_BordoDaTagliare)/2,
pDy1/2 - height_y_SupportoRame_SiDetector/2,
-pDz/2 -thickness_z_SupportoRame_SiDetector/2
),    //position, gi� negativa
logic_Box_material_SupportoRame_SiliconDetector_1Up, "logic_Box_material_SupportoRame_SiliconDetector_1Up", //its fLogical volume
logicSupportSiliconDetector_1Up,            //its mother volume
false,                //no boolean op.
0);                //copy nb.
logic_Box_material_SupportoRame_SiliconDetector_1Up->SetVisAttributes(Box_material_SupportoRame_SiliconDetector_att);
  if(physics_Box_material_SupportoRame_SiliconDetector_1Up == NULL) {}


//========================================================================================================================
  //=================================================== DETECTOR 2Up  ========================================================
//========================================================================================================================
  G4RotationMatrix* rotationMatrix_2Up = new G4RotationMatrix();
  rotationMatrix_2Up->rotateX(0.*deg);
  rotationMatrix_2Up->rotateY(180.*deg - angle_between_xAxis_AND_zAxis);
  rotationMatrix_2Up->rotateZ(90.*deg);

  G4LogicalVolume* logicSupportSiliconDetector_2Up = new G4LogicalVolume(supportSiliconDetector,
materialSupportSiliconDetector,
"logicSupportSiliconDetector_2Up",
0, 0, 0);
  G4VPhysicalVolume* physSupportSiliconDetector_2Up = new G4PVPlacement(rotationMatrix_2Up,
G4ThreeVector(fRadius_PlasticScintillator + spazio_tra_Scintillatore_e_BordoSiDetector + pDy1/2 - (pDy1/2*(1-cos(angle_between_xAxis_AND_zAxis))), 0., z_height_Source_biggerBaseSiDet_inVerticale + pDz/2 + ((pDy1/2)*sin(thetaInclinazione_SiDetector))-offset), // ultima espressione implementata nella coord z per correggere per effetto di rotazione risp al baricentro (sempre la solita vecchia storia)
logicSupportSiliconDetector_2Up,"physSupportSiliconDetector_2Up",
fLogicWorld,
false,
                                                                   0);
logicSupportSiliconDetector_2Up->SetVisAttributes(SupportSiliconDetector_att);
  if(physSupportSiliconDetector_2Up == NULL) {}

  ///////
  G4LogicalVolume* logicSupportSiliconDetector_2Upvide = new G4LogicalVolume(supportSiliconDetectorvide,
 vide,
 "logicSupportSiliconDetector_2Upvide",
                                                      0, 0, 0);
  G4VPhysicalVolume* physSupportSiliconDetector_2Upvide = new G4PVPlacement(rotationMatrix_2Up,
 G4ThreeVector(fRadius_PlasticScintillator + spazio_tra_Scintillatore_e_BordoSiDetector + pDy1/2 - ((pDy1/2)*(1-cos(angle_between_xAxis_AND_zAxis))) + (pDz+thicknessSiDetector)/2*-sin(angle_between_xAxis_AND_zAxis), 0., -(pDz+thicknessSiDetector)/2*cos(angle_between_xAxis_AND_zAxis) + z_height_Source_biggerBaseSiDet_inVerticale + pDz/2 + ((pDy1/2)*sin(thetaInclinazione_SiDetector))-offset), // ultima espressione implementata nella coord z per correggere per effetto di rotazione risp al baricentro (sempre la solita vecchia storia)
 logicSupportSiliconDetector_2Upvide,"logicSupportSiliconDetector_2Upvide",
                                                   fLogicWorld,
                                                   false,
                                                   0);
 logicSupportSiliconDetector_2Upvide->SetVisAttributes(Box_material_SupportoRame_SiliconDetector_attvide);
 if(physSupportSiliconDetector_2Upvide == NULL) {}
 /////////////

  // STRIP 5
  G4LogicalVolume* logicSiDet_2Up_Strip_5 = new G4LogicalVolume(SiDet_Strip_5,
materialSiliconDetector,
"logicSiDet_2Up_Strip_5",
0, 0, 0);
  G4VPhysicalVolume* physSiDet_2Up_Strip_5 = new G4PVPlacement(0,
G4ThreeVector(0, -pDy1/2 + spazio_tra_Bordo_e_strip5 + y_SiDet_Strip_5/2, 0.),
logicSiDet_2Up_Strip_5,"logicSiDet_2Up_Strip_5",
logicSupportSiliconDetector_2Upvide,
false,
                                                           0);
logicSiDet_2Up_Strip_5->SetVisAttributes(SiliconDetector_att);
  if(physSiDet_2Up_Strip_5 == NULL) {}


  // Strip n. 4
  G4LogicalVolume* logicSiDet_2Up_Strip_4 = new G4LogicalVolume(SiDet_Strip_4,
materialSiliconDetector,
"logicSi_2Up_Det_Strip_4",
                                                              0, 0, 0);
  G4VPhysicalVolume* physSiDet_2Up_Strip_4 = new G4PVPlacement(0,
G4ThreeVector(0, -pDy1/2 + spazio_tra_Bordo_e_strip5 + y_SiDet_Strip_5 + spazio_tra_Strip + y_SiDet_Strip_4/2, 0.),
logicSiDet_2Up_Strip_4,"logicSiDet_2Up_Strip_4",
logicSupportSiliconDetector_2Upvide,
false,
                                                             0);
logicSiDet_2Up_Strip_4->SetVisAttributes(SiliconDetector_att);
  if(physSiDet_2Up_Strip_4 == NULL) {}

  // Strip n. 3
  G4LogicalVolume* logicSiDet_2Up_Strip_3 = new G4LogicalVolume(SiDet_Strip_3,
materialSiliconDetector,
"logicSi_2Up_Det_Strip_3",
0, 0, 0);
  G4VPhysicalVolume* physSiDet_2Up_Strip_3 = new G4PVPlacement(0,
G4ThreeVector(0, -pDy1/2 + spazio_tra_Bordo_e_strip5 + y_SiDet_Strip_5 + spazio_tra_Strip + y_SiDet_Strip_4 + spazio_tra_Strip + y_SiDet_Strip_3/2, 0.),
logicSiDet_2Up_Strip_3,"logicSiDet_2Up_Strip_3",
logicSupportSiliconDetector_2Upvide,
false,
0);
logicSiDet_2Up_Strip_3->SetVisAttributes(SiliconDetector_att);
  if(physSiDet_2Up_Strip_3 == NULL) {}

  // Strip n. 2
  G4LogicalVolume* logicSiDet_2Up_Strip_2 = new G4LogicalVolume(SiDet_Strip_2,
materialSiliconDetector,
"logicSi_2Up_Det_Strip_2",
0, 0, 0);
  G4VPhysicalVolume* physSiDet_2Up_Strip_2 = new G4PVPlacement(0,
G4ThreeVector(0, -pDy1/2 + spazio_tra_Bordo_e_strip5 + y_SiDet_Strip_5 + spazio_tra_Strip + y_SiDet_Strip_4 + spazio_tra_Strip + y_SiDet_Strip_3 + spazio_tra_Strip + y_SiDet_Strip_2/2, 0.),
logicSiDet_2Up_Strip_2,"logicSiDet_2Up_Strip_2",
logicSupportSiliconDetector_2Upvide,
false,
0);
logicSiDet_2Up_Strip_2->SetVisAttributes(SiliconDetector_att);
  if(physSiDet_2Up_Strip_2 == NULL) {}

  // Strip n. 1
  G4LogicalVolume* logicSiDet_2Up_Strip_1 = new G4LogicalVolume(SiDet_Strip_1,
materialSiliconDetector,
"logicSi_2Up_Det_Strip_1",
0, 0, 0);
  G4VPhysicalVolume* physSiDet_2Up_Strip_1 = new G4PVPlacement(0,
G4ThreeVector(0, -pDy1/2 + spazio_tra_Bordo_e_strip5 + y_SiDet_Strip_5 + spazio_tra_Strip + y_SiDet_Strip_4 + spazio_tra_Strip + y_SiDet_Strip_3 + spazio_tra_Strip + y_SiDet_Strip_2 + spazio_tra_Strip + y_SiDet_Strip_1/2, 0.),
logicSiDet_2Up_Strip_1,"logicSiDet_2Up_Strip_1",
logicSupportSiliconDetector_2Upvide,
false,
0);
logicSiDet_2Up_Strip_1->SetVisAttributes(SiliconDetector_att);
  if(physSiDet_2Up_Strip_1 == NULL) {}

  // SUPPORTO IN RAME
  G4LogicalVolume* logic_Box_material_SupportoRame_SiliconDetector_2Up   = new G4LogicalVolume(Box_material_SupportoRame_SiliconDetector, material_SupportoRame_SiliconDetector, "logic_Box_material_SupportoRame_SiliconDetector_2Up"); //solid, material, name
  G4VPhysicalVolume* physics_Box_material_SupportoRame_SiliconDetector_2Up = new G4PVPlacement(0,                //no rotation
G4ThreeVector((length_x_SupportoRame_SiDetector/2 - x_smallBox_daTagliare_SupportoRame_SiDetector - distanza_latoDxBoxTagliata_e_bordoDxSupportoRame_SiDetector - (y_smallBox_daTagliare_SupportoRame_SiDetector/tan(thetaInclinazione_SiDetector) -1.*mm)), pDy1/2 - height_y_SupportoRame_SiDetector/2, -pDz/2 -thickness_z_SupportoRame_SiDetector/2),    //position, gi� negativa
logic_Box_material_SupportoRame_SiliconDetector_2Up, "logic_Box_material_SupportoRame_SiliconDetector_2Up", //its fLogical volume
logicSupportSiliconDetector_2Up,            //its mother volume
false,                //no boolean op.
0);                //copy nb.
logic_Box_material_SupportoRame_SiliconDetector_2Up->SetVisAttributes(Box_material_SupportoRame_SiliconDetector_att);
  if(physics_Box_material_SupportoRame_SiliconDetector_2Up == NULL) {}


  /////////////////Dead Layer 2Up
  //Strip n. 5
  G4LogicalVolume* logicSiDet_2Up_Strip_5_dl = new G4LogicalVolume(SiDet_Strip_5_dl,
    materialSiliconDetector,
    "logicSiDet_2Up_Strip_5_dl",                                               0, 0, 0);
  G4VPhysicalVolume* physSiDet_2Up_Strip_5_dl = new G4PVPlacement(0,
    G4ThreeVector(0., 0., thicknessSiDetector/2 - thicknessSiDetectorDeadLayer/2),
    logicSiDet_2Up_Strip_5_dl,"logicSiDet_2Up_Strip_5_dl",
    logicSiDet_2Up_Strip_5,
    false,
    0);

  //Strip n. 4
  G4LogicalVolume* logicSiDet_2Up_Strip_4_dl = new G4LogicalVolume(SiDet_Strip_4_dl,
    materialSiliconDetector,
    "logicSiDet_2Up_Strip_4_dl",                                              0, 0, 0);
  G4VPhysicalVolume* physSiDet_2Up_Strip_4_dl = new G4PVPlacement(0,
    G4ThreeVector(0., 0., thicknessSiDetector/2 - thicknessSiDetectorDeadLayer/2),
    logicSiDet_2Up_Strip_4_dl,"logicSiDet_2Up_Strip_4_dl",
    logicSiDet_2Up_Strip_4,
    false,
    0);

  //Strip n. 3
  G4LogicalVolume* logicSiDet_2Up_Strip_3_dl = new G4LogicalVolume(SiDet_Strip_3_dl,
    materialSiliconDetector,
    "logicSiDet_2Up_Strip_3_dl",                                               0, 0, 0);
  G4VPhysicalVolume* physSiDet_2Up_Strip_3_dl = new G4PVPlacement(0,
    G4ThreeVector(0., 0., thicknessSiDetector/2 - thicknessSiDetectorDeadLayer/2),
    logicSiDet_2Up_Strip_3_dl,"logicSiDet_2Up_Strip_3_dl",
    logicSiDet_2Up_Strip_3,
    false,
    0);

  //Strip n. 2
  G4LogicalVolume* logicSiDet_2Up_Strip_2_dl = new G4LogicalVolume(SiDet_Strip_2_dl,
    materialSiliconDetector,
    "logicSiDet_2Up_Strip_2_dl",                                               0, 0, 0);
  G4VPhysicalVolume* physSiDet_2Up_Strip_2_dl = new G4PVPlacement(0,
    G4ThreeVector(0., 0., thicknessSiDetector/2 - thicknessSiDetectorDeadLayer/2),
    logicSiDet_2Up_Strip_2_dl,"logicSiDet_2Up_Strip_2_dl",
    logicSiDet_2Up_Strip_2,
    false,
    0);

//Strip n. 1
G4LogicalVolume* logicSiDet_2Up_Strip_1_dl = new G4LogicalVolume(SiDet_Strip_1_dl,
  materialSiliconDetector,
  "logicSiDet_2Up_Strip_1_dl",                                               0, 0, 0);
G4VPhysicalVolume* physSiDet_2Up_Strip_1_dl = new G4PVPlacement(0,
  G4ThreeVector(0., 0., thicknessSiDetector/2 - thicknessSiDetectorDeadLayer/2),
  logicSiDet_2Up_Strip_1_dl,"logicSiDet_2Up_Strip_1_dl",
  logicSiDet_2Up_Strip_1,
  false,
  0);

  if(physSiDet_2Up_Strip_5_dl == NULL) {}
  if(physSiDet_2Up_Strip_4_dl == NULL) {}
  if(physSiDet_2Up_Strip_3_dl == NULL) {}
  if(physSiDet_2Up_Strip_2_dl == NULL) {}
  if(physSiDet_2Up_Strip_1_dl == NULL) {}

//========================================================================================================================
  //=================================================== DETECTOR 3Up  ========================================================
//========================================================================================================================
  G4RotationMatrix* rotationMatrix_3Up = new G4RotationMatrix();
rotationMatrix_3Up->rotateX(-angle_between_xAxis_AND_zAxis);
  rotationMatrix_3Up->rotateY(180.*deg);
  rotationMatrix_3Up->rotateZ(0.*deg);

  G4LogicalVolume* logicSupportSiliconDetector_3Up = new G4LogicalVolume(supportSiliconDetector,
materialSupportSiliconDetector,
"logicSupportSiliconDetector_3Up",
                                                      0, 0, 0);
  G4VPhysicalVolume* physSupportSiliconDetector_3Up = new G4PVPlacement(rotationMatrix_3Up,
G4ThreeVector(0, -fRadius_PlasticScintillator - spazio_tra_Scintillatore_e_BordoSiDetector - pDy1/2  + (pDy1/2*(1-cos(angle_between_xAxis_AND_zAxis))), z_height_Source_biggerBaseSiDet_inVerticale + pDz/2 + ((pDy1/2)*sin(thetaInclinazione_SiDetector))-offset), // ultima espressione implementata nella coord z per correggere per effetto di rotazione risp al baricentro (sempre la solita vecchia storia)
logicSupportSiliconDetector_3Up,"physSupportSiliconDetector_3Up",
                                                   fLogicWorld,
                                                   false,
                                                   0);
logicSupportSiliconDetector_3Up->SetVisAttributes(SupportSiliconDetector_att);
 if(physSupportSiliconDetector_3Up == NULL) {}

 ///////
 G4LogicalVolume* logicSupportSiliconDetector_3Upvide = new G4LogicalVolume(supportSiliconDetectorvide,
vide,
"logicSupportSiliconDetector_3Upvide",
                                                     0, 0, 0);
 G4VPhysicalVolume* physSupportSiliconDetector_3Upvide = new G4PVPlacement(rotationMatrix_3Up,
G4ThreeVector(0, -fRadius_PlasticScintillator - spazio_tra_Scintillatore_e_BordoSiDetector - pDy1/2  + (pDy1/2*(1-cos(angle_between_xAxis_AND_zAxis))) - (pDz+thicknessSiDetector)/2*-sin(angle_between_xAxis_AND_zAxis) , -(pDz+thicknessSiDetector)/2*cos(angle_between_xAxis_AND_zAxis) + z_height_Source_biggerBaseSiDet_inVerticale + pDz/2 + ((pDy1/2)*sin(thetaInclinazione_SiDetector))-offset), // ultima espressione implementata nella coord z per correggere per effetto di rotazione risp al baricentro (sempre la solita vecchia storia)
logicSupportSiliconDetector_3Upvide,"logicSupportSiliconDetector_3Upvide",
                                                  fLogicWorld,
                                                  false,
                                                  0);
logicSupportSiliconDetector_3Upvide->SetVisAttributes(Box_material_SupportoRame_SiliconDetector_attvide);
if(physSupportSiliconDetector_3Upvide == NULL) {}
/////////////

  // STRIP 5
  G4LogicalVolume* logicSiDet_3Up_Strip_5 = new G4LogicalVolume(SiDet_Strip_5,
materialSiliconDetector,
"logicSiDet_3Up_Strip_5",
                                               0, 0, 0);
  G4VPhysicalVolume* physSiDet_3Up_Strip_5 = new G4PVPlacement(0,
                                           G4ThreeVector(0, -pDy1/2 + spazio_tra_Bordo_e_strip5 + y_SiDet_Strip_5/2,0.),
logicSiDet_3Up_Strip_5,"logicSiDet_3Up_Strip_5",
logicSupportSiliconDetector_3Upvide,
                                           false,
                                           0);
logicSiDet_3Up_Strip_5->SetVisAttributes(SiliconDetector_att);
 if(physSiDet_3Up_Strip_5 == NULL) {}


 // Strip n. 4
 G4LogicalVolume* logicSiDet_3Up_Strip_4 = new G4LogicalVolume(SiDet_Strip_4,
materialSiliconDetector,
"logicSi_3Up_Det_Strip_4",
                                                              0, 0, 0);
 G4VPhysicalVolume* physSiDet_3Up_Strip_4 = new G4PVPlacement(0,
G4ThreeVector(0, -pDy1/2 + spazio_tra_Bordo_e_strip5 + y_SiDet_Strip_5 + spazio_tra_Strip + y_SiDet_Strip_4/2, 0.),
logicSiDet_3Up_Strip_4,"logicSiDet_3Up_Strip_4",
logicSupportSiliconDetector_3Upvide,
false,
                                                             0);
logicSiDet_3Up_Strip_4->SetVisAttributes(SiliconDetector_att);
 if(physSiDet_3Up_Strip_4 == NULL) {}

  // Strip n. 3
  G4LogicalVolume* logicSiDet_3Up_Strip_3 = new G4LogicalVolume(SiDet_Strip_3,
materialSiliconDetector,
"logicSi_3Up_Det_Strip_3",
0, 0, 0);
  G4VPhysicalVolume* physSiDet_3Up_Strip_3 = new G4PVPlacement(0,
G4ThreeVector(0, -pDy1/2 + spazio_tra_Bordo_e_strip5 + y_SiDet_Strip_5 + spazio_tra_Strip + y_SiDet_Strip_4 + spazio_tra_Strip + y_SiDet_Strip_3/2, 0.),
logicSiDet_3Up_Strip_3,"logicSiDet_3Up_Strip_3",
logicSupportSiliconDetector_3Upvide,
false,
0);
logicSiDet_3Up_Strip_3->SetVisAttributes(SiliconDetector_att);
  if(physSiDet_3Up_Strip_3 == NULL) {}

  // Strip n. 2
  G4LogicalVolume* logicSiDet_3Up_Strip_2 = new G4LogicalVolume(SiDet_Strip_2,
materialSiliconDetector,
"logicSi_3Up_Det_Strip_2",
0, 0, 0);
  G4VPhysicalVolume* physSiDet_3Up_Strip_2 = new G4PVPlacement(0,
G4ThreeVector(0, -pDy1/2 + spazio_tra_Bordo_e_strip5 + y_SiDet_Strip_5 + spazio_tra_Strip + y_SiDet_Strip_4 + spazio_tra_Strip + y_SiDet_Strip_3 + spazio_tra_Strip + y_SiDet_Strip_2/2, 0.),
logicSiDet_3Up_Strip_2,"logicSiDet_3Up_Strip_2",
logicSupportSiliconDetector_3Upvide,
false,
0);
logicSiDet_3Up_Strip_2->SetVisAttributes(SiliconDetector_att);
  if(physSiDet_3Up_Strip_2 == NULL) {}

  // Strip n. 1
  G4LogicalVolume* logicSiDet_3Up_Strip_1 = new G4LogicalVolume(SiDet_Strip_1,
materialSiliconDetector,
"logicSi_3Up_Det_Strip_1",
0, 0, 0);
  G4VPhysicalVolume* physSiDet_3Up_Strip_1 = new G4PVPlacement(0,
G4ThreeVector(0, -pDy1/2 + spazio_tra_Bordo_e_strip5 + y_SiDet_Strip_5 + spazio_tra_Strip + y_SiDet_Strip_4 + spazio_tra_Strip + y_SiDet_Strip_3 + spazio_tra_Strip + y_SiDet_Strip_2 + spazio_tra_Strip + y_SiDet_Strip_1/2, 0.),
logicSiDet_3Up_Strip_1,"logicSiDet_3Up_Strip_1",
logicSupportSiliconDetector_3Upvide,
false,
0);
logicSiDet_3Up_Strip_1->SetVisAttributes(SiliconDetector_att);
  if(physSiDet_3Up_Strip_1 == NULL) {}

  // SUPPORTO IN RAME
  G4LogicalVolume* logic_Box_material_SupportoRame_SiliconDetector_3Up   = new G4LogicalVolume(Box_material_SupportoRame_SiliconDetector, material_SupportoRame_SiliconDetector, "logic_Box_material_SupportoRame_SiliconDetector_3Up"); //solid, material, name
  G4VPhysicalVolume* physics_Box_material_SupportoRame_SiliconDetector_3Up = new G4PVPlacement(0,                //no rotation
G4ThreeVector((length_x_SupportoRame_SiDetector/2 - x_smallBox_daTagliare_SupportoRame_SiDetector - distanza_latoDxBoxTagliata_e_bordoDxSupportoRame_SiDetector - (y_smallBox_daTagliare_SupportoRame_SiDetector/tan(thetaInclinazione_SiDetector) -1.*mm)), pDy1/2 - height_y_SupportoRame_SiDetector/2, -pDz/2 -thickness_z_SupportoRame_SiDetector/2),    //position, gi� negativa
logic_Box_material_SupportoRame_SiliconDetector_3Up, "logic_Box_material_SupportoRame_SiliconDetector_3Up", //its fLogical volume
logicSupportSiliconDetector_3Up,            //its mother volume
false,                //no boolean op.
0);                //copy nb.
logic_Box_material_SupportoRame_SiliconDetector_3Up->SetVisAttributes(Box_material_SupportoRame_SiliconDetector_att);
  if(physics_Box_material_SupportoRame_SiliconDetector_3Up == NULL) {}

  /////////////////Dead Layer 3Up
  //Strip n. 5
  G4LogicalVolume* logicSiDet_3Up_Strip_5_dl = new G4LogicalVolume(SiDet_Strip_5_dl,
    materialSiliconDetector,
    "logicSiDet_3Up_Strip_5_dl",                                               0, 0, 0);
  G4VPhysicalVolume* physSiDet_3Up_Strip_5_dl = new G4PVPlacement(0,
    G4ThreeVector(0., 0., thicknessSiDetector/2 - thicknessSiDetectorDeadLayer/2),
    logicSiDet_3Up_Strip_5_dl,"logicSiDet_3Up_Strip_5_dl",
    logicSiDet_3Up_Strip_5,
    false,
    0);

  //Strip n. 4
  G4LogicalVolume* logicSiDet_3Up_Strip_4_dl = new G4LogicalVolume(SiDet_Strip_4_dl,
    materialSiliconDetector,
    "logicSiDet_3Up_Strip_4_dl",                                              0, 0, 0);
  G4VPhysicalVolume* physSiDet_3Up_Strip_4_dl = new G4PVPlacement(0,
    G4ThreeVector(0., 0., thicknessSiDetector/2 - thicknessSiDetectorDeadLayer/2),
    logicSiDet_3Up_Strip_4_dl,"logicSiDet_3Up_Strip_4_dl",
    logicSiDet_3Up_Strip_4,
    false,
    0);

  //Strip n. 3
  G4LogicalVolume* logicSiDet_3Up_Strip_3_dl = new G4LogicalVolume(SiDet_Strip_3_dl,
    materialSiliconDetector,
    "logicSiDet_3Up_Strip_3_dl",                                               0, 0, 0);
  G4VPhysicalVolume* physSiDet_3Up_Strip_3_dl = new G4PVPlacement(0,
    G4ThreeVector(0., 0., thicknessSiDetector/2 - thicknessSiDetectorDeadLayer/2),
    logicSiDet_3Up_Strip_3_dl,"logicSiDet_3Up_Strip_3_dl",
    logicSiDet_3Up_Strip_3,
    false,
    0);

  //Strip n. 2
  G4LogicalVolume* logicSiDet_3Up_Strip_2_dl = new G4LogicalVolume(SiDet_Strip_2_dl,
    materialSiliconDetector,
    "logicSiDet_3Up_Strip_2_dl",                                               0, 0, 0);
  G4VPhysicalVolume* physSiDet_3Up_Strip_2_dl = new G4PVPlacement(0,
    G4ThreeVector(0., 0., thicknessSiDetector/2 - thicknessSiDetectorDeadLayer/2),
    logicSiDet_3Up_Strip_2_dl,"logicSiDet_3Up_Strip_2_dl",
    logicSiDet_3Up_Strip_2,
    false,
    0);

//Strip n. 1
G4LogicalVolume* logicSiDet_3Up_Strip_1_dl = new G4LogicalVolume(SiDet_Strip_1_dl,
  materialSiliconDetector,
  "logicSiDet_3Up_Strip_1_dl",                                               0, 0, 0);
G4VPhysicalVolume* physSiDet_3Up_Strip_1_dl = new G4PVPlacement(0,
  G4ThreeVector(0., 0., thicknessSiDetector/2 - thicknessSiDetectorDeadLayer/2),
  logicSiDet_3Up_Strip_1_dl,"logicSiDet_3Up_Strip_1_dl",
  logicSiDet_3Up_Strip_1,
  false,
  0);

  if(physSiDet_3Up_Strip_5_dl == NULL) {}
  if(physSiDet_3Up_Strip_4_dl == NULL) {}
  if(physSiDet_3Up_Strip_3_dl == NULL) {}
  if(physSiDet_3Up_Strip_2_dl == NULL) {}
  if(physSiDet_3Up_Strip_1_dl == NULL) {}

//========================================================================================================================
  //=================================================== DETECTOR 4Up  ========================================================
//========================================================================================================================
  G4RotationMatrix* rotationMatrix_4Up = new G4RotationMatrix();
  rotationMatrix_4Up->rotateX(0.*deg);
  rotationMatrix_4Up->rotateY(180.*deg + angle_between_xAxis_AND_zAxis);
  rotationMatrix_4Up->rotateZ(-90.*deg);

  G4LogicalVolume* logicSupportSiliconDetector_4Up = new G4LogicalVolume(supportSiliconDetector,
materialSupportSiliconDetector,
"logicSupportSiliconDetector_4Up",
0, 0, 0);
  G4VPhysicalVolume* physSupportSiliconDetector_4Up = new G4PVPlacement(rotationMatrix_4Up,
G4ThreeVector(-fRadius_PlasticScintillator - spazio_tra_Scintillatore_e_BordoSiDetector - pDy1/2  + (pDy1/2*(1-cos(angle_between_xAxis_AND_zAxis))), 0., z_height_Source_biggerBaseSiDet_inVerticale + pDz/2 +150*um + ((pDy1/2)*sin(thetaInclinazione_SiDetector))-offset), // ultima espressione implementata nella coord z per correggere per effetto di rotazione risp al baricentro (sempre la solita vecchia storia)
logicSupportSiliconDetector_4Up,"physSupportSiliconDetector_4Up",
fLogicWorld,
false,
                                                                   0);
logicSupportSiliconDetector_4Up->SetVisAttributes(SupportSiliconDetector_att);
  if(physSupportSiliconDetector_4Up == NULL) {}

  ///////
  G4LogicalVolume* logicSupportSiliconDetector_4Upvide = new G4LogicalVolume(supportSiliconDetectorvide,
 vide,
 "logicSupportSiliconDetector_4Upvide",
                                                      0, 0, 0);
  G4VPhysicalVolume* physSupportSiliconDetector_4Upvide = new G4PVPlacement(rotationMatrix_4Up,
G4ThreeVector(-fRadius_PlasticScintillator - spazio_tra_Scintillatore_e_BordoSiDetector - pDy1/2  + (pDy1/2*(1-cos(angle_between_xAxis_AND_zAxis))) - (pDz+thicknessSiDetector)/2*-sin(angle_between_xAxis_AND_zAxis), 0., -(pDz+thicknessSiDetector)/2*cos(angle_between_xAxis_AND_zAxis) + z_height_Source_biggerBaseSiDet_inVerticale + pDz/2 +150*um + ((pDy1/2)*sin(thetaInclinazione_SiDetector))-offset), // ultima espressione implementata nella coord z per correggere per effetto di rotazione risp al baricentro (sempre la solita vecchia storia)
 logicSupportSiliconDetector_4Upvide,"logicSupportSiliconDetector_4Upvide",
                                                   fLogicWorld,
                                                   false,
                                                   0);
 logicSupportSiliconDetector_4Upvide->SetVisAttributes(Box_material_SupportoRame_SiliconDetector_attvide);
 if(physSupportSiliconDetector_4Upvide == NULL) {}
 /////////////

  // STRIP 5
  G4LogicalVolume* logicSiDet_4Up_Strip_5 = new G4LogicalVolume(SiDet_Strip_5,
materialSiliconDetector,
"logicSiDet_4Up_Strip_5",
0, 0, 0);
  G4VPhysicalVolume* physSiDet_4Up_Strip_5 = new G4PVPlacement(0,
G4ThreeVector(0, -pDy1/2 + spazio_tra_Bordo_e_strip5 + y_SiDet_Strip_5/2, 0.),
logicSiDet_4Up_Strip_5,"logicSiDet_4Up_Strip_5",
logicSupportSiliconDetector_4Upvide,
false,
                                                           0);
logicSiDet_4Up_Strip_5->SetVisAttributes(SiliconDetector_att);
  if(physSiDet_4Up_Strip_5 == NULL) {}


  // Strip n. 4
  G4LogicalVolume* logicSiDet_4Up_Strip_4 = new G4LogicalVolume(SiDet_Strip_4,
materialSiliconDetector,
"logicSi_4Up_Det_Strip_4",
                                                              0, 0, 0);
  G4VPhysicalVolume* physSiDet_4Up_Strip_4 = new G4PVPlacement(0,
G4ThreeVector(0, -pDy1/2 + spazio_tra_Bordo_e_strip5 + y_SiDet_Strip_5 + spazio_tra_Strip + y_SiDet_Strip_4/2, 0.),
logicSiDet_4Up_Strip_4,"logicSiDet_4Up_Strip_4",
logicSupportSiliconDetector_4Upvide,
false,
0);
logicSiDet_4Up_Strip_4->SetVisAttributes(SiliconDetector_att);
  if(physSiDet_4Up_Strip_4 == NULL) {}

  // Strip n. 3
  G4LogicalVolume* logicSiDet_4Up_Strip_3 = new G4LogicalVolume(SiDet_Strip_3,
materialSiliconDetector,
"logicSi_4Up_Det_Strip_3",
0, 0, 0);
  G4VPhysicalVolume* physSiDet_4Up_Strip_3 = new G4PVPlacement(0,
G4ThreeVector(0, -pDy1/2 + spazio_tra_Bordo_e_strip5 + y_SiDet_Strip_5 + spazio_tra_Strip + y_SiDet_Strip_4 + spazio_tra_Strip + y_SiDet_Strip_3/2, 0.),
logicSiDet_4Up_Strip_3,"logicSiDet_4Up_Strip_3",
logicSupportSiliconDetector_4Upvide,
false,
0);
logicSiDet_4Up_Strip_3->SetVisAttributes(SiliconDetector_att);
  if(physSiDet_4Up_Strip_3 == NULL) {}

  // Strip n. 2
  G4LogicalVolume* logicSiDet_4Up_Strip_2 = new G4LogicalVolume(SiDet_Strip_2,
materialSiliconDetector,
"logicSi_4Up_Det_Strip_2",
0, 0, 0);
  G4VPhysicalVolume* physSiDet_4Up_Strip_2 = new G4PVPlacement(0,
G4ThreeVector(0, -pDy1/2 + spazio_tra_Bordo_e_strip5 + y_SiDet_Strip_5 + spazio_tra_Strip + y_SiDet_Strip_4 + spazio_tra_Strip + y_SiDet_Strip_3 + spazio_tra_Strip + y_SiDet_Strip_2/2,0.),
logicSiDet_4Up_Strip_2,"logicSiDet_4Up_Strip_2",
logicSupportSiliconDetector_4Upvide,
false,
0,
true);
logicSiDet_4Up_Strip_2->SetVisAttributes(SiliconDetector_att);
  if(physSiDet_4Up_Strip_2 == NULL) {}

  // Strip n. 1
  G4LogicalVolume* logicSiDet_4Up_Strip_1 = new G4LogicalVolume(SiDet_Strip_1,
materialSiliconDetector,
"logicSi_4Up_Det_Strip_1",
0, 0, 0);
  G4VPhysicalVolume* physSiDet_4Up_Strip_1 = new G4PVPlacement(0,
G4ThreeVector(0, -pDy1/2 + spazio_tra_Bordo_e_strip5 + y_SiDet_Strip_5 + spazio_tra_Strip + y_SiDet_Strip_4 + spazio_tra_Strip + y_SiDet_Strip_3 + spazio_tra_Strip + y_SiDet_Strip_2 + spazio_tra_Strip + y_SiDet_Strip_1/2, 0.),
logicSiDet_4Up_Strip_1,"logicSiDet_4Up_Strip_1",
logicSupportSiliconDetector_4Upvide,
false,
0,
true);

logicSiDet_4Up_Strip_1->SetVisAttributes(SiliconDetector_att);
  if(physSiDet_4Up_Strip_1 == NULL) {}

  // SUPPORTO IN RAME
  G4LogicalVolume* logic_Box_material_SupportoRame_SiliconDetector_4Up   = new G4LogicalVolume(Box_material_SupportoRame_SiliconDetector, material_SupportoRame_SiliconDetector, "logic_Box_material_SupportoRame_SiliconDetector_4Up"); //solid, material, name
  G4VPhysicalVolume* physics_Box_material_SupportoRame_SiliconDetector_4Up = new G4PVPlacement(0,                //no rotation
G4ThreeVector((length_x_SupportoRame_SiDetector/2 - x_smallBox_daTagliare_SupportoRame_SiDetector - distanza_latoDxBoxTagliata_e_bordoDxSupportoRame_SiDetector - (y_smallBox_daTagliare_SupportoRame_SiDetector/tan(thetaInclinazione_SiDetector) -1.*mm)), pDy1/2 - height_y_SupportoRame_SiDetector/2, -pDz/2 -thickness_z_SupportoRame_SiDetector/2),    //position, gi� negativa
logic_Box_material_SupportoRame_SiliconDetector_4Up, "logic_Box_material_SupportoRame_SiliconDetector_4Up", //its fLogical volume
logicSupportSiliconDetector_4Up,            //its mother volume
false,                //no boolean op.
0);                //copy nb.
logic_Box_material_SupportoRame_SiliconDetector_4Up->SetVisAttributes(Box_material_SupportoRame_SiliconDetector_att);
  if(physics_Box_material_SupportoRame_SiliconDetector_4Up == NULL) {}

  /////////////////Dead Layer 4Up
  //Strip n. 5
  G4LogicalVolume* logicSiDet_4Up_Strip_5_dl = new G4LogicalVolume(SiDet_Strip_5_dl,
    materialSiliconDetector,
    "logicSiDet_4Up_Strip_5_dl",                                               0, 0, 0);
  G4VPhysicalVolume* physSiDet_4Up_Strip_5_dl = new G4PVPlacement(0,
    G4ThreeVector(0., 0., thicknessSiDetector/2 - thicknessSiDetectorDeadLayer/2),
    logicSiDet_4Up_Strip_5_dl,"logicSiDet_4Up_Strip_5_dl",
    logicSiDet_4Up_Strip_5,
    false,
    0);

  //Strip n. 4
  G4LogicalVolume* logicSiDet_4Up_Strip_4_dl = new G4LogicalVolume(SiDet_Strip_4_dl,
    materialSiliconDetector,
    "logicSiDet_4Up_Strip_4_dl",                                              0, 0, 0);
  G4VPhysicalVolume* physSiDet_4Up_Strip_4_dl = new G4PVPlacement(0,
    G4ThreeVector(0., 0., thicknessSiDetector/2 - thicknessSiDetectorDeadLayer/2),
    logicSiDet_4Up_Strip_4_dl,"logicSiDet_4Up_Strip_4_dl",
    logicSiDet_4Up_Strip_4,
    false,
    0);

  //Strip n. 3
  G4LogicalVolume* logicSiDet_4Up_Strip_3_dl = new G4LogicalVolume(SiDet_Strip_3_dl,
    materialSiliconDetector,
    "logicSiDet_4Up_Strip_3_dl",                                               0, 0, 0);
  G4VPhysicalVolume* physSiDet_4Up_Strip_3_dl = new G4PVPlacement(0,
    G4ThreeVector(0., 0., thicknessSiDetector/2 - thicknessSiDetectorDeadLayer/2),
    logicSiDet_4Up_Strip_3_dl,"logicSiDet_4Up_Strip_3_dl",
    logicSiDet_4Up_Strip_3,
    false,
    0);

  //Strip n. 2
  G4LogicalVolume* logicSiDet_4Up_Strip_2_dl = new G4LogicalVolume(SiDet_Strip_2_dl,
    materialSiliconDetector,
    "logicSiDet_4Up_Strip_2_dl",                                               0, 0, 0);
  G4VPhysicalVolume* physSiDet_4Up_Strip_2_dl = new G4PVPlacement(0,
    G4ThreeVector(0., 0., thicknessSiDetector/2 - thicknessSiDetectorDeadLayer/2),
    logicSiDet_4Up_Strip_2_dl,"logicSiDet_4Up_Strip_2_dl",
    logicSiDet_4Up_Strip_2,
    false,
    0);

//Strip n. 1
G4LogicalVolume* logicSiDet_4Up_Strip_1_dl = new G4LogicalVolume(SiDet_Strip_1_dl,
  materialSiliconDetector,
  "logicSiDet_4Up_Strip_1_dl",                                               0, 0, 0);
G4VPhysicalVolume* physSiDet_4Up_Strip_1_dl = new G4PVPlacement(0,
  G4ThreeVector(0., 0., thicknessSiDetector/2 - thicknessSiDetectorDeadLayer/2),
  logicSiDet_4Up_Strip_1_dl,"logicSiDet_4Up_Strip_1_dl",
  logicSiDet_4Up_Strip_1,
  false,
  0);

  if(physSiDet_4Up_Strip_5_dl == NULL) {}
  if(physSiDet_4Up_Strip_4_dl == NULL) {}
  if(physSiDet_4Up_Strip_3_dl == NULL) {}
  if(physSiDet_4Up_Strip_2_dl == NULL) {}
  if(physSiDet_4Up_Strip_1_dl == NULL) {}

//========================================================================================================================
  //=================================================== DETECTOR 1Down  ========================================================
//========================================================================================================================
  G4RotationMatrix* rotationMatrix_1Down = new G4RotationMatrix();
rotationMatrix_1Down->rotateX(-angle_between_xAxis_AND_zAxis);
  rotationMatrix_1Down->rotateY(0.*deg);
  rotationMatrix_1Down->rotateZ(180.*deg);

  G4LogicalVolume* logicSupportSiliconDetector_1Down = new G4LogicalVolume(supportSiliconDetector,
materialSupportSiliconDetector,
"logicSupportSiliconDetector_1Down",
                                                      0, 0, 0);
  G4VPhysicalVolume* physSupportSiliconDetector_1Down = new G4PVPlacement(rotationMatrix_1Down,
G4ThreeVector(0, fRadius_PlasticScintillator + spazio_tra_Scintillatore_e_BordoSiDetector + pDy1/2  - (pDy1/2*(1-cos(angle_between_xAxis_AND_zAxis))), -z_height_Source_biggerBaseSiDet_inVerticale - pDz/2 - ((pDy1/2)*sin(thetaInclinazione_SiDetector))+offset), // ultima espressione implementata nella coord z per correggere per effetto di rotazione risp al baricentro (sempre la solita vecchia storia)
logicSupportSiliconDetector_1Down,"physSupportSiliconDetector_1Down",
                                                   fLogicWorld,
                                                   false,
                                                   0);
logicSupportSiliconDetector_1Down->SetVisAttributes(SupportSiliconDetector_att);
 if(physSupportSiliconDetector_1Down == NULL) {}

 ///////
 G4LogicalVolume* logicSupportSiliconDetector_1Downvide = new G4LogicalVolume(supportSiliconDetectorvide,
vide,
"logicSupportSiliconDetector_1Downvide",
                                                     0, 0, 0);
 G4VPhysicalVolume* physSupportSiliconDetector_1Downvide = new G4PVPlacement(rotationMatrix_1Down,
G4ThreeVector(0, fRadius_PlasticScintillator + spazio_tra_Scintillatore_e_BordoSiDetector + pDy1/2  - (pDy1/2*(1-cos(angle_between_xAxis_AND_zAxis))) + (pDz+thicknessSiDetector)/2*-sin(angle_between_xAxis_AND_zAxis), +(pDz+thicknessSiDetector)/2*cos(angle_between_xAxis_AND_zAxis)-z_height_Source_biggerBaseSiDet_inVerticale - pDz/2 - ((pDy1/2)*sin(thetaInclinazione_SiDetector))+offset), // ultima espressione implementata nella coord z per correggere per effetto di rotazione risp al baricentro (sempre la solita vecchia storia)
logicSupportSiliconDetector_1Downvide,"logicSupportSiliconDetector_1Downvide",
                                                  fLogicWorld,
                                                  false,
                                                  0);
logicSupportSiliconDetector_1Downvide->SetVisAttributes(Box_material_SupportoRame_SiliconDetector_attvide);
if(physSupportSiliconDetector_1Downvide == NULL) {}
/////////////

  // STRIP 5
  G4LogicalVolume* logicSiDet_1Down_Strip_5 = new G4LogicalVolume(SiDet_Strip_5,
materialSiliconDetector,
"logicSiDet_1Down_Strip_5",
                                               0, 0, 0);
  G4VPhysicalVolume* physSiDet_1Down_Strip_5 = new G4PVPlacement(0,
                                           G4ThreeVector(0, -pDy1/2 + spazio_tra_Bordo_e_strip5 + y_SiDet_Strip_5/2, 0.),
logicSiDet_1Down_Strip_5,"logicSiDet_1Down_Strip_5",
logicSupportSiliconDetector_1Downvide,
                                           false,
                                           0);
logicSiDet_1Down_Strip_5->SetVisAttributes(SiliconDetector_att);
 if(physSiDet_1Down_Strip_5 == NULL) {}


 // Strip n. 4
 G4LogicalVolume* logicSiDet_1Down_Strip_4 = new G4LogicalVolume(SiDet_Strip_4,
materialSiliconDetector,
"logicSi_1Down_Det_Strip_4",
                                                              0, 0, 0);
 G4VPhysicalVolume* physSiDet_1Down_Strip_4 = new G4PVPlacement(0,
G4ThreeVector(0, -pDy1/2 + spazio_tra_Bordo_e_strip5 + y_SiDet_Strip_5 + spazio_tra_Strip + y_SiDet_Strip_4/2, 0.),
logicSiDet_1Down_Strip_4,"logicSiDet_1Down_Strip_4",
logicSupportSiliconDetector_1Downvide,
false,
                                                             0);
logicSiDet_1Down_Strip_4->SetVisAttributes(SiliconDetector_att);
 if(physSiDet_1Down_Strip_4 == NULL) {}

  // Strip n. 3
  G4LogicalVolume* logicSiDet_1Down_Strip_3 = new G4LogicalVolume(SiDet_Strip_3,
materialSiliconDetector,
"logicSi_1Down_Det_Strip_3",
0, 0, 0);
  G4VPhysicalVolume* physSiDet_1Down_Strip_3 = new G4PVPlacement(0,
G4ThreeVector(0, -pDy1/2 + spazio_tra_Bordo_e_strip5 + y_SiDet_Strip_5 + spazio_tra_Strip + y_SiDet_Strip_4 + spazio_tra_Strip + y_SiDet_Strip_3/2, 0.),
logicSiDet_1Down_Strip_3,"logicSiDet_1Down_Strip_3",
logicSupportSiliconDetector_1Downvide,
false,
0);
logicSiDet_1Down_Strip_3->SetVisAttributes(SiliconDetector_att);
  if(physSiDet_1Down_Strip_3 == NULL) {}

  // Strip n. 2
  G4LogicalVolume* logicSiDet_1Down_Strip_2 = new G4LogicalVolume(SiDet_Strip_2,
materialSiliconDetector,
"logicSi_1Down_Det_Strip_2",
0, 0, 0);
  G4VPhysicalVolume* physSiDet_1Down_Strip_2 = new G4PVPlacement(0,
G4ThreeVector(0, -pDy1/2 + spazio_tra_Bordo_e_strip5 + y_SiDet_Strip_5 + spazio_tra_Strip + y_SiDet_Strip_4 + spazio_tra_Strip + y_SiDet_Strip_3 + spazio_tra_Strip + y_SiDet_Strip_2/2, 0.),
logicSiDet_1Down_Strip_2,"logicSiDet_1Down_Strip_2",
logicSupportSiliconDetector_1Downvide,
false,
0);
logicSiDet_1Down_Strip_2->SetVisAttributes(SiliconDetector_att);
  if(physSiDet_1Down_Strip_2 == NULL) {}

  // Strip n. 1
  G4LogicalVolume* logicSiDet_1Down_Strip_1 = new G4LogicalVolume(SiDet_Strip_1,
materialSiliconDetector,
"logicSi_1Down_Det_Strip_1",
0, 0, 0);
  G4VPhysicalVolume* physSiDet_1Down_Strip_1 = new G4PVPlacement(0,
G4ThreeVector(0, -pDy1/2 + spazio_tra_Bordo_e_strip5 + y_SiDet_Strip_5 + spazio_tra_Strip + y_SiDet_Strip_4 + spazio_tra_Strip + y_SiDet_Strip_3 + spazio_tra_Strip + y_SiDet_Strip_2 + spazio_tra_Strip + y_SiDet_Strip_1/2, 0.),
logicSiDet_1Down_Strip_1,"logicSiDet_1Down_Strip_1",
logicSupportSiliconDetector_1Downvide,
false,
0);
logicSiDet_1Down_Strip_1->SetVisAttributes(SiliconDetector_att);
  if(physSiDet_1Down_Strip_1 == NULL) {}

  // SUPPORTO IN RAME
  G4LogicalVolume* logic_Box_material_SupportoRame_SiliconDetector_1Down   = new G4LogicalVolume(Box_material_SupportoRame_SiliconDetector, material_SupportoRame_SiliconDetector, "logic_Box_material_SupportoRame_SiliconDetector_1Down"); //solid, material, name
  G4VPhysicalVolume* physics_Box_material_SupportoRame_SiliconDetector_1Down = new G4PVPlacement(0,                //no rotation
G4ThreeVector((length_x_SupportoRame_SiDetector/2 - x_smallBox_daTagliare_SupportoRame_SiDetector - distanza_latoDxBoxTagliata_e_bordoDxSupportoRame_SiDetector - (y_smallBox_daTagliare_SupportoRame_SiDetector/tan(thetaInclinazione_SiDetector) -1.*mm)), pDy1/2 - height_y_SupportoRame_SiDetector/2, -pDz/2 -thickness_z_SupportoRame_SiDetector/2),    //position, gi� negativa
logic_Box_material_SupportoRame_SiliconDetector_1Down, "logic_Box_material_SupportoRame_SiliconDetector_1Down", //its fLogical volume
logicSupportSiliconDetector_1Down,            //its mother volume
false,                //no boolean op.
0);                //copy nb.
logic_Box_material_SupportoRame_SiliconDetector_1Down->SetVisAttributes(Box_material_SupportoRame_SiliconDetector_att);
  if(physics_Box_material_SupportoRame_SiliconDetector_1Down == NULL) {}

  /////////////////Dead Layer 1Down
  //Strip n. 5
  G4LogicalVolume* logicSiDet_1Down_Strip_5_dl = new G4LogicalVolume(SiDet_Strip_5_dl,
    materialSiliconDetector,
    "logicSiDet_1Down_Strip_5_dl",                                               0, 0, 0);
  G4VPhysicalVolume* physSiDet_1Down_Strip_5_dl = new G4PVPlacement(0,
    G4ThreeVector(0., 0., thicknessSiDetector/2 - thicknessSiDetectorDeadLayer/2),
    logicSiDet_1Down_Strip_5_dl,"logicSiDet_1Down_Strip_5_dl",
    logicSiDet_1Down_Strip_5,
    false,
    0);

  //Strip n. 4
  G4LogicalVolume* logicSiDet_1Down_Strip_4_dl = new G4LogicalVolume(SiDet_Strip_4_dl,
    materialSiliconDetector,
    "logicSiDet_1Down_Strip_4_dl",                                              0, 0, 0);
  G4VPhysicalVolume* physSiDet_1Down_Strip_4_dl = new G4PVPlacement(0,
    G4ThreeVector(0., 0., thicknessSiDetector/2 - thicknessSiDetectorDeadLayer/2),
    logicSiDet_1Down_Strip_4_dl,"logicSiDet_1Down_Strip_4_dl",
    logicSiDet_1Down_Strip_4,
    false,
    0);

  //Strip n. 3
  G4LogicalVolume* logicSiDet_1Down_Strip_3_dl = new G4LogicalVolume(SiDet_Strip_3_dl,
    materialSiliconDetector,
    "logicSiDet_1Down_Strip_3_dl",                                               0, 0, 0);
  G4VPhysicalVolume* physSiDet_1Down_Strip_3_dl = new G4PVPlacement(0,
    G4ThreeVector(0., 0., thicknessSiDetector/2 - thicknessSiDetectorDeadLayer/2),
    logicSiDet_1Down_Strip_3_dl,"logicSiDet_1Down_Strip_3_dl",
    logicSiDet_1Down_Strip_3,
    false,
    0);

  //Strip n. 2
  G4LogicalVolume* logicSiDet_1Down_Strip_2_dl = new G4LogicalVolume(SiDet_Strip_2_dl,
    materialSiliconDetector,
    "logicSiDet_1Down_Strip_2_dl",                                               0, 0, 0);
  G4VPhysicalVolume* physSiDet_1Down_Strip_2_dl = new G4PVPlacement(0,
    G4ThreeVector(0., 0., thicknessSiDetector/2 - thicknessSiDetectorDeadLayer/2),
    logicSiDet_1Down_Strip_2_dl,"logicSiDet_1Down_Strip_2_dl",
    logicSiDet_1Down_Strip_2,
    false,
    0);

//Strip n. 1
G4LogicalVolume* logicSiDet_1Down_Strip_1_dl = new G4LogicalVolume(SiDet_Strip_1_dl,
  materialSiliconDetector,
  "logicSiDet_1Down_Strip_1_dl",                                               0, 0, 0);
G4VPhysicalVolume* physSiDet_1Down_Strip_1_dl = new G4PVPlacement(0,
  G4ThreeVector(0., 0., thicknessSiDetector/2 - thicknessSiDetectorDeadLayer/2),
  logicSiDet_1Down_Strip_1_dl,"logicSiDet_1Down_Strip_1_dl",
  logicSiDet_1Down_Strip_1,
  false,
  0);

  if(physSiDet_1Down_Strip_5_dl == NULL) {}
  if(physSiDet_1Down_Strip_4_dl == NULL) {}
  if(physSiDet_1Down_Strip_3_dl == NULL) {}
  if(physSiDet_1Down_Strip_2_dl == NULL) {}
  if(physSiDet_1Down_Strip_1_dl == NULL) {}
//========================================================================================================================
  //=================================================== DETECTOR 2Down  ========================================================
//========================================================================================================================
  G4RotationMatrix* rotationMatrix_2Down = new G4RotationMatrix();
  rotationMatrix_2Down->rotateX(180.*deg);
  rotationMatrix_2Down->rotateY(180.*deg - angle_between_xAxis_AND_zAxis);
  rotationMatrix_2Down->rotateZ(90.*deg);

  G4LogicalVolume* logicSupportSiliconDetector_2Down = new G4LogicalVolume(supportSiliconDetector,
materialSupportSiliconDetector,
"logicSupportSiliconDetector_2Down",
0, 0, 0);
  G4VPhysicalVolume* physSupportSiliconDetector_2Down = new G4PVPlacement(rotationMatrix_2Down,
G4ThreeVector(fRadius_PlasticScintillator + spazio_tra_Scintillatore_e_BordoSiDetector + pDy1/2  - (pDy1/2*(1-cos(angle_between_xAxis_AND_zAxis))), 0., -z_height_Source_biggerBaseSiDet_inVerticale - pDz/2 - ((pDy1/2)*sin(thetaInclinazione_SiDetector))+offset), // ultima espressione implementata nella coord z per correggere per effetto di rotazione risp al baricentro (sempre la solita vecchia storia)
logicSupportSiliconDetector_2Down,"physSupportSiliconDetector_2Down",
fLogicWorld,
false,
                                                                   0);
logicSupportSiliconDetector_2Down->SetVisAttributes(SupportSiliconDetector_att);
  if(physSupportSiliconDetector_2Down == NULL) {}

  ///////
  G4LogicalVolume* logicSupportSiliconDetector_2Downvide = new G4LogicalVolume(supportSiliconDetectorvide,
  vide,
  "logicSupportSiliconDetector_2Downvide",
                                                      0, 0, 0);
  G4VPhysicalVolume* physSupportSiliconDetector_2Downvide = new G4PVPlacement(rotationMatrix_2Down,
  G4ThreeVector(fRadius_PlasticScintillator + spazio_tra_Scintillatore_e_BordoSiDetector + pDy1/2  - (pDy1/2*(1-cos(angle_between_xAxis_AND_zAxis))) + (pDz+thicknessSiDetector)/2*-sin(angle_between_xAxis_AND_zAxis), 0., + +(pDz+thicknessSiDetector)/2*cos(angle_between_xAxis_AND_zAxis)-z_height_Source_biggerBaseSiDet_inVerticale - pDz/2 - ((pDy1/2)*sin(thetaInclinazione_SiDetector))+offset), // ultima espressione implementata nella coord z per correggere per effetto di rotazione risp al baricentro (sempre la solita vecchia storia)
  logicSupportSiliconDetector_2Downvide,"logicSupportSiliconDetector_2Downvide",
                                                   fLogicWorld,
                                                   false,
                                                   0);
  logicSupportSiliconDetector_2Downvide->SetVisAttributes(Box_material_SupportoRame_SiliconDetector_attvide);
  if(physSupportSiliconDetector_2Downvide == NULL) {}
  /////////////

  // STRIP 5
  G4LogicalVolume* logicSiDet_2Down_Strip_5 = new G4LogicalVolume(SiDet_Strip_5,
materialSiliconDetector,
"logicSiDet_2Down_Strip_5",
0, 0, 0);
  G4VPhysicalVolume* physSiDet_2Down_Strip_5 = new G4PVPlacement(0,
G4ThreeVector(0, -pDy1/2 + spazio_tra_Bordo_e_strip5 + y_SiDet_Strip_5/2, 0.),
logicSiDet_2Down_Strip_5,"logicSiDet_2Down_Strip_5",
logicSupportSiliconDetector_2Downvide,
false,
                                                           0);
logicSiDet_2Down_Strip_5->SetVisAttributes(SiliconDetector_att);
  if(physSiDet_2Down_Strip_5 == NULL) {}


  // Strip n. 4
  G4LogicalVolume* logicSiDet_2Down_Strip_4 = new G4LogicalVolume(SiDet_Strip_4,
materialSiliconDetector,
"logicSi_2Down_Det_Strip_4",
                                                              0, 0, 0);
  G4VPhysicalVolume* physSiDet_2Down_Strip_4 = new G4PVPlacement(0,
G4ThreeVector(0, -pDy1/2 + spazio_tra_Bordo_e_strip5 + y_SiDet_Strip_5 + spazio_tra_Strip + y_SiDet_Strip_4/2, 0.),
logicSiDet_2Down_Strip_4,"logicSiDet_2Down_Strip_4",
logicSupportSiliconDetector_2Downvide,
false,
                                                             0);
logicSiDet_2Down_Strip_4->SetVisAttributes(SiliconDetector_att);
  if(physSiDet_2Down_Strip_4 == NULL) {}

  // Strip n. 3
  G4LogicalVolume* logicSiDet_2Down_Strip_3 = new G4LogicalVolume(SiDet_Strip_3,
materialSiliconDetector,
"logicSi_2Down_Det_Strip_3",
0, 0, 0);
  G4VPhysicalVolume* physSiDet_2Down_Strip_3 = new G4PVPlacement(0,
G4ThreeVector(0, -pDy1/2 + spazio_tra_Bordo_e_strip5 + y_SiDet_Strip_5 + spazio_tra_Strip + y_SiDet_Strip_4 + spazio_tra_Strip + y_SiDet_Strip_3/2, 0.),
logicSiDet_2Down_Strip_3,"logicSiDet_2Down_Strip_3",
logicSupportSiliconDetector_2Downvide,
false,
0);
logicSiDet_2Down_Strip_3->SetVisAttributes(SiliconDetector_att);
  if(physSiDet_2Down_Strip_3 == NULL) {}

  // Strip n. 2
  G4LogicalVolume* logicSiDet_2Down_Strip_2 = new G4LogicalVolume(SiDet_Strip_2,
materialSiliconDetector,
"logicSi_2Down_Det_Strip_2",
0, 0, 0);
  G4VPhysicalVolume* physSiDet_2Down_Strip_2 = new G4PVPlacement(0,
G4ThreeVector(0, -pDy1/2 + spazio_tra_Bordo_e_strip5 + y_SiDet_Strip_5 + spazio_tra_Strip + y_SiDet_Strip_4 + spazio_tra_Strip + y_SiDet_Strip_3 + spazio_tra_Strip + y_SiDet_Strip_2/2, 0.),
logicSiDet_2Down_Strip_2,"logicSiDet_2Down_Strip_2",
logicSupportSiliconDetector_2Downvide,
false,
0);
logicSiDet_2Down_Strip_2->SetVisAttributes(SiliconDetector_att);
  if(physSiDet_2Down_Strip_2 == NULL) {}

  // Strip n. 1
  G4LogicalVolume* logicSiDet_2Down_Strip_1 = new G4LogicalVolume(SiDet_Strip_1,
materialSiliconDetector,
"logicSi_2Down_Det_Strip_1",
0, 0, 0);
  G4VPhysicalVolume* physSiDet_2Down_Strip_1 = new G4PVPlacement(0,
G4ThreeVector(0, -pDy1/2 + spazio_tra_Bordo_e_strip5 + y_SiDet_Strip_5 + spazio_tra_Strip + y_SiDet_Strip_4 + spazio_tra_Strip + y_SiDet_Strip_3 + spazio_tra_Strip + y_SiDet_Strip_2 + spazio_tra_Strip + y_SiDet_Strip_1/2, 0.),
logicSiDet_2Down_Strip_1,"logicSiDet_2Down_Strip_1",
logicSupportSiliconDetector_2Downvide,
false,
0);
logicSiDet_2Down_Strip_1->SetVisAttributes(SiliconDetector_att);
  if(physSiDet_2Down_Strip_1 == NULL) {}

  // SUPPORTO IN RAME
  G4LogicalVolume* logic_Box_material_SupportoRame_SiliconDetector_2Down   = new G4LogicalVolume(Box_material_SupportoRame_SiliconDetector, material_SupportoRame_SiliconDetector, "logic_Box_material_SupportoRame_SiliconDetector_2Down"); //solid, material, name
  G4VPhysicalVolume* physics_Box_material_SupportoRame_SiliconDetector_2Down = new G4PVPlacement(0,                //no rotation
G4ThreeVector((length_x_SupportoRame_SiDetector/2 - x_smallBox_daTagliare_SupportoRame_SiDetector - distanza_latoDxBoxTagliata_e_bordoDxSupportoRame_SiDetector - (y_smallBox_daTagliare_SupportoRame_SiDetector/tan(thetaInclinazione_SiDetector) -1.*mm)), pDy1/2 - height_y_SupportoRame_SiDetector/2, -pDz/2 -thickness_z_SupportoRame_SiDetector/2),    //position, gi� negativa
logic_Box_material_SupportoRame_SiliconDetector_2Down, "logic_Box_material_SupportoRame_SiliconDetector_2Down", //its fLogical volume
logicSupportSiliconDetector_2Down,            //its mother volume
false,                //no boolean op.
0);                //copy nb.
logic_Box_material_SupportoRame_SiliconDetector_2Down->SetVisAttributes(Box_material_SupportoRame_SiliconDetector_att);
  if(physics_Box_material_SupportoRame_SiliconDetector_2Down == NULL) {}

  /////////////////Dead Layer 2Down
  //Strip n. 5
  G4LogicalVolume* logicSiDet_2Down_Strip_5_dl = new G4LogicalVolume(SiDet_Strip_5_dl,
    materialSiliconDetector,
    "logicSiDet_2Down_Strip_5_dl",                                               0, 0, 0);
  G4VPhysicalVolume* physSiDet_2Down_Strip_5_dl = new G4PVPlacement(0,
    G4ThreeVector(0., 0., thicknessSiDetector/2 - thicknessSiDetectorDeadLayer/2),
    logicSiDet_2Down_Strip_5_dl,"logicSiDet_2Down_Strip_5_dl",
    logicSiDet_2Down_Strip_5,
    false,
    0);

  //Strip n. 4
  G4LogicalVolume* logicSiDet_2Down_Strip_4_dl = new G4LogicalVolume(SiDet_Strip_4_dl,
    materialSiliconDetector,
    "logicSiDet_2Down_Strip_4_dl",                                              0, 0, 0);
  G4VPhysicalVolume* physSiDet_2Down_Strip_4_dl = new G4PVPlacement(0,
    G4ThreeVector(0., 0., thicknessSiDetector/2 - thicknessSiDetectorDeadLayer/2),
    logicSiDet_2Down_Strip_4_dl,"logicSiDet_2Down_Strip_4_dl",
    logicSiDet_2Down_Strip_4,
    false,
    0);

  //Strip n. 3
  G4LogicalVolume* logicSiDet_2Down_Strip_3_dl = new G4LogicalVolume(SiDet_Strip_3_dl,
    materialSiliconDetector,
    "logicSiDet_2Down_Strip_3_dl",                                               0, 0, 0);
  G4VPhysicalVolume* physSiDet_2Down_Strip_3_dl = new G4PVPlacement(0,
    G4ThreeVector(0., 0., thicknessSiDetector/2 - thicknessSiDetectorDeadLayer/2),
    logicSiDet_2Down_Strip_3_dl,"logicSiDet_2Down_Strip_3_dl",
    logicSiDet_2Down_Strip_3,
    false,
    0);

  //Strip n. 2
  G4LogicalVolume* logicSiDet_2Down_Strip_2_dl = new G4LogicalVolume(SiDet_Strip_2_dl,
    materialSiliconDetector,
    "logicSiDet_2Down_Strip_2_dl",                                               0, 0, 0);
  G4VPhysicalVolume* physSiDet_2Down_Strip_2_dl = new G4PVPlacement(0,
    G4ThreeVector(0., 0., thicknessSiDetector/2 - thicknessSiDetectorDeadLayer/2),
    logicSiDet_2Down_Strip_2_dl,"logicSiDet_2Down_Strip_2_dl",
    logicSiDet_2Down_Strip_2,
    false,
    0);

//Strip n. 1
G4LogicalVolume* logicSiDet_2Down_Strip_1_dl = new G4LogicalVolume(SiDet_Strip_1_dl,
  materialSiliconDetector,
  "logicSiDet_2Down_Strip_1_dl",                                               0, 0, 0);
G4VPhysicalVolume* physSiDet_2Down_Strip_1_dl = new G4PVPlacement(0,
  G4ThreeVector(0., 0., thicknessSiDetector/2 - thicknessSiDetectorDeadLayer/2),
  logicSiDet_2Down_Strip_1_dl,"logicSiDet_2Down_Strip_1_dl",
  logicSiDet_2Down_Strip_1,
  false,
  0);

  if(physSiDet_2Down_Strip_5_dl == NULL) {}
  if(physSiDet_2Down_Strip_4_dl == NULL) {}
  if(physSiDet_2Down_Strip_3_dl == NULL) {}
  if(physSiDet_2Down_Strip_2_dl == NULL) {}
  if(physSiDet_2Down_Strip_1_dl == NULL) {}



//========================================================================================================================
  //=================================================== DETECTOR 3Down  ========================================================
//========================================================================================================================
  G4RotationMatrix* rotationMatrix_3Down = new G4RotationMatrix();
rotationMatrix_3Down->rotateX(angle_between_xAxis_AND_zAxis);
  rotationMatrix_3Down->rotateY(0.*deg);
  rotationMatrix_3Down->rotateZ(0.*deg);

  G4LogicalVolume* logicSupportSiliconDetector_3Down = new G4LogicalVolume(supportSiliconDetector,
materialSupportSiliconDetector,
"logicSupportSiliconDetector_3Down",
                                                      0, 0, 0);
  G4VPhysicalVolume* physSupportSiliconDetector_3Down = new G4PVPlacement(rotationMatrix_3Down,
G4ThreeVector(0, -fRadius_PlasticScintillator - spazio_tra_Scintillatore_e_BordoSiDetector - pDy1/2  + (pDy1/2*(1-cos(angle_between_xAxis_AND_zAxis))), -z_height_Source_biggerBaseSiDet_inVerticale - pDz/2 - ((pDy1/2)*sin(thetaInclinazione_SiDetector))+offset), // ultima espressione implementata nella coord z per correggere per effetto di rotazione risp al baricentro (sempre la solita vecchia storia)
logicSupportSiliconDetector_3Down,"physSupportSiliconDetector_3Down",
                                                   fLogicWorld,
                                                   false,
                                                   0);
logicSupportSiliconDetector_3Down->SetVisAttributes(SupportSiliconDetector_att);
 if(physSupportSiliconDetector_3Down == NULL) {}

 ///////
 G4LogicalVolume* logicSupportSiliconDetector_3Downvide = new G4LogicalVolume(supportSiliconDetectorvide,
 vide,
 "logicSupportSiliconDetector_3Downvide",
                                                     0, 0, 0);
 G4VPhysicalVolume* physSupportSiliconDetector_3Downvide = new G4PVPlacement(rotationMatrix_3Down,
 G4ThreeVector(0, -fRadius_PlasticScintillator - spazio_tra_Scintillatore_e_BordoSiDetector - pDy1/2  + (pDy1/2*(1-cos(angle_between_xAxis_AND_zAxis))) - (pDz+thicknessSiDetector)/2*-sin(angle_between_xAxis_AND_zAxis), +(pDz+thicknessSiDetector)/2*cos(angle_between_xAxis_AND_zAxis)-z_height_Source_biggerBaseSiDet_inVerticale - pDz/2 - ((pDy1/2)*sin(thetaInclinazione_SiDetector))+offset), // ultima espressione implementata nella coord z per correggere per effetto di rotazione risp al baricentro (sempre la solita vecchia storia)
 logicSupportSiliconDetector_3Downvide,"logicSupportSiliconDetector_3Downvide",
                                                  fLogicWorld,
                                                  false,
                                                  0);
 logicSupportSiliconDetector_3Downvide->SetVisAttributes(Box_material_SupportoRame_SiliconDetector_attvide);
 if(physSupportSiliconDetector_3Downvide == NULL) {}
 /////////////

  // STRIP 5
  G4LogicalVolume* logicSiDet_3Down_Strip_5 = new G4LogicalVolume(SiDet_Strip_5,
materialSiliconDetector,
"logicSiDet_3Down_Strip_5",
                                               0, 0, 0);
  G4VPhysicalVolume* physSiDet_3Down_Strip_5 = new G4PVPlacement(0,
                                           G4ThreeVector(0, -pDy1/2 + spazio_tra_Bordo_e_strip5 + y_SiDet_Strip_5/2, 0.),
logicSiDet_3Down_Strip_5,"logicSiDet_3Down_Strip_5",
logicSupportSiliconDetector_3Downvide,
                                           false,
                                           0);
logicSiDet_3Down_Strip_5->SetVisAttributes(SiliconDetector_att);
 if(physSiDet_3Down_Strip_5 == NULL) {}


 // Strip n. 4
 G4LogicalVolume* logicSiDet_3Down_Strip_4 = new G4LogicalVolume(SiDet_Strip_4,
materialSiliconDetector,
"logicSi_3Down_Det_Strip_4",
                                                              0, 0, 0);
 G4VPhysicalVolume* physSiDet_3Down_Strip_4 = new G4PVPlacement(0,
G4ThreeVector(0, -pDy1/2 + spazio_tra_Bordo_e_strip5 + y_SiDet_Strip_5 + spazio_tra_Strip + y_SiDet_Strip_4/2, 0.),
logicSiDet_3Down_Strip_4,"logicSiDet_3Down_Strip_4",
logicSupportSiliconDetector_3Downvide,
false,
                                                             0);
logicSiDet_3Down_Strip_4->SetVisAttributes(SiliconDetector_att);
 if(physSiDet_3Down_Strip_4 == NULL) {}

  // Strip n. 3
  G4LogicalVolume* logicSiDet_3Down_Strip_3 = new G4LogicalVolume(SiDet_Strip_3,
materialSiliconDetector,
"logicSi_3Down_Det_Strip_3",
0, 0, 0);
  G4VPhysicalVolume* physSiDet_3Down_Strip_3 = new G4PVPlacement(0,
G4ThreeVector(0, -pDy1/2 + spazio_tra_Bordo_e_strip5 + y_SiDet_Strip_5 + spazio_tra_Strip + y_SiDet_Strip_4 + spazio_tra_Strip + y_SiDet_Strip_3/2, 0.),
logicSiDet_3Down_Strip_3,"logicSiDet_3Down_Strip_3",
logicSupportSiliconDetector_3Downvide,
false,
0);
logicSiDet_3Down_Strip_3->SetVisAttributes(SiliconDetector_att);
  if(physSiDet_3Down_Strip_3 == NULL) {}

  // Strip n. 2
  G4LogicalVolume* logicSiDet_3Down_Strip_2 = new G4LogicalVolume(SiDet_Strip_2,
materialSiliconDetector,
"logicSi_3Down_Det_Strip_2",
0, 0, 0);
  G4VPhysicalVolume* physSiDet_3Down_Strip_2 = new G4PVPlacement(0,
G4ThreeVector(0, -pDy1/2 + spazio_tra_Bordo_e_strip5 + y_SiDet_Strip_5 + spazio_tra_Strip + y_SiDet_Strip_4 + spazio_tra_Strip + y_SiDet_Strip_3 + spazio_tra_Strip + y_SiDet_Strip_2/2, 0.),
logicSiDet_3Down_Strip_2,"logicSiDet_3Down_Strip_2",
logicSupportSiliconDetector_3Downvide,
false,
0);
logicSiDet_3Down_Strip_2->SetVisAttributes(SiliconDetector_att);
  if(physSiDet_3Down_Strip_2 == NULL) {}

  // Strip n. 1
  G4LogicalVolume* logicSiDet_3Down_Strip_1 = new G4LogicalVolume(SiDet_Strip_1,
materialSiliconDetector,
"logicSi_3Down_Det_Strip_1",
0, 0, 0);
  G4VPhysicalVolume* physSiDet_3Down_Strip_1 = new G4PVPlacement(0,
G4ThreeVector(0, -pDy1/2 + spazio_tra_Bordo_e_strip5 + y_SiDet_Strip_5 + spazio_tra_Strip + y_SiDet_Strip_4 + spazio_tra_Strip + y_SiDet_Strip_3 + spazio_tra_Strip + y_SiDet_Strip_2 + spazio_tra_Strip + y_SiDet_Strip_1/2, 0.),
logicSiDet_3Down_Strip_1,"logicSiDet_3Down_Strip_1",
logicSupportSiliconDetector_3Downvide,
false,
0);
logicSiDet_3Down_Strip_1->SetVisAttributes(SiliconDetector_att);
  if(physSiDet_3Down_Strip_1 == NULL) {}

  // SUPPORTO IN RAME
  G4LogicalVolume* logic_Box_material_SupportoRame_SiliconDetector_3Down   = new G4LogicalVolume(Box_material_SupportoRame_SiliconDetector, material_SupportoRame_SiliconDetector, "logic_Box_material_SupportoRame_SiliconDetector_3Down"); //solid, material, name
  G4VPhysicalVolume* physics_Box_material_SupportoRame_SiliconDetector_3Down = new G4PVPlacement(0,                //no rotation
G4ThreeVector((length_x_SupportoRame_SiDetector/2 - x_smallBox_daTagliare_SupportoRame_SiDetector - distanza_latoDxBoxTagliata_e_bordoDxSupportoRame_SiDetector - (y_smallBox_daTagliare_SupportoRame_SiDetector/tan(thetaInclinazione_SiDetector) -1.*mm)), pDy1/2 - height_y_SupportoRame_SiDetector/2, -pDz/2 -thickness_z_SupportoRame_SiDetector/2),    //position, gi� negativa
logic_Box_material_SupportoRame_SiliconDetector_3Down, "logic_Box_material_SupportoRame_SiliconDetector_3Down", //its fLogical volume
logicSupportSiliconDetector_3Down,            //its mother volume
false,                //no boolean op.
0);                //copy nb.
logic_Box_material_SupportoRame_SiliconDetector_3Down->SetVisAttributes(Box_material_SupportoRame_SiliconDetector_att);
  if(physics_Box_material_SupportoRame_SiliconDetector_3Down == NULL) {}

  /////////////////Dead Layer 3Down
  //Strip n. 5
  G4LogicalVolume* logicSiDet_3Down_Strip_5_dl = new G4LogicalVolume(SiDet_Strip_5_dl,
    materialSiliconDetector,
    "logicSiDet_3Down_Strip_5_dl",                                               0, 0, 0);
  G4VPhysicalVolume* physSiDet_3Down_Strip_5_dl = new G4PVPlacement(0,
    G4ThreeVector(0., 0., thicknessSiDetector/2 - thicknessSiDetectorDeadLayer/2),
    logicSiDet_3Down_Strip_5_dl,"logicSiDet_3Down_Strip_5_dl",
    logicSiDet_3Down_Strip_5,
    false,
    0);

  //Strip n. 4
  G4LogicalVolume* logicSiDet_3Down_Strip_4_dl = new G4LogicalVolume(SiDet_Strip_4_dl,
    materialSiliconDetector,
    "logicSiDet_3Down_Strip_4_dl",                                              0, 0, 0);
  G4VPhysicalVolume* physSiDet_3Down_Strip_4_dl = new G4PVPlacement(0,
    G4ThreeVector(0., 0., thicknessSiDetector/2 - thicknessSiDetectorDeadLayer/2),
    logicSiDet_3Down_Strip_4_dl,"logicSiDet_3Down_Strip_4_dl",
    logicSiDet_3Down_Strip_4,
    false,
    0);

  //Strip n. 3
  G4LogicalVolume* logicSiDet_3Down_Strip_3_dl = new G4LogicalVolume(SiDet_Strip_3_dl,
    materialSiliconDetector,
    "logicSiDet_3Down_Strip_3_dl",                                               0, 0, 0);
  G4VPhysicalVolume* physSiDet_3Down_Strip_3_dl = new G4PVPlacement(0,
    G4ThreeVector(0., 0., thicknessSiDetector/2 - thicknessSiDetectorDeadLayer/2),
    logicSiDet_3Down_Strip_3_dl,"logicSiDet_3Down_Strip_3_dl",
    logicSiDet_3Down_Strip_3,
    false,
    0);

  //Strip n. 2
  G4LogicalVolume* logicSiDet_3Down_Strip_2_dl = new G4LogicalVolume(SiDet_Strip_2_dl,
    materialSiliconDetector,
    "logicSiDet_3Down_Strip_2_dl",                                               0, 0, 0);
  G4VPhysicalVolume* physSiDet_3Down_Strip_2_dl = new G4PVPlacement(0,
    G4ThreeVector(0., 0., thicknessSiDetector/2 - thicknessSiDetectorDeadLayer/2),
    logicSiDet_3Down_Strip_2_dl,"logicSiDet_3Down_Strip_2_dl",
    logicSiDet_3Down_Strip_2,
    false,
    0);

//Strip n. 1
G4LogicalVolume* logicSiDet_3Down_Strip_1_dl = new G4LogicalVolume(SiDet_Strip_1_dl,
  materialSiliconDetector,
  "logicSiDet_3Down_Strip_1_dl",                                               0, 0, 0);
G4VPhysicalVolume* physSiDet_3Down_Strip_1_dl = new G4PVPlacement(0,
  G4ThreeVector(0., 0., thicknessSiDetector/2 - thicknessSiDetectorDeadLayer/2),
  logicSiDet_3Down_Strip_1_dl,"logicSiDet_3Down_Strip_1_dl",
  logicSiDet_3Down_Strip_1,
  false,
  0);

  if(physSiDet_3Down_Strip_5_dl == NULL) {}
  if(physSiDet_3Down_Strip_4_dl == NULL) {}
  if(physSiDet_3Down_Strip_3_dl == NULL) {}
  if(physSiDet_3Down_Strip_2_dl == NULL) {}
  if(physSiDet_3Down_Strip_1_dl == NULL) {}


//========================================================================================================================
  //=================================================== DETECTOR 4Down  ========================================================
//========================================================================================================================
  G4RotationMatrix* rotationMatrix_4Down = new G4RotationMatrix();
  rotationMatrix_4Down->rotateX(180.*deg);
  rotationMatrix_4Down->rotateY(180.*deg + angle_between_xAxis_AND_zAxis);
  rotationMatrix_4Down->rotateZ(-90.*deg);

  G4LogicalVolume* logicSupportSiliconDetector_4Down = new G4LogicalVolume(supportSiliconDetector,
materialSupportSiliconDetector,
"logicSupportSiliconDetector_4Down",
0, 0, 0);
  G4VPhysicalVolume* physSupportSiliconDetector_4Down = new G4PVPlacement(rotationMatrix_4Down,
G4ThreeVector(-fRadius_PlasticScintillator - spazio_tra_Scintillatore_e_BordoSiDetector - pDy1/2  + (pDy1/2*(1-cos(angle_between_xAxis_AND_zAxis))), 0., -z_height_Source_biggerBaseSiDet_inVerticale - pDz/2 - ((pDy1/2)*sin(thetaInclinazione_SiDetector))+offset), // ultima espressione implementata nella coord z per correggere per effetto di rotazione risp al baricentro (sempre la solita vecchia storia)
logicSupportSiliconDetector_4Down,"physSupportSiliconDetector_4Down",
fLogicWorld,
false,
                                                                   0);
logicSupportSiliconDetector_4Down->SetVisAttributes(SupportSiliconDetector_att);
  if(physSupportSiliconDetector_4Down == NULL) {}

  ///////
  G4LogicalVolume* logicSupportSiliconDetector_4Downvide = new G4LogicalVolume(supportSiliconDetectorvide,
  vide,
  "logicSupportSiliconDetector_4Downvide",
                                                      0, 0, 0);
  G4VPhysicalVolume* physSupportSiliconDetector_4Downvide = new G4PVPlacement(rotationMatrix_4Down,
  G4ThreeVector(-fRadius_PlasticScintillator - spazio_tra_Scintillatore_e_BordoSiDetector - pDy1/2  + (pDy1/2*(1-cos(angle_between_xAxis_AND_zAxis))) - (pDz+thicknessSiDetector)/2*-sin(angle_between_xAxis_AND_zAxis), 0., +(pDz+thicknessSiDetector)/2*cos(angle_between_xAxis_AND_zAxis)-z_height_Source_biggerBaseSiDet_inVerticale - pDz/2 - ((pDy1/2)*sin(thetaInclinazione_SiDetector))+offset), // ultima espressione implementata nella coord z per correggere per effetto di rotazione risp al baricentro (sempre la solita vecchia storia)
  logicSupportSiliconDetector_4Downvide,"logicSupportSiliconDetector_4Downvide",
                                                   fLogicWorld,
                                                   false,
                                                   0);
  logicSupportSiliconDetector_4Downvide->SetVisAttributes(Box_material_SupportoRame_SiliconDetector_attvide);
  if(physSupportSiliconDetector_4Downvide == NULL) {}
  /////////////

  // STRIP 5
  G4LogicalVolume* logicSiDet_4Down_Strip_5 = new G4LogicalVolume(SiDet_Strip_5,
materialSiliconDetector,
"logicSiDet_4Down_Strip_5",
0, 0, 0);
  G4VPhysicalVolume* physSiDet_4Down_Strip_5 = new G4PVPlacement(0,
G4ThreeVector(0, -pDy1/2 + spazio_tra_Bordo_e_strip5 + y_SiDet_Strip_5/2, 0.),
logicSiDet_4Down_Strip_5,"logicSiDet_4Down_Strip_5",
logicSupportSiliconDetector_4Downvide,
false,
                                                           0);
logicSiDet_4Down_Strip_5->SetVisAttributes(SiliconDetector_att);
  if(physSiDet_4Down_Strip_5 == NULL) {}


  // Strip n. 4
  G4LogicalVolume* logicSiDet_4Down_Strip_4 = new G4LogicalVolume(SiDet_Strip_4,
materialSiliconDetector,
"logicSi_4Down_Det_Strip_4",
                                                              0, 0, 0);
  G4VPhysicalVolume* physSiDet_4Down_Strip_4 = new G4PVPlacement(0,
G4ThreeVector(0, -pDy1/2 + spazio_tra_Bordo_e_strip5 + y_SiDet_Strip_5 + spazio_tra_Strip + y_SiDet_Strip_4/2, 0.),
logicSiDet_4Down_Strip_4,"logicSiDet_4Down_Strip_4",
logicSupportSiliconDetector_4Downvide,
false,
                                                             0);
logicSiDet_4Down_Strip_4->SetVisAttributes(SiliconDetector_att);
  if(physSiDet_4Down_Strip_4 == NULL) {}

  // Strip n. 3
  G4LogicalVolume* logicSiDet_4Down_Strip_3 = new G4LogicalVolume(SiDet_Strip_3,
materialSiliconDetector,
"logicSi_4Down_Det_Strip_3",
0, 0, 0);
  G4VPhysicalVolume* physSiDet_4Down_Strip_3 = new G4PVPlacement(0,
G4ThreeVector(0, -pDy1/2 + spazio_tra_Bordo_e_strip5 + y_SiDet_Strip_5 + spazio_tra_Strip + y_SiDet_Strip_4 + spazio_tra_Strip + y_SiDet_Strip_3/2, 0.),
logicSiDet_4Down_Strip_3,"logicSiDet_4Down_Strip_3",
logicSupportSiliconDetector_4Downvide,
false,
0);
logicSiDet_4Down_Strip_3->SetVisAttributes(SiliconDetector_att);
  if(physSiDet_4Down_Strip_3 == NULL) {}

  // Strip n. 2
  //file:///usr/share/doc/HTML/en-US/index.html
  G4LogicalVolume* logicSiDet_4Down_Strip_2 = new G4LogicalVolume(SiDet_Strip_2,
materialSiliconDetector,
"logicSi_4Down_Det_Strip_2",
0, 0, 0);
  G4VPhysicalVolume* physSiDet_4Down_Strip_2 = new G4PVPlacement(0,
G4ThreeVector(0, -pDy1/2 + spazio_tra_Bordo_e_strip5 + y_SiDet_Strip_5 + spazio_tra_Strip + y_SiDet_Strip_4 + spazio_tra_Strip + y_SiDet_Strip_3 + spazio_tra_Strip + y_SiDet_Strip_2/2, 0.),
logicSiDet_4Down_Strip_2,"logicSiDet_4Down_Strip_2",
logicSupportSiliconDetector_4Downvide,
false,
0);
logicSiDet_4Down_Strip_2->SetVisAttributes(SiliconDetector_att);
  if(physSiDet_4Down_Strip_2 == NULL) {}

  // Strip n. 1
  //file:///usr/share/doc/HTML/en-US/index.html
  G4LogicalVolume* logicSiDet_4Down_Strip_1 = new G4LogicalVolume(SiDet_Strip_1,
materialSiliconDetector,
"logicSi_4Down_Det_Strip_1",
0, 0, 0);
  G4VPhysicalVolume* physSiDet_4Down_Strip_1 = new G4PVPlacement(0,
G4ThreeVector(0, -pDy1/2 + spazio_tra_Bordo_e_strip5 + y_SiDet_Strip_5 + spazio_tra_Strip + y_SiDet_Strip_4 + spazio_tra_Strip + y_SiDet_Strip_3 + spazio_tra_Strip + y_SiDet_Strip_2 + spazio_tra_Strip + y_SiDet_Strip_1/2, 0.),
logicSiDet_4Down_Strip_1,"logicSiDet_4Down_Strip_1",
logicSupportSiliconDetector_4Downvide,
false,
0);
logicSiDet_4Down_Strip_1->SetVisAttributes(SiliconDetector_att);
  if(physSiDet_4Down_Strip_1 == NULL) {}


  // SUPPORTO IN RAME
  ///usr/share/doc/HTML/en-US/index.html
  G4LogicalVolume* logic_Box_material_SupportoRame_SiliconDetector_4Down = new G4LogicalVolume(Box_material_SupportoRame_SiliconDetector, material_SupportoRame_SiliconDetector, "logic_Box_material_SupportoRame_SiliconDetector_4Down"); //solid, material, name
  G4VPhysicalVolume* physics_Box_material_SupportoRame_SiliconDetector_4Down = new G4PVPlacement(0,                //no rotation
G4ThreeVector((length_x_SupportoRame_SiDetector/2 - x_smallBox_daTagliare_SupportoRame_SiDetector - distanza_latoDxBoxTagliata_e_bordoDxSupportoRame_SiDetector - (y_smallBox_daTagliare_SupportoRame_SiDetector/tan(thetaInclinazione_SiDetector) -1.*mm)), pDy1/2 - height_y_SupportoRame_SiDetector/2, -pDz/2 -thickness_z_SupportoRame_SiDetector/2),    //position, gi� negativa
logic_Box_material_SupportoRame_SiliconDetector_4Down, "logic_Box_material_SupportoRame_SiliconDetector_4Down", //its fLogical volume
logicSupportSiliconDetector_4Down,            //its mother volume
false,                //no boolean op.
0);                //copy nb.
logic_Box_material_SupportoRame_SiliconDetector_4Down->SetVisAttributes(Box_material_SupportoRame_SiliconDetector_att);
  if(physics_Box_material_SupportoRame_SiliconDetector_4Down == NULL) {}

  /////////////////Dead Layer 4Down
  //Strip n. 5
  G4LogicalVolume* logicSiDet_4Down_Strip_5_dl = new G4LogicalVolume(SiDet_Strip_5_dl,
    materialSiliconDetector,
    "logicSiDet_4Down_Strip_5_dl",                                               0, 0, 0);
  G4VPhysicalVolume* physSiDet_4Down_Strip_5_dl = new G4PVPlacement(0,
    G4ThreeVector(0., 0., thicknessSiDetector/2 - thicknessSiDetectorDeadLayer/2),
    logicSiDet_4Down_Strip_5_dl,"logicSiDet_4Down_Strip_5_dl",
    logicSiDet_4Down_Strip_5,
    false,
    0);

  //Strip n. 4
  G4LogicalVolume* logicSiDet_4Down_Strip_4_dl = new G4LogicalVolume(SiDet_Strip_4_dl,
    materialSiliconDetector,
    "logicSiDet_4Down_Strip_4_dl",                                              0, 0, 0);
  G4VPhysicalVolume* physSiDet_4Down_Strip_4_dl = new G4PVPlacement(0,
    G4ThreeVector(0., 0., thicknessSiDetector/2 - thicknessSiDetectorDeadLayer/2),
    logicSiDet_4Down_Strip_4_dl,"logicSiDet_4Down_Strip_4_dl",
    logicSiDet_4Down_Strip_4,
    false,
    0);

  //Strip n. 3
  G4LogicalVolume* logicSiDet_4Down_Strip_3_dl = new G4LogicalVolume(SiDet_Strip_3_dl,
    materialSiliconDetector,
    "logicSiDet_4Down_Strip_3_dl",                                               0, 0, 0);
  G4VPhysicalVolume* physSiDet_4Down_Strip_3_dl = new G4PVPlacement(0,
    G4ThreeVector(0., 0., thicknessSiDetector/2 - thicknessSiDetectorDeadLayer/2),
    logicSiDet_4Down_Strip_3_dl,"logicSiDet_4Down_Strip_3_dl",
    logicSiDet_4Down_Strip_3,
    false,
    0);

  //Strip n. 2
  G4LogicalVolume* logicSiDet_4Down_Strip_2_dl = new G4LogicalVolume(SiDet_Strip_2_dl,
    materialSiliconDetector,
    "logicSiDet_4Down_Strip_2_dl",                                               0, 0, 0);
  G4VPhysicalVolume* physSiDet_4Down_Strip_2_dl = new G4PVPlacement(0,
    G4ThreeVector(0., 0., thicknessSiDetector/2 - thicknessSiDetectorDeadLayer/2),
    logicSiDet_4Down_Strip_2_dl,"logicSiDet_4Down_Strip_2_dl",
    logicSiDet_4Down_Strip_2,
    false,
    0);

//Strip n. 1
G4LogicalVolume* logicSiDet_4Down_Strip_1_dl = new G4LogicalVolume(SiDet_Strip_1_dl,
  materialSiliconDetector,
  "logicSiDet_4Down_Strip_1_dl",                                               0, 0, 0);
G4VPhysicalVolume* physSiDet_4Down_Strip_1_dl = new G4PVPlacement(0,
  G4ThreeVector(0., 0., thicknessSiDetector/2 - thicknessSiDetectorDeadLayer/2),
  logicSiDet_4Down_Strip_1_dl,"logicSiDet_4Down_Strip_1_dl",
  logicSiDet_4Down_Strip_1,
  false,
  0);

  if(physSiDet_4Down_Strip_5_dl == NULL) {}
  if(physSiDet_4Down_Strip_4_dl == NULL) {}
  if(physSiDet_4Down_Strip_3_dl == NULL) {}
  if(physSiDet_4Down_Strip_2_dl == NULL) {}
  if(physSiDet_4Down_Strip_1_dl == NULL) {}

//===================================================================================================================================================================
//===================================================================================================================================================================
  //==================================================   WISArD PLASTIC SCINTILLATOR - WITH SUPPORT ===================================================================
//===================================================================================================================================================================
//===================================================================================================================================================================
  //NB: per ogni dubbio sulle misure, consultare i disegni tecnici inviati da M. Roche (mail al mio indirizzo cenbg il 18/05/2021).
  G4double distanza_tra_BaseInfScintillatore_e_BordoSuperioreDeiSiDetector = 25.575*mm;


  G4Material* fMaterial_PlasticScintillator = G4NistManager::Instance()->FindOrBuildMaterial("G4_POLYSTYRENE");

  G4Tubs*          fSolid_PlasticScintillator = new G4Tubs("PlasticScintillator",0.,fRadius_PlasticScintillator,0.5*fLength_PlasticScintillator,0.,360*deg);//name, r : 0->1cm, L : 5cm, phi : 0->2pi
  G4LogicalVolume* fLogic_PlasticScintillator = new G4LogicalVolume(fSolid_PlasticScintillator,fMaterial_PlasticScintillator,"PlasticScintillator"); //solid, material, name
  G4PVPlacement*   fPhys_PlasticScintillator  = new G4PVPlacement(0,        //rotationMatrix
G4ThreeVector(0., 0., z_height_Source_biggerBaseSiDet_inVerticale + distanza_tra_BaseInfScintillatore_e_BordoSuperioreDeiSiDetector + fLength_PlasticScintillator/2-offset),    //position
fLogic_PlasticScintillator,"PlasticScintillator",    //its fLogical volume
fLogicWorld,              //its mother volume
false,                      //no boolean op.
0);                         //copy nb.


  G4VisAttributes* PlasticScintillator_att = new G4VisAttributes(G4Colour(0., 0., 0.));//red
  PlasticScintillator_att->SetForceWireframe(false);
  PlasticScintillator_att->SetForceSolid(true);
fLogic_PlasticScintillator->SetVisAttributes(PlasticScintillator_att);

  if(fPhys_PlasticScintillator == NULL) {}

  G4double length_x_SupportoRame_PlasticScint                            = 20.5*mm;
  G4double height_y_SupportoRame_PlasticScint                            = 20.5*mm;
  G4double thickness_z_SupportoRame_PlasticScint                         = 1.5*mm;

  G4Material* material_SupportoRame_PlasticScint        = G4NistManager::Instance()->FindOrBuildMaterial("G4_Cu");
  G4VSolid*   bigBox_material_SupportoRame_PlasticScint = new G4Box("bigBox_material_SupportoRame_PlasticScint" , length_x_SupportoRame_PlasticScint, height_y_SupportoRame_PlasticScint, thickness_z_SupportoRame_PlasticScint/2); //corretto non div per due x e y
  G4VSolid*   holeForPlasticScint                       = new G4Tubs("holeForPlasticScint", 0., fRadius_PlasticScintillator, thickness_z_SupportoRame_PlasticScint, 0., 360*deg);

  G4VSolid*   Box_material_SupportoRame_PlasticScint    = new G4SubtractionSolid("Box_material_SupportoRame_PlasticScint",
bigBox_material_SupportoRame_PlasticScint, holeForPlasticScint,
0,
G4ThreeVector(0., 0., 0.));

  G4LogicalVolume* logic_Box_material_SupportoRame_PlasticScint   = new G4LogicalVolume(Box_material_SupportoRame_PlasticScint, material_SupportoRame_PlasticScint, "logic_Box_material_SupportoRame_PlasticScint"); //solid, material, name
  G4VPhysicalVolume* physics_Box_material_SupportoRame_PlasticScint = new G4PVPlacement(0,                //no rotation
G4ThreeVector(0., 0., -fLength_PlasticScintillator/2 + thickness_z_SupportoRame_SiDetector/2),
logic_Box_material_SupportoRame_PlasticScint, "logic_Box_material_SupportoRame_PlasticScint",        //its fLogical volume
fLogic_PlasticScintillator,            //its mother volume
false,                //no boolean op.
0);                //copy nb.



  G4VisAttributes* Box_material_SupportoRame_PlasticScint_att = new G4VisAttributes(G4Colour(0.7, 0, 0.7));//(r,g,b) => magenta
Box_material_SupportoRame_PlasticScint_att->SetForceWireframe(false);
Box_material_SupportoRame_PlasticScint_att->SetForceSolid(true);
logic_Box_material_SupportoRame_PlasticScint->SetVisAttributes(Box_material_SupportoRame_PlasticScint_att);

  if(physics_Box_material_SupportoRame_PlasticScint == NULL) {}

  G4double lenghtPlaccaCilidrica = fLength_PlasticScintillator - thickness_z_SupportoRame_SiDetector;

  G4VSolid*   placcaRame_Cilindrica                 = new G4Tubs("placcaRame_Cilindrica", fRadius_PlasticScintillator, fRadius_PlasticScintillator+thickness_z_SupportoRame_PlasticScint, lenghtPlaccaCilidrica/2, 0., 360*deg);
  G4LogicalVolume*    logic_placcaRame_Cilindrica   = new G4LogicalVolume(placcaRame_Cilindrica, material_SupportoRame_PlasticScint, "placcaRame_Cilindrica"); //solid, material, name
  G4VPhysicalVolume*  physics_placcaRame_Cilindrica = new G4PVPlacement(0,                //no rotation
G4ThreeVector(0., 0.,-fLength_PlasticScintillator/2 + thickness_z_SupportoRame_SiDetector/2 + lenghtPlaccaCilidrica/2),
logic_placcaRame_Cilindrica, "logic_placcaRame_Cilindrica", //its fLogical volume
fLogic_PlasticScintillator,            //its mother volume
false,                //no boolean op.
0);

logic_placcaRame_Cilindrica->SetVisAttributes(Box_material_SupportoRame_PlasticScint_att);
  if(physics_placcaRame_Cilindrica == NULL) {}

fLogic_PlasticScintillator->SetSensitiveDetector( manager_ptr->GetWisardSensor_PlasticScintillator() );

logicSiDet_1Up_Strip_1->SetSensitiveDetector( manager_ptr->GetWisardSensor_1Up_Strip_1() );
logicSiDet_1Up_Strip_2->SetSensitiveDetector( manager_ptr->GetWisardSensor_1Up_Strip_2() );
logicSiDet_1Up_Strip_3->SetSensitiveDetector( manager_ptr->GetWisardSensor_1Up_Strip_3() );
logicSiDet_1Up_Strip_4->SetSensitiveDetector( manager_ptr->GetWisardSensor_1Up_Strip_4() );
logicSiDet_1Up_Strip_5->SetSensitiveDetector( manager_ptr->GetWisardSensor_1Up_Strip_5() );
logicSupportSiliconDetector_1Up->SetSensitiveDetector( manager_ptr->GetWisardSensor_1Up_Support() );

logicSiDet_2Up_Strip_1->SetSensitiveDetector( manager_ptr->GetWisardSensor_2Up_Strip_1() );
logicSiDet_2Up_Strip_2->SetSensitiveDetector( manager_ptr->GetWisardSensor_2Up_Strip_2() );
logicSiDet_2Up_Strip_3->SetSensitiveDetector( manager_ptr->GetWisardSensor_2Up_Strip_3() );
logicSiDet_2Up_Strip_4->SetSensitiveDetector( manager_ptr->GetWisardSensor_2Up_Strip_4() );
logicSiDet_2Up_Strip_5->SetSensitiveDetector( manager_ptr->GetWisardSensor_2Up_Strip_5() );
logicSupportSiliconDetector_2Up->SetSensitiveDetector( manager_ptr->GetWisardSensor_2Up_Support() );

logicSiDet_3Up_Strip_1->SetSensitiveDetector( manager_ptr->GetWisardSensor_3Up_Strip_1() );
logicSiDet_3Up_Strip_2->SetSensitiveDetector( manager_ptr->GetWisardSensor_3Up_Strip_2() );
logicSiDet_3Up_Strip_3->SetSensitiveDetector( manager_ptr->GetWisardSensor_3Up_Strip_3() );
logicSiDet_3Up_Strip_4->SetSensitiveDetector( manager_ptr->GetWisardSensor_3Up_Strip_4() );
logicSiDet_3Up_Strip_5->SetSensitiveDetector( manager_ptr->GetWisardSensor_3Up_Strip_5() );
logicSupportSiliconDetector_3Up->SetSensitiveDetector( manager_ptr->GetWisardSensor_3Up_Support() );

logicSiDet_4Up_Strip_1->SetSensitiveDetector( manager_ptr->GetWisardSensor_4Up_Strip_1() );
logicSiDet_4Up_Strip_2->SetSensitiveDetector( manager_ptr->GetWisardSensor_4Up_Strip_2() );
logicSiDet_4Up_Strip_3->SetSensitiveDetector( manager_ptr->GetWisardSensor_4Up_Strip_3() );
logicSiDet_4Up_Strip_4->SetSensitiveDetector( manager_ptr->GetWisardSensor_4Up_Strip_4() );
logicSiDet_4Up_Strip_5->SetSensitiveDetector( manager_ptr->GetWisardSensor_4Up_Strip_5() );
logicSupportSiliconDetector_4Up->SetSensitiveDetector( manager_ptr->GetWisardSensor_4Up_Support() );

logicSiDet_1Down_Strip_1->SetSensitiveDetector( manager_ptr->GetWisardSensor_1Down_Strip_1() );
logicSiDet_1Down_Strip_2->SetSensitiveDetector( manager_ptr->GetWisardSensor_1Down_Strip_2() );
logicSiDet_1Down_Strip_3->SetSensitiveDetector( manager_ptr->GetWisardSensor_1Down_Strip_3() );
logicSiDet_1Down_Strip_4->SetSensitiveDetector( manager_ptr->GetWisardSensor_1Down_Strip_4() );
logicSiDet_1Down_Strip_5->SetSensitiveDetector( manager_ptr->GetWisardSensor_1Down_Strip_5() );
logicSupportSiliconDetector_1Down->SetSensitiveDetector( manager_ptr->GetWisardSensor_1Down_Support() );

logicSiDet_2Down_Strip_1->SetSensitiveDetector( manager_ptr->GetWisardSensor_2Down_Strip_1() );
logicSiDet_2Down_Strip_2->SetSensitiveDetector( manager_ptr->GetWisardSensor_2Down_Strip_2() );
logicSiDet_2Down_Strip_3->SetSensitiveDetector( manager_ptr->GetWisardSensor_2Down_Strip_3() );
logicSiDet_2Down_Strip_4->SetSensitiveDetector( manager_ptr->GetWisardSensor_2Down_Strip_4() );
logicSiDet_2Down_Strip_5->SetSensitiveDetector( manager_ptr->GetWisardSensor_2Down_Strip_5() );
logicSupportSiliconDetector_2Down->SetSensitiveDetector( manager_ptr->GetWisardSensor_2Down_Support() );

logicSiDet_3Down_Strip_1->SetSensitiveDetector( manager_ptr->GetWisardSensor_3Down_Strip_1() );
logicSiDet_3Down_Strip_2->SetSensitiveDetector( manager_ptr->GetWisardSensor_3Down_Strip_2() );
logicSiDet_3Down_Strip_3->SetSensitiveDetector( manager_ptr->GetWisardSensor_3Down_Strip_3() );
logicSiDet_3Down_Strip_4->SetSensitiveDetector( manager_ptr->GetWisardSensor_3Down_Strip_4() );
logicSiDet_3Down_Strip_5->SetSensitiveDetector( manager_ptr->GetWisardSensor_3Down_Strip_5() );
logicSupportSiliconDetector_3Down->SetSensitiveDetector( manager_ptr->GetWisardSensor_3Down_Support() );

logicSiDet_4Down_Strip_1->SetSensitiveDetector( manager_ptr->GetWisardSensor_4Down_Strip_1() );
logicSiDet_4Down_Strip_2->SetSensitiveDetector( manager_ptr->GetWisardSensor_4Down_Strip_2() );
logicSiDet_4Down_Strip_3->SetSensitiveDetector( manager_ptr->GetWisardSensor_4Down_Strip_3() );
logicSiDet_4Down_Strip_4->SetSensitiveDetector( manager_ptr->GetWisardSensor_4Down_Strip_4() );
logicSiDet_4Down_Strip_5->SetSensitiveDetector( manager_ptr->GetWisardSensor_4Down_Strip_5() );
logicSupportSiliconDetector_4Down->SetSensitiveDetector( manager_ptr->GetWisardSensor_4Down_Support() );

//dl
logicSiDet_1Up_Strip_1_dl->SetSensitiveDetector( manager_ptr->GetWisardSensor_1Up_Strip_1_dl() );
logicSiDet_1Up_Strip_2_dl->SetSensitiveDetector( manager_ptr->GetWisardSensor_1Up_Strip_2_dl() );
logicSiDet_1Up_Strip_3_dl->SetSensitiveDetector( manager_ptr->GetWisardSensor_1Up_Strip_3_dl() );
logicSiDet_1Up_Strip_4_dl->SetSensitiveDetector( manager_ptr->GetWisardSensor_1Up_Strip_4_dl() );
logicSiDet_1Up_Strip_5_dl->SetSensitiveDetector( manager_ptr->GetWisardSensor_1Up_Strip_5_dl() );

logicSiDet_2Up_Strip_1_dl->SetSensitiveDetector( manager_ptr->GetWisardSensor_2Up_Strip_1_dl() );
logicSiDet_2Up_Strip_2_dl->SetSensitiveDetector( manager_ptr->GetWisardSensor_2Up_Strip_2_dl() );
logicSiDet_2Up_Strip_3_dl->SetSensitiveDetector( manager_ptr->GetWisardSensor_2Up_Strip_3_dl() );
logicSiDet_2Up_Strip_4_dl->SetSensitiveDetector( manager_ptr->GetWisardSensor_2Up_Strip_4_dl() );
logicSiDet_2Up_Strip_5_dl->SetSensitiveDetector( manager_ptr->GetWisardSensor_2Up_Strip_5_dl() );

logicSiDet_3Up_Strip_1_dl->SetSensitiveDetector( manager_ptr->GetWisardSensor_3Up_Strip_1_dl() );
logicSiDet_3Up_Strip_2_dl->SetSensitiveDetector( manager_ptr->GetWisardSensor_3Up_Strip_2_dl() );
logicSiDet_3Up_Strip_3_dl->SetSensitiveDetector( manager_ptr->GetWisardSensor_3Up_Strip_3_dl() );
logicSiDet_3Up_Strip_4_dl->SetSensitiveDetector( manager_ptr->GetWisardSensor_3Up_Strip_4_dl() );
logicSiDet_3Up_Strip_5_dl->SetSensitiveDetector( manager_ptr->GetWisardSensor_3Up_Strip_5_dl() );

logicSiDet_4Up_Strip_1_dl->SetSensitiveDetector( manager_ptr->GetWisardSensor_4Up_Strip_1_dl() );
logicSiDet_4Up_Strip_2_dl->SetSensitiveDetector( manager_ptr->GetWisardSensor_4Up_Strip_2_dl() );
logicSiDet_4Up_Strip_3_dl->SetSensitiveDetector( manager_ptr->GetWisardSensor_4Up_Strip_3_dl() );
logicSiDet_4Up_Strip_4_dl->SetSensitiveDetector( manager_ptr->GetWisardSensor_4Up_Strip_4_dl() );
logicSiDet_4Up_Strip_5_dl->SetSensitiveDetector( manager_ptr->GetWisardSensor_4Up_Strip_5_dl() );

logicSiDet_1Down_Strip_1_dl->SetSensitiveDetector( manager_ptr->GetWisardSensor_1Down_Strip_1_dl() );
logicSiDet_1Down_Strip_2_dl->SetSensitiveDetector( manager_ptr->GetWisardSensor_1Down_Strip_2_dl() );
logicSiDet_1Down_Strip_3_dl->SetSensitiveDetector( manager_ptr->GetWisardSensor_1Down_Strip_3_dl() );
logicSiDet_1Down_Strip_4_dl->SetSensitiveDetector( manager_ptr->GetWisardSensor_1Down_Strip_4_dl() );
logicSiDet_1Down_Strip_5_dl->SetSensitiveDetector( manager_ptr->GetWisardSensor_1Down_Strip_5_dl() );

logicSiDet_2Down_Strip_1_dl->SetSensitiveDetector( manager_ptr->GetWisardSensor_2Down_Strip_1_dl() );
logicSiDet_2Down_Strip_2_dl->SetSensitiveDetector( manager_ptr->GetWisardSensor_2Down_Strip_2_dl() );
logicSiDet_2Down_Strip_3_dl->SetSensitiveDetector( manager_ptr->GetWisardSensor_2Down_Strip_3_dl() );
logicSiDet_2Down_Strip_4_dl->SetSensitiveDetector( manager_ptr->GetWisardSensor_2Down_Strip_4_dl() );
logicSiDet_2Down_Strip_5_dl->SetSensitiveDetector( manager_ptr->GetWisardSensor_2Down_Strip_5_dl() );

logicSiDet_3Down_Strip_1_dl->SetSensitiveDetector( manager_ptr->GetWisardSensor_3Down_Strip_1_dl() );
logicSiDet_3Down_Strip_2_dl->SetSensitiveDetector( manager_ptr->GetWisardSensor_3Down_Strip_2_dl() );
logicSiDet_3Down_Strip_3_dl->SetSensitiveDetector( manager_ptr->GetWisardSensor_3Down_Strip_3_dl() );
logicSiDet_3Down_Strip_4_dl->SetSensitiveDetector( manager_ptr->GetWisardSensor_3Down_Strip_4_dl() );
logicSiDet_3Down_Strip_5_dl->SetSensitiveDetector( manager_ptr->GetWisardSensor_3Down_Strip_5_dl() );

logicSiDet_4Down_Strip_1_dl->SetSensitiveDetector( manager_ptr->GetWisardSensor_4Down_Strip_1_dl() );
logicSiDet_4Down_Strip_2_dl->SetSensitiveDetector( manager_ptr->GetWisardSensor_4Down_Strip_2_dl() );
logicSiDet_4Down_Strip_3_dl->SetSensitiveDetector( manager_ptr->GetWisardSensor_4Down_Strip_3_dl() );
logicSiDet_4Down_Strip_4_dl->SetSensitiveDetector( manager_ptr->GetWisardSensor_4Down_Strip_4_dl() );
logicSiDet_4Down_Strip_5_dl->SetSensitiveDetector( manager_ptr->GetWisardSensor_4Down_Strip_5_dl() );

Logic_MylarSource->SetSensitiveDetector( manager_ptr->GetWisardSensor_CatcherMylar() );
// Logic_AlSource1->SetSensitiveDetector( manager_ptr->GetWisardSensor_CatcherAl1() );
// Logic_AlSource2->SetSensitiveDetector( manager_ptr->GetWisardSensor_CatcherAl2() );

return fPhysiWorld;
}
