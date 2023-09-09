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
#include "G4VSolid.hh"

#include "G4Trap.hh"
#include "G4ThreeVector.hh"

#include <unordered_map>
#include <tuple>
#include <string>
#include <iostream>

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

  

    G4Tubs* fSolidWorld;
    G4LogicalVolume* fLogicWorld;
    G4VPhysicalVolume* fPhysiWorld;
    G4Material* fDefaultMaterial;

    G4double fLength_PlasticScintillator = 5*cm;
    G4double fRadius_PlasticScintillator = 1.5*cm;

    G4bool event;
    void               SetMagField();

    
    std::tuple<std::pair<G4LogicalVolume*, G4VPhysicalVolume*>,
                  std::pair<G4LogicalVolume*, G4VPhysicalVolume*>,
                  std::pair<G4LogicalVolume*, G4VPhysicalVolume*>,  
                  std::pair<G4LogicalVolume*, G4VPhysicalVolume*>,
                  std::pair<G4LogicalVolume*, G4VPhysicalVolume*>,
                  std::pair<G4LogicalVolume*, G4VPhysicalVolume*>,
                  std::pair<G4LogicalVolume*, G4VPhysicalVolume*>,
                  std::pair<G4LogicalVolume*, G4VPhysicalVolume*>,
                  std::pair<G4LogicalVolume*, G4VPhysicalVolume*>,
                  std::pair<G4LogicalVolume*, G4VPhysicalVolume*>,
                  std::pair<G4LogicalVolume*, G4VPhysicalVolume*>,
                  std::pair<G4LogicalVolume*, G4VPhysicalVolume*>,
                  std::pair<G4LogicalVolume*, G4VPhysicalVolume*>> Make_Sidet(int num, string dir, G4VisAttributes* strip_att, G4VisAttributes* cooling_att, G4VisAttributes* support_att, G4VisAttributes* vide_att, G4Material* strip_mat, G4Material* cooling_mat, G4Material*support_mat, G4Material* vide_mat);
    std::pair<G4LogicalVolume*, G4VPhysicalVolume*> MakeSupport(int num, string dir, G4VisAttributes* support_att, G4Material* support_mat);
    std::pair<G4LogicalVolume*, G4VPhysicalVolume*> MakeCooling(int num, string dir, G4LogicalVolume* mother, G4VisAttributes* cooling_att, G4Material* cooling_mat);

    std::pair<G4LogicalVolume*, G4VPhysicalVolume*> MakeVide(int num, string dir, G4VisAttributes* vide_att, G4Material* vide_mat);
    std::pair<G4LogicalVolume*, G4VPhysicalVolume*> MakeStrip(int strip, int num, string dir, G4LogicalVolume* mother, G4VisAttributes* strip_att, G4Material* strip_mat);
    std::pair<G4LogicalVolume*, G4VPhysicalVolume*> MakeDL(int strip, int num, string dir, G4LogicalVolume* mother, G4VisAttributes* strip_att, G4Material* strip_mat);


    std::unordered_map<int,  std::tuple<G4Trap*, G4ThreeVector, G4Trap*>> dic_strip;
    std::unordered_map<std::string, G4ThreeVector> dic_position;
    std::unordered_map<std::string, G4ThreeVector> dic_positionvide;
    std::unordered_map<std::string, std::tuple<double, double, double>> dic_rotate;

    G4VSolid* supportSiliconDetectorvide;
    G4Trap* SiDet_Strip_1;
    G4Trap* SiDet_Strip_2;
    G4Trap* SiDet_Strip_3;
    G4Trap* SiDet_Strip_4;
    G4Trap* SiDet_Strip_5;
    G4Trap* SiDet_Strip_1_dl;
    G4Trap* SiDet_Strip_2_dl;
    G4Trap* SiDet_Strip_3_dl;
    G4Trap* SiDet_Strip_4_dl;
    G4Trap* SiDet_Strip_5_dl;
    double r, z, r_vide, z_vide, theta;
    G4VSolid* Box_material_SupportoRame_SiliconDetector;
    G4double spazio_tra_Bordo_e_strip5;
    G4double spazio_tra_Strip;
    G4double thicknessSiDetector, thicknessSiDetectorDeadLayer, length_x_SupportoRame_SiDetector, x_smallBox_daTagliare_SupportoRame_SiDetector, distanza_latoDxBoxTagliata_e_bordoDxSupportoRame_SiDetector, y_smallBox_daTagliare_SupportoRame_SiDetector, thetaInclinazione_SiDetector, pDy1, height_y_SupportoRame_SiDetector, pDz, thickness_z_SupportoRame_SiDetector; 
    G4VSolid* supportSiliconDetector;
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

};

inline std::tuple<std::pair<G4LogicalVolume*, G4VPhysicalVolume*>,
                  std::pair<G4LogicalVolume*, G4VPhysicalVolume*>,
                  std::pair<G4LogicalVolume*, G4VPhysicalVolume*>,  
                  std::pair<G4LogicalVolume*, G4VPhysicalVolume*>,
                  std::pair<G4LogicalVolume*, G4VPhysicalVolume*>,
                  std::pair<G4LogicalVolume*, G4VPhysicalVolume*>,
                  std::pair<G4LogicalVolume*, G4VPhysicalVolume*>,
                  std::pair<G4LogicalVolume*, G4VPhysicalVolume*>,
                  std::pair<G4LogicalVolume*, G4VPhysicalVolume*>,
                  std::pair<G4LogicalVolume*, G4VPhysicalVolume*>,
                  std::pair<G4LogicalVolume*, G4VPhysicalVolume*>,
                  std::pair<G4LogicalVolume*, G4VPhysicalVolume*>,
                  std::pair<G4LogicalVolume*, G4VPhysicalVolume*>>
   Wisard_Detector::Make_Sidet(int num, string dir, G4VisAttributes* strip_att, G4VisAttributes* cooling_att, G4VisAttributes* support_att, G4VisAttributes* vide_att, G4Material* strip_mat, G4Material* cooling_mat, G4Material*support_mat, G4Material* vide_mat)
{
  std::pair<G4LogicalVolume*, G4VPhysicalVolume*> Support = MakeSupport(num, dir, support_att, support_mat);
  std::pair<G4LogicalVolume*, G4VPhysicalVolume*> Cooling = MakeCooling(num, dir, Support.first, cooling_att, cooling_mat);
  std::pair<G4LogicalVolume*, G4VPhysicalVolume*> Vide = MakeVide(num, dir, vide_att, vide_mat);
  std::pair<G4LogicalVolume*, G4VPhysicalVolume*> Strips[10];
  
  for (int i = 1; i < 6; i++)
  {
    cout<<i<<endl;
    Strips[i-1] = MakeStrip(i, num, dir, Vide.first, strip_att, strip_mat);
    Strips[i+4] = MakeDL(i, num, dir, Strips[i-1].first, vide_att, strip_mat);

    Strips[i-1].first->SetSensitiveDetector( manager_ptr->GetWisardSensor_Detector((num+dir+"_Strip_"+i).Data()) );
    Strips[i+4].first->SetSensitiveDetector( manager_ptr->GetWisardSensor_Detector((num+dir+"_Strip_"+i+"_dl").Data()) );
  } 

  return std::make_tuple(Support, Cooling, Vide, Strips[0], Strips[1], Strips[2], Strips[3], Strips[4], Strips[5], Strips[6], Strips[7], Strips[8], Strips[9]);
}


inline std::pair<G4LogicalVolume*, G4VPhysicalVolume*> Wisard_Detector::MakeSupport(int num, string dir, G4VisAttributes* support_att, G4Material* support_mat)
{
  G4LogicalVolume* logicSupportSiliconDetector = new G4LogicalVolume( 
      supportSiliconDetector,
      support_mat,
      ("logicSupportSiliconDetector_"+num+dir).data());

  G4RotationMatrix* rotate = new G4RotationMatrix();
  rotate->rotateX(get<0>(dic_rotate[(num+dir).Data()]));
  rotate->rotateY(get<1>(dic_rotate[(num+dir).Data()]));
  rotate->rotateZ(get<2>(dic_rotate[(num+dir).Data()]));

  G4VPhysicalVolume* physSupportSiliconDetector = new G4PVPlacement(
      rotate,
      dic_position[(num+dir).Data()],
      logicSupportSiliconDetector,
      ("physSupportSiliconDetector_"+std::to_string(num)+dir).data(),
      fLogicWorld,
      false,
      0);  

  logicSupportSiliconDetector->SetVisAttributes(support_att);

  return std::make_pair(logicSupportSiliconDetector, physSupportSiliconDetector);
}

inline std::pair<G4LogicalVolume*, G4VPhysicalVolume*> Wisard_Detector::MakeVide(int num, string dir, G4VisAttributes* vide_att, G4Material* vide_mat)
{
  G4LogicalVolume* logicSupportSiliconDetectorvide = new G4LogicalVolume( 
      supportSiliconDetectorvide,
      vide_mat,
      ("logicSupportSiliconDetector_"+num+dir+"vide").data());

  G4RotationMatrix* rotate = new G4RotationMatrix();
  rotate->rotateX(get<0>(dic_rotate[(num+dir).Data()]));
  rotate->rotateY(get<1>(dic_rotate[(num+dir).Data()]));
  rotate->rotateZ(get<2>(dic_rotate[(num+dir).Data()]));

  G4VPhysicalVolume* physSupportSiliconDetectorvide = new G4PVPlacement(
      rotate,
      dic_positionvide[(num+dir).Data()],
      logicSupportSiliconDetectorvide,
      ("physSupportSiliconDetector_"+num+dir+"vide").data(),
      fLogicWorld,
      false,
      0);  
  logicSupportSiliconDetectorvide->SetVisAttributes(vide_att);

  return std::make_pair(logicSupportSiliconDetectorvide, physSupportSiliconDetectorvide);
}

inline std::pair<G4LogicalVolume*, G4VPhysicalVolume*> Wisard_Detector::MakeCooling(int num, string dir, G4LogicalVolume* mother, G4VisAttributes* cooling_att, G4Material* cooling_mat)
{
  G4LogicalVolume* logic_Box_material_SupportoRame_SiliconDetector = new G4LogicalVolume( 
      Box_material_SupportoRame_SiliconDetector,
      cooling_mat,
      ("logic_Box_material_SupportoRame_SiliconDetector_"+num+dir).data());

  G4VPhysicalVolume* physics_Box_material_SupportoRame_SiliconDetector = new G4PVPlacement(
      0,
      G4ThreeVector((length_x_SupportoRame_SiDetector/2 - x_smallBox_daTagliare_SupportoRame_SiDetector - distanza_latoDxBoxTagliata_e_bordoDxSupportoRame_SiDetector - (y_smallBox_daTagliare_SupportoRame_SiDetector/tan(thetaInclinazione_SiDetector) -1.*mm)), pDy1/2 - height_y_SupportoRame_SiDetector/2, -pDz/2 -thickness_z_SupportoRame_SiDetector/2),
      logic_Box_material_SupportoRame_SiliconDetector,
      ("phys_Box_material_SupportoRame_SiliconDetector_"+num+dir).data(),
      mother,
      false,
      0);  

  logic_Box_material_SupportoRame_SiliconDetector->SetVisAttributes(cooling_att);

  return std::make_pair(logic_Box_material_SupportoRame_SiliconDetector, physics_Box_material_SupportoRame_SiliconDetector);
}

inline std::pair<G4LogicalVolume*, G4VPhysicalVolume*> Wisard_Detector::MakeStrip(int strip, int num, string dir, G4LogicalVolume* vide, G4VisAttributes* strip_att, G4Material* strip_mat)
{
  G4LogicalVolume* logicSiDet = new G4LogicalVolume(
      get<0>(dic_strip[strip]),
      strip_mat,
      ("logicSi_"+num+dir+"_Det_Strip_"+strip).Data());
  
  G4VPhysicalVolume* physSiDet = new G4PVPlacement(
      0,
      get<1>(dic_strip[strip]),
      logicSiDet,
      ("physSi_"+num+dir+"_Det_Strip_"+strip).Data(),
      vide,
      false,
      0);

  logicSiDet->SetVisAttributes(strip_att);

  return std::make_pair(logicSiDet, physSiDet);
}

inline std::pair<G4LogicalVolume*, G4VPhysicalVolume*> Wisard_Detector::MakeDL(int strip, int num, string dir, G4LogicalVolume* mother, G4VisAttributes* strip_att, G4Material* strip_mat)
{
  G4LogicalVolume* logicSiDet = new G4LogicalVolume(
      get<2>(dic_strip[strip]),
      strip_mat,
      ("logicSiDet_"+num+dir+"_Strip_"+strip+"_dl").Data());

  G4VPhysicalVolume* physSiDet = new G4PVPlacement(
      0,
      G4ThreeVector(0., 0., thicknessSiDetector/2 - thicknessSiDetectorDeadLayer/2),
      logicSiDet,
      ("physSiDet_"+num+dir+"_Strip_"+strip+"_dl").Data(),
      mother,
      false,
      0);

  logicSiDet->SetVisAttributes(strip_att);

  return std::make_pair(logicSiDet, physSiDet);
}



#endif
