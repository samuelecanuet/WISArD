#include "Wisard_Detector.hh"
#include "Wisard_Sensor.hh"
#include "Wisard_MagnetField.hh"

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
#include "G4GenericMessenger.hh"

// #include "Wisard_Messenger.hh"

#include "G4Trap.hh"

#include <unordered_map>
#include <utility> // Pour std::pair
#include "G4SDManager.hh"

//----------------------------------------------------------------------

Wisard_Detector::Wisard_Detector(Wisard_RunManager *mgr)
{

  cout << "Constructor Wisard_Detectors" << endl;
  manager_ptr = mgr;

  pDz = 1.5 * mm;    // Spessore del supporto (su cui soo messe le strip
  pDy1 = 34.12 * mm; // Altezza (y) del trapezio sulla prima faccia. Se le facce non sono svasate é uguale a pDy2

  thicknessSiDetector = 300. * um;
  spazio_tra_Bordo_e_strip5 = 1.29 * mm + 0.225 * mm; // spazio tra bordo superiore e strip 5 (la più lunga). Per dettagli guardare disegno di Sean.
  spazio_tra_Strip = 0.07 * mm;                       // spazio tra ciascuna strip
  thetaInclinazione_SiDetector = 52.64 * degree;
  G4double spazio_tra_Scintillatore_e_BordoSiDetector = 3.745 * mm;

  // Riferito al disegno di Sean e di Mathieu (foto su telegram, piani stampati in data 17/05/2021 su formato A1)
  xLow_SiDet_Strip_5 = 63.63 * mm;
  y_SiDet_Strip_5 = 4.11 * mm;
  xHigh_SiDet_Strip_5 = xLow_SiDet_Strip_5 + (2 * y_SiDet_Strip_5 * (cos(thetaInclinazione_SiDetector) / sin(thetaInclinazione_SiDetector)));
  xLow_SiDet_Strip_4 = 56.58 * mm;
  y_SiDet_Strip_4 = 4.55 * mm;
  xHigh_SiDet_Strip_4 = xLow_SiDet_Strip_4 + (2 * y_SiDet_Strip_4 * (cos(thetaInclinazione_SiDetector) / sin(thetaInclinazione_SiDetector)));
  xLow_SiDet_Strip_3 = 48.52 * mm;
  y_SiDet_Strip_3 = 5.21 * mm;
  xHigh_SiDet_Strip_3 = xLow_SiDet_Strip_3 + (2 * y_SiDet_Strip_3 * (cos(thetaInclinazione_SiDetector) / sin(thetaInclinazione_SiDetector)));
  xLow_SiDet_Strip_2 = 38.82 * mm;
  y_SiDet_Strip_2 = 6.28 * mm;
  xHigh_SiDet_Strip_2 = xLow_SiDet_Strip_2 + (2 * y_SiDet_Strip_2 * (cos(thetaInclinazione_SiDetector) / sin(thetaInclinazione_SiDetector)));
  xLow_SiDet_Strip_1 = 25.65 * mm;
  y_SiDet_Strip_1 = 8.56 * mm;
  xHigh_SiDet_Strip_1 = xLow_SiDet_Strip_1 + (2 * y_SiDet_Strip_1 * (cos(thetaInclinazione_SiDetector) / sin(thetaInclinazione_SiDetector)));

  theta = 40. * degree;
  G4double z_height_Source_biggerBaseSiDet_inVerticale = 24.92 * mm;
  r = fRadius_PlasticScintillator + spazio_tra_Scintillatore_e_BordoSiDetector + pDy1 / 2 - (pDy1 / 2 * (1 - cos(theta)));
  r_vide = (pDz + thicknessSiDetector) / 2 * -sin(theta);
  z_vide = -(pDz + thicknessSiDetector) / 2 * cos(theta);
  z = z_height_Source_biggerBaseSiDet_inVerticale + pDz / 2 + ((pDy1 / 2) * sin(thetaInclinazione_SiDetector));

  //___________ Supporto in rame della placca in PBC sulla quale é montato il Si detector  ____________
  length_x_SupportoRame_SiDetector = 56. * mm;
  height_y_SupportoRame_SiDetector = 33.548 * mm;
  thickness_z_SupportoRame_SiDetector = 1.5 * mm;
  x_smallBox_daTagliare_SupportoRame_SiDetector = 10. * mm; // 6
  y_smallBox_daTagliare_SupportoRame_SiDetector = 10. * mm; // 5
  distanza_latoDxBoxTagliata_e_bordoDxSupportoRame_SiDetector = 3.2 * mm;

  /// colors ///
  SiliconDetector_att = new G4VisAttributes(G4Colour(1., 0., 0.)); // red (r,g,b) ->red
  SiliconDetector_att->SetForceWireframe(false);
  SiliconDetector_att->SetForceSolid(true);

  dic_position["1Up"] = G4ThreeVector(0, r, z);
  dic_position["2Up"] = G4ThreeVector(r, 0, z);
  dic_position["3Up"] = G4ThreeVector(0, -r, z);
  dic_position["4Up"] = G4ThreeVector(-r, 0, z);
  dic_position["1Down"] = G4ThreeVector(0, r, -z);
  dic_position["2Down"] = G4ThreeVector(r, 0, -z);
  dic_position["3Down"] = G4ThreeVector(0, -r, -z);
  dic_position["4Down"] = G4ThreeVector(-r, 0, -z);

  dic_positionvide["1Up"] = G4ThreeVector(0, r + r_vide, z + z_vide);
  dic_positionvide["2Up"] = G4ThreeVector(r + r_vide, 0, z + z_vide);
  dic_positionvide["3Up"] = G4ThreeVector(0, -r - r_vide, z + z_vide);
  dic_positionvide["4Up"] = G4ThreeVector(-r - r_vide, 0, z + z_vide);
  dic_positionvide["1Down"] = G4ThreeVector(0, r + r_vide, -z - z_vide);
  dic_positionvide["2Down"] = G4ThreeVector(r + r_vide, 0, -z - z_vide);
  dic_positionvide["3Down"] = G4ThreeVector(0, -r - r_vide, -z - z_vide);
  dic_positionvide["4Down"] = G4ThreeVector(-r - r_vide, 0, -z - z_vide);

  dic_rotate["1Up"] = std::make_tuple(theta, 180. * deg, 180 * deg);
  dic_rotate["2Up"] = std::make_tuple(0, 180. * deg - theta, 90. * deg);
  dic_rotate["3Up"] = std::make_tuple(-theta, 180. * deg, 0. * deg);
  dic_rotate["4Up"] = std::make_tuple(0., 180. * deg + theta, -90. * deg);
  dic_rotate["1Down"] = std::make_tuple(-theta, 0. * deg, 180. * deg);
  dic_rotate["2Down"] = std::make_tuple(180 * deg, 180 * deg - theta, 90. * deg);
  dic_rotate["3Down"] = std::make_tuple(theta, 0., 0.);
  dic_rotate["4Down"] = std::make_tuple(180. * deg, 180. * deg + theta, -90. * deg);
}

// destructor
Wisard_Detector::~Wisard_Detector()
{
  cout << "Destructor Wisard_Detectors" << endl;
}

//----------------------------------------------------------------------

// Function that builds all the simulation geometry
G4VPhysicalVolume *Wisard_Detector::Construct()
{

  static G4bool fieldIsInitialized = false;

  if (!fieldIsInitialized)
  {
    G4FieldManager *pFieldMgr;
    G4MagneticField *WisardMagField = new WisardMagnetField("MAGNETIC_FIELD_data/wisard_field_complete.txt", 0.004);
    pFieldMgr = G4TransportationManager::GetTransportationManager()->GetFieldManager();
    G4ChordFinder *pChordFinder = new G4ChordFinder(WisardMagField);
    pChordFinder->SetDeltaChord(1 * um);
    pFieldMgr->SetChordFinder(pChordFinder);
    pFieldMgr->SetDetectorField(WisardMagField);
    fieldIsInitialized = true;
  }

  // #endif

  G4GeometryManager::GetInstance()->OpenGeometry();
  G4PhysicalVolumeStore::GetInstance()->Clean();
  G4LogicalVolumeStore::GetInstance()->Clean();
  G4SolidStore::GetInstance()->Clean();

  //--------------------------------------------------------------------------------------
  //------------------------------------ World -------------------------------------------
  //--------------------------------------------------------------------------------------
  G4double innerRadius = 0 * cm;
  G4double outerRadius = 6.5 * cm; // réduit au raypon du Bore pour opti6.5
  G4double length = 120. * cm;      // réduit pour opti21cm
  G4double theta1 = 90.0 * deg;
  G4double phi = 360.0 * deg;

  fSolidWorld = new G4Tubs("World", innerRadius, outerRadius, length / 2, theta1, phi);

  fLogicWorld = new G4LogicalVolume(fSolidWorld, // its solid
                                    vide,        // its material
                                    "World");    // its name

  fPhysiWorld = new G4PVPlacement(0,               // no rotation
                                  G4ThreeVector(), // at (0,0,0)
                                  fLogicWorld,     // its fLogical volume
                                  "World",         // its name
                                  0,               // its mother  volume
                                  false,           // no boolean operation
                                  0);              // copy number

  G4VisAttributes *visAtt_World = new G4VisAttributes(G4Colour(1, 1, 1)); // white
  visAtt_World->SetVisibility(false);
  fLogicWorld->SetVisAttributes(visAtt_World);

  //==================================================================================================
  //========================================  WISArD MAGNET =========================================
  //==================================================================================================
  G4Material *materialIron = G4NistManager::Instance()->FindOrBuildMaterial("G4_Fe");
  G4double radius_wisard = 70.0 * cm;
  G4double radius_bore = 6.5 * cm;
  G4double length_bore = 80 * cm;
  G4double radius_wall = 0.3 * cm;

  G4Tubs *wisard = new G4Tubs("Wisard_magnet",
                              radius_bore + radius_wall,
                              radius_wisard,
                              0.5L * (length_bore),
                              0.L, 360.L * deg);
  G4LogicalVolume *wisard_logic = new G4LogicalVolume(wisard,        // forme
                                                      materialIron,  // matiere
                                                      "WisardLogic", // nom
                                                      0, 0, 0);
  // - physical volume
  G4VPhysicalVolume *wisard_phys = new G4PVPlacement(0,
                                                     G4ThreeVector(0., 0., 0.),
                                                     wisard_logic, // ptr sur vol. log.
                                                     "Wisard",     // nom du volume
                                                     fLogicWorld,  // volume parent
                                                     false, true);

  // - make the bore volume "visible"
  G4VisAttributes *visAtt_WITCHMagnet = new G4VisAttributes(G4Colour(0.5, 0.7, 0.7, 0.2));
  visAtt_WITCHMagnet->SetVisibility(true);
  visAtt_WITCHMagnet->SetForceWireframe(false);
  visAtt_WITCHMagnet->SetForceSolid(true);
  wisard_logic->SetVisAttributes(visAtt_WITCHMagnet);

  if (wisard_phys == NULL)
  {
  }

  //==================================================================================================
  //=======================================  BORE - MAGNET ==========================================
  //==================================================================================================
  G4Material *materialAluminum = G4NistManager::Instance()->FindOrBuildMaterial("G4_Al");

  G4Tubs *bore = new G4Tubs("Bore_Tube",
                            radius_bore,
                            radius_bore + radius_wall,
                            0.5L * (length_bore),
                            0.L, 360.L * deg);
  G4LogicalVolume *bore_logic = new G4LogicalVolume(bore,             // forme
                                                    materialAluminum, // matiere
                                                    "BoreLogic",      // nom
                                                    0, 0, 0);
  // - physical volume
  G4VPhysicalVolume *bore_phys = new G4PVPlacement(0,
                                                   G4ThreeVector(0., 0., 0.),
                                                   bore_logic,  // ptr sur vol. log.
                                                   "Bore",      // nom du volume
                                                   fLogicWorld, // volume parent
                                                   false, true);

  // - make the bore volume "visible"
  G4VisAttributes *visAttr_Bore = new G4VisAttributes(G4Colour(0.4, 0.4, 0.4, 0.2));
  visAttr_Bore->SetVisibility(true);
  visAttr_Bore->SetForceWireframe(false);
  visAttr_Bore->SetForceSolid(true);
  bore_logic->SetVisAttributes(visAttr_Bore);

  if (bore_phys == NULL)
  {
  }

  //======================================================================================================
  //====================================== CYLINDER ======================================================
  //======================================================================================================
  innerRadius = 71. * cm;
  outerRadius = 72. * cm;
  length = 100. * cm;
  theta = 90.0 * deg;
  phi = 360.0 * deg;

  G4Material *fMaterial_Al = G4NistManager::Instance()->FindOrBuildMaterial("G4_Al");
  G4Tubs *fSolidMYCILINDER = new G4Tubs("MYCILINDER", innerRadius, outerRadius, length / 2, theta, phi);

  G4LogicalVolume *fLogicMYCILINDER = new G4LogicalVolume(fSolidMYCILINDER, // its solid
                                                          fMaterial_Al,     // its material
                                                          "MYCILINDER");    // its name

  G4VPhysicalVolume *fPhysiMYCILINDER = new G4PVPlacement(0,                      // no rotation
                                                          G4ThreeVector(0, 0, 0), // at (0,0,0)
                                                          fLogicMYCILINDER,       // its fLogical volume
                                                          "MYCILINDER",           // its name
                                                          fLogicWorld,            // its mother  volume
                                                          false,                  // no boolean operation
                                                          0,
                                                          false); // copy number

  G4VisAttributes *visAtt_MYCILINDER = new G4VisAttributes(G4Colour(1, 1, 0)); // white
  visAtt_MYCILINDER->SetVisibility(true);
  fLogicMYCILINDER->SetVisAttributes(visAtt_MYCILINDER);

  if (fPhysiMYCILINDER == NULL)
  {
  }

  // BASE 1
  innerRadius = 0 * mm;
  outerRadius = 72 * cm;
  G4double lengthB = 1. * cm;
  theta = 90.0 * deg;
  phi = 360.0 * deg;

  G4Tubs *fSolidMYCILINDER_B1 = new G4Tubs("MYCILINDER_B1", innerRadius, outerRadius, lengthB / 2, theta, phi);

  G4LogicalVolume *fLogicMYCILINDER_B1 = new G4LogicalVolume(fSolidMYCILINDER_B1, // its solid
                                                             fMaterial_Al,        // its material
                                                             "MYCILINDER_B1");    // its name

  G4VPhysicalVolume *fPhysiMYCILINDER_B1 = new G4PVPlacement(0,                                                  // no rotation
                                                             G4ThreeVector(0, 0, -(length / 2) - (lengthB / 2)), // at (0,0,0)
                                                             fLogicMYCILINDER_B1,                                // its fLogical volume
                                                             "MYCILINDER_B1",                                    // its name
                                                             fLogicMYCILINDER,                                   // its mother  volume
                                                             false,                                              // no boolean operation
                                                             0,
                                                             false); // copy number

  G4VisAttributes *visAtt_MYCILINDER_B1 = new G4VisAttributes(G4Colour(1, 1, 0)); // white
  visAtt_MYCILINDER_B1->SetVisibility(true);
  fLogicMYCILINDER_B1->SetVisAttributes(visAtt_MYCILINDER_B1);

  if (fPhysiMYCILINDER_B1 == NULL)
  {
  }

  // BASE 2
  G4Tubs *fSolidMYCILINDER_B2 = new G4Tubs("MYCILINDER_B2", innerRadius, outerRadius, lengthB / 2, theta, phi);

  G4LogicalVolume *fLogicMYCILINDER_B2 = new G4LogicalVolume(fSolidMYCILINDER_B2, // its solid
                                                             fMaterial_Al,        // its material
                                                             "MYCILINDER_B2");    // its name

  G4VPhysicalVolume *fPhysiMYCILINDER_B2 = new G4PVPlacement(0,                                                 // no rotation
                                                             G4ThreeVector(0, 0, (length / 2) + (lengthB / 2)), // at (0,0,0)
                                                             fLogicMYCILINDER_B2,                               // its fLogical volume
                                                             "MYCILINDER_B2",                                   // its name
                                                             fLogicMYCILINDER,                                  // its mother  volume
                                                             false,                                             // no boolean operation
                                                             0,
                                                             false); // copy number

  G4VisAttributes *visAtt_MYCILINDER_B2 = new G4VisAttributes(G4Colour(1, 1, 0)); // white
  visAtt_MYCILINDER_B2->SetVisibility(true);
  fLogicMYCILINDER_B2->SetVisAttributes(visAtt_MYCILINDER_B2);

  if (fPhysiMYCILINDER_B2 == NULL)
  {
  }

  //       // ===========================================================================================================
  //       // ================================ CATCHER/SORGENTE RAD./SUPPORTO  ==========================================
  //       // ===========================================================================================================
  //       // Guardiamo il supporto del catcher dalla parte attaccata al perno verso la sorgente.
  //       // La parte 1 è il supporto rettangolare lungo, la parte 2 è il il supporto rettangolare largo su cui si trovano i buchi per il catcher e il posizionamento sorgente radioattiva.
  //       // Per maggiori dettagli vedere mail M. Roche 10/06/2021 alle ore 11-50 circa (e foto su cui ho scritto ed evidenziato salvata su telegram).
  //        G4double x_baseCatcher_Spessa_1          = 14.*mm;
  //        G4double y_baseCatcher_Spessa_1          = 16.*mm;
  //        G4double thickness_baseCatcher_Spessa_1  = 12.*mm;
  //        G4double radius_buco_baseSpessa          = 5.*mm;
  //        G4double pos_buco_baseSpessa             = 0.8963*mm;
  //        G4double x_baseCatcher_Sottile_1         = x_baseCatcher_Spessa_1;
  //        G4double y_baseCatcher_Sottile_1         = 24.*mm;
  //        G4double thickness_baseCatcher_Sottile_1 = 4.*mm;
  //        G4double x_baseCatcher_2                 = 60.*mm;
  //        G4double y_baseCatcher_2                 = 32.*mm;
  //        G4double z_baseCatcher_2                 = thickness_baseCatcher_Sottile_1; //NB: tutte le misure precedenti sono state prese direttamente dai disegni tecnici di M. Roche
  //        G4double radius_ext_bucoCatcher          = (25./2)*mm;
  //        G4double radius_bucoCatcher              = (15./2)*mm;
  //        G4double thickness_supp_catcher          = 2.*mm;
  //        G4double radius_bucoSource               = (20./2)*mm;
  //        //G4double thickness_supp_radSource        = 3.*mm;
  //        //G4double altezza_supp_radSource          = 0.5*mm; //Da Verificare
  //        G4double x_pos_bucoCatcher               = (25.88/2)*mm;
  //       // G4double x_pos_bucoSource                = x_pos_bucoCatcher;
  //        G4double x_y_triangularCut_baseSpessa    = 5.*mm;
  //        G4double x_y_triangularCut_baseSottile   = 8.*mm;
  //        G4Material* materialSupport_Catcher_source  = G4NistManager::Instance()->FindOrBuildMaterial("G4_Al");

  //        G4RotationMatrix* myRotation_trSupp = new G4RotationMatrix();
  //        myRotation_trSupp->rotateX(0.*deg);
  //        myRotation_trSupp->rotateY(180.*deg);
  //        myRotation_trSupp->rotateZ(0.*rad);

  //        G4RotationMatrix* myRotation_trSupp2 = new G4RotationMatrix();
  //        myRotation_trSupp2->rotateX(180.*deg);
  //        myRotation_trSupp2->rotateY(180.*deg);
  //        myRotation_trSupp2->rotateZ(0.*rad);

  //        G4RotationMatrix* myRotation_trSupp3 = new G4RotationMatrix();
  //        myRotation_trSupp3->rotateX(180.*deg);
  //        myRotation_trSupp3->rotateY(0.*deg);
  //        myRotation_trSupp3->rotateZ(0.*rad);

  //        G4Box* SuppCatcher_baseSpessa_senzaBuco  = new G4Box("SuppCatcher_baseSpessa_senzaBuco", x_baseCatcher_Spessa_1/2, y_baseCatcher_Spessa_1/2, thickness_baseCatcher_Spessa_1/2);
  //        G4Tubs* bucoBaseSpessa                   = new G4Tubs("bucoBaseSpessa", 0., radius_buco_baseSpessa, 2*thickness_baseCatcher_Spessa_1, 0., 360*deg);
  //        G4VSolid* SuppCatcher_baseSpessa_intera  = new G4SubtractionSolid("SuppCatcher_baseSpessa_intera", SuppCatcher_baseSpessa_senzaBuco, bucoBaseSpessa, 0, G4ThreeVector(0.,-0.2962*mm,0.));

  //        const G4int nsect = 3;
  //        std::vector<G4TwoVector> polygon(nsect);
  //        polygon[0].set(x_y_triangularCut_baseSpessa, 0.);
  //        polygon[1].set(0., 0.);
  //        polygon[2].set(x_y_triangularCut_baseSpessa, x_y_triangularCut_baseSpessa);

  //        G4TwoVector offsetA(0,0), offsetB(0,0);
  //        G4double scaleA = 1, scaleB = 1;
  //        G4VSolid* triangularCut_baseSpessa = new G4ExtrudedSolid("triangularCut_baseSpessa", polygon, thickness_baseCatcher_Spessa_1, offsetA, scaleA, offsetB, scaleB);

  //        G4VSolid* SuppCatcher_baseSpessa_parziale  = new G4SubtractionSolid("SuppCatcher_baseSpessa_parziale", SuppCatcher_baseSpessa_intera, triangularCut_baseSpessa, 0, G4ThreeVector(x_baseCatcher_Spessa_1/2 - x_y_triangularCut_baseSpessa+0.5*mm, -y_baseCatcher_Spessa_1/2, 0.));
  //        G4VSolid* SuppCatcher_baseSpessa           = new G4SubtractionSolid("SuppCatcher_baseSpessa", SuppCatcher_baseSpessa_parziale, triangularCut_baseSpessa, myRotation_trSupp, G4ThreeVector(-x_baseCatcher_Spessa_1/2 + x_y_triangularCut_baseSpessa-0.5*mm, -y_baseCatcher_Spessa_1/2 -0.5*mm, 0.));

  //        G4ThreeVector pos_SuppCatcher_baseSpessa = G4ThreeVector(0., pos_buco_baseSpessa, 0.);
  //        G4Transform3D tr_SuppCatcher_baseSpessa  = G4Transform3D(G4RotationMatrix(), pos_SuppCatcher_baseSpessa);

  //        G4Box* SuppCatcher_baseSottile            = new G4Box("SuppCatcher_baseSottile", x_baseCatcher_Sottile_1/2, y_baseCatcher_Sottile_1/2, thickness_baseCatcher_Sottile_1/2);
  //        G4ThreeVector pos_SuppCatcher_baseSottile = G4ThreeVector(0., y_baseCatcher_Spessa_1/2 + y_baseCatcher_Sottile_1/2, thickness_baseCatcher_Spessa_1/2 - thickness_baseCatcher_Sottile_1/2);
  //        G4Transform3D tr_SuppCatcher_baseSottile  = G4Transform3D(G4RotationMatrix(), pos_SuppCatcher_baseSottile);

  //        G4Box* SuppCatcher_base_2_senzaBuco         = new G4Box("SuppCatcher_base_2_senzaBuco", x_baseCatcher_2/2, y_baseCatcher_2/2, z_baseCatcher_2/2);
  //        G4Tubs* bucoCatcher                         = new G4Tubs("bucoCatcher", 0., radius_bucoCatcher, 2*z_baseCatcher_2, 0., 360*deg);
  //        G4VSolid* SuppCatcher_base_2_conBucoCatcher = new G4SubtractionSolid("SuppCatcher_baseSpessa_conbucoCatcher", SuppCatcher_base_2_senzaBuco, bucoCatcher, 0, G4ThreeVector(-x_pos_bucoCatcher, 0., 0.));
  //        G4Tubs* bucoSource                          = new G4Tubs("bucoSource", 0., radius_bucoSource, 2*z_baseCatcher_2, 0., 360*deg);
  //        G4VSolid* SuppCatcher_base_2_intero         = new G4SubtractionSolid("SuppCatcher_base_2_intero", SuppCatcher_base_2_conBucoCatcher, bucoSource, 0, G4ThreeVector(x_pos_bucoCatcher, 0., 0.));

  //        std::vector<G4TwoVector> polygon2(nsect);
  //        polygon2[0].set(x_y_triangularCut_baseSottile, 0.);
  //        polygon2[1].set(0., 0.);
  //        polygon2[2].set(x_y_triangularCut_baseSottile, x_y_triangularCut_baseSottile);

  //        G4VSolid* triangularCut_baseSottile     = new G4ExtrudedSolid("triangularCut_baseSottile", polygon2, thickness_baseCatcher_Spessa_1, offsetA, scaleA, offsetB, scaleB);
  //        G4VSolid* SuppCatcher_base_2_unAngolo   = new G4SubtractionSolid("SuppCatcher_base_2_unAngolo", SuppCatcher_base_2_intero, triangularCut_baseSottile, 0, G4ThreeVector(+x_baseCatcher_2/2 - x_y_triangularCut_baseSottile +0.5*mm, -y_baseCatcher_2/2 -0.5*mm, 0.));
  //        G4VSolid* SuppCatcher_base_2_dueAngoli  = new G4SubtractionSolid("SuppCatcher_base_2_dueAngoli", SuppCatcher_base_2_unAngolo, triangularCut_baseSottile, myRotation_trSupp, G4ThreeVector(-x_baseCatcher_2/2 + x_y_triangularCut_baseSottile -0.5*mm, -y_baseCatcher_2/2 -0.5*mm, 0.));
  //        G4VSolid* SuppCatcher_base_2_treAngoli  = new G4SubtractionSolid("SuppCatcher_base_2_treAngoli", SuppCatcher_base_2_dueAngoli, triangularCut_baseSottile, myRotation_trSupp2, G4ThreeVector(-x_baseCatcher_2/2 + x_y_triangularCut_baseSottile -0.5*mm, +y_baseCatcher_2/2 +0.5*mm, 0.));
  //        G4VSolid* SuppCatcher_base_2            = new G4SubtractionSolid("SuppCatcher_base_2", SuppCatcher_base_2_treAngoli, triangularCut_baseSottile, myRotation_trSupp3, G4ThreeVector(x_baseCatcher_2/2 - x_y_triangularCut_baseSottile +0.5*mm, +y_baseCatcher_2/2 +0.5*mm, 0.));

  //        G4ThreeVector pos_SuppCatcher_base_2 = G4ThreeVector(0., y_baseCatcher_Spessa_1/2 + y_baseCatcher_Sottile_1 + y_baseCatcher_2/2, thickness_baseCatcher_Spessa_1/2 - z_baseCatcher_2/2);
  //        G4Transform3D tr_SuppCatcher_base_2  = G4Transform3D(G4RotationMatrix(), pos_SuppCatcher_base_2);

  //        G4MultiUnion* munion_supportCatcher_Source = new G4MultiUnion("munion_supportCatcher_Source");
  // munion_supportCatcher_Source->AddNode(*SuppCatcher_baseSpessa, tr_SuppCatcher_baseSpessa);
  // munion_supportCatcher_Source->AddNode(*SuppCatcher_baseSottile, tr_SuppCatcher_baseSottile);
  // munion_supportCatcher_Source->AddNode(*SuppCatcher_base_2     , tr_SuppCatcher_base_2);
  //        munion_supportCatcher_Source->Voxelize();

  //        G4LogicalVolume* logic_munion_supportCatcher_Source  = new G4LogicalVolume(munion_supportCatcher_Source, materialSupport_Catcher_source, "logic_munion_supportCatcher_Source");
  //        G4VPhysicalVolume* phys_munion_supportCatcher_Source = new G4PVPlacement(0,                        //no rotation
  // G4ThreeVector(x_pos_bucoCatcher, - (y_baseCatcher_Spessa_1/2 + y_baseCatcher_Sottile_1 + y_baseCatcher_2/2), -(thickness_baseCatcher_Spessa_1/2 + 0/2 + 0/2)),     //+thickness_supp_catcher/2   //at (0,0,0) //thickness_baseCatcher_Sottile_1
  // logic_munion_supportCatcher_Source,                //its fLogical volume
  // "phys_munion_supportCatcher_Source",                //its name
  // fLogicWorld,                        //its mother  volume
  // false,                        //no boolean operation
  // 0,
  // false);                        //copy number

  //        G4VisAttributes* visAtt_munion_supportCatcher_Source  = new G4VisAttributes(G4Colour(0.8,0.8,0.8));
  // visAtt_munion_supportCatcher_Source->SetVisibility(true);
  // visAtt_munion_supportCatcher_Source->SetForceWireframe(false);
  // visAtt_munion_supportCatcher_Source->SetForceSolid(true);
  //        logic_munion_supportCatcher_Source ->SetVisAttributes(visAtt_munion_supportCatcher_Source);

  //        if(phys_munion_supportCatcher_Source == NULL) {}

  //        //G4double x_pos_Catcher_risp_munion   = x_pos_bucoCatcher;
  //        G4double y_pos_Catcher_risp_munion   = y_baseCatcher_Spessa_1/2 + y_baseCatcher_Sottile_1 + y_baseCatcher_2/2;

  //        G4Tubs* Supp_catcher                 = new G4Tubs("Supp_catcher", radius_bucoCatcher, radius_ext_bucoCatcher, thickness_supp_catcher/2, 0., 360*deg);
  //        G4LogicalVolume* logic_Supp_catcher  = new G4LogicalVolume(Supp_catcher, materialSupport_Catcher_source, "logic_Supp_catcher");
  //        G4VPhysicalVolume* phys_Supp_catcher = new G4PVPlacement(0,                        //no rotation
  // G4ThreeVector(-x_pos_bucoCatcher, y_pos_Catcher_risp_munion, thickness_baseCatcher_Spessa_1/2),        //at (0,0,0)
  // logic_Supp_catcher,                //its fLogical volume
  // "phys_Supp_catcher",                //its name
  // logic_munion_supportCatcher_Source,                        //its mother  volume
  // false,                        //no boolean operation
  // 0,
  // false);                        //copy number

  //       G4VisAttributes* visAtt_Supp_catcher  = new G4VisAttributes(G4Colour(0.3,0.3,0.3));
  //       visAtt_Supp_catcher->SetVisibility(true);
  //       visAtt_Supp_catcher->SetForceWireframe(false);
  //       visAtt_Supp_catcher->SetForceSolid(true);
  //       logic_Supp_catcher ->SetVisAttributes(visAtt_Supp_catcher );

  //       if(phys_Supp_catcher == NULL) {}

  //       G4Material* Mylar = G4NistManager::Instance()->FindOrBuildMaterial("G4_MYLAR");

  //       //G4double thicknessMylarSource    = 500.*nm;
  //       G4double thicknessMylarSource    = 6.*um;
  //       G4double outerRadius_MylarSource = radius_bucoCatcher;

  //       G4Tubs*              MylarSource          = new G4Tubs("MylarSource", 0., outerRadius_MylarSource, thicknessMylarSource/2, 0., 360.*deg);
  //       G4LogicalVolume*     Logic_MylarSource    = new G4LogicalVolume(MylarSource, Mylar, "LogicMylarSource"); //solid, material, name
  //       G4VPhysicalVolume*   Physics_MylarSource  = new G4PVPlacement(0,                //no rotation
  // G4ThreeVector(0., 0., 0.),    //position
  // Logic_MylarSource,"LogicMylarSource",        //its fLogical volume
  // fLogicWorld,            //its mother volume
  // false,                //no boolean op.
  // 0,
  // true);                //copy nb.

  //       G4VisAttributes* MylarSource_att = new G4VisAttributes(G4Colour(0.94, 0.5, 0.5)); //pink
  //       MylarSource_att->SetVisibility(true);
  //       MylarSource_att->SetForceWireframe(false);
  //       MylarSource_att->SetForceSolid(true);
  //       Logic_MylarSource->SetVisAttributes(MylarSource_att);

  //      if(Physics_MylarSource == NULL) {}

  /////////////////////////////////////////////////////////////
  ///////////////////////NEW SUPPORT CATCHER//////////////////
  /////////////////////////////////////////////////////////////

  G4double x_baseCatcher_Spessa_1 = 14. * mm;
  y_baseCatcher_Spessa_1 = 16. * mm;
  thickness_baseCatcher_Spessa_1 = 12. * mm;
  G4double radius_buco_baseSpessa = 5. * mm;
  G4double pos_buco_baseSpessa = 0.8963 * mm;
  G4double x_baseCatcher_Sottile_1 = x_baseCatcher_Spessa_1;
  y_baseCatcher_Sottile_1 = 24. * mm;
  thickness_baseCatcher_Sottile_1 = 2. * mm;
  G4double x_baseCatcher_2 = 60. * mm;
  y_baseCatcher_2 = 32. * mm;
  G4double z_baseCatcher_2 = thickness_baseCatcher_Sottile_1; // NB: tutte le misure precedenti sono state prese direttamente dai disegni tecnici di M. Roche
  G4double radius_int_bucoCatcher = 7.5 * mm;                 /// a revoir
  G4double radius_bucoCatcher = 9 * mm;
  thickness_supp_catcher = 1.5 * mm;
  G4double radius_bucoSource = (20. / 2) * mm;
  // G4double thickness_supp_radSource        = 3.*mm;
  // G4double altezza_supp_radSource          = 0.5*mm; //Da Verificare
  x_pos_bucoCatcher = (25.88 / 2) * mm;
  // G4double x_pos_bucoSource                = x_pos_bucoCatcher;
  G4double x_y_triangularCut_baseSpessa = 5. * mm;
  G4double x_y_triangularCut_baseSottile = 8. * mm;
  G4Material *materialSupport_Catcher_source = G4NistManager::Instance()->FindOrBuildMaterial("G4_Al");

  G4RotationMatrix *myRotation_trSupp = new G4RotationMatrix();
  myRotation_trSupp->rotateX(0. * deg);
  myRotation_trSupp->rotateY(180. * deg);
  myRotation_trSupp->rotateZ(0. * rad);

  G4RotationMatrix *myRotation_trSupp2 = new G4RotationMatrix();
  myRotation_trSupp2->rotateX(180. * deg);
  myRotation_trSupp2->rotateY(180. * deg);
  myRotation_trSupp2->rotateZ(0. * rad);

  G4RotationMatrix *myRotation_trSupp3 = new G4RotationMatrix();
  myRotation_trSupp3->rotateX(180. * deg);
  myRotation_trSupp3->rotateY(0. * deg);
  myRotation_trSupp3->rotateZ(0. * rad);

  G4Box *SuppCatcher_baseSpessa_senzaBuco = new G4Box("SuppCatcher_baseSpessa_senzaBuco", x_baseCatcher_Spessa_1 / 2, y_baseCatcher_Spessa_1 / 2, thickness_baseCatcher_Spessa_1 / 2);
  G4Tubs *bucoBaseSpessa = new G4Tubs("bucoBaseSpessa", 0., radius_buco_baseSpessa, 2 * thickness_baseCatcher_Spessa_1, 0., 360 * deg);
  G4VSolid *SuppCatcher_baseSpessa_intera = new G4SubtractionSolid("SuppCatcher_baseSpessa_intera", SuppCatcher_baseSpessa_senzaBuco, bucoBaseSpessa, 0, G4ThreeVector(0., -0.2962 * mm, 0.));

  const G4int nsect = 3;
  std::vector<G4TwoVector> polygon(nsect);
  polygon[0].set(x_y_triangularCut_baseSpessa, 0.);
  polygon[1].set(0., 0.);
  polygon[2].set(x_y_triangularCut_baseSpessa, x_y_triangularCut_baseSpessa);

  G4TwoVector offsetA(0, 0), offsetB(0, 0);
  G4double scaleA = 1, scaleB = 1;
  G4VSolid *triangularCut_baseSpessa = new G4ExtrudedSolid("triangularCut_baseSpessa", polygon, thickness_baseCatcher_Spessa_1, offsetA, scaleA, offsetB, scaleB);

  G4VSolid *SuppCatcher_baseSpessa_parziale = new G4SubtractionSolid("SuppCatcher_baseSpessa_parziale", SuppCatcher_baseSpessa_intera, triangularCut_baseSpessa, 0, G4ThreeVector(x_baseCatcher_Spessa_1 / 2 - x_y_triangularCut_baseSpessa + 0.5 * mm, -y_baseCatcher_Spessa_1 / 2, 0.));
  G4VSolid *SuppCatcher_baseSpessa = new G4SubtractionSolid("SuppCatcher_baseSpessa", SuppCatcher_baseSpessa_parziale, triangularCut_baseSpessa, myRotation_trSupp, G4ThreeVector(-x_baseCatcher_Spessa_1 / 2 + x_y_triangularCut_baseSpessa - 0.5 * mm, -y_baseCatcher_Spessa_1 / 2 - 0.5 * mm, 0.));

  G4ThreeVector pos_SuppCatcher_baseSpessa = G4ThreeVector(0., pos_buco_baseSpessa, 0.);
  G4Transform3D tr_SuppCatcher_baseSpessa = G4Transform3D(G4RotationMatrix(), pos_SuppCatcher_baseSpessa);

  G4Box *SuppCatcher_baseSottile = new G4Box("SuppCatcher_baseSottile", x_baseCatcher_Sottile_1 / 2, y_baseCatcher_Sottile_1 / 2, thickness_baseCatcher_Sottile_1 / 2);
  G4ThreeVector pos_SuppCatcher_baseSottile = G4ThreeVector(0., y_baseCatcher_Spessa_1 / 2 + y_baseCatcher_Sottile_1 / 2, thickness_baseCatcher_Spessa_1 / 2 - thickness_baseCatcher_Sottile_1 / 2);
  G4Transform3D tr_SuppCatcher_baseSottile = G4Transform3D(G4RotationMatrix(), pos_SuppCatcher_baseSottile);

  G4Box *SuppCatcher_base_2_senzaBuco = new G4Box("SuppCatcher_base_2_senzaBuco", x_baseCatcher_2 / 2, y_baseCatcher_2 / 2, z_baseCatcher_2 / 2);
  G4Tubs *bucoCatcher = new G4Tubs("bucoCatcher", 0., radius_bucoCatcher, thickness_supp_catcher / 2, 0., 360 * deg);
  G4VSolid *SuppCatcher_base_2_conBucoCatcher = new G4SubtractionSolid("SuppCatcher_baseSpessa_conbucoCatcher", SuppCatcher_base_2_senzaBuco, bucoCatcher, 0, G4ThreeVector(-x_pos_bucoCatcher, 0., thickness_baseCatcher_Sottile_1 - thickness_supp_catcher - 0.25 * mm));

  G4Tubs *bucoCatcher1 = new G4Tubs("bucoCatcher1", 0., radius_int_bucoCatcher, thickness_baseCatcher_Sottile_1 - thickness_supp_catcher, 0., 360 * deg);
  G4VSolid *SuppCatcher_base_2_conBucoCatcher1 = new G4SubtractionSolid("SuppCatcher_baseSpessa_conbucoCatcher1", SuppCatcher_base_2_conBucoCatcher, bucoCatcher1, 0, G4ThreeVector(-x_pos_bucoCatcher, 0., thickness_baseCatcher_Sottile_1 / 2 - thickness_supp_catcher));

  G4Tubs *bucoSource = new G4Tubs("bucoSource", 0., radius_bucoSource, 2 * z_baseCatcher_2, 0., 360 * deg);
  G4VSolid *SuppCatcher_base_2_intero = new G4SubtractionSolid("SuppCatcher_base_2_intero", SuppCatcher_base_2_conBucoCatcher1, bucoSource, 0, G4ThreeVector(x_pos_bucoCatcher, 0., 0.));

  std::vector<G4TwoVector> polygon2(nsect);
  polygon2[0].set(x_y_triangularCut_baseSottile, 0.);
  polygon2[1].set(0., 0.);
  polygon2[2].set(x_y_triangularCut_baseSottile, x_y_triangularCut_baseSottile);

  G4VSolid *triangularCut_baseSottile = new G4ExtrudedSolid("triangularCut_baseSottile", polygon2, thickness_baseCatcher_Spessa_1, offsetA, scaleA, offsetB, scaleB);
  G4VSolid *SuppCatcher_base_2_unAngolo = new G4SubtractionSolid("SuppCatcher_base_2_unAngolo", SuppCatcher_base_2_intero, triangularCut_baseSottile, 0, G4ThreeVector(+x_baseCatcher_2 / 2 - x_y_triangularCut_baseSottile + 0.5 * mm, -y_baseCatcher_2 / 2 - 0.5 * mm, 0.));
  G4VSolid *SuppCatcher_base_2_dueAngoli = new G4SubtractionSolid("SuppCatcher_base_2_dueAngoli", SuppCatcher_base_2_unAngolo, triangularCut_baseSottile, myRotation_trSupp, G4ThreeVector(-x_baseCatcher_2 / 2 + x_y_triangularCut_baseSottile - 0.5 * mm, -y_baseCatcher_2 / 2 - 0.5 * mm, 0.));
  G4VSolid *SuppCatcher_base_2_treAngoli = new G4SubtractionSolid("SuppCatcher_base_2_treAngoli", SuppCatcher_base_2_dueAngoli, triangularCut_baseSottile, myRotation_trSupp2, G4ThreeVector(-x_baseCatcher_2 / 2 + x_y_triangularCut_baseSottile - 0.5 * mm, +y_baseCatcher_2 / 2 + 0.5 * mm, 0.));
  G4VSolid *SuppCatcher_base_2 = new G4SubtractionSolid("SuppCatcher_base_2", SuppCatcher_base_2_treAngoli, triangularCut_baseSottile, myRotation_trSupp3, G4ThreeVector(x_baseCatcher_2 / 2 - x_y_triangularCut_baseSottile + 0.5 * mm, +y_baseCatcher_2 / 2 + 0.5 * mm, 0.));

  G4ThreeVector pos_SuppCatcher_base_2 = G4ThreeVector(0., y_baseCatcher_Spessa_1 / 2 + y_baseCatcher_Sottile_1 + y_baseCatcher_2 / 2, thickness_baseCatcher_Spessa_1 / 2 - z_baseCatcher_2 / 2);
  G4Transform3D tr_SuppCatcher_base_2 = G4Transform3D(G4RotationMatrix(), pos_SuppCatcher_base_2);

  G4MultiUnion *munion_supportCatcher_Source = new G4MultiUnion("munion_supportCatcher_Source");
  munion_supportCatcher_Source->AddNode(*SuppCatcher_baseSpessa, tr_SuppCatcher_baseSpessa);
  munion_supportCatcher_Source->AddNode(*SuppCatcher_baseSottile, tr_SuppCatcher_baseSottile);
  munion_supportCatcher_Source->AddNode(*SuppCatcher_base_2, tr_SuppCatcher_base_2);
  munion_supportCatcher_Source->Voxelize();

  G4LogicalVolume *logic_munion_supportCatcher_Source = new G4LogicalVolume(munion_supportCatcher_Source, materialSupport_Catcher_source, "logic_munion_supportCatcher_Source");
  phys_munion_supportCatcher_Source = new G4PVPlacement(0,                                                                                                                                                                                                                                                     // no rotation
                                                        G4ThreeVector(x_pos_bucoCatcher, -(y_baseCatcher_Spessa_1 / 2 + y_baseCatcher_Sottile_1 + y_baseCatcher_2 / 2), -thickness_baseCatcher_Spessa_1 / 2 + thickness_baseCatcher_Sottile_1 + thickness_baseCatcher_Sottile_1 / 2 - thickness_supp_catcher), //+thickness_supp_catcher/2   //at (0,0,0) //thickness_baseCatcher_Sottile_1
                                                        logic_munion_supportCatcher_Source,                                                                                                                                                                                                                    // its fLogical volume
                                                        "phys_munion_supportCatcher_Source",                                                                                                                                                                                                                   // its name
                                                        fLogicWorld,                                                                                                                                                                                                                                           // its mother  volume
                                                        false,                                                                                                                                                                                                                                                 // no boolean operation
                                                        0);                                                                                                                                                                                                                                                    // copy number

  G4VisAttributes *visAtt_munion_supportCatcher_Source = new G4VisAttributes(G4Colour(0.8, 0.8, 0.8));
  visAtt_munion_supportCatcher_Source->SetVisibility(true);
  visAtt_munion_supportCatcher_Source->SetForceWireframe(false);
  visAtt_munion_supportCatcher_Source->SetForceSolid(true);
  logic_munion_supportCatcher_Source->SetVisAttributes(visAtt_munion_supportCatcher_Source);

  if (phys_munion_supportCatcher_Source == NULL)
  {
  }

  // G4double x_pos_Catcher_risp_munion   = x_pos_bucoCatcher;
  y_pos_Catcher_risp_munion = y_baseCatcher_Spessa_1 / 2 + y_baseCatcher_Sottile_1 + y_baseCatcher_2 / 2;

  G4NistManager *man = G4NistManager::Instance();
  G4Material *material_Catcher_Ring = new G4Material("PEEKS", 1.32 * g / cm3, 3); // Plastic
  material_Catcher_Ring->AddElement(man->FindOrBuildElement("C"), 19);
  material_Catcher_Ring->AddElement(man->FindOrBuildElement("H"), 12);
  material_Catcher_Ring->AddElement(man->FindOrBuildElement("O"), 3);

  G4Tubs *Supp_catcher = new G4Tubs("Supp_catcher", radius_int_bucoCatcher, radius_bucoCatcher, thickness_supp_catcher / 2, 0., 360 * deg);
  G4LogicalVolume *logic_Supp_catcher = new G4LogicalVolume(Supp_catcher, vide, "logic_Supp_catcher");
  phys_Supp_catcher = new G4PVPlacement(0,                                                                                                                                                                                                                                                                                                                                   // no rotation
                                        G4ThreeVector(0., -(y_baseCatcher_Spessa_1 / 2 + y_baseCatcher_Sottile_1 + y_baseCatcher_2 / 2) + y_pos_Catcher_risp_munion, thickness_baseCatcher_Spessa_1 / 2 + thickness_baseCatcher_Sottile_1 + thickness_baseCatcher_Sottile_1 / 2 - thickness_supp_catcher - thickness_baseCatcher_Spessa_1 / 2 - thickness_supp_catcher / 2), // at (0,0,0)
                                        logic_Supp_catcher,                                                                                                                                                                                                                                                                                                                  // its fLogical volume
                                        "phys_Supp_catcher",                                                                                                                                                                                                                                                                                                                 // its name
                                        fLogicWorld,                                                                                                                                                                                                                                                                                                                         // its mother  volume
                                        false,                                                                                                                                                                                                                                                                                                                               // no boolean operation
                                        0);                                                                                                                                                                                                                                                                                                                                  // copy number

  G4VisAttributes *visAtt_Supp_catcher = new G4VisAttributes(G4Colour(1., 1., 1.));
  visAtt_Supp_catcher->SetVisibility(true);
  visAtt_Supp_catcher->SetForceWireframe(false);
  visAtt_Supp_catcher->SetForceSolid(true);
  logic_Supp_catcher->SetVisAttributes(visAtt_Supp_catcher);

  if (phys_Supp_catcher == NULL)
  {
  }

  G4Material *Mylar = G4NistManager::Instance()->FindOrBuildMaterial("G4_MYLAR");
  G4Material *Al = G4NistManager::Instance()->FindOrBuildMaterial("G4_Al");

  thicknessMylarSource = 0.5 * um;
  thicknessAlSource = 50. * nm;
  G4double outerRadius_MylarSource = radius_bucoCatcher;

  G4Tubs *AlSource1 = new G4Tubs("AlSource1", 0., outerRadius_MylarSource, thicknessAlSource / 2, 0., 360. * deg);
  G4LogicalVolume *Logic_AlSource1 = new G4LogicalVolume(AlSource1, Al, "LogicAlSource1"); // solid, material, name
  Physics_AlSource1 = new G4PVPlacement(0,                                                 // no rotation
                                        G4ThreeVector(0., 0., thicknessAlSource / 2),      // position
                                        Logic_AlSource1, "LogicAlSource1",                 // its fLogical volume
                                        fLogicWorld,                                       // its mother volume
                                        false,                                             // no boolean op.
                                        0);                                                // copy nb.

  MylarSource = new G4Tubs("MylarSource", 0., outerRadius_MylarSource, thicknessMylarSource / 2, 0., 360. * deg);
  G4LogicalVolume *Logic_MylarSource = new G4LogicalVolume(MylarSource, Mylar, "LogicMylarSource");            // solid, material, name
  Physics_MylarSource = new G4PVPlacement(0,                                                                   // no rotation
                                          G4ThreeVector(0., 0., thicknessMylarSource / 2 + thicknessAlSource), // position
                                          Logic_MylarSource, "Logic_MylarSource",                              // its fLogical volume
                                          fLogicWorld,                                                         // its mother volume
                                          false,                                                               // no boolean op.
                                          0);                                                                  // copy nb.

  G4Tubs *AlSource2 = new G4Tubs("AlSource2", 0., outerRadius_MylarSource, thicknessAlSource / 2, 0., 360. * deg);
  G4LogicalVolume *Logic_AlSource2 = new G4LogicalVolume(AlSource2, Al, "LogicAlSource2");                                       // solid, material, name
  Physics_AlSource2 = new G4PVPlacement(0,                                                                                       // no rotation
                                        G4ThreeVector(0., 0., thicknessAlSource / 2 + thicknessMylarSource + thicknessAlSource), // position
                                        Logic_AlSource2, "LogicAlSource2",                                                       // its fLogical volume
                                        fLogicWorld,                                                                             // its mother volume
                                        false,                                                                                   // no boolean op.
                                        0);                                                                                      // copy nb.

  G4VisAttributes *MylarSource_att = new G4VisAttributes(G4Colour(0.94, 0.5, 0.5)); // pink
  MylarSource_att->SetVisibility(true);
  MylarSource_att->SetForceWireframe(false);
  MylarSource_att->SetForceSolid(true);
  Logic_MylarSource->SetVisAttributes(MylarSource_att);

  G4VisAttributes *AlSource_att = new G4VisAttributes(G4Colour(0.94, 0.2, 0.5)); // pink
  AlSource_att->SetVisibility(true);
  AlSource_att->SetForceWireframe(false);
  AlSource_att->SetForceSolid(true);
  Logic_AlSource1->SetVisAttributes(AlSource_att);
  Logic_AlSource2->SetVisAttributes(AlSource_att);

  if (Physics_MylarSource == NULL)
  {
  }
  if (Physics_AlSource2 == NULL)
  {
  }
  if (Physics_AlSource1 == NULL)
  {
  }

  //=========================================================================================================================
  //========================================== SILICON DETECTORS _ COMMON PARAMETERS ========================================
  //=========================================================================================================================
  G4double a, density;
  G4String name, symbol;
  G4int nelements, natoms;

  // Il materiale che compone il supporto dei detector al silicio é il PCB. Non esiste nella libreria di G4, occorre fabbricarselo
  G4Element *elSi = new G4Element(name = "Silicon", symbol = "Si", z = 14., a = 28.0855 * g / mole);
  G4Element *elO = new G4Element(name = "Oxigen", symbol = "O", z = 8., a = 15.9994 * g / mole);
  materialSupportSiliconDetector = new G4Material(name = "Vetronite", density = 2. * g / cm3, nelements = 2); // vetro epossidico, ossia PCB
  materialSupportSiliconDetector->AddElement(elSi, natoms = 1);
  materialSupportSiliconDetector->AddElement(elO, natoms = 2);

  G4double z_height_Source_biggerBaseSiDet_inVerticale = 24.92 * mm;

  // Common parameteres to all Si detectors
  G4double pTheta = 0. * degree;
  G4double pPhi = 0. * degree;
  G4double pDx1 = 83.3 * mm;   // Lunghezza (x) della base maggiore del trapezio sulla prima faccia. Se le facce non sono svasate é uguale a pDx3
  G4double pDx2 = 31. * mm;    // Lunghezza (x) della base minore del trapezio sulla prima faccia. Se le facce non sono svasate é uguale a pDx4
  G4double pAlp1 = 0 * degree; // Angolo tra i centri delle due facce
  G4double pDy2 = 34.12 * mm;  // Altezza (y) del trapezio sulla prima faccia
  G4double pDx3 = 83.3 * mm;   // Lunghezza (x) della base maggiore del trapezio sulla prima faccia
  G4double pDx4 = 31. * mm;    // Lunghezza (x) della base minore del trapezio sulla prima faccia
  G4double pAlp2 = 0. * degree;

  supportSiliconDetector = new G4Trap("supportSiliconDetector",
                                      pDz / 2, pTheta, pPhi, pDy1 / 2,
                                      pDx1 / 2, pDx2 / 2, pAlp1, pDy2 / 2,
                                      pDx3 / 2, pDx4 / 2, pAlp2);

  SupportSiliconDetector_att = new G4VisAttributes(G4Colour(0.5, 0.5, 0.5)); // red (r,g,b) ->grey
  SupportSiliconDetector_att->SetForceWireframe(false);
  SupportSiliconDetector_att->SetForceSolid(true);

  //========================================================================================================================
  //============================================= PISTE DETECTOR SILICIO  ==================================================
  //========================================================================================================================

  materialSiliconDetector = G4NistManager::Instance()->FindOrBuildMaterial("G4_Si");
  SiliconDetector_att = new G4VisAttributes(G4Colour(1., 0., 0.)); // red (r,g,b) ->red
  SiliconDetector_att->SetForceWireframe(false);
  SiliconDetector_att->SetForceSolid(true);

  //___________ Supporto in rame della placca in PBC sulla quale é montato il Si detector  ____________
  material_SupportoRame_SiliconDetector = G4NistManager::Instance()->FindOrBuildMaterial("G4_Cu");
  G4VSolid *biggerBox_material_SupportoRame_SiliconDetector = new G4Box("biggerBox_material_SupportoRame_SiliconDetector", length_x_SupportoRame_SiDetector / 2, height_y_SupportoRame_SiDetector / 2, thickness_z_SupportoRame_SiDetector / 2);
  G4VSolid *smallerBox_material_SupportoRame_SiliconDetector = new G4Box("smallerBox_material_SupportoRame_SiliconDetector", x_smallBox_daTagliare_SupportoRame_SiDetector / 2, y_smallBox_daTagliare_SupportoRame_SiDetector / 2, thickness_z_SupportoRame_SiDetector);
  Box_material_SupportoRame_SiliconDetector = new G4SubtractionSolid("biggerBox_material_SupportoRame_SiliconDetector-smallerBox_material_SupportoRame_SiliconDetector",
                                                                     biggerBox_material_SupportoRame_SiliconDetector, smallerBox_material_SupportoRame_SiliconDetector,
                                                                     0,
                                                                     G4ThreeVector(length_x_SupportoRame_SiDetector / 2 - distanza_latoDxBoxTagliata_e_bordoDxSupportoRame_SiDetector - x_smallBox_daTagliare_SupportoRame_SiDetector / 2,
                                                                                   height_y_SupportoRame_SiDetector / 2 - y_smallBox_daTagliare_SupportoRame_SiDetector / 2,
                                                                                   0.));

  Box_material_SupportoRame_SiliconDetector_att = new G4VisAttributes(G4Colour(0.7, 0, 0.7));       //(r,g,b) => magenta
  Box_material_SupportoRame_SiliconDetector_attvide = new G4VisAttributes(G4Colour(0.5, 0.2, 0.8)); //(r,g,b) => magenta
  Box_material_SupportoRame_SiliconDetector_att->SetForceWireframe(false);
  Box_material_SupportoRame_SiliconDetector_att->SetForceSolid(true);

  // Support
  supportSiliconDetector = new G4Trap("supportSiliconDetector",
                                      pDz / 2, pTheta, pPhi, pDy1 / 2,
                                      pDx1 / 2, pDx2 / 2, pAlp1, pDy2 / 2,
                                      pDx3 / 2, pDx4 / 2, pAlp2);

  // Mother volume
  supportSiliconDetectorvide = new G4Trap("supportSiliconDetectorvide",
                                          (thicknessSiDetector) / 2, pTheta, pPhi, pDy1 / 2,
                                          pDx1 / 2, pDx2 / 2, pAlp1, pDy2 / 2,
                                          pDx3 / 2, pDx4 / 2, pAlp2);

  // Strip n. 5
  SiDet_Strip_5 = new G4Trap("SiDet_Strip_5",
                             thicknessSiDetector / 2, 0. * degree, 0. * degree, y_SiDet_Strip_5 / 2,
                             xHigh_SiDet_Strip_5 / 2, xLow_SiDet_Strip_5 / 2, 0. * degree, y_SiDet_Strip_5 / 2,
                             xHigh_SiDet_Strip_5 / 2, xLow_SiDet_Strip_5 / 2, 0. * degree);

  // Strip n. 4
  SiDet_Strip_4 = new G4Trap("SiDet_Strip_4",
                             thicknessSiDetector / 2, 0. * degree, 0. * degree, y_SiDet_Strip_4 / 2,
                             xHigh_SiDet_Strip_4 / 2, xLow_SiDet_Strip_4 / 2, 0. * degree, y_SiDet_Strip_4 / 2,
                             xHigh_SiDet_Strip_4 / 2, xLow_SiDet_Strip_4 / 2, 0. * degree);

  // Strip n. 3
  SiDet_Strip_3 = new G4Trap("SiDet_Strip_3",
                             thicknessSiDetector / 2, 0. * degree, 0. * degree, y_SiDet_Strip_3 / 2,
                             xHigh_SiDet_Strip_3 / 2, xLow_SiDet_Strip_3 / 2, 0. * degree, y_SiDet_Strip_3 / 2,
                             xHigh_SiDet_Strip_3 / 2, xLow_SiDet_Strip_3 / 2, 0. * degree);

  // Strip n. 2
  SiDet_Strip_2 = new G4Trap("SiDet_Strip_2",
                             thicknessSiDetector / 2, 0. * degree, 0. * degree, y_SiDet_Strip_2 / 2,
                             xHigh_SiDet_Strip_2 / 2, xLow_SiDet_Strip_2 / 2, 0. * degree, y_SiDet_Strip_2 / 2,
                             xHigh_SiDet_Strip_2 / 2, xLow_SiDet_Strip_2 / 2, 0. * degree);

  // Strip n. 1
  SiDet_Strip_1 = new G4Trap("SiDet_Strip_1",
                             thicknessSiDetector / 2, 0. * degree, 0. * degree, y_SiDet_Strip_1 / 2,
                             xHigh_SiDet_Strip_1 / 2, xLow_SiDet_Strip_1 / 2, 0. * degree, y_SiDet_Strip_1 / 2,
                             xHigh_SiDet_Strip_1 / 2, xLow_SiDet_Strip_1 / 2, 0. * degree);

  ////////////////////////////Dead Layer//////////////////////////////////

  // Strip n. 5
  SiDet_Strip_5_dl = new G4Trap("SiDet_Strip_5_dl",
                                thicknessSiDetectorDeadLayer / 2, 0. * degree, 0. * degree, y_SiDet_Strip_5 / 2,
                                xHigh_SiDet_Strip_5 / 2, xLow_SiDet_Strip_5 / 2, 0. * degree, y_SiDet_Strip_5 / 2,
                                xHigh_SiDet_Strip_5 / 2, xLow_SiDet_Strip_5 / 2, 0. * degree);

  // Strip n. 4
  SiDet_Strip_4_dl = new G4Trap("SiDet_Strip_4_dl",
                                thicknessSiDetectorDeadLayer / 2, 0. * degree, 0. * degree, y_SiDet_Strip_4 / 2,
                                xHigh_SiDet_Strip_4 / 2, xLow_SiDet_Strip_4 / 2, 0. * degree, y_SiDet_Strip_4 / 2,
                                xHigh_SiDet_Strip_4 / 2, xLow_SiDet_Strip_4 / 2, 0. * degree);

  // Strip n. 3
  SiDet_Strip_3_dl = new G4Trap("SiDet_Strip_3_dl",
                                thicknessSiDetectorDeadLayer / 2, 0. * degree, 0. * degree, y_SiDet_Strip_3 / 2,
                                xHigh_SiDet_Strip_3 / 2, xLow_SiDet_Strip_3 / 2, 0. * degree, y_SiDet_Strip_3 / 2,
                                xHigh_SiDet_Strip_3 / 2, xLow_SiDet_Strip_3 / 2, 0. * degree);

  // Strip n. 2
  SiDet_Strip_2_dl = new G4Trap("SiDet_Strip_2_dl",
                                thicknessSiDetectorDeadLayer / 2, 0. * degree, 0. * degree, y_SiDet_Strip_2 / 2,
                                xHigh_SiDet_Strip_2 / 2, xLow_SiDet_Strip_2 / 2, 0. * degree, y_SiDet_Strip_2 / 2,
                                xHigh_SiDet_Strip_2 / 2, xLow_SiDet_Strip_2 / 2, 0. * degree);

  // Strip n. 1
  SiDet_Strip_1_dl = new G4Trap("SiDet_Strip_1_dl",
                                thicknessSiDetectorDeadLayer / 2, 0. * degree, 0. * degree, y_SiDet_Strip_1 / 2,
                                xHigh_SiDet_Strip_1 / 2, xLow_SiDet_Strip_1 / 2, 0. * degree, y_SiDet_Strip_1 / 2,
                                xHigh_SiDet_Strip_1 / 2, xLow_SiDet_Strip_1 / 2, 0. * degree);

  dic_strip[1] = std::make_tuple(SiDet_Strip_1, G4ThreeVector(0, -pDy1 / 2 + spazio_tra_Bordo_e_strip5 + y_SiDet_Strip_5 + spazio_tra_Strip + y_SiDet_Strip_4 + spazio_tra_Strip + y_SiDet_Strip_3 + spazio_tra_Strip + y_SiDet_Strip_2 + spazio_tra_Strip + y_SiDet_Strip_1 / 2, 0.), SiDet_Strip_1_dl);
  dic_strip[2] = std::make_tuple(SiDet_Strip_2, G4ThreeVector(0, -pDy1 / 2 + spazio_tra_Bordo_e_strip5 + y_SiDet_Strip_5 + spazio_tra_Strip + y_SiDet_Strip_4 + spazio_tra_Strip + y_SiDet_Strip_3 + spazio_tra_Strip + y_SiDet_Strip_2 / 2, 0.), SiDet_Strip_2_dl);
  dic_strip[3] = std::make_tuple(SiDet_Strip_3, G4ThreeVector(0, -pDy1 / 2 + spazio_tra_Bordo_e_strip5 + y_SiDet_Strip_5 + spazio_tra_Strip + y_SiDet_Strip_4 + spazio_tra_Strip + y_SiDet_Strip_3 / 2, 0.), SiDet_Strip_3_dl);
  dic_strip[4] = std::make_tuple(SiDet_Strip_4, G4ThreeVector(0, -pDy1 / 2 + spazio_tra_Bordo_e_strip5 + y_SiDet_Strip_5 + spazio_tra_Strip + y_SiDet_Strip_4 / 2, 0.), SiDet_Strip_4_dl);
  dic_strip[5] = std::make_tuple(SiDet_Strip_5, G4ThreeVector(0, -pDy1 / 2 + spazio_tra_Bordo_e_strip5 + y_SiDet_Strip_5 / 2, 0.), SiDet_Strip_5_dl);

  // Loop for construct Silicon Detetors
  int index = 0;
  for (int i = 1; i < 5; i++)
  {
    for (const std::string &dir : directions)
    {
      tab[index] = Make_Sidet(i, dir, SiliconDetector_att, Box_material_SupportoRame_SiliconDetector_att,
                              SupportSiliconDetector_att, Box_material_SupportoRame_SiliconDetector_attvide, materialSiliconDetector, material_SupportoRame_SiliconDetector, materialSupportSiliconDetector, vide);
      index++;
    }
  }

  //===================================================================================================================================================================
  //===================================================================================================================================================================
  //==================================================   WISArD PLASTIC SCINTILLATOR - WITH SUPPORT ===================================================================
  //===================================================================================================================================================================
  //===================================================================================================================================================================
  // NB: per ogni dubbio sulle misure, consultare i disegni tecnici inviati da M. Roche (mail al mio indirizzo cenbg il 18/05/2021).
  G4double distanza_tra_BaseInfScintillatore_e_BordoSuperioreDeiSiDetector = 25.575 * mm;
  G4double delta = 0*cm;
  G4Material *fMaterial_PlasticScintillator = G4NistManager::Instance()->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE");

  G4Tubs *fSolid_PlasticScintillator = new G4Tubs("PlasticScintillator", 0., fRadius_PlasticScintillator, 0.5 * fLength_PlasticScintillator, 0., 360 * deg);                                                                           // name, r : 0->1cm, L : 5cm, phi : 0->2pi
  G4LogicalVolume *fLogic_PlasticScintillator = new G4LogicalVolume(fSolid_PlasticScintillator, fMaterial_PlasticScintillator, "PlasticScintillator");                                                                                 // solid, material, name
  G4PVPlacement *fPhys_PlasticScintillator = new G4PVPlacement(0,                                                                                                                                                                      // rotationMatrix
                                                               G4ThreeVector(0., 0., z_height_Source_biggerBaseSiDet_inVerticale + distanza_tra_BaseInfScintillatore_e_BordoSuperioreDeiSiDetector + fLength_PlasticScintillator / 2 + delta), // position
                                                               fLogic_PlasticScintillator, "PlasticScintillator",                                                                                                                      // its fLogical volume
                                                               fLogicWorld,                                                                                                                                                            // its mother volume
                                                               false,                                                                                                                                                                  // no boolean op.
                                                               0);                                                                                                                                                                   // copy nb.
  G4VisAttributes *PlasticScintillator_att = new G4VisAttributes(G4Colour(0., 0., 0.)); // red
  PlasticScintillator_att->SetForceWireframe(false);
  PlasticScintillator_att->SetForceSolid(true);
  fLogic_PlasticScintillator->SetVisAttributes(PlasticScintillator_att);

  if (fPhys_PlasticScintillator == NULL)
  {
  }

  G4double length_x_SupportoRame_PlasticScint = 20.5 * mm - 2 * mm;
  G4double height_y_SupportoRame_PlasticScint = 20.5 * mm - 2 * mm;
  G4double thickness_z_SupportoRame_PlasticScint = 1.5 * mm;

  G4Material *material_SupportoRame_PlasticScint = G4NistManager::Instance()->FindOrBuildMaterial("G4_Cu");
  G4VSolid *bigBox_material_SupportoRame_PlasticScint = new G4Box("bigBox_material_SupportoRame_PlasticScint", length_x_SupportoRame_PlasticScint, height_y_SupportoRame_PlasticScint, thickness_z_SupportoRame_PlasticScint / 2); // corretto non div per due x e y
  G4VSolid *holeForPlasticScint = new G4Tubs("holeForPlasticScint", 0., fRadius_PlasticScintillator, thickness_z_SupportoRame_PlasticScint, 0., 360 * deg);

  G4VSolid *Box_material_SupportoRame_PlasticScint = new G4SubtractionSolid("Box_material_SupportoRame_PlasticScint",
                                                                            bigBox_material_SupportoRame_PlasticScint, holeForPlasticScint,
                                                                            0,
                                                                            G4ThreeVector(0., 0., 0.));

  G4LogicalVolume *logic_Box_material_SupportoRame_PlasticScint = new G4LogicalVolume(Box_material_SupportoRame_PlasticScint, material_SupportoRame_PlasticScint, "logic_Box_material_SupportoRame_PlasticScint"); // solid, material, name
  G4VPhysicalVolume *physics_Box_material_SupportoRame_PlasticScint = new G4PVPlacement(0,                                                                                                                         // no rotation
                                                                                        G4ThreeVector(0., 0., delta +z_height_Source_biggerBaseSiDet_inVerticale + distanza_tra_BaseInfScintillatore_e_BordoSuperioreDeiSiDetector + thickness_z_SupportoRame_SiDetector / 2),
                                                                                        logic_Box_material_SupportoRame_PlasticScint, "logic_Box_material_SupportoRame_PlasticScint", // its fLogical volume
                                                                                        fLogicWorld,                                                                                  // its mother volume
                                                                                        false,                                                                                        // no boolean op.
                                                                                        0);                                                                                           // copy nb.

  G4VisAttributes *Box_material_SupportoRame_PlasticScint_att = new G4VisAttributes(G4Colour(0.7, 0, 0.7)); //(r,g,b) => magenta
  Box_material_SupportoRame_PlasticScint_att->SetForceWireframe(false);
  Box_material_SupportoRame_PlasticScint_att->SetForceSolid(true);
  logic_Box_material_SupportoRame_PlasticScint->SetVisAttributes(Box_material_SupportoRame_PlasticScint_att);

  if (physics_Box_material_SupportoRame_PlasticScint == NULL)
  {
  }

  G4double lenghtPlaccaCilidrica = fLength_PlasticScintillator - thickness_z_SupportoRame_SiDetector;

  G4VSolid *placcaRame_Cilindrica = new G4Tubs("placcaRame_Cilindrica", fRadius_PlasticScintillator, fRadius_PlasticScintillator + thickness_z_SupportoRame_PlasticScint, lenghtPlaccaCilidrica / 2, 0., 360 * deg);
  G4LogicalVolume *logic_placcaRame_Cilindrica = new G4LogicalVolume(placcaRame_Cilindrica, material_SupportoRame_PlasticScint, "placcaRame_Cilindrica"); // solid, material, name
  G4VPhysicalVolume *physics_placcaRame_Cilindrica = new G4PVPlacement(0,                                                                                 // no rotation
                                                                       G4ThreeVector(0., 0., delta +z_height_Source_biggerBaseSiDet_inVerticale + distanza_tra_BaseInfScintillatore_e_BordoSuperioreDeiSiDetector + thickness_z_SupportoRame_SiDetector + lenghtPlaccaCilidrica / 2),
                                                                       logic_placcaRame_Cilindrica, "logic_placcaRame_Cilindrica", // its fLogical volume
                                                                       fLogicWorld,                                                // its mother volume
                                                                       false,                                                      // no boolean op.
                                                                       0);

  logic_placcaRame_Cilindrica->SetVisAttributes(Box_material_SupportoRame_PlasticScint_att);
  if (physics_placcaRame_Cilindrica == NULL)
  {
  }

  //////// SET SENSITIVE DETECTOR OTHER THAN SiDet /////////
  fLogic_PlasticScintillator->SetSensitiveDetector(manager_ptr->GetWisardSensor_PlasticScintillator());
  Logic_MylarSource->SetSensitiveDetector(manager_ptr->GetWisardSensor_CatcherMylar());
  Logic_AlSource1->SetSensitiveDetector(manager_ptr->GetWisardSensor_CatcherAl1());
  Logic_AlSource2->SetSensitiveDetector(manager_ptr->GetWisardSensor_CatcherAl2());

  return fPhysiWorld;
}

void Wisard_Detector::SetSiDeadLayer_Thickness(G4double value)
{
  // Strip n. 5
  SiDet_Strip_5_dl->SetAllParameters(
      value / 2, 0. * degree, 0. * degree, y_SiDet_Strip_5 / 2,
      xHigh_SiDet_Strip_5 / 2, xLow_SiDet_Strip_5 / 2, 0. * degree, y_SiDet_Strip_5 / 2,
      xHigh_SiDet_Strip_5 / 2, xLow_SiDet_Strip_5 / 2, 0. * degree);

  // Strip n. 4
  SiDet_Strip_4_dl->SetAllParameters(
      value / 2, 0. * degree, 0. * degree, y_SiDet_Strip_4 / 2,
      xHigh_SiDet_Strip_4 / 2, xLow_SiDet_Strip_4 / 2, 0. * degree, y_SiDet_Strip_4 / 2,
      xHigh_SiDet_Strip_4 / 2, xLow_SiDet_Strip_4 / 2, 0. * degree);

  // Strip n. 3
  SiDet_Strip_3_dl->SetAllParameters(
      value / 2, 0. * degree, 0. * degree, y_SiDet_Strip_3 / 2,
      xHigh_SiDet_Strip_3 / 2, xLow_SiDet_Strip_3 / 2, 0. * degree, y_SiDet_Strip_3 / 2,
      xHigh_SiDet_Strip_3 / 2, xLow_SiDet_Strip_3 / 2, 0. * degree);

  // Strip n. 2
  SiDet_Strip_2_dl->SetAllParameters(
      value / 2, 0. * degree, 0. * degree, y_SiDet_Strip_2 / 2,
      xHigh_SiDet_Strip_2 / 2, xLow_SiDet_Strip_2 / 2, 0. * degree, y_SiDet_Strip_2 / 2,
      xHigh_SiDet_Strip_2 / 2, xLow_SiDet_Strip_2 / 2, 0. * degree);

  // Strip n. 1
  SiDet_Strip_1_dl->SetAllParameters(
      value / 2, 0. * degree, 0. * degree, y_SiDet_Strip_1 / 2,
      xHigh_SiDet_Strip_1 / 2, xLow_SiDet_Strip_1 / 2, 0. * degree, y_SiDet_Strip_1 / 2,
      xHigh_SiDet_Strip_1 / 2, xLow_SiDet_Strip_1 / 2, 0. * degree);

  int index = 0;
  for (int i = 1; i < 5; i++)
  {
    for (const std::string &dir : directions)
    {
      get<7>(tab[index]).second->SetTranslation(G4ThreeVector(0., 0., thicknessSiDetector / 2 - value / 2));
      get<8>(tab[index]).second->SetTranslation(G4ThreeVector(0., 0., thicknessSiDetector / 2 - value / 2));
      get<9>(tab[index]).second->SetTranslation(G4ThreeVector(0., 0., thicknessSiDetector / 2 - value / 2));
      get<10>(tab[index]).second->SetTranslation(G4ThreeVector(0., 0., thicknessSiDetector / 2 - value / 2));
      get<11>(tab[index]).second->SetTranslation(G4ThreeVector(0., 0., thicknessSiDetector / 2 - value / 2));
      index++;
    }
  }
  G4RunManager::GetRunManager()->GeometryHasBeenModified();
}

void Wisard_Detector::SetBFieldValue(G4double value)
{
  G4FieldManager *pFieldMgr;
  G4MagneticField *WisardMagField = new WisardMagnetField(GetInputNameB(), value);
  pFieldMgr = G4TransportationManager::GetTransportationManager()->GetFieldManager();
  G4ChordFinder *pChordFinder = new G4ChordFinder(WisardMagField);
  pChordFinder->SetDeltaChord(1 * um);
  pFieldMgr->SetChordFinder(pChordFinder);
  pFieldMgr->SetDetectorField(WisardMagField);
  G4RunManager::GetRunManager()->GeometryHasBeenModified();
}

void Wisard_Detector::SetCatcherPosition_z(G4double value)
{
  phys_munion_supportCatcher_Source->SetTranslation(G4ThreeVector(x_pos_bucoCatcher, -(y_baseCatcher_Spessa_1 / 2 + y_baseCatcher_Sottile_1 + y_baseCatcher_2 / 2), value - thickness_baseCatcher_Spessa_1 / 2 + thickness_baseCatcher_Sottile_1 + thickness_baseCatcher_Sottile_1 / 2 - thickness_supp_catcher));
  phys_Supp_catcher->SetTranslation(G4ThreeVector(0., -(y_baseCatcher_Spessa_1 / 2 + y_baseCatcher_Sottile_1 + y_baseCatcher_2 / 2) + y_pos_Catcher_risp_munion, value + thickness_baseCatcher_Spessa_1 / 2 + thickness_baseCatcher_Sottile_1 + thickness_baseCatcher_Sottile_1 / 2 - thickness_supp_catcher - thickness_baseCatcher_Spessa_1 / 2 - thickness_supp_catcher / 2));
  Physics_AlSource1->SetTranslation(G4ThreeVector(0., 0., value + thicknessAlSource / 2));
  Physics_MylarSource->SetTranslation(G4ThreeVector(0., 0., value + thicknessMylarSource / 2 + thicknessAlSource));
  Physics_AlSource2->SetTranslation(G4ThreeVector(0., 0., value + thicknessAlSource / 2 + thicknessMylarSource + thicknessAlSource));
  G4RunManager::GetRunManager()->GeometryHasBeenModified();
}

void Wisard_Detector::SetCatcher_Thickness(G4double value)
{
  phys_Supp_catcher->SetTranslation(G4ThreeVector(0., -(y_baseCatcher_Spessa_1 / 2 + y_baseCatcher_Sottile_1 + y_baseCatcher_2 / 2) + y_pos_Catcher_risp_munion, value + thickness_baseCatcher_Spessa_1 / 2 + thickness_baseCatcher_Sottile_1 + thickness_baseCatcher_Sottile_1 / 2 - thickness_supp_catcher - thickness_baseCatcher_Spessa_1 / 2 - thickness_supp_catcher / 2));
  Physics_AlSource2->SetTranslation(G4ThreeVector(0, 0, Physics_AlSource2->GetTranslation().z() - thicknessMylarSource + value));
  MylarSource->SetZHalfLength(value / 2);
  Physics_MylarSource->SetTranslation(G4ThreeVector(0, 0, Physics_MylarSource->GetTranslation().z() - thicknessMylarSource / 2 + value / 2));
  G4RunManager::GetRunManager()->GeometryHasBeenModified();
}
