#ifndef WITCH_RUNMANAGER_HH
#define WITCH_RUNMANAGER_HH

#include "Wisard_Global.hh"
#include "G4RunManager.hh"
#include "Wisard_Sensor.hh"
#include "G4AnalysisManager.hh"

#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include "TTreeReader.h"
#include <TDirectory.h>

#include <unordered_map>
#include <utility>
#include <vector>

class Wisard_RunManager : public G4RunManager
{

protected:
  Wisard_Sensor *wisard_sensor_PlasticScintillator;
  Wisard_Sensor *wisard_sensor_CatcherMylar_central;
  Wisard_Sensor *wisard_sensor_CatcherAl1_central;
  Wisard_Sensor *wisard_sensor_CatcherAl2_central;
  Wisard_Sensor *wisard_sensor_CatcherMylar_side;
  Wisard_Sensor *wisard_sensor_CatcherAl1_side;
  Wisard_Sensor *wisard_sensor_CatcherAl2_side;

  /// TREE VARIABLES ////
  vector<G4int> Particle_PDG;
  vector<G4double> x;
  vector<G4double> y; 
  vector<G4double> z;
  vector<G4double> px; 
  vector<G4double> py; 
  vector<G4double> pz;
  vector<G4double> Kinetic_Energy;
  
  // catcher //
  vector<G4double> Catcher_Central_Energy_Deposit, Catcher_Side_Energy_Deposit;

  // plastic scientillator //
  vector<G4double> PlasticScintillator_Hit_Angle, PlasticScintillator_Energy_Deposit;
  vector<G4ThreeVector> PlasticScintillator_Hit_Position;

  // silicon detectors //
  vector<vector<G4ThreeVector>> Silicon_Detector_Hit_Position;
  vector<vector<G4double>> Silicon_Detector_Hit_Angle, Silicon_Detector_Energy_Deposit, Silicon_Detector_DL_Energy_Deposit;
  vector<vector<G4int>> Silicon_Detector_Code;

  vector<vector<vector<G4ThreeVector>>> Silicon_Detector_InterStrip_Hit_Position;
  vector<vector<vector<G4double>>> Silicon_Detector_InterStrip_Energy_Deposit;

  
  ///////////////////////

  G4int count = 0;

  ifstream input_txt;
  string input_name;

  ifstream inputSRIM;
  string input_nameSRIM;

  G4double threshold;
  G4String filename;

public:
  // constructor and destructor
  Wisard_RunManager();
  ~Wisard_RunManager();

  static const int nb_det = 40;

  TFile *f;

  TTree *Tree;

  TH1D *silicon_coinc[nb_det];
  TH1D *silicon_nocoinc[nb_det];
  TH1D *silicon_single[nb_det];
  TH1D *plastic_coinc;

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
  
  int InterDetector_Code[32] = { 
      1150, 1250, 1350, 1450,
      2150, 2250, 2350, 2450,
      3150, 3250, 3350, 3450,
      4150, 4250, 4350, 4450,
      5150, 5250, 5350, 5450,
      6150, 6250, 6350, 6450,
      7150, 7250, 7350, 7450,
      8150, 8250, 8350, 8450};

      
  std::unordered_map<int, std::pair<Wisard_Sensor *, Wisard_Sensor *>> dic_detector;
  std::unordered_map<int, Wisard_Sensor *> dic_interdetector;

  Wisard_Sensor *GetWisardSensor_PlasticScintillator();
  Wisard_Sensor *GetWisardSensor_Detector(string name);
  Wisard_Sensor *GetWisardSensor_CatcherMylar_central();
  Wisard_Sensor *GetWisardSensor_CatcherAl1_central();
  Wisard_Sensor *GetWisardSensor_CatcherAl2_central();
  Wisard_Sensor *GetWisardSensor_CatcherMylar_side();
  Wisard_Sensor *GetWisardSensor_CatcherAl1_side();
  Wisard_Sensor *GetWisardSensor_CatcherAl2_side();

  //----------------------------------------------------------
  // Commands definitions
  ////--------------------------------------------------
  //// U.I. commands creation function
  // void  DefineSimulationCommands ( );     // inline
  //----------------------------------------------------------
  // Functions for input file
  ////--------------------------------------------------
  //// added input file functions
  ////                  declaration
  int OpenInput(const string &fname);
  void CloseInput();                  // inline
  ifstream &GetInput_TXT();               // inline
  const string &GetInputName() const; // inline

  int OpenInputSRIM(const string &fname);
  void CloseInputSRIM();    // inline
  ifstream &GetInputSRIM(); // inline

  void SetThreshold(G4double th);
  G4double GetThreshold();

  void SetOutputFilename(G4String fn);
  G4String GetOutputFilename();

  //----------------------------------------------------------
  // Functions for events processing and output histogram

  // redefine the function from base class G4RunManager
  void AnalyzeEvent(G4Event *event);
};

//----------------------------------------------------------------------
// inline functions for the class

inline Wisard_Sensor *Wisard_RunManager::GetWisardSensor_Detector(string name)
{
  int code = stoi ( name.substr(1, 1) ) * 10 + stoi( name.substr(3, 4)) ;

  if (name.substr(name.length() - 2) == "dl")
  {
    if (dic_detector.find( code ) != dic_detector.end())
    {
      return (dic_detector[ code ].second);
    }
    else
    {
      G4Exception("GetWisard_Sensor_GetDetector", ("Detector Name not found : " + name).c_str(), JustWarning, "");
      return nullptr;
    }
  }

  else if (name.length() == 4)
  {
    if (dic_detector.find( code ) != dic_detector.end())
    {
      return (dic_detector[ code ].first);
    }
    else
    {
      G4Exception("GetWisard_Sensor_GetDetector", ("Detector Name not found : " + name).c_str(), JustWarning, "");
      return nullptr;
    }
  }

  else 
  {
    code = stoi ( name.substr(1, 1) ) * 1000 + (stoi( name.substr(3, 1) ) + stoi( name.substr(4, 1) )) * 100 / 2;
    if (dic_interdetector.find( code ) != dic_interdetector.end())
    {
      return (dic_interdetector[ code ]);
    }
    else
    {
      G4Exception("GetWisard_Sensor_GetDetector", ("Detector Name not found : " + name).c_str(), JustWarning, "");
      return nullptr;
    }
  }
}

inline Wisard_Sensor *Wisard_RunManager::GetWisardSensor_PlasticScintillator() { return (wisard_sensor_PlasticScintillator); }
inline Wisard_Sensor *Wisard_RunManager::GetWisardSensor_CatcherMylar_central() { return (wisard_sensor_CatcherMylar_central); }
inline Wisard_Sensor *Wisard_RunManager::GetWisardSensor_CatcherAl1_central() { return (wisard_sensor_CatcherAl1_central); }
inline Wisard_Sensor *Wisard_RunManager::GetWisardSensor_CatcherAl2_central() { return (wisard_sensor_CatcherAl2_central); }
inline Wisard_Sensor *Wisard_RunManager::GetWisardSensor_CatcherMylar_side() { return (wisard_sensor_CatcherMylar_side); }
inline Wisard_Sensor *Wisard_RunManager::GetWisardSensor_CatcherAl1_side() { return (wisard_sensor_CatcherAl1_side); }
inline Wisard_Sensor *Wisard_RunManager::GetWisardSensor_CatcherAl2_side() { return (wisard_sensor_CatcherAl2_side); }

// Close the input file
inline void Wisard_RunManager::CloseInput()
{
  input_txt.close();
  input_name = "";
}

// Get the input file stream information
inline ifstream &Wisard_RunManager::GetInput_TXT()
{
  return (input_txt);
}

// Get the input file name
inline const string &Wisard_RunManager::GetInputName() const
{
  return (input_name);
}

// SRIM
inline void Wisard_RunManager::CloseInputSRIM()
{
  inputSRIM.close();
  input_nameSRIM = "";
}
inline ifstream &Wisard_RunManager::GetInputSRIM()
{
  return (inputSRIM);
}

// THRESHOOLD
inline void Wisard_RunManager::SetThreshold(G4double th) { threshold = th; }
inline G4double Wisard_RunManager::GetThreshold() { return (threshold); }

// OUPUT FILE
inline void Wisard_RunManager::SetOutputFilename(G4String fn) { filename = fn; }
inline G4String Wisard_RunManager::GetOutputFilename() { return filename; }
#endif
