#ifndef WISARD_RUN_ACTION_HH
#define WISARD_RUN_ACTION_HH

#include "G4UserRunAction.hh"
#include "ParticleInformation.hh"
#include "G4GenericMessenger.hh"
#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include "TObjString.h"
#include "TTreeReader.h"
#include "TDirectory.h"
#include "G4UImanager.hh"

#include "Wisard_Detector.hh"

class Wisard_RunAction : public G4UserRunAction
{
public:
  Wisard_RunAction(G4String);
  virtual ~Wisard_RunAction();

  void BeginOfRunAction(const G4Run *run) override;
  void EndOfRunAction(const G4Run *run) override;

  G4String GetFileName();
  G4String MacroFileName;

  G4GenericMessenger *RunMessenger;
  G4double threshold;
  G4String filename;
  G4String dummy;

  TFile *f;
  TTree *Tree;

  TH1D *silicon_coinc[Wisard_Detector::nb_det];
  TH1D *silicon_nocoinc[Wisard_Detector::nb_det];
  TH1D *silicon_single[Wisard_Detector::nb_det];
  TH1D *plastic_coinc;
  map<G4int, TH1D*> H_E0 = {};

  /// TREE VARIABLES ////
  vector<G4int> Particle_PDG;
  vector<G4double> x;
  vector<G4double> y;
  vector<G4double> z;
  vector<G4double> px;
  vector<G4double> py;
  vector<G4double> pz;
  vector<G4double> Kinetic_Energy;
  vector<G4double> T0;

  // catcher //
  vector<G4double> Catcher_Central_Energy_Deposit, Catcher_Side_Energy_Deposit;

  // plastic scintillator //
  vector<G4double> PlasticScintillator_Hit_Angle, PlasticScintillator_Energy_Deposit, PlasticScintillator_Hit_Time;
  vector<G4ThreeVector> PlasticScintillator_Hit_Position;

  // silicon detectors //
  vector<vector<G4ThreeVector>> Silicon_Detector_Hit_Position;
  vector<vector<G4double>> Silicon_Detector_Hit_Angle, Silicon_Detector_Energy_Deposit, Silicon_Detector_DL_Energy_Deposit, Silicon_Detector_Hit_Time;
  vector<vector<G4int>> Silicon_Detector_Code;
  ///////////////////////

  vector<G4double> Silicon_Detector_Energy_Deposit_part, Silicon_Detector_DL_Energy_Deposit_part, Silicon_Detector_Hit_Angle_part, Silicon_Detector_Hit_Time_part;
  vector<G4ThreeVector> Silicon_Detector_Hit_Position_part;
  vector<G4int> Silicon_Detector_Code_part;

  void UpdateTree(ParticleInformation *Part_Info);
  void WrittingTree();


  std::string Detector_Name[Wisard_Detector::nb_det] = {
      "D1.1", "D1.2", "D1.3", "D1.4", "D1.5",
      "D2.1", "D2.2", "D2.3", "D2.4", "D2.5",
      "D3.1", "D3.2", "D3.3", "D3.4", "D3.5",
      "D4.1", "D4.2", "D4.3", "D4.4", "D4.5",
      "D5.1", "D5.2", "D5.3", "D5.4", "D5.5",
      "D6.1", "D6.2", "D6.3", "D6.4", "D6.5",
      "D7.1", "D7.2", "D7.3", "D7.4", "D7.5",
      "D8.1", "D8.2", "D8.3", "D8.4", "D8.5"};



  int Detector_Code[Wisard_Detector::nb_det] = {
      11, 12, 13, 14, 15,
      21, 22, 23, 24, 25, 
      31, 32, 33, 34, 35,
      41, 42, 43, 44, 45,
      51, 52, 53, 54, 55,
      61, 62, 63, 64, 65,
      71, 72, 73, 74, 75,
      81, 82, 83, 84, 85};
};

#endif // WISARD_RUN_ACTION_HH