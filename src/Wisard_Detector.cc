#include <unordered_map>
#include <utility>

#include "Wisard_Detector.hh"
#include "Wisard_MagnetField.hh"
#include "G4UserLimits.hh"

#include "Wisard_Sensor.hh"
#include "Wisard_Killer.hh"

//----------------------------------------------------------------------

Wisard_Detector::Wisard_Detector()
{
  G4cout << "\033[32m" << "Constructor Wisard_Detectors" << "\033[0m" << G4endl;

  pDz = 1.5 * mm;

  thicknessSiDetector = 300. * um;
  thicknessSiDetectorGrid = 700. * nm;
  WidthSiDetectorGrid = 30 * um;
  spazio_tra_Bordo_e_strip5 = 2.0 * mm; // 1.29 * mm + 0.225 * mm;
  spazio_tra_Strip = 70 * um;
  thetaInclinazione_SiDetector = 52.64 * degree;
  spazio_tra_Scintillatore_e_BordoSiDetector = 3.745 * mm;

  G4double a = WidthSiDetectorGrid / cos(90 * degree - thetaInclinazione_SiDetector);
  G4double b = spazio_tra_Strip / 2 / tan(thetaInclinazione_SiDetector) + WidthSiDetectorGrid / tan(thetaInclinazione_SiDetector);

  xLow_SiDet_Strip_1 = 25.65 * mm + 2 * b - a + 15 * um;
  y_SiDet_Strip_1 = 8.50 * mm + 2 * WidthSiDetectorGrid + spazio_tra_Strip;
  xHigh_SiDet_Strip_1 = xLow_SiDet_Strip_1 + 2 * ((y_SiDet_Strip_1) / tan(thetaInclinazione_SiDetector));

  xLow_SiDet_Strip_2 = xHigh_SiDet_Strip_1;
  y_SiDet_Strip_2 = 6.22 * mm + 2 * WidthSiDetectorGrid + spazio_tra_Strip;
  xHigh_SiDet_Strip_2 = xLow_SiDet_Strip_2 + 2 * ((y_SiDet_Strip_2) / tan(thetaInclinazione_SiDetector));

  xLow_SiDet_Strip_3 = xHigh_SiDet_Strip_2;
  y_SiDet_Strip_3 = 5.15 * mm + 2 * WidthSiDetectorGrid + spazio_tra_Strip;
  xHigh_SiDet_Strip_3 = xLow_SiDet_Strip_3 + 2 * ((y_SiDet_Strip_3) / tan(thetaInclinazione_SiDetector));

  xLow_SiDet_Strip_4 = xHigh_SiDet_Strip_3;
  y_SiDet_Strip_4 = 4.49 * mm + 2 * WidthSiDetectorGrid + spazio_tra_Strip;
  xHigh_SiDet_Strip_4 = xLow_SiDet_Strip_4 + 2 * ((y_SiDet_Strip_4) / tan(thetaInclinazione_SiDetector));

  xLow_SiDet_Strip_5 = xHigh_SiDet_Strip_4;
  y_SiDet_Strip_5 = 4.05 * mm + 2 * WidthSiDetectorGrid + spazio_tra_Strip;
  xHigh_SiDet_Strip_5 = xLow_SiDet_Strip_5 + 2 * ((y_SiDet_Strip_5) / tan(thetaInclinazione_SiDetector));

  y_Si_Bulk = y_SiDet_Strip_5 + y_SiDet_Strip_4 + y_SiDet_Strip_3 + y_SiDet_Strip_2 + y_SiDet_Strip_1;
  xHigh_Si_Bulk = xHigh_SiDet_Strip_5;
  xLow_Si_Bulk = xLow_SiDet_Strip_1;

  pDy1 = y_Si_Bulk + 2 * spazio_tra_Bordo_e_strip5;

  theta = (40.2) * degree; ////40deg
  z_height_Source_biggerBaseSiDet_inVerticale = 24.92 * mm;
  r = fRadius_PlasticScintillator + spazio_tra_Scintillatore_e_BordoSiDetector + pDy1 / 2 - (pDy1 / 2 * (1 - cos(theta))) - 2.7 * mm;
  r_vide = (pDz + thicknessSiDetector + thicknessSiDetectorGrid) / 2 * -sin(theta);
  z_vide = -(pDz + thicknessSiDetector + thicknessSiDetectorGrid) / 2 * cos(theta);
  z = z_height_Source_biggerBaseSiDet_inVerticale + pDz / 2 + ((pDy1 / 2) * sin(thetaInclinazione_SiDetector)) - 4.525 * mm;

  length_x_SupportoRame_SiDetector = 56. * mm;
  height_y_SupportoRame_SiDetector = 33.548 * mm;
  thickness_z_SupportoRame_SiDetector = 1.5 * mm;
  x_smallBox_daTagliare_SupportoRame_SiDetector = 10. * mm;
  y_smallBox_daTagliare_SupportoRame_SiDetector = 10. * mm;
  distanza_latoDxBoxTagliata_e_bordoDxSupportoRame_SiDetector = 3.2 * mm;

  // MATERIALS //
  G4NistManager *man = G4NistManager::Instance();
  Material_Vacuum = G4NistManager::Instance()->FindOrBuildMaterial("G4_Galactic");
  Material_Si = G4NistManager::Instance()->FindOrBuildMaterial("G4_Si");
  Material_SiO2 = G4NistManager::Instance()->FindOrBuildMaterial("G4_SILICON_DIOXIDE");
  Material_Al = G4NistManager::Instance()->FindOrBuildMaterial("G4_Al");
  Material_Mylar = G4NistManager::Instance()->FindOrBuildMaterial("G4_MYLAR");
  Material_Cu = G4NistManager::Instance()->FindOrBuildMaterial("G4_Cu");
  Material_Fe = G4NistManager::Instance()->FindOrBuildMaterial("G4_Fe");
  Material_Plastic = G4NistManager::Instance()->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE");
  PEEK = new G4Material("PEEKS", 1.32 * g / cm3, 3);
  PEEK->AddElement(man->FindOrBuildElement("C"), 19);
  PEEK->AddElement(man->FindOrBuildElement("H"), 12);
  PEEK->AddElement(man->FindOrBuildElement("O"), 3);
  Material_PCB = new G4Material("PCB", 1.86 * g / cm3, 4);
  Material_PCB->AddElement(man->FindOrBuildElement("H"), 10);
  Material_PCB->AddElement(man->FindOrBuildElement("C"), 8);
  Material_PCB->AddElement(man->FindOrBuildElement("O"), 2);
  Material_PCB->AddElement(man->FindOrBuildElement("Si"), 1);
  //////////////

  // VISUALIZATION //
  Vis_Vacuum = new G4VisAttributes(G4Colour(0.5, 0.5, 0.5));
  Vis_Vacuum->SetVisibility(false);
  Vis_Vacuum->SetForceWireframe(false);
  Vis_Vacuum->SetForceSolid(false);
  Vis_Si = new G4VisAttributes(G4Colour(0., 0., 0.));
  Vis_Si->SetVisibility(true);
  Vis_Si->SetForceWireframe(false);
  Vis_Si->SetForceSolid(true);
  Vis_SiO2 = new G4VisAttributes(G4Colour(0.19, 0.8, 0.19));
  Vis_SiO2->SetVisibility(true);
  Vis_SiO2->SetForceWireframe(false);
  Vis_SiO2->SetForceSolid(true);
  Vis_Al = new G4VisAttributes(G4Colour(0.5, 0.5, 0.5));
  Vis_Al->SetVisibility(true);
  Vis_Al->SetForceWireframe(false);
  Vis_Al->SetForceSolid(true);
  Vis_Mylar = new G4VisAttributes(G4Colour(1.0, 0.0, 1.0));
  Vis_Mylar->SetVisibility(true);
  Vis_Mylar->SetForceWireframe(false);
  Vis_Mylar->SetForceSolid(true);
  Vis_Cu = new G4VisAttributes(G4Colour(0.72, 0.45, 0.2));
  Vis_Cu->SetVisibility(true);
  Vis_Cu->SetForceWireframe(false);
  Vis_Cu->SetForceSolid(true);
  Vis_Fe = new G4VisAttributes(G4Colour(0.5, 0.5, 0.5));
  Vis_Fe->SetVisibility(true);
  Vis_Fe->SetForceWireframe(false);
  Vis_Fe->SetForceSolid(true);
  Vis_Plastic = new G4VisAttributes(G4Colour(0.6, 0.6, 0.6, 0.6));
  Vis_Plastic->SetVisibility(true);
  Vis_Plastic->SetForceWireframe(false);
  Vis_Plastic->SetForceSolid(true);
  Vis_PEEK = new G4VisAttributes(G4Colour(1., 1., 1.));
  Vis_PEEK->SetVisibility(true);
  Vis_PEEK->SetForceWireframe(false);
  Vis_PEEK->SetForceSolid(true);
  Vis_PCB = new G4VisAttributes(G4Colour(0.19, 0.8, 0.19));
  Vis_PCB->SetVisibility(true);
  Vis_PCB->SetForceWireframe(false);
  Vis_PCB->SetForceSolid(true);
  ///////////////////

  GeometryMessenger = new G4GenericMessenger(this, "/Geometry/", "All Geometry Settings");

  GeometryMessenger->DeclarePropertyWithUnit("SiDeadLayer_Thickness", "nm", thicknessSiDetectorDeadLayer)
      .SetGuidance("Set silicon detector thickness.")
      .SetParameterName("SiDeadLayer_Thickness", false)
      .SetDefaultValue("100 nm");

  GeometryMessenger->DeclarePropertyWithUnit("Magnetic_Field_Value", "tesla", Magnetic_Field)
      .SetGuidance("Set Magnetic Field.")
      .SetParameterName("Magnetic_Field value", false)
      .SetDefaultValue("4 tesla");

  GeometryMessenger->DeclareProperty("Magnetic_Field_Mapping", Magnetic_Field_Mapping_flag)
      .SetGuidance("Set Magnetic Field map.")
      .SetParameterName("Magnetic_Field", false)
      .SetDefaultValue("false");

  GeometryMessenger->DeclareProperty("Collimator", Collimator_flag)
      .SetGuidance("Set Collimator.")
      .SetParameterName("Collimator_flag", false)
      .SetDefaultValue("true");

  GeometryMessenger->DeclarePropertyWithUnit("Catcher_Position_z", "mm", Catcher_Position_z)
      .SetGuidance("Set Catcher Position z.")
      .SetParameterName("Catcher_Position_z", false)
      .SetDefaultValue("0 mm");

  GeometryMessenger->DeclareProperty("Catcher_Position", Catcher_Position)
      .SetGuidance("Set Catcher Position.")
      .SetParameterName("Catcher_Position", false)
      .SetDefaultValue("catcher1");

  GeometryMessenger->DeclarePropertyWithUnit("Catcher_Angle", "deg", Catcher_Angle)
      .SetGuidance("Set Catcher Angle.")
      .SetParameterName("Catcher_Angle", false)
      .SetDefaultValue("0 deg");

  GeometryMessenger->DeclarePropertyWithUnit("Catcher_Thickness_Al1", "nm", Catcher_Thickness_Al1)
      .SetGuidance("Set Catcher Thickness Al.")
      .SetParameterName("Catcher_Thickness_Al1", false)
      .SetDefaultValue("50 nm");

  GeometryMessenger->DeclarePropertyWithUnit("Catcher_Thickness_Mylar", "nm", Catcher_Thickness_Mylar)
      .SetGuidance("Set Catcher Thickness Mylar.")
      .SetParameterName("Catcher_Thickness_Mylar", false)
      .SetDefaultValue("500 nm");

  GeometryMessenger->DeclarePropertyWithUnit("Catcher_Thickness_Al2", "nm", Catcher_Thickness_Al2)
      .SetGuidance("Set Catcher Thickness Al.")
      .SetParameterName("Catcher_Thickness_Al2", false)
      .SetDefaultValue("50 nm");


  G4double xc = 0 * cm;
  G4double yc = 0 * cm;
  G4double zc = 0 * cm;

  dic_correction["D1"] = G4ThreeVector(xc, yc, zc);
  dic_correction["D2"] = G4ThreeVector(yc, -xc, zc);
  dic_correction["D3"] = G4ThreeVector(-xc, -yc, zc);
  dic_correction["D4"] = G4ThreeVector(-yc, xc, zc);
  dic_correction["D5"] = G4ThreeVector(-xc, yc, -zc);
  dic_correction["D6"] = G4ThreeVector(yc, xc, -zc);
  dic_correction["D7"] = G4ThreeVector(xc, -yc, -zc);
  dic_correction["D8"] = G4ThreeVector(-yc, -xc, -zc);

  dic_position["D1"] = G4ThreeVector(0, r, z);
  dic_position["D2"] = G4ThreeVector(r, 0, z);
  dic_position["D3"] = G4ThreeVector(0, -r, z);
  dic_position["D4"] = G4ThreeVector(-r, 0, z);
  dic_position["D5"] = G4ThreeVector(0, r, -z);
  dic_position["D6"] = G4ThreeVector(r, 0, -z);
  dic_position["D7"] = G4ThreeVector(0, -r, -z);
  dic_position["D8"] = G4ThreeVector(-r, 0, -z);

  dic_positionvide["D1"] = G4ThreeVector(0, r_vide, z_vide);
  dic_positionvide["D2"] = G4ThreeVector(r_vide, 0, z_vide);
  dic_positionvide["D3"] = G4ThreeVector(0, -r_vide, z_vide);
  dic_positionvide["D4"] = G4ThreeVector(-r_vide, 0, z_vide);
  dic_positionvide["D5"] = G4ThreeVector(0, r_vide, -z_vide);
  dic_positionvide["D6"] = G4ThreeVector(r_vide, 0, -z_vide);
  dic_positionvide["D7"] = G4ThreeVector(0, -r_vide, -z_vide);
  dic_positionvide["D8"] = G4ThreeVector(-r_vide, 0, -z_vide);

  dic_rotate["D1"] = std::make_tuple(theta, 180. * deg, 180 * deg);
  dic_rotate["D2"] = std::make_tuple(0, 180. * deg - theta, 90. * deg);
  dic_rotate["D3"] = std::make_tuple(-theta, 180. * deg, 0. * deg);
  dic_rotate["D4"] = std::make_tuple(0., 180. * deg + theta, -90. * deg);
  dic_rotate["D5"] = std::make_tuple(-theta, 0. * deg, 180. * deg);
  dic_rotate["D6"] = std::make_tuple(180 * deg, 180 * deg - theta, 90. * deg);
  dic_rotate["D7"] = std::make_tuple(theta, 0., 0.);
  dic_rotate["D8"] = std::make_tuple(180. * deg, 180. * deg + theta, -90. * deg);

  for (int i = 1; i <= 8; i++)
  {
    GeometryMessenger->DeclareProperty(("D"+to_string(i)).c_str(), string_pos[i])
      .SetGuidance("Set Detector position correction.")
      .SetDefaultValue("0 0 0");

    GeometryMessenger->DeclarePropertyWithUnit(("D"+to_string(i)+"_Angle").c_str(), "deg", Angle_Correction[i])
      .SetGuidance("Set Detector angle correction.")
      .SetDefaultValue("0 degree");
  }

  

  myStepLimit = new G4UserLimits();
  myStepLimit->SetMaxAllowedStep(10 * nm);
}

// destructor
Wisard_Detector::~Wisard_Detector()
{
 G4cout << "\033[31m" << "Destructor Wisard_Detector"  << "\033[0m" << G4endl;
}


void Wisard_Detector::ConstructSDandField()
{
  G4FieldManager *pFieldMgr;
  G4MagneticField *WisardMagField = nullptr;
  if (Magnetic_Field_Mapping_flag)
    WisardMagField = new WisardMagnetField(Magnetic_Field); /// NON UNIFORM MAG FIELD GETFIELDVALUE method
  else
    WisardMagField = new G4UniformMagField(G4ThreeVector(0., 0., Magnetic_Field));

  pFieldMgr = G4TransportationManager::GetTransportationManager()->GetFieldManager();
  G4ChordFinder *pChordFinder = new G4ChordFinder(WisardMagField);
  pChordFinder->SetDeltaChord(0.01 * mm);
  pFieldMgr->SetChordFinder(pChordFinder);
  pFieldMgr->SetMinimumEpsilonStep(1e-5);
  pFieldMgr->SetMaximumEpsilonStep(1e-5);
  pFieldMgr->SetDetectorField(WisardMagField);
  

  auto wisard_sensor_PlasticScintillator = new Wisard_Sensor(99, "PlasticScintillator");
  SetSensitiveDetector(fLogic_PlasticScintillator, wisard_sensor_PlasticScintillator);

  for (int i = 1; i <= 8; i++)
  {
    for (int j = 1; j <= 5; j++)
    {
      G4int detcode = i * 10 + j;
      for (int k = 0; k < nb_det; k++)
      {
        if (Detector_Code[k] == detcode)
        {
          auto wisard_sensor_SiliconDetector = new Wisard_Sensor(detcode, Detector_Name[k]);
          SetSensitiveDetector(tab[i-1][1+j].first, wisard_sensor_SiliconDetector);

          auto wisard_sensor_SiliconDetectordl = new Wisard_Sensor(detcode, Detector_Name[k] + "_DL");
          SetSensitiveDetector(tab[i-1][6+j].first, wisard_sensor_SiliconDetectordl);
          break;
        }
      }
    }
  }

  auto wisard_sensor_CatcherMylar_central = new Wisard_Sensor(1, "CatcherMylar_central");
  SetSensitiveDetector(fLogic_MylarSource_central, wisard_sensor_CatcherMylar_central);
  auto wisard_sensor_CatcherAl1_central = new Wisard_Sensor(2, "CatcherAl1_central");
  SetSensitiveDetector(fLogic_AlSource1_central, wisard_sensor_CatcherAl1_central);
  auto wisard_sensor_CatcherAl2_central = new Wisard_Sensor(3, "CatcherAl2_central");
  SetSensitiveDetector(fLogic_AlSource2_central, wisard_sensor_CatcherAl2_central);
  auto wisard_sensor_CatcherMylar_side = new Wisard_Sensor(4, "CatcherMylar_side");
  SetSensitiveDetector(fLogic_MylarSource_side, wisard_sensor_CatcherMylar_side);
  auto wisard_sensor_CatcherAl1_side = new Wisard_Sensor(5, "CatcherAl1_side");
  SetSensitiveDetector(fLogic_AlSource1_side, wisard_sensor_CatcherAl1_side);
  auto wisard_sensor_CatcherAl2_side = new Wisard_Sensor(6, "CatcherAl2_side");
  SetSensitiveDetector(fLogic_AlSource2_side, wisard_sensor_CatcherAl2_side);

  if (!Magnetic_Field_Mapping_flag)
  {
    auto wisard_killer = new Wisard_Killer();
    SetSensitiveDetector(fLogic_Killer, wisard_killer);
  }
}
//----------------------------------------------------------------------

G4VPhysicalVolume *Wisard_Detector::Construct()
{
  G4GeometryManager::GetInstance()->OpenGeometry();
  G4PhysicalVolumeStore::GetInstance()->Clean();
  G4LogicalVolumeStore::GetInstance()->Clean();
  G4SolidStore::GetInstance()->Clean();
 
  bool BeamLineVisibility = true;  

  

  

  //--------------------------------------------------------------------------------------
  //------------------------------------ World -------------------------------------------
  //--------------------------------------------------------------------------------------
  G4double innerRadius = 0 * cm;
  G4double outerRadius = 6.5 * cm; // réduit au raypon du Bore pour opti6.5
  G4double length = 210. * cm;      // réduit pour opti21cm
  G4double theta1 = 90.0 * deg;
  G4double phi = 360.0 * deg;

  fSolidWorld = new G4Tubs("World", innerRadius, outerRadius, length / 2, theta1, phi);

  fLogicWorld = new G4LogicalVolume(fSolidWorld, // its solid
                                    Material_Vacuum,        // its material
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

  // G4double maxStep = 1*km;
  // G4UserLimits *myStepLimit = new G4UserLimits();
  // myStepLimit->SetUserMaxTrackLength(maxStep);
  // fLogicWorld->SetUserLimits(myStepLimit);

  //==================================================================================================
  //========================================  WISArD MAGNET =========================================
  //==================================================================================================
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
                                                      Material_Fe,  // matiere
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
  G4VisAttributes *visAtt_WISARDMagnet = new G4VisAttributes(G4Colour(0.5, 0.7, 0.7, 0.2));
  visAtt_WISARDMagnet->SetVisibility(BeamLineVisibility);
  visAtt_WISARDMagnet->SetForceWireframe(false);
  visAtt_WISARDMagnet->SetForceSolid(true);
  wisard_logic->SetVisAttributes(visAtt_WISARDMagnet);

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
  visAttr_Bore->SetVisibility(BeamLineVisibility);
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
  G4double thetacylinder = 90.0 * deg;
  phi = 360.0 * deg;

  G4Material *fMaterial_Al = G4NistManager::Instance()->FindOrBuildMaterial("G4_Al");
  G4Tubs *fSolidMYCILINDER = new G4Tubs("MYCILINDER", innerRadius, outerRadius, length / 2, thetacylinder, phi);

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
  visAtt_MYCILINDER->SetVisibility(BeamLineVisibility);
  fLogicMYCILINDER->SetVisAttributes(visAtt_MYCILINDER);

  if (fPhysiMYCILINDER == NULL)
  {
  }

  // BASE 1
  innerRadius = 0 * mm;
  outerRadius = 72 * cm;
  G4double lengthB = 1. * cm;
  thetacylinder = 90.0 * deg;
  phi = 360.0 * deg;

  G4Tubs *fSolidMYCILINDER_B1 = new G4Tubs("MYCILINDER_B1", innerRadius, outerRadius, lengthB / 2, thetacylinder, phi);

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
  visAtt_MYCILINDER_B1->SetVisibility(BeamLineVisibility);
  fLogicMYCILINDER_B1->SetVisAttributes(visAtt_MYCILINDER_B1);

  if (fPhysiMYCILINDER_B1 == NULL)
  {
  }

  // BASE 2
  G4Tubs *fSolidMYCILINDER_B2 = new G4Tubs("MYCILINDER_B2", innerRadius, outerRadius, lengthB / 2, thetacylinder, phi);

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
  visAtt_MYCILINDER_B2->SetVisibility(BeamLineVisibility);
  fLogicMYCILINDER_B2->SetVisAttributes(visAtt_MYCILINDER_B2);

  if (fPhysiMYCILINDER_B2 == NULL)
  {
  }

  // SUPPORT CATCHER 2024 //
  ///////////////ELEMENTS//////////////
  // G4double outerRadius = 6.5 * cm;
  /////////// Position ///////////
  G4RotationMatrix *myRotation = new G4RotationMatrix();
  myRotation->rotateX(0. * deg);
  myRotation->rotateY(0. * deg);

  if (Catcher_Position == "source")
  {
    myRotation->rotateZ(30 * deg + Catcher_Angle);
  }
  else if (Catcher_Position == "catcher2")
  {
    myRotation->rotateZ(Catcher_Angle);
  }
  else if (Catcher_Position == "catcher1")
  {
    myRotation->rotateZ(-22.5 * deg + Catcher_Angle);
  }
  else
  {
    G4Exception("Wisard_Detector", ("Wrong Catcher position : " + Catcher_Position).c_str(), JustWarning, "");
  }
  ///////////////////////
  
  // MOTHER CATCHER
  G4double Radius_Rotation = 50 * mm;
  G4Tubs *mother_catcher = new G4Tubs("mother", 0., outerRadius * 5, 5 * mm, 0., 360. * deg);
  G4LogicalVolume *logic_mother_catcher = new G4LogicalVolume(mother_catcher, Material_Vacuum, "logic_mother_catcher");
  phys_mother_catcher = new G4PVPlacement(myRotation, // no rotation
                                          G4ThreeVector(0, -Radius_Rotation, Catcher_Position_z),
                                          logic_mother_catcher, // its fLogical volume
                                          "mothercatcher",      // its name
                                          fLogicWorld,          // its mother  volume
                                          false,                // no boolean operation
                                          0);
  
  // logic_mother_catcher->SetUserLimits(myStepLimit);

  G4double SuppCatcher_Tige_width = 20 * mm;
  G4double SuppCatcher_Tige_height = 31.2 * mm;
  G4double SuppCatcher_Catcher_radius_inner = 7.5 * mm;
  G4double SuppCatcher_Catcher_radius_outer = 9 * mm;
  G4double SuppCatcher_Source_radius_inner = 4.5 * mm;
  G4double SuppCatcher_Source_radius_outer = 5.5 * mm;
  PEEK_thikness = 1.5 * mm;

  /// Main Plate
  G4double L1 = 30 * mm;
  G4double L2 = 31 * mm;
  G4double H = 30 * mm;
  G4double c = 10 * mm;
  G4double cc = 4 * mm;
  G4double l = 20 * mm;
  G4double h = 9 * mm;
  G4double Hl = 11 * mm;

  G4double SuppCatcher_Plate_height = H;

  std::vector<G4TwoVector> polygon(10);

  polygon[0].set(-L2 + c, -H / 2);
  polygon[1].set(-L2, -H / 2 + c);
  polygon[2].set(-L2, H / 2 + -Hl - cc);
  polygon[3].set(-L2 + cc, H / 2 + -Hl);
  polygon[4].set(-L2 + l, -H / 2 + h + c);
  polygon[5].set(-L2 + l, H / 2);
  polygon[6].set(L1 - c, H / 2);
  polygon[7].set(L1, H / 2 - c);
  polygon[8].set(L1, -H / 2 + c);
  polygon[9].set(L1 - c, -H / 2);

  G4TwoVector offsetA(0, 0), offsetB(0, 0);
  G4double scaleA = 1, scaleB = 1;
  G4VSolid *SuppCatcher_Plate = new G4ExtrudedSolid("Extruded", polygon, SuppCatcher_thikness / 2, offsetA, scaleA, offsetB, scaleB);

  // PEEK ring
  G4Tubs *PEEK_Ring = new G4Tubs("PEEK_ring", SuppCatcher_Catcher_radius_inner, SuppCatcher_Catcher_radius_outer, PEEK_thikness / 2, 0., 360 * deg);

  // Catcher Hole Inner
  G4Tubs *SuppCatcher_Catcher_inner = new G4Tubs("Catcher_inner", 0., SuppCatcher_Catcher_radius_inner, SuppCatcher_thikness, 0., 360. * deg);

  // Catcher Hole Outer
  G4Tubs *SuppCatcher_Catcher_outer = new G4Tubs("Catcher_outer", 0., SuppCatcher_Catcher_radius_outer, PEEK_thikness / 2, 0., 360. * deg);

  // Source Hole Inner
  G4Tubs *SuppCatcher_Source_inner = new G4Tubs("Catcher_inner", 0., SuppCatcher_Source_radius_inner, SuppCatcher_thikness, 0., 360. * deg);

  // Source Hole Outer
  G4Tubs *SuppCatcher_Source_outer = new G4Tubs("Catcher_outer", 0., SuppCatcher_Source_radius_outer, PEEK_thikness / 2, 0., 360. * deg);

  // Tige
  G4Box *SuppCatcher_Tige = new G4Box("SuppCatcher_Tige", SuppCatcher_Tige_width / 2, SuppCatcher_Tige_height / 2, SuppCatcher_thikness / 2);

  ///////////ASSEMBLY////////////
  Support_Position = G4ThreeVector(0, -Radius_Rotation * (1 - cos(22.5 * deg)) + Radius_Rotation, -SuppCatcher_thikness / 2);
  // Central Catcher Hole
  Central_Hole_Position = G4ThreeVector(0, Radius_Rotation * (1 - cos(22.5 * deg)), 0);
  G4VSolid *SuppCatcher_Plate_centralCatcher_inner = new G4SubtractionSolid("SuppCatcher_Plate-centralCatcher", SuppCatcher_Plate, SuppCatcher_Catcher_inner, 0, Central_Hole_Position);
  G4VSolid *SuppCatcher_Plate_centralCatcher = new G4SubtractionSolid("SuppCatcher_Plate-centralCatcher", SuppCatcher_Plate_centralCatcher_inner, SuppCatcher_Catcher_outer, 0, Central_Hole_Position + G4ThreeVector(0, 0, SuppCatcher_thikness / 2 - PEEK_thikness / 2));

  G4LogicalVolume *logic_centralPEEK_Ring = new G4LogicalVolume(PEEK_Ring, PEEK, "logic_Supp_catcher");
  phys_centralPEEK_ring = new G4PVPlacement(0, // no rotation
                                            Support_Position + Central_Hole_Position + G4ThreeVector(0, 0, SuppCatcher_thikness / 2 - PEEK_thikness / 2),
                                            logic_centralPEEK_Ring, // its fLogical volume
                                            "centralPEEK_Ring",     // its name
                                            logic_mother_catcher,   // its mother  volume
                                            false,                  // no boolean operation
                                            0);

  logic_centralPEEK_Ring->SetVisAttributes(Vis_PEEK);

  // Side Catcher Hole
  Side_Hole_Position = G4ThreeVector(Radius_Rotation * sin(22.5 * deg), 0, 0);
  G4VSolid *SuppCatcher_Plate_centralsideCatcher_inner = new G4SubtractionSolid("SuppCatcher_Plate-sideCatcher", SuppCatcher_Plate_centralCatcher, SuppCatcher_Catcher_inner, 0, Side_Hole_Position);
  G4VSolid *SuppCatcher_Plate_centralsideCatcher = new G4SubtractionSolid("SuppCatcher_Plate-sideCatcher", SuppCatcher_Plate_centralsideCatcher_inner, SuppCatcher_Catcher_outer, 0, Side_Hole_Position + G4ThreeVector(0, 0, SuppCatcher_thikness / 2 - PEEK_thikness / 2));

  G4LogicalVolume *logic_sidePEEK_Ring = new G4LogicalVolume(PEEK_Ring, PEEK, "logic_Supp_catcher");
  phys_sidePEEK_ring = new G4PVPlacement(0, // no rotation
                                         Support_Position + Side_Hole_Position + G4ThreeVector(0, 0, SuppCatcher_thikness / 2 - PEEK_thikness / 2),
                                         logic_sidePEEK_Ring,  // its fLogical volume
                                         "sidePEEK_Ring",      // its name
                                         logic_mother_catcher, // its mother  volume
                                         false,                // no boolean operation
                                         0);

  logic_sidePEEK_Ring->SetVisAttributes(Vis_PEEK);

  // Source Hole
  G4ThreeVector Source_Hole_Position = G4ThreeVector(-Radius_Rotation * sin(30 * deg), Radius_Rotation * (cos(30 * deg) - cos(22.5 * deg)), 0);
  G4VSolid *SuppCatcher_Plate_centralsidesourceCatcher_inner = new G4SubtractionSolid("SuppCatcher_Plate-source", SuppCatcher_Plate_centralsideCatcher, SuppCatcher_Source_inner, 0, Source_Hole_Position);
  G4VSolid *SuppCatcher_Plate_centralsidesourceCatcher = new G4SubtractionSolid("SuppCatcher_Plate-source", SuppCatcher_Plate_centralsidesourceCatcher_inner, SuppCatcher_Source_outer, 0, Source_Hole_Position + G4ThreeVector(0, 0, SuppCatcher_thikness / 2 - PEEK_thikness / 2));

  // Tige
  G4ThreeVector Tige_Position = G4ThreeVector(0, -SuppCatcher_Tige_height / 2 - SuppCatcher_Plate_height / 2, 0);
  G4UnionSolid *SuppCatcher_Plate_Tige = new G4UnionSolid("SuppCatcher_Plate_Tige", SuppCatcher_Plate_centralsidesourceCatcher, SuppCatcher_Tige, 0, Tige_Position);

  // Plate
  G4LogicalVolume *logic_SuppCatcher_Plate = new G4LogicalVolume(SuppCatcher_Plate_Tige, Material_Al, "logic_Supp_catcher");
  phys_Supp_catcher = new G4PVPlacement(0, // no rotation
                                        Support_Position,
                                        logic_SuppCatcher_Plate,  // its fLogical volume
                                        "phys_SuppCatcher_Plate", // its name
                                        logic_mother_catcher,     // its mother  volume
                                        false,                    // no boolean operation
                                        0);

  logic_SuppCatcher_Plate->SetVisAttributes(Vis_Al);

  //// CATCHER //////
  /// CENTRAL
  Catcher_central_Position = Support_Position + Central_Hole_Position + G4ThreeVector(0, 0, SuppCatcher_thikness / 2);
  Catcher_central_Position = G4ThreeVector(Support_Position.x() + Central_Hole_Position.x(), Support_Position.y() + Central_Hole_Position.y(), 0);

  AlSource1 = new G4Tubs("AlSource1_central", 0., SuppCatcher_Catcher_radius_inner, Catcher_Thickness_Al1 / 2, 0., 360. * deg);
  fLogic_AlSource1_central = new G4LogicalVolume(AlSource1, Material_Al, "LogicAlSource1_central");               // solid, material, name
  Physics_AlSource1_central = new G4PVPlacement(0,                                                                       // no rotation
                                                Catcher_central_Position + G4ThreeVector(0., 0., Catcher_Thickness_Al1 / 2), // position
                                                fLogic_AlSource1_central, "LogicAlSource1_central",                       // its fLogical volume
                                                logic_mother_catcher,                                                    // its mother volume
                                                false,                                                                   // no boolean op.
                                                0,                                                                       // copy nb.
                                                false);                                                                  // copy nb.
  // fLogic_AlSource1_central->SetUserLimits(myStepLimit);

  MylarSource_central = new G4Tubs("MylarSource", 0., SuppCatcher_Catcher_radius_inner, Catcher_Thickness_Mylar / 2, 0., 360. * deg);
  fLogic_MylarSource_central = new G4LogicalVolume(MylarSource_central, Material_Mylar, "LogicMylarSource_central");                               // solid, material, name
  Physics_MylarSource_central = new G4PVPlacement(0,                                                                                                      // no rotation
                                                  Catcher_central_Position + G4ThreeVector(0., 0., Catcher_Thickness_Mylar / 2 + Catcher_Thickness_Al1), // position
                                                  fLogic_MylarSource_central, "Logic_MylarSource_central",                                                 // its fLogical volume
                                                  logic_mother_catcher,                                                                                   // its mother volume
                                                  false,                                                                                                  // no boolean op.
                                                  0,                                                                                                      // copy nb.
                                                  false);                                                                                                 // copy nb.

  AlSource2 = new G4Tubs("AlSource2_central", 0., SuppCatcher_Catcher_radius_inner, Catcher_Thickness_Al2 / 2, 0., 360. * deg);
  fLogic_AlSource2_central = new G4LogicalVolume(AlSource2, Material_Al, "LogicAlSource2_central");                                                                  // solid, material, name
  Physics_AlSource2_central = new G4PVPlacement(0,                                                                                                                          // no rotation
                                                Catcher_central_Position + G4ThreeVector(0., 0., Catcher_Thickness_Al1 / 2 + Catcher_Thickness_Mylar + Catcher_Thickness_Al2), // position
                                                fLogic_AlSource2_central, "LogicAlSource2_central",                                                                          // its fLogical volume
                                                logic_mother_catcher,                                                                                                       // its mother volume
                                                false,                                                                                                                      // no boolean op.
                                                0,                                                                                                                          // copy nb.
                                                false);                                                                                                                     // copy nb.


  fLogic_MylarSource_central->SetVisAttributes(Vis_Mylar);
  fLogic_AlSource1_central->SetVisAttributes(Vis_Al);
  fLogic_AlSource2_central->SetVisAttributes(Vis_Al);

  /// SIDE
  Catcher_side_Position = Support_Position + Side_Hole_Position + G4ThreeVector(0, 0, SuppCatcher_thikness / 2);

  fLogic_AlSource1_side = new G4LogicalVolume(AlSource1, Material_Al, "LogicAlSource1_side");               // solid, material, name
  Physics_AlSource1_side = new G4PVPlacement(0,                                                                    // no rotation
                                             Catcher_side_Position + G4ThreeVector(0., 0., Catcher_Thickness_Al1 / 2), // position
                                             fLogic_AlSource1_side, "LogicAlSource1_side",                          // its fLogical volume
                                             logic_mother_catcher,                                                 // its mother volume
                                             false,                                                                // no boolean op.
                                             0,                                                                    // copy nb.
                                             false);
  // fLogic_AlSource1_side->SetUserLimits(myStepLimit);

  MylarSource_side = new G4Tubs("MylarSource", 0., SuppCatcher_Catcher_radius_inner, Catcher_Thickness_Al1 / 2, 0., 360. * deg);
  fLogic_MylarSource_side = new G4LogicalVolume(MylarSource_side, Material_Mylar, "LogicMylarSource_side");                               // solid, material, name
  Physics_MylarSource_side = new G4PVPlacement(0,                                                                                                // no rotation
                                               Catcher_side_Position + G4ThreeVector(0., 0., Catcher_Thickness_Al1 / 2 + Catcher_Thickness_Al1), // position
                                               fLogic_MylarSource_side, "Logic_MylarSource_side",                                                 // its fLogical volume
                                               logic_mother_catcher,                                                                             // its mother volume
                                               false,                                                                                            // no boolean op.
                                               0,                                                                                                // copy nb.
                                               false);
  fLogic_AlSource2_side = new G4LogicalVolume(AlSource2, Material_Al, "LogicAlSource2_side");                                                               // solid, material, name
  Physics_AlSource2_side = new G4PVPlacement(0,                                                                                                                    // no rotation
                                             Catcher_side_Position + G4ThreeVector(0., 0., Catcher_Thickness_Al1 / 2 + Catcher_Thickness_Al1 + Catcher_Thickness_Al2), // position
                                             fLogic_AlSource2_side, "LogicAlSource2_side",                                                                          // its fLogical volume
                                             logic_mother_catcher,                                                                                                 // its mother volume
                                             false,                                                                                                                // no boolean op.
                                             0,                                                                                                                    // copy nb.
                                             false);
  fLogic_MylarSource_side->SetVisAttributes(Vis_Mylar);
  fLogic_AlSource1_side->SetVisAttributes(Vis_Al);
  fLogic_AlSource2_side->SetVisAttributes(Vis_Al);

  /// SOURCE
  G4double thicknessSource = 0.1 * mm;
  Source_Position = Support_Position + Source_Hole_Position + G4ThreeVector(0, 0, SuppCatcher_thikness / 2 - PEEK_thikness);

  Source = new G4Tubs("Source", 0., SuppCatcher_Source_radius_inner, thicknessSource / 2, 0., 360. * deg);
  G4LogicalVolume *Logic_Source = new G4LogicalVolume(Source, Material_Cu, "Logic_Source");               // solid, material, name
  Physics_Source = new G4PVPlacement(0,                                                            // no rotation
                                     Source_Position + G4ThreeVector(0., 0., thicknessSource / 2), // position
                                     Logic_Source, "Phys_Source",                                  // its fLogical volume
                                     logic_mother_catcher,                                         // its mother volume
                                     false,                                                        // no boolean op.
                                     0);

  Logic_Source->SetVisAttributes(Vis_Cu);

  //// GARAGE ////
  G4double Garage_height = 40 * mm;
  G4double Garage_width = 32 * mm;
  G4double Garage_Thickness = 1 * mm;
  G4double Garage_Space1 = 5 * mm;
  G4double Garage_Space2 = 5 * mm;
  G4double Garage_X = -4.5 * cm;
  G4double Garage_Y = -4.5 * cm;
  G4double Garage_Z = 0 * mm;
  G4ThreeVector Garage_Position = G4ThreeVector(Garage_X, Garage_Y + 4 * mm, Garage_Z);
  G4VSolid *Garage1 = new G4Box("Garage1", Garage_width / 2, Garage_height / 2, Garage_Thickness / 2);
  G4LogicalVolume *Logic_Garage1 = new G4LogicalVolume(Garage1, Material_Al, "Logic_Garage1");                                                          // solid, material, name
  G4PVPlacement *Physics_Garage1 = new G4PVPlacement(0,                                                                                        // no rotation
                                                     Garage_Position + G4ThreeVector(Garage_width / 2, Garage_height / 2, -Garage_Space1 / 2), // position
                                                     Logic_Garage1, "Phys_Garage1",                                                            // its fLogical volume
                                                     fLogicWorld,                                                                              // its mother volume
                                                     false,                                                                                    // no boolean op.
                                                     0);
  G4VSolid *Garage2 = new G4Box("Garage2", Garage_width / 2, Garage_height / 2, Garage_Thickness / 2);
  G4LogicalVolume *Logic_Garage2 = new G4LogicalVolume(Garage2, Material_Al, "Logic_Garage2");                                                         // solid, material, name
  G4PVPlacement *Physics_Garage2 = new G4PVPlacement(0,                                                                                       // no rotation
                                                     Garage_Position + G4ThreeVector(Garage_width / 2, Garage_height / 2, Garage_Space2 / 2), // position
                                                     Logic_Garage2, "Phys_Garage2",                                                           // its fLogical volume
                                                     fLogicWorld,                                                                             // its mother volume
                                                     false,                                                                                   // no boolean op.
                                                     0);

  if (Physics_Garage1 == NULL || Physics_Garage2 == NULL)
  {
    }

    Logic_Garage2->SetVisAttributes(Vis_Al);
    Logic_Garage1->SetVisAttributes(Vis_Al);

  //=========================================================================================================================
  //========================================== SILICON DETECTORS _ COMMON PARAMETERS ========================================
  //=========================================================================================================================
  
  // Apply corrections from macro file //
  for (int i = 1; i <= 8; i++)
  {
    r = fRadius_PlasticScintillator + spazio_tra_Scintillatore_e_BordoSiDetector + pDy1 / 2 - (pDy1 / 2 * (1 - cos(theta+ Angle_Correction[i]))); // -2.7mm
    r_vide = (pDz + thicknessSiDetector + thicknessSiDetectorGrid) / 2 * -sin(theta + Angle_Correction[i]);
    z_vide = -(pDz + thicknessSiDetector + thicknessSiDetectorGrid) / 2 * cos(theta + Angle_Correction[i]);
    z = z_height_Source_biggerBaseSiDet_inVerticale + pDz / 2 + ((pDy1 / 2) * sin(theta+ Angle_Correction[i]));  // -4.525mm

    G4String DetName = "D" + to_string(i);

    // correction position from macro
    dic_correction[DetName] = Cylindrical_Convertion(DetName, ConvertStringToG4ThreeVector(string_pos[i]) * mm);

    // adding shift from the rotation of angle correction
    G4ThreeVector vec_c;
    vec_c = Cylindrical_Convertion(DetName, G4ThreeVector(0, 0, -((pDy1) * (sin(theta+ Angle_Correction[i]) - sin(theta)))));

    // reset position with correction
    G4ThreeVector vec = G4ThreeVector(0, r, z);
    dic_position[DetName] = Cylindrical_Convertion(DetName, vec) + dic_correction[DetName] + vec_c;


    // reset position vide
    G4ThreeVector vec_vide = G4ThreeVector(0, r_vide, z_vide);
    dic_positionvide[DetName] = Cylindrical_Convertion(DetName, vec_vide);

    // reset angle with correction
    CylindricalAngle_Convertion(DetName, Angle_Correction[i]);

  }
  //////////////////////////////////////

  // Il materiale che compone il supporto dei detector al silicio é il PCB. Non esiste nella libreria di G4, occorre fabbricarselo  

  // Common parameteres to all Si detectors
  G4double pTheta = 0. * degree;
  G4double pPhi = 0. * degree;
  G4double pDx1 = xHigh_Si_Bulk + 2*(spazio_tra_Bordo_e_strip5/tan(thetaInclinazione_SiDetector) + spazio_tra_Bordo_e_strip5/ cos(90*degree - thetaInclinazione_SiDetector)); //83.3 * mm;   // Lunghezza (x) della base maggiore del trapezio sulla prima faccia. Se le facce non sono svasate é uguale a pDx3
  G4double pDx2 = xLow_Si_Bulk + 2*(-spazio_tra_Bordo_e_strip5/tan(thetaInclinazione_SiDetector) + spazio_tra_Bordo_e_strip5/ cos(90*degree - thetaInclinazione_SiDetector));  //31. * mm;    // Lunghezza (x) della base minore del trapezio sulla prima faccia. Se le facce non sono svasate é uguale a pDx4
  G4double pAlp1 = 0 * degree; // Angolo tra i centri delle due facce
  G4double pDy2 = pDy1;  //34.12 * mm;  // Altezza (y) del trapezio sulla prima faccia
  G4double pDx3 = pDx1; //83.3 * mm;   // Lunghezza (x) della base maggiore del trapezio sulla prima faccia
  G4double pDx4 = pDx2; //31. * mm;    // Lunghezza (x) della base minore del trapezio sulla prima faccia
  G4double pAlp2 = 0. * degree;

  //========================================================================================================================
  //============================================= PISTE DETECTOR SILICIO  ==================================================
  //========================================================================================================================

  //___________ Supporto in rame della placca in PBC sulla quale é montato il Si detector  ____________
  G4VSolid *biggerBox_material_SupportoRame_SiliconDetector = new G4Box("biggerBox_material_SupportoRame_SiliconDetector", length_x_SupportoRame_SiDetector / 2, height_y_SupportoRame_SiDetector / 2, thickness_z_SupportoRame_SiDetector / 2);
  G4VSolid *smallerBox_material_SupportoRame_SiliconDetector = new G4Box("smallerBox_material_SupportoRame_SiliconDetector", x_smallBox_daTagliare_SupportoRame_SiDetector / 2, y_smallBox_daTagliare_SupportoRame_SiDetector / 2, thickness_z_SupportoRame_SiDetector);
  Box_material_SupportoRame_SiliconDetector = new G4SubtractionSolid("biggerBox_material_SupportoRame_SiliconDetector-smallerBox_material_SupportoRame_SiliconDetector",
                                                                     biggerBox_material_SupportoRame_SiliconDetector, smallerBox_material_SupportoRame_SiliconDetector,
                                                                     0,
                                                                     G4ThreeVector(length_x_SupportoRame_SiDetector / 2 - distanza_latoDxBoxTagliata_e_bordoDxSupportoRame_SiDetector - x_smallBox_daTagliare_SupportoRame_SiDetector / 2,
                                                                                   height_y_SupportoRame_SiDetector / 2 - y_smallBox_daTagliare_SupportoRame_SiDetector / 2,
                                                                                   0.));


  // Support
  supportSiliconDetector = new G4Trap("supportSiliconDetector",
                                      pDz / 2, pTheta, pPhi, pDy1 / 2,
                                      pDx1 / 2, pDx2 / 2, pAlp1, pDy2 / 2,
                                      pDx3 / 2, pDx4 / 2, pAlp2);

  // Si Bulk (for subtraction)
  G4VSolid *SiBulk = new G4Trap("SiBulk",
                   (thicknessSiDetector+thicknessSiDetectorGrid), pTheta, pPhi, y_Si_Bulk / 2,
                   xHigh_Si_Bulk / 2, xLow_Si_Bulk / 2, pAlp1, y_Si_Bulk / 2,
                   xHigh_Si_Bulk / 2, xLow_Si_Bulk / 2, pAlp2);

  // Mother volume
  supportSiliconDetectorvide = new G4Trap("supportSiliconDetectorvide",
                                          (thicknessSiDetector+thicknessSiDetectorGrid) / 2, pTheta, pPhi, pDy1 / 2,
                                          pDx1 / 2, pDx2 / 2, pAlp1, pDy2 / 2,
                                          pDx3 / 2, pDx4 / 2, pAlp2);

  // All Frame
  AlFrameSiliconDetector = new G4SubtractionSolid("AlFrameSiliconDetector",
                                          supportSiliconDetectorvide, SiBulk,
                                          0,
                                          G4ThreeVector(0, 0, 0));

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

  //////////////////////////// Al Grid //////////////////////////////////
  G4double xOffset_Grid = WidthSiDetectorGrid / cos(90*degree - thetaInclinazione_SiDetector);
  G4double xOffset_Interstrip_And_Grid = spazio_tra_Strip / 2 / tan(thetaInclinazione_SiDetector) + WidthSiDetectorGrid / tan(thetaInclinazione_SiDetector);

  // Strip n. 5
  G4Trap* SiDet_Strip_5_grid_in = new G4Trap("SiDet_Strip_5_grid_in",
                                thicknessSiDetectorGrid, 0. * degree, 0. * degree, y_SiDet_Strip_5 / 2 - WidthSiDetectorGrid  - spazio_tra_Strip / 2,
                                xHigh_SiDet_Strip_5 / 2 - xOffset_Interstrip_And_Grid - xOffset_Grid, xLow_SiDet_Strip_5 / 2 + xOffset_Interstrip_And_Grid - xOffset_Grid, 0. * degree, y_SiDet_Strip_5 / 2 - WidthSiDetectorGrid  - spazio_tra_Strip / 2,
                                xHigh_SiDet_Strip_5 / 2 - xOffset_Interstrip_And_Grid - xOffset_Grid, xLow_SiDet_Strip_5 / 2 + xOffset_Interstrip_And_Grid - xOffset_Grid, 0. * degree);
                          
  G4Trap* SiDet_Strip_5_grid_full= new G4Trap("SiDet_Strip_5_grid_full",
                                thicknessSiDetectorGrid / 2, 0. * degree, 0. * degree, y_SiDet_Strip_5 / 2 - spazio_tra_Strip / 2,
                                xHigh_SiDet_Strip_5 / 2 - spazio_tra_Strip / 2 / tan(thetaInclinazione_SiDetector), xLow_SiDet_Strip_5 / 2 + spazio_tra_Strip / 2 / tan(thetaInclinazione_SiDetector), 0. * degree, y_SiDet_Strip_5 / 2 - spazio_tra_Strip / 2,
                                xHigh_SiDet_Strip_5 / 2 - spazio_tra_Strip / 2 / tan(thetaInclinazione_SiDetector), xLow_SiDet_Strip_5 / 2 + spazio_tra_Strip / 2 / tan(thetaInclinazione_SiDetector), 0. * degree);

  SiDet_Strip_5_grid = new G4SubtractionSolid("SiDet_Strip_5_grid",
                                                                            SiDet_Strip_5_grid_full, SiDet_Strip_5_grid_in,
                                                                            0,
                                                                            G4ThreeVector(0., 0., 0.));

  // Strip n. 4
  G4Trap* SiDet_Strip_4_grid_in = new G4Trap("SiDet_Strip_4_grid_in",
                                thicknessSiDetectorGrid, 0. * degree, 0. * degree, y_SiDet_Strip_4 / 2 - WidthSiDetectorGrid  - spazio_tra_Strip / 2,
                                xHigh_SiDet_Strip_4 / 2 - xOffset_Interstrip_And_Grid - xOffset_Grid, xLow_SiDet_Strip_4 / 2 + xOffset_Interstrip_And_Grid - xOffset_Grid, 0. * degree, y_SiDet_Strip_4 / 2 - WidthSiDetectorGrid  - spazio_tra_Strip / 2,
                                xHigh_SiDet_Strip_4 / 2 - xOffset_Interstrip_And_Grid - xOffset_Grid, xLow_SiDet_Strip_4 / 2 + xOffset_Interstrip_And_Grid - xOffset_Grid, 0. * degree);

  G4Trap* SiDet_Strip_4_grid_full= new G4Trap("SiDet_Strip_4_grid_full",
                                thicknessSiDetectorGrid / 2, 0. * degree, 0. * degree, y_SiDet_Strip_4 / 2 - spazio_tra_Strip / 2,
                                xHigh_SiDet_Strip_4 / 2 - spazio_tra_Strip / 2 / tan(thetaInclinazione_SiDetector), xLow_SiDet_Strip_4 / 2 + spazio_tra_Strip / 2 / tan(thetaInclinazione_SiDetector), 0. * degree, y_SiDet_Strip_4 / 2 - spazio_tra_Strip / 2,
                                xHigh_SiDet_Strip_4 / 2 - spazio_tra_Strip / 2 / tan(thetaInclinazione_SiDetector), xLow_SiDet_Strip_4 / 2 + spazio_tra_Strip / 2 / tan(thetaInclinazione_SiDetector), 0. * degree);  

  SiDet_Strip_4_grid = new G4SubtractionSolid("SiDet_Strip_4_grid",
                                                                            SiDet_Strip_4_grid_full, SiDet_Strip_4_grid_in,
                                                                            0,
                                                                            G4ThreeVector(0., 0., 0.));
                                                        
  // Strip n. 3
  G4Trap* SiDet_Strip_3_grid_in = new G4Trap("SiDet_Strip_3_grid_in",
                                thicknessSiDetectorGrid, 0. * degree, 0. * degree, y_SiDet_Strip_3 / 2 - WidthSiDetectorGrid  - spazio_tra_Strip / 2,
                                xHigh_SiDet_Strip_3 / 2 - xOffset_Interstrip_And_Grid - xOffset_Grid, xLow_SiDet_Strip_3 / 2 + xOffset_Interstrip_And_Grid - xOffset_Grid, 0. * degree, y_SiDet_Strip_3 / 2 - WidthSiDetectorGrid  - spazio_tra_Strip / 2,
                                xHigh_SiDet_Strip_3 / 2 - xOffset_Interstrip_And_Grid - xOffset_Grid, xLow_SiDet_Strip_3 / 2 + xOffset_Interstrip_And_Grid - xOffset_Grid, 0. * degree);
  
  G4Trap* SiDet_Strip_3_grid_full= new G4Trap("SiDet_Strip_3_grid_full",
                                thicknessSiDetectorGrid / 2, 0. * degree, 0. * degree, y_SiDet_Strip_3 / 2 - spazio_tra_Strip / 2,
                                xHigh_SiDet_Strip_3 / 2 - spazio_tra_Strip / 2 / tan(thetaInclinazione_SiDetector), xLow_SiDet_Strip_3 / 2 + spazio_tra_Strip / 2 / tan(thetaInclinazione_SiDetector), 0. * degree, y_SiDet_Strip_3 / 2 - spazio_tra_Strip / 2,
                                xHigh_SiDet_Strip_3 / 2 - spazio_tra_Strip / 2 / tan(thetaInclinazione_SiDetector), xLow_SiDet_Strip_3 / 2 + spazio_tra_Strip / 2 / tan(thetaInclinazione_SiDetector), 0. * degree);  
  
  SiDet_Strip_3_grid = new G4SubtractionSolid("SiDet_Strip_3_grid",
                                                                            SiDet_Strip_3_grid_full, SiDet_Strip_3_grid_in,
                                                                            0,
                                                                            G4ThreeVector(0., 0., 0.));
  
  // Strip n. 2
  G4Trap* SiDet_Strip_2_grid_in = new G4Trap("SiDet_Strip_2_grid_in",
                                thicknessSiDetectorGrid, 0. * degree, 0. * degree, y_SiDet_Strip_2 / 2 - WidthSiDetectorGrid  - spazio_tra_Strip / 2,
                                xHigh_SiDet_Strip_2 / 2 - xOffset_Interstrip_And_Grid - xOffset_Grid, xLow_SiDet_Strip_2 / 2 + xOffset_Interstrip_And_Grid - xOffset_Grid, 0. * degree, y_SiDet_Strip_2 / 2 - WidthSiDetectorGrid  - spazio_tra_Strip / 2,
                                xHigh_SiDet_Strip_2 / 2 - xOffset_Interstrip_And_Grid - xOffset_Grid, xLow_SiDet_Strip_2 / 2 + xOffset_Interstrip_And_Grid - xOffset_Grid, 0. * degree);

  G4Trap* SiDet_Strip_2_grid_full= new G4Trap("SiDet_Strip_2_grid_full",
                                thicknessSiDetectorGrid / 2, 0. * degree, 0. * degree, y_SiDet_Strip_2 / 2 - spazio_tra_Strip / 2,
                                xHigh_SiDet_Strip_2 / 2 - spazio_tra_Strip / 2 / tan(thetaInclinazione_SiDetector), xLow_SiDet_Strip_2 / 2 + spazio_tra_Strip / 2 / tan(thetaInclinazione_SiDetector), 0. * degree, y_SiDet_Strip_2 / 2 - spazio_tra_Strip / 2,
                                xHigh_SiDet_Strip_2 / 2 - spazio_tra_Strip / 2 / tan(thetaInclinazione_SiDetector), xLow_SiDet_Strip_2 / 2 + spazio_tra_Strip / 2 / tan(thetaInclinazione_SiDetector), 0. * degree);

  SiDet_Strip_2_grid = new G4SubtractionSolid("SiDet_Strip_2_grid",
                                                                            SiDet_Strip_2_grid_full, SiDet_Strip_2_grid_in,
                                                                            0,
                                                                            G4ThreeVector(0., 0., 0.));

  // Strip n. 1
  G4Trap* SiDet_Strip_1_grid_in = new G4Trap("SiDet_Strip_1_grid_in",
                                thicknessSiDetectorGrid, 0. * degree, 0. * degree, y_SiDet_Strip_1 / 2 - WidthSiDetectorGrid  - spazio_tra_Strip / 2,
                                xHigh_SiDet_Strip_1 / 2 - xOffset_Interstrip_And_Grid - xOffset_Grid, xLow_SiDet_Strip_1 / 2 + xOffset_Interstrip_And_Grid - xOffset_Grid, 0. * degree, y_SiDet_Strip_1 / 2 - WidthSiDetectorGrid - spazio_tra_Strip / 2,
                                xHigh_SiDet_Strip_1 / 2 - xOffset_Interstrip_And_Grid - xOffset_Grid, xLow_SiDet_Strip_1 / 2 + xOffset_Interstrip_And_Grid - xOffset_Grid, 0. * degree);

  G4Trap* SiDet_Strip_1_grid_full= new G4Trap("SiDet_Strip_1_grid_full",
                                thicknessSiDetectorGrid / 2, 0. * degree, 0. * degree, y_SiDet_Strip_1 / 2 - spazio_tra_Strip / 2,
                                xHigh_SiDet_Strip_1 / 2 - spazio_tra_Strip / 2 / tan(thetaInclinazione_SiDetector), xLow_SiDet_Strip_1 / 2 + spazio_tra_Strip / 2 / tan(thetaInclinazione_SiDetector), 0. * degree, y_SiDet_Strip_1 / 2 - spazio_tra_Strip / 2,
                                xHigh_SiDet_Strip_1 / 2 - spazio_tra_Strip / 2 / tan(thetaInclinazione_SiDetector), xLow_SiDet_Strip_1 / 2 + spazio_tra_Strip / 2 / tan(thetaInclinazione_SiDetector), 0. * degree);
  
  SiDet_Strip_1_grid = new G4SubtractionSolid("SiDet_Strip_1_grid",
                                                                            SiDet_Strip_1_grid_full, SiDet_Strip_1_grid_in,
                                                                            0,
                                                                            G4ThreeVector(0., 0., 0.));

  //////////////////////////// Interstrips insultor ////////////////////////////////// 

  // InterStrip n. 5-
  G4Trap* SiDet_InterStrip_5 = new G4Trap("SiDet_InterStrip_5-",
                             (thicknessSiDetectorGrid) / 2, 0. * degree, 0. * degree, spazio_tra_Strip / 4,
                             xHigh_SiDet_Strip_5 / 2, xHigh_SiDet_Strip_5 / 2 - spazio_tra_Strip / 2 / tan(thetaInclinazione_SiDetector), 0. * degree, spazio_tra_Strip / 4,
                             xHigh_SiDet_Strip_5 / 2, xHigh_SiDet_Strip_5 / 2 - spazio_tra_Strip / 2 / tan(thetaInclinazione_SiDetector), 0. * degree);

  // InterStrip n. 4-5
  G4Trap* SiDet_InterStrip_45 = new G4Trap("SiDet_InterStrip_45",
                             (thicknessSiDetectorGrid) / 2, 0. * degree, 0. * degree, spazio_tra_Strip / 2,
                             xLow_SiDet_Strip_5 / 2 + spazio_tra_Strip / 2 / tan(thetaInclinazione_SiDetector), xHigh_SiDet_Strip_4 / 2 - spazio_tra_Strip / 2 / tan(thetaInclinazione_SiDetector), 0. * degree, spazio_tra_Strip / 2,
                             xLow_SiDet_Strip_5 / 2 + spazio_tra_Strip / 2 / tan(thetaInclinazione_SiDetector), xHigh_SiDet_Strip_4 / 2 - spazio_tra_Strip / 2 / tan(thetaInclinazione_SiDetector), 0. * degree);

  // InterStrip n. 3-4
  G4Trap* SiDet_InterStrip_34 = new G4Trap("SiDet_InterStrip_34",
                             (thicknessSiDetectorGrid) / 2, 0. * degree, 0. * degree, spazio_tra_Strip / 2,
                             xLow_SiDet_Strip_4 / 2 + spazio_tra_Strip / 2 / tan(thetaInclinazione_SiDetector), xHigh_SiDet_Strip_3 / 2 - spazio_tra_Strip / 2 / tan(thetaInclinazione_SiDetector), 0. * degree, spazio_tra_Strip / 2,
                             xLow_SiDet_Strip_4 / 2 + spazio_tra_Strip / 2 / tan(thetaInclinazione_SiDetector), xHigh_SiDet_Strip_3 / 2 - spazio_tra_Strip / 2 / tan(thetaInclinazione_SiDetector), 0. * degree);

  // InterStrip n. 2-3
  G4Trap* SiDet_InterStrip_23 = new G4Trap("SiDet_InterStrip_23",
                             (thicknessSiDetectorGrid) / 2, 0. * degree, 0. * degree, spazio_tra_Strip / 2,
                             xLow_SiDet_Strip_3 / 2 + spazio_tra_Strip / 2 / tan(thetaInclinazione_SiDetector), xHigh_SiDet_Strip_2 / 2 - spazio_tra_Strip / 2 / tan(thetaInclinazione_SiDetector), 0. * degree, spazio_tra_Strip / 2,
                             xLow_SiDet_Strip_3 / 2 + spazio_tra_Strip / 2 / tan(thetaInclinazione_SiDetector), xHigh_SiDet_Strip_2 / 2 - spazio_tra_Strip / 2 / tan(thetaInclinazione_SiDetector), 0. * degree);

  // InterStrip n. 1-2
  G4Trap* SiDet_InterStrip_12 = new G4Trap("SiDet_InterStrip_12",
                             (thicknessSiDetectorGrid) / 2, 0. * degree, 0. * degree, spazio_tra_Strip / 2,
                             xLow_SiDet_Strip_2 / 2 + spazio_tra_Strip / 2 / tan(thetaInclinazione_SiDetector), xHigh_SiDet_Strip_1 / 2 - spazio_tra_Strip / 2 / tan(thetaInclinazione_SiDetector), 0. * degree, spazio_tra_Strip / 2,
                             xLow_SiDet_Strip_2 / 2 + spazio_tra_Strip / 2 / tan(thetaInclinazione_SiDetector), xHigh_SiDet_Strip_1 / 2 - spazio_tra_Strip / 2 / tan(thetaInclinazione_SiDetector), 0. * degree);

  // // InterStrip n. -1
  G4Trap* SiDet_InterStrip_1 = new G4Trap("SiDet_InterStrip_-1",
                             (thicknessSiDetectorGrid) / 2, 0. * degree, 0. * degree, spazio_tra_Strip / 4,
                             xLow_SiDet_Strip_1 / 2 + spazio_tra_Strip / 2 / tan(thetaInclinazione_SiDetector), xLow_SiDet_Strip_1 / 2, 0. * degree, spazio_tra_Strip / 4,
                             xLow_SiDet_Strip_1 / 2 + spazio_tra_Strip / 2 / tan(thetaInclinazione_SiDetector), xLow_SiDet_Strip_1 / 2, 0. * degree);


  //////////////////////////// Strip ////////////////////////////////// 
  // // Strip n. 5 //
  // G4Trap* SiDet_Strip_5_U = new G4Trap("SiDet_InterStrip_5_4side",
  //                            (thicknessSiDetector) / 2, 0. * degree, 0. * degree, y_SiDet_Strip_5 / 2 + spazio_tra_Strip / 2,
  //                            xHigh_SiDet_Strip_5 / 2, ( xLow_SiDet_Strip_5 + (xLow_SiDet_Strip_5-xHigh_SiDet_Strip_4) / 2 ) / 2, 0. * degree, y_SiDet_Strip_5 / 2 + spazio_tra_Strip / 2,
  //                            xHigh_SiDet_Strip_5 / 2, ( xLow_SiDet_Strip_5 + (xLow_SiDet_Strip_5-xHigh_SiDet_Strip_4) / 2 ) / 2, 0. * degree);
  
  // // Strip n. 4 //
  // G4Trap* SiDet_Strip_4_U = new G4Trap("SiDet_InterStrip_4_5side",
  //                            (thicknessSiDetector) / 2, 0. * degree, 0. * degree, y_SiDet_Strip_4 / 2 + spazio_tra_Strip / 2,
  //                            ( xHigh_SiDet_Strip_4 + (xLow_SiDet_Strip_5-xHigh_SiDet_Strip_4) / 2 ) / 2, ( xHigh_SiDet_Strip_3 + (xLow_SiDet_Strip_4-xHigh_SiDet_Strip_3) / 2 ) / 2, 0. * degree, y_SiDet_Strip_4 / 2 + spazio_tra_Strip / 2,
  //                            ( xHigh_SiDet_Strip_4 + (xLow_SiDet_Strip_5-xHigh_SiDet_Strip_4) / 2 ) / 2, ( xHigh_SiDet_Strip_3 + (xLow_SiDet_Strip_4-xHigh_SiDet_Strip_3) / 2 ) / 2, 0. * degree);

  // // Strip n. 3 //
  // G4Trap* SiDet_Strip_3_U = new G4Trap("SiDet_InterStrip_3_4side",
  //                            (thicknessSiDetector) / 2, 0. * degree, 0. * degree, y_SiDet_Strip_3 / 2 + spazio_tra_Strip / 2,
  //                            ( xHigh_SiDet_Strip_3 + (xLow_SiDet_Strip_4-xHigh_SiDet_Strip_3) / 2 ) / 2, ( xHigh_SiDet_Strip_2 + (xLow_SiDet_Strip_3-xHigh_SiDet_Strip_2) / 2 ) / 2, 0. * degree, y_SiDet_Strip_3 / 2 + spazio_tra_Strip / 2,
  //                            ( xHigh_SiDet_Strip_3 + (xLow_SiDet_Strip_4-xHigh_SiDet_Strip_3) / 2 ) / 2, ( xHigh_SiDet_Strip_2 + (xLow_SiDet_Strip_3-xHigh_SiDet_Strip_2) / 2 ) / 2, 0. * degree);
  
  // // Strip n. 2 //
  // G4Trap* SiDet_Strip_2_U = new G4Trap("SiDet_InterStrip_2_3side",
  //                            (thicknessSiDetector) / 2, 0. * degree, 0. * degree, y_SiDet_Strip_2 / 2 + spazio_tra_Strip / 2,
  //                            ( xHigh_SiDet_Strip_2 + (xLow_SiDet_Strip_3-xHigh_SiDet_Strip_2) / 2 ) / 2, ( xHigh_SiDet_Strip_1 + (xLow_SiDet_Strip_2-xHigh_SiDet_Strip_1) / 2 ) / 2, 0. * degree, y_SiDet_Strip_2 / 2 + spazio_tra_Strip / 2,
  //                            ( xHigh_SiDet_Strip_2 + (xLow_SiDet_Strip_3-xHigh_SiDet_Strip_2) / 2 ) / 2, ( xHigh_SiDet_Strip_1 + (xLow_SiDet_Strip_2-xHigh_SiDet_Strip_1) / 2 ) / 2, 0. * degree);
  
  // // Strip n. 1 //
  // G4Trap* SiDet_Strip_1_U = new G4Trap("SiDet_InterStrip_1_2side",
  //                            (thicknessSiDetector) / 2, 0. * degree, 0. * degree, y_SiDet_Strip_1 / 2 + spazio_tra_Strip / 2,
  //                            ( xHigh_SiDet_Strip_1 + (xLow_SiDet_Strip_2-xHigh_SiDet_Strip_1) / 2 ) / 2, xLow_SiDet_Strip_1 / 2, 0. * degree, y_SiDet_Strip_1 / 2 +  spazio_tra_Strip / 2,
  //                            ( xHigh_SiDet_Strip_1 + (xLow_SiDet_Strip_2-xHigh_SiDet_Strip_1) / 2 ) / 2, xLow_SiDet_Strip_1 / 2, 0. * degree);
                
  dic_strip[1] = std::make_tuple(SiDet_Strip_1, G4ThreeVector(0, -pDy1 / 2 + spazio_tra_Bordo_e_strip5 + y_SiDet_Strip_5 + y_SiDet_Strip_4 + y_SiDet_Strip_3 + y_SiDet_Strip_2 + y_SiDet_Strip_1 / 2, -thicknessSiDetectorGrid/2), SiDet_Strip_1_dl, SiDet_Strip_1_grid);
  dic_strip[2] = std::make_tuple(SiDet_Strip_2, G4ThreeVector(0, -pDy1 / 2 + spazio_tra_Bordo_e_strip5 + y_SiDet_Strip_5 + y_SiDet_Strip_4 + y_SiDet_Strip_3 + y_SiDet_Strip_2 / 2, -thicknessSiDetectorGrid/2), SiDet_Strip_2_dl, SiDet_Strip_2_grid);
  dic_strip[3] = std::make_tuple(SiDet_Strip_3, G4ThreeVector(0, -pDy1 / 2 + spazio_tra_Bordo_e_strip5 + y_SiDet_Strip_5 + y_SiDet_Strip_4 + y_SiDet_Strip_3 / 2, -thicknessSiDetectorGrid/2), SiDet_Strip_3_dl, SiDet_Strip_3_grid);
  dic_strip[4] = std::make_tuple(SiDet_Strip_4, G4ThreeVector(0, -pDy1 / 2 + spazio_tra_Bordo_e_strip5 + y_SiDet_Strip_5 + y_SiDet_Strip_4 / 2, -thicknessSiDetectorGrid/2), SiDet_Strip_4_dl, SiDet_Strip_4_grid);
  dic_strip[5] = std::make_tuple(SiDet_Strip_5, G4ThreeVector(0, -pDy1 / 2 + spazio_tra_Bordo_e_strip5 + y_SiDet_Strip_5 / 2, -thicknessSiDetectorGrid/2), SiDet_Strip_5_dl, SiDet_Strip_5_grid);

  dic_interstrip[-10] = std::make_tuple(SiDet_InterStrip_1, G4ThreeVector(0, -pDy1 / 2 + spazio_tra_Bordo_e_strip5 + y_SiDet_Strip_5 + y_SiDet_Strip_4 + y_SiDet_Strip_3 + y_SiDet_Strip_2 + y_SiDet_Strip_1 - spazio_tra_Strip/4, 0));
  dic_interstrip[12] = std::make_tuple(SiDet_InterStrip_12, G4ThreeVector(0, -pDy1 / 2 + spazio_tra_Bordo_e_strip5 + y_SiDet_Strip_5 + y_SiDet_Strip_4 + y_SiDet_Strip_3 + y_SiDet_Strip_2, 0));
  dic_interstrip[23] = std::make_tuple(SiDet_InterStrip_23, G4ThreeVector(0, -pDy1 / 2 + spazio_tra_Bordo_e_strip5 + y_SiDet_Strip_5 + y_SiDet_Strip_4 + y_SiDet_Strip_3, 0));
  dic_interstrip[34] = std::make_tuple(SiDet_InterStrip_34, G4ThreeVector(0, -pDy1 / 2 + spazio_tra_Bordo_e_strip5 + y_SiDet_Strip_5 + y_SiDet_Strip_4, 0));
  dic_interstrip[45] = std::make_tuple(SiDet_InterStrip_45, G4ThreeVector(0, -pDy1 / 2 + spazio_tra_Bordo_e_strip5 + y_SiDet_Strip_5, 0));
  dic_interstrip[-43] = std::make_tuple(SiDet_InterStrip_5, G4ThreeVector(0, -pDy1 / 2 + spazio_tra_Bordo_e_strip5 + spazio_tra_Strip / 4, 0));

  // Loop to construct Silicon Detetors
  for (int i = 1; i <= 8; i++)
  {
      tab[i-1] = Make_Sidet(i);
  }

  //===================================================================================================================================================================
  //===================================================================================================================================================================
  //==================================================   WISArD PLASTIC SCINTILLATOR - WITH SUPPORT ===================================================================
  //===================================================================================================================================================================
  //===================================================================================================================================================================
  // NB: per ogni dubbio sulle misure, consultare i disegni tecnici inviati da M. Roche (mail al mio indirizzo cenbg il 18/05/2021).
  G4double distanza_tra_BaseInfScintillatore_e_BordoSuperioreDeiSiDetector = 25.575 * mm;
  G4double delta = 0 * cm;

  // TODO Add Birks Constant
  Material_Plastic->GetIonisation()->SetBirksConstant(0.126 * mm / MeV); 

  G4Tubs *fSolid_PlasticScintillator = new G4Tubs("PlasticScintillator", 0., fRadius_PlasticScintillator, 0.5 * fLength_PlasticScintillator, 0., 360 * deg);                                                                                   // name, r : 0->1cm, L : 5cm, phi : 0->2pi
  fLogic_PlasticScintillator = new G4LogicalVolume(fSolid_PlasticScintillator, Material_Plastic, "PlasticScintillator");                                                                                         // solid, material, name
  G4PVPlacement *fPhys_PlasticScintillator = new G4PVPlacement(0,                                                                                                                                                                              // rotationMatrix
                                                               G4ThreeVector(0., 0., z_height_Source_biggerBaseSiDet_inVerticale + distanza_tra_BaseInfScintillatore_e_BordoSuperioreDeiSiDetector + fLength_PlasticScintillator / 2 + delta), // position
                                                               fLogic_PlasticScintillator, "PlasticScintillator",                                                                                                                              // its fLogical volume
                                                               fLogicWorld,                                                                                                                                                                    // its mother volume
                                                               false,                                                                                                                                                                          // no boolean op.
                                                               99);                                                                                                                                                                             // copy nb.
  fLogic_PlasticScintillator->SetVisAttributes(Vis_Plastic);

  if (fPhys_PlasticScintillator == NULL)
  {
  }

  G4double length_x_SupportoRame_PlasticScint = 20.5 * mm - 2 * mm;
  G4double height_y_SupportoRame_PlasticScint = 20.5 * mm - 2 * mm;
  G4double thickness_z_SupportoRame_PlasticScint = 1.5 * mm;

  G4VSolid *bigBox_material_SupportoRame_PlasticScint = new G4Box("bigBox_material_SupportoRame_PlasticScint", length_x_SupportoRame_PlasticScint, height_y_SupportoRame_PlasticScint, thickness_z_SupportoRame_PlasticScint / 2); // corretto non div per due x e y
  G4VSolid *holeForPlasticScint = new G4Tubs("holeForPlasticScint", 0., fRadius_PlasticScintillator, thickness_z_SupportoRame_PlasticScint, 0., 360 * deg);

  G4VSolid *Box_material_SupportoRame_PlasticScint = new G4SubtractionSolid("Box_material_SupportoRame_PlasticScint",
                                                                            bigBox_material_SupportoRame_PlasticScint, holeForPlasticScint,
                                                                            0,
                                                                            G4ThreeVector(0., 0., 0.));

  G4LogicalVolume *logic_Box_material_SupportoRame_PlasticScint = new G4LogicalVolume(Box_material_SupportoRame_PlasticScint, Material_Cu, "logic_Box_material_SupportoRame_PlasticScint"); // solid, material, name
  G4VPhysicalVolume *physics_Box_material_SupportoRame_PlasticScint = new G4PVPlacement(0,                                                                                         // no rotation
                                                                                        G4ThreeVector(0., 0., delta + z_height_Source_biggerBaseSiDet_inVerticale + distanza_tra_BaseInfScintillatore_e_BordoSuperioreDeiSiDetector + thickness_z_SupportoRame_SiDetector / 2),
                                                                                        logic_Box_material_SupportoRame_PlasticScint, "logic_Box_material_SupportoRame_PlasticScint", // its fLogical volume
                                                                                        fLogicWorld,                                                                                  // its mother volume
                                                                                        false,                                                                                        // no boolean op.
                                                                                        0);                                                                                           // copy nb.

  logic_Box_material_SupportoRame_PlasticScint->SetVisAttributes(Vis_Cu);
  

  if (physics_Box_material_SupportoRame_PlasticScint == NULL)
  {
  }

  G4double lenghtPlaccaCilidrica = fLength_PlasticScintillator - thickness_z_SupportoRame_SiDetector;

  G4VSolid *placcaRame_Cilindrica = new G4Tubs("placcaRame_Cilindrica", fRadius_PlasticScintillator, fRadius_PlasticScintillator + thickness_z_SupportoRame_PlasticScint, lenghtPlaccaCilidrica / 2, 0., 360 * deg);
  G4LogicalVolume *logic_placcaRame_Cilindrica = new G4LogicalVolume(placcaRame_Cilindrica, Material_Cu, "placcaRame_Cilindrica"); // solid, material, name
  G4VPhysicalVolume *physics_placcaRame_Cilindrica = new G4PVPlacement(0,                                                 // no rotation
                                                                       G4ThreeVector(0., 0., delta + z_height_Source_biggerBaseSiDet_inVerticale + distanza_tra_BaseInfScintillatore_e_BordoSuperioreDeiSiDetector + thickness_z_SupportoRame_SiDetector + lenghtPlaccaCilidrica / 2),
                                                                       logic_placcaRame_Cilindrica, "logic_placcaRame_Cilindrica", // its fLogical volume
                                                                       fLogicWorld,                                                // its mother volume
                                                                       false,                                                      // no boolean op.
                                                                       0);

  logic_placcaRame_Cilindrica->SetVisAttributes(Vis_Cu);
  if (physics_placcaRame_Cilindrica == NULL)
  {
  }

  /////// SET SENSITIVE DETECOR FOR KILLER //////
  if (!Magnetic_Field_Mapping_flag)
  {
    G4Tubs *fSolid_Killer = new G4Tubs("KillerSolid", 0., fRadius_PlasticScintillator, 0.1 * mm, 0., 360 * deg);
    fLogic_Killer = new G4LogicalVolume(fSolid_Killer, Material_Vacuum, "Killer");              // solid, material, name
    G4PVPlacement *fPhys_Killer = new G4PVPlacement(0,                               // rotationMatrix
                                                    G4ThreeVector(0., 0., -85 * mm), // position
                                                    fLogic_Killer, "Killer",         // its fLogical volume
                                                    fLogicWorld,                     // its mother volume
                                                    false,                           // no boolean op.
                                                    -1);                             // copy nb.

    if (fPhys_Killer == NULL)
    {
    }
    G4VisAttributes *Killer_att = new G4VisAttributes(G4Colour(0.6, 0.6, 0.6, 0.6)); // red
    Killer_att->SetForceWireframe(false);
    Killer_att->SetForceSolid(true);
    Killer_att->SetVisibility(true);
    fLogic_Killer->SetVisAttributes(Killer_att);
  }

    /////// SET TUBE ENTRANCE /////////
  G4double delta_entrance = -62 * mm;
  G4double Tube_length = 27.5*mm;
  G4Tubs *fSolid_TubeEntrance = new G4Tubs("TubeEntranceSolid", fRadius_PlasticScintillator, fRadius_PlasticScintillator+6*mm, Tube_length/2, 0., 360 * deg);
  G4LogicalVolume *fLogic_TubeEntrance = new G4LogicalVolume(fSolid_TubeEntrance, Material_Cu, "TubeEntrance");                                                                                         // solid, material, name
  G4PVPlacement *fPhys_TubeEntrance = new G4PVPlacement(0,                                                                                                                                                                              // rotationMatrix
                                                               G4ThreeVector(0., 0., delta_entrance),
                                                               fLogic_TubeEntrance, "TubeEntrance",                                                                                                                              // its fLogical volume
                                                               fLogicWorld,                                                                                                                                                                    // its mother volume
                                                               false,                                                                                                                                                                          // no boolean op.
                                                               -1);

  if (fPhys_TubeEntrance == NULL)
  {
  }
  fLogic_TubeEntrance->SetVisAttributes(Vis_Cu);

  /////// SET PLAT ENTRANCE /////////
  G4double Plate_tickness = 2.5*mm;
  G4Tubs *fSolid_PlateEntrance = new G4Tubs("PlateEntranceSolid", fRadius_PlasticScintillator, 60*mm, Plate_tickness/2, 0., 360 * deg);
  G4LogicalVolume *fLogic_PlateEntrance = new G4LogicalVolume(fSolid_PlateEntrance, Material_Cu, "PlateEntrance");                                                                                         // solid, material, name
  G4PVPlacement *fPhys_PlateEntrance = new G4PVPlacement(0,                                                                                                                                                                              // rotationMatrix
                                                               G4ThreeVector(0., 0., delta_entrance-Tube_length/2-Plate_tickness/2),
                                                               fLogic_PlateEntrance, "PlateEntrance",                                                                                                                              // its fLogical volume
                                                               fLogicWorld,                                                                                                                                                                    // its mother volume
                                                               false,                                                                                                                                                                          // no boolean op.
                                                               -1); 

  if (fPhys_PlateEntrance == NULL)
  {
  }

  fLogic_PlateEntrance->SetVisAttributes(Vis_Al);                            


  /////// SET COLLIMATOR ENTRANCE //////
  if (Collimator_flag)
  {
    G4double Collimator_thickness = 2*mm;
    G4Tubs *fSolid_CollimatorEntrance = new G4Tubs("CollimatorEntranceSolid", 2.5*mm, 2*fRadius_PlasticScintillator, Collimator_thickness/2, 0., 360 * deg);
    G4LogicalVolume *fLogic_CollimatorEntrance = new G4LogicalVolume(fSolid_CollimatorEntrance, Material_Al, "CollimatorEntrance");                                                                                         // solid, material, name
    G4PVPlacement *fPhys_CollimatorEntrance = new G4PVPlacement(0,                                                                                                                                                                              // rotationMatrix
                                                                G4ThreeVector(0., 0., delta_entrance-Tube_length/2-Plate_tickness/2-Collimator_thickness),
                                                                fLogic_CollimatorEntrance, "CollimatorEntrance",                                                                                                                              // its fLogical volume
                                                                fLogicWorld,                                                                                                                                                                    // its mother volume
                                                                false,                                                                                                                                                                          // no boolean op.
                                                                -1);   

    if (fPhys_CollimatorEntrance == NULL)
    {
    }

    fLogic_CollimatorEntrance->SetVisAttributes(Vis_Al);  
  }                                                                                                                                                                        // copy nb.

  if (Magnetic_Field_Mapping_flag)
  {
    //==================================================================================================
    //========================================  WISArD BEAMLINE =========================================
    //==================================================================================================
    G4double radius_beamline = 10 * cm / 2;
    G4double length_beamline = 22 * cm;

    G4Tubs *wisard_beamline = new G4Tubs("Wisard_beamline",
                                         radius_beamline,
                                         radius_beamline + 1 * mm,
                                         0.5L * (length_beamline),
                                         0.L, 360.L * deg);
    G4LogicalVolume *wisard_beamline_logic = new G4LogicalVolume(wisard_beamline, // forme
                                                                 Material_Fe,    // matiere
                                                                 "WisardLogic",   // nom
                                                                 0, 0, 0);
    // - physical volume
    G4VPhysicalVolume *wisard_beamline_phys = new G4PVPlacement(0,
                                                                G4ThreeVector(0., 0., -40 * cm - length_beamline / 2),
                                                                wisard_beamline_logic, // ptr sur vol. log.
                                                                "Wisard",              // nom du volume
                                                                fLogicWorld,           // volume parent
                                                                false, false);

    // - make the bore volume "visible"
    wisard_beamline_logic->SetVisAttributes(Vis_Fe);

    if (wisard_beamline_phys == NULL)
    {
    }

    //==================================================================================================
    //=============================  WISArD BEAMLINE COLLIMATOR ========================================
    //==================================================================================================
    G4double radius_beamline_collimator = 10 * mm;
    G4double length_beamline_collimator = 5 * cm;

    G4Tubs *wisard_beamline_collimator = new G4Tubs("Wisard_beamline",
                                                    radius_beamline_collimator,
                                                    radius_beamline,
                                                    0.5L * (length_beamline_collimator),
                                                    0.L, 360.L * deg);
    G4LogicalVolume *wisard_beamline_collimator_logic = new G4LogicalVolume(wisard_beamline_collimator, // forme
                                                                            Material_Fe,               // matiere
                                                                            "WisardLogic",              // nom
                                                                            0, 0, 0);
    // - physical volume
    G4VPhysicalVolume *wisard_beamline_collimator_phys = new G4PVPlacement(0,
                                                                           G4ThreeVector(0., 0., -40 * cm - length_beamline - length_beamline_collimator / 2),
                                                                           wisard_beamline_collimator_logic, // ptr sur vol. log.
                                                                           "Wisard",                         // nom du
                                                                           fLogicWorld,                      // volume parent
                                                                           false, false);

    // - make the bore volume "visible"
    wisard_beamline_collimator_logic->SetVisAttributes(Vis_Al);
    if (wisard_beamline_collimator_phys == NULL)
    {
    }
  }

  return fPhysiWorld;
}
