#ifndef WISARD_DETECTOR_HH
#define WISARD_DETECTOR_HH

#include <unordered_map>
#include <tuple>
#include <string>
#include <iostream>

#include "G4VUserDetectorConstruction.hh"
#include "G4ThreeVector.hh"
#include "G4UnionSolid.hh"
#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"
#include "G4SubtractionSolid.hh"
#include "G4ExtrudedSolid.hh"
#include "G4MultiUnion.hh"
#include "G4VisAttributes.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4TransportationManager.hh"
#include "G4ChordFinder.hh"
#include "G4FieldManager.hh"
#include "G4Trap.hh"
#include "G4VSolid.hh"
#include "G4UniformMagField.hh"
#include "G4SDManager.hh"

#include "Wisard_Global.hh"
#include "G4GenericMessenger.hh"

//----------------------------------------------------------------------

class Wisard_Detector : public G4VUserDetectorConstruction
{
public:
  Wisard_Detector();
  ~Wisard_Detector();

  G4VPhysicalVolume *Construct();
  void ConstructSDandField();

 G4double pos_detp1, pos_detp2, pos_dete;
 G4double radius1_detp, radius2_detp, radius1_dete, radius2_dete;
 G4double thickness_detp, thickness_dete;
 G4double pos_catcher, thickness_catcher, radius_catcher;

 G4GenericMessenger *GeometryMessenger;           

  G4double z_height_Source_biggerBaseSiDet_inVerticale;
  G4double thicknessSiDetectorDeadLayer = 100*nm;
  G4double spazio_tra_Scintillatore_e_BordoSiDetector;
  G4double Magnetic_Field = 4*tesla;
  G4bool Magnetic_Field_Mapping_flag = false;
  G4bool Collimator_flag = true;
  G4double Catcher_Position_z = 0.*mm;
  G4String Catcher_Position = "catcher1";
  G4double Catcher_Angle = 0*deg;
  G4double Catcher_Thickness_Al1 = 50 * nm;
  G4double Catcher_Thickness_Mylar = 500 *nm;
  G4double Catcher_Thickness_Al2 = 50*nm;
  ifstream inputB;
  G4String input_nameB;
  G4Tubs *MylarSource;
  G4Tubs *AlSource1;
  G4Tubs *AlSource2;
  G4Tubs *MylarSource_central;
  G4Tubs *MylarSource_side;
  G4Tubs *Source;

  G4Tubs *fSolidWorld;
  G4LogicalVolume *fLogicWorld;
  G4VPhysicalVolume *fPhysiWorld;
  G4Material *fDefaultMaterial;

  G4double fLength_PlasticScintillator = 5 * cm;
  G4double fRadius_PlasticScintillator = 1.5 * cm;

  G4bool event;
  

  G4UserLimits *myStepLimit ;

  std::vector<std::pair<G4LogicalVolume *, G4VPhysicalVolume *>>
  Make_Sidet(int num);
  std::pair<G4LogicalVolume *, G4VPhysicalVolume *> MakeSupport(int num, G4VisAttributes *support_att, G4Material *support_mat);
  std::pair<G4LogicalVolume *, G4VPhysicalVolume *> MakeCooling(int num, G4LogicalVolume *mother, G4VisAttributes *cooling_att, G4Material *cooling_mat);

  std::pair<G4LogicalVolume *, G4VPhysicalVolume *> MakeVide(int num, G4VisAttributes *vide_att, G4Material *vide_mat);
  std::pair<G4LogicalVolume *, G4VPhysicalVolume *> MakeStrip(int strip, int num, G4LogicalVolume *mother, G4VisAttributes *strip_att, G4Material *strip_mat);
  std::pair<G4LogicalVolume *, G4VPhysicalVolume *> MakeDL(int strip, int num, G4LogicalVolume *mother, G4VisAttributes *strip_att, G4Material *strip_mat);
  std::pair<G4LogicalVolume *, G4VPhysicalVolume *> MakeStripGrid(int strip, int num, G4LogicalVolume *videe, G4VisAttributes *grid_att, G4Material *grid_mat);
  std::pair<G4LogicalVolume *, G4VPhysicalVolume *> MakeAlFrame(int num, G4LogicalVolume *mother, G4Material *frame_mat);
  std::pair<G4LogicalVolume *, G4VPhysicalVolume *> MakeInterStrip(int strip, int num, G4LogicalVolume *videe, G4VisAttributes *strip_att, G4Material*);
  G4ThreeVector ConvertStringToG4ThreeVector(G4String str);
  G4ThreeVector Cylindrical_Convertion(G4String, G4ThreeVector);
  void CylindricalAngle_Convertion(G4String, G4double);


  std::unordered_map<int, std::tuple<G4Trap* , G4ThreeVector, G4Trap *, G4VSolid*>> dic_strip;
  std::unordered_map<int, std::tuple<G4Trap *, G4ThreeVector>> dic_interstrip;
  std::unordered_map<std::string, G4ThreeVector> dic_position;
  std::unordered_map<std::string, G4ThreeVector> dic_positionvide;
  std::unordered_map<std::string, G4ThreeVector> dic_correction;
  std::unordered_map<std::string, std::tuple<G4double,G4double,G4double>> dic_rotate;

  std::vector<std::pair<G4LogicalVolume *, G4VPhysicalVolume *>>
      tab[8];

  G4VisAttributes *Box_material_SupportoRame_SiliconDetector_attvide;
  G4VisAttributes *Box_material_SupportoRame_SiliconDetector_att;
  G4VisAttributes *SupportSiliconDetector_att;
  G4VisAttributes *SiliconDetector_att;
  
  
  G4double thicknessMylarSource;

  G4double thicknessMylarSource_central;
  G4double thicknessMylarSource_side;

  G4double thicknessAlSource;
  G4VPhysicalVolume *Physics_AlSource1;
  G4VPhysicalVolume *Physics_AlSource1_central;
  G4VPhysicalVolume *Physics_AlSource1_side;

  G4VPhysicalVolume *Physics_MylarSource;
  G4VPhysicalVolume *Physics_MylarSource_central;
  G4VPhysicalVolume *Physics_MylarSource_side;

  G4VPhysicalVolume *Physics_AlSource2;
  G4VPhysicalVolume *Physics_AlSource2_central;
  G4VPhysicalVolume *Physics_AlSource2_side;

  G4VPhysicalVolume *Physics_Source;


  G4VPhysicalVolume *phys_centralPEEK_ring;
  G4VPhysicalVolume *phys_sidePEEK_ring;

  G4ThreeVector Support_Position;
  G4ThreeVector Central_Hole_Position;
  G4ThreeVector Side_Hole_Position;
  G4ThreeVector Catcher_side_Position;
  G4ThreeVector Catcher_central_Position;
  G4ThreeVector Source_Position;
  G4double PEEK_thikness;
  G4double SuppCatcher_thikness = 2*mm;

  G4VPhysicalVolume *phys_Supp_catcher;
  G4VPhysicalVolume *phys_mother_catcher;
  G4double y_baseCatcher_Spessa_1;
  G4double y_baseCatcher_Sottile_1;
  G4double y_baseCatcher_2;
  G4double y_pos_Catcher_risp_munion;
  G4double thickness_baseCatcher_Spessa_1;
  G4double thickness_baseCatcher_Sottile_1;
  G4double thickness_supp_catcher;
  G4VPhysicalVolume *phys_munion_supportCatcher_Source;
  G4double x_pos_bucoCatcher;

  G4VSolid *supportSiliconDetectorvide;
  G4Trap *SiDet_Strip_1;
  G4Trap *SiDet_Strip_2;
  G4Trap *SiDet_Strip_3;
  G4Trap *SiDet_Strip_4;
  G4Trap *SiDet_Strip_5;
  G4Trap *SiDet_Strip_1_dl;
  G4Trap *SiDet_Strip_2_dl;
  G4Trap *SiDet_Strip_3_dl;
  G4Trap *SiDet_Strip_4_dl;
  G4Trap *SiDet_Strip_5_dl;
  G4VSolid *SiDet_Strip_1_grid;
  G4VSolid *SiDet_Strip_2_grid;
  G4VSolid *SiDet_Strip_3_grid;
  G4VSolid *SiDet_Strip_4_grid;
  G4VSolid *SiDet_Strip_5_grid;
 G4double r, z, r_vide, z_vide, theta;
  G4VSolid *Box_material_SupportoRame_SiliconDetector;
  G4double spazio_tra_Bordo_e_strip5;
  G4double spazio_tra_Strip;
  G4double thicknessSiDetector, length_x_SupportoRame_SiDetector, x_smallBox_daTagliare_SupportoRame_SiDetector, distanza_latoDxBoxTagliata_e_bordoDxSupportoRame_SiDetector, y_smallBox_daTagliare_SupportoRame_SiDetector, thetaInclinazione_SiDetector, pDy1, height_y_SupportoRame_SiDetector, pDz, thickness_z_SupportoRame_SiDetector;
  G4VSolid *supportSiliconDetector;
  G4double thicknessSiDetectorGrid;
  G4VSolid *AlFrameSiliconDetector;
  G4double WidthSiDetectorGrid;
  G4double xHigh_SiDet_Strip_5;
  G4double xLow_SiDet_Strip_5;
  G4double y_SiDet_Strip_5;
  G4double xLow_SiDet_Strip_4;
  G4double y_SiDet_Strip_4;
  G4double xHigh_SiDet_Strip_4;
  G4double xLow_SiDet_Strip_3;
  G4double y_SiDet_Strip_3;
  G4double xHigh_SiDet_Strip_3;
  G4double xLow_SiDet_Strip_2;
  G4double y_SiDet_Strip_2;
  G4double xHigh_SiDet_Strip_2;
  G4double xLow_SiDet_Strip_1;
  G4double y_SiDet_Strip_1;
  G4double xHigh_SiDet_Strip_1;
  G4double y_Si_Bulk; 
  G4double xHigh_Si_Bulk;
  G4double xLow_Si_Bulk;

  G4LogicalVolume *fLogic_PlasticScintillator;
  G4LogicalVolume *fLogic_Killer;
  G4LogicalVolume *fLogic_AlSource1_central;
  G4LogicalVolume *fLogic_MylarSource_central;
  G4LogicalVolume *fLogic_AlSource2_central;
  G4LogicalVolume *fLogic_AlSource1_side;
  G4LogicalVolume *fLogic_MylarSource_side;
  G4LogicalVolume *fLogic_AlSource2_side;

  G4Material *Material_Vacuum;
  G4Material *Material_Si;
  G4Material *Material_SiO2;
  G4Material *Material_Al;
  G4Material *Material_Mylar;
  G4Material *Material_Cu;
  G4Material *Material_Fe;
  G4Material *Material_Plastic;
  G4Material *PEEK;
  G4Material* Material_PCB;

  G4VisAttributes *Vis_Vacuum;
  G4VisAttributes *Vis_Si ;
  G4VisAttributes *Vis_SiO2;
  G4VisAttributes *Vis_Al;
  G4VisAttributes *Vis_Mylar;
  G4VisAttributes *Vis_Cu;
  G4VisAttributes *Vis_Fe;
  G4VisAttributes *Vis_Plastic;
  G4VisAttributes *Vis_PEEK;
  G4VisAttributes *Vis_PCB;

  G4String string_pos[9];
  G4double Angle_Correction[9];

  static const G4int nb_det = 40;

   std::string Detector_Name[nb_det] = {
      "D1.1", "D1.2", "D1.3", "D1.4", "D1.5",
      "D2.1", "D2.2", "D2.3", "D2.4", "D2.5",
      "D3.1", "D3.2", "D3.3", "D3.4", "D3.5",
      "D4.1", "D4.2", "D4.3", "D4.4", "D4.5",
      "D5.1", "D5.2", "D5.3", "D5.4", "D5.5",
      "D6.1", "D6.2", "D6.3", "D6.4", "D6.5",
      "D7.1", "D7.2", "D7.3", "D7.4", "D7.5",
      "D8.1", "D8.2", "D8.3", "D8.4", "D8.5"};

  int Detector_Code[nb_det] = {
      11, 12, 13, 14, 15,
      21, 22, 23, 24, 25, 
      31, 32, 33, 34, 35,
      41, 42, 43, 44, 45,
      51, 52, 53, 54, 55,
      61, 62, 63, 64, 65,
      71, 72, 73, 74, 75,
      81, 82, 83, 84, 85};

};


inline std::vector<std::pair<G4LogicalVolume *, G4VPhysicalVolume *>>
Wisard_Detector::Make_Sidet(int num)
{
  std::pair<G4LogicalVolume *, G4VPhysicalVolume *> Support = MakeSupport(num, Vis_PCB, Material_PCB);
  // std::pair<G4LogicalVolume*, G4VPhysicalVolume*> Cooling = MakeCooling(num, dir, Support.first, Vis_Cu, Material_Cu);
  std::pair<G4LogicalVolume *, G4VPhysicalVolume *> Vide = MakeVide(num, Vis_Vacuum, Material_Vacuum);
  std::pair<G4LogicalVolume *, G4VPhysicalVolume *> AlFrame = MakeAlFrame(num, Vide.first, Material_Al);
  std::pair<G4LogicalVolume *, G4VPhysicalVolume *> Strips[21];

  for (int i = 1; i <= 5; i++)
  {
    Strips[i - 1] = MakeStrip(i, num, Vide.first, Vis_Si, Material_Si);
    Strips[i + 4] = MakeDL(i, num, Strips[i - 1].first, Vis_Si, Material_Si);
    Strips[i + 9] = MakeStripGrid(i, num, Vide.first, Vis_Al, Material_Al);
    if (i != 5)
    {
      Strips[i + 14] = MakeInterStrip(i, num, Vide.first, Vis_SiO2, Material_SiO2);
    }

    // Strips[i - 1].first->SetSensitiveDetector(manager_ptr->GetWisardSensor_Detector( ("D"+ to_string(num) + "." + to_string(i))) );
    // Strips[i + 4].first->SetSensitiveDetector(manager_ptr->GetWisardSensor_Detector( ("D"+ to_string(num) + "." + to_string(i) + "dl") ) );

  } 

  // boder inetrstrip
  Strips[19] = MakeInterStrip(-1, 0, Vide.first, Vis_SiO2, Material_SiO2);
  Strips[20] = MakeInterStrip(-4, 0, Vide.first, Vis_SiO2, Material_SiO2);

  return {Support, Vide, Strips[0], Strips[1], Strips[2], Strips[3], Strips[4], Strips[5], Strips[6], Strips[7], Strips[8], Strips[9], Strips[10], Strips[11], Strips[12], Strips[13], Strips[14], Strips[15], Strips[16], Strips[17], Strips[18], AlFrame};
}

inline std::pair<G4LogicalVolume *, G4VPhysicalVolume *> Wisard_Detector::MakeSupport(int num, G4VisAttributes *support_att, G4Material *support_mat)
{
  G4LogicalVolume *logicSupportSiliconDetector = new G4LogicalVolume(
      supportSiliconDetector,
      support_mat,
      ("D" + to_string(num) + "_PCB").data());

  G4RotationMatrix *rotate = new G4RotationMatrix();
  rotate->rotateX(get<0>(dic_rotate["D" + to_string(num)]));
  rotate->rotateY(get<1>(dic_rotate["D" + to_string(num)]));
  rotate->rotateZ(get<2>(dic_rotate["D" + to_string(num)]));

  G4VPhysicalVolume *physSupportSiliconDetector = new G4PVPlacement(
      rotate,
      dic_position["D" + to_string(num)],
      logicSupportSiliconDetector,
      ("D" + to_string(num) + "_PCB").data() ,
      fLogicWorld,
      false,
      0);

  logicSupportSiliconDetector->SetVisAttributes(support_att);

  return std::make_pair(logicSupportSiliconDetector, physSupportSiliconDetector);
}

inline std::pair<G4LogicalVolume *, G4VPhysicalVolume *> Wisard_Detector::MakeVide(int num, G4VisAttributes *vide_att, G4Material *vide_mat)
{
  G4LogicalVolume *logicSupportSiliconDetectorvide = new G4LogicalVolume(
      supportSiliconDetectorvide,
      vide_mat,
      ("D" + to_string(num) + "_Handler").data());

  G4RotationMatrix *rotate = new G4RotationMatrix();
  rotate->rotateX(get<0>(dic_rotate["D" + to_string(num)]));
  rotate->rotateY(get<1>(dic_rotate["D" + to_string(num)]));
  rotate->rotateZ(get<2>(dic_rotate["D" + to_string(num)]));
  
  G4VPhysicalVolume *physSupportSiliconDetectorvide = new G4PVPlacement(
      rotate,
      dic_position["D" + to_string(num)] + dic_positionvide["D" + to_string(num)],
      logicSupportSiliconDetectorvide,
      ("D" + to_string(num) + "_Handler").data(),
      fLogicWorld,
      false,
      0);
  logicSupportSiliconDetectorvide->SetVisAttributes(vide_att);

  return std::make_pair(logicSupportSiliconDetectorvide, physSupportSiliconDetectorvide);
}

inline std::pair<G4LogicalVolume *, G4VPhysicalVolume *> Wisard_Detector::MakeCooling(int num, G4LogicalVolume *mother, G4VisAttributes *cooling_att, G4Material *cooling_mat)
{
  G4LogicalVolume *logic_Box_material_SupportoRame_SiliconDetector = new G4LogicalVolume(
      Box_material_SupportoRame_SiliconDetector,
      cooling_mat,
      ("D" + to_string(num) + "_Cooling").data());

  G4VPhysicalVolume *physics_Box_material_SupportoRame_SiliconDetector = new G4PVPlacement(
      0,
      G4ThreeVector((length_x_SupportoRame_SiDetector / 2 - x_smallBox_daTagliare_SupportoRame_SiDetector - distanza_latoDxBoxTagliata_e_bordoDxSupportoRame_SiDetector - (y_smallBox_daTagliare_SupportoRame_SiDetector / tan(thetaInclinazione_SiDetector) - 1. * mm)), pDy1 / 2 - height_y_SupportoRame_SiDetector / 2, -pDz / 2 - thickness_z_SupportoRame_SiDetector / 2),
      logic_Box_material_SupportoRame_SiliconDetector,
      ("D" + to_string(num) + "_Cooling").data(),
      mother,
      false,
      0);

  logic_Box_material_SupportoRame_SiliconDetector->SetVisAttributes(cooling_att);

  return std::make_pair(logic_Box_material_SupportoRame_SiliconDetector, physics_Box_material_SupportoRame_SiliconDetector);
}

inline std::pair<G4LogicalVolume *, G4VPhysicalVolume *> Wisard_Detector::MakeStrip(int strip, int num, G4LogicalVolume *videe, G4VisAttributes *strip_att, G4Material *strip_mat)
{
  G4LogicalVolume *logicSiDet = new G4LogicalVolume(
      get<0>(dic_strip[strip]),
      strip_mat,
      ("D" + to_string(num) + "." + to_string(strip)).data());

  G4VPhysicalVolume *physSiDet = new G4PVPlacement(
      0,
      get<1>(dic_strip[strip]),
      logicSiDet,
      ("D" + to_string(num) + "." + to_string(strip)).data(),
      videe,
      false,
      num*10+strip);

  logicSiDet->SetVisAttributes(strip_att);

  return std::make_pair(logicSiDet, physSiDet);
}

inline std::pair<G4LogicalVolume *, G4VPhysicalVolume *> Wisard_Detector::MakeDL(int strip, int num, G4LogicalVolume *mother, G4VisAttributes *strip_att, G4Material *strip_mat)
{
  G4LogicalVolume *logicSiDet = new G4LogicalVolume(
      get<2>(dic_strip[strip]),
      strip_mat,
      ("logicSi_" + to_string(num) + "_Strip_" + to_string(strip) + "_dl").data());

  G4VPhysicalVolume *physSiDet = new G4PVPlacement(
      0,
      G4ThreeVector(0., 0., thicknessSiDetector / 2 - thicknessSiDetectorDeadLayer / 2),
      logicSiDet,
      ("D" + to_string(num) + "." + to_string(strip) + "_dl").data(),
      mother,
      false,
      num*100+strip*10);

  logicSiDet->SetVisAttributes(strip_att);
  // logicSiDet->SetUserLimits(myStepLimit);

  return std::make_pair(logicSiDet, physSiDet);
}

inline std::pair<G4LogicalVolume *, G4VPhysicalVolume *> Wisard_Detector::MakeStripGrid(int strip, int num, G4LogicalVolume *videe, G4VisAttributes *grid_att, G4Material *grid_mat)
{

  G4ThreeVector position = get<1>(dic_strip[strip]);
  position.setZ(position.z() + thicknessSiDetector / 2 + thicknessSiDetectorGrid / 2);

  G4LogicalVolume *logicSiDet = new G4LogicalVolume(
      get<3>(dic_strip[strip]),
      grid_mat,
      ("D" + to_string(num) + "." + to_string(strip) + "_Grid").data());

  G4VPhysicalVolume *physSiDet = new G4PVPlacement(
      0,
      position,
      logicSiDet,
      ("D" + to_string(num) + "." + to_string(strip) + "_Grid").data(),
      videe,
      false,
      0);

  logicSiDet->SetVisAttributes(grid_att);
  // logicSiDet->SetUserLimits(myStepLimit);

  return std::make_pair(logicSiDet, physSiDet);
}

inline std::pair<G4LogicalVolume *, G4VPhysicalVolume *> Wisard_Detector::MakeAlFrame(int num, G4LogicalVolume *mother, G4Material *frame_mat)
{
  G4LogicalVolume *logicSiDet = new G4LogicalVolume(
      AlFrameSiliconDetector,
      frame_mat,
      ("D" + to_string(num) + "_AlFrame").data());

  G4VPhysicalVolume *physSiDet = new G4PVPlacement(
      0,
      G4ThreeVector(0., 0., 0),
      logicSiDet,
      ("D" + to_string(num) + "_AlFrame").data(),
      mother,
      false,
      0);

  logicSiDet->SetVisAttributes(Vis_Al);

  return std::make_pair(logicSiDet, physSiDet);
}

inline std::pair<G4LogicalVolume *, G4VPhysicalVolume *> Wisard_Detector::MakeInterStrip(int strip, int num, G4LogicalVolume *videe, G4VisAttributes *strip_att, G4Material *interstripMat)
{
  G4ThreeVector position = get<1>(dic_interstrip[strip*10+strip+1]);
  position.setZ(position.z() + thicknessSiDetector / 2);

  G4LogicalVolume *logicSiDet = new G4LogicalVolume(
      get<0>(dic_interstrip[strip*10+strip+1]),
      interstripMat, // SiO2
      ("D" + to_string(num) + "." + to_string(strip) + to_string(strip+1) + "_InterStrip" ).data());

  G4VPhysicalVolume *physSiDet = new G4PVPlacement(
      0,
      position,
      logicSiDet,
      ("D" + to_string(num) + "." + to_string(strip) + to_string(strip+1) + "_InterStrip" ).data(),
      videe,
      false,
      num*1000 + (2*strip+1)*100/2);

  logicSiDet->SetVisAttributes(strip_att);

  return std::make_pair(logicSiDet, physSiDet);
}

inline G4ThreeVector Wisard_Detector::ConvertStringToG4ThreeVector(G4String str)
{
  if (str == "")
  {
    return G4ThreeVector(0, 0, 0);
  }
  std::istringstream iss(str);
  G4double x0, y0, z0;
  if (iss >> x0 >> y0 >> z0)
  {
    return G4ThreeVector(x0, y0, z0);
  }
  else
  {
    G4Exception("Wisard_Detector::ConvertStringToG4ThreeVector", "Impossible to convert string to G4ThreeVector", JustWarning, "");
    return G4ThreeVector(0, 0, 0);
  }
}

inline G4ThreeVector Wisard_Detector::Cylindrical_Convertion(G4String detname, G4ThreeVector Init)
{
  
  if (detname == "D1")
  {
    return G4ThreeVector(Init.x(), Init.y(), Init.z());
  }
  else if (detname == "D2")
  {
    return G4ThreeVector(Init.y(), -Init.x(), Init.z());
  }
  else if (detname == "D3")
  {
    return G4ThreeVector(-Init.x(), -Init.y(), Init.z());
  }
  else if (detname == "D4")
  {
    return G4ThreeVector(-Init.y(), Init.x(), Init.z());
  }
  else if (detname == "D5")
  {
    return G4ThreeVector(-Init.x(), Init.y(), -Init.z());
  }
  else if (detname == "D6")
  {
    return G4ThreeVector(Init.y(), Init.x(), -Init.z());
  }
  else if (detname == "D7")
  {
    return G4ThreeVector(Init.x(), -Init.y(), -Init.z());
  }
  else if (detname == "D8")
  {
    return G4ThreeVector(-Init.y(), -Init.x(), -Init.z());
  }
  else
  {
    G4Exception("Wisard_Detector::Cylindrical_Convertion", "Impossible to find the Dtector Name", JustWarning, "");
    return G4ThreeVector(0, 0, 0);
  }
}

inline void Wisard_Detector::CylindricalAngle_Convertion(G4String detname, G4double angle)
{
  // std::tuple<G4double,G4double,G4double> rotate = dic_rotate[detname];
  if (detname == "D1")
  {
    get<0>(dic_rotate[detname]) += angle;
    return; 
  }
  else if (detname == "D2")
  {
    get<1>(dic_rotate[detname]) -= angle;
    return; 
  }
  else if (detname == "D3")
  {
    get<0>(dic_rotate[detname]) -= angle;
    return; 
  }
  else if (detname == "D4")
  {
    get<1>(dic_rotate[detname]) += angle;
    return; 
  }
  else if (detname == "D5")
  {
    get<0>(dic_rotate[detname]) -= angle;
    return; 
  }
  else if (detname == "D6")
  {
    get<1>(dic_rotate[detname]) -= angle;
    return; 
  }
  else if (detname == "D7")
  {
    get<0>(dic_rotate[detname]) += angle;
    return; 
  }
  else if (detname == "D8")
  {
    get<1>(dic_rotate[detname]) += angle;
    return; 
  }
  else
  {
    G4Exception("Wisard_Detector::CylindricalAngle_Convertion", "Impossible to find the Dtector Name", JustWarning, "");
    return; 
  }
}

#endif
